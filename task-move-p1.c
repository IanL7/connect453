#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "task-move-p1.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>


void task_move_p1(void *param)
{
    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);

    /* Repeatedly running part of the task */
    for (;;)
    {
        if (!cyhal_gpio_read(P10_4))
        {
            // Debounce?
            vTaskDelay(100)
            if (!cyhal_gpio_read(P10_4))
            {
                // Go to player 2 state
            }
        }
    }
}