int buzzerPin = 3; // Digital pin connected to your LED

void setup()
{
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{

  for (int tempi = 0; tempi < 3; tempi++)
  {
    tone(buzzerPin, 1000, 500); // play 1000Hz tone for 500ms
    delay(200);                 // wait for a second
    tone(buzzerPin, 2000, 500); // play 2000Hz tone for 500ms
    delay(200);                 // wait for a second
  }

  delay(2000); // wait for a second

  for (unsigned char i = 0; i < 80; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(1); // wait for 1ms
    digitalWrite(buzzerPin, LOW);
    delay(1); // wait for 1ms
  }
  // output another frequency
  for (unsigned char i = 0; i < 100; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(2); // wait for 2ms
    digitalWrite(buzzerPin, LOW);
    delay(2); // wait for 2ms
  }
  for (int i = 0; i < 80; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(1); // wait for 1ms
    digitalWrite(buzzerPin, LOW);
    delay(1); // wait n control the LED brightness
  }
  delay(2000); // wait for a second
}
