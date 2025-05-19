#ifndef ECG_EMULATOR_H
#define ECG_EMULATOR_H

#include <Adafruit_NeoPixel.h>

class ECGEmulator {
public:
    ECGEmulator(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(unsigned long currentMillis);

private:
    static const uint8_t BASELINE_BRIGHTNESS = 5;
    static const uint8_t P_WAVE_BRIGHTNESS = 20;
    static const uint8_t QRS_PEAK_BRIGHTNESS = 65;
    static const uint8_t T_WAVE_BRIGHTNESS = 30;
    static const uint8_t U_WAVE_BRIGHTNESS = 10;
    static const uint16_t CYCLE_DURATION = 800;  // 75 bpm

    Adafruit_NeoPixel pixels;
    unsigned long lastUpdateTime;
    uint16_t currentStep;

    int getECGBrightness(uint16_t step, uint16_t totalSteps);
    void setAllLEDs(int brightness);
};

#endif