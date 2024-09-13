#include <uECG.h>
#include "AfterglowJewelEmulator.h"

#define PIN 6
#define NUMPIXELS 7  // NeoPixel Jewel has 7 LEDs

int rf_cen = 8; // nRF24 chip enable pin
int rf_cs = 9;  // nRF24 CS pin

HRVJewelEmulator hrvJewel(PIN, NUMPIXELS);

unsigned long lastBeatTime = 0;
int lastRRid = -1;
bool playingECG = false;
unsigned long ecgStartTime = 0;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  hrvJewel.begin();
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
    hrvJewel.update(currentMillis - ecgStartTime, rr);
  }

  if (playingECG) {
    int currentRR = uECG.getLastRR();
    hrvJewel.update(currentMillis - ecgStartTime, currentRR);

    if (currentMillis - ecgStartTime >= hrvJewel.getCycleDuration(currentRR)) {
      playingECG = false;
      hrvJewel.setLEDs(0);
      hrvJewel.show();
    }
  }
}