/*
 * CAN.h
 *
 * Created: 17.10.2018 12:24:28
 *  Author: johangh
 */ 


#ifndef CAN_H_
#define CAN_H_
typedef struct{
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
}can_msg;

typedef enum{
	JOYSTICK_X,
	DIFFICULTY,
	JOYSTICK_BUTTON,
	SLIDER_LEFT,
	SLIDER_RIGHT,
	SLIDER_BUTTON_LEFT,
	SLIDER_BUTTON_RIGHT,
	GAME_START
}USB_PACKAGE; // data-index from usb_board

void can_init();

void can_send(can_msg msg);

can_msg can_recieve();

void print_msg(can_msg msg);

// sends package to node 1 if the game is over
can_msg CAN_package(uint8_t end_game);

#endif /* CAN_H_ */