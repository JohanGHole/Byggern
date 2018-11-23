/*
 * MCP2515.c
 *
 * Created: 17.10.2018 08:05:23
 *  Author: johangh
 */ 
#define F_CPU 16000000
#include "MCP2515.h"
#include "SPI.h"
#include <util/delay.h>

void mcp2515_init(void){
	SPI_MasterInit();
	mcp2515_reset();
	if (mcp2515_read(MCP_CANSTAT) != MODE_CONFIG){
		printf("Error with the controller. Registers not properly reset. \n");
		return;
	}
}
uint8_t mcp2515_read(uint8_t address){
	SPI_slave_on();
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(address);
	uint8_t mcp2515_data = SPI_MasterRecieve();
	SPI_slave_off();
	return mcp2515_data;
	
}
void mcp2515_write(uint8_t address, uint8_t data){
	SPI_slave_on();
	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	SPI_slave_off();
}
void mcp2515_reset(void){
	SPI_slave_on();
	SPI_MasterTransmit(MCP_RESET);
	SPI_slave_off();
}
void mcp2515_request_to_send(uint8_t cmd){
	SPI_slave_on();
	SPI_MasterTransmit(MODE_CONFIG | (cmd & 7));
	SPI_slave_off();
}
void mcp2515_bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data){
	SPI_slave_on();
	SPI_MasterTransmit(MCP_BITMOD);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask_byte); // NB: if the register is not bit-modifiable, this will force the mask to FFh. This will allow byte_writes to the registers, not bit modify
	SPI_MasterTransmit(data);
	SPI_slave_off();
}
uint8_t mcp2515_read_status(void){
	SPI_slave_on();
	SPI_MasterTransmit(MCP_READ_STATUS);
	uint8_t data = SPI_MasterRecieve();
	SPI_slave_off();
	return data;
}

void mcp2515_test(void){
	mcp2515_init();
	printf("\n the data from the canctrl: %d", mcp2515_read(MCP_CANCTRL));
	mcp2515_bit_modify(MCP_CANCTRL,0x07,0x50);
	printf("\n the data from the canctrl after bit modifying (setting the last 3 bits to zero: %d\n", mcp2515_read(MCP_CANCTRL));
	mcp2515_init();
}