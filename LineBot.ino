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
#define PWMB 6		//OCR4D

/* Time it takes for my bot to turn at a movespeed of 30 */
#define arc_turn_delay 2500 //in ms
#define tank_turn_delay 1250 //in ms


#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/sleep.h>

const uint8_t maxSpeed = 50;
uint16_t IR_data[4];
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
  Serial.begin(9600);
}

/*Functions Availabe:
	moveForward(left, right);
	
*/



void loop() {
  
}

void calibration() {
	for(uint16_t i = 0; i < 4; i++) {
		if(IR_data[i] > maxIRValue) {
			maxIRValue = IR_data[i];
		}else if(IR_data[i] < minIRValue) {
			minIRValue = IR_
	}
	
}
void lineFollow() {
	if([IR_data[backLeftData] > 300 && IR_data[backRightData] ) {
	  moveForward(maxSpeed, maxSpeed);
  }else{
	  
  }
}
/**Interrupt service routine to read analogIRData
 *
 */
ISR(TIMER1_COMPA_vect) {
	IR_data[0] = analogRead(frontLeftIR);
	IR_data[1] = analogRead(frontRightIR);
	IR_data[2] = analogRead(backLeftIR);
	IR_data[3] = analogRead(backRightIR);
}
