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
#define BIN2 3
#define BIN1 4
#define PWMB 6		//OCR4D test test

/* Time it takes for my bot to turn at a movespeed of 30 */
#define arc_turn_delay 2500 //in ms
#define tank_turn_delay 1250 //in ms


#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/sleep.h>

const uint8_t maxSpeed = 50;
volatile uint16_t IR_data[4];
uint16_t maxIRValue;
uint16_t minIRValue;
/*In the setup we will set pins 2-8,10 to outputs for the motors and set A0-A4 to inputs for the IR Sensors*/
void setup() {
	motor_init();
	timer4_pwm_init();
	sensor_init();
	timer_isr_init();
	sei();					//Enable Global Interrupts
	//Short delay before starting
	delay(2000);
	calibration();
	/*
	Serial.begin(9600);
	while(!Serial);
	Serial.println("Calibration Done");
	Serial.println(maxIRValue);
	Serial.println(minIRValue);
	*/
}

/*Functions Availabe:
	moveForward(left, right);
	
*/



void loop() {
/*
	Serial.println(IR_data[0]);
	Serial.println(IR_data[1]);
	Serial.println(IR_data[2]);
	Serial.println(IR_data[3]);
	delay(1000);
*/
	lineFollow();	
}

/**Function to calibrate the sensors and set a min or max value
 * INPUTS: 	None
 * OUTPUTS: None
 */
void calibration() {
	minIRValue = 1000;
	while(millis() < 7000) {
		for(uint16_t i = 0; i < 4; i++) {
			if(IR_data[i] > maxIRValue) {
				maxIRValue = IR_data[i];
			}else if(IR_data[i] < minIRValue) {
				minIRValue = IR_data[i];
			}
		}
	}
}
/**Function to allow line following for the bot
 *
 */
void lineFollow() {
	if(IR_data[backLeftData] > 300 && IR_data[backRightData] > 300) {
		moveForward(maxSpeed, maxSpeed);
	} else {
		uint8_t leftSpeed = maxSpeed - (((IR_data[backLeftData]-minIRValue)/maxIRValue)*maxSpeed);
		uint8_t rightSpeed = maxSpeed - (((IR_data[backRightData]-minIRValue)/maxIRValue)*maxSpeed);
		moveForward(leftSpeed, rightSpeed);
	}
}
/**Interrupt service routine to read analogIRData
 *
 */
ISR(TIMER1_COMPA_vect) {
	cli();
	IR_data[0] = analogRead(frontLeftIR);
	IR_data[1] = analogRead(frontRightIR);
	IR_data[2] = analogRead(backLeftIR);
	IR_data[3] = analogRead(backRightIR);
	sei();
}
