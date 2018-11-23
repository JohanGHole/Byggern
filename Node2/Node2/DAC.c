/*
 * DAC.c
 *
 * Created: 13.11.2018 16:57:35
 *  Author: edvardsa
 */ 
#include <stdint.h>
#include <avr/interrupt.h>
#include "TWI_Master.h"

void DAC_init(){
	sei();
	TWI_Master_Initialise();
	//reset DAC-outputs everytime we initialize
	unsigned char address_write = 0b01010000;
	unsigned char TWI_package_length = 2;
	unsigned char TWI_reset_package[2];
	unsigned char command = 0b00010000;
	TWI_reset_package[0] = address_write;
	TWI_reset_package[1] = command;
	TWI_Start_Transceiver_With_Data(TWI_reset_package, TWI_package_length);
}

void DAC_send(unsigned char data){
	unsigned char address_write = 0b01010000;
	unsigned char TWI_package_length = 3;
	unsigned char TWI_package[3];
	unsigned char command = 0b00000000;
	TWI_package[0] = address_write;
	TWI_package[1] = command;
	TWI_package[2] = data;
	TWI_Start_Transceiver_With_Data(TWI_package, TWI_package_length);
}