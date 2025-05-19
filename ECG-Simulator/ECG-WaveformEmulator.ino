#include <Adafruit_NeoPixel.h>

#define PIN        6  // The pin connected to the NeoPixel ring
#define NUMPIXELS  12 // Number of LEDs in the ring

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Brightness levels for different parts of the ECG (reduced to 1/3)
#define BASELINE_BRIGHTNESS 2
#define P_WAVE_BRIGHTNESS 13
#define QRS_PEAK_BRIGHTNESS 85
#define T_WAVE_BRIGHTNESS 33
#define U_WAVE_BRIGHTNESS 7

// Overall brightness control (0-255)
#define OVERALL_BRIGHTNESS 128

void setup() {
  pixels.begin();
  pixels.setBrightness(OVERALL_BRIGHTNESS);  // Set overall brightness
}

void loop() {
  // Simulate a heartbeat cycle at about 60 bpm
  showECGWave(1000);  // 1000ms per complete cycle (60 beats per minute)
}

void showECGWave(int cycleDuration) {
  int steps = 100;  // Number of steps in the animation
  int stepDuration = cycleDuration / steps;

  for (int i = 0; i < steps; i++) {
    int intensity = getECGIntensity(i, steps);
    setAllLEDs(intensity);
    pixels.show();
    delay(stepDuration);
  }
}

int getECGIntensity(int step, int totalSteps) {
  if (step < totalSteps * 0.15) {  // P wave
    return map(step, 0, totalSteps * 0.15, BASELINE_BRIGHTNESS, P_WAVE_BRIGHTNESS);
  } else if (step < totalSteps * 0.25) {  // PR segment
    return map(step, totalSteps * 0.15, totalSteps * 0.25, P_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
  } else if (step < totalSteps * 0.35) {  // QRS complex
    int qrsProgress = (step - totalSteps * 0.25) / (totalSteps * 0.1);
    if (qrsProgress < 0.5) {
      return map(qrsProgress * 2, 0, 1, BASELINE_BRIGHTNESS, QRS_PEAK_BRIGHTNESS);
    } else {
      return map((qrsProgress - 0.5) * 2, 0, 1, QRS_PEAK_BRIGHTNESS, BASELINE_BRIGHTNESS);
    }
  } else if (step < totalSteps * 0.65) {  // ST segment
    return BASELINE_BRIGHTNESS;
  } else if (step < totalSteps * 0.85) {  // T wave
    return map(step, totalSteps * 0.65, totalSteps * 0.85, BASELINE_BRIGHTNESS, T_WAVE_BRIGHTNESS);
  } else if (step < totalSteps * 0.9) {  // U wave
    return map(step, totalSteps * 0.85, totalSteps * 0.9, T_WAVE_BRIGHTNESS, U_WAVE_BRIGHTNESS);
  } else {  // Back to baseline
    return map(step, totalSteps * 0.9, totalSteps, U_WAVE_BRIGHTNESS, BASELINE_BRIGHTNESS);
  }
}

void setAllLEDs(int intensity) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(intensity, 0, 0));
  }
}