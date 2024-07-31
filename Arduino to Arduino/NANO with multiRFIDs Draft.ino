
#include <SPI.h>
#include <MFRC522.h>

#define RELAY1   A0
#define RELAY2   A1
#define RELAY3   A2
#define RELAY4   A3

#define RST_PIN         9          
#define SS_PIN1          2
#define SS_PIN2          3
#define SS_PIN3          4
#define SS_PIN4          5      

// MFRC522 rfid1(SS_PIN1, RST_PIN);
// MFRC522 rfid2(SS_PIN2, RST_PIN);
// MFRC522 rfid3(SS_PIN3, RST_PIN);
// MFRC522 rfid4(SS_PIN4, RST_PIN);

const int MaxRfidCount = 4;
MFRC522* rfid[MaxRfidCount] = {new MFRC522(SS_PIN1, RST_PIN), new MFRC522(SS_PIN2, RST_PIN), new MFRC522(SS_PIN3,)};

byte rfidStatus [MaxRfidCount] = {0,0,0,0};

const byte GREEN_CARD[4] = {99, 90, 150, 13};
const byte RED_CARD[4] = {115, 181, 132, 13};




void setup() {
	Serial.begin(9600);		          // Initialize serial communications with the PC
	while (!Serial);		            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			              // Init SPI bus
	rfid1.PCD_Init();		            // Init MFRC522
	delay(4);				                // Optional delay. Some board do need more time after init to be ready, see Readme
	rfid1.PCD_DumpVersionToSerial();
  for (int i = 0; i < MaxRfidCount; i++)
  {
    rfid[i].PCD_Init();
    delay(4);
    rfid[i].PCD_DumpVersionToSerial();
  }
  
  	// Show details of PCD - MFRC522 Card Reader details
  rfid2.PCD_Init();		            // Init MFRC522
	delay(4);				                // Optional delay. Some board do need more time after init to be ready, see Readme
	rfid2.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  rfid3.PCD_Init();		            // Init MFRC522
	delay(4);				                // Optional delay. Some board do need more time after init to be ready, see Readme
	rfid3.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  rfid4.PCD_Init();		            // Init MFRC522
	delay(4);				                // Optional delay. Some board do need more time after init to be ready, see Readme
	rfid4.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details

	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
}

bool locked = false;


void loop() {
  bool cardPresent = PICC_IsAnyCardPresent(rfid1);
  
  if (! locked && ! cardPresent)
    return;

  MFRC522::StatusCode result = rfid1.PICC_Select(&rfid1.uid,8*rfid1.uid.size);

  if(!locked && result == MFRC522::STATUS_OK)
  {
    locked=true;
    // Action on card detection.
    Serial.print(F("locked! NUID tag: "));
    printHex(rfid1.uid.uidByte, rfid1.uid.size);
    Serial.println();

    for (int i=0; i < MaxRfidCount; i++)
    {
      rfidStatus[i]=0;
    }
    if (CheckCardUID(rfid[0].uid.uidByte, GREEN_CARD)) {
      digitalWrite(RELAY1, HIGH);
      rfidStatus[0]=1;
    } else if (CheckCardUID(rfid1.uid.uidByte, RED_CARD)) {
      digitalWrite(RELAY2, HIGH);
    } else {
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
    }
  } else if(locked && result != MFRC522::STATUS_OK)
  {
    locked=false;
    rfid1.uid.size = 0;
    // Action on card removal.
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);

    Serial.print(F("unlocked! Reason for unlocking: "));
    Serial.println(rfid1.GetStatusCodeName(result));
  } else if(!locked && result != MFRC522::STATUS_OK)
  {
    rfid1.uid.size = 0;
  }
  rfid1.PICC_HaltA();
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(((buffer[i])>>4)&0x0F,  HEX);
    Serial.print(buffer[i]&0x0F, HEX);
    Serial.print(" ");
  }
}

bool CheckCardUID(byte *buffer, byte* cardUID) {
  bool match = false;
   if (buffer[0] == cardUID[0] &&
    buffer[1] == cardUID[1] &&
    buffer[2] == cardUID[2] &&
    buffer[3] == cardUID[3] )
    {
      Serial.println(F("Card Match."));
      return true;
    }

    return false;
}

bool PICC_IsAnyCardPresent(MFRC522 rfid) {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
  
  // Reset baud rates
  rfid.PCD_WriteRegister(rfid.TxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.RxModeReg, 0x00);
  // Reset ModWidthReg
  rfid.PCD_WriteRegister(rfid.ModWidthReg, 0x26);
  
  MFRC522::StatusCode result = rfid.PICC_WakeupA(bufferATQA, &bufferSize);
  return (result == MFRC522::STATUS_OK || result == MFRC522::STATUS_COLLISION);
} // End PICC_IsAnyCardPresent()
