#include <Adafruit_NeoPixel.h>

#define PIN        6  // The pin connected to the NeoPixel ring
#define NUMPIXELS  12 // Number of LEDs in the ring

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Brightness levels for different parts of the ECG
#define BASELINE_BRIGHTNESS 5
#define P_WAVE_BRIGHTNESS 20
#define QRS_PEAK_BRIGHTNESS 65
#define T_WAVE_BRIGHTNESS 30
#define U_WAVE_BRIGHTNESS 10

void setup() {
  pixels.begin();
  pixels.setBrightness(15);
}

void loop() {
  showECGWave(800);  // 800ms per complete cycle (75 beats per minute)
}

void showECGWave(int cycleDuration) {
  int steps = 100;
  int stepDuration = cycleDuration / steps;

  for (int i = 0; i < steps; i++) {
    int brightness = getECGBrightness(i, steps);
    setLEDs(brightness, i, steps);
    pixels.show();
    delay(stepDuration);
  }
}

int getECGBrightness(int step, int totalSteps) {
  if (step < totalSteps * 0.10) {  // P wave
    return map(step, 0, totalSteps * 0.10, BASELINE_BRIGHTNESS, P_WAVE_BRIGHTNESS);
  } else if (step < totalSteps * 0.16) {  // PR segment
    return map(step, totalSteps * 0.10, totalSteps * 0.16, P_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
  } else if (step < totalSteps * 0.20) {  // Q wave
    return map(step, totalSteps * 0.16, totalSteps * 0.20, BASELINE_BRIGHTNESS, BASELINE_BRIGHTNESS - 5);
  } else if (step < totalSteps * 0.24) {  // R wave
    return map(step, totalSteps * 0.20, totalSteps * 0.24, BASELINE_BRIGHTNESS - 5, QRS_PEAK_BRIGHTNESS);
  } else if (step < totalSteps * 0.28) {  // S wave
    return map(step, totalSteps * 0.24, totalSteps * 0.28, QRS_PEAK_BRIGHTNESS, BASELINE_BRIGHTNESS - 10);
  } else if (step < totalSteps * 0.36) {  // ST segment
    return map(step, totalSteps * 0.28, totalSteps * 0.36, BASELINE_BRIGHTNESS - 10, BASELINE_BRIGHTNESS);
  } else if (step < totalSteps * 0.56) {  // T wave
    float progress = (step - totalSteps * 0.36) / (totalSteps * 0.20);
    return BASELINE_BRIGHTNESS + (T_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(progress * PI);
  } else if (step < totalSteps * 0.64) {  // U wave
    float progress = (step - totalSteps * 0.56) / (totalSteps * 0.08);
    return BASELINE_BRIGHTNESS + (U_WAVE_BRIGHTNESS - BASELINE_BRIGHTNESS) * sin(progress * PI);
  } else {  // Back to baseline
    return BASELINE_BRIGHTNESS;
  }
}

void setLEDs(int brightness, int step, int totalSteps) {
  int ledIndex = map(step, 0, totalSteps, 0, NUMPIXELS);
  for (int i = 0; i < NUMPIXELS; i++) {
    if (i == ledIndex) {
      pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
    } else {
      pixels.setPixelColor(i, pixels.Color(BASELINE_BRIGHTNESS, 0, 0));
    }
  }
}