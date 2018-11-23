/*
 * buffer.c
 *
 * Created: 23.11.2018 00:54:30
 *  Author: johangh
 */ 
#include "bit_manipulation.h"
#include "memory.h"
#include "buffer.h"
#include <stdint.h>
#include <stdio.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/pgmspace.h>

void oled_update_buffer(uint8_t row, uint8_t column){
	uint8_t page = row/OLED_PAGES;
	uint8_t data;
	data = row % OLED_PAGES;
	uint8_t new_data = (1 << data);
	address_sram[(OLED_WIDTH)*page+column] |= new_data;
}
void oled_buffer_init(){
	// looping through SRAM, writing all pages for the display
	for(int col = 0; col < OLED_WIDTH; col++){
		for (int pages = 0; pages < OLED_PAGES; pages++){
			address_sram[(OLED_WIDTH)*pages + col] = 0;
		}
	}
}

void oled_buffer_display(){
	for (int col = 0; col < OLED_WIDTH; col++){
		for (int pages = 0; pages < OLED_PAGES; pages++){
			oled_goto_page(pages);
			oled_goto_column(col);
			oled_write_d(address_sram[(OLED_WIDTH*pages)+col]);
		}
	}
}

void oled_buffer_rectangle(uint8_t posX,uint8_t posY, uint8_t lengthX, uint8_t lengthY,uint8_t fill){
	for (int y = posY; y < posY+lengthY; y++){
		for(int x = posX; x < posX+lengthX; x++){
			if (fill){
				oled_update_buffer(y,x);
			}
			else if (y == posY || y == lengthY-1 || x == posX || x == lengthX-1){
				oled_update_buffer(y,x);
			}
		}
	}
}