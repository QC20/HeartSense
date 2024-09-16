#include "HRVJewelEmulator.h"
#include <Arduino.h>

HRVJewelEmulator::HRVJewelEmulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), lastUpdateTime(0),
      lastHRVUpdateTime(0), currentAfterglowDuration(0), isAfterglowActive(false), afterglowStartTime(0),
      movingAverage(0), recentHRVsIndex(0), intensityModulationFactor(1.0), lastHRV(0),
      hrvUpdateInterval(HRV_UPDATE_INTERVAL),
      significantChangeThreshold(SIGNIFICANT_CHANGE_THRESHOLD),
      maxAfterglowDurationFactor(MAX_AFTERGLOW_DURATION_FACTOR) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
        recentHRVs[i] = 0;
    }
}

void HRVJewelEmulator::begin() {
    pixels.begin();
    pixels.setBrightness(10);
    pixels.show();
}

void HRVJewelEmulator::update(unsigned long currentMillis, float currentHRV) {
    updateHRV(currentHRV, currentMillis);

    if (currentMillis - lastUpdateTime >= 20) { // Update every 20ms for smooth transitions
        int brightness = calculateBrightness(currentHRV);
        brightness = applyIntensityModulation(brightness);
        setLEDs(brightness);
        pixels.show();

        lastUpdateTime = currentMillis;
    }
}

int HRVJewelEmulator::calculateBrightness(float hrv) {
    // Map HRV to brightness. This is a simple linear mapping and may need adjustment.
    // Assuming HRV typically ranges from about 20 to 200 ms
    float normalizedHRV = constrain(hrv, 20, 200);
    int brightness = map(normalizedHRV, 20, 200, MAX_BRIGHTNESS, BASELINE_BRIGHTNESS);
    return brightness;
}

void HRVJewelEmulator::setLEDs(int brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        if (i == 0) {  // Center LED
            int centerBrightness = min(255, static_cast<int>(brightness * CENTER_LED_FACTOR));
            pixels.setPixelColor(i, pixels.Color(centerBrightness, 0, 0, centerBrightness));
        } else {
            pixels.setPixelColor(i, pixels.Color(brightness, 0, 0, brightness));
        }
    }
}

void HRVJewelEmulator::show() {
    pixels.show();
}

void HRVJewelEmulator::updateHRV(float newHRV, unsigned long currentMillis) {
    if (currentMillis - lastHRVUpdateTime > hrvUpdateInterval) {
        updateMovingAverage(newHRV);

        if (isSignificantChange(newHRV)) {
            float changeMagnitude = abs(newHRV - movingAverage) / movingAverage;
            currentAfterglowDuration = BASE_AFTERGLOW_DURATION + 
                static_cast<int>(min(changeMagnitude, maxAfterglowDurationFactor - 1.0) * AFTERGLOW_SCALE_FACTOR * 1500);
            isAfterglowActive = true;
            afterglowStartTime = currentMillis;

            // Update intensity modulation
            if (newHRV > lastHRV) {
                intensityModulationFactor = 1.0 + (newHRV - lastHRV) / lastHRV;
            } else {
                intensityModulationFactor = 1.0 - (lastHRV - newHRV) / lastHRV;
            }
        }

        lastHRV = newHRV;
        lastHRVUpdateTime = currentMillis;
    }
}

void HRVJewelEmulator::updateMovingAverage(float newHRV) {
    recentHRVs[recentHRVsIndex] = newHRV;
    recentHRVsIndex = (recentHRVsIndex + 1) % WINDOW_SIZE;

    float sum = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        sum += recentHRVs[i];
    }
    movingAverage = sum / WINDOW_SIZE;
}

bool HRVJewelEmulator::isSignificantChange(float newHRV) {
    if (movingAverage == 0) return false;
    float change = abs(newHRV - movingAverage) / movingAverage;
    return change > significantChangeThreshold;
}

int HRVJewelEmulator::applyIntensityModulation(int brightness) {
    brightness = static_cast<int>(brightness * intensityModulationFactor);
    return constrain(brightness, BASELINE_BRIGHTNESS, MAX_BRIGHTNESS);
}