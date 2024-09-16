#include "HeartRateFlowVisualizer.h"

#define NEOPIXEL_PIN 6
#define NUMPIXELS 24  // Adjust based on your LED ring

HeartRateFlowVisualizer hrFlowVisualizer(NEOPIXEL_PIN, NUMPIXELS);

// Simulated heart rate for demonstration
float currentHR = 60;
float hrChangeDirection = 1;

void setup() {
  Serial.begin(115200);
  hrFlowVisualizer.begin();
}

void loop() {
  // Simulate changing heart rate for demonstration
  currentHR += hrChangeDirection * random(0, 5) * 0.1;
  if (currentHR > 120 || currentHR < 50) {
    hrChangeDirection *= -1;
  }

  hrFlowVisualizer.update(currentHR, millis());
  hrFlowVisualizer.show();

  Serial.print("Current HR: ");
  Serial.println(currentHR);
  
  delay(50);  // Small delay to prevent overwhelming the LED updates
}