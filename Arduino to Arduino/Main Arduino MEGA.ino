
void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(53, INPUT_PULLUP);
  pinMode(52, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
  pinMode(50, INPUT_PULLUP);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);



}

void loop() {
  //read the pushbutton value into a variable
  bool RFID_A_1 = digitalRead(53);
  bool RFID_A_2 = digitalRead(52);
  //print out the value of the pushbutton
  Serial.println(RFID_A_1);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
    if (RFID_A_1 == 1) {
    digitalWrite(2, HIGH); //GREEN LED bright black bulbs
    digitalWrite(3, LOW); //RED LED bright black bulbs
    digitalWrite(4, HIGH); //RED LED costco card
    digitalWrite(5, LOW); //GREEN LED costco card
  } 
  
    else {
    digitalWrite(2, LOW); 
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
  }
      if (RFID_A_2 == 1) {
    digitalWrite(6, LOW); 
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
  } 
  
    else {
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
  }
}
