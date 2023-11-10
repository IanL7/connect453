#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

/*******************************************************************************
* Global Variables
******************************************************************************/
cyhal_pwm_t lin_fore_pwm_obj;
cyhal_pwm_t lin_back_pwm_obj;
cyhal_pwm_t servo_pwm_obj;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

void pwm_init(int lin_hz, int lin_duty)
{
    cy_rslt_t rslt;

    /////////////////////////////////////////////////////////////////
    // Linear Actuator Foreward
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&lin_fore_pwm_obj, P5_6, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&lin_fore_pwm_obj, lin_duty, lin_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);

    /////////////////////////////////////////////////////////////////
    // Linear Actuator Backward
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&lin_back_pwm_obj, P7_7, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&lin_back_pwm_obj, lin_duty, lin_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&lin_back_pwm_obj);
    
    /////////////////////////////////////////////////////////////////
    // Servo (dropper unit)
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    //rslt = cyhal_pwm_init(&servo_pwm_obj, P6_3, NULL);
    /* Set duty cycle */
    //rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, servo_duty, servo_hz);
    /* Stop the PWM output */
    //rslt = cyhal_pwm_stop(&servo_pwm_obj);
}

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    __enable_irq();

    console_init();

    // SERVO: ?hz, ?% duty cycle
    pwm_init(250000, 50);

    // LINEAR ACTUATOR: ?hz ?% duty cycle

    /////////////////////////////////////////////////////////////////
    // TEST:
    // 1. Move linear actuator foreward for 5 sec
    // 2. Wait 5 sec
    // 3. Move linear actuator backward for 5 sec
    // 4. Wait 5 sec
    // 5. Move servo for 5 sec
    /////////////////////////////////////////////////////////////////

    // Linear Actuator:

    /* Foreward */
    rslt = cyhal_pwm_start(&lin_fore_pwm_obj);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Stop */
    rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Backward */
    rslt = cyhal_pwm_start(&lin_back_pwm_obj);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Stop */
    rslt = cyhal_pwm_stop(&lin_back_pwm_obj);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    // Servo:

    /* Go */
    //rslt = cyhal_pwm_start(&servo_pwm_obj);

    /* Delay for observing the output */
    //cyhal_system_delay_ms(5000);

    /* Stop */
    //rslt = cyhal_pwm_stop(&servo_pwm_obj);

    for (;;)
    {
    }
}