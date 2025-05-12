#include "HapticECGEmulator.h"
#include <Arduino.h>

HapticECGEmulator::HapticECGEmulator(uint8_t pin)
    : hapticPin(pin), lastUpdateTime(0), beatDuration(1000), pulseDuration(MIN_PULSE_DURATION_MS),
      pulseInterval(300), isVibrating(false), currentBPM(60) {}

void HapticECGEmulator::begin() {
    pinMode(hapticPin, OUTPUT);
    digitalWrite(hapticPin, LOW);
    updateTimings();
}

void HapticECGEmulator::update(unsigned long currentMillis) {
    unsigned long elapsedTime = currentMillis - lastUpdateTime;

    if (isVibrating && elapsedTime >= pulseDuration) {
        digitalWrite(hapticPin, LOW);
        isVibrating = false;
        lastUpdateTime = currentMillis;
    } else if (!isVibrating && elapsedTime >= pulseInterval) {
        analogWrite(hapticPin, MAX_INTENSITY);
        isVibrating = true;
        lastUpdateTime = currentMillis;
    }
}

void HapticECGEmulator::setBPM(int bpm) {
    if (bpm > 0 && bpm <= 300) {  // Set a reasonable upper limit for BPM
        currentBPM = bpm;
        updateTimings();
    }
}

void HapticECGEmulator::updateTimings() {
    beatDuration = 60000 / currentBPM;
    
    // Ensure minimum pulse duration
    pulseDuration = max(MIN_PULSE_DURATION_MS, beatDuration / 3);
    
    // Calculate pulse interval (time between pulses)
    pulseInterval = (beatDuration * PULSE_INTERVAL_PERCENT) / 100;
    
    // Ensure pulse interval is not shorter than pulse duration
    pulseInterval = max(pulseInterval, pulseDuration);
}