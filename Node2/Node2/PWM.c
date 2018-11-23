/*
 * PWM.c
 *
 * Created: 07.11.2018 22:41:22
 *  Author: johangh
 */ 
// INVERTED = 1 when the servo is inverted
#define INVERTED 0
#define F_CPU 16000000
#define PERIOD 0.02
#define PRESCALER 64
#define OFFSET 5
#include "bit_manipulation.h"
#include <avr/io.h>


void PWM_init(){
	//setting prescaler 64 to ensure that we can get a 20 ms period
	set_bit(TCCR1B,CS10);
	set_bit(TCCR1B,CS11);
	clear_bit(TCCR1B,CS12);
	//Setting the TOP value for TCNT equal to a period 
	ICR1 = (F_CPU/PRESCALER) * PERIOD;
	set_bit(DDRB,PB5);
	//setting initial pulse value to middle value
	PWM_set_pulse(1.5);
}

void PWM_set_pulse(float ms){
	if (ms < 0.9){
		ms = 0.9;
	}
	else if (ms > 2.1){
		ms = 2.1;
	}
	//setting duty-cycle
	OCR1A = (F_CPU/PRESCALER) * ms/1000; //
}


float PWM_joystick_to_PWM(uint8_t position){
	if (INVERTED){
		position = 255-position;
	}
	return (0.9 + (2.0/425)*position);
}
