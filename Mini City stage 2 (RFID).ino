/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          
#define SS_1_PIN        2         
#define SS_2_PIN        3         
#define SS_3_PIN        4
#define SS_4_PIN        23
#define SS_5_PIN        6
#define SS_6_PIN        7

#define NR_OF_READERS   6

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN, SS_6_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

const int RELAY4_4 = 8;
const int RELAY4_3 = 9;
const int RELAY4_2 = 10;
const int RELAY4_1 = 11; //connected
const int RELAY_R = 12; //connected
const int RELAY_L = 13; //connected

byte PICC_CARD1[4] = {99, 90, 150, 13};
byte PICC_CARD2[4] = {115, 181, 132, 13};
byte PICC_CARD3[4] = {227, 209, 104, 247};
byte PICC_CARD4[4] = {227, 209, 104, 247};
byte PICC_CARD5[4] = {227, 102, 184, 13};
byte PICC_CARD6[4] = {211, 50, 93, 16};
byte PICC_CARD7[4] = {195, 197, 85, 16};
byte PICC_CARD8[4] = {179, 96, 69, 16};
byte PICC_CARD9[4] = {147, 85, 37, 18};
byte PICC_CARD10[4] = {115, 70, 48, 16};
byte PICC_CARD11[4] = {179, 39, 96, 16};

byte greenCards[] = {PICC_CARD1, PICC_CARD2, PICC_CARD3, PICC_CARD4, PICC_CARD5};
byte redCards[] = {PICC_CARD6, PICC_CARD7, PICC_CARD8, PICC_CARD9, PICC_CARD10, PICC_CARD11};

void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  pinMode(RELAY4_4, OUTPUT);
  pinMode(RELAY4_3, OUTPUT);
  pinMode(RELAY4_2, OUTPUT);
  pinMode(RELAY4_1, OUTPUT);
  pinMode(RELAY_R, OUTPUT);
  pinMode(RELAY_L, OUTPUT);

  digitalWrite(RELAY4_1, LOW);
  digitalWrite(RELAY4_2, LOW);
  digitalWrite(RELAY4_3, HIGH);
  digitalWrite(RELAY_R, LOW);
  digitalWrite(RELAY_L, HIGH);

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/**
 * Main loop.
 */
void loop() {
  
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

 if(CheckCardUID(mfrc522[reader].uid.uidByte, PICC_CARD1))
   {
   digitalWrite(RELAY4_1, HIGH);
   Serial.print("Relay4_1: ");
   Serial.println(RELAY4_1);
   digitalWrite(RELAY4_2, HIGH);
   Serial.print("Relay4_2: ");
   Serial.println(RELAY4_2);
   }
   else if(CheckCardUID(mfrc522[reader].uid.uidByte, PICC_CARD2))
  {
   digitalWrite(RELAY_R, HIGH);
   digitalWrite(RELAY_L, LOW);
   digitalWrite(RELAY4_3, LOW);
   Serial.print("Relay4_3: ");
   Serial.println(RELAY4_3);
   Serial.print("Relay_Right: ");
   Serial.println(RELAY_R);
   Serial.print("Relay_Left: ");
   Serial.println(RELAY_L);



}
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
  
}


bool CheckCardUID(byte *buffer, byte* cardUID)
{
  bool match = false;
   if (buffer[0] == cardUID[0] &&
    buffer[1] == cardUID[1] &&
    buffer[2] == cardUID[2] &&
    buffer[3] == cardUID[3] )
    {
      Serial.println(F("Card Match."));
      match = true;
    }

    return match;
}
