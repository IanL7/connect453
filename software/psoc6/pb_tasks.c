/*
 * pb_tasks.c
 *
 *  Created on: Nov 30, 2023
 *      Authors: Ian Lodes
 */

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "main.h"
#include "pb_tasks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "console.h"


void task_pole_passturn_pb(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    TickType_t xLastWakeTime;

    // PB check freq is 20ms
    const TickType_t xFrequency = 20/portTICK_PERIOD_MS;

    bool curr_pb_state = PB_NOT_PRESSED;
    bool prev_pb_state = PB_NOT_PRESSED;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency);


        // Get PB level
        curr_pb_state = cyhal_gpio_read(PIN_PASS_TURN_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
        }
        prev_pb_state = curr_pb_state;
    }
}

void task_pole_start_pb(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    TickType_t xLastWakeTime;

    // PB check freq is 20ms
    const TickType_t xFrequency = 20/portTICK_PERIOD_MS;

    bool curr_pb_state = PB_NOT_PRESSED;
    bool prev_pb_state = PB_NOT_PRESSED;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency);


        // Get PB level
        curr_pb_state = cyhal_gpio_read(PIN_START_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_START_GAME_MASK);
        }
        prev_pb_state = curr_pb_state;
    }
}