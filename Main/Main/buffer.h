/*
 * buffer.h
 *
 * Created: 23.11.2018 00:54:42
 *  Author: johangh
 */ 


#ifndef BUFFER_H_
#define BUFFER_H_

#define OLED_HEIGHT 64
#define OLED_WIDTH 128
#define OLED_PAGES (OLED_HEIGHT/8)
#define FONT_OFFSET 32
#include <stdint.h>

void oled_buffer_init();

void oled_buffer_display();

void oled_buffer_rectangle(uint8_t posX,uint8_t posY, uint8_t lengthX, uint8_t lengthY,uint8_t fill);

void oled_update_buffer(uint8_t row, uint8_t column);

#endif /* BUFFER_H_ */