/*
 * motor.h
 *
 * Created: 14.11.2018 10:32:23
 *  Author: einarlor
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

typedef enum  {
	LEFT,
	RIGHT
}direction;

void motor_init();

int16_t motor_read_encoder();

void motor_speed(uint8_t speed);

void motor_direction(direction dir);

void motor_calibration();

// joystick open loop motor control 
void motor_open_loop(uint8_t x_position);

int16_t motor_get_max_encoder();

int16_t motor_get_min_encoder();


#endif /* MOTOR_H_ */