#include <uECG.h>
#include "HRVVariabilityVisualizer.h"

// Pin definitions
#define NEOPIXEL_PIN 6
#define NUMPIXELS 7  // NeoPixel Jewel has 7 LEDs
#define RF_CEN_PIN 8 // nRF24 chip enable pin
#define RF_CS_PIN 9  // nRF24 CS pin

// Create HRVVariabilityVisualizer instance
HRVVariabilityVisualizer hrvVisualizer(NEOPIXEL_PIN, NUMPIXELS);

// Variables for HRV calculation
const int HRV_WINDOW_SIZE = 10; // Number of RR intervals to use for HRV calculation
int rrIntervals[HRV_WINDOW_SIZE];
int rrIndex = 0;

// Timing variables
unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_INTERVAL = 1000; // Update HRV every 1 second

// Other variables
int lastRRid = -1;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize uECG
  uECG.begin(RF_CS_PIN, RF_CEN_PIN);

  // Initialize HRV Variability Visualizer
  hrvVisualizer.begin();

  Serial.println("HRV Variability Visualizer initialized");
}

// Calculate HRV using SDNN method (Standard Deviation of NN intervals)
float calculateHRV() {
  long sum = 0;
  long sumOfSquares = 0;

  // Calculate sum and sum of squares
  for (int i = 0; i < HRV_WINDOW_SIZE; i++) {
    sum += rrIntervals[i];
    sumOfSquares += (long)rrIntervals[i] * rrIntervals[i];
  }

  // Calculate mean and variance
  float mean = (float)sum / HRV_WINDOW_SIZE;
  float variance = ((float)sumOfSquares / HRV_WINDOW_SIZE) - (mean * mean);

  // Return standard deviation (SDNN)
  return sqrt(variance);
}

void loop() {
  // Run uECG processing
  uECG.run();

  // Get current time
  unsigned long currentMillis = millis();

  // Check for new RR interval
  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    lastRRid = currentRRid;
    int rr = uECG.getLastRR();
    
    // Update RR interval array
    rrIntervals[rrIndex] = rr;
    rrIndex = (rrIndex + 1) % HRV_WINDOW_SIZE;

    Serial.print("New RR interval: ");
    Serial.println(rr);
  }

  // Update HRV and visualizer every UPDATE_INTERVAL
  if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL) {
    // Calculate HRV if we have enough data
    if (rrIndex == HRV_WINDOW_SIZE - 1) {
      float hrv = calculateHRV();
      Serial.print("HRV (SDNN): ");
      Serial.println(hrv);

      // Update the HRV Variability Visualizer
      hrvVisualizer.update(hrv);
      hrvVisualizer.show();
    }

    lastUpdateTime = currentMillis;
  }
}


