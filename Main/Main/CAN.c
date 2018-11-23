/*
 * CAN.c
 *
 * Created: 17.10.2018 12:24:12
 *  Author: johangh
 */ 
#define F_CPU 4915200
#include <util/delay.h>
#include "MCP2515.h"
#include "SPI.h"
#include "CAN.h"
#include "adc.h"
#include "joystick.h"
#include "slider.h"

void CAN_init(){
	SPI_MasterInit();
	mcp2515_reset();
	mcp2515_bit_modify(MCP_CANCTRL,MCP_CANCTRL_SET_OPERATION_MODE,MODE_NORMAL);
}

void CAN_send(CAN_msg msg){
	// SIDL and SIDH are identifier bits.
	// DLC represents the number of bytes that will be transmitted
	while((mcp2515_read(MCP_TXB0CTRL) & MCP_TXBnCTRL_TXREQ)){} //checking if TXB0CTRL.TXREQ = 1
	mcp2515_write(MCP_TXB0_SIDH,msg.id << 5);
	mcp2515_write(MCP_TXB0_SIDL,msg.id >> 3);
	mcp2515_write(MCP_TXB0DLC,msg.length);
	for (int i = 0; i < msg.length; i++){ // TXBnDm -> TRANSMIT BUFFER n DATA BYTE m, has to loop through to write each data byte from can_msg to transmit buffer
		mcp2515_write(MCP_TXB0Dm+i,msg.data[i]);		
	}
	mcp2515_request_to_send(1);
}

CAN_msg CAN_recieve(){
	CAN_msg msg;
	// set everything to zero (null_msg package)
	msg.id = 0;
	msg.length = 0;
	for(int i = 0; i < 8; i++){
		msg.data[i] = 0;
	}
	if(!(mcp2515_read(MCP_CANINTF) & MCP_RX0IF)){ // checks if the receive register is empty and able to accept a new message. if not, return null_msg
		return msg;
	}
	msg.id = (mcp2515_read(MCP_RXB0_SIDL) >> 5) | (mcp2515_read(MCP_RXB0_SIDH) << 3);
	msg.length = (mcp2515_read(MCP_RXB0_DLC) & (0x0f));
	for (int i = 0; i < msg.length; i++){
		msg.data[i] = mcp2515_read(MCP_RXB0_Dm + i);
	}
	mcp2515_bit_modify(MCP_CANINTF,0x01,0x00); // set RX0IF to zero, ensuring that the MCU has finished with the message before a new message is loaded
	return msg;
}

void CAN_print_msg(CAN_msg msg){
	printf("id: %d\nlength: %d \n",msg.id,msg.length);
	printf("Data:\n{");
	for (int i = 0;i < msg.length; i++){
		printf("%d ",msg.data[i]);
	}
	printf("}\n");
}

CAN_msg CAN_package(uint8_t start_game, uint8_t difficulty){
	CAN_msg msg;
	msg.id = 1;
	msg.length = 8;
	position XY = joystick_find_position();
	slider_position slider = slider_find_position();
	msg.data[JOYSTICK_X] = ADC_read(CH_X);
	msg.data[DIFFICULTY] = difficulty;
	msg.data[JOYSTICK_BUTTON] = joystick_button();
	msg.data[SLIDER_LEFT] = slider.left;
	msg.data[SLIDER_RIGHT] = slider.right;
	msg.data[SLIDER_BUTTON_LEFT] = slider_left_button();
	msg.data[SLIDER_BUTTON_RIGHT] = slider_right_button();
	msg.data[GAME_START] = start_game;
	return msg;
}

void CAN_test_loopback_1(CAN_msg msg){
	CAN_init();
	printf("This is the first message to be sent:\n");
	CAN_print_msg(msg);
	CAN_send(msg);
	CAN_print_msg(CAN_recieve(msg));
	printf("\nthe first message is received.");
}

void CAN_test_loopback_2(CAN_msg msg, CAN_msg msg2){
	CAN_init();
	printf("This is the first message to be sent:\n");
	CAN_print_msg(msg);
	CAN_send(msg);
	CAN_print_msg(CAN_recieve(msg));
	printf("\nthe first message is received.");
	printf("\nThis is the second message to be sent:\n");
	CAN_print_msg(msg2);
	CAN_send(msg2);
	CAN_print_msg(CAN_recieve(msg2));
	printf("\nthe second message is received.");
}

