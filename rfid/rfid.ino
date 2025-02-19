#include <SPI.h>
#include <MFRC522.h>

// Buzzer issue on code

#define SS_PIN 10
#define RST_PIN 9
int buzzerPin = 8;
int LED = 2;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

int unlock[] = {147, 237, 03, 23}; // This is the stored UID (Unlock Card)
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
    Serial.println(int(unlock[i]));

    if (!(int(rfid.uid.uidByte[i]) == int(unlock[i])))
    {
      match = false;
    }

    i++;
  }

  digitalWrite(buzzerPin, HIGH); // Turn on the buzzer

  Serial.println("Buzzing");
  delay(200);                   // Make a sound for 200 milliseconds
  digitalWrite(buzzerPin, LOW); // Turn off the buzzer

  if (match)
  {
    Serial.println("\n*** Unlocked ***");
    Serial.println("\nWelcome Al-Amin");
    delay(200);
    digitalWrite(LED, HIGH);
    delay(2000);
    digitalWrite(LED, LOW);
  }
  else
  {
    Serial.println("\nUnknown Card");
    delay(200);                    // Make a sound for 200 milliseconds
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    digitalWrite(LED, HIGH);
    Serial.println("Buzzing as unknown card, red LED");
    delay(2000);                  // Wait for 200 milliseconds before repeating
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    digitalWrite(LED, LOW);
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
