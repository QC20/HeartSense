#ifndef HAPTIC_ECG_EMULATOR_H
#define HAPTIC_ECG_EMULATOR_H

#include <stdint.h>

class HapticECGEmulator {
public:
    HapticECGEmulator(uint8_t pin);
    void begin();
    void update(unsigned long currentMillis);
    void setBPM(int bpm);
    inline int getBPM() const { return currentBPM; }

private:
    static const uint8_t MAX_INTENSITY = 255;
    static const uint16_t MIN_PULSE_DURATION_MS = 100; // Minimum pulse duration in milliseconds
    static const uint16_t PULSE_INTERVAL_PERCENT = 30; // Percentage of beat duration for pulse interval

    uint8_t hapticPin;
    unsigned long lastUpdateTime;
    unsigned long beatDuration;
    unsigned long pulseDuration;
    unsigned long pulseInterval;
    bool isVibrating;
    int currentBPM;

    void updateTimings();
};

#endif