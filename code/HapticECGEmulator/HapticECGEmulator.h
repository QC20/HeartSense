#ifndef HAPTIC_ECG_EMULATOR_H
#define HAPTIC_ECG_EMULATOR_H

class HapticECGEmulator {
public:
    HapticECGEmulator(uint8_t pin);
    void begin();
    void update(unsigned long currentMillis);
    void setHapticIntensity(int intensity);
    void updateCycleDuration(int bpm);
    uint16_t getCycleDuration() const;

private:
    static const uint8_t BASELINE_INTENSITY = 0;
    static const uint8_t P_WAVE_INTENSITY = 50;
    static const uint8_t QRS_PEAK_INTENSITY = 255;
    static const uint8_t T_WAVE_INTENSITY = 100;

    uint8_t hapticPin;
    unsigned long lastUpdateTime;
    uint16_t currentStep;
    uint16_t cycleDuration;  // in milliseconds

    int getECGIntensity(uint16_t step, uint16_t totalSteps);
};

#endif