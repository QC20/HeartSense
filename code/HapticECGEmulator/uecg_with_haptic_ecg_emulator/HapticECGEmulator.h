#ifndef HAPTIC_ECG_EMULATOR_H
#define HAPTIC_ECG_EMULATOR_H

#include <stdint.h>

class HapticECGEmulator {
public:
    HapticECGEmulator(uint8_t pin);
    void begin();
    void update(unsigned long currentMillis);
    void setHapticIntensity(int intensity);
    void setBPM(int bpm);
    int getBPM() const;

private:
    static const uint8_t BASELINE_INTENSITY = 0;
    static const uint8_t P_WAVE_INTENSITY = 50;
    static const uint8_t QRS_PEAK_INTENSITY = 255;
    static const uint8_t T_WAVE_INTENSITY = 100;
    static const uint16_t TOTAL_STEPS = 95;

    uint8_t hapticPin;
    unsigned long lastUpdateTime;
    uint16_t currentStep;
    int currentBPM;
    unsigned long stepDuration;

    int getECGIntensity(uint16_t step, uint16_t totalSteps);
    void updateStepDuration();
};

#endif