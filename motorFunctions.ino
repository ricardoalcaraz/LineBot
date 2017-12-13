
/*---------------------------------------------------------------------------------------
 * Function to stop motor functions
 *
 */
void stop(){
	digitalWrite(STBY, LOW);
}


/*---------------------------------------------------------------------------------------
 * Function to continue motor functions
 *
 */
void go(){
	digitalWrite(STBY, HIGH);
}



/*---------------------------------------------------------------------------------------
 * Move the robot forward
 * Sets the appropriate motors pins so the driver can tell the motor to move forward
 * NOTES: OCR4B is right motor and OCR4D is left motor
 * INPUTS: uint8_t, desired motor speed from 0 - 255
 * OUTPUTS: None
 */
void moveForward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  	OCR4D = leftMotorSpeed+leftOffset;
  	OCR4B = rightMotorSpeed+rightOffset;
}


/*---------------------------------------------------------------------------------------
 * Move the robot backwards
 * Sets the appropriate motors pins so the driver can tell the motor to move forward
 * INPUTS: uint16_t, desired motor speed from 0 - 255
 * OUTPUTS: None
 */
void moveBackward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed){
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  	OCR4D = leftMotorSpeed+leftOffset;
  	OCR4B = rightMotorSpeed+rightOffset;
}





/*---------------------------------------------------------------------------------------
 * Function to the turn the robot right through a tank turn
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: uint16_t speed
 * OUTPUTS: None
 */
void tankTurnRight(uint8_t leftSpeed, uint8_t rightSpeed){
	/*Setting Right motor to move back so it pivots in place*/
	digitalWrite(AIN1, HIGH);
	digitalWrite(AIN2, LOW);
	/*Setting left motor to move forward*/
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	/*Setting the turning speed*/
	OCR4B = rightSpeed+rightOffset;
	OCR4D = leftSpeed+leftOffset;
//	delay(tank_turn_delay);
}




/*
 * Function to turn the robot left through a tank turn
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: None
 * Outputs: None
 */
void tankTurnLeft(uint8_t motorSpeed){
	/*Setting Right motor to move back so it pivots in place*/
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	/*Setting left motor to move forward*/
	digitalWrite(BIN1, HIGH);
	digitalWrite(BIN2, LOW);
	/*Setting the turning speed*/
	OCR4B = motorSpeed+rightOffset;
	OCR4D = motorSpeed+leftOffset;
}






/*---------------------------------------------------------------------------------------
 * Function to turn the robot left
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: None
 * Outputs: None
 */
void arcTurnLeft(uint8_t motorSpeed){
  /*Disabling Left motor so robot can pivot*/
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(PWMB, HIGH);
  /*Setting right motor to move forward*/
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  /*Setting the turning speed*/
  OCR4B = motorSpeed;
  digitalWrite(STBY, HIGH);
  delay(arc_turn_delay);
}





/*---------------------------------------------------------------------------------------
 * Function to the turn the robot right
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: uint16_t speed
 * OUTPUTS: None
 */
void arcTurnRight(uint8_t motorSpeed){
  /*Disabling Right motor so robot can pivot*/
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  /*Setting right motor to move forward*/
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  /*Setting the turning speed*/
  OCR4D = motorSpeed;
}

/*Turn right using the encoders as feedback
 * Turning was implemented by counting ticks between different states
 * INPUTS: None
 * OUTPUTS: None
 */
void rotaryRight() {
  digitalWrite(LED, HIGH);
  while( IR_data[backLeftData] < 500 || IR_data[backRightData] < 500 ) {
    lineFollow();
  }
  stop();
  delay(1000);
  noInterrupts();
  uint8_t rightCounter = 0;
  uint8_t leftCounter = 0;
  uint8_t currentStateLeft = PINB & 0x0A;
  uint8_t prevStateLeft = currentStateLeft;
  uint8_t currentStateRight = PIND & 0x03;
  uint8_t prevStateRight = currentStateRight;
  tankTurnRight(maxSpeed, maxSpeed);
  while(rightCounter < 20 || leftCounter < 18 ) {
    currentStateRight = PIND & 0x03;
    currentStateLeft = PINB & 0x0A;
    if(currentStateRight != prevStateRight) rightCounter++;
    if(currentStateLeft != prevStateLeft) leftCounter++;
    prevStateRight = currentStateRight;
    prevStateLeft = currentStateLeft;
    delay(100);
    go();
  }
  interrupts();
  stop();
  moveForward(maxSpeed, maxSpeed);
  delay(1000);
  digitalWrite(LED, LOW);
}

/*Turn left using the encoders as feedback
 * Turning was implemented by counting ticks between different states
 * INPUTS: None
 * OUTPUTS: None
 */
void rotaryLeft() {
  digitalWrite(LED, HIGH);
  while( IR_data[backLeftData] < 500 || IR_data[backRightData] < 500 ) {
    lineFollow();
  }
  stop();
  delay(1000);
  noInterrupts();
  uint8_t rightCounter = 0;
  uint8_t leftCounter = 0;
  uint8_t currentStateLeft = PINB & 0x0A;
  uint8_t prevStateLeft = currentStateLeft;
  uint8_t currentStateRight = PIND & 0x03;
  uint8_t prevStateRight = currentStateRight;
  tankTurnLeft(maxSpeed);
  while(rightCounter < 19 || leftCounter < 18 ) {
    currentStateRight = PIND & 0x03;
    currentStateLeft = PINB & 0x0A;
    if(currentStateRight != prevStateRight) rightCounter++;
    if(currentStateLeft != prevStateLeft) leftCounter++;
    prevStateRight = currentStateRight;
    prevStateLeft = currentStateLeft;
    delay(100);
    go();
  }
  interrupts();
  stop();
  moveForward(maxSpeed, maxSpeed);
  delay(1000);
  digitalWrite(LED, LOW);
}

/*Turn around using the encoders as feedback
 * Turning was implemented by counting ticks between different states
 * INPUTS: None
 * OUTPUTS: None
 */
void rotaryTurnAround() {
digitalWrite(LED, HIGH);
  noInterrupts();
  stop();
  delay(1000);
  uint8_t rightCounter = 0;
  uint8_t leftCounter = 0;
  uint8_t currentStateLeft = PINB & 0x0A;
  uint8_t prevStateLeft = currentStateLeft;
  uint8_t currentStateRight = PIND & 0x03;
  uint8_t prevStateRight = currentStateRight;
  tankTurnRight(maxSpeed, maxSpeed);
  go();
  while(rightCounter < 40 || leftCounter < 39 ) {
    currentStateRight = PIND & 0x03;
    currentStateLeft = PINB & 0x0A;
    if(currentStateRight != prevStateRight) rightCounter++;
    if(currentStateLeft != prevStateLeft) leftCounter++;
    prevStateRight = currentStateRight;
    prevStateLeft = currentStateLeft;
    delay(100);
    go();
  }
  stop();
  interrupts();
  moveForward(maxSpeed, maxSpeed);
  digitalWrite(LED, LOW);
}

/**Basic Line following state machine
 * Does rudimentary error correction by taking into account last state that the bot was in
 * INPUTS: None
 * OUTPUTS: None
 */
void lineFollow() {
  static uint8_t prevState = 0;
  uint8_t newState;
  if( IR_data[frontLeftData] > 500 && IR_data[frontRightData] > 500 ) {
    newState = prevState;
  } else if( IR_data[frontLeftData] < 500 && IR_data[frontRightData] < 500 ) {
    newState = 2;
  } else if( IR_data[frontLeftData] > 500 && IR_data[frontRightData] < 500 ) {
    newState = 3;
  } else if( IR_data[frontLeftData] < 500 && IR_data[frontRightData] > 500 ) { 
    newState = 4;
  }
  if(prevState != newState) {
    switch (newState) {
      case 1: moveForward(maxSpeed,maxSpeed);
          break;
      case 2: if(prevState == 3) {
              moveForward( maxSpeed+20, maxSpeed-10);
              delay(95);
          } else if(prevState == 4) {
              moveForward( maxSpeed-10, maxSpeed+20);
              delay(95);
          }
          stop();
          moveForward(maxSpeed, maxSpeed);
          go();
          digitalWrite(LED, LOW);
          break;
      case 3: moveForward(maxSpeed, maxSpeed+10);
          break;
      case 4: moveForward(maxSpeed+10, maxSpeed);
          break;
    }
    prevState = newState;
  }
  go();
}

