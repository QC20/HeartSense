/*
  Heart Signal LED Ring Mapping

  This code simulates an Electrocardiogram (ECG) signal and maps the synthetic ECG data to a NeoPixel LED ring.
  The LED brightness is adjusted based on the simulated ECG signal, with peaks and troughs visually represented
  as changes in LED brightness.

  Key Features:
  - Uses a predefined array of synthetic ECG values.
  - Updates the LED ring every 10ms to match the synthetic data rate.
  - Maps the ECG values to LED brightness, with more pronounced changes to highlight peaks.
  - Sends current ECG values to the Serial Monitor for debugging and visualization.

  How to Use:
  - Connect a NeoPixel LED ring to pin 6 of your Arduino.
  - Upload this code to your Arduino board.
  - Open the Serial Monitor in the Arduino IDE (set baud rate to 115200) to view the ECG values.
  - Observe the LED ring for visual representation of the synthetic ECG signal.
*/

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 12
#define EKG_SIZE 100

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Synthetic ECG data
const double ekg[EKG_SIZE] = {
    0.19136, 0.19136, 0.27337, 0.41006, 0.54675, 0.74282, 2.0672, 4.8761, 9.1102, 13.701,
    18.291, 22.881, 27.472, 30.226, 29.92, 26.554, 21.963, 17.373, 12.782, 8.1921,
    4.2015, 1.7103, 0.7185, 0.62638, 0.53427, 0.44215, 0.35004, 0.25792, 0.16581, -0.9667,
    -6.2608, -14.335, -14.306, -1.4965, 22.712, 48.475, 71.736, 76.651, 60.719, 26.441,
    -10.339, -44.249, -57.116, -46.071, -20.956, -4.5927, 0.149, 0.24213, 0.33526, 0.42839,
    0.52151, 0.61464, 0.70777, 0.80089, 0.89402, 0.98715, 1.0803, 1.1734, 1.2665, 1.3597,
    1.4528, 1.5459, 1.9248, 3.0181, 4.8258, 7.0621, 9.2985, 11.535, 13.771, 15.65,
    16.097, 15.053, 12.877, 10.64, 8.404, 6.1676, 4.0046, 2.3792, 1.3647, 0.88781,
    0.48964, 0.1937, 0.032284, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

unsigned long lastUpdateTime = 0;
int currentStep = 0;

void setup()
{
  pixels.begin();
  pixels.setBrightness(16); // Set to 6.25% brightness (0-255)
  pixels.show();

  // Initialize Serial communication for the Serial Plotter
  Serial.begin(115200);
}

void loop()
{
  unsigned long currentMillis = millis();

  // Update LEDs every 10ms to match the original synthetic data rate
  if (currentMillis - lastUpdateTime >= 10)
  {
    updateLEDs();
    currentStep = (currentStep + 1) % EKG_SIZE;
    lastUpdateTime = currentMillis;
  }

  // Send the current ECG value to the Serial Plotter
  Serial.println(ekg[currentStep], 4);
}

void updateLEDs()
{
  // Map the ECG value to LED brightness with more pronounced changes
  int brightness = mapECGtoBrightness(ekg[currentStep]);

  // Reduce the intensity of the red color
  int redIntensity = brightness / 2; // Scale down the brightness to make red dimmer

  // Set all LEDs to the calculated brightness
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(redIntensity, 0, 0)); // Dim red color
  }

  pixels.show();
}

int mapECGtoBrightness(double ekgValue)
{
  // Find the min and max values in the ekg array
  double minVal = ekg[0];
  double maxVal = ekg[0];
  for (int i = 1; i < EKG_SIZE; i++)
  {
    if (ekg[i] < minVal)
      minVal = ekg[i];
    if (ekg[i] > maxVal)
      maxVal = ekg[i];
  }

  // Apply a more pronounced mapping to highlight peaks and troughs
  double scaledValue = (ekgValue - minVal) / (maxVal - minVal);
  int brightness = (int)(255 * pow(scaledValue, 1.50)); // Square the value to emphasize peaks
  // int brightness = (int)(255 * pow(scaledValue, 2.45)); // Highest value to emphasize peaks

  // Apply a softer lower threshold to ensure minimal brightness during idle periods
  if (brightness < 10)
  {
    brightness = 1; // Keep LEDs on but very dim during idle
  }

  return brightness;
}
