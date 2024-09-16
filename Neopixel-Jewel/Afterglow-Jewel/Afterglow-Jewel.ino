#include <uECG.h>
#include "AfterglowJewelEmulator.h"

#define PIN 6
#define NUMPIXELS 12  // NeoPixel Jewel has 7 LEDs

int rf_cen = 8; // nRF24 chip enable pin
int rf_cs = 9;  // nRF24 CS pin

AfterglowJewelEmulator AfterglowJewelEmulator(PIN, NUMPIXELS);

unsigned long lastBeatTime = 0;
int lastRRid = -1;
bool playingECG = false;
unsigned long ecgStartTime = 0;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  AfterglowJewelEmulator.begin();
}

void loop() {
  uECG.run();
  unsigned long currentMillis = millis();

  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    lastRRid = currentRRid;
    int rr = uECG.getLastRR(); // Get the RR interval

    Serial.print("RR Interval: ");
    Serial.println(rr);

    playingECG = true;
    ecgStartTime = currentMillis;

    // Update the HRV Jewel with the new RR interval
    AfterglowJewelEmulator.update(currentMillis - ecgStartTime, rr);
  }

  if (playingECG) {
    int currentRR = uECG.getLastRR();
    AfterglowJewelEmulator.update(currentMillis - ecgStartTime, currentRR);

    if (currentMillis - ecgStartTime >= AfterglowJewelEmulator.getCycleDuration(currentRR)) {
      playingECG = false;
      AfterglowJewelEmulator.setLEDs(0);
      AfterglowJewelEmulator.show();
    }
  }
}