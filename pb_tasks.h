/*
 * pb_tasks.h
 *
 *  Created on: Nov 30, 2023
 *      Author: Ian Lodes
 */

#ifndef PB_TASKS_H_
#define PB_TASKS_H_

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include <event_groups.h>

extern EventGroupHandle_t xConnectFourEventGroup;

/* Function Prototypes */
void task_pole_passturn_pb(void *param);
void task_pole_start_pb(void *param);

#endif /* PB_TASKS_H_ */