
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
void fsmRight(uint8_t motorSpeed, uint16_t IR_data[4]){
	/*State 1 - Moving the bot into a good position to start the turn*/
	while(IR_data[frontLeftData] > 300 && IR_data[frontRightData] > 300){
		moveBackward(motorSpeed, motorSpeed);
	}
	/*Setting up for a tank turn*/
	stop();
	/*Setting Right motor to move forward*/
	digitalWrite(AIN1, HIGH);
	digitalWrite(AIN2, LOW);
	/*Setting left motor to move backward*/
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	/*Setting the turning speed*/
	/*Initiate turn*/
	go();
	/*State 2 Read IR sensor data until the front two sensor are hitting white, the line follow will autocorrect if it's not perfectly centered*/
	do{
		//readIRSensors(IR_data);
	}while(IR_data[frontLeftData] < 300 && IR_data[frontRightData] < 300 );
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
	digitalWrite(STBY, LOW);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
	OCR4D = leftMotorSpeed;
	OCR4B = rightMotorSpeed;
    digitalWrite(STBY, HIGH);
}





/*---------------------------------------------------------------------------------------
 * Move the robot backwards
 * Sets the appropriate motors pins so the driver can tell the motor to move forward
 * INPUTS: uint16_t, desired motor speed from 0 - 255
 * OUTPUTS: None
 */
void moveBackward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed){
	digitalWrite(STBY, LOW);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	OCR4D = leftMotorSpeed;
	OCR4B = rightMotorSpeed;
    digitalWrite(STBY, HIGH);
}





/*---------------------------------------------------------------------------------------
 * Function to the turn the robot right through a tank turn
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: uint16_t speed
 * OUTPUTS: None
 */
void tankTurnRight(uint8_t motorSpeed){
	/*Disabling motors*/
	digitalWrite(STBY, LOW);
	/*Setting Right motor to move back so it pivots in place*/
	digitalWrite(AIN1, HIGH);
	digitalWrite(AIN2, LOW);
	/*Setting left motor to move forward*/
	digitalWrite(BIN1, LOW);
	digitalWrite(BIN2, HIGH);
	/*Setting the turning speed*/
	OCR4B = motorSpeed;
	OCR4D = motorSpeed;
	digitalWrite(STBY, HIGH);
	delay(tank_turn_delay);
}





/*---------------------------------------------------------------------------------------
 * Function to turn the robot left through a tank turn
 * Needs a time delay of 2.5s if running at the pwm at 30/255
 * Inputs: None
 * Outputs: None
 */
void tankTurnLeft(uint8_t motorSpeed){
	/*Disabling motors*/
	digitalWrite(STBY, LOW);
	/*Setting Right motor to move back so it pivots in place*/
	digitalWrite(AIN1, LOW);
	digitalWrite(AIN2, HIGH);
	/*Setting left motor to move forward*/
	digitalWrite(BIN1, HIGH);
	digitalWrite(BIN2, LOW);
	/*Setting the turning speed*/
	OCR4B = motorSpeed;
	OCR4D = motorSpeed;
	digitalWrite(STBY, HIGH);
	delay(tank_turn_delay);
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
  digitalWrite(STBY, HIGH);
  delay(arc_turn_delay);
}


