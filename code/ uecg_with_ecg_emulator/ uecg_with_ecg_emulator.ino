#include <uECG.h>
#include "ECGEmulator.h"

#define PIN 6
#define NUMPIXELS 12

int rf_cen = 8; //nRF24 chip enable pin
int rf_cs = 9; //nRF24 CS pin

ECGEmulator ecg(PIN, NUMPIXELS);

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
    
    // Print data for each beat
    Serial.print("BPM: ");
    Serial.print(bpm);
    Serial.print(", RR: ");
    Serial.println(rr);
    
    // Start playing the ECG waveform
    playingECG = true;
    ecgStartTime = currentMillis;
  }

  if (playingECG) {
    // Update ECG emulator
    ecg.update(currentMillis - ecgStartTime);
    
    // Check if we've completed one ECG cycle
    if (currentMillis - ecgStartTime >= ECGEmulator::CYCLE_DURATION) {
      playingECG = false;
      // Turn off all LEDs after the cycle
      ecg.setAllLEDs(0);
      ecg.show();
    }
  }
}