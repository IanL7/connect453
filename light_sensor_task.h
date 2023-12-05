/*
 * light_sensor_task.h
 *
 *  Created on: Nov 30, 2023
 *      Author: Christian VanIten and Ian Lodes
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

#define ALS_SUBORDINATE_ADDR 0x29

/* Function Prototypes */
void light_sensor_init();
void task_light_sensor(void *pvParameters);
void test_light_sensor();

#endif /* LIGHT_SENSOR_TASK_H_ */