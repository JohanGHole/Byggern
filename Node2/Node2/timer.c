/*
 * timer.c
 *
 * Created: 07.11.2018 20:04:35
 *  Author: johangh
 */ 
#include "timer.h"
#include "bit_manipulation.h"

#include <avr/io.h>

 void timer_init(){
	 //Setting fast-PWM
	 clear_bit(TCCR1A,WGM10);
	 set_bit(TCCR1A,WGM11);
	 set_bit(TCCR1B,WGM12);
	 set_bit(TCCR1B,WGM13); 
	 //Setting output compare. This is a flag that gets set high when TCNT and OCR is equal
	 clear_bit(TCCR1A,COM1A0);
	 set_bit(TCCR1A,COM1A1); 
 }
