#include "HRVTrendVisualizer.h"
#include <Arduino.h>

// Constructor
HRVTrendVisualizer::HRVTrendVisualizer(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), historyIndex(0) {
    // Initialize HRV history with zeros
    for (int i = 0; i < HRV_HISTORY_SIZE; i++) {
        hrvHistory[i] = 0;
    }
}

// Initialize the NeoPixel
void HRVTrendVisualizer::begin() {
    pixels.begin();
    pixels.setBrightness(50);  // Set to 50% brightness
    pixels.show();
}

// Update the LED display based on new HRV data
void HRVTrendVisualizer::update(float currentHRV) {
    // Update the HRV history
    updateHistory(currentHRV);

    // Set the center LED color based on the current HRV
    setLEDColor(0, getColorForHRV(currentHRV));

    // Set the surrounding LEDs based on historical HRV values
    for (int i = 0; i < HRV_HISTORY_SIZE; i++) {
        setLEDColor(i + 1, getColorForHRV(hrvHistory[i]));
    }
}

// Show the current LED state
void HRVTrendVisualizer::show() {
    pixels.show();
}

// Update the HRV history with a new value
void HRVTrendVisualizer::updateHistory(float newHRV) {
    hrvHistory[historyIndex] = newHRV;
    historyIndex = (historyIndex + 1) % HRV_HISTORY_SIZE;
}

// Get the color for a given HRV value
uint32_t HRVTrendVisualizer::getColorForHRV(float hrv) {
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

// Set the color of a specific LED
void HRVTrendVisualizer::setLEDColor(uint8_t index, uint32_t color) {
    if (index < pixels.numPixels()) {
        pixels.setPixelColor(index, color);
    }
}