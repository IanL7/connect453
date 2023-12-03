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

/* Function Prototypes */
void task_rpi(void *param);

#endif /* RPI_TASK_H_ */