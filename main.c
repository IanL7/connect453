#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "cybsp.h"
#include "cyhal.h"
#include "main.h"
#include "audio.h"
#include "cybsp_types.h"
#include "ble_task.h"
#include "light_sensor_task.h"
#include "rpi_task.h"
#include "connect453_lib.h"
#include "state_manager_task.h"

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

uint8_t board_state_curr[BOARD_SIZE + 1];

TimerHandle_t timer_handle;
TaskHandle_t ble_task_handle;

QueueHandle_t ble_cmdQ;
QueueHandle_t xLightQueue;
QueueHandle_t xPieceQueue;
QueueHandle_t xBoardQueue;

// NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
// NOTE: PLAYER 2 ASSUMED TO BE BLUE!

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

void task_pole_start_pb(void *param)
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
        curr_pb_state = cyhal_gpio_read(PIN_START_PB);

        if (curr_pb_state == PB_PRESSED && prev_pb_state == PB_NOT_PRESSED)
        {
            xEventGroupSetBits(xConnectFourEventGroup, EVENT_START_GAME_MASK);
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

    printf("* -- Enabling Interrupts\n\r");
	/* Enable global interrupts */
	__enable_irq();

    printf("* --- Initializing LEDs                                     --- *\n\r");
    leds_init();

    printf("* --- Initializing Push Buttons                              --- *\n\r");
    pbs_init();

    printf("* --- Initializing Light Sensor                             --- *\n\r");
    //light_sensor_init();

    printf("* --- Initializing Motor Control                            --- *\n\r");
    servo_gpio_init();
    lin_act_init();
    control_lin(STOP);

    printf("* --- Playing startup sound                                 --- *\n\r");
    play_sound(SOUND_STARTUP);

    xConnectFourEventGroup = xEventGroupCreate();

    if (xConnectFourEventGroup == NULL)
    {
        printf("XXX ERROR: Event group not created\n\r");
    }

    // FreeRTOS things
    ble_cmdQ = xQueueCreate(BLE_CMD_Q_LEN, sizeof(uint8_t));
    timer_handle = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, NULL, rtos_timer_cb);

    xLightQueue = xQueueCreate(1, sizeof(uint16_t));
    xPieceQueue = xQueueCreate(1, sizeof(uint8_t));
    xBoardQueue = xQueueCreate(1, sizeof(char[43]));
    
    xTaskCreate(
        task_pole_passturn_pb,
        "Pole Pass-Turn Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &xPPBHandle);

    xTaskCreate(
        task_pole_start_pb,
        "Pole Start-Game Push Button",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);
    
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
    
    xTaskCreate(
        task_rpi,
        "Raspberry Pi",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        NULL);
    /*
    xTaskCreate(
        task_light_sensor,
        "Light Sensor",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        NULL);
    */

    printf("* --- Starting task scheduler                               --- *\n\r");

    vTaskStartScheduler();

    for (;;)
    {
    }
}