/*
 * snake.c
 *
 * Created: 19.11.2018 12.50.37
 *  Author: Johan Gangsås Hole
 */ 
#define F_CPU 4915200 // clock frequency in Hz

#include <avr/io.h>
/*
 * snake.c
 *
 * Created: 19.11.2018 12.50.37
 *  Author: Johan Gangsås Hole
 */ 
#define F_CPU 4915200 // clock frequency in Hz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "UART.h"
#include "sram.h"
#include "joystick.h"
#include "adc.h"
#include "memory.h"
#include "bit_manipulation.h"
#include "test.h"
#include "buffer.h"
#include "menu.h"
#include "snake.h"

#define  WIDTH 32
#define HEIGHT 16
uint8_t gameOver;
#define MAX_SNAKE_LENGTH 50
int8_t fruitX, fruitY, score;
int8_t x,y;
int8_t tailX[MAX_SNAKE_LENGTH], tailY[MAX_SNAKE_LENGTH];
uint8_t nTail;
DIRECTION dir;
DIRECTION last_direction;
GAME_MODE diff = EASY;

void snake_setup(GAME_MODE difficulty){
	diff = difficulty;
	gameOver = 0;
	dir = NEUTRAL;
	x = WIDTH / 2;
	y = HEIGHT / 2;
	rand();
	// yield different seeds each time (unless srand() is called before thisfunction)
	uint16_t seed = ADC_read(CH_RIGHT_SLIDER);//rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	fruitX = rand() % WIDTH;
	fruitY = rand() % HEIGHT;
	score = 0;
	nTail = 0;
}
void snake_play_snake(GAME_MODE difficulty){
	//int seed = time(NULL);
	snake_setup(difficulty);
	while (!gameOver){
		snake_draw();
		snake_input();
		snake_logic();
		if (difficulty == EASY){
			_delay_ms(100);
		}
		if (difficulty == MEDIUM){
			_delay_ms(80);
		}
		if (difficulty == INSANE){
			_delay_ms(40);
		}
	}
}

void snake_draw(){
	oled_buffer_init();
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// snake head
			if (i == y && j == x){
				snake_draw_pixel(j,i);
			}
			// snake "fruit"
			else if (i == fruitY && j == fruitX){
				snake_draw_pixel(j,i);
			}
			else {
				for (int k = 0; k < nTail; k++){
					if (tailX[k] == j && tailY[k] == i){
						snake_draw_pixel(j,i);
					}
				}
			}
		}
	}
	oled_buffer_display();
}
void snake_logic(){
	int8_t prevX = tailX[0];
	int8_t prevY = tailY[0];
	int8_t prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	// Updating body
	for (int i = 1; i < nTail; i++){
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
		case LEFT:
		x--;
		break;
		case RIGHT:
		x++;
		break;
		case UP:
		y--;
		break;
		case DOWN:
		y++;
		break;
		default:
		break;
	}
	printf("Before limits\n");
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){
		gameOver = 1;
		printf("Game over set in border cases \n");
	}
	for (int i = 0; i < nTail; i++){
		if (tailX[i] == x && tailY[i] == y){
			gameOver = 1;
			printf("Game over set in tail collision \n");
		}
	}
	
	if (x == fruitX && y == fruitY){
		score += 1;
		fruitX = rand() % WIDTH;
		fruitY = rand() % HEIGHT;
		nTail++;
	}
}
void snake_input(){
	last_direction = dir;
	dir = joystick_find_direction();
	printf("current direction: %d\n",dir);
	if ((dir == NEUTRAL) || (last_direction == LEFT && dir == RIGHT) || (last_direction == RIGHT && dir == LEFT) || (last_direction == UP && dir == DOWN) || (last_direction == DOWN && dir == UP)){
		dir = last_direction;
	}
	// if you want to break out of the game
}
void snake_draw_pixel(x,y){
	oled_buffer_rectangle(x*4,y*4,4,4,1);
}
