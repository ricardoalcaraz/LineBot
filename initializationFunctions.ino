/**Initializing motor pins 2-10 as outputs
 * INPUTS:  None
 * OUTPUTS: None
 */
void motor_init() {
	/*Setting Pin 4 and 6 to outputs*/
	DDRD |= (  _BV(PD4) | _BV(PD7) );	//Pin 6  - has timer output OC4D
  PORTD &= ~( _BV(PD1) | _BV(PD4) );
	/*Setting Pin 5 to an output*/
	DDRC |= ( _BV(PC6) );
  PORTC &= ~( _BV(PC6) );
	/*Setting Pin 7 to an output*/
	DDRE |= ( _BV(PE6) );
  PORTE &= ~( _BV(PE6) );
	/*Setting Pin 8, 9, and 10 to outputs*/
	DDRB |= ( _BV(PB4) | _BV(PB5) | _BV(PB6) );
  PORTE &= ~( _BV(PB4) | _BV(PB5) | _BV(PB6) );
}

void timer4_pwm_init() {
	//Timer 4 control registers, I clear the registers before writing to them
	TCCR4A |= ( _BV(COM4B1) | _BV(PWM4B) );	//Enable PWM output on this pin
	TCCR4B &= ~( _BV(CS43) | _BV(CS42) | _BV(CS41) );  //clear prior settings
	TCCR4B |=  _BV(CS40) ;		//Set the clock prescaler, we use no division since the clock is already halved by using phase correct PWM
	TCCR4C |= ( _BV(COM4B1S) | _BV(COM4D1) | _BV(PWM4D) );	//Set these bits to overtake the pins, for some reason only setting the shadow bit works for OCR4B
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
	OCR1A = 0x0280;	//Setting interrupts for 40 times per second( .001/T where T = 1/(clk/256) )
	TIMSK1 = 0;
	TIMSK1 |= _BV(OCIE1A);	//Enable interrupts on compare match A
}

void sensor_init() {
	//Setting the Analog Pins as inputs with no pullup resistor
	DDRF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
	PORTF &= ~( _BV(PF7) | _BV(PF6) | _BV(PF5) | _BV(PF4) );
}

/**Initializing outputs for the encoders
 * Pins 2,3,14 and 15
 * INPUTS: None
 * OUTPUTS:None
 */
void encoder_init() {
  DDRD |= ( _BV(PD0) | _BV(PD1) );
  PORTD |= ( _BV(PD0) | _BV(PD1) );
  //Setting pins 14 and 15 as inputs with a pullup resistor
  DDRB |=  ( _BV(PB1) | _BV(PB3) );
  PORTB |= ( _BV(PB1) | _BV(PB3) );
}

