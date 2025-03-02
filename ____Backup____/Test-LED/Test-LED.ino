#define LED 2

void setup()
{
    pinMode(LED, OUTPUT);

    // for (size_t i = 0; i < 5; i++)
    // {

    // }
}

void loop()
{
    digitalWrite(LED, HIGH);
    delay(2000);
    digitalWrite(LED, LOW);
    delay(2000);
}