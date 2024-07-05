#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

// PINS
const int startb = 13;
const int endb = 12;
const int resetb = 11; // Add a new button for reset

const int rfrst_A1 = 1; // rfid1 - rst pin - Board A
const int rfss_A1 = 2;  // rfid1 - sda pin - Board A
const int rfrst_A2 = 3; // rfid2 - rst pin - Board A
const int rfss_A2 = 4;  // rfid2 - sda pin - Board A
const int rfrst_B1 = 5; // rfid1 - rst pin - Board B
const int rfss_B1 = 6;  // rfid1 - sda pin - Board B

const int rl_A1 = 21; //  relay 1 - Board A
const int rl_A2 = 22; //  relay 2 - Board A
const int rl_B1_in1 = 23; // relay 1 - In 1 - Board B

const int rl_B1_in2 = 24; // relay 1 - In 2 - Board B
const int rl_B1_in3 = 25; // relay 1 - In 3 - Board B
const int rl_B1_in4 = 26; // relay 1 - In 4 - Board B


// Cards
byte dirtyEnergyUID1[] = {0xD3, 0xF7, 0x43, 0x10};  // Red Card 1: D3 F7 43 10
byte dirtyEnergyUID2[] = {0xA3, 0x5A, 0xBB, 0x10};  // Red Card 2: A3 5A BB 10
byte cleanEnergyUID1[] = {0xB3, 0x60, 0x45, 0x10};  // Green Card 1: B3 60 45 10
byte cleanEnergyUID2[] = {0x43, 0x5E, 0x44, 0x10};  // Green Card 2: 43 5E 44 10

// Timer
unsigned long bpresstime = 0;
bool cdstart = false;
unsigned long elapsedTime = 0;
const unsigned long cdduration = 20000;

// RFID
MFRC522 mfrc522_1(rfss_A1, rfrst_A1);
MFRC522 mfrc522_2(rfss_A2, rfrst_A2);
MFRC522 mfrc522_3(rfss_B1, rfrst_B1);

// Debounce
bool resetPressed = false;
unsigned long lastResetPressTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  // Set up for RFID 1
  SPI.begin();
  mfrc522_1.PCD_Init();
  mfrc522_1.PCD_SetAntennaGain(mfrc522_1.RxGain_max);
  delay(4);
  mfrc522_1.PCD_DumpVersionToSerial();
  Serial.println("START MFRC522 1");

  // Set up for RFID 2
  mfrc522_2.PCD_Init();
  mfrc522_2.PCD_SetAntennaGain(mfrc522_2.RxGain_max);
  delay(4);
  mfrc522_2.PCD_DumpVersionToSerial();
  Serial.println("START MFRC522 2");

  // Set up for RFID 3
  mfrc522_3.PCD_Init();
  mfrc522_3.PCD_SetAntennaGain(mfrc522_3.RxGain_max);
  delay(4);
  mfrc522_3.PCD_DumpVersionToSerial();
  Serial.println("START MFRC522 3");

  // Set up for LEDs
  pinMode(rl_B1_in1, OUTPUT);
  pinMode(rl_B1_in2, OUTPUT);
  pinMode(rl_B1_in3, OUTPUT);
  pinMode(rl_B1_in4, OUTPUT);
  pinMode(rl_A1, OUTPUT);
  pinMode(rl_A2, OUTPUT);

  // Initialize LEDs to initial state
  resetLEDs();

  // Set up for Buttons
  pinMode(startb, INPUT_PULLUP);
  pinMode(endb, INPUT_PULLUP);
  pinMode(resetb, INPUT_PULLUP); // Initialize reset button

  // Initialize Serial
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  // Debounce Reset Button
  if (digitalRead(resetb) == HIGH) {
    if (!resetPressed) {
      unsigned long currentTime = millis();
      if (currentTime - lastResetPressTime > debounceDelay) {
        resetSystem();
        resetPressed = true;
        lastResetPressTime = currentTime;
      }
    }
  } else {
    resetPressed = false;
  }

  // Start Button
  if (digitalRead(startb) == HIGH) {
    bpresstime = millis();
    cdstart = true;
    Serial.println("Countdown Start...");
  }

  // Counting Down
  if (cdstart) {
    elapsedTime = millis() - bpresstime;
    unsigned long remainingTime = cdduration - elapsedTime;
    Serial.print("Countdown: ");
    Serial.print(remainingTime / 1000);
    Serial.println(" seconds");

    // Stop Countdown when reach zero
    if (elapsedTime >= cdduration) {
      cdstart = false;
      Serial.println("Times UP");
      // Call the function to check LED status
      checkLEDStatus();
    }

    // Check if end button is pressed on time
    if (digitalRead(endb) == HIGH && elapsedTime < cdduration) {
      // RFID Starts Scanning
      rfidStart();
      checkLEDStatus();
      cdstart = false;
    }
  }

  // If end button not pressed on time
  if (elapsedTime >= cdduration && !cdstart) {
    cdstart = false;
    digitalWrite(rl_B1_in1, LOW);
    digitalWrite(rl_B1_in2, LOW);
    digitalWrite(rl_B1_in3, LOW);
    digitalWrite(rl_B1_in4, LOW);
    digitalWrite(rl_A1, HIGH);
    digitalWrite(rl_A2, HIGH);
  }
}

// RFID starts scanning
void rfidStart() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial()) {
    // Dirty Cards
    if (compareUID(mfrc522_1.uid.uidByte, dirtyEnergyUID1, mfrc522_1.uid.size) || compareUID(mfrc522_1.uid.uidByte, dirtyEnergyUID2, mfrc522_1.uid.size)) {
      digitalWrite(rl_A1, LOW);

    }
    // Clean Cards
    else if (compareUID(mfrc522_1.uid.uidByte, cleanEnergyUID1, mfrc522_1.uid.size) || compareUID(mfrc522_1.uid.uidByte, cleanEnergyUID2, mfrc522_1.uid.size)) {
      digitalWrite(rl_A1, HIGH);
    }

  }

  if (mfrc522_2.PICC_IsNewCardPresent() && mfrc522_2.PICC_ReadCardSerial()) {
    // Dirty Cards
    if (compareUID(mfrc522_2.uid.uidByte, dirtyEnergyUID1, mfrc522_2.uid.size) || compareUID(mfrc522_2.uid.uidByte, dirtyEnergyUID2, mfrc522_2.uid.size)) {

      digitalWrite(rl_A2, LOW);
    }
    // Clean Cards
    else if (compareUID(mfrc522_2.uid.uidByte, cleanEnergyUID1, mfrc522_2.uid.size) || compareUID(mfrc522_2.uid.uidByte, cleanEnergyUID2, mfrc522_2.uid.size)) {

      digitalWrite(rl_A2, HIGH);
    }

  }
}

// Function to compare two UID arrays
bool compareUID(byte* uid1, byte* uid2, int size) {
  for (int i = 0; i < size; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

// Function to check LED status
void checkLEDStatus() {
  Serial.println("Checking LED Status:");
  Serial.print("Board B Relay1 In1: ");
  Serial.println(digitalRead(rl_B1_in1));
  Serial.print("Board B Relay 1 In2: ");
  Serial.println(digitalRead(rl_B1_in2));
  Serial.print("Board B Relay1 In3: ");
  Serial.println(digitalRead(rl_B1_in3));
  Serial.print("Board B Relay1 In4: ");
  Serial.println(digitalRead(rl_B1_in4));
  Serial.print("Board A Relay1: ");
  Serial.println(digitalRead(rl_A1));
  Serial.print("Board A Relay2: ");
  Serial.println(digitalRead(rl_A2));
}

// Function to reset LEDs to initial state
void resetLEDs() {
  digitalWrite(rl_B1_in1, LOW);
  digitalWrite(rl_B1_in2, LOW);
  digitalWrite(rl_B1_in3, LOW);
  digitalWrite(rl_B1_in4, LOW);
  digitalWrite(rl_A1, HIGH);
  digitalWrite(rl_A2, HIGH);
}

// Function to reset the system
void resetSystem() {
  bpresstime = 0;
  cdstart = false;
  elapsedTime = 0;
  resetLEDs();
  Serial.println("System has been reset.");
}
