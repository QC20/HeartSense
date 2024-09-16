#ifndef PULSE_WAVE_VISUALIZER_H
#define PULSE_WAVE_VISUALIZER_H

#include <Adafruit_NeoPixel.h>

class PulseWaveVisualizer {
public:
    PulseWaveVisualizer(uint8_t pin, uint16_t numPixels);
    void begin();
    void update(float newHR, unsigned long currentMillis);
    void show();

    // Configurable constants
    static constexpr float MIN_HR = 40.0;
    static constexpr float MAX_HR = 180.0;
    static constexpr float RAPID_CHANGE_THRESHOLD = 10.0;
    static constexpr unsigned long MIN_BREATH_DURATION = 333;  // 180 bpm
    static constexpr unsigned long MAX_BREATH_DURATION = 1500; // 40 bpm
    static constexpr uint8_t MIN_BRIGHTNESS = 20;
    static constexpr uint8_t MAX_BRIGHTNESS = 255;
    static constexpr uint8_t FLUTTER_AMPLITUDE = 30;
    static constexpr uint8_t FLUTTER_FREQUENCY = 10;

private:
    Adafruit_NeoPixel pixels;
    float currentHR, prevHR, rateOfChange;
    unsigned long breathDuration, lastBreathTime;
    bool breathState;
    uint32_t baseColor;

    uint8_t calculateBreathBrightness(unsigned long currentMillis);
    void setAllLEDs(uint8_t brightness);
    uint32_t applyBrightness(uint32_t color, uint8_t brightness);
    uint8_t applyFlutter(uint8_t brightness, unsigned long currentMillis);
};

#endif