#include <Adafruit_NeoPixel.h>

const int PulseWire = A0;
const int LED_PIN = 6;
const int NUM_LEDS = 12;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int Signal;
int Threshold = 565;  // Adjust this value as needed
int Peak = 550;
int Trough = 550;
int lastPeakTime = 0;
int BPM = 0;
const int SAMPLE_RATE = 10;  // 10ms between readings

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial communication started");

  ring.begin();
  ring.show();
  ring.setBrightness(50);

  Serial.println("Setup completed. Entering main loop...");
}

void loop() {
  static unsigned long lastReading = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastReading >= SAMPLE_RATE) {
    lastReading = currentTime;

    Signal = analogRead(PulseWire);
    Serial.println(Signal);  // Print raw signal for debugging

    // Update Peak and Trough
    Peak = max(Peak, Signal);
    Trough = min(Trough, Signal);

    // Dynamically adjust threshold
    Threshold = (Peak + Trough) / 2;

    // Beat detection
    if (Signal > Threshold && (currentTime - lastPeakTime > 300)) {
      ring.fill(ring.Color(255, 0, 0));  // Fill ring with red
      ring.show();
      
      BPM = 60000 / (currentTime - lastPeakTime);  // Calculate BPM
      lastPeakTime = currentTime;

      Serial.print("♥ Beat! BPM: ");
      Serial.println(BPM);

      delay(50);  // Keep LEDs on briefly
      ring.clear();
      ring.show();
    }

    // Reset Peak and Trough periodically
    if (currentTime % 5000 == 0) {  // Every 5 seconds
      Peak = Signal;
      Trough = Signal;
    }
  }
}