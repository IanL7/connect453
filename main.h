/*
 * main.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "drivers/leds.h"
#include "drivers/push_button.h"
#include "drivers/console.h"
#include "drivers/i2c.h"
#include "drivers/eeprom.h"
//#include "drivers/io_expander.h"
#include "drivers/LM75.h"

#define BIT_0       (1 << 0)
#define BIT_1       (1 << 1)
#define BIT_2       (1 << 2)
#define BIT_3       (1 << 3)
#define BIT_4       (1 << 4)
#define BIT_5       (1 << 5)
#define BIT_6       (1 << 6)
#define BIT_7       (1 << 7)

#define RGB_GREEN   1
#define RGB_YELLOW  2
#define RGB_RED     3
#define RGB_OFF     4

#define STATE_PAUSE         0
#define STATE_INIT          1
#define STATE_P1_TURN       2
#define STATE_P2_TURN       3
#define STATE_PLAYER1_WIN   4
#define STATE_PLAYER2_WIN   5
#define STATE_MOVE_ERROR    6

#define PB_PRESSED      false
#define PB_NOT_PRESSED  true

#define ALL_EVENTS_MASK             0xFFFFFFFF
#define EVENT_PASS_TURN_MASK        BIT_0
#define EVENT_PAUSE_MASK            BIT_1 
#define EVENT_DROPPER_CLEAR_MASK    BIT_2   
#define EVENT_PLAYER1_WIN           BIT_3
#define EVENT_PLAYER2_WIN           BIT_4
#define EVENT_MOVE_ERROR            BIT_5

#define PIN_PASS_TURN_PB    P10_4
#define PIN_PAUSE_PB        P10_2
#define PIN_PLAYER1_LED     P10_1
#define PIN_PLAYER2_LED     P9_4
#define PIN_ONOFF_LED       P5_1

extern cyhal_pwm_t lin_fore_pwm_obj;
extern cyhal_pwm_t lin_back_pwm_obj;
extern cyhal_pwm_t servo_pwm_obj;
extern cyhal_pwm_t game_state_led_b;
extern cyhal_pwm_t game_state_led_r;
extern cyhal_pwm_t game_state_led_g;

extern void rgb_on(cyhal_pwm_t *rgb_obj_r, cyhal_pwm_t *rgb_obj_g, cyhal_pwm_t *rgb_obj_b, int color);
extern QueueHandle_t state_queue;


#endif /* MAIN_H_ */
