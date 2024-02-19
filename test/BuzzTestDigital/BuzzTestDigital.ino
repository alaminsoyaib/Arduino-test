int buzzerPin = 3;

void setup() {
   pinMode(buzzerPin, OUTPUT);
}

void loop() {
  digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
  delay(500);                     // Make a sound for 500 milliseconds
  digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
  delay(500);                     // Wait for 500 milliseconds before repeating 

  //tone(6, 1000, 500);
}
