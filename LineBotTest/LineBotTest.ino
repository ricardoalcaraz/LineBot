
/*
  Lab 2 - Creating the Line Following Instruction
  Version:  0.1
  Written By: Ricardo Alcaraz
  ID:     012157754
  Date:   October 22, 2018
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
#include "Arduino.h"
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

//LineBot mazeBot;

void setup() {
	//mazeBot.begin();
	//mazeBot.setMaxSpeed(30);
	pinMode(PWMA, OUTPUT);
	pinMode(PWMB, OUTPUT);
	pinMode(STBY, OUTPUT);
	digitalWrite(STBY, HIGH);
	phaseCorrectSetup();
}

void loop() {

}


//Pins 5 and 10 let you use timer 4
void phaseCorrectSetup() {
	//Setting mode for phase correct on register B
	TCCR4A |= (_BV(COM4B1) | _BV(PWM4B)) ;
	//setting the prescaler to 16M/256
	TCCR4B |= (_BV(CS43) | _BV(CS40)) ;
	TCCR4C |= (_BV(COM4D1) | _BV(PWM4D)) ;
	//Enabling phase correct PWM
	TCCR4D |= _BV(WGM40);
	OCR4D = 30;
	OCR4C = 30;
	OCR4B = 30;
}

