/*
 * connect453_lib.h
 *
 *  Created on: Dec 9, 2023
 *      Author: Ian Lodes
 */

#ifndef CONNECT453_LIB_H_
#define CONNECT453_LIB_H_

/******************************************************************************
 * Include header files
 *****************************************************************************/

/* Function Prototypes */
void set_rgb(int led, int state);
uint8_t process_board(char board_state_curr[43]);
void lin_act_init();
//void servo_pwm_init();
void servo_gpio_init();
void control_lin(int action);
//void deposit(int column);
void play_sound(int sound);
void leds_init();
void pbs_init();
bool board_clear();

#endif /* CONNECT453_LIB_H_ */