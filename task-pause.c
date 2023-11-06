#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "task-pause.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>

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