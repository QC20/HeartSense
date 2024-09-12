#include "HRVPulseModulator.h"
#include <Arduino.h>

// Constructor
HRVPulseModulator::HRVPulseModulator(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), currentHRV(0), 
      pulseDuration(MIN_PULSE_DURATION), lastPulseTime(0), pulseState(false) {
}

// Initialize the NeoPixel
void HRVPulseModulator::begin() {
    pixels.begin();
    pixels.setBrightness(MAX_BRIGHTNESS);
    pixels.show();
}

// Update the LED pulsing based on new HRV data
void HRVPulseModulator::update(float newHRV, unsigned long currentMillis) {
    // Update HRV and pulse duration
    currentHRV = newHRV;
    updatePulseDuration(newHRV);

    // Calculate current brightness based on pulse timing
    uint8_t brightness = calculateBrightness(currentMillis);

    // Set all LEDs to the calculated brightness
    setAllLEDs(brightness);
}

// Show the current LED state
void HRVPulseModulator::show() {
    pixels.show();
}

// Update pulse duration based on HRV
void HRVPulseModulator::updatePulseDuration(float hrv) {
    // Normalize HRV value to 0-1 range
    float normalizedHRV = constrain(hrv, MIN_HRV, MAX_HRV);
    normalizedHRV = (normalizedHRV - MIN_HRV) / (MAX_HRV - MIN_HRV);

    // Map normalized HRV to pulse duration
    // Higher HRV (more relaxed) = longer pulse duration
    pulseDuration = map(normalizedHRV * 1000, 0, 1000, MIN_PULSE_DURATION, MAX_PULSE_DURATION);
}

// Calculate current brightness based on pulse timing
uint8_t HRVPulseModulator::calculateBrightness(unsigned long currentMillis) {
    unsigned long elapsedTime = currentMillis - lastPulseTime;

    // Check if it's time to switch pulse state
    if (elapsedTime >= pulseDuration) {
        pulseState = !pulseState;
        lastPulseTime = currentMillis;
    }

    // Calculate brightness based on current pulse state and elapsed time
    if (pulseState) {
        // Fading in
        return map(elapsedTime, 0, pulseDuration, 0, MAX_BRIGHTNESS);
    } else {
        // Fading out
        return map(elapsedTime, 0, pulseDuration, MAX_BRIGHTNESS, 0);
    }
}

// Set all LEDs to the specified brightness
void HRVPulseModulator::setAllLEDs(uint8_t brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness, brightness));
    }
}