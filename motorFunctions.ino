
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
 * FSM turn right.
 * Function to implement an exact turn without implementing a machine dependent delay
 * Due to the placement of my sensor I was able to implement and FSM turn in two states
 */
void fsmRight(){
	uint8_t state = 0;
	go();
	do {
		lineFollow();
	} while(IR_data[backLeftData] < 500 || IR_data[backRightData] < 500);
	stop();
	delay(1000);
	tankTurnRight(maxSpeed, maxSpeed);
	go();
	while(IR_data[frontRightData] > 500);
	stop();
	delay(1000);
	go();
	while(IR_data[frontRightData] < 500);
	stop();
	delay(1000);
	go();
	while(IR_data[frontRightData] > 500);
	
/*
		stop();
		digitalWrite(LED, HIGH);
		delay(1000);
		do {
			if(IR_data[frontLeftData] < 500){
				moveBackward(0,maxSpeed);
			} else if(IR_data[frontRightData] < 500) {
				moveBackward(maxSpeed,0);
			} else {
				moveBackward(maxSpeed, maxSpeed);
			}
			go();
		} while( IR_data[frontRightData] > 500 || IR_data[frontLeftData] > 500);
		stop();
		delay(1000);
		do {	
			tankTurnRight(maxSpeed, maxSpeed);
			go();
		} while( (IR_data[frontRightData] < 700) || IR_data[frontLeftData] < 700  ); 
		stop();
		do {
			if(IR_data[frontLeftData] < 500){
				moveForward(0,maxSpeed);
			} else if(IR_data[frontRightData] < 500) {
				moveForward(maxSpeed,0);
			} else {
				moveForward(maxSpeed, maxSpeed);
			}
			go();
		} while(IR_data[frontRightData] > 500 || IR_data[frontLeftData] > 500); 
		digitalWrite(LED,LOW);
		stop();
		delay(1000);
*/
}





/*---------------------------------------------------------------------------------------
 * FSM turn right.
 * Function to implement an exact turn without implementing a machine dependent delay
 */
void fsmLeft(uint8_t motorSpeed, uint16_t IR_data[4]){
	/*Moving the bot into a good position to start the turn*/
	while(IR_data[frontLeftData] > 300 && IR_data[frontRightData] > 300){
		moveBackward(motorSpeed, motorSpeed);
	}
	/*Setting up for a tank turn*/
	stop();
	/*Setting Right motor to move backward*/
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	/*Setting Left motor to move forward*/
	digitalWrite(BIN1, HIGH);
	digitalWrite(BIN2, LOW);
	/*Setting the turning speed*/
	/*Initiate turn*/
	go();
	/*State 2 Read IR sensor data until the front two sensor are hitting white, the line follow will autocorrect if it's not perfectly centered*/
	do{
		//readIRSensors(IR_data);
	}while( IR_data[frontLeftData] < 300 && IR_data[frontRightData] < 300 );
}





/*---------------------------------------------------------------------------------------
 *Function to make the bot line follow
 */
void lineFollow(uint16_t IR_data[4], uint8_t maxSpeed) {
	  if(IR_data[backLeftData] < 300 && IR_data[backRightData] > 300){
		  moveForward(maxSpeed, maxSpeed+7);
	  } else if(IR_data[backLeftData] > 300 && IR_data[backRightData] < 300){
		  moveForward(maxSpeed+7, maxSpeed);
	  } else {
		  moveForward(maxSpeed, maxSpeed);
	  }
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

/*Function to turn right with rotary encoders
 * 
 */
const uint8_t turn_ticks = 30;
void rotaryRight() {
  noInterrupts();
  uint8_t rightCounter = 0;
  uint8_t leftCounter = 0;
  uint8_t currentStateLeft = 0;
  uint8_t prevStateLeft = 0;
  uint8_t currentStateRight = 0;
  uint8_t prevStateRight = 0;
  tankTurnRight(maxSpeed, maxSpeed);
  go();
  while(rightCounter < turn_ticks && leftCounter < turn_ticks) {
    currentStateRight = PIND & 0x03;
    currentStateLeft = PINB & 0x0A;
    if(currentStateRight != prevStateRight) rightCounter++;
    if(currentStateLeft != prevStateLeft) leftCounter++;
    prevStateRight = currentStateRight;
    prevStateLeft = currentStateLeft;
  }
  stop();
  interrupts();
}

void rotaryLeft() {
  noInterrupts();
  uint8_t rightCounter = 0;
  uint8_t leftCounter = 0;
  uint8_t currentStateLeft = 0;
  uint8_t prevStateLeft = 0;
  uint8_t currentStateRight = 0;
  uint8_t prevStateRight = 0;
  tankTurnLeft(maxSpeed);
  go();
  while(rightCounter < turn_ticks && leftCounter < turn_ticks) {
    currentStateRight = PIND & 0x03;
    currentStateLeft = PINB & 0x0A;
    if(currentStateRight != prevStateRight) rightCounter++;
    if(currentStateLeft != prevStateLeft) leftCounter++;
    prevStateRight = currentStateRight;
    prevStateLeft = currentStateLeft;
  }
  stop();
  interrupts();
}


