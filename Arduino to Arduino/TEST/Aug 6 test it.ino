#include <SPI.h>
#include <MFRC522.h>

#define RELAY_1  A0
#define RELAY_2  A1
#define RELAY_3  A2
#define RELAY_4  A3

#define RST_PIN   9          
#define SS_PIN_1  10
#define SS_PIN_2  11
#define SS_PIN_3  12      

#define NR_OF_READERS 3

byte ssPins[] = {SS_PIN1, SS_PIN2, SS_PIN3};

MFRC522 rfid[NR_OF_READERS];   // Create MFRC522 instance.

const byte GREEN_CARD[4] = {99, 90, 150, 13};
const byte RED_CARD[4] = {115, 181, 132, 13};
const byte GREEN_CARD8[4] = {179, 96, 69, 16};
const byte RED_CARD7[4] = {195, 197, 85, 16};

void setup() 
{
	Serial.begin(9600);	// Initialize serial communications with the PC
	while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin(); // Init SPI bus
	
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
    {
    rfid[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("RFID Reader #: "));
    Serial.print(reader);
    Serial.print(F(": "));
    rfid[reader].PCD_DumpVersionToSerial();
    }
    
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode(RELAY3, OUTPUT);
    pinMode(RELAY4, OUTPUT);
}

bool locked = false;

void loop()
{
    for (int i = 0; i < 4; i++) {
    if (PICC_IsAnyCardPresent(rfid[i])) {
      if (rfid[i].PICC_ReadCardSerial()) {
        locked = true;
        

        if (CheckCardUID(rfid[0].uid.uidByte, GREEN_CARD)) {
          Serial.println("RFID1 GREEN");
          digitalWrite(RELAY1, HIGH);
        } else if (CheckCardUID(rfid[0].uid.uidByte, RED_CARD)) {
          Serial.println("RFID1 RED");
          digitalWrite(RELAY2, HIGH);
        } else {
          digitalWrite(RELAY1, LOW);
          digitalWrite(RELAY2, LOW);
        }

        if (CheckCardUID(rfid[1].uid.uidByte, GREEN_CARD8)) {
          digitalWrite(RELAY3, HIGH);
          Serial.println("RFID2 GREEN");
        } else if (CheckCardUID(rfid[1].uid.uidByte, RED_CARD7)) {
          digitalWrite(RELAY4, HIGH);
          Serial.println("RFID2 RED");
        } else {
          digitalWrite(RELAY3, LOW);
          digitalWrite(RELAY4, LOW);
        }


        rfid[i].PICC_HaltA();
        rfid[i].PCD_StopCrypto1();
        break; // Exit loop after detecting a card
      }
    }
  }
  if (!locked) {
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    digitalWrite(RELAY3, LOW);
    digitalWrite(RELAY4, LOW);
  }

  delay(200);
}



void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(((buffer[i]) >> 4) & 0x0F, HEX);
    Serial.print(buffer[i] & 0x0F, HEX);
    Serial.print(" ");
  }
}

bool CheckCardUID(byte *buffer, byte* cardUID) {
  for (byte i = 0; i < 4; i++) {
    if (buffer[i] != cardUID[i]) {
      return false;
    }
  }
  Serial.println(F("Card Match."));
  return true;
}

bool PICC_IsAnyCardPresent(MFRC522 &rfid) {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
  
  rfid.PCD_WriteRegister(rfid.TxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.RxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.ModWidthReg, 0x26);
  
  MFRC522::StatusCode result = rfid.PICC_WakeupA(bufferATQA, &bufferSize);
  return (result == MFRC522::STATUS_OK || result == MFRC522::STATUS_COLLISION);
}


