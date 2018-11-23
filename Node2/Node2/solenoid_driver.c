/*
 * solenoid_driver.c
 *
 * Created: 12.11.2018 00:18:49
 *  Author: edvardsa
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "bit_manipulation.h"
#include "PD.h"
#include <stdint.h>
#include "solenoid_driver.h"
#include <util/delay.h>


void solenoid_init(){
	//enable PF1 as output for solenoid
	set_bit(DDRF,PF1);
	clear_bit(PORTF,PF1);
}

void solenoid_hit(uint8_t button_pressed, GAME_MODE game_difficulty){
	if (game_difficulty == EASY){
		if (button_pressed){
			set_bit(PORTF,PF1);
			_delay_ms(30);
			clear_bit(PORTF,PF1);
			
		}
		else if(!button_pressed){
			clear_bit(PORTF,PF1);
		}
	}
	else {
		if (button_pressed){
			set_bit(PORTF,PF1);
		}
		else if (!button_pressed) {
			clear_bit(PORTF,PF1);
		}
	}
}