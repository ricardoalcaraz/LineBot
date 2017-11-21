/*
 * LineBot.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: cinna
 */

/*
  Lab 3 - Turning - Timed and FSM
  Version:  0.4
  Written By: Ricardo Alcaraz
  ID:     012157754
  Date:   November 8, 2018
  Class:    EE444 MW
*/

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

/*Made specifically for an ATMEGA 32U4 with the TB6612FNG*/
/*Initial version will have to have pins wired a certain way or it will not work*/

#include "LineBot.h"

#define frontLeftIR A3
#define frontRightIR A2
#define backLeftIR A1
#define backRightIR A0
#define frontLeftData 0
#define frontRightData 1
#define backLeftData 2
#define backRightData 3
#define PWMA 10
#define AIN2 8
#define AIN1 7
#define STBY 5
#define BIN2 3
#define BIN1 4
#define PWMB 6

/* Time it takes for my bot to turn at a movespeed of 30 */
#define arc_turn_delay 2500 //in ms
#define tank_turn_delay 1250 //in ms

LineBot::LineBot() {
	this->maxSpeed = 50;
}

/**Function to initialize all the necessary pins to outputs
 * Inputs: None
 * Outputs: None
 */
void LineBot::begin(){
	/*Setting Pin 2, 3, 4, and 6 to outputs*/
	DDRD |= ( _BV(PD0) | _BV(PD1) | _BV(PD4) | _BV(PD7) );
	/*Setting Pin 5 to an output*/
	DDRC |= ( _BV(PC6) );
	/*Setting Pin 7 to an output*/
	DDRE |= ( _BV(PE6) );
	/*Setting Pin 8 and 10 to outputs*/
	DDRB |= ( _BV(PB6) | _BV(PB4) );
	/*Setting the Analog Pins as inputs with no pullup resistor*/
	DDRF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
	PORTF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
}




void LineBot::moveForward() {
	digitalWrite(STBY, LOW);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, this->maxSpeed);
    analogWrite(PWMB, this->maxSpeed);
    digitalWrite(STBY, HIGH);
}

void LineBot::setMaxSpeed(uint8_t speed) {
	this->maxSpeed = speed;
}

void LineBot::stop() {
	digitalWrite(STBY, LOW);
}
LineBot::~LineBot() {
	// TODO Auto-generated destructor stub
}

/*Setting the left motor to outputs the arduino way
void LineBot::setLeftMotor(uint8_t AIN1, uint8_t AIN2, uint8_t PWMA){
	this->AIN1 = AIN1;
	this->AIN2 = AIN2;
	this->PWMA = PWMA;
	uint8_t AIN1Bit = digitalPinToBitMask(AIN1);
	uint8_t AIN1Port = digitalPinToPort(AIN1);
	uint8_t AIN2Bit = digitalPinToBitMask(AIN2);
	uint8_t AIN2Port = digitalPinToPort(AIN2);
	uint8_t PWMABit = digitalPinToBitMask(PWMA);
	uint8_t PWMAPort = digitalPinToPort(PWMA);
	volatile uint8_t *reg0, *out0;
	volatile uint8_t *reg1, *out1;
	volatile uint8_t *reg2, *out2;
	//If wrong pins were used return
	if (AIN1Port == NOT_A_PIN || AIN2Port == NOT_A_PIN || PWMAPort == NOT_A_PIN) return;

	reg0 = portModeRegister(AIN1Port);
	out0 = portOutputRegister(AIN1Port);
	reg1 = portModeRegister(AIN2Port);
	out1 = portOutputRegister(AIN2Port);
	reg2 = portModeRegister(PWMAPort);
	out2 = portOutputRegister(PWMAPort);

	//Setting to an output
	uint8_t oldSREG = SREG;
	cli();
	*reg0 |= AIN1Bit;
	*reg1 |= AIN2Bit;
	*reg2 |= PWMABit;
	SREG = oldSREG;

}
*/
