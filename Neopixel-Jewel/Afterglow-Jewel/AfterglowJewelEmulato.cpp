#include "AfterglowJewelEmulator.h"
#include <Arduino.h>

AfterglowJewelEmulator::AfterglowJewelEmulator(uint8_t pin, uint16_t numPixels)
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

void AfterglowJewelEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(10);
    pixels.show();
}

void AfterglowJewelEmulator::update(unsigned long currentMillis, int currentRRInterval) {
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

int AfterglowJewelEmulator::getECGBrightness(uint16_t step, uint16_t totalSteps) {
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

void AfterglowJewelEmulator::setLEDs(int brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        if (i == 0) {  // Center LED
            int centerBrightness = min(255, static_cast<int>(brightness * CENTER_LED_FACTOR));
            pixels.setPixelColor(i, pixels.Color(centerBrightness, 0, 0));
        } else {
            pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
        }
    }
}

void AfterglowJewelEmulator::show() {
    pixels.show();
}

void AfterglowJewelEmulator::updateHRV(int newRRInterval, unsigned long currentMillis) {
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

void AfterglowJewelEmulator::updateMovingAverageHRV(int newRRInterval) {
    recentRRIntervals[recentRRIntervalsIndex] = newRRInterval;
    recentRRIntervalsIndex = (recentRRIntervalsIndex + 1) % WINDOW_SIZE;

    movingAverageHRV = calculateHRV();
}

bool AfterglowJewelEmulator::isSignificantChange(int newRRInterval) {
    if (movingAverageHRV == 0) return false;
    float currentHRV = calculateHRV();
    float change = abs(currentHRV - movingAverageHRV) / movingAverageHRV;
    return change > significantChangeThreshold;
}

bool AfterglowJewelEmulator::isRWavePortion(uint16_t step, uint16_t totalSteps) const {
    float progress = static_cast<float>(step) / totalSteps;
    return progress >= 0.20f && progress < 0.24f;
}

int AfterglowJewelEmulator::applyIntensityModulation(int brightness) {
    if (isIntensityModulationActive) {
        brightness = static_cast<int>(brightness * intensityModulationFactor);
    }
    return min(255, brightness); // Ensure brightness doesn't exceed 255
}

float AfterglowJewelEmulator::calculateHRV() {
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