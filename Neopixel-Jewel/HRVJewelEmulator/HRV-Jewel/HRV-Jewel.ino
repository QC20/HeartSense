#include <uECG.h>
#include "HRVJewelEmulator.h"

#define PIN 6
#define NUMPIXELS 7  // NeoPixel Jewel has 7 LEDs

// Try changing these around if it does not work. Sometimes I have soldered them on vice versa. 
int rf_cen = 9; // nRF24 chip enable pin
int rf_cs = 8;  // nRF24 CS pin

HRVJewelEmulator hrvJewel(PIN, NUMPIXELS);

unsigned long lastUpdateTime = 0;
int lastRRid = -1;
const int HRV_WINDOW_SIZE = 10; // Number of RR intervals to use for HRV calculation
int rrIntervals[HRV_WINDOW_SIZE];
int rrIndex = 0;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  hrvJewel.begin();
}

float calculateHRV() {
  long sum = 0;
  long sumOfSquares = 0;
  for (int i = 0; i < HRV_WINDOW_SIZE; i++) {
    sum += rrIntervals[i];
    sumOfSquares += (long)rrIntervals[i] * rrIntervals[i];
  }
  float mean = (float)sum / HRV_WINDOW_SIZE;
  float variance = ((float)sumOfSquares / HRV_WINDOW_SIZE) - (mean * mean);
  return sqrt(variance); // This gives us SDNN (Standard Deviation of NN intervals)
}

void loop() {
  uECG.run();
  unsigned long currentMillis = millis();

  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    lastRRid = currentRRid;
    int rr = uECG.getLastRR();
    
    // Update RR interval array
    rrIntervals[rrIndex] = rr;
    rrIndex = (rrIndex + 1) % HRV_WINDOW_SIZE;

    // Calculate HRV if we have enough data
    if (rrIndex == 0) {
      float hrv = calculateHRV();
      Serial.print("HRV (SDNN): ");
      Serial.println(hrv);

      // Update the HRV Jewel
      hrvJewel.update(currentMillis, hrv);
    }
  }

  // Continuously update the Jewel to handle transitions and afterglow effects
  if (currentMillis - lastUpdateTime >= 20) { // Update every 20ms for smooth transitions
    hrvJewel.update(currentMillis, 0); // Pass 0 as HRV to use the last known value
    lastUpdateTime = currentMillis;
  }
}