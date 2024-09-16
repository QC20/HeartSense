#include "PulseWaveVisualizer.h"

#define NEOPIXEL_PIN 6
#define NUMPIXELS 16  // Adjust based on your LED ring

PulseWaveVisualizer pulseVisualizer(NEOPIXEL_PIN, NUMPIXELS);

// Simulated heart rate for demonstration
float currentHR = 60;
float hrChangeDirection = 1;

void setup() {
  Serial.begin(115200);
  pulseVisualizer.begin();
}

void loop() {
  // Simulate changing heart rate for demonstration
  currentHR += hrChangeDirection * random(0, 5) * 0.1;
  if (currentHR > 120 || currentHR < 50) {
    hrChangeDirection *= -1;
  }

  pulseVisualizer.update(currentHR, millis());
  pulseVisualizer.show();

  Serial.print("Current HR: ");
  Serial.println(currentHR);
  
  delay(50);  // Small delay to prevent overwhelming the LED updates
}