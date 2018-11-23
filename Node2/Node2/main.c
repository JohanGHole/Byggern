/*
 * main.c
 *
 * Created: 05.11.2018 11:27:46
 *  Author: einarlor
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP2515.h"
#include "SPI.h"
#include "UART.h"
#include "bit_manipulation.h"
#include "PWM.h"
#include "ADC.h"
#include "game.h"
#include "solenoid_driver.h"
#include "timer.h"
#include "DAC.h"
#include "motor.h"
#include "PD.h"

int main(){
	/* ---------------------- DRIVER INIT ---------------------- */
	uart_init();
	mcp2515_init();
	can_init();
	timer_init();
	PWM_init();
	ADC_init();
	solenoid_init();
	DAC_init();
	motor_init();
	motor_calibration();
	PD_init();
	while(1){
		/* ---------------------- RUN GAME ---------------------- */
		while (!(game_over())){
			game_run();
		}
		can_send(CAN_package(1)); // the game is over -> sends package to node 1
		_delay_ms(500); // delay to ensure that the CAN_package is sent
	}
}
