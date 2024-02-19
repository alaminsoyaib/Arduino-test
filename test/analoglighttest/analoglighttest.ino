int ledPin = 4; // Digital pin connected to your LED

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Your main code can control the LED brightness
  analogWrite(ledPin, 255); // Full brightness
  delay(1000);

  analogWrite(ledPin, 0); // Turn off the LED
  delay(1000);
}
