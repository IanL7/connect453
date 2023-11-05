#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "audio.h"
#include "connect4-tasks.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

void rgb_on(cyhal_pwm_t *rgb_obj_r, cyhal_pwm_t *rgb_obj_g, cyhal_pwm_t *rgb_obj_b, int color)
{
   switch (color)
   {
    case RGB_GREEN:
        cyhal_pwm_stop(rgb_obj_r);
        cyhal_pwm_stop(rgb_obj_b);
        cyhal_pwm_start(rgb_obj_g);
    
    case RGB_RED:
        cyhal_pwm_start(rgb_obj_r);
        cyhal_pwm_stop(rgb_obj_b);
        cyhal_pwm_stop(rgb_obj_g);
    
    case RGB_YELLOW:
        cyhal_pwm_stop(rgb_obj_r);
        cyhal_pwm_start(rgb_obj_b);
        cyhal_pwm_start(rgb_obj_g);

    default:
        cyhal_pwm_stop(rgb_obj_r);
        cyhal_pwm_stop(rgb_obj_b);
        cyhal_pwm_stop(rgb_obj_g);
   }
}


// Put this on the back-burner
// Should have highest priority
void task_pause(void *param)
{
    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_YELLOW);

    rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);
    rslt = cyhal_pwm_stop(&lin_back_pwm_obj);
    rslt = cyhal_pwm_stop(&servo_pwm_obj);

    // vTaskSuspend() all tasks? Or maybe have this be in the "calling" task (all of them)
    // Deferred Interrupt Handling (freertos)?

    for (;;)
    {
        // If pause button pressed again, leave pause state and resume
    }
}

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
            vTaskDelay()
        }
    }
}