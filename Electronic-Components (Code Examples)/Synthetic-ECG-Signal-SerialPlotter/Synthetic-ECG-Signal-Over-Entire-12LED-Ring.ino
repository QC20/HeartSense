#include <Adafruit_NeoPixel.h>

#define PIN 6            // Pin connected to the LED strip
#define NUMPIXELS 12     // Number of LEDs in the strip
#define EKG_SIZE 100     // Size of the synthetic ECG data array

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Synthetic ECG data array, should include P wave, QRS complex, T wave, U wave
const double ekg[EKG_SIZE] = {
  // Fill with synthetic data that represents the P, QRS, T, and U waves
};

unsigned long lastUpdateTime = 0; // Time of the last update
int currentStep = 0;              // Current step in the ECG data array
const int updateInterval = 10;    // Interval between updates (in milliseconds)

void setup() {
  pixels.begin();                  // Initialize the LED strip
  pixels.setBrightness(16);        // Set brightness to a low level
  pixels.show();                   // Initialize the LED strip to off
  
  Serial.begin(115200);            // Initialize Serial communication for debugging
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time
  
  // Check if it's time to update the LEDs
  if (currentMillis - lastUpdateTime >= updateInterval) {
    updateLEDs();                    // Update the LED display
    currentStep = (currentStep + 1) % EKG_SIZE;  // Move to the next data point
    lastUpdateTime = currentMillis;  // Update the last update time
  }
  
  Serial.println(ekg[currentStep], 4);  // Output the current ECG value for debugging
}

void updateLEDs() {
  // Calculate the minimum and maximum EKG values
  double minVal = getMinEKGValue();
  double maxVal = getMaxEKGValue();
  
  // Clear LEDs first
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off LED
  }

  // Update LED colors based on the current EKG values
  for (int i = 0; i < NUMPIXELS; i++) {
    int index = (currentStep + i) % EKG_SIZE;   // Calculate the index for the current LED
    double ekgValue = ekg[index];               // Get the ECG value for this index
    
    int brightness = mapECGtoBrightness(ekgValue, minVal, maxVal); // Map ECG value to brightness
    pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));      // Set LED color to red with calculated brightness
  }
  pixels.show();  // Update the LED strip
}

double getMinEKGValue() {
  // Find the minimum value in the ECG data array
  double minVal = ekg[0];
  for (int i = 1; i < EKG_SIZE; i++) {
    if (ekg[i] < minVal) minVal = ekg[i];
  }
  return minVal;
}

double getMaxEKGValue() {
  // Find the maximum value in the ECG data array
  double maxVal = ekg[0];
  for (int i = 1; i < EKG_SIZE; i++) {
    if (ekg[i] > maxVal) maxVal = ekg[i];
  }
  return maxVal;
}

int mapECGtoBrightness(double ekgValue, double minVal, double maxVal) {
  // Normalize the ECG value to a range of 0 to 1
  double normalized = (ekgValue - minVal) / (maxVal - minVal);
  
  // Calculate brightness with emphasis on peaks
  int brightness = (int)(155 * pow(normalized, 1.50)); // Adjust the exponent to highlight peaks
  return brightness;
}
