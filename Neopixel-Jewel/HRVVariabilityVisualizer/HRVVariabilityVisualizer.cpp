#include "HRVVariabilityVisualizer.h"
#include <Arduino.h>

// Constructor
HRVVariabilityVisualizer::HRVVariabilityVisualizer(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), historyIndex(0) {
    // Initialize HRV history with zeros
    for (int i = 0; i < HRV_HISTORY_SIZE; i++) {
        hrvHistory[i] = 0;
    }
}

// Initialize the NeoPixel
void HRVVariabilityVisualizer::begin() {
    pixels.begin();
    pixels.setBrightness(MAX_BRIGHTNESS);
    pixels.show();
}

// Update the LED display based on new HRV data
void HRVVariabilityVisualizer::update(float currentHRV) {
    // Update the HRV history
    updateHistory(currentHRV);

    // Calculate variability
    uint8_t variability = calculateVariability();

    // Set LEDs based on variability
    setLEDs(variability);
}

// Show the current LED state
void HRVVariabilityVisualizer::show() {
    pixels.show();
}

// Update the HRV history with a new value
void HRVVariabilityVisualizer::updateHistory(float newHRV) {
    hrvHistory[historyIndex] = newHRV;
    historyIndex = (historyIndex + 1) % HRV_HISTORY_SIZE;
}

// Calculate variability based on HRV history
uint8_t HRVVariabilityVisualizer::calculateVariability() {
    float sum = 0;
    float sumOfSquares = 0;
    
    // Calculate mean and variance
    for (int i = 0; i < HRV_HISTORY_SIZE; i++) {
        sum += hrvHistory[i];
        sumOfSquares += hrvHistory[i] * hrvHistory[i];
    }
    float mean = sum / HRV_HISTORY_SIZE;
    float variance = (sumOfSquares / HRV_HISTORY_SIZE) - (mean * mean);
    
    // Calculate standard deviation
    float stdDev = sqrt(variance);
    
    // Map standard deviation to number of LEDs to light (0-6)
    return map(constrain(stdDev, 0, 50), 0, 50, 0, 6);
}

// Set LEDs based on variability
void HRVVariabilityVisualizer::setLEDs(uint8_t numLit) {
    // Get color based on the most recent HRV value
    uint32_t color = getColorForHRV(hrvHistory[(historyIndex - 1 + HRV_HISTORY_SIZE) % HRV_HISTORY_SIZE]);

    // Set LEDs
    for (int i = 0; i < pixels.numPixels(); i++) {
        if (i < numLit) {
            pixels.setPixelColor(i, color);
        } else {
            pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
        }
    }
}

// Get color for a given HRV value
uint32_t HRVVariabilityVisualizer::getColorForHRV(float hrv) {
    // Normalize HRV value to 0-1 range
    float normalizedHRV = constrain(hrv, MIN_HRV, MAX_HRV);
    normalizedHRV = (normalizedHRV - MIN_HRV) / (MAX_HRV - MIN_HRV);

    // Color gradient: Red (low HRV) -> Yellow -> Green (high HRV)
    uint8_t r, g, b;
    if (normalizedHRV < 0.5) {
        // Red to Yellow
        r = 255;
        g = 255 * (normalizedHRV * 2);
        b = 0;
    } else {
        // Yellow to Green
        r = 255 * ((1 - normalizedHRV) * 2);
        g = 255;
        b = 0;
    }

    return pixels.Color(r, g, b);
}