#ifndef ECG_JEWEL_EMULATOR_H
#define ECG_JEWEL_EMULATOR_H

#include <Adafruit_NeoPixel.h>

class ImprovedECGEmulator {
public:
    ImprovedECGEmulator(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(unsigned long currentMillis, int currentHeartRate);
    void setAllLEDs(int brightness);
    void show();
    void clear();

    uint16_t getCycleDuration(int heartRate) const { return 60000 / heartRate; }

    // Configurable constants - reduced brightness levels for ambient use
    static constexpr uint8_t BASELINE_BRIGHTNESS = 0;  // Was 5, now 0 for no light when inactive
    static constexpr uint8_t P_WAVE_BRIGHTNESS = 8;    // Was 20, reduced to 8
    static constexpr uint8_t QRS_PEAK_BRIGHTNESS = 25; // Was 65, reduced to 25
    static constexpr uint8_t T_WAVE_BRIGHTNESS = 12;   // Was 30, reduced to 12
    static constexpr uint8_t WINDOW_SIZE = 10;
    static constexpr uint16_t BASE_AFTERGLOW_DURATION = 30; // Was 50ms, reduced to 30ms
    static constexpr float AFTERGLOW_SCALE_FACTOR = 1.5;    // Was 2.0, reduced to 1.5
    static constexpr float CHANGE_THRESHOLD = 0.08;         // Was 0.05, increased to 0.08 (8%)
    static constexpr uint8_t INTENSITY_MODULATION_BEATS = 2;// Was 3, reduced to 2
    static constexpr float INTENSITY_MODULATION_FACTOR = 0.03; // Was 0.05 (5%), reduced to 0.03 (3%)
    static constexpr float BRIGHTNESS_SCALE = 0.8;      // Was 0.925, reduced to 0.8

    // Sensitivity controls
    static constexpr unsigned long HEART_RATE_UPDATE_INTERVAL = 2000; // Was 1500ms, increased to 2000ms
    static constexpr float SIGNIFICANT_CHANGE_THRESHOLD = 0.08; // Was 0.05 (5%), increased to 0.08 (8%)
    static constexpr float MAX_AFTERGLOW_DURATION_FACTOR = 2.0; // Was 3.0, reduced to 2.0

    // Methods to adjust sensitivity at runtime
    void setHeartRateUpdateInterval(unsigned long interval) { heartRateUpdateInterval = interval; }
    void setSignificantChangeThreshold(float threshold) { significantChangeThreshold = threshold; }
    void setMaxAfterglowDurationFactor(float factor) { maxAfterglowDurationFactor = factor; }

private:
    Adafruit_NeoPixel pixels;
    unsigned long lastUpdateTime;
    uint16_t currentStep;
    int recentHeartRates[WINDOW_SIZE];
    uint8_t recentHeartRatesIndex;
    unsigned long lastHeartRateUpdateTime;
    int currentAfterglowDuration;
    bool isAfterglowActive;
    unsigned long afterglowStartTime;
    float movingAverage;
    bool isIntensityModulationActive;
    uint8_t intensityModulationBeatsRemaining;
    float intensityModulationFactor;
    int lastHeartRate;

    // Runtime-adjustable sensitivity parameters
    unsigned long heartRateUpdateInterval;
    float significantChangeThreshold;
    float maxAfterglowDurationFactor;

    int getECGBrightness(uint16_t step, uint16_t totalSteps);
    void updateHeartRate(int newHeartRate, unsigned long currentMillis);
    void updateMovingAverage(int newHeartRate);
    bool isSignificantChange(int newHeartRate);
    bool isRWavePortion(uint16_t step, uint16_t totalSteps) const;
    int applyIntensityModulation(int brightness);
};

#endif