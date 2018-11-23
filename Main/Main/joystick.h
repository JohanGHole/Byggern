#include "adc.h"
#include <stdint.h>
#pragma once

typedef enum
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NEUTRAL = 4
}DIRECTION;

void joystick_init(void);

void joystick_find_center(void);

position joystick_find_position(void);

void joystick_print_position();

DIRECTION joystick_find_direction(void);

int joystick_find_degree(void);

uint8_t joystick_button(void);


