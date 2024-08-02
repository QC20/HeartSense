// WCMUCU-2812B-12 LED ring is compatible with the Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>

#define LED_PIN     6
#define NUM_LEDS    12  // Adjust this number based on how many LEDs are on your module

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(50); // Set brightness to about 1/5 (max = 255)
}

void loop() {
  heartbeat(strip.Color(255, 0, 0), 2); // Red color, 2 beats
}

void heartbeat(uint32_t color, int repetitions) {
  for (int i = 0; i < repetitions; i++) {
    // First beat
    fadeToColor(color, 20);
    fadeToBlack(20);
    
    // Short pause between beats
    delay(200);
    
    // Second beat (usually softer)
    fadeToColor(color, 40);
    fadeToBlack(40);
    
    // Longer pause until next heartbeat
    delay(600);
  }
}

void fadeToColor(uint32_t color, int steps) {
  for (int i = 0; i <= steps; i++) {
    for (int j = 0; j < strip.numPixels(); j++) {
      strip.setPixelColor(j, 
        ((red(color) * i) / steps),
        ((green(color) * i) / steps),
        ((blue(color) * i) / steps)
      );
    }
    strip.show();
    delay(5);
  }
}

void fadeToBlack(int steps) {
  for (int i = steps; i >= 0; i--) {
    for (int j = 0; j < strip.numPixels(); j++) {
      strip.setPixelColor(j, 
        ((strip.getPixelColor(j) >> 16 & 0xFF) * i) / steps,
        ((strip.getPixelColor(j) >> 8 & 0xFF) * i) / steps,
        ((strip.getPixelColor(j) & 0xFF) * i) / steps
      );
    }
    strip.show();
    delay(5);
  }
}

// Helper functions to extract RGB from uint32_t color
uint8_t red(uint32_t color) {
  return (color >> 16) & 0xFF;
}

uint8_t green(uint32_t color) {
  return (color >> 8) & 0xFF;
}

uint8_t blue(uint32_t color) {
  return color & 0xFF;
}