#ifndef AFTERGLOW_JEWEL_EMULATOR_H
#define AFTERGLOW_JEWEL_EMULATOR_H

#include <Adafruit_NeoPixel.h>

class AfterglowJewelEmulator {
public:
    AfterglowJewelEmulator(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(unsigned long currentMillis, int currentRR);
    void setLEDs(int brightness);
    void show();

    // Calculate cycle duration based on current RR interval
    uint16_t getCycleDuration(int rrInterval) const { return rrInterval; }

    // Configurable constants
    static constexpr uint8_t BASELINE_BRIGHTNESS = 5;
    static constexpr uint8_t P_WAVE_BRIGHTNESS = 20;
    static constexpr uint8_t QRS_PEAK_BRIGHTNESS = 65;
    static constexpr uint8_t T_WAVE_BRIGHTNESS = 30;
    static constexpr uint8_t WINDOW_SIZE = 10;
    static constexpr uint16_t BASE_AFTERGLOW_DURATION = 50; // milliseconds
    static constexpr float AFTERGLOW_SCALE_FACTOR = 2.0;
    static constexpr float CHANGE_THRESHOLD = 0.05; // 5%
    static constexpr uint8_t INTENSITY_MODULATION_BEATS = 3;
    static constexpr float INTENSITY_MODULATION_FACTOR = 0.05; // 5%
    static constexpr float BRIGHTNESS_SCALE = 0.925;
    static constexpr float CENTER_LED_FACTOR = 2.0; // Center LED intensity factor

    // Sensitivity controls
    static constexpr unsigned long HRV_UPDATE_INTERVAL = 1500; // milliseconds
    static constexpr float SIGNIFICANT_CHANGE_THRESHOLD = 0.05; // 5%
    static constexpr float MAX_AFTERGLOW_DURATION_FACTOR = 3.0; // Maximum afterglow duration as a factor of BASE_AFTERGLOW_DURATION

    // Methods to adjust sensitivity at runtime
    void setHRVUpdateInterval(unsigned long interval) { hrvUpdateInterval = interval; }
    void setSignificantChangeThreshold(float threshold) { significantChangeThreshold = threshold; }
    void setMaxAfterglowDurationFactor(float factor) { maxAfterglowDurationFactor = factor; }

private:
    Adafruit_NeoPixel pixels;
    unsigned long lastUpdateTime;
    uint16_t currentStep;
    int recentRRIntervals[WINDOW_SIZE];
    uint8_t recentRRIntervalsIndex;
    unsigned long lastHRVUpdateTime;
    int currentAfterglowDuration;
    bool isAfterglowActive;
    unsigned long afterglowStartTime;
    float movingAverageHRV;
    bool isIntensityModulationActive;
    uint8_t intensityModulationBeatsRemaining;
    float intensityModulationFactor;
    int lastRRInterval;

    // Runtime-adjustable sensitivity parameters
    unsigned long hrvUpdateInterval;
    float significantChangeThreshold;
    float maxAfterglowDurationFactor;

    int getECGBrightness(uint16_t step, uint16_t totalSteps);
    void updateHRV(int newRRInterval, unsigned long currentMillis);
    void updateMovingAverageHRV(int newRRInterval);
    bool isSignificantChange(int newRRInterval);
    bool isRWavePortion(uint16_t step, uint16_t totalSteps) const;
    int applyIntensityModulation(int brightness);
    float calculateHRV();
};

#endif