#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "cybsp.h"
#include "cyhal.h"
#include "main.h"
#include "audio.h"
#include "cybsp_types.h"
#include "ble_task.h"
#include "light_sensor_task.h"

/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

// NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
// NOTE: PLAYER 2 ASSUMED TO BE BLUE!
TaskHandle_t xSMHandle = NULL;
TaskHandle_t xPPBHandle = NULL;

EventGroupHandle_t xConnectFourEventGroup;

uint8_t board_state_curr[BOARD_SIZE];
bool brd_rdy;

TimerHandle_t timer_handle;
TaskHandle_t ble_task_handle;
QueueHandle_t ble_cmdQ;
QueueHandle_t xLightQueue;
QueueHandle_t xPieceQueue;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

void play_sound(int sound)
{
    uint32_t sample;
    cy_rslt_t rslt;
    cyhal_pwm_t pwm_obj;
    cyhal_dac_t my_dac_obj;
    cy_rslt_t dac_result;

    switch (sound)
    {
        case SOUND_STARTUP:
            /* Initialize PWM on the supplied pin and assign a new clock */
            rslt = cyhal_pwm_init(&pwm_obj, P9_6, NULL);
            rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 440);
            /* Start the PWM output */
            rslt = cyhal_pwm_start(&pwm_obj);
            cyhal_system_delay_ms(500);
            rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 523);
            cyhal_system_delay_ms(500);
            rslt = cyhal_pwm_stop(&pwm_obj);
            cyhal_pwm_free(&pwm_obj);
            break;

        case SOUND_BEGIN:
            /* Initialize PWM on the supplied pin and assign a new clock */
            rslt = cyhal_pwm_init(&pwm_obj, P9_6, NULL);
            rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 523);
            /* Start the PWM output */
            rslt = cyhal_pwm_start(&pwm_obj);
            cyhal_system_delay_ms(500);
            rslt = cyhal_pwm_stop(&pwm_obj);
            cyhal_pwm_free(&pwm_obj);
            break;

        case SOUND_ERROR:
            /* Initialize DAC, set Pin 9.6 as the DAC output */
            dac_result = cyhal_dac_init(&my_dac_obj, P9_6);

            /* Check the return of cyhal_dac_init to confirm initialization was successful */
            if (dac_result != CY_RSLT_SUCCESS)
            {
                printf("ERROR: DAC failed to initialize\n\r");
            }
            // Play the error sound
            for (sample = 0; sample < 16344; sample++)
            {
                /* Write the 16 bit value as DAC input */
                cyhal_dac_write(&my_dac_obj, 0xFF * error_sound[sample]);

                // Delay for ~1/8000ths of a second, (sample rate of audio array is 8khz)
                cyhal_system_delay_us(125);
            }
            cyhal_dac_free(&my_dac_obj);
            break;
    }
}

void leds_init()
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
            printf("ERROR: ON/OFF LED failed to initialize\n\r");
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
            printf("ERROR: LED P2 failed to initialize\n\r");
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
            printf("ERROR: LED P1 failed to initialize\n\r");
        };
    }

    // RGB LED 1
    rslt = cyhal_gpio_init(
        PIN_GAME_STATE_LED_R,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 1 - R failed to initialize\n\r");
        };
    }
    rslt = cyhal_gpio_init(
        PIN_GAME_STATE_LED_G,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 1 - G failed to initialize\n\r");
        };
    }
    rslt = cyhal_gpio_init(
        PIN_GAME_STATE_LED_B,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 1 - B failed to initialize\n\r");
        };
    }

    // RGB LED 2
    rslt = cyhal_gpio_init(
        PIN_WINNER_LED_R,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 2 - R failed to initialize\n\r");
        };
    }
    rslt = cyhal_gpio_init(
        PIN_WINNER_LED_G,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 2 - G failed to initialize\n\r");
        };
    }
    rslt = cyhal_gpio_init(
        PIN_WINNER_LED_B,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        false);

    if (rslt != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        while (1)
        {
            printf("ERROR: RGB LED 2 - B failed to initialize\n\r");
        };
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

    uint8_t light_value;

    BaseType_t rtos_api_result;

    /* Suppress warning for unused parameter */
    (void)param;

    STATE = STATE_INIT;

    for (;;) 
    {
        switch (STATE)
        {     
            case STATE_INIT:
                printf("* --- Currently in INIT state                   --- *\n\r");

                cyhal_gpio_write(PIN_GAME_STATE_LED_R, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, true);

                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);
                
                STATE = STATE_P1_TURN;
                break;

            case STATE_P1_TURN:
                printf("* --- Currently in Player 1's Turn state        --- *\n\r");

                // In case pass turn pb was pressed while in P2 turn
                xEventGroupClearBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
                
                // Set LEDs
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, true);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, false);

                // Should I be clearing all bits here?
                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);

                /*
                rtos_api_result = xQueueReceive(xLightQueue, &light_value, portMAX_DELAY);
                if (rtos_api_result == pdFALSE)
                {
                    printf("ERROR: light value not received\n\r");
                    CY_ASSERT(0);
                }

                if (light_value > LIGHT_THRESHOLD)
                {
                    play_sound(SOUND_ERROR);
                    printf("Dropper is jammed! Player 1 intervention required.\n\r");
                    continue; // stay in p1 turn state
                } 
                */

                STATE = STATE_P2_TURN;
                break;

            case STATE_P2_TURN:
                printf("* --- Currently in Player 2's Turn state        --- *\n\r");
                uint8_t p2_move;
                // Set LEDs
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, true);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, false);

                // Wait for P2's move over BLE
                xQueueReceive(xPieceQueue, &p2_move, portMAX_DELAY);
                // Cleanup move 
                p2_move = p2_move & 0x07;
                printf("move received in sm: %x \n\r", p2_move);
                // Move dropper unit here

                // Replace this with reading from pi
                board_state_curr[p2_move] = YELLOW_PIECE;

                /* Send response to GATT Client device */
                ble_write_response();
                    
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

    printf("* --- Initializing LEDs                                     --- *\n\r");
    leds_init();

    printf("* --- Initializing Push Button                              --- *\n\r");
    rslt = cyhal_gpio_init(
        PIN_PASS_TURN_PB,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false);

    printf("* --- Turning on Power LED                                  --- *\n\r");
    cyhal_gpio_write(PIN_ONOFF_LED, true);

    printf("* --- Playing startup sound                                 --- *\n\r");
    play_sound(SOUND_STARTUP);
    play_sound(SOUND_ERROR);
    play_sound(SOUND_BEGIN);
    
    cyhal_pwm_t pwm_obj;
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&pwm_obj, P9_6, NULL);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 440);
    /* Start the PWM output */
    rslt = cyhal_pwm_start(&pwm_obj);
    cyhal_system_delay_ms(500);
    rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 523);
    cyhal_system_delay_ms(500);
    rslt = cyhal_pwm_stop(&pwm_obj);

    printf("* --- Done with startup sound                                 --- *\n\r");

    xConnectFourEventGroup = xEventGroupCreate();

    if (xConnectFourEventGroup == NULL)
    {
        printf("XXX ERROR: Event group not created\n\r");
    }

    for (uint8_t i = 0; i < BOARD_SIZE; i++)
    {
        board_state_curr[i] = 0;
    }

    // FreeRTOS things
    ble_cmdQ = xQueueCreate(BLE_CMD_Q_LEN, sizeof(uint8_t));

    xLightQueue = xQueueCreate(1, sizeof(uint8_t));
    xPieceQueue = xQueueCreate(1, sizeof(uint8_t));

    timer_handle = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, rtos_timer_cb);

    
    xTaskCreate(
        task_pole_passturn_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &xPPBHandle);
    

    
    xTaskCreate(
        task_state_manager,
        "State Manager",
        SM_TASK_STACK_SIZE,
        NULL,
        3,
        &xSMHandle);
    

    xTaskCreate(
        task_BLE,
        "BLE Process",
        BLE_TASK_STACK_SIZE,
        NULL,
        1,
        &ble_task_handle);

    printf("* --- Starting task scheduler                               --- *\n\r");

    vTaskStartScheduler();

    for (;;)
    {
    }
}