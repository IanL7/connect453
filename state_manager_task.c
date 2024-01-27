/*
 *  task_state_manager.c
 *
 *  Created on: Dec 9, 2023
 *      Author: Ian Lodes
 */

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "state_manager_task.h"
#include "console.h"
#include "connect453_lib.h"

/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>

EventGroupHandle_t xConnectFourEventGroup;

static cyhal_pwm_t servo_pwm_obj;
static cyhal_pwm_t pwm_obj;

void task_state_manager(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    static int STATE;
    char board_from_pi[500];
    cy_rslt_t rslt;
    uint16_t light_value;
    BaseType_t rtos_api_result;
    uint8_t p2_move;
    EventBits_t xEventBits;

    uint8_t board_state_curr[BOARD_SIZE + 1];
    bool board_clear = true;

    // Game has not started - BLE
    for (int i = 0; i < BOARD_SIZE+1; i++)
    {
        board_state_curr[i] = 0;
    }
    board_state_curr[42] = 2;
    xQueueOverwrite(xBoardBLEQueue, board_state_curr);

    STATE = STATE_INIT;

    bool p1_first = true;

    bool send_p1_state = true;

    bool restart = true;

    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&pwm_obj, P9_6, NULL);

    // Wait until BLE is connected
    for (;;)
    {
        // flash game state led blue
        set_rgb(LED_GAME_STATE, RGB_BLUE);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        set_rgb(LED_GAME_STATE, RGB_OFF); 
        vTaskDelay(1000/portTICK_PERIOD_MS); 

        xEventBits = xEventGroupGetBits(xConnectFourEventGroup);
        if ( (xEventBits & EVENT_BLE_CONNECTED) )
        {
            xEventGroupClearBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
            break;
        }
    }

    play_sound(SOUND_BLE_CONNECTED);

    // In case pass turn pb or game start pb was pressed while waiting for ble
    xEventGroupClearBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);
    xEventGroupClearBits(xConnectFourEventGroup, EVENT_START_GAME_MASK);

    for (;;) 
    {
        switch (STATE)
        {     
            case STATE_INIT:
                printf("* --- Currently in INIT state                   --- *\n\r");
                set_rgb(LED_GAME_STATE, RGB_GREEN);
                cyhal_gpio_write(PIN_PLAYER1_LED, false);
                cyhal_gpio_write(PIN_PLAYER2_LED, false);

                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_START_GAME_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);

                play_sound(SOUND_BEGIN);

                if (p1_first)
                {
                    STATE = STATE_P1_TURN;
                }
                else 
                {
                    STATE = STATE_P2_TURN;
                }
                break;

            case STATE_P1_TURN:
                printf("* --- Currently in Player 1's Turn state        --- *\n\r");
                cyhal_gpio_write(PIN_PLAYER1_LED, true);
                cyhal_gpio_write(PIN_PLAYER2_LED, false);

                // P1 turn - BLE
                if (send_p1_state)
                {
                    board_state_curr[42] = 0;
                    xQueueOverwrite(xBoardBLEQueue, board_state_curr);
                }

                // In case pass turn pb was pressed while in P2 turn
                xEventGroupClearBits(xConnectFourEventGroup, EVENT_PASS_TURN_MASK);

                // wait for push button
                xEventGroupWaitBits(
                    xConnectFourEventGroup, 
                    EVENT_PASS_TURN_MASK,
                    pdTRUE,
                    pdTRUE,
                    portMAX_DELAY);
                
                // Player 1 should have put a piece in at this point
                light_value = read_light_sensor();
                printf("Light value received: %d", light_value);

                
                if (light_value > LIGHT_THRESHOLD)
                {
                    printf("Dropper is jammed! Player 1 intervention required.\n\r");

                    // Flash LED red and play error sound
                    rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 400);
                    rslt = cyhal_pwm_start(&pwm_obj);
                    vTaskDelay(500/portTICK_PERIOD_MS);
                    rslt = cyhal_pwm_stop(&pwm_obj);

                    send_p1_state = false;

                    STATE = STATE_P1_TURN;
                    break; // stay in p1 turn state
                } 
                else 
                {
                    send_p1_state = true;
                    printf("Dropper is loaded.\n\r");
                }

                // Wait for board state from rpi, and send values over ble as well as player 2 turn
                xQueueReceive(xBoardQueue, board_from_pi, portMAX_DELAY);
                printf("board received in sm: %s", board_from_pi);

                // Decode char
                for (int i = 0; i < 42; i++)
                {
                    if (board_from_pi[i] == '0')
                    {
                        board_state_curr[i] = 0;
                    }
                    else if (board_from_pi[i] == '1')
                    {
                        board_state_curr[i] = 1;
                    }
                    else if (board_from_pi[i] == '2')
                    {
                        board_state_curr[i] = 2;
                    }
                    else 
                    {
                        printf("ERROR: Received an invalid piece value from RPi: %c\n\r", board_from_pi[i]);
                    }
                }
                
                switch (process_board(board_state_curr))
                {
                    case BOARD_NORMAL:
                        board_state_curr[42] = 1;
                        STATE = STATE_P2_TURN;
                        break;
                    case BOARD_WIN_P1:
                        board_state_curr[42] = 5;
                        STATE = STATE_P1_WIN;
                        break;
                    case BOARD_WIN_P2:
                        board_state_curr[42] = 4;
                        STATE = STATE_P2_WIN;
                        break;
                    default:
                        board_state_curr[42] = 1;
                        STATE = STATE_P2_TURN;
                        break;
                }
                break;

            case STATE_P2_TURN:
                printf("* --- Currently in Player 2's Turn state        --- *\n\r");
                cyhal_gpio_write(PIN_PLAYER1_LED, false);
                cyhal_gpio_write(PIN_PLAYER2_LED, true);

                vTaskDelay(100/portTICK_PERIOD_MS);
                xQueueOverwrite(xBoardBLEQueue, board_state_curr);

                // Wait for P2's move over BLE
                xQueueReceive(xPieceQueue, &p2_move, portMAX_DELAY);

                // Cleanup move 
                p2_move = p2_move & 0x07;
                printf("move received in sm: %x \n\r", p2_move);

                // Control motors to deposit piece
                if (p2_move > 6 || p2_move < 0)
                {
                    printf("ERROR: Received an invalid column number from P2. Not depositing piece\n\r");
                }

                // --- Move forward ---
                int move_time = 1500;
                switch (p2_move)
                {
                    case 6:
                        break;
                    case 5:
                        move_time += 650;
                        break;
                    case 4:
                        move_time += 1300;
                        break;
                    case 3:
                        move_time += 1950;
                        break;
                    case 2:
                        move_time += 2600;
                        break;
                    case 1:
                        move_time += 3250;
                        break;
                    case 0:
                        move_time += 3900;
                        break;
                }
                control_lin(FORWARD);
                vTaskDelay(move_time/portTICK_PERIOD_MS);
                control_lin(STOP);
                vTaskDelay(250/portTICK_PERIOD_MS);
                // --- Deposit piece ---

                // Reassign servo pin to PWM 
                // PWM pin on servo seems to need to be grounded while 
                // linear actuator is moving if servo running directly off uController
                cyhal_gpio_free(PIN_SERVO);
                servo_pwm_init();

                printf("Moving to deposit\r\n");
                rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, 6.25, 50);
                rslt = cyhal_pwm_start(&servo_pwm_obj);
                vTaskDelay(2000/portTICK_PERIOD_MS);

                printf("Moving to retrieve\r\n");
                rslt = cyhal_pwm_set_duty_cycle(&servo_pwm_obj, 12.5, 50);
                vTaskDelay(2000/portTICK_PERIOD_MS);

                rslt = cyhal_pwm_stop(&servo_pwm_obj);

                cyhal_pwm_free(&servo_pwm_obj);

                // Reassign servo pin to GPIO
                servo_gpio_init();

                // --- Backup ---

                control_lin(BACKWARD);
                vTaskDelay(move_time/portTICK_PERIOD_MS); // Wait a bit to prevent fast direction changes

                // Wait for board state from rpi, and send values over ble as well as player 1 turn
                xQueueReceive(xBoardQueue, board_from_pi, portMAX_DELAY);
                printf("board received in sm: %s", board_from_pi);

                // Decode char
                for (int i = 0; i < 42; i++)
                {
                    if (board_from_pi[i] == '0')
                    {
                        board_state_curr[i] = 0;
                    }
                    else if (board_from_pi[i] == '1')
                    {
                        board_state_curr[i] = 1;
                    }
                    else if (board_from_pi[i] == '2')
                    {
                        board_state_curr[i] = 2;
                    }
                    else 
                    {
                        printf("ERROR: Received an invalid piece value from RPi: %c\n\r", board_from_pi[i]);
                    }
                }

                switch (process_board(board_state_curr))
                {
                    case BOARD_NORMAL:
                        board_state_curr[42] = 0;
                        STATE = STATE_P1_TURN;
                        break;
                    case BOARD_WIN_P1:
                        board_state_curr[42] = 5;
                        STATE = STATE_P1_WIN;
                        break;
                    case BOARD_WIN_P2:
                        board_state_curr[42] = 4;
                        STATE = STATE_P2_WIN;
                        break;
                    default:
                        board_state_curr[42] = 0;
                        STATE = STATE_P1_TURN;
                        break;
                }
                xQueueOverwrite(xBoardBLEQueue, board_state_curr);

                /* Send response to GATT Client device */
                ble_write_response();
                break;

            case STATE_P1_WIN:
                set_rgb(LED_GAME_STATE, RGB_BLUE);
                board_state_curr[42] = 5;

                xQueueOverwrite(xBoardBLEQueue, board_state_curr);

                for (;;)
                {
                    // wait for push button
                    xEventGroupWaitBits(
                        xConnectFourEventGroup, 
                        EVENT_START_GAME_MASK,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

                    // Make sure board is clear before restarting
                    board_clear = true;

                    xQueueReceive(xBoardQueue, board_from_pi, portMAX_DELAY);
                    printf("board received in sm: %s", board_from_pi);

                    // Decode char
                    for (int i = 0; i < 42; i++)
                    {
                        if (board_from_pi[i] == '0')
                        {
                            board_state_curr[i] = 0;
                        }
                        else if (board_from_pi[i] == '1')
                        {
                            board_state_curr[i] = 1;
                            board_clear = false;
                        }
                        else if (board_from_pi[i] == '2')
                        {
                            board_state_curr[i] = 2;
                            board_clear = false;
                        }
                        else 
                        {
                            printf("ERROR: Received an invalid piece value from RPi: %c\n\r", board_from_pi[i]);
                        }
                    }
                    xQueueOverwrite(xBoardBLEQueue, board_state_curr);

                    if (board_clear)
                    {
                        restart = true;
                        break;
                    }
                    else
                    {
                        printf("Clear board before restarting!\n\r");

                        // Flash LED red and play error sound
                        rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 400);
                        rslt = cyhal_pwm_start(&pwm_obj);
                        vTaskDelay(500/portTICK_PERIOD_MS);
                        rslt = cyhal_pwm_stop(&pwm_obj);
                        STATE = STATE_P1_WIN;
                        restart = false;
                        break;
                    }
                }

                if (restart)
                {
                    set_rgb(LED_GAME_STATE, RGB_GREEN);
                    p1_first = !p1_first;
                    if (p1_first)
                    {  
                        board_state_curr[42] = 0;
                        STATE = STATE_P1_TURN;
                    }
                    else 
                    {
                        board_state_curr[42] = 1;
                        STATE = STATE_P2_TURN;
                    }
                }
                break;

            case STATE_P2_WIN:
                set_rgb(LED_GAME_STATE, RGB_YELLOW);
                board_state_curr[42] = 4;

                xQueueOverwrite(xBoardBLEQueue, board_state_curr);
                
                for (;;)
                {
                    // wait for push button
                    xEventGroupWaitBits(
                        xConnectFourEventGroup, 
                        EVENT_START_GAME_MASK,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY);

                    // Make sure board is clear before restarting
                    board_clear = true;

                    xQueueReceive(xBoardQueue, board_from_pi, portMAX_DELAY);
                    printf("board received in sm: %s", board_from_pi);

                    // Decode char
                    for (int i = 0; i < 42; i++)
                    {
                        if (board_from_pi[i] == '0')
                        {
                            board_state_curr[i] = 0;
                        }
                        else if (board_from_pi[i] == '1')
                        {
                            board_state_curr[i] = 1;
                            board_clear = false;
                        }
                        else if (board_from_pi[i] == '2')
                        {
                            board_state_curr[i] = 2;
                            board_clear = false;
                        }
                        else 
                        {
                            printf("ERROR: Received an invalid piece value from RPi: %c\n\r", board_from_pi[i]);
                        }
                    }
                    xQueueOverwrite(xBoardBLEQueue, board_state_curr);

                    if (board_clear)
                    {
                        restart = true;
                        break;
                    }
                    else
                    {
                        printf("Clear board before restarting!\n\r");
                        restart = false;
                        // Flash LED red and play error sound
                        rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 400);
                        rslt = cyhal_pwm_start(&pwm_obj);
                        vTaskDelay(500/portTICK_PERIOD_MS);
                        rslt = cyhal_pwm_stop(&pwm_obj);
                        STATE = STATE_P2_WIN;
                        break;
                    }
                }

                if (restart)
                {
                    set_rgb(LED_GAME_STATE, RGB_GREEN);
                    p1_first = !p1_first;
                    if (p1_first)
                    {  
                        board_state_curr[42] = 0;
                        STATE = STATE_P1_TURN;
                    }
                    else 
                    {
                        board_state_curr[42] = 1;
                        STATE = STATE_P2_TURN;
                    }
                }
                break;
        }
    }
}
