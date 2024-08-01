#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif
// DI pin of LED ring
#define PIN 11
// number of pixels in the ring
#define NUMPIXELS 16
// input pin for connecting uECG
int in_pin = 3;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
    pixels.begin();          // This initializes the NeoPixel library.
    pinMode(in_pin, INPUT);  // set pin to input mode
    digitalWrite(in_pin, 1); // enable PULLUP: this is critical, uECG doesn't have internal pull-up
}
// we store last 20 heartbeats to averge BPM over them
// with higher value, it will become more reliable,
// but it will take more time to see output change when BPM changes
#define BEAT_HIST 20
long beats[BEAT_HIST];
void push_beat(long ms) // shift all beats in array and insert current one
{
    for (int x = 0; x < BEAT_HIST - 1; x++)
    {
        beats[x] = beats[x + 1];
    }
    beats[BEAT_HIST - 1] = ms;
}
int get_bpm() // using time difference between first and last beats
{
    long dt = beats[BEAT_HIST - 1] - beats[0];
    long bpm = BEAT_HIST * 60000 / dt;
    return bpm;
}
long last_pix_upd = 0; // to keep track of when we updated pixels previous time
int prev_in_state = 0; // previous state of input pin: we want to process only changes of state
void loop()
{
    long ms = millis();
    int in_state = digitalRead(in_pin);      // 1 when no beat detected, 0 in beat
    if (in_state == 1 && prev_in_state == 0) // react only to change
    {
        push_beat(ms);
    }
    prev_in_state = in_state;
    if (ms - last_pix_upd > 10) // don't update pixels too often
    {
        int r, g, b;
        last_pix_upd = ms;
        int bpm = get_bpm();
        int max_bright = 120;      // value of maximum brightness, max 255. But you don't always want it at max :)
        float dd = 20;             // change in BPM between color tones (blue->green->yellow->pink->red)
        float t1 = 90, t2, t3, t4; // t1 - "base" BPM, lower than t1 would be blue
        t2 = t1 + dd;
        t3 = t2 + dd;
        t4 = t3 + dd;
        // code for changing color depending in which t1...t4 range we are now
        if (bpm < t1)
        {
            r = 0;
            g = 0;
            b = max_bright;
        }
        else if (bpm < t2)
        {
            r = 0;
            g = max_bright * (bpm - t1) / dd;
            b = max_bright - g;
        }
        else if (bpm < t3)
        {
            r = max_bright * (bpm - t2) / dd;
            g = max_bright - r;
            b = r / 4;
        }
        else if (bpm < t4)
        {
            r = max_bright;
            g = 0;
            b = max_bright / 2 - max_bright * (bpm - t3) / (2 * dd);
        }
        else
        {
            r = max_bright;
            g = 0;
            b = 0;
        }
        if (in_state) // when not in beat, 1/4 intensity, so only beats are highlighted
        {
            r *= 0.25;
            g *= 0.25;
            b *= 0.25;
        }
        int on_pixels = (bpm + 5) / 10; // number of used LEDs: for 60 BPM, 6 LEDs will be on, for 120 - 12 etc
        for (int i = 0; i < NUMPIXELS; i++)
        {
            if (i < on_pixels)
                pixels.setPixelColor(i, pixels.Color(r, g, b));
            else
                pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // turn off all other LEDs
        }
        pixels.show();
    }
}