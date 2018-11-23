/*
 * motor.c
 *
 * Created: 14.11.2018 10:31:17
 *  Author: einarlor
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include "bit_manipulation.h"
#include "motor.h"
#include "CAN.h"
#include "DAC.h"
uint16_t MIN_ENCODER;
uint16_t MAX_ENCODER;

void motor_init(){
	// enable motor
	set_bit(DDRH,DDH4);
	set_bit(PORTH,PH4);
	//enable input from motor
	set_bit(DDRH,DDH5);
	//enable setting of direction
	set_bit(DDRH,DDH1);
	//enable setting of select-bit (select hi/lo byte of encoder)
	set_bit(DDRH,DDH3);
	clear_bit(PORTH,PINH3);
	//reset motor
	set_bit(DDRH,DDH6);
	clear_bit(PORTH,PH6);
	_delay_ms(100);
	set_bit(PORTH,PH6); // setting PH6 high again because reset is active low
	//set D0-D07 to input from motor
	clear_bit(DDRK,PK0);
	clear_bit(DDRK,PK1);
	clear_bit(DDRK,PK2);
	clear_bit(DDRK,PK3);
	clear_bit(DDRK,PK4);
	clear_bit(DDRK,PK5);
	clear_bit(DDRK,PK6);
	clear_bit(DDRK,PK7);
}


int16_t motor_read_encoder(){
	// set !OE low to enable output of encoder
	clear_bit(PORTH,PINH5);
	// set SEL low to get high byte
	clear_bit(PORTH,PINH3);
	_delay_ms(1);
	//Read MSB
	int16_t data = (PINK << 8);
	//Set SEL high to get low byte
	set_bit(PORTH,PINH3);
	 _delay_ms(1);
	// Read LSB
	data |= PINK;
	//Set !OE high to disable output of encoder
	set_bit(PORTH,PINH5);
	return data;
}

void motor_speed(uint8_t speed){
	DAC_send(speed);
}
void reset_encoder(){
	clear_bit(PORTH,PH6);
	_delay_ms(100);
	set_bit(PORTH,PH6);
}

void motor_direction(direction dir){
	if (dir == LEFT){
		clear_bit(PORTH,PH1);
	}
	else if (dir == RIGHT){
		set_bit(PORTH,PH1);
	}
}

void motor_calibration(){
	// driving the motor from left to right, updating upper and lower limit
	int16_t last_position;
	int16_t position = motor_read_encoder();
	motor_direction(LEFT);
	motor_speed(95);
	_delay_ms(20);
	last_position = position;
	position = motor_read_encoder();
	while (last_position != position){
		_delay_ms(20);
		last_position = position;
		position = motor_read_encoder();
	}
	motor_speed(0);
	reset_encoder();
	MIN_ENCODER = motor_read_encoder();
	motor_direction(RIGHT);
	motor_speed(95);
	_delay_ms(20);
	last_position = position;
	position = motor_read_encoder();
	while (last_position != position){
		_delay_ms(20);
		last_position = position;
		position = motor_read_encoder();
	}
	motor_speed(0);
	_delay_ms(200);
	MAX_ENCODER = motor_read_encoder();
	printf("\ncalibration complete\n");
}

void motor_open_loop(uint8_t x_position){
	if (x_position < 137 && x_position > 117){
		motor_speed(0);
	}
	else if (x_position >= 137){
		motor_direction(RIGHT);
		motor_speed(80);
	}
	else if(x_position <= 117){
		motor_direction(LEFT);
		motor_speed(80);
	}
}
int16_t motor_get_max_encoder(){
	return MAX_ENCODER;
}
int16_t motor_get_min_encoder(){
	return MIN_ENCODER;
}
