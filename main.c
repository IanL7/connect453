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
cyhal_pwm_t lin_fore_pwm_obj;
cyhal_pwm_t lin_back_pwm_obj;
cyhal_pwm_t servo_pwm_obj;
cyhal_pwm_t game_state_led_b;
cyhal_pwm_t game_state_led_r;
cyhal_pwm_t game_state_led_g;
cyhal_pwm_t winner_led_b;
cyhal_pwm_t winner_led_r;
cyhal_pwm_t winner_led_g;

/*******************************************************************************
* Global Variables (add static?)
******************************************************************************/
EventGroupHandle_t xSMEventGroup;
QueueHandle_t xDistanceQueue;
QueueHandle_t xBoardQueue;

// Message to request board state
uint8_t rpi_i2c_request[3] = {1, 2, 3};

// Board state responses
// prev initialized to all empties for start of game
uint8_t rpi_i2c_response_prev[BOARD_SIZE] = {EMPTY_PIECE};
uint8_t rpi_i2c_response_curr[BOARD_SIZE];

/*******************************************************************************
* Task Handles
******************************************************************************/
TaskHandle_t state_manager_handle;
TaskHandle_t pole_pause_pb_handle;
TaskHandle_t pole_passturn_pb_handle;
TaskHandle_t clear_dropper_handle;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

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
    
    case RGB_BLUE:
        cyhal_pwm_stop(rgb_obj_r);
        cyhal_pwm_start(rgb_obj_b);
        cyhal_pwm_stop(rgb_obj_g);
        break;

    default:
        cyhal_pwm_stop(rgb_obj_r);
        cyhal_pwm_stop(rgb_obj_b);
        cyhal_pwm_stop(rgb_obj_g);
        break;
   }
}

void mcu_all_leds_init()
{
    // TODO: Add serial indication if an error occurred for a specific LED
    cy_rslt_t rslt;

    // LED1 (ON - OFF)
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

    // LED 2 (Player 2 Turn)
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

    // LED 3 (Player 1 Turn)
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

    // PB 1 - Pass turn
    rslt = cyhal_gpio_init(
        PIN_PASS_TURN_PB,
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

    // PB 2 - Pause / Resume
    rslt = cyhal_gpio_init(
        PIN_PAUSE_PB,
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

void pwm_init(int lin_hz, int lin_duty, int servo_hz, int servo_duty, int rgb_hz, int rgb_duty)
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
    rslt = cyhal_pwm_init(&servo_pwm_obj, P6_3, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, servo_duty, servo_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&servo_pwm_obj);

    /////////////////////////////////////////////////////////////////
    // RGB1 (game state led)
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_b, PIN_GAME_STATE_LED_B, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_b, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_b);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_r, PIN_GAME_STATE_LED_R, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_r, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_r);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_g, PIN_GAME_STATE_LED_G, NULL);
    /* Set duty cycle of 50% */
    rslt = cyhal_pwm_set_duty_cycle(&game_state_led_g, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_g);

    /////////////////////////////////////////////////////////////////
    // RGB2 (winner led)
    /////////////////////////////////////////////////////////////////
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_b, PIN_WINNER_LED_B, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&winner_led_b, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_b);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_r, PIN_WINNER_LED_R, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&winner_led_r, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_r);

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&game_state_led_g, PIN_WINNER_LED_G, NULL);
    /* Set duty cycle */
    rslt = cyhal_pwm_set_duty_cycle(&winner_led_g, rgb_duty, rgb_hz);
    /* Stop the PWM output */
    rslt = cyhal_pwm_stop(&game_state_led_g);
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

void i2c_init()
{
    /* Declare variables */
    cy_rslt_t rslt;

    /* Define the I2C master configuration structure */
    cyhal_i2c_cfg_t i2c_master_config = {CYHAL_I2C_MODE_MASTER, 0 /* address is not used for master mode */, I2C_MASTER_FREQUENCY};

    /* Initialize I2C master, set the SDA and SCL pins and assign a new clock */
    rslt = cyhal_i2c_init(&i2c_master_obj, PIN_I2C_SDA, PIN_I2C_SCL, NULL);

    /* Configure the I2C resource to be master */
    rslt = cyhal_i2c_configure(&i2c_master_obj, &i2c_master_config);
}

/* Defining master callback handler */
void master_event_handler(void *callback_arg, cyhal_i2c_event_t event)
{
    BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* To remove unused variable warning */
    (void) callback_arg;
    /* Check write complete event */
    if (0UL != (CYHAL_I2C_SLAVE_WR_CMPLT_EVENT & event))
    {
        /* Perform the required functions */
    }
    /* Check read complete event */
    if (0UL != (CYHAL_I2C_SLAVE_RD_CMPLT_EVENT & event))
    {
        xResult = xEventGroupSetBitsFromISR(
                              xSMEventGroup,   /* The event group being updated. */
                              EVENT_BOARD_RECEIVED_MASK, /* The bits being set. */
                              &xHigherPriorityTaskWoken );

        /* Was the message posted successfully? */
        if( xResult != pdFAIL )
        {
            /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
            switch should be requested.  The macro used is port specific and will
            be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
            the documentation page for the port being used. */
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    }
    if (0UL != (CYHAL_I2C_SLAVE_ERR_EVENT & event))
    {
        /* In case of error abort transfer */
        cyhal_i2c_abort_async(&i2c_master_obj);
    }
}

void i2c_get_board_state()
{
    cy_rslt_t rslt;

    // TODO: Put correct RPi slave address here
    uint16_t I2C_SLAVE_ADDRESS = 0x08u;

    cyhal_i2c_register_callback(&i2c_master_obj, (cyhal_i2c_event_callback_t) master_event_handler, NULL);
    cyhal_i2c_enable_event(&i2c_master_obj, (cyhal_i2c_event_t)   \
                                 (CYHAL_I2C_SLAVE_WR_CMPLT_EVENT  \
                                | CYHAL_I2C_SLAVE_RD_CMPLT_EVENT  \
                                | CYHAL_I2C_SLAVE_ERR_EVENT),    
                                3u , true);

    /* Initiate master write and read transfer using tx buffer and rx buffer respectively. */
    cyhal_i2c_master_transfer_async(&i2c_master_obj, I2C_SLAVE_ADDRESS, rpi_i2c_request, 3, rpi_i2c_response_curr, BOARD_SIZE);
}

// Figure out state of board
// Did someone win? Is there an error? etc.
// Priority - error > winner > valid
uint8_t process_board()
{
    // Check if error
    int diff_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // checking if p1 placed too many pieces
        if ( rpi_i2c_response_prev[i] == EMPTY_PIECE && rpi_i2c_response_curr[i] == BLUE_PIECE ||
             rpi_i2c_response_prev[i] == EMPTY_PIECE && rpi_i2c_response_curr[i] == YELLOW_PIECE )
        {
            diff_count++;
        }
        // p1 took out a piece
        if ( rpi_i2c_response_prev[i] == BLUE_PIECE && rpi_i2c_response_curr[i] == EMPTY_PIECE ||
             rpi_i2c_response_prev[i] == YELLOW_PIECE && rpi_i2c_response_curr[i] == EMPTY_PIECE )
        {
            return BOARD_ERROR;
        }
    }
    if (diff_count > 1) 
    {
        return BOARD_ERROR;
    }
    // TODO: implement winner checks
    return BOARD_VALID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeRTOS Tasks
///////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Add serial outputs at key spots (ex. at different states)
void task_state_manager(void *param)
{
    static int STATE;

    cy_rslt_t rslt;

    /* Suppress warning for unused parameter */
    (void)param;

    STATE = STATE_INIT;
    uint32_t event;

    for (;;) 
    {
        switch (STATE)
        {
            case STATE_INIT:
                vTaskResume(clear_dropper_handle);

                // Set LEDs
                rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_YELLOW);
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);

                xEventGroupWaitBits(
                    xSMEventGroup, 
                    EVENT_PAUSE_MASK | EVENT_DROPPER_CLEAR_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);
                
                // Set Game state LED to green
                rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);

                STATE = STATE_P1_TURN;
                break;
            
            case STATE_P1_TURN:
                vTaskSuspend(clear_dropper_handle);

                // Set LEDs
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, true);

                // Send I2C signal to RPi to send over game state?

                // Include mask here for error handling of player 1 move (doesn't put exactly one piece in)
                // Should I be clearing all bits here?
                for (;;) 
                {
                    // First wait for pass turn PB
                    xEventGroupWaitBits(
                        xSMEventGroup, 
                        EVENT_PASS_TURN_MASK,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

                    // Send request to RPi for board state info via I2C
                    i2c_get_board_state();

                    // Wait for board state from RPi
                    // State A - Player 1 
                    xEventGroupWaitBits(
                        xSMEventGroup, 
                        EVENT_BOARD_RECEIVED_MASK,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);
                    
                    uint8_t board_state = process_board();

                    if (board_state == BOARD_ERROR) 
                    {
                        rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_RED);
                        // play error sound?
                        continue;   // If there's an error, stay in P1 turn until they fix it
                    }
                    else if (board_state == BOARD_WIN_P1)
                    {
                        rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_OFF);
                        STATE = STATE_PLAYER1_WIN;
                        break;
                    }
                    else if (board_state == BOARD_WIN_P2)
                    {
                        rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_OFF);
                        STATE = STATE_PLAYER2_WIN;
                        break;
                    } 
                    // Board is valid
                    // TODO: Send current board state over BLE to computer
                    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_GREEN);
                    STATE = STATE_P2_TURN;
                    break;
                }
                break;

            case STATE_P2_TURN:
                // Set LEDs
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, true);

                // TODO: Wait for P2 move from computer over BLE
                xEventGroupWaitBits(
                        xSMEventGroup, 
                        EVENT_P2_MOVE_RECEIVED_MASK,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

            case STATE_PLAYER1_WIN:
                vTaskSuspend(clear_dropper_handle);
                // Set LEDs
                // NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
                rgb_on(&winner_led_r, &winner_led_g, &winner_led_b, RGB_YELLOW);
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);

            case STATE_PLAYER2_WIN:
                vTaskSuspend(clear_dropper_handle);
                // Set LEDs
                // NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
                rgb_on(&winner_led_r, &winner_led_g, &winner_led_b, RGB_BLUE);
                cyhal_gpio_write(PIN_PLAYER2_LED, false);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);


        }
    }
}

void task_pole_pause_pb(void *param)
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
        curr_pb_state = cyhal_gpio_read(PIN_PAUSE_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            xEventGroupSetBits(xSMEventGroup, EVENT_PAUSE_MASK);
        }
        prev_pb_state = curr_pb_state;
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
            xEventGroupSetBits(xSMEventGroup, EVENT_PASS_TURN_MASK);
        }
        prev_pb_state = curr_pb_state;
    }
}

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

void task_clear_dropper(void *param)
{
    cy_rslt_t rslt;

    uint8_t curr_dist_val;

    /* Suppress warning for unused parameter */
    (void)param;

    unsigned long dist_buffer;

    TickType_t xLastWakeTime;

    // Distance check freq is 50ms
    const TickType_t xFrequency = 50/portTICK_PERIOD_MS;

    rgb_on(&game_state_led_r, &game_state_led_g, &game_state_led_b, RGB_YELLOW);

    curr_dist_val = xQueueReceive(xDistanceQueue, &dist_buffer, portMAX_DELAY); // is max delay appropriate here?
    if (curr_dist_val > 10)
    {
        rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);
        rslt = cyhal_pwm_start(&lin_back_pwm_obj);
    }

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    /* Repeatedly running part of the task */
    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency);

        curr_dist_val = xQueueReceive(xDistanceQueue, &dist_buffer, portMAX_DELAY);

        if (curr_dist_val <= 10)
        {
            rslt = cyhal_pwm_stop(&lin_fore_pwm_obj);
            rslt = cyhal_pwm_stop(&lin_back_pwm_obj);
            
            xEventGroupSetBits(xSMEventGroup, EVENT_DROPPER_CLEAR_MASK);
        }
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
    mcu_all_leds_init();
    mcu_all_pbs_init();
    i2c_init();

    __enable_irq();
    
    // Turn on ON-OFF LED
    cyhal_gpio_write(PIN_ONOFF_LED, true);

    // Startup sound
    mcu_startup_sound();

    // Create inter-task messaging handles
    xDistanceQueue = xQueueCreate(1, sizeof( unsigned long )); // TODO: change type as necessary
    xBoardQueue = xQueueCreate(1, sizeof( unsigned long )); // TODO: change type as necessary
    xSMEventGroup = xEventGroupCreate();

    // Create tasks
    // TODO: Update priorites
    xTaskCreate(
        task_state_manager,
        "State Manager",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &state_manager_handle);
    
    xTaskCreate(
        task_pole_pause_pb,
        "Pole Pause Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &pole_pause_pb_handle);
    
    xTaskCreate(
        task_pole_passturn_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &pole_passturn_pb_handle);

    xTaskCreate(
        task_clear_dropper,
        "Clear Dropper",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &clear_dropper_handle);

    // DISTANCE SENSOR TASK SHOULD USE xQueueOverwrite TO SEND DATA!
    
    // Start the scheduler
    vTaskStartScheduler();

    for (;;)
    {
    }
}