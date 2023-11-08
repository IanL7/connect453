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

/*******************************************************************************
* Static Global Variables (take away static when we have multiple files)
******************************************************************************/
static EventGroupHandle_t xConnectFourEventGroup;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////
void mcu_reg_leds_init()
{
    // TODO: Add serial indication if an error occurred for a specific LED
    cy_rslt_t rslt;

    // LED1 (ON - OFF)
    rslt = cyhal_gpio_init(
        PIN_ONOFF_LED,
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

    // LED 2 (Player 2 Turn)
    rslt = cyhal_gpio_init(
        PIN_PLAYER2_LED,
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

    // LED 3 (Player 1 Turn)
    rslt = cyhal_gpio_init(
        PIN_PLAYER1_LED,
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

void pwm_init(int rgb_hz, int rgb_duty)
{
    cy_rslt_t rslt;

    /////////////////////////////////////////////////////////////////
    // RGB1 (game state led)
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_b, P5_5, NULL);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_b, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_b);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_r, P5_3, NULL);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_r, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_r);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_g, P5_2, NULL);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_g, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_g);
}

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    __enable_irq();

    // RGB: 1000hz, 50% duty cycle
    pwm_init(1000, 50);

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);

    xTaskCreate(
        task_state_manager,
        "State Manager",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        NULL);
    
    xTaskCreate(
        task_pole_passturn_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        NULL);

    // Start the scheduler
    vTaskStartScheduler();
    for (;;)
    {
    }
}