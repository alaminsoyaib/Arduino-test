#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// RFID setup
#define SS_PIN 10 // SDA
#define RST_PIN 9 // RST
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
int unlockCode[] = {67, 109, 178, 14}; // Stored UID for Unlocking
int lockCode[] = {147, 237, 3, 23};    // Stored UID for Unlocking
int buzzerPin = 8;
int LED = A0;

// Bluetooth setup
SoftwareSerial BT(6, 7); // TX, RX
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
    controlCar(); // Always call controlCar to read Bluetooth input
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

    if (isCardMatch(rfid.uid.uidByte, unlockCode))
    {
        if (carUnlocked)
        {
            indicateAlreadyUnlocked();
        }
        else
        {
            indicateSuccess();
        }
    }
    else if (isCardMatch(rfid.uid.uidByte, lockCode))
    {
        if (carUnlocked)
        {
            indicateLock();
        }
        else
        {
            indicateAlreadyLocked();
        }
    }
    else
    {
        Serial.println("\nUnknown Card");
        UnknownCard();
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

bool isCardMatch(byte *card, int *code)
{
    for (int i = 0; i < 4; i++)
    {
        if (int(card[i]) != code[i])
        {
            return false;
        }
    }
    return true;
}

void UnknownCard()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED, HIGH);
        tone(buzzerPin, 1000, 500);
        digitalWrite(LED, LOW);
        delay(200);
        digitalWrite(LED, HIGH);
        tone(buzzerPin, 2000, 500);
        digitalWrite(LED, LOW);
        delay(200);
        digitalWrite(LED, HIGH);
        delay(200);
        digitalWrite(LED, LOW);
    }
}

void indicateSuccess()
{
    readdata = "";
    Serial.println("\n*** Unlocked ***\n");
    carUnlocked = true;
    digitalWrite(LED, HIGH);
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
    delay(200);
    digitalWrite(LED, LOW);
}

void indicateLock()
{
    readdata = "";
    Serial.println("\n*** Locked ***\n");
    carUnlocked = false;
    digitalWrite(LED, HIGH);
    for (unsigned char i = 0; i < 80; i++)
    {
        digitalWrite(buzzerPin, HIGH);
        delay(2);
        digitalWrite(buzzerPin, LOW);
        delay(2);
    }
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
    digitalWrite(LED, HIGH);
    for (unsigned char i = 0; i < 100; i++)
    {
        digitalWrite(buzzerPin, HIGH);
        delay(1);
        digitalWrite(buzzerPin, LOW);
        delay(1);
    }
    delay(50);
    digitalWrite(LED, LOW);
}

void indicateAlreadyLocked()
{
    readdata = "";
    digitalWrite(LED, HIGH);
    Serial.println("\n*** Already locked ***");
    tone(buzzerPin, 500, 300);
    delay(100);
    digitalWrite(LED, LOW);
    delay(50);
    digitalWrite(LED, HIGH);
    tone(buzzerPin, 500, 300);
    delay(100);
    digitalWrite(LED, LOW);
}

void indicateAlreadyUnlocked()
{
    readdata = "";
    digitalWrite(LED, HIGH);
    Serial.println("\n*** Already Unlocked ***");
    tone(buzzerPin, 1000, 300);
    delay(500);
    digitalWrite(LED, LOW);
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
    if (BT.available())
    {
        delay(10);
        char c = BT.read();
        readdata = c;

        if (readdata == "m") // unlock car
        {
            if (!carUnlocked)
            {
                Serial.println("\n*** BT Unlocked ***");
                indicateSuccess();
                return;
            }
            else
            {
                indicateAlreadyUnlocked();
                return;
            }
        }
        if (readdata == "n") // lock car
        {
            if (carUnlocked)
            {
                Serial.println("\n*** BT locked ***");
                indicateLock();
                return;
            }
            else
            {
                indicateAlreadyLocked();
                return;
            }
        }

        if (readdata.length() > 0 && !carUnlocked)
        {
            Serial.print("locked: ");
            Serial.println(readdata);
            tone(buzzerPin, 2100, 100); // play 2100Hz tone for 100ms
            tone(buzzerPin, 500, 100);  // play 500Hz tone for 100ms
            readdata = "";
            return;
        }
    }

    if (readdata.length() > 0)
    {
        Serial.print(readdata);

        if (readdata == "f") // front
        {
            digitalWrite(LED, HIGH);
            tone(buzzerPin, 1500, 100); // play 1500Hz tone for 100ms
            tone(buzzerPin, 200, 100);  // play 200Hz tone for 100ms
            digitalWrite(Left_F, HIGH);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, HIGH);
            digitalWrite(Right_B, LOW);
            Serial.print(" - Going Forward");
        }
        else if (readdata == "b") // back
        {
            digitalWrite(LED, HIGH);
            tone(buzzerPin, 1500, 100); // play 1500Hz tone for 100ms
            tone(buzzerPin, 200, 100);  // play 200Hz tone for 100ms
            digitalWrite(Left_F, LOW);
            digitalWrite(Left_B, HIGH);
            digitalWrite(Right_F, LOW);
            digitalWrite(Right_B, HIGH);
            Serial.print(" - Going Backward");
        }
        else if (readdata == "l") // left
        {
            digitalWrite(LED, HIGH);
            tone(buzzerPin, 1500, 100); // play 1500Hz tone for 100ms
            tone(buzzerPin, 200, 100);  // play 200Hz tone for 100ms
            digitalWrite(Left_F, LOW);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, HIGH);
            digitalWrite(Right_B, LOW);
            Serial.print(" - Going Left");
        }
        else if (readdata == "r") // right
        {
            digitalWrite(LED, HIGH);
            tone(buzzerPin, 1500, 100); // play 1500Hz tone for 100ms
            tone(buzzerPin, 200, 100);  // play 200Hz tone for 100ms
            digitalWrite(Left_F, HIGH);
            digitalWrite(Left_B, LOW);
            digitalWrite(Right_F, LOW);
            digitalWrite(Right_B, LOW);
            Serial.print(" - Going Right");
        }
        else if (readdata == "x")
        {
            digitalWrite(LED, LOW);
            tone(buzzerPin, 1500, 100); // play 1500Hz tone for 100ms
            tone(buzzerPin, 200, 100);  // play 200Hz tone for 100ms
            stop();
        }

        readdata = "";
        Serial.println();
        // Serial.println("***Memory Cleared***\n");
    }
}

void stop()
{
    digitalWrite(Left_F, LOW);
    digitalWrite(Left_B, LOW);
    digitalWrite(Right_F, LOW);
    digitalWrite(Right_B, LOW);
    Serial.println(" - Stopping");
    // delay(100);
}
