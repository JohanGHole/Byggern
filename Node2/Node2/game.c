/*
 * game.c
 *
 * Created: 10.11.2018 13:53:19
 *  Author: johangh
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include "ADC.h"
#include "PD.h"
#include "MCP2515.h"
#include "CAN.h"
#include "PWM.h"
#include "solenoid_driver.h"
#include "PD.h"
#include <stdint.h>

uint8_t game_over(){
	// if the IR-signal is obstructed, the game is over
	if (ADC_read() <= 20){
		return 1;
	}
	else
	{
		return 0;
	}
}
void game_run(){
	can_msg msg_usb_board = can_recieve();
	PD_set_game_mode(msg_usb_board.data[DIFFICULTY]);
	PWM_set_pulse(PWM_joystick_to_PWM(msg_usb_board.data[JOYSTICK_X]));
	solenoid_hit(msg_usb_board.data[SLIDER_BUTTON_RIGHT],msg_usb_board.data[DIFFICULTY]);
	PD_set_position_reference(msg_usb_board.data[SLIDER_RIGHT]);
}
