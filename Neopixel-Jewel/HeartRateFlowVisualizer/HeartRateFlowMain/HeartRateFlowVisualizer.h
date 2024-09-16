#ifndef HEART_RATE_FLOW_VISUALIZER_H
#define HEART_RATE_FLOW_VISUALIZER_H

#include <Adafruit_NeoPixel.h>

class HeartRateFlowVisualizer {
public:
    HeartRateFlowVisualizer(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(float newHR, unsigned long currentMillis);
    void show();

    // Configurable constants
    static constexpr float MIN_HR = 40.0;
    static constexpr float MAX_HR = 180.0;
    static constexpr float MAX_RATE_OF_CHANGE = 20.0;
    static constexpr uint8_t MIN_BRIGHTNESS = 20;
    static constexpr uint8_t MAX_BRIGHTNESS = 255;
    static constexpr float FLOW_SPEED_FACTOR = 0.02;

private:
    Adafruit_NeoPixel pixels;
    float currentHR, prevHR, rateOfChange;
    float flowPosition;
    uint32_t baseColor;

    void updateFlowPosition(unsigned long currentMillis);
    uint32_t getColorForPixel(int pixelIndex, float flowCenter);
    uint32_t interpolateColor(uint32_t color1, uint32_t color2, float ratio);
    uint8_t getPixelBrightness(int pixelIndex, float flowCenter);
};

#endif