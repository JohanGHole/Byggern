/*
 * CAN.h
 *
 * Created: 17.10.2018 12:24:28
 *  Author: johangh
 */ 


typedef struct{
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
}CAN_msg;

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

void CAN_init();

void CAN_send(CAN_msg msg);

CAN_msg CAN_recieve();

void CAN_print_msg(CAN_msg msg);

// CAN_package: values from the USB board
CAN_msg CAN_package(uint8_t start_game, uint8_t difficulty);