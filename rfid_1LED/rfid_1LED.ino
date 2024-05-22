#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
int buzzerPin = 3;
int LED = 2;
// int ledPin = 4;
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

int code[] = {67, 109, 178, 14}; // This is the stored UID (Unlock Card)
int codeRead = 0;
String uidString;

void setup()
{
  Serial.begin(9600);
  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  Serial.println(F("Stamford University Robotics Club"));

  pinMode(buzzerPin, OUTPUT);
  pinMode(LED, OUTPUT);
  // pinMode(ledPin, OUTPUT);
}

void loop()
{
  if (rfid.PICC_IsNewCardPresent())
  {
    readRFID();
  }
  delay(100);
}

void readRFID()
{
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    for (int tempi = 0; tempi < 5; tempi++)
    {
      tone(buzzerPin, 2200, 500); // play 1000Hz tone for 500ms
      digitalWrite(LED, HIGH);
      delay(200);                 // wait for a second
      tone(buzzerPin, 1800, 500); // play 2000Hz tone for 500ms
      digitalWrite(LED, LOW);
      delay(200); // wait for a second
    }
    return;
  }

  Serial.println("Scanned PICC's UID:");
  printDec(rfid.uid.uidByte, rfid.uid.size);
  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);

  int i = 0;
  boolean match = true;

  while (i < rfid.uid.size)
  {
    Serial.println(int(rfid.uid.uidByte[i]));
    Serial.println(int(code[i]));

    if (!(int(rfid.uid.uidByte[i]) == int(code[i])))
    {
      match = false;
    }

    i++;
  }

  if (match)
  {
    Serial.println("\n*** Unlocked ***");
    Serial.println("\nWelcome Al-Amin");
    digitalWrite(LED, HIGH);
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
    delay(1000);
    digitalWrite(LED, LOW);
  }
  else
  {
    Serial.println("\nUnknown Card");
    Serial.println("Buzzing as unknown card, red LED");

    for (int tempi = 0; tempi < 3; tempi++)
    {
      digitalWrite(LED, HIGH);
      tone(buzzerPin, 1000, 500); // play 1000Hz tone for 500ms
      delay(200);                 // wait for a second
      tone(buzzerPin, 2000, 500); // play 2000Hz tone for 500ms
      digitalWrite(LED, LOW);
      delay(200); // wait for a second
    }
  }

  Serial.println("============================");

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
