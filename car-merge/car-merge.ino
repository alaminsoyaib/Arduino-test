#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// RFID setup
#define SS_PIN 10 // SDA
#define RST_PIN 9 // RST
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
int unlock[] = {67, 109, 178, 14}; // Stored UID (Unlock Card)
int buzzerPin = 8;
int LED = 2;

// Bluetooth setup
SoftwareSerial BT(6, 7); // TX, RX respectively
String readdata;

// Car control pins
#define Left_F 2
#define Left_B 3
#define Right_F 4
#define Right_B 5

bool carUnlocked = false; // Flag to check if the car is unlocked

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    BT.begin(9600);
    pinMode(buzzerPin, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(Left_F, OUTPUT);
    pinMode(Left_B, OUTPUT);
    pinMode(Right_F, OUTPUT);
    pinMode(Right_B, OUTPUT);
    Serial.println("---Project is ready---\n");
}

void loop()
{
    if (rfid.PICC_IsNewCardPresent())
    {
        readRFID();
    }
    if (carUnlocked)
    {
        controlCar();
    }
    delay(100);
}

void readRFID()
{
    rfid.PICC_ReadCardSerial();
    Serial.print(F("\nPICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

    Serial.println("Scanned PICC's UID:");
    printDec(rfid.uid.uidByte, rfid.uid.size);

    bool match = true;
    for (int i = 0; i < rfid.uid.size; i++)
    {
        if (int(rfid.uid.uidByte[i]) != unlock[i])
        {
            match = false;
            break;
        }
    }

    if (match)
    {
        if (carUnlocked)
        {
            Serial.println("\n*** Locked ***");
            carUnlocked = false;
            digitalWrite(LED, LOW);
        }
        else
        {
            Serial.println("\n*** Unlocked ***");
            Serial.println("\nWelcome Al-Amin");
            carUnlocked = true;
            digitalWrite(LED, HIGH);
            indicateSuccess();
            delay(1000);
            digitalWrite(LED, LOW);
        }
    }
    else
    {
        Serial.println("\nUnknown Card");
        indicateError();
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

void indicateError()
{
    for (int tempi = 0; tempi < 3; tempi++)
    {
        digitalWrite(LED, HIGH);
        tone(buzzerPin, 1000, 500);
        delay(200);
        tone(buzzerPin, 2000, 500);
        digitalWrite(LED, LOW);
        delay(200);
    }
}

void indicateSuccess()
{
    for (unsigned char i = 0; i < 80; i++)
    {
        digitalWrite(buzzerPin, HIGH);
        delay(1);
        digitalWrite(buzzerPin, LOW);
        delay(1);
    }
    for (unsigned char i = 0; i < 100; i++)
    {
        digitalWrite(buzzerPin, HIGH);
        delay(2);
        digitalWrite(buzzerPin, LOW);
        delay(2);
    }
}

void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
    Serial.println();
}

void controlCar()
{
    while (BT.available())
    {
        delay(10);
        char c = BT.read();
        readdata = c;
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
}

void stop()
{
    digitalWrite(Left_F, LOW);
    digitalWrite(Left_B, LOW);
    digitalWrite(Right_F, LOW);
    digitalWrite(Right_B, LOW);
    Serial.println("Stopping\n");
    delay(100);
}