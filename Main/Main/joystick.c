#include "joystick.h"
#include "adc.h"
#include <math.h>
#include <avr/io.h>
#include "bit_manipulation.h"
#define THRESHOLD 70

static uint8_t x_center;
static uint8_t y_center;

void joystick_init(void){
	clear_bit(DDRB,DDB2);
}

uint8_t joystick_button(void){
	if ((PINB & (1 << PINB2))){
		return 0;
	}
	else {
		return 1;
	}
}


void joystick_find_center(void){
	x_center = ADC_read(CH_X);
	y_center = ADC_read(CH_Y);
}

position joystick_find_position(void){
	position XY;
	// calibrating position based on neutral reading
	XY.x = ADC_read(CH_X) - x_center;
	XY.y = ADC_read(CH_Y) - y_center;
	return XY;
}

void joystick_print_position(){
	position XY = joystick_find_position();
	printf("X-position: %d\n", XY.x);
	printf("Y-position: %d\n", XY.y);
}

DIRECTION joystick_find_direction(void){
	int degree = joystick_find_degree();
	DIRECTION dir;
	if ((degree >= 0 && degree < 45) || (degree >= 315 && degree <= 360)){
		dir = RIGHT;
	} else if (degree < 135 && degree >= 45){
		dir = UP;
	} else if (degree < 225 && degree >= 135){
		dir = LEFT;
	} else if(degree < 315 && degree >= 225){
		dir = DOWN;
	}
	else if (degree == -1){
		dir = NEUTRAL;
	}
	return dir;
}


int joystick_find_degree(void){
	position XY = joystick_find_position();
	if (XY.x < THRESHOLD && XY.y < THRESHOLD && XY.x > -THRESHOLD && XY.y > -THRESHOLD){
		return -1;
	}
	double degree = atan2(XY.y,XY.x);
	degree *= 180/M_PI;
	if (degree < 0){
		degree += 360;
	}
	return (int)(degree);
}

