const int ecgPin = A0;
int upperThreshold = 100;
int lowerThreshold = 0;
int ecgOffset = 400;
float beatsPerMinute = 0.0;
bool alreadyPeaked = false;
unsigned long firstPeakTime = 0;
unsigned long secondPeakTime = 0;
unsigned long rrInterval = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 
  int ecgReading = analogRead(ecgPin) - ecgOffset; 
  // Measure the ECG reading minus an offset to bring it into the same
  // range as the heart rate (i.e. around 60 to 100 bpm)

  if (ecgReading > upperThreshold && alreadyPeaked == false) { 
  // Check if the ECG reading is above the upper threshold and that
  // we aren't already in an existing peak
      if (firstPeakTime == 0) {
        // If this is the very first peak, set the first peak time
        firstPeakTime = millis();
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        // Otherwise set the second peak time and calculate the 
        // R-to-R interval. Once calculated we shift the second
        // peak to become our first peak and start the process
        // again
        secondPeakTime = millis();
        rrInterval = secondPeakTime - firstPeakTime;
        firstPeakTime = secondPeakTime;
        digitalWrite(LED_BUILTIN, HIGH);
      }
      alreadyPeaked = true;
  }

  if (ecgReading < lowerThreshold) {
  // Check if the ECG reading has fallen below the lower threshold
  // and if we are ready to detect another peak
    alreadyPeaked = false;
    digitalWrite(LED_BUILTIN, LOW);
  }  

  // Calculate the beats per minute, rrInterval is measured in
  // milliseconds so we must multiply by 1000
  beatsPerMinute = (1.0/rrInterval) * 60.0 * 1000;

  // Print the final values to be read by the serial plotter
  Serial.print(ecgReading);
  Serial.print(",");         
  Serial.println(beatsPerMinute);        
  delay(5);
}