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
EventGroupHandle_t xConnectFourEventGroup;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

void pwm_init()
{
    cy_rslt_t rslt;

    /////////////////////////////////////////////////////////////////
    // Servo (dropper unit)
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&servo_pwm_obj, PIN_SERVO, NULL);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&servo_pwm_obj);
}

void task_pole_deposit_pb(void *param)
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
        curr_pb_state = cyhal_gpio_read(PIN_DEPOSIT_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            printf("    - PB pressed!\n\r");
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_DEPOSIT_MASK);
        }
        prev_pb_state = curr_pb_state;
    }
}

void task_servo_deposit(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    cy_rslt_t rslt = cybsp_init();

    for (;;)
    {
        xEventGroupWaitBits(
            xConnectFourEventGroup, 
            EVENT_DEPOSIT_MASK,
            pdTRUE,
            pdTRUE,
            portMAX_DELAY);

        printf("Moving to deposit\r\n");
        rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, 5, 50);
        rslt = cyhal_pwm_start(&servo_pwm_obj);
        vTaskDelay(2000);

        printf("Moving to retrieve\r\n");
        rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, 12.5, 50);
        vTaskDelay(2000);

        rslt = cyhal_pwm_stop(&servo_pwm_obj);
    }
}

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    __enable_irq();

    console_init();

    pwm_init();

    // Init PB
    rslt = cyhal_gpio_init(PIN_DEPOSIT_PB, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

    xConnectFourEventGroup = xEventGroupCreate();

    if (xConnectFourEventGroup == NULL)
    {
        printf("XXX ERROR: Event group not created\n\r");
    }

    xTaskCreate(
        task_pole_deposit_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);

    xTaskCreate(
        task_servo_deposit,
        "State Manager",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);

    printf("* --- Starting task scheduler                               --- *\n\r");
    vTaskStartScheduler();

    for (;;)
    {
    }
}