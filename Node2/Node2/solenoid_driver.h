/*
 * solenoid_driver.h
 *
 * Created: 12.11.2018 00:18:03
 *  Author: edvardsa
 */ 


#ifndef SOLENOID_DRIVER_H_
#define SOLENOID_DRIVER_H_

#include "PD.h"

void solenoid_init();

void solenoid_hit(uint8_t button_pressed, GAME_MODE game_difficulty);



#endif /* SOLENOID_DRIVER_H_ */