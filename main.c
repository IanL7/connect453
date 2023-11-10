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

// NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
// NOTE: PLAYER 2 ASSUMED TO BE BLUE!

EventGroupHandle_t xConnectFourEventGroup;
cyhal_pwm_t game_state_led_b;
cyhal_pwm_t game_state_led_r;
cyhal_pwm_t game_state_led_g;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

void mcu_play_sound(uint16_t array[], int array_len)
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
        for (sample = 0; sample < array_len; sample++)
        {
            /* Write the 16 bit value as DAC input */
            cyhal_dac_write(&my_dac_obj, array[sample]);
            /* Return the 16 bit DAC register value */
            dac_read = cyhal_dac_read(&my_dac_obj);
            if (dac_read == sample)
            {
                /* Insert code */
            }
            // TODO: is this enough? Do I need a timer instead?
            // Delay for ~1/24000ths of a second, (sample rate of audio array is 24khz)
            cyhal_system_delay_us(125);
        }
    }
    /* Release DAC object after use by calling cyhal_dac_free, this de-initializes the DAC (including its output) */
    cyhal_dac_free(&my_dac_obj);
}

void mcu_reg_leds_init()
{
    // TODO: Add serial indication if an error occurred for a specific LED
    cy_rslt_t rslt;

    // LED1 (ON - OFF)
    rslt = cyhal_gpio_init(
        PIN_ONOFF_LED,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("XXX ERROR: ON/OFF LED failed to initialize\n\r");
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
            printf("XXX ERROR: LED P2 failed to initialize\n\r");
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
            printf("XXX ERROR: LED P1 failed to initialize\n\r");
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeRTOS tasks
///////////////////////////////////////////////////////////////////////////////////////////////////////

// Basic implementation - just switch back and forth between player 1 and 2 move
void task_state_manager(void *param)
{
    static int STATE;

    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    STATE = STATE_INIT;

    for (;;) 
    {
        switch (STATE)
        {     
            case STATE_INIT:
                printf("* --- Currently in INIT state                   --- *\n\r");
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);

                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);
                
                mcu_play_sound(begin, 35278);
                rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);

            case STATE_P1_TURN:
                printf("* --- Currently in Player 1's Turn state        --- *\n\r");
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

                STATE = STATE_P2_TURN;
                break;

            case STATE_P2_TURN:
                printf("* --- Currently in Player 2's Turn state        --- *\n\r");
                // Set LEDs
                cyhal_gpio_write(PIN_PLAYER2_LED, true);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);

                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);
                    
                STATE = STATE_P1_TURN;
                break;
        }
    }
}

void task_pole_passturn_pb(void *param)
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
        curr_pb_state = cyhal_gpio_read(PIN_PASS_TURN_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            printf("    - PB pressed!\n\r");
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
        }
        prev_pb_state = curr_pb_state;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Entry point - init, power led, startup sound, start schedular
///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    cy_rslt_t rslt;

    /* Initialize the device and board peripherals */
    rslt = cybsp_init();
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    console_init();     // For printing to console

    printf("* ------------------------------------------------------------- *\n\r");
    printf("* --- Starting Connect453 Custom MCU Board Week 10 Demo     --- *\n\r");
    printf("* ------------------------------------------------------------- *\n\r");
    printf("\n\r");

    printf("* --- Initializing PWM                                      --- *\n\r");
    pwm_init(300, 50);

    printf("* --- Initializing LEDs                                     --- *\n\r");
    mcu_reg_leds_init();

    printf("* --- Initializing Push Button                              --- *\n\r");
    rslt = cyhal_gpio_init(
        PIN_PASS_TURN_PB,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false);

    printf("* --- Turning on Power LED                                  --- *\n\r");
    cyhal_gpio_write(PIN_ONOFF_LED, true);

    printf("* --- Playing startup sound                                 --- *\n\r");
    mcu_play_sound(startup, 34078);

    xConnectFourEventGroup = xEventGroupCreate();

    if (xConnectFourEventGroup == NULL)
    {
        printf("XXX ERROR: Event group not created\n\r");
    }

    xTaskCreate(
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

    printf("* --- Starting task scheduler                               --- *\n\r");
    vTaskStartScheduler();

    for (;;)
    {
    }
}