
#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); // TX, RX respetively
String readdata;

void setup()
{
    BT.begin(9600);
    Serial.begin(9600);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
}
//-----------------------------------------------------------------------//
void loop()
{
    while (BT.available())
    {                       // Check if there is an available byte to read
        delay(10);          // Delay added to make thing stable
        char c = BT.read(); // Conduct a serial read
        // int servopo3 = BT.read();
        readdata = c; // build the string- "forward", "reverse", "left" and "right"
    }
    if (readdata.length() > 0)
    {
        Serial.println(readdata);

        if (readdata == "u")
        {
            digitalWrite(3, HIGH);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);
            delay(100);
        }

        else if (readdata == "d")
        {
            digitalWrite(3, LOW);
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            digitalWrite(6, LOW);
            delay(100);
        }

        else if (readdata == "r")
        {
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);
            delay(100);
        }

        else if (readdata == "l")
        {
            digitalWrite(3, HIGH);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            delay(100);
        }

        else if (readdata == "p")
        {
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            delay(100);
        }

        readdata = "";
    }
} // Reset the variable