#include "bit_manipulation.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include "oled.h"
#include "fonts.h"
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/pgmspace.h>
static FILE oled_stdout = FDEV_SETUP_STREAM(oled_print,NULL, _FDEV_SETUP_WRITE);

static uint8_t column = 0;
static uint8_t page = 0;
static uint8_t font_width = 8;
uint8_t page_data = 0;
void oled_write_c(char command){
	*address_oled_cmd = command;
}
void oled_write_d(char data){
	*address_oled_data = data;
}
char oled_get_address_data(){
	return *address_oled_data;
}

void oled_init(void){
	      oled_write_c(0xae);        //  display  off
	      oled_write_c(0xa1);        //segment  remap
	      oled_write_c(0xda);        //common  pads  hardware:  alternative
	      oled_write_c(0x12);
	      oled_write_c(0xc8);        //common output scan direction:com63~com0
	      oled_write_c(0xa8);        //multiplex  ration  mode:63
	      oled_write_c(0x3f);
	      oled_write_c(0xd5);        //display divide ratio/osc. freq. mode
	      oled_write_c(0x80);        
	      oled_write_c(0x81);        //contrast  control
	      oled_write_c(0x50);
	      oled_write_c(0xd9);        //set  pre-charge  period
	      oled_write_c(0x21);
	      oled_write_c(0x20);        //Set  Memory  Addressing  Mode
	      oled_write_c(0x02);        //Page-addressing mode
	      oled_write_c(0xdb);        //VCOM  deselect  level  mode
	      oled_write_c(0x30);
	      oled_write_c(0xad);        //master  configuration
	      oled_write_c(0x00);
	      oled_write_c(0xa4);        //out  follows  RAM  content
	      oled_write_c(0xa6);        //set  normal  display
	      oled_write_c(0xaf);        //  display  on
}

void oled_reset(void){
	for (int page = 0; page < OLED_PAGES; page++){
		oled_clear_page(page);
	}
	oled_goto_column(0);
	oled_goto_page(0);
	page_data = 0;
}
void oled_clear_page(uint8_t my_page){
	oled_goto_page(my_page);
	oled_goto_column(0);
	for(int i = 0; i < 128; i++){
		oled_write_d(0);
	}
}
void oled_goto_page(uint8_t my_page){
	oled_write_c(0xb0 + my_page);
	page = my_page;
}
void oled_goto_column(uint8_t my_column){
	column = my_column;
	oled_write_c(0b00001111 & my_column);
	oled_write_c(0x10+((0b11110000 & my_column)>> 4));
	
}
void oled_test(){
	oled_goto_page(0);
	oled_goto_column(0);
	*address_oled_data = 1;
	/*
	for (int j = 0; j < 8; j++){
		oled_goto_page(0);
		for(int i = 0; i < 128; i++){
			*address_oled_data = 1;
		}
	}
	*/
}
void oled_draw_matrix() {
	//horisontal lines
	for(int j = 0; j < OLED_PAGES; j++){
		oled_goto_page(j);
		oled_goto_column(0);
		for (int i = 0; i < OLED_WIDTH; i++){
			if (j == OLED_PAGES-1){
				oled_write_d(0b10000001);
			}
			else {
				oled_write_d(0x01);
			}
			_delay_ms(1);
		}
	}
	for(int j = 0; j < OLED_PAGES; j++){
		oled_goto_page(j);
		for (int i = 0; i < OLED_PAGES*2; i++){
			if (i == OLED_PAGES*2-1){
				oled_goto_column(127);
				oled_write_d(0xff);
			}
			oled_goto_column(i*8);
			oled_write_d(0xFF);
		}
	}

	/*
	oled_goto_page(0);
	for (int j = 0; j < 16; j++){
		for (int i = 0; i < OLED_WIDTH; i++){
			oled_goto_column(i,j);
			oled_write_d(1);
			_delay_ms(30);
		}
	}
	*/
	
}
 void oled_print(char letter){
	 oled_goto_page(page);
	 oled_goto_column(column);
	 char *byte;
	 if (font_width == 8){
		 byte = font8[letter-FONT_OFFSET];
	 }
	 if (font_width == 5){
		byte = font5[letter-FONT_OFFSET];
	 }
	 if (font_width == 4){
		 byte = font4[letter-FONT_OFFSET];
	 }
	 if (((column+font_width) >  OLED_WIDTH)){
		 oled_goto_page(page+1);
		 oled_goto_column(0);
		 if (page == 8){
			 oled_goto_page(0);
		 }
	 }
	 for (int i = 0; i < font_width; i++){
		 oled_write_d(pgm_read_byte(byte + i));
		 column += 1;
	 }
 }
 
 void oled_pos(uint8_t row,uint8_t column){
	 uint8_t page = row/OLED_PAGES;
	 uint8_t data;
	data = row % OLED_PAGES;
	 if (data == 0){
		 page_data = 0;
	 }
	 uint8_t new_data = (1 << data ); 
	 // to prevent us from overwriting the previous values on the column page
	 new_data |= page_data;
	 page_data = new_data;
	 oled_goto_page(page);
	 oled_goto_column(column);
	 oled_write_d(new_data);
 }
 
 void oled_draw_smiley(uint8_t row, uint8_t column){
	 oled_pos(row+3,column);
	 oled_pos(row+1,column+1);
	 oled_pos(row+2,column+1);
	 oled_pos(row+4,column+1);
	 oled_pos(row+4,column+2);
	 oled_pos(row+4,column+3);
	 oled_pos(row+4,column+4);
	 oled_pos(row+1,column+4);
	 oled_pos(row+2,column+4);
	 oled_pos(row+3,column+5); 
 }
 
 void oled_set_width(uint8_t width){
	 font_width = width;
 }
 
 void oled_printf(const char* fmt, ...){
	 va_list args;
	 va_start(args, fmt);
	 vfprintf(&oled_stdout, fmt, args);
	 va_end(args);
 }