#ifndef HRV_VARIABILITY_VISUALIZER_H
#define HRV_VARIABILITY_VISUALIZER_H

#include <Adafruit_NeoPixel.h>

class HRVVariabilityVisualizer {
public:
    // Constructor
    HRVVariabilityVisualizer(uint8_t pin, uint16_t numPixels);

    // Initialize the NeoPixel
    void begin();

    // Update the LED display based on new HRV data
    void update(float currentHRV);

    // Show the current LED state
    void show();

    // Configurable constants
    static constexpr float MIN_HRV = 20.0;   // Minimum expected HRV value (ms)
    static constexpr float MAX_HRV = 200.0;  // Maximum expected HRV value (ms)
    static constexpr uint8_t MAX_BRIGHTNESS = 255;  // Maximum LED brightness
    static constexpr uint8_t HRV_HISTORY_SIZE = 10; // Number of historical HRV values to store

private:
    Adafruit_NeoPixel pixels;
    float hrvHistory[HRV_HISTORY_SIZE];
    uint8_t historyIndex;

    // Helper functions
    void updateHistory(float newHRV);
    uint8_t calculateVariability();
    void setLEDs(uint8_t numLit);
    uint32_t getColorForHRV(float hrv);
};

#endif