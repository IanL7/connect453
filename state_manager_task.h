/*
 * state_manager_task.h
 *
 *  Created on: Dec 9, 2023
 *      Author: Ian Lodes
 */

#ifndef STATE_MANAGER_TASK_H_
#define STATE_MANAGER_TASK_H_

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

extern QueueHandle_t xBoardQueue;
extern QueueHandle_t xLightQueue;
extern QueueHandle_t xPieceQueue;
extern TaskHandle_t flash_red_task_handle;
extern QueueHandle_t xBoardBLEQueue;

/* Function Prototypes */
void task_state_manager(void *param);
void task_flash_red(void *param);


#endif /* STATE_MANAGER_TASK_H_ */