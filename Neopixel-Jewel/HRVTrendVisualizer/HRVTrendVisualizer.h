#ifndef HRV_TREND_VISUALIZER_H
#define HRV_TREND_VISUALIZER_H

#include <Adafruit_NeoPixel.h>

class HRVTrendVisualizer {
public:
    // Constructor
    HRVTrendVisualizer(uint8_t pin, uint16_t numPixels);

    // Initialize the NeoPixel
    void begin();

    // Update the LED display based on new HRV data
    void update(float currentHRV);

    // Show the current LED state
    void show();

    // Configurable constants
    static constexpr uint8_t HRV_HISTORY_SIZE = 6;  // Number of historical HRV values to store (excluding current)
    static constexpr float MIN_HRV = 20.0;  // Minimum expected HRV value
    static constexpr float MAX_HRV = 200.0; // Maximum expected HRV value

private:
    Adafruit_NeoPixel pixels;
    float hrvHistory[HRV_HISTORY_SIZE];
    uint8_t historyIndex;

    // Helper functions
    void updateHistory(float newHRV);
    uint32_t getColorForHRV(float hrv);
    void setLEDColor(uint8_t index, uint32_t color);
};

#endif