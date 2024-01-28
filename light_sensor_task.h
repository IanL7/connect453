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
#include "main.h"

#define ALS_SUBORDINATE_ADDR 0x29

/* Function Prototypes */
void light_sensor_init();
uint16_t read_light_sensor();

#endif /* LIGHT_SENSOR_TASK_H_ */