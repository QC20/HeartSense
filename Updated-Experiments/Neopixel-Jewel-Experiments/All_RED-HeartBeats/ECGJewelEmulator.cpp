#includse "ECGJewelEmulator.h"
#include <Arduino.h>

ImprovedECGEmulator::ImprovedECGEmulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), lastUpdateTime(0), currentStep(0),
      lastHeartRateUpdateTime(0), currentAfterglowDuration(0), isAfterglowActive(false), afterglowStartTime(0),
      movingAverage(0), recentHeartRatesIndex(0), isIntensityModulationActive(false), 
      intensityModulationBeatsRemaining(0), intensityModulationFactor(1.0), lastHeartRate(0),
      heartRateUpdateInterval(HEART_RATE_UPDATE_INTERVAL),
      significantChangeThreshold(SIGNIFICANT_CHANGE_THRESHOLD),
      maxAfterglowDurationFactor(MAX_AFTERGLOW_DURATION_FACTOR) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        recentHeartRates[i] = 0;
    }
}

void ImprovedECGEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(200); // Reduced from 255 to 200 for overall dimmer effect
    pixels.show();
}

void ImprovedECGEmulator::update(unsigned long currentMillis, int currentHeartRate) {
    static const uint16_t TOTAL_STEPS = 95;
    uint16_t cycleDuration = getCycleDuration(currentHeartRate);
    unsigned long stepDuration = cycleDuration / TOTAL_STEPS;

    updateHeartRate(currentHeartRate, currentMillis);

    if (currentMillis - lastUpdateTime >= stepDuration) {
        int brightness = getECGBrightness(currentStep, TOTAL_STEPS);
        brightness = applyIntensityModulation(brightness);
        setAllLEDs(brightness);
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

int ImprovedECGEmulator::getECGBrightness(uint16_t step, uint16_t totalSteps) {
    float progress = static_cast<float>(step) / totalSteps;
    int brightness;

    // Simplified ECG curve with less visual feedback
    if (progress < 0.10f) {  // P wave (subtle)
        brightness = map(step, 0, totalSteps * 0.10, BASELINE_BRIGHTNESS, P_WAVE_BRIGHTNESS);
    } else if (progress < 0.16f) {  // PR segment
        brightness = map(step, totalSteps * 0.10, totalSteps * 0.16, P_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
    } else if (progress < 0.20f) {  // Q wave (minimal)
        brightness = map(step, totalSteps * 0.16, totalSteps * 0.20, BASELINE_BRIGHTNESS, BASELINE_BRIGHTNESS);
    } else if (progress < 0.24f) {  // R wave (main peak but more subtle)
        brightness = map(step, totalSteps * 0.20, totalSteps * 0.24, BASELINE_BRIGHTNESS, QRS_PEAK_BRIGHTNESS);

        if (isAfterglowActive && isRWavePortion(step, totalSteps)) {
            unsigned long elapsedAfterglowTime = millis() - afterglowStartTime;
            if (elapsedAfterglowTime < currentAfterglowDuration) {
                brightness = QRS_PEAK_BRIGHTNESS;
            } else {
                isAfterglowActive = false;
            }
        }
    } else if (progress < 0.28f) {  // S wave (quicker fade)
        brightness = map(step, totalSteps * 0.24, totalSteps * 0.28, QRS_PEAK_BRIGHTNESS, BASELINE_BRIGHTNESS);
    } else if (progress < 0.36f) {  // ST segment
        brightness = BASELINE_BRIGHTNESS;
    } else if (progress < 0.52f) {  // T wave (more subtle)
        float t_progress = (progress - 0.36f) / 0.20f;
        brightness = BASELINE_BRIGHTNESS + (T_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(t_progress * PI);
    } else {  // Back to baseline (off)
        brightness = BASELINE_BRIGHTNESS;
    }

    return static_cast<int>(brightness * BRIGHTNESS_SCALE);
}

void ImprovedECGEmulator::setAllLEDs(int brightness) {
    // Add a small amount of warm white to soften the red at low levels for a nicer ambient effect
    int white = (brightness > 0 && brightness < 10) ? 1 : 0;
    
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, 0, 0, white));
    }
}

void ImprovedECGEmulator::show() {
    pixels.show();
}

void ImprovedECGEmulator::clear() {
    // Turn off all LEDs
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
    }
    pixels.show();
}

void ImprovedECGEmulator::updateHeartRate(int newHeartRate, unsigned long currentMillis) {
    if (currentMillis - lastHeartRateUpdateTime > heartRateUpdateInterval) {
        updateMovingAverage(newHeartRate);

        if (isSignificantChange(newHeartRate)) {
            float changeMagnitude = abs(newHeartRate - movingAverage) / movingAverage;
            currentAfterglowDuration = BASE_AFTERGLOW_DURATION + 
                static_cast<int>(min(changeMagnitude, maxAfterglowDurationFactor - 1.0) * AFTERGLOW_SCALE_FACTOR * 1000);
            isAfterglowActive = true;
            afterglowStartTime = currentMillis;

            // Activate intensity modulation but with reduced effect
            isIntensityModulationActive = true;
            intensityModulationBeatsRemaining = INTENSITY_MODULATION_BEATS;
            if (newHeartRate > lastHeartRate) {
                intensityModulationFactor = 1.0 + INTENSITY_MODULATION_FACTOR;
            } else {
                intensityModulationFactor = 1.0 - INTENSITY_MODULATION_FACTOR;
            }
        }

        lastHeartRate = newHeartRate;
        lastHeartRateUpdateTime = currentMillis;
    }
}

void ImprovedECGEmulator::updateMovingAverage(int newHeartRate) {
    recentHeartRates[recentHeartRatesIndex] = newHeartRate;
    recentHeartRatesIndex = (recentHeartRatesIndex + 1) % WINDOW_SIZE;

    float sum = 0;
    int validValues = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        if (recentHeartRates[i] > 0) {
            sum += recentHeartRates[i];
            validValues++;
        }
    }
    movingAverage = validValues > 0 ? sum / validValues : 0;
}

bool ImprovedECGEmulator::isSignificantChange(int newHeartRate) {
    if (movingAverage == 0) return false;
    float change = abs(newHeartRate - movingAverage) / movingAverage;
    return change > significantChangeThreshold;
}

bool ImprovedECGEmulator::isRWavePortion(uint16_t step, uint16_t totalSteps) const {
    float progress = static_cast<float>(step) / totalSteps;
    return progress >= 0.20f && progress < 0.24f;
}

int ImprovedECGEmulator::applyIntensityModulation(int brightness) {
    if (isIntensityModulationActive) {
        brightness = static_cast<int>(brightness * intensityModulationFactor);
    }
    return min(255, brightness); // Ensure brightness doesn't exceed 255
}