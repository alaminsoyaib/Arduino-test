
#include <SoftwareSerial.h>
SoftwareSerial BT(6, 7); // TX, RX respetively
String readdata;

#define Left_F 2
#define Left_B 3
#define Right_F 4
#define Right_B 5

void stop()
{
    digitalWrite(Left_F, LOW);
    digitalWrite(Left_B, LOW);
    digitalWrite(Right_F, LOW);
    digitalWrite(Right_B, LOW);
    Serial.println("Stopping\n");
    delay(100);
}

void setup()
{
    BT.begin(9600);
    Serial.begin(9600);
    Serial.println("---Project is ready---\n");
    pinMode(Left_F, OUTPUT);
    pinMode(Left_B, OUTPUT);
    pinMode(Right_F, OUTPUT);
    pinMode(Right_B, OUTPUT);
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

        if (readdata == "f")
        {
            digitalWrite(Left_F, HIGH);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, HIGH);
            digitalWrite(Right_B, LOW);
            Serial.println("Going Forward\n");
            delay(1000);
            stop();
        }

        else if (readdata == "b")
        {
            digitalWrite(Left_F, LOW);
            digitalWrite(Left_B, HIGH);
            digitalWrite(Right_F, LOW);
            digitalWrite(Right_B, HIGH);
            Serial.println("Going Backward\n");
            delay(1000);
            stop();
        }

        else if (readdata == "l")
        {
            digitalWrite(Left_F, LOW);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, HIGH);
            digitalWrite(Right_B, LOW);
            Serial.println("Going Left\n");
            delay(500);
            stop();
        }

        else if (readdata == "r")
        {
            digitalWrite(Left_F, HIGH);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, LOW);
            digitalWrite(Right_B, LOW);
            Serial.println("Going Right\n");
            delay(500);
            stop();
        }

        else if (readdata == "x")
        {
            stop();
        }

        readdata = "";
        Serial.println("***Memory Cleared***\n");
    }
} // Reset the variable