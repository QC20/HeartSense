#ifndef AFTERGLOW_HEARTSENSE_EMULATOR_H
#define AFTERGLOW_HEARTSENSE_EMULATOR_H

#include <Adafruit_NeoPixel.h>

class HeartSenseEmulator {
public:
    HeartSenseEmulator(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(unsigned long currentMillis, int currentHeartRate);
    void setAllLEDs(int brightness);
    void show();

    static const uint16_t CYCLE_DURATION = 800;  // 75 bpm

private:
    static const uint8_t BASELINE_BRIGHTNESS = 5;
    static const uint8_t P_WAVE_BRIGHTNESS = 20;
    static const uint8_t QRS_PEAK_BRIGHTNESS = 65;
    static const uint8_t T_WAVE_BRIGHTNESS = 30;
    static const uint8_t U_WAVE_BRIGHTNESS = 10;

    static const uint8_t MAX_RECENT_RATES = 10;
    static const uint16_t BASE_AFTERGLOW_DURATION = 50; // milliseconds
    static const float AFTERGLOW_SCALE_FACTOR = 2.0;
    static const float CHANGE_THRESHOLD = 0.05; // 5%

    Adafruit_NeoPixel pixels;
    unsigned long lastUpdateTime;
    uint16_t currentStep;
    int recentHeartRates[MAX_RECENT_RATES]; // Fixed-size array
    uint8_t recentRateCount; // To keep track of the number of elements in the array
    unsigned long lastHeartRateUpdateTime;
    int currentAfterglowDuration;
    bool isAfterglowActive;
    unsigned long afterglowStartTime;

    int getHeartSenseBrightness(uint16_t step, uint16_t totalSteps);
    void updateHeartRate(int newHeartRate, unsigned long currentMillis);
    float calculateAverageHeartRate();
    bool isRWavePortion(uint16_t step, uint16_t totalSteps);
};

#endif