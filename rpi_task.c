/*
 * rpi_task.c
 *
 *  Created on: Nov 30, 2023
 *      Authors: Billy Begos and Ian Lodes
 */

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "main.h"
#include "rpi_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "console.h"


void task_rpi(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    while(1)
    {
        vTaskDelay(100);

        if(ALERT_CONSOLE_RX)
        {
            printf("Rx'd: %s\n", pcInputString);
            
            memset(pcInputString, 0, 500);
            cInputIndex = 0;
            ALERT_CONSOLE_RX = false;
        }
    }
}