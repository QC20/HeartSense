#include "ECGEmulator.h"
#include <Arduino.h>

const float BRIGHTNESS_SCALE = 0.925;

ECGEmulator::ECGEmulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), currentStep(0),
      lastHeartRateUpdateTime(0), currentAfterglowDuration(0), isAfterglowActive(false), afterglowStartTime(0), movingAverage(0) {}

void ECGEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(15);
    pixels.show();
}

void ECGEmulator::update(unsigned long currentMillis, int currentHeartRate) {
    static const uint16_t TOTAL_STEPS = 95;
    static const unsigned long STEP_DURATION = CYCLE_DURATION / TOTAL_STEPS;

    updateHeartRate(currentHeartRate, currentMillis);

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
    int brightness;

    if (progress < 0.10f) {  // P wave
        brightness = map(step, 0, totalSteps * 0.10, BASELINE_BRIGHTNESS, P_WAVE_BRIGHTNESS);
    } else if (progress < 0.16f) {  // PR segment
        brightness = map(step, totalSteps * 0.10, totalSteps * 0.16, P_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
    } else if (progress < 0.20f) {  // Q wave
        brightness = map(step, totalSteps * 0.16, totalSteps * 0.20, BASELINE_BRIGHTNESS, BASELINE_BRIGHTNESS - 5);
    } else if (progress < 0.24f) {  // R wave
        brightness = map(step, totalSteps * 0.20, totalSteps * 0.24, BASELINE_BRIGHTNESS - 5, QRS_PEAK_BRIGHTNESS);
        
        if (isAfterglowActive && isRWavePortion(step, totalSteps)) {
            unsigned long elapsedAfterglowTime = millis() - afterglowStartTime;
            if (elapsedAfterglowTime < currentAfterglowDuration) {
                brightness = QRS_PEAK_BRIGHTNESS;
            } else {
                isAfterglowActive = false;
            }
        }
    } else if (progress < 0.28f) {  // S wave
        brightness = map(step, totalSteps * 0.24, totalSteps * 0.28, QRS_PEAK_BRIGHTNESS, BASELINE_BRIGHTNESS - 10);
    } else if (progress < 0.36f) {  // ST segment
        brightness = map(step, totalSteps * 0.28, totalSteps * 0.32, BASELINE_BRIGHTNESS - 10, BASELINE_BRIGHTNESS);
    } else if (progress < 0.52f) {  // T wave
        float t_progress = (progress - 0.36f) / 0.20f;
        brightness = BASELINE_BRIGHTNESS + (T_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(t_progress * PI);
    } else {  // Back to baseline
        brightness = BASELINE_BRIGHTNESS;
    }

    return static_cast<int>(brightness * BRIGHTNESS_SCALE);
}

void ECGEmulator::setAllLEDs(int brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
    }
}

void ECGEmulator::show() {
    pixels.show();
}

void ECGEmulator::updateHeartRate(int newHeartRate, unsigned long currentMillis) {
    if (currentMillis - lastHeartRateUpdateTime > 5000) { // Update every 5 seconds
        updateMovingAverage(newHeartRate);
        
        if (isSignificantChange(newHeartRate)) {
            currentAfterglowDuration = BASE_AFTERGLOW_DURATION + static_cast<int>(abs(newHeartRate - movingAverage) / movingAverage * AFTERGLOW_SCALE_FACTOR * 1000);
            isAfterglowActive = true;
            afterglowStartTime = currentMillis;
        }
        
        lastHeartRateUpdateTime = currentMillis;
    }
}

void ECGEmulator::updateMovingAverage(int newHeartRate) {
    if (recentHeartRates.size() >= WINDOW_SIZE) {
        recentHeartRates.pop_front();
    }
    recentHeartRates.push_back(newHeartRate);
    
    movingAverage = std::accumulate(recentHeartRates.begin(), recentHeartRates.end(), 0.0) / recentHeartRates.size();
}

bool ECGEmulator::isSignificantChange(int newHeartRate) {
    if (recentHeartRates.empty()) return false;
    double change = std::abs(newHeartRate - movingAverage) / movingAverage;
    return change > CHANGE_THRESHOLD;
}

bool ECGEmulator::isRWavePortion(uint16_t step, uint16_t totalSteps) const {
    float progress = static_cast<float>(step) / totalSteps;
    return progress >= 0.20f && progress < 0.24f;
}