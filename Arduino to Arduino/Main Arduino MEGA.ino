

void setup() 
{
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);

  
  pinMode(53, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);

  // digitalWrite(2, LOW);
  // digitalWrite(3, LOW);
  // digitalWrite(4, LOW);
  // digitalWrite(5, LOW);
  // digitalWrite(6, HIGH);
  // digitalWrite(7, HIGH);
  // digitalWrite(8, HIGH);
  // digitalWrite(9, HIGH);

}

void loop() {
  //read the pushbutton value into a variable
  bool RFID_E&F_1_G = digitalRead(2);
  bool RFID_E&F_1_R = digitalRead(3);
  bool RFID_E&F_2_G = digitalRead(4);
  bool RFID_E&F_2_R = digitalRead(5); 
  bool RFID_E&F_3_G = digitalRead(6);
  bool RFID_E&F_3_R = digitalRead(7); 

  //print out the value of the pushbutton

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:

  //GREEN
  if (RFID_A_1 == 1) 
  {
    Serial.print("RFID_A_1 pin 53: ");
    Serial.println(RFID_A_1);
    digitalWrite(53, HIGH);
  }
  else if (RFID_A_1 == 0) 
  {
    Serial.print("RFID_A_1 pin 53: ");
    Serial.println(RFID_A_1);
    digitalWrite(53, LOW); 
  } 
    
    
  
  //RED
  if (RFID_A_2 == 1) {
    digitalWrite(51, HIGH);
    } 
    else if (RFID_A_2 == 0) { 
    digitalWrite(51, LOW); 
  }  
  //GREEN
  if (RFID_A_3 == 1) {
    digitalWrite(49, HIGH);           
    } 
    else if (RFID_A_3 == 0){
    digitalWrite(49, LOW);           
    
  }
  //RED
  if (RFID_A_4 == 1) {
    digitalWrite(47, HIGH);
    } 
    else if (RFID_A_4 == 0){
    digitalWrite(47, LOW); 
    
  }


}
