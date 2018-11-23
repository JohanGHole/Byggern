/*
 * PD.h
 *
 * Created: 15.11.2018 16:44:08
 *  Author: einarlor
 */ 


#ifndef PD_H_
#define PD_H_
typedef enum{
	EASY,
	MEDIUM,
	INSANE,
} GAME_MODE;


void PD_init();

int16_t PD_set_position_reference(int16_t ref);

void PD_set_game_mode(uint8_t difficulty);

uint8_t PD_get_highscore();

void PD_set_highscore();

#endif /* PD_H_ */