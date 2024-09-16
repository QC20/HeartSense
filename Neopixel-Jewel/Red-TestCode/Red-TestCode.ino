#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIXELS 7

// Use NEO_GRBW for RGBW NeoPixels
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.setBrightness(50);
}

void loop() {
  // Set all pixels to red with no white component
  pixels.fill(pixels.Color(255, 0, 0, 0)); // Red, no white
  pixels.show();
  delay(1000);
}
