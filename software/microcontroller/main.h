/*
 * main.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

// Board stuff
#define BOARD_WIDTH                 7
#define BOARD_HEIGHT                6
#define BOARD_SIZE                  (BOARD_WIDTH * BOARD_HEIGHT)
#define EMPTY_PIECE                 0
#define BLUE_PIECE                  1
#define YELLOW_PIECE                2
#define BOARD_NORMAL                1
#define BOARD_WIN_P1                2
#define BOARD_WIN_P2                3

// Bits
#define BIT_0                       (1 << 0)
#define BIT_1                       (1 << 1)
#define BIT_2                       (1 << 2)
#define BIT_3                       (1 << 3)
#define BIT_4                       (1 << 4)
#define BIT_5                       (1 << 5)
#define BIT_6                       (1 << 6)
#define BIT_7                       (1 << 7)

// RGB LED states
#define RGB_OFF                     0
#define RGB_GREEN                   1
#define RGB_YELLOW                  2
#define RGB_RED                     3
#define RGB_BLUE                    5

// RGBs
#define LED_GAME_STATE              0
#define LED_WINNER                  1   

// SM states
#define STATE_PAUSE                 0
#define STATE_INIT                  1
#define STATE_P1_TURN               2
#define STATE_P2_TURN               3
#define STATE_P1_WIN                4
#define STATE_P2_WIN                5

// PB states
#define PB_PRESSED                  false
#define PB_NOT_PRESSED              true

// FreeRTOS Event Group masks
#define ALL_EVENTS_MASK                     0xFFFFFFFF
#define EVENT_PASS_TURN_MASK                BIT_0
#define EVENT_START_GAME_MASK               BIT_1
#define EVENT_BLE_CONNECTED                 BIT_2

// Pins
#define PIN_PASS_TURN_PB            P10_4
#define PIN_START_PB                P10_3
#define PIN_PLAYER1_LED             P10_1
#define PIN_PLAYER2_LED             P9_4
#define PIN_RPI_TX                  P5_1
#define PIN_RPI_RX                  P5_0
#define PIN_GAME_STATE_LED_R        P5_3
#define PIN_GAME_STATE_LED_G        P5_2
#define PIN_GAME_STATE_LED_B        P5_5
#define PIN_WINNER_LED_R            P12_6
#define PIN_WINNER_LED_G            P12_7
#define PIN_WINNER_LED_B            P10_2
#define PIN_I2C_SCL                 P9_0
#define PIN_I2C_SDA                 P9_1
#define PIN_SERVO                   P6_3
#define PIN_LIN_BACK                P7_7    // Connect to brown lead of linear actuator
#define PIN_LIN_FORE                P5_6    // Connect to blue lead of linear actuator

// Sounds
#define SOUND_BLE_CONNECTED         0
#define SOUND_ERROR                 1
#define SOUND_BEGIN                 2

// Configurations
#define BLE_TASK_STACK_SIZE         (configMINIMAL_STACK_SIZE * 32)
#define SM_TASK_STACK_SIZE          (configMINIMAL_STACK_SIZE * 32)
#define BLE_CMD_Q_LEN               (10u)
#define LIGHT_THRESHOLD             300

// Linear Actuator
#define FORWARD                     0       
#define BACKWARD                    1  
#define STOP                        2

//extern void rgb_on(cyhal_pwm_t *rgb_obj_r, cyhal_pwm_t *rgb_obj_g, cyhal_pwm_t *rgb_obj_b, int color);

#endif /* MAIN_H_ */