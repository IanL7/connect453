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

extern cyhal_pwm_t lin_fore_pwm_obj;
extern cyhal_pwm_t lin_back_pwm_obj;
extern cyhal_pwm_t servo_pwm_obj;
extern cyhal_pwm_t game_state_led_b;
extern cyhal_pwm_t game_state_led_r;
extern cyhal_pwm_t game_state_led_g;

#endif /* MAIN_H_ */
