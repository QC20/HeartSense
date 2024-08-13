const int ecgInput = A0;

void setup() {
  Serial.begin(115200);
}

void loop() { 
  int ecgReading = analogRead(ecgInput);
    
  Serial.println(ecgReading);    
  delay(5);
}