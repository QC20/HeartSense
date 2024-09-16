#ifndef HRV_PULSE_MODULATOR_H
#define HRV_PULSE_MODULATOR_H

#include <Adafruit_NeoPixel.h>

class HRVPulseModulator {
public:
    // Constructor
    HRVPulseModulator(uint8_t pin, uint16_t numPixels);

    // Initialize the NeoPixel
    void begin();

    // Update the LED pulsing based on new HRV data
    void update(float currentHRV, unsigned long currentMillis);

    // Show the current LED state
    void show();

    // Configurable constants
    static constexpr float MIN_HRV = 20.0;   // Minimum expected HRV value (ms)
    static constexpr float MAX_HRV = 200.0;  // Maximum expected HRV value (ms)
    static constexpr unsigned long MIN_PULSE_DURATION = 500;   // Minimum pulse duration (ms)
    static constexpr unsigned long MAX_PULSE_DURATION = 3000;  // Maximum pulse duration (ms)
    static constexpr uint8_t MAX_BRIGHTNESS = 255;  // Maximum LED brightness

private:
    Adafruit_NeoPixel pixels;
    float currentHRV;
    unsigned long pulseDuration;
    unsigned long lastPulseTime;
    bool pulseState;

    // Helper functions
    void updatePulseDuration(float hrv);
    uint8_t calculateBrightness(unsigned long currentMillis);
    void setAllLEDs(uint8_t brightness);
};

#endif