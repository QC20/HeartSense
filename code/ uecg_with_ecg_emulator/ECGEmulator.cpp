#include "HapticECGEmulator.h"

HapticECGEmulator::HapticECGEmulator(uint8_t pin)
    : hapticPin(pin), lastUpdateTime(0), currentStep(0), cycleDuration(CYCLE_DURATION) {}

void HapticECGEmulator::begin() {
    pinMode(hapticPin, OUTPUT);
    analogWrite(hapticPin, 0);
}

void HapticECGEmulator::update(unsigned long currentMillis) {
    unsigned long stepDuration = cycleDuration / TOTAL_STEPS;

    if (currentMillis - lastUpdateTime >= stepDuration) {
        int intensity = getECGIntensity(currentStep, TOTAL_STEPS);
        setHapticIntensity(intensity);

        currentStep = (currentStep + 1) % TOTAL_STEPS;
        lastUpdateTime = currentMillis;
    }
}

void HapticECGEmulator::updateCycleDuration(int bpm) {
    if (bpm > 0) {
        cycleDuration = 60000 / bpm;
    }
}

uint16_t HapticECGEmulator::getCycleDuration() const {
    return cycleDuration;
}

int HapticECGEmulator::getECGIntensity(uint16_t step, uint16_t totalSteps) {
    float progress = static_cast<float>(step) / totalSteps;
    int intensity;

    if (progress < 0.10f) {  // P wave
        intensity = map(step, 0, totalSteps * 0.10, BASELINE_INTENSITY, P_WAVE_INTENSITY);
    } else if (progress < 0.16f) {  // PR segment
        intensity = map(step, totalSteps * 0.10, totalSteps * 0.16, P_WAVE_INTENSITY, BASELINE_INTENSITY);
    } else if (progress < 0.20f) {  // Q wave
        intensity = BASELINE_INTENSITY;
    } else if (progress < 0.24f) {  // R wave
        intensity = map(step, totalSteps * 0.20, totalSteps * 0.24, BASELINE_INTENSITY, QRS_PEAK_INTENSITY);
    } else if (progress < 0.28f) {  // S wave
        intensity = map(step, totalSteps * 0.24, totalSteps * 0.28, QRS_PEAK_INTENSITY, BASELINE_INTENSITY);
    } else if (progress < 0.36f) {  // ST segment
        intensity = BASELINE_INTENSITY;
    } else if (progress < 0.52f) {  // T wave
        float t_progress = (progress - 0.36f) / 0.16f;
        intensity = BASELINE_INTENSITY + (T_WAVE_INTENSITY - BASELINE_INTENSITY) * sin(t_progress * PI);
    } else {  // Back to baseline
        intensity = BASELINE_INTENSITY;
    }

    return intensity;
}

void HapticECGEmulator::setHapticIntensity(int intensity) {
    analogWrite(hapticPin, intensity);
}