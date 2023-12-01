/*
 * light_sensor_task.h
 *
 *  Created on: Nov 30, 2023
 *      Author: Ian Lodes
 */

#ifndef LIGHT_SENSOR_TASK_H_
#define LIGHT_SENSOR_TASK_H_

/******************************************************************************
 * Include header files
 *****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

extern QueueHandle_t xLightQueue;

/* Function Prototypes */
void light_sensor_init();

#endif /* LIGHT_SENSOR_TASK_H_ */