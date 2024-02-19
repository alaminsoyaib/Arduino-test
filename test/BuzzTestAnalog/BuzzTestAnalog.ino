int buzzerPin = A0;  // Change to an analog pin

void setup() {
  pinMode(buzzerPin, OUTPUT);  // Not strictly necessary for analog pins
}

void loop() {
  analogWrite(buzzerPin, 127);  // Apply a medium voltage to the buzzer
  delay(100);                   // Hold for 100 milliseconds
  analogWrite(buzzerPin, 0);    // Turn off the buzzer
  delay(100);                   // Wait for 100 milliseconds
}
