#include <uECG.h>
#include "HapticECGEmulator.h"

#define HAPTIC_PIN 6  // Change this to the pin connected to your haptic module

int rf_cen = 8; //nRF24 chip enable pin
int rf_cs = 9; //nRF24 CS pin

HapticECGEmulator ecg(HAPTIC_PIN);

int lastRRid = -1;

void setup() {
  Serial.begin(115200);
  uECG.begin(rf_cs, rf_cen);
  ecg.begin();
}

void loop() {
  uECG.run(); // Need to call this often
  unsigned long currentMillis = millis();

  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    // A new beat has been detected
    lastRRid = currentRRid;
    int bpm = uECG.getBPM();
    int rr = uECG.getLastRR();

    // Update BPM for the haptic emulator
    ecg.setBPM(bpm);

    // Print data for each beat
    Serial.print("BPM: ");
    Serial.print(bpm);
    Serial.print(", RR: ");
    Serial.print(rr);
    Serial.print(", Emulator BPM: ");
    Serial.println(ecg.getBPM());
  }

  // Update ECG emulator
  ecg.update(currentMillis);
}