#include "PulseWaveVisualizer.h"
#include <Arduino.h>

PulseWaveVisualizer::PulseWaveVisualizer(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), 
      currentHR(60), prevHR(60), rateOfChange(0),
      breathDuration(1000), lastBreathTime(0), breathState(false) {
    baseColor = pixels.Color(255, 0, 0);  // Default to blue
}

void PulseWaveVisualizer::begin() {
    pixels.begin();
    pixels.setBrightness(MAX_BRIGHTNESS);
    pixels.show();
}

void PulseWaveVisualizer::update(float newHR, unsigned long currentMillis) {
    // Calculate rate of change
    rateOfChange = newHR - currentHR;
    prevHR = currentHR;
    currentHR = newHR;

    // Update breath duration based on heart rate
    breathDuration = map(currentHR, MIN_HR, MAX_HR, MAX_BREATH_DURATION, MIN_BREATH_DURATION);

    // Calculate current brightness based on breathing effect
    uint8_t brightness = calculateBreathBrightness(currentMillis);

    // Apply flutter effect if rate of change is above threshold
    if (abs(rateOfChange) > RAPID_CHANGE_THRESHOLD) {
        brightness = applyFlutter(brightness, currentMillis);
    }

    // Set LEDs with calculated brightness
    setAllLEDs(brightness);
}

void PulseWaveVisualizer::show() {
    pixels.show();
}

uint8_t PulseWaveVisualizer::calculateBreathBrightness(unsigned long currentMillis) {
    unsigned long elapsedTime = currentMillis - lastBreathTime;

    if (elapsedTime >= breathDuration) {
        breathState = !breathState;
        lastBreathTime = currentMillis;
    }

    if (breathState) {
        return map(elapsedTime, 0, breathDuration, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    } else {
        return map(elapsedTime, 0, breathDuration, MAX_BRIGHTNESS, MIN_BRIGHTNESS);
    }
}

void PulseWaveVisualizer::setAllLEDs(uint8_t brightness) {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, applyBrightness(baseColor, brightness));
    }
}

uint32_t PulseWaveVisualizer::applyBrightness(uint32_t color, uint8_t brightness) {
    uint8_t r = (uint8_t)(color >> 16);
    uint8_t g = (uint8_t)(color >> 8);
    uint8_t b = (uint8_t)color;
    
    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;
    
    return pixels.Color(r, g, b, 0);
}

uint8_t PulseWaveVisualizer::applyFlutter(uint8_t brightness, unsigned long currentMillis) {
    // Create a rapid oscillation effect
    int8_t flutter = sin((currentMillis * FLUTTER_FREQUENCY) / 1000.0 * 2 * PI) * FLUTTER_AMPLITUDE;
    return constrain(brightness + flutter, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
}