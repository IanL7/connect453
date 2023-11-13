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

#define STATE_P1_TURN       2
#define STATE_P2_TURN       3

#define PIN_SERVO           P6_3
#define PIN_DEPOSIT_PB      P10_4
#define PIN_PAUSE_PB        P10_2
#define PIN_PLAYER1_LED     P10_1
#define PIN_PLAYER2_LED     P9_4
#define PIN_ONOFF_LED       P5_1

#define PB_PRESSED      false
#define PB_NOT_PRESSED  true

#define ALL_EVENTS_MASK             0xFFFFFFFF
#define EVENT_DEPOSIT_MASK          BIT_0

#endif /* MAIN_H_ */
