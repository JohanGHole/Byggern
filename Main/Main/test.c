#include "adc.h"
#include "bit_manipulation.h"
#include "CAN.h"
#include <avr/io.h>
#include "memory.h"
#include "joystick.h"
#include "slider.h"
#include "MCP2515.h"
#define F_CPU 4915200
#include <util/delay.h>

void ADC_test(void) {
	int degree;
	joystick_find_center();
	DIRECTION dir;
	position current;
	slider_position current_slider;
	uint8_t left_button;
	uint8_t right_button;
	while(1){
		// code for decode testing
		/*
		address_sram[0] = 0xFF;
		_delay_ms(500);
		address_adc[0] = 0xFF;
		_delay_ms(500);
		*/
		if ((PINB & (1 << PINB1))) {
			printf("HØY_høyreknapp \n");
		}
		if ((PINB & (1 << PINB0))) {
			printf("HØY_venstreknapp \n");
		}
		current = joystick_find_position();
		degree = joystick_find_degree();
		dir = joystick_find_direction();
		current_slider = slider_find_position();
		left_button = slider_left_button();
		right_button = slider_right_button();
		printf("Joystick position: \n");
		printf("x_value: %d",current.x);
		printf("\n");
		printf("y_value: %d",current.y);
		printf("\n");
		printf("Slider positions: \n");
		printf("left slider: %d",current_slider.left);
		printf("\n");
		printf("right slider: %d",current_slider.right);
		printf("\n");
		printf("Left button: %d", left_button);
		printf("\n");
		printf("right button: %d", right_button);
		printf("\n");
		printf("degree: %d",degree);
		printf("\n");
		printf("Direction: %d",dir);
		printf("\n");
		_delay_ms(1000);
	}
}

void slider_test(){
	slider_position pos = slider_find_position();
	printf("Left slider position: %d\n",pos.left);
	printf("Right slider position: %d\n",pos.right);
	if (slider_left_button()){
		printf("Left button pressed!!\n");
	}
	if (slider_right_button()){
		printf("Right button pressed!!\n");
	}
}

void SPI_test(){
	SPI_MasterInit();
	//SPI_reset();
	SPI_slave_on();
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(0x0E);
	uint8_t some_data = SPI_MasterRecieve();
	SPI_slave_off();
	printf("this should now be 0x80: %d",some_data);
}

void mcp2515_test(void){
	mcp2515_init();
	//mcp2515_write(MCP_CANCTRL,231);
	printf("\n the data from the canctrl: %d", mcp2515_read(MCP_CANCTRL));
	mcp2515_bit_modify(MCP_CANCTRL,0x07,0x50);
	printf("\n the data from the canctrl after bit modifying (setting the last 3 bits to zero: %d\n", mcp2515_read(MCP_CANCTRL));
	//mcp2515_write(MCP_CANSTAT,0xFF);
	mcp2515_init();
}