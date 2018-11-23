/*
 * CAN.c
 *
 * Created: 17.10.2018 12:24:12
 *  Author: johangh
 */ 
#include "MCP2515.h"
#include "SPI.h"
#include "CAN.h"
#define F_CPU 4915200
#include <util/delay.h>

void can_init(){
	SPI_MasterInit();
	mcp2515_reset();
	mcp2515_bit_modify(MCP_CANCTRL,MCP_CANCTRL_SET_OPERATION_MODE,MODE_NORMAL);
}
void can_send(can_msg msg){
	// SIDL and SIDH are identifier bits.
	// DLC represents the number of bytes that will be transmitted
	while((mcp2515_read(MCP_TXB0CTRL) & MCP_TXBnCTRL_TXREQ)){} // checking if TXB0CTRL.TXREQ = 1
	mcp2515_write(MCP_TXB0_SIDH,msg.id << 5);
	mcp2515_write(MCP_TXB0_SIDL,msg.id >> 3);
	mcp2515_write(MCP_TXB0DLC,msg.length);
	for (int i = 0; i < msg.length; i++){ // TXBnDm -> TRANSMIT BUFFER n DATA BYTE m, has to loop through to write each data byte from can_msg too can buffer
		mcp2515_write(MCP_TXB0Dm+i,msg.data[i]);		
	}
	mcp2515_request_to_send(1);
}

can_msg can_recieve(){
	can_msg msg;
	// set everything to zero (null_msg package)
	msg.id = 0;
	msg.length = 0;
	for(int i = 0; i < 8; i++){
		msg.data[i] = 0;
	}
	while(!(mcp2515_read(MCP_CANINTF) & MCP_RX0IF)); // loops until the receive register is empty and able to accept a new message
	msg.id = (mcp2515_read(MCP_RXB0_SIDL) >> 5) | (mcp2515_read(MCP_RXB0_SIDH) << 3);
	msg.length = (mcp2515_read(MCP_RXB0_DLC) & (0x0f));
	for (int i = 0; i < msg.length; i++){
		msg.data[i] = mcp2515_read(MCP_RXB0_Dm + i);
	}
	mcp2515_bit_modify(MCP_CANINTF,0x01,0x00); // set RX0IF to zero, ensuring that the can bus has finished with the message before a new message is loaded
	return msg;
}

void print_msg(can_msg msg){
	printf("id: %d\nlength: %d \n",msg.id,msg.length);
	printf("Data:\n{");
	for (int i = 0;i < msg.length; i++){
		printf("%d ",msg.data[i]);
	}
	printf("}\n");
}

can_msg CAN_package(uint8_t end_game){
	can_msg msg;
	msg.id = 2;
	msg.length = 1;
	msg.data[0] = end_game;
	return msg;
}
