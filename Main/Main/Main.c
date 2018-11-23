/*
 * Main.cpp
 *
 * Created: 29.08.2018 09:27:21
 *  Author: Johangh
 */ 

#define F_CPU 4915200 // clock frequency in Hz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "sram.h"
#include "joystick.h"
#include "adc.h"
#include "memory.h"
#include "bit_manipulation.h"
#include "test.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include "MCP2515.h"
#include "buffer.h"
#include "snake.h"
#include <math.h>

int main(void){
	cli();
	uart_init();
	memory_init();
	SRAM_test();
	ADC_init();
	oled_init();
	joystick_init();
	oled_reset();
	joystick_find_center();
	CAN_init();
	menu_init();
	_delay_ms(100);
	CAN_send(CAN_package(0,menu_get_difficulty()));
	
	while(1){
		menu_move();
		_delay_ms(150);
		position XY = joystick_find_position();
		if( (menu_get_current_menu()->menu_function != NULL) && joystick_button() ){
			_delay_ms(200);
			menu_get_current_menu()->menu_function();
		}
		else if (menu_get_current_menu()->name == "Play Snake" && joystick_button()){
			_delay_ms(200);
			snake_play_snake(MEDIUM);
			menu_display(menu_get_main());
		}
	}

}
