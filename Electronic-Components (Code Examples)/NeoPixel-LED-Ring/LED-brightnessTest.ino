#include <Adafruit_NeoPixel.h>

#define LED_PIN    6    // Data pin connected to the LED strip
#define LED_COUNT  12   // Number of LEDs in your strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();           // Initialize NeoPixel strip object
  strip.show();            // Turn OFF all pixels
}

void loop() {
  /*
  // High (bright) red with changing brightness
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    strip.setBrightness(brightness);
    setAllPixels(255, 0, 0);
    delay(50);
  }
  delay(1000);

  // Middle red with changing brightness
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    strip.setBrightness(brightness);
    setAllPixels(128, 0, 0);
    delay(50);
  }
  delay(1000);

  // Low (dark) red with changing brightness
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    strip.setBrightness(brightness);
    setAllPixels(64, 0, 0);
    delay(50);
  }
  */
    // Extra super non-bright red
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    strip.setBrightness(brightness);
    setAllPixels(10, 0, 0);
    delay(1000);
  }

  delay(1000);
}

void setAllPixels(uint8_t red, uint8_t green, uint8_t blue) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}