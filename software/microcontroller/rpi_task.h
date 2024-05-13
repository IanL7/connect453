/*
 * light_sensor_task.h
 *
 *  Created on: Nov 30, 2023
 *      Author: Billy Begos and Ian Lodes
 */

#ifndef RPI_TASK_H_
#define RPI_TASK_H_

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

extern QueueHandle_t xBoardQueue;

/* Function Prototypes */
void task_rpi(void *param);
static bool equal(char board1[], char board2[]);
static void init_prev_board_state();

#endif /* RPI_TASK_H_ */