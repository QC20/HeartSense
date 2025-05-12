#include <uECG.h>
#include "ECGJewelEmulator.h"

#define PIN 6
#define NUMPIXELS 7  // For the Adafruit NeoPixel Jewel

// CE is on pin 8 and CS is on pin 9 on RF-NANO
int rf_cen = 8; // nRF24 chip enable pin
int rf_cs = 9;  // nRF24 CS pin

ImprovedECGEmulator ecg(PIN, NUMPIXELS);

unsigned long lastBeatTime = 0;
int lastRRid = -1;
bool playingECG = false;
unsigned long ecgStartTime = 0;
unsigned long lastReadingTime = 0;
const unsigned long MAX_INACTIVE_TIME = 5000; // 5 seconds of no reading before turning off

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  ecg.begin();
  
  // Start with all LEDs off
  ecg.clear();
}

void loop() {
  uECG.run();
  unsigned long currentMillis = millis();

  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    lastRRid = currentRRid;
    int bpm = uECG.getBPM();
    int rr = uECG.getLastRR();

    Serial.print("BPM: ");
    Serial.print(bpm);
    Serial.print(", RR: ");
    Serial.println(rr);

    // Only activate display when we have a valid reading
    if (bpm > 30 && bpm < 200) {  // Simple validity check
      playingECG = true;
      ecgStartTime = currentMillis;
      lastReadingTime = currentMillis;
    }
  }

  // Handle the ECG animation when active
  if (playingECG) {
    int currentBPM = uECG.getBPM();
    ecg.update(currentMillis - ecgStartTime, currentBPM);

    // Complete one cycle then stop
    if (currentMillis - ecgStartTime >= ecg.getCycleDuration(currentBPM)) {
      playingECG = false;
      ecg.clear(); // Turn off LEDs
    }
  }
  
  // If no reading for MAX_INACTIVE_TIME, ensure LEDs are off
  if (currentMillis - lastReadingTime > MAX_INACTIVE_TIME && !playingECG) {
    ecg.clear();
  }
}