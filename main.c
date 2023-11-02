#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "audio.h"
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

void mcu_all_leds_init()
{
    // TODO: Add serial indication if an error occurred for a specific LED
    cy_rslt_t rslt;

    // LED1
    rslt = cyhal_gpio_init(
        P5_1,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // LED 2
    rslt = cyhal_gpio_init(
        P9_4,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // LED 3
    rslt = cyhal_gpio_init(
        P10_1,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 1.1
    rslt = cyhal_gpio_init(
        P5_2,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 1.2
    rslt = cyhal_gpio_init(
        P5_3,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 1.3
    rslt = cyhal_gpio_init(
        P5_5,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 2.1
    rslt = cyhal_gpio_init(
        P12_7,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 2.2
    rslt = cyhal_gpio_init(
        P12_6,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // RGB LED 2.3
    rslt = cyhal_gpio_init(
        P10_2,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        true);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }
}

void mcu_all_pbs_init()
{
    // TODO: Add serial indication if an error occurred for a specific PB
    cy_rslt_t rslt;

    // PB 1
    rslt = cyhal_gpio_init(
        P10_4,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false
    );

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }

    // PB 2
    rslt = cyhal_gpio_init(
        P10_2,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false
    );

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
        };
    }
}

void mcu_startup_sound()
{
    // DAC should output 3.3V at max sample value (0xFFFF)
    uint32_t sample;

    cyhal_dac_t my_dac_obj;
    cy_rslt_t dac_result;
    uint16_t dac_read;

    /* Initialize DAC, set Pin 9[6] as the DAC output */
    dac_result = cyhal_dac_init(&my_dac_obj, P9_6);

    /* Check the return of cyhal_dac_init to confirm initialization was successful */
    if (dac_result == CY_RSLT_SUCCESS)
    {
        // Play the startup sound
        for (sample = 0; sample < 85036; sample++)
        {
            /* Write the 16 bit value as DAC input */
            cyhal_dac_write(&my_dac_obj, startup[sample]);
            /* Return the 16 bit DAC register value */
            dac_read = cyhal_dac_read(&my_dac_obj);
            if (dac_read == sample)
            {
                /* Insert code */
            }
            // TODO: is this enough? Do I need a timer instead?
            // Delay for ~1/24000ths of a second, (sample rate of audio array is 24khz)
            cyhal_system_delay_us(42);
        }
    }
    /* Release DAC object after use by calling cyhal_dac_free, this de-initializes the DAC (including its output) */
    cyhal_dac_free(&my_dac_obj);
}

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    mcu_all_leds_init();
    mcu_all_pbs_init();

    __enable_irq();

    // Startup sound
    mcu_startup_sound();

    // create a task to blink the onboard LED
    xTaskCreate(
        task_blink_led,
        "Blink LED Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);
    
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


    /* Repeatedly running part of the task */
    for (;;)
    {
        vTaskDelay(300);
        cyhal_gpio_toggle(P5_5);
    }
}
