// Simple solenoid test

#define PIN 3

byte val = 200; 

void setup() {
  pinMode(PIN, OUTPUT);
}

void loop() {
  analogWrite(PIN, val);
  delay(100);
  analogWrite(PIN, LOW);
  delay(900);
}
