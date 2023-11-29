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

void pwm_init(int hz, int duty)
{
    cy_rslt_t rslt;

    /////////////////////////////////////////////////////////////////
    // Linear Actuator Foreward
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&lin_fore_pwm_obj, P5_6, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&lin_fore_pwm_obj, duty, hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);

    /////////////////////////////////////////////////////////////////
    // Linear Actuator Backward
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&lin_back_pwm_obj, P7_7, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&lin_back_pwm_obj, duty, hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&lin_back_pwm_obj);
}

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    __enable_irq();

    console_init();

    pwm_init(1000, 75);

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

    //rslt = cyhal_gpio_init(P5_6, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    //rslt = cyhal_gpio_init(P7_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    /* Write the value to the output pin */

    /* Foreward */
    printf("Backward\n\r");
    cyhal_pwm_start(&lin_back_pwm_obj);
    //cyhal_gpio_write(P5_6, 1);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Stop */
    cyhal_pwm_stop(&lin_back_pwm_obj);
    printf("Stop\n\r");
    //cyhal_gpio_write(P7_7, 1);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Backward */
    printf("Forward\n\r");
    cyhal_pwm_start(&lin_fore_pwm_obj);
    //cyhal_gpio_write(P5_6, 0);

    /* Delay for observing the output */
    cyhal_system_delay_ms(5000);

    /* Stop */
    cyhal_pwm_stop(&lin_fore_pwm_obj);
    printf("Stop\n\r");
    //cyhal_gpio_write(P5_6, 1);

    /* Delay for observing the output */
    cyhal_system_delay_ms(500);

    for (;;)
    {
    }
}