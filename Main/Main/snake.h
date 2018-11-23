/*
 * snake.h
 *
 * Created: 19.11.2018 23:20:37
 *  Author: einarlor
 */ 


#ifndef SNAKE_H_
#define SNAKE_H_

void snake_setup(GAME_MODE diff);

void snake_play_snake(GAME_MODE diff);

void snake_draw();

void snake_logic();

void snake_input();

void snake_draw_pixel(x,y);


#endif /* SNAKE_H_ */