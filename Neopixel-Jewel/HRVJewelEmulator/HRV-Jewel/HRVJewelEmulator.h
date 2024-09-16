#ifndef HRV_JEWEL_EMULATOR_H
#define HRV_JEWEL_EMULATOR_H

#include <Adafruit_NeoPixel.h>

class HRVJewelEmulator {
public:
    HRVJewelEmulator(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(unsigned long currentMillis, float currentHRV);
    void setLEDs(int brightness);
    void show();

    // Configurable constants
    static constexpr uint8_t BASELINE_BRIGHTNESS = 5;
    static constexpr uint8_t MAX_BRIGHTNESS = 65;
    static constexpr uint8_t WINDOW_SIZE = 10;
    static constexpr uint16_t BASE_AFTERGLOW_DURATION = 50; // milliseconds
    static constexpr float AFTERGLOW_SCALE_FACTOR = 2.0;
    static constexpr float BRIGHTNESS_SCALE = 0.925;
    static constexpr float CENTER_LED_FACTOR = 2.0; // Center LED intensity factor

    // Sensitivity controls
    static constexpr unsigned long HRV_UPDATE_INTERVAL = 1500; // milliseconds
    static constexpr float SIGNIFICANT_CHANGE_THRESHOLD = 0.05; // 5%
    static constexpr float MAX_AFTERGLOW_DURATION_FACTOR = 3.0;

    // Methods to adjust sensitivity at runtime
    void setHRVUpdateInterval(unsigned long interval) { hrvUpdateInterval = interval; }
    void setSignificantChangeThreshold(float threshold) { significantChangeThreshold = threshold; }
    void setMaxAfterglowDurationFactor(float factor) { maxAfterglowDurationFactor = factor; }

private:
    Adafruit_NeoPixel pixels;
    unsigned long lastUpdateTime;
    float recentHRVs[WINDOW_SIZE];
    uint8_t recentHRVsIndex;
    unsigned long lastHRVUpdateTime;
    int currentAfterglowDuration;
    bool isAfterglowActive;
    unsigned long afterglowStartTime;
    float movingAverage;
    float intensityModulationFactor;
    float lastHRV;

    // Runtime-adjustable sensitivity parameters
    unsigned long hrvUpdateInterval;
    float significantChangeThreshold;
    float maxAfterglowDurationFactor;

    int calculateBrightness(float hrv);
    void updateHRV(float newHRV, unsigned long currentMillis);
    void updateMovingAverage(float newHRV);
    bool isSignificantChange(float newHRV);
    int applyIntensityModulation(int brightness);
};

#endif