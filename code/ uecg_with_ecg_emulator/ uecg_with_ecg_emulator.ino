#include <uECG.h>
#include "HapticECGEmulator.h"

#define HAPTIC_PIN 6  // Change this to the pin connected to your vibration motor

int rf_cen = 8; //nRF24 chip enable pin
int rf_cs = 9; //nRF24 CS pin

HapticECGEmulator ecg(HAPTIC_PIN);

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
  uECG.run(); // Need to call this often
  unsigned long currentMillis = millis();

  int currentRRid = uECG.getLastRRid();
  if (currentRRid != lastRRid) {
    // A new beat has been detected
    lastRRid = currentRRid;
    int bpm = uECG.getBPM();
    int rr = uECG.getLastRR();

    // Update cycle duration based on the new BPM
    ecg.updateCycleDuration(bpm);

    // Print data for each beat
    Serial.print("BPM: ");
    Serial.print(bpm);
    Serial.print(", RR: ");
    Serial.print(rr);
    Serial.print(", Cycle Duration: ");
    Serial.println(ecg.getCycleDuration());

    // Start playing the ECG waveform
    playingECG = true;
    ecgStartTime = currentMillis;
  }

  if (playingECG) {
    // Update ECG emulator
    ecg.update(currentMillis - ecgStartTime);

    // Check if we've completed one ECG cycle
    if (currentMillis - ecgStartTime >= ecg.getCycleDuration()) {
      playingECG = false;
      // Turn off vibration motor after the cycle
      ecg.setHapticIntensity(0);
    }
  }
}