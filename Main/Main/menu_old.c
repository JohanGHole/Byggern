/*
 * menu.c
 *
 * Created: 03.10.2018 12:21:21
 *  Author: johangh
 */ 
#define F_CPU 4915200 // clock frequency in Hz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "sram.h"
#include "joystick.h"
#include "adc.h"
#include "memory.h"
#include "bit_manipulation.h"
#include "test.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include "MCP2515.h"

menu_node* main_menu;
menu_node* current_menu;
uint8_t difficulty = EASY;
double score = 0;

menu_node* menu_create(char* name,void *myfunction(void), uint8_t page_nr){
	menu_node* menu = malloc(sizeof(menu_node));
	menu->name = name;
	menu->page_id = page_nr;
	menu->sibling_left = NULL;
	menu->sibling_right = NULL;
	menu->child_menu = NULL;
	menu->parent_menu = NULL;
	menu->menu_function = myfunction;
	menu->offset = "  ";
	return menu;
}

void menu_init(){
	main_menu = menu_create("Main Menu",NULL,0);
	
	/* --- play_pong --- */
	
	main_menu->child_menu = menu_create("Play Pong",&menu_play_pong,1);
	menu_node* play_pong = main_menu->child_menu;
	play_pong->parent_menu = main_menu;
	
	/* --- play_pong submenu -> replay? --- */
	
	play_pong->child_menu = menu_create("Replay?",NULL,1);
	menu_node* replay = play_pong->child_menu;
	replay->parent_menu = play_pong;
	
	/* --- replay submenu -> yes! --- */
	
	replay->child_menu = menu_create("Yes!",&menu_replay_yes,1);
	menu_node * yes = replay->child_menu;
	yes->parent_menu = replay;
	
	/* --- replay submenu -> no =( --- */
	
	yes->sibling_right = menu_create("No=(",&menu_replay_no,2);
	menu_node* no = yes->sibling_right;
	no->sibling_left = yes;
	no->parent_menu = replay;	
	
	/*  Difficulty --- */
	
	play_pong->sibling_right = menu_create("Difficulty", &menu_difficulty,2);
	menu_node* difficulty = play_pong->sibling_right;
	difficulty->sibling_left = play_pong;
	difficulty->parent_menu = main_menu;
	
	/* Difficulty submeny -> Easy --- */
	
	difficulty->child_menu = menu_create("Easy", &menu_difficulty_easy,1);
	menu_node* easy = difficulty->child_menu;
	easy->parent_menu = difficulty;
	
	/* Difficulty submeny -> Medium --- */
	
	easy->sibling_right = menu_create("Medium", &menu_difficulty_medium,2);
	menu_node* medium = easy->sibling_right;
	medium->sibling_left = easy;
	medium->parent_menu = difficulty;
	
	/* Difficulty submeny -> Insane --- */
	
	medium->sibling_right = menu_create("Insane", &menu_difficulty_insane,3);
	menu_node* insane = medium->sibling_right;
	insane->sibling_left = medium;
	insane->parent_menu = difficulty;
	
	/* --- Help --- */
	
	difficulty->sibling_right = menu_create("Help",&menu_help,3);
	menu_node* help = difficulty->sibling_right;
	help->sibling_left = difficulty;
	help->parent_menu = main_menu;
	
	/* --- set global current menu --- */
	
	current_menu = main_menu;
	menu_display(current_menu);
}


void menu_display(menu_node* menu){
	oled_reset();
	if (menu->child_menu == NULL){
		return;
	}
	menu_node* current = menu->child_menu;
	/* start by printing the menu name (parent menu) */
	oled_goto_page(0);
	oled_goto_column(0);
	oled_printf("---");
	oled_printf(current->parent_menu->name);
	oled_printf("---");
	oled_goto_page(1);
	oled_goto_column(0);
	oled_printf("->");
	oled_printf(current->name);
	uint8_t i = 2;
	while (current->sibling_right != NULL){
		current = current->sibling_right;
		oled_goto_page(i);
		oled_goto_column(0);
		oled_printf("  ");
		oled_printf(current->name);
		i++;
	} 
	current_menu = menu->child_menu;
}


void menu_move(){
	if (joystick_find_direction() == UP){
		if (current_menu->sibling_left == NULL || current_menu->page_id == 1){
			return;
		}
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf(current_menu->offset);
		current_menu = current_menu->sibling_left;
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf("->");
	}
	else if (joystick_find_direction() == DOWN){
		if (current_menu->sibling_right == NULL){
			return;
		}
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf(current_menu->offset);
		current_menu = current_menu->sibling_right;
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf("->");
	}
	else if (joystick_find_direction() == LEFT){
		if (current_menu->name == main_menu->name){
			//Do nothing, preventing main menu to load itself when direction = LEFT
		}
		else {
			current_menu = main_menu;
			menu_display(main_menu);
		}
	}
}

menu_node* menu_get_current_menu(){
	return current_menu;
}

/* --------- MENU NODE FUNCTIONS ------------- */

void menu_play_pong(){
	menu_reset_score();
	current_menu = main_menu->child_menu;
	oled_reset();
	oled_goto_page(2);
	oled_goto_column(0);
	oled_printf("   Good luck");
	oled_goto_page(3);
	oled_goto_column(0);
	oled_printf("   have fun!");
	menu_timer_init();
	while(1){
		_delay_ms(100);
		CAN_send(CAN_package(1,menu_get_difficulty()));
		if(CAN_recieve().data[0] == 1){ // if node 2 sends game_over can package
			oled_reset();
			oled_goto_column(0);
			oled_goto_page(0);
			oled_printf("-- Game over --");
			CAN_send(CAN_package(0,menu_get_difficulty()));
			oled_goto_column(0);
			oled_goto_page(1);
			oled_printf("score: %d",menu_get_score());
			_delay_ms(8000);
			current_menu = current_menu->child_menu;
			menu_display(current_menu);
			break;
		}
	}
	menu_reset_score();
}
void menu_replay_yes(){
	current_menu = main_menu->child_menu;
	current_menu->menu_function();
}
void menu_replay_no(){
	oled_reset();
	current_menu = main_menu->child_menu;
	menu_display(main_menu);
}

void menu_difficulty(){
	oled_reset();
	menu_display(main_menu->child_menu->sibling_right);
}


void menu_difficulty_easy(){
	difficulty = EASY;
	menu_display(main_menu);
}

void menu_difficulty_medium(){
	difficulty = MEDIUM;
	menu_display(main_menu);
}

void menu_difficulty_insane(){
	difficulty = INSANE;
	menu_display(main_menu);
}

uint8_t menu_get_difficulty(){
	return difficulty;
}

void menu_help(){
	oled_reset();
	oled_goto_page(0);
	oled_goto_column(0);
	oled_printf("--- HELP ---");
	oled_goto_page(1);
	oled_goto_column(0);
	oled_printf("Use the right   slider to navi- gate. To adjust the angel use   the joystick.   Try not to loosethe ball. GL");
	
}

/* ---------- MENU TIMER FUNCTIONS ------------ */

void menu_timer_init(){
	cli();
	// enable overflow interupt
	set_bit(TIMSK,TOIE0);
	// set prescalar
	set_bit(TCCR0,CS02);
	clear_bit(TCCR0,CS01);
	set_bit(TCCR0,CS00);
	
	//set normal mode
	clear_bit(TCCR0,WGM00);
	clear_bit(TCCR0,WGM01);
	// should now have a timer interrupt every 53 ms -> (1/(4915200 / 1024 (prescaler) /256 (1 byte))
	sei();
}

ISR(TIMER0_OVF_vect){
	score += 0.053; // since the overflow flag is set every 53 ms
}
uint8_t menu_get_score(){
	return score;
}
void menu_reset_score(){
	score = 0;
}
void menu_save_score(){
	// save highscore in EEPROM, implement if time. 
}