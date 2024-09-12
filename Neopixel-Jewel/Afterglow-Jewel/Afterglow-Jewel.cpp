#include "Afterglow-Jewel.h"
#include <Arduino.h>

HRVJewelEmulator::HRVJewelEmulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRB + NEO_KHZ800), lastUpdateTime(0), currentStep(0),
      lastHRVUpdateTime(0), currentAfterglowDuration(0), isAfterglowActive(false), afterglowStartTime(0),
      movingAverageHRV(0), recentRRIntervalsIndex(0), isIntensityModulationActive(false), 
      intensityModulationBeatsRemaining(0), intensityModulationFactor(1.0), lastRRInterval(0),
      hrvUpdateInterval(HRV_UPDATE_INTERVAL),
      significantChangeThreshold(SIGNIFICANT_CHANGE_THRESHOLD),
      maxAfterglowDurationFactor(MAX_AFTERGLOW_DURATION_FACTOR) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        recentRRIntervals[i] = 0;
    }
}

void HRVJewelEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(10);
    pixels.show();
}

void HRVJewelEmulator::update(unsigned long currentMillis, int currentRRInterval) {
    static const uint16_t TOTAL_STEPS = 95;
    uint16_t cycleDuration = getCycleDuration(currentRRInterval);
    unsigned long stepDuration = cycleDuration / TOTAL_STEPS;

    updateHRV(currentRRInterval, currentMillis);

    if (currentMillis - lastUpdateTime >= stepDuration) {
        int brightness = getECGBrightness(currentStep, TOTAL_STEPS);
        brightness = applyIntensityModulation(brightness);
        setLEDs(brightness);
        pixels.show();

        currentStep = (currentStep + 1) % TOTAL_STEPS;
        if (currentStep == 0 && isIntensityModulationActive) {
            intensityModulationBeatsRemaining--;
            if (intensityModulationBeatsRemaining == 0) {
                isIntensityModulationActive = false;
                intensityModulationFactor = 1.0;
            }
        }
        lastUpdateTime = currentMillis;
    }
}

int HRVJewelEmulator::getECGBrightness(uint16_t step, uint16_t totalSteps) {
    // ECG waveform generation logic remains the same
    // ... (previous implementation)
}

void HRVJewelEmulator::setLEDs(int brightness) {
    // LED setting logic remains the same
    // ... (previous implementation)
}

void HRVJewelEmulator::show() {
    pixels.show();
}

void HRVJewelEmulator::updateHRV(int newRRInterval, unsigned long currentMillis) {
    if (currentMillis - lastHRVUpdateTime > hrvUpdateInterval) {
        updateMovingAverageHRV(newRRInterval);

        if (isSignificantChange(newRRInterval)) {
            float hrv = calculateHRV();
            float changeMagnitude = abs(hrv - movingAverageHRV) / movingAverageHRV;
            
            // Calculate afterglow duration based on HRV change
            currentAfterglowDuration = BASE_AFTERGLOW_DURATION + 
                static_cast<int>(min(changeMagnitude, maxAfterglowDurationFactor - 1.0) * AFTERGLOW_SCALE_FACTOR * 1500);
            isAfterglowActive = true;
            afterglowStartTime = currentMillis;

            // Activate intensity modulation based on HRV change
            isIntensityModulationActive = true;
            intensityModulationBeatsRemaining = INTENSITY_MODULATION_BEATS;
            if (hrv > movingAverageHRV) {
                intensityModulationFactor = 1.0 + INTENSITY_MODULATION_FACTOR;
            } else {
                intensityModulationFactor = 1.0 - INTENSITY_MODULATION_FACTOR;
            }
        }

        lastRRInterval = newRRInterval;
        lastHRVUpdateTime = currentMillis;
    }
}

void HRVJewelEmulator::updateMovingAverageHRV(int newRRInterval) {
    recentRRIntervals[recentRRIntervalsIndex] = newRRInterval;
    recentRRIntervalsIndex = (recentRRIntervalsIndex + 1) % WINDOW_SIZE;

    movingAverageHRV = calculateHRV();
}

bool HRVJewelEmulator::isSignificantChange(int newRRInterval) {
    if (movingAverageHRV == 0) return false;
    float currentHRV = calculateHRV();
    float change = abs(currentHRV - movingAverageHRV) / movingAverageHRV;
    return change > significantChangeThreshold;
}

bool HRVJewelEmulator::isRWavePortion(uint16_t step, uint16_t totalSteps) const {
    float progress = static_cast<float>(step) / totalSteps;
    return progress >= 0.20f && progress < 0.24f;
}

int HRVJewelEmulator::applyIntensityModulation(int brightness) {
    if (isIntensityModulationActive) {
        brightness = static_cast<int>(brightness * intensityModulationFactor);
    }
    return min(255, brightness); // Ensure brightness doesn't exceed 255
}

float HRVJewelEmulator::calculateHRV() {
    // Calculate RMSSD (Root Mean Square of Successive Differences)
    float sumSquaredDiff = 0;
    int validIntervals = 0;

    for (int i = 1; i < WINDOW_SIZE; i++) {
        if (recentRRIntervals[i] > 0 && recentRRIntervals[i-1] > 0) {
            int diff = recentRRIntervals[i] - recentRRIntervals[i-1];
            sumSquaredDiff += diff * diff;
            validIntervals++;
        }
    }

    if (validIntervals > 0) {
        return sqrt(sumSquaredDiff / validIntervals);
    } else {
        return 0;
    }
}