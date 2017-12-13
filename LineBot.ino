/**************************************************
	Name	  	: Lab 06
	Author		: Ricardo Alcaraz
	Date	  	: 17 November 2017
	Version		: 0.8
**************************************************/

/*  Atmega to Motor Driver Pinout
    Pin 9 - PWMA
    Pin 8 - AIN2
    Pin 7 - AIN1
    Pin 6 - STBY
    Pin 5 - BIN1
    Pin 4 - BIN2
    Pin 3 - PWMB
    NOTES: A is left motor, B is right motor
    -----------
    Atmega to Sensor Pinout
    A0    - Front Left Sensor
    A1    - Front Right Sensor
    A2    - Back Right
    A3    - Back Left
    -----------
    Pin to Port References
    Grabbed from sparkfun datasheet https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ProMicro16MHzv1.pdf
    PIN
    0(RX1)  - PD2
    1(TXO)  - PD3
    2   - PD1
    3   - PD0
    4   - PD4(ADC)(PWM)
    5   - PC6
    6   - PD7
    7   - PE6
    8   - PB4(ADC)
    9   - PB5(ADC)(PWM)
    10    - PB6
    14    - PB3
    15    - PB1
    16    - PB2
    A0    - PF7
    A1    - PF6
    A2    - PF5
    A3    - PF4

    Slowest Speed Bot can support @ 4 charged AA batteries: 30/255
*/

#define frontLeftIR A3
#define frontRightIR A2
#define backLeftIR A1
#define backRightIR A0
#define frontLeftData 0
#define frontRightData 1
#define backLeftData 2
#define backRightData 3
#define PWMA 10		//OCR4B
#define AIN2 8
#define AIN1 7
#define STBY 5
#define BIN2 9
#define BIN1 4
#define PWMB 6		//OCR4D test test
#define LED 16
/* Time it takes for my bot to turn at a movespeed of 30 */
#define arc_turn_delay 2500 //in ms
#define tank_turn_delay 1250 //in ms


#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <elapsedMillis.h>
#include "maze.h"

const uint8_t maxSpeed = 35;
const uint8_t leftOffset = 0;
const uint8_t rightOffset = 3;
volatile uint16_t IR_data[4];

void setup() {
	motor_init();
	timer4_pwm_init();
	sensor_init();
	timer_isr_init();
  encoder_init();
	//sei();					//Enable Global Interrupts
  pinMode(LED, OUTPUT);
	//Short delay before starting
	delay(1000);
	digitalWrite(LED, HIGH);
	delay(500);
	digitalWrite(LED, LOW);
	delay(500);
	digitalWrite(LED, HIGH);
	delay(500);
	digitalWrite(LED, LOW);
	delay(500);

}

/*Functions Availabe:
	moveForward(left, right);
	
*/



void loop() {

  //Initializing the virtual robot
  static MazeValues virtBot = {0x00, 0x00, 0x00, 0x03, 0x13, 0x00};
  if( isIntersection() ) {
    stop();
    delay(1000);
    virtBot = enterRoom(virtBot);
    char choice = whichWay(virtBot);
    switch (choice) {
      case 'S': 
                while(IR_data[frontRightData] > 500 && IR_data[frontLeftData] > 500) lineFollow();
                break;
      case 'L': virtBot.turn = 0b10;
                virtBot = turnInMaze(virtBot);
                rotaryLeft();
                break;
      case 'R': virtBot.turn = 0b01;
                virtBot = turnInMaze(virtBot);
                rotaryRight();
                break;
      case 'T': virtBot.turn = 0b11;
                virtBot = turnInMaze(virtBot);
                rotaryTurnAround();
                break;   
    }
  }
  lineFollow();
}

/**Check if the bot is on an intersection or not
 * Checks the IR data array which is available globally
 * INPUTS: None
 * OUTPUTS: None
 */
boolean isIntersection() {
  if(IR_data[frontRightData] > 500 && IR_data[frontLeftData] > 500) {
    return true;
  }
  return false;
}


/**Interrupt service routine to read analogIRData
 * Data is then averaged out with the previous 3 values
 */
ISR(TIMER1_COMPA_vect) {
	cli();
	static uint16_t frontLeft[4] = {0,0,0,0};
	static uint16_t frontRight[4] = {0,0,0,0};
	static uint16_t backRight[4] = {0,0,0,0};
	static uint16_t backLeft[4] = {0,0,0,0};
	static uint8_t counter = 0;
	uint16_t sums[4] = {0,0,0,0};
	frontLeft[counter] = analogRead(frontLeftIR);
	frontRight[counter] = analogRead(frontRightIR);
	backLeft[counter] = analogRead(backLeftIR);
	backRight[counter] = analogRead(backRightIR);
 //Data is summed then the average is taken
	for(int i = 0; i < 4; i++) {
		sums[0] += frontLeft[i];
		sums[1] += frontRight[i];
		sums[2] += backLeft[i];
		sums[3] += backRight[i];
	}
 //Global array is updated with new values
	IR_data[frontLeftData] = (sums[0] >> 2);
	IR_data[frontRightData] = (sums[1] >> 2);
	IR_data[backLeftData] = (sums[2] >> 2);
	IR_data[backRightData] = (sums[3] >> 2);
	if(counter == 3) {
		counter = 0;
	} else {
		counter++;
	}
	sei();
}
