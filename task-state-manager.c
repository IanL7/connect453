#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "task-state-manager.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

QueueHandle_t state_queue = NULL;

void task_state_manager(void *param)
{
    static int STATE;

    cy_rslt_t rslt;

    uint8_t received_state;

    /* Suppress warning for unused parameter */
    (void)param;

    STATE = STATE_INIT;

    state_queue = xQueueCreate(1, sizeof(uint8_t))

    if (state_queue == NULL)
    {
        // Queue not created
    }

    for (;;) 
    {
        received_state = xQueueReceive(state_queue, )
        switch (STATE)
        {
            case STATE_INIT:
                break;
        }
    }



}