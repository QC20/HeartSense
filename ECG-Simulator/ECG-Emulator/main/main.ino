#include "ECGEmulator.h"

#define PIN 6
#define NUMPIXELS 12

ECGEmulator ecg(PIN, NUMPIXELS);

void setup() {
    ecg.begin();
}

void loop() {
    ecg.update(millis());
}