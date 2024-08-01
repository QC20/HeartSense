#include "ECGEmulator.h"
#include <Arduino.h>

ECGEmulator::ECGEmulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), currentStep(0) {}

void ECGEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(15);
    pixels.show();
}

void ECGEmulator::update(unsigned long currentMillis) {
    static const uint16_t TOTAL_STEPS = 100;
    static const unsigned long STEP_DURATION = CYCLE_DURATION / TOTAL_STEPS;

    if (currentMillis - lastUpdateTime >= STEP_DURATION) {
        int brightness = getECGBrightness(currentStep, TOTAL_STEPS);
        setAllLEDs(brightness);
        pixels.show();

        currentStep = (currentStep + 1) % TOTAL_STEPS;
        lastUpdateTime = currentMillis;
    }
}

int ECGEmulator::getECGBrightness(uint16_t step, uint16_t totalSteps) {
    float progress = static_cast<float>(step) / totalSteps;

    if (progress < 0.10f) {  // P wave
        return map(step, 0, totalSteps * 0.10, BASELINE_BRIGHTNESS, P_WAVE_BRIGHTNESS);
    } else if (progress < 0.16f) {  // PR segment
        return map(step, totalSteps * 0.10, totalSteps * 0.16, P_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
    } else if (progress < 0.20f) {  // Q wave
        return map(step, totalSteps * 0.16, totalSteps * 0.20, BASELINE_BRIGHTNESS, BASELINE_BRIGHTNESS - 5);
    } else if (progress < 0.24f) {  // R wave
        return map(step, totalSteps * 0.20, totalSteps * 0.24, BASELINE_BRIGHTNESS - 5, QRS_PEAK_BRIGHTNESS);
    } else if (progress < 0.28f) {  // S wave
        return map(step, totalSteps * 0.24, totalSteps * 0.28, QRS_PEAK_BRIGHTNESS, BASELINE_BRIGHTNESS - 10);
    } else if (progress < 0.36f) {  // ST segment
        return map(step, totalSteps * 0.28, totalSteps * 0.36, BASELINE_BRIGHTNESS - 10, BASELINE_BRIGHTNESS);
    } else if (progress < 0.56f) {  // T wave
        float t_progress = (progress - 0.36f) / 0.20f;
        return BASELINE_BRIGHTNESS + (T_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(t_progress * PI);
    } else if (progress < 0.64f) {  // U wave
        float u_progress = (progress - 0.56f) / 0.08f;
        return BASELINE_BRIGHTNESS + (U_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(u_progress * PI);
    } else {  // Back to baseline
        return BASELINE_BRIGHTNESS;
    }
}

void ECGEmulator::setAllLEDs(int brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
    }
}