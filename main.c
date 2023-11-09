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
cyhal_pwm_t game_state_led_b;
cyhal_pwm_t game_state_led_r;
cyhal_pwm_t game_state_led_g;

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
        false);

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
        false);

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

void rgb_on(cyhal_pwm_t *rgb_obj_r, cyhal_pwm_t *rgb_obj_g, cyhal_pwm_t *rgb_obj_b, int color)
{
   switch (color)
   {
        case RGB_GREEN:
            cyhal_pwm_stop(rgb_obj_r);
            cyhal_pwm_stop(rgb_obj_b);
            cyhal_pwm_start(rgb_obj_g);
            break;
        
        case RGB_RED:
            cyhal_pwm_start(rgb_obj_r);
            cyhal_pwm_stop(rgb_obj_b);
            cyhal_pwm_stop(rgb_obj_g);
            break;
        
        case RGB_YELLOW:
            cyhal_pwm_stop(rgb_obj_r);
            cyhal_pwm_start(rgb_obj_b);
            cyhal_pwm_start(rgb_obj_g);
            break;

        default:
            cyhal_pwm_stop(rgb_obj_r);
            cyhal_pwm_stop(rgb_obj_b);
            cyhal_pwm_stop(rgb_obj_g);
            break;
   }
}

// Basic implementation - just switch back and forth between player 1 and 2 move
void task_state_manager(void *param)
{
    static int STATE;

    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    STATE = STATE_P1_TURN;

    for (;;) 
    {
        printf("it1\n\r");
        switch (STATE)
        {     
            case STATE_P1_TURN:
                printf("*-- P1 Turn --*\n\r");
                // Set LEDs
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, true);

                // Should I be clearing all bits here?
                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);

                printf("*-- passed wait pb1 --*\n\r");

                STATE = STATE_P2_TURN;
                break;

            case STATE_P2_TURN:
                printf("*-- P2 Turn --*\n\r");
                // Set LEDs
                cyhal_gpio_write(PIN_PLAYER2_LED, true);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);

                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);

                printf("*-- passed wait pb2 --*\n\r");
                    
                STATE = STATE_P1_TURN;
                break;
        }
    }
}

void task_pole_passturn_pb(void *param)
{
    printf("Pole PB entered\n\r");
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
        printf("pb iter\n\r");
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency);


        // Get PB level
        curr_pb_state = cyhal_gpio_read(PIN_PASS_TURN_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            printf("PB pressed\n\r");
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
        }
        prev_pb_state = curr_pb_state;
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
    printf("started\n\r");

    // RGB: 300hz, 50% duty cycle
    pwm_init(300, 50);

    mcu_reg_leds_init();

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);

    // Init PB
    rslt = cyhal_gpio_init(
        PIN_PASS_TURN_PB,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false);

    BaseType_t pole_result = xTaskCreate(
        task_pole_passturn_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);

    xTaskCreate(
        task_state_manager,
        "State Manager",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);

    if (pole_result == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
    {
        printf("error creating poling task\n\r");
    }
    else 
    {
        printf("successfully created poling task\n\r");
    }

    // Start the scheduler
    vTaskStartScheduler();
    for (;;)
    {
    }
}