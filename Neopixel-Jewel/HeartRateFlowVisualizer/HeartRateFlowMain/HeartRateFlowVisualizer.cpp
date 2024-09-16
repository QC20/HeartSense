#include "HeartRateFlowVisualizer.h"
#include <Arduino.h>

HeartRateFlowVisualizer::HeartRateFlowVisualizer(uint8_t pin, uint16_t numPixels)
    : pixels(numPixels, pin, NEO_GRBW + NEO_KHZ800), 
      currentHR(60), prevHR(60), rateOfChange(0), flowPosition(0) {
    baseColor = pixels.Color(0, 0, 255);  // Default to blue
}

void HeartRateFlowVisualizer::begin() {
    pixels.begin();
    pixels.setBrightness(MAX_BRIGHTNESS);
    pixels.show();
}

void HeartRateFlowVisualizer::update(float newHR, unsigned long currentMillis) {
    // Calculate rate of change
    rateOfChange = newHR - currentHR;
    prevHR = currentHR;
    currentHR = newHR;

    updateFlowPosition(currentMillis);

    // Set LEDs based on flow position
    for (int i = 0; i < pixels.numPixels(); i++) {
        uint32_t pixelColor = getColorForPixel(i, flowPosition);
        uint8_t brightness = getPixelBrightness(i, flowPosition);
        pixels.setPixelColor(i, pixels.Color(
            ((pixelColor >> 16) & 0xFF) * brightness / 255,
            ((pixelColor >> 8) & 0xFF) * brightness / 255,
            (pixelColor & 0xFF) * brightness / 255
        ));
    }
}

void HeartRateFlowVisualizer::show() {
    pixels.show();
}

void HeartRateFlowVisualizer::updateFlowPosition(unsigned long currentMillis) {
    float flowSpeed = rateOfChange * FLOW_SPEED_FACTOR;
    flowPosition += flowSpeed;
    if (flowPosition >= pixels.numPixels()) flowPosition -= pixels.numPixels();
    if (flowPosition < 0) flowPosition += pixels.numPixels();
}

uint32_t HeartRateFlowVisualizer::getColorForPixel(int pixelIndex, float flowCenter) {
    float distance = abs(pixelIndex - flowCenter);
    if (distance > pixels.numPixels() / 2) {
        distance = pixels.numPixels() - distance;
    }
    
    uint32_t leadColor, trailColor;
    if (rateOfChange > 0) {
        leadColor = pixels.Color(255, 0, 0);  // Red for increasing
        trailColor = pixels.Color(0, 255, 0); // Green for trailing
    } else {
        leadColor = pixels.Color(0, 255, 0);  // Green for decreasing
        trailColor = pixels.Color(255, 0, 0); // Red for trailing
    }
    
    float ratio = distance / (pixels.numPixels() / 4.0);
    ratio = constrain(ratio, 0, 1);
    
    return interpolateColor(leadColor, trailColor, ratio);
}

uint32_t HeartRateFlowVisualizer::interpolateColor(uint32_t color1, uint32_t color2, float ratio) {
    uint8_t r1 = (color1 >> 16) & 0xFF, r2 = (color2 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF, g2 = (color2 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF, b2 = color2 & 0xFF;
    
    uint8_t r = r1 + (r2 - r1) * ratio;
    uint8_t g = g1 + (g2 - g1) * ratio;
    uint8_t b = b1 + (b2 - b1) * ratio;
    
    return pixels.Color(r, g, b);
}

uint8_t HeartRateFlowVisualizer::getPixelBrightness(int pixelIndex, float flowCenter) {
    float distance = abs(pixelIndex - flowCenter);
    if (distance > pixels.numPixels() / 2) {
        distance = pixels.numPixels() - distance;
    }
    
    float brightnessRatio = 1 - (distance / (pixels.numPixels() / 2));
    brightnessRatio = brightnessRatio * brightnessRatio; // Square for more pronounced effect
    
    return MIN_BRIGHTNESS + (MAX_BRIGHTNESS - MIN_BRIGHTNESS) * brightnessRatio;
}