const int vibrationPin = 6;
const int baseIntensity = 65;     // Lowered to create more contrast
const int peakIntensity = 255;   // Maximum intensity for the peak of each beat
const int beatsPerMinute = 125;  // Maintaining 100 BPM

void setup() {
  pinMode(vibrationPin, OUTPUT);
}

void loop() {
  int beatDuration = 60000 / beatsPerMinute; // Duration of one complete beat cycle in milliseconds
  int pulseDuration = 150;  // Shortened pulse duration for more pronounced effect

  // First pulse ("lub")
  analogWrite(vibrationPin, peakIntensity);
  delay(pulseDuration);
  analogWrite(vibrationPin, baseIntensity);
  delay(30);  // Brief pause between "lub" and "dub"

  // Second pulse ("dub")
  analogWrite(vibrationPin, peakIntensity);
  delay(pulseDuration);
  analogWrite(vibrationPin, baseIntensity);

  // Rest for the remainder of the beat cycle
  delay(beatDuration - (2 * pulseDuration) - 30);
}