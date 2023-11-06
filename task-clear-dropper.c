#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "task-clear-dropper.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>


void task_clear_dropper(void *param)
{
    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_YELLOW);

    /* Repeatedly running part of the task */
    for (;;)
    {
        if (dist_sensor_val > 10)
        {
            rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);
            rslt = cyhal_pwm_start(&lin_back_pwm_obj);
        }
        else 
        {
            rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);
            rslt = cyhal_pwm_stop(&lin_back_pwm_obj);
            // After this, probably should not check distance anymore.
        }
    }
}