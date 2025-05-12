#include <uECG.h>
#include "Afterglow_HeartSense_Emulator.h"

#define PIN 6
#define NUMPIXELS 12

// This is the correct setup for the following RF-Nano V3.0 clone: 
int rf_cen = 8; //nRF24 chip enable pin
int rf_cs = 9; //nRF24 CS pin

HeartSenseEmulator heartSense(PIN, NUMPIXELS);

unsigned long lastBeatTime = 0;
int lastRRid = -1;
bool playingHeartSense = false;
unsigned long heartSenseStartTime = 0;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  heartSense.begin();
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
    
    playingHeartSense = true;
    heartSenseStartTime = currentMillis;
  }

  if (playingHeartSense) {
    int currentBPM = uECG.getBPM();
    heartSense.update(currentMillis - heartSenseStartTime, currentBPM);
    
    if (currentMillis - heartSenseStartTime >= HeartSenseEmulator::CYCLE_DURATION) {
      playingHeartSense = false;
      heartSense.setAllLEDs(0);
      heartSense.show();
    }
  }
}