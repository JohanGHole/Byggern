#include "bit_manipulation.h"
#include "slider.h"
#include <avr/io.h>

void slider_init(void) {
	// set the button pins to input
	clear_bit(DDRB,DDB0);
	clear_bit(DDRB,DDB1);
}

uint8_t slider_left_button(void){
	if ((PINB & (1 << PINB1))){
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t slider_right_button(void){
	if ((PINB & (1 << PINB0))){
		return 1;
	}
	else {
		return 0;
	}
}

slider_position slider_find_position(void){
	slider_position sp;
	sp.left = ADC_read(CH_LEFT_SLIDER);
	sp.right = ADC_read(CH_RIGHT_SLIDER);
	return sp;
}

