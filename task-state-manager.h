/*
 * task-state-manager.h
 *
 *  Created on: Nov 4, 2023
 *      Author: Ian Lodes
 */

#ifndef TASKSTATEMANAGER_H_
#define TASKSTATEMANAGER_H_

#include "drivers/leds.h"
#include "drivers/push_button.h"
#include "drivers/console.h"
#include "drivers/i2c.h"
#include "drivers/eeprom.h"
//#include "drivers/io_expander.h"
#include "drivers/LM75.h"
#include "main.h"

extern QueueHandle_t state_queue;

#define STATE_PAUSE     = 0
#define STATE_INIT      = 1
#define STATE_P1_TURN   = 2
#define STATE_P2_TURN   = 3
#define STATE_GAME_OVER = 4

#endif /* TASKSTATEMANAGER_H_ */