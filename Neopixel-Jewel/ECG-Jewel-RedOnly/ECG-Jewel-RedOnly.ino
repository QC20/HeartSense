#include <uECG.h>
#include "ECGJewelEmulator.h"

#define PIN 6
#define NUMPIXELS 7  // Changed to 7 for the Adafruit NeoPixel Jewel

// CE is on pin 8 and CS is on pin 9 on RF-NANO
int rf_cen = 8; // nRF24 chip enable pin
int rf_cs = 9;  // nRF24 CS pin

ImprovedECGEmulator ecg(PIN, NUMPIXELS);

unsigned long lastBeatTime = 0;
int lastRRid = -1;
bool playingECG = false;
unsigned long ecgStartTime = 0;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  ecg.begin();
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

    playingECG = true;
    ecgStartTime = currentMillis;
  }

  if (playingECG) {
    int currentBPM = uECG.getBPM();
    ecg.update(currentMillis - ecgStartTime, currentBPM);

    if (currentMillis - ecgStartTime >= ecg.getCycleDuration(currentBPM)) {
      playingECG = false;
      ecg.setAllLEDs(0);
      ecg.show();
    }
  }
}