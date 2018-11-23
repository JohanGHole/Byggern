/*
 * PWM.h
 *
 * Created: 07.11.2018 22:41:35
 *  Author: johangh
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

void PWM_init();

void PWM_set_pulse(float ms);

//Converts slider position (0-255) to PWM pulse (0.9 - 2.1 ms)
float PWM_joystick_to_PWM(uint8_t position);

#endif /* PWM_H_ */