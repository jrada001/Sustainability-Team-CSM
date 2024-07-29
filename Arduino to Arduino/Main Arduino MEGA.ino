
void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(53, OUTPUT);
  pinMode(52, OUTPUT);


}

void loop() {
  //read the pushbutton value into a variable
  int RFID_A_1 = digitalRead(2);
  int RFID_A_2 = digitalRead(3);
  //print out the value of the pushbutton
  Serial.println(RFID_A_1);
  delay(300);
  Serial.println(RFID_A_2);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (RFID_A_1 == HIGH) {
    digitalWrite(53, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(53, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (RFID_A_2 == HIGH) {
    digitalWrite(52, LOW);

  } else {
    digitalWrite(52, HIGH);
  }
}
