
void motor_init() {
	/*Setting Pin 2, 3, 4, and 6 to outputs*/
	DDRD |= ( _BV(PD0) | _BV(PD1) | _BV(PD4) );	//Pin 6  - has timer output OC4D
	/*Setting Pin 5 to an output*/
	DDRC |= ( _BV(PC6) );
	/*Setting Pin 7 to an output*/
	DDRE |= ( _BV(PE6) );
	/*Setting Pin 8 and 9 to outputs*/
	DDRB |= ( _BV(PB4) | _BV(PB5) | _BV(PB2) );
}

void timer4_pwm_init() {
	DDRD |= _BV(PD7);
	//Setting Timer Output 10 to an output
	DDRB |= _BV(PB6);			//Pin 10 - has timer output OC4B going here
	//Timer 4 control registers, I clear the registers before writing to them
	TCCR4A = 0;
	TCCR4A |= ( _BV(COM4B1) | _BV(PWM4B) );	//Enable PWM output on this pin
	TCCR4B = 0;
	TCCR4B |=  _BV(CS40) ;		//Set the clock prescaler, we use no division since the clock is already halved by using phase correct PWM
	TCCR4C = 0;
	TCCR4C |= ( _BV(COM4B1S) | _BV(COM4D1) | _BV(PWM4D) );	//Set these bits to overtake the pins, for some reason only setting the shadow bit works for OCR4B
	TCCR4D = 0;
	TCCR4D |=  _BV(WGM40);		//Setting phase correct PWM
	//Programmable Counter Unit
	OCR4B = maxSpeed;				//Initial Duty Cycle
	OCR4D = maxSpeed;				//Initial Duty Cycle
}

void timer_isr_init() {
	//Setting up timer 1 for periodic reading of the analog sensors
	TCCR1A = 0; 	//Clearing Control registers since we don't want the timer attached to any pins
	TCCR1B = 0;
	TCCR1B = ( _BV(WGM12) | _BV(CS12) );		//Set timer to CTC(clear timer on compare) and prescaler to clk/256
	TCCR1C = 0;		//Not forcing output compare
	TCNT1 = 0;		//Setting Initial timer counter value_comp
	OCR1A = 0x030D;	//Setting interrupts for 40 times per second( .001/T where T = 1/(clk/256) )
	TIMSK1 = 0;
	TIMSK1 |= _BV(OCIE1A);	//Enable interrupts on compare match A
}

void sensor_init() {
	//Setting the Analog Pins as inputs with no pullup resistor
	DDRF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
	PORTF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
}
