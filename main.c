#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>
/*******************************************************************************
* External Global Variables
******************************************************************************/
void task_blink_led(void *param);

int main(void)
{
    cy_rslt_t rslt;
    
    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    console_init();
    printf("* --- PSoC6 INIT successful             --- *\n\r");
    __enable_irq();
    printf("* --- Enabling IRQ successful           --- *\n\r");
    
    // Power LED
    rslt = cyhal_gpio_init(
        P5_1,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    // create a task to blink the onboard LED
    xTaskCreate(
        task_blink_led,
        "Blink LED Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);
    printf("* --- FreeRTOS task creation successful --- *\n\r");
    printf("* --- Starting scheduler                --- *\n\r");
    // Start the scheduler
    vTaskStartScheduler();
    for (;;)
    {
    }
}
void task_blink_led(void *param)
{
    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    

    // Initialize the pins that control the RGB LED
    rslt = cyhal_gpio_init(
        P5_5,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    rslt = cyhal_gpio_init(
        P5_2,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    rslt = cyhal_gpio_init(
        P5_3,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    // Initialize the pin that control the LED
    rslt = cyhal_gpio_init(
        P9_4,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }
    /* Repeatedly running part of the task */
    for (;;)
    {
        vTaskDelay(300);
        cyhal_gpio_write(P5_2, true);
        cyhal_gpio_write(P5_3, false);
        cyhal_gpio_write(P5_5, false);
        vTaskDelay(300);
        cyhal_gpio_write(P5_2, false);
        cyhal_gpio_write(P5_3, true);
        cyhal_gpio_write(P5_5, false);
        vTaskDelay(300);
        cyhal_gpio_write(P5_2, false);
        cyhal_gpio_write(P5_3, false);
        cyhal_gpio_write(P5_5, true);
    }
}