#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "cybsp.h"
#include "cyhal.h"
#include "main.h"
#include "audio.h"
#include "cybsp_types.h"
#include "connect453_lib.h"

static cyhal_pwm_t servo_pwm_obj;

void set_rgb(int led, int state)
{
    switch (state)
    {
        case RGB_OFF:
            if (led == LED_GAME_STATE)
            {
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, false);
            }
            else 
            {
                cyhal_gpio_write(PIN_WINNER_LED_R, false);
                cyhal_gpio_write(PIN_WINNER_LED_B, false);
                cyhal_gpio_write(PIN_WINNER_LED_G, false);
            }
            break;

        case RGB_GREEN:
            if (led == LED_GAME_STATE)
            {
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, true);
            }
            else 
            {
                cyhal_gpio_write(PIN_WINNER_LED_R, false);
                cyhal_gpio_write(PIN_WINNER_LED_B, false);
                cyhal_gpio_write(PIN_WINNER_LED_G, true);
            }
            break;

        case RGB_YELLOW:
            if (led == LED_GAME_STATE)
            {
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, true);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, true);
            }
            else 
            {
                cyhal_gpio_write(PIN_WINNER_LED_R, true);
                cyhal_gpio_write(PIN_WINNER_LED_B, false);
                cyhal_gpio_write(PIN_WINNER_LED_G, true);
            }
            break;

        case RGB_RED:
            if (led == LED_GAME_STATE)
            {
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, true);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, false);
            }
            else 
            {
                cyhal_gpio_write(PIN_WINNER_LED_R, true);
                cyhal_gpio_write(PIN_WINNER_LED_B, false);
                cyhal_gpio_write(PIN_WINNER_LED_G, false);
            }
            break;

        case RGB_BLUE:
            if (led == LED_GAME_STATE)
            {
                cyhal_gpio_write(PIN_GAME_STATE_LED_R, false);
                cyhal_gpio_write(PIN_GAME_STATE_LED_B, true);
                cyhal_gpio_write(PIN_GAME_STATE_LED_G, false);
            }
            else 
            {
                cyhal_gpio_write(PIN_WINNER_LED_R, false);
                cyhal_gpio_write(PIN_WINNER_LED_B, true);
                cyhal_gpio_write(PIN_WINNER_LED_G, false);
            }
            break;
    }
}

uint8_t process_board()
{
    // Check for winner vert
    for (int i = 0; i < BOARD_SIZE - BOARD_WIDTH * 3; i++)
    {
        if (board_state_curr[i] == YELLOW_PIECE &&
            board_state_curr[i+7] == YELLOW_PIECE &&
            board_state_curr[i+14] == YELLOW_PIECE &&
            board_state_curr[i+21] == YELLOW_PIECE)
        {
            return BOARD_WIN_P2;
        }
        else if (board_state_curr[i] == BLUE_PIECE &&
                 board_state_curr[i+7] == BLUE_PIECE &&
                 board_state_curr[i+14] == BLUE_PIECE &&
                 board_state_curr[i+21] == BLUE_PIECE)
        {
            return BOARD_WIN_P1;
        }
    }

    // Check for winner horiz
    for (int r = 0; r < (BOARD_SIZE - BOARD_WIDTH) + 1; r += BOARD_WIDTH)
    {
        for (int c = r; c < 4; c++)
            if (board_state_curr[c] == YELLOW_PIECE &&
                board_state_curr[c+1] == YELLOW_PIECE &&
                board_state_curr[c+2] == YELLOW_PIECE &&
                board_state_curr[c+3] == YELLOW_PIECE)
            {
                return BOARD_WIN_P2;
            }
            else if (board_state_curr[c] == BLUE_PIECE &&
                    board_state_curr[c+1] == BLUE_PIECE &&
                    board_state_curr[c+2] == BLUE_PIECE &&
                    board_state_curr[c+3] == BLUE_PIECE)
            {
                return BOARD_WIN_P1;
            }
    }

    // Check for winner diag 1 
    // X
    //   X
    //     X
    //       X
    for (int r = 0; r < (BOARD_SIZE - BOARD_WIDTH - BOARD_WIDTH * 3) + 1; r += BOARD_WIDTH)
    {
        for (int c = r; c < 4; c++)
        {
            if (board_state_curr[c] == BLUE_PIECE &&
                board_state_curr[c+(BOARD_WIDTH+1)] == YELLOW_PIECE &&
                board_state_curr[c+2*(BOARD_WIDTH+1)] == YELLOW_PIECE &&
                board_state_curr[c+3*(BOARD_WIDTH+1)] == YELLOW_PIECE)
            {
                return BOARD_WIN_P2;
            }
            else if (board_state_curr[c] == BLUE_PIECE &&
                    board_state_curr[c+(BOARD_WIDTH+1)] == BLUE_PIECE &&
                    board_state_curr[c+2*(BOARD_WIDTH+1)] == BLUE_PIECE &&
                    board_state_curr[c+3*(BOARD_WIDTH+1)] == BLUE_PIECE)
            {
                return BOARD_WIN_P1;
            }
        }
    }

    // Check for winner diag 2
    //       X
    //     X
    //   X
    // X
    for (int r = BOARD_WIDTH - 4; r < BOARD_SIZE - BOARD_WIDTH * 3; r += BOARD_WIDTH)
    {
        for (int c = r; c < 4; c++)
        {
            if (board_state_curr[c] == BLUE_PIECE &&
                board_state_curr[c+(BOARD_WIDTH-1)] == YELLOW_PIECE &&
                board_state_curr[c+2*(BOARD_WIDTH-1)] == YELLOW_PIECE &&
                board_state_curr[c+3*(BOARD_WIDTH-1)] == YELLOW_PIECE)
            {
                return BOARD_WIN_P2;
            }
            else if (board_state_curr[c] == BLUE_PIECE &&
                    board_state_curr[c+(BOARD_WIDTH-1)] == BLUE_PIECE &&
                    board_state_curr[c+2*(BOARD_WIDTH-1)] == BLUE_PIECE &&
                    board_state_curr[c+3*(BOARD_WIDTH-1)] == BLUE_PIECE)
            {
                return BOARD_WIN_P1;
            }
        }
    }
    return BOARD_NORMAL;
}

// Inits P5_6 and P7_7 as GPIO and outputs 0 on both 
void lin_act_init()
{
    cy_rslt_t rslt;

    rslt = cyhal_gpio_init(PIN_LIN_FORE, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("ERROR: Failed to initialize Linear Actuator Fore\n\r");
    }
    rslt = cyhal_gpio_init(PIN_LIN_BACK, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("ERROR: Failed to initialize Linear Actuator Back\n\r");
    }
}


// Inits P6_3 as GPIO and outputs 0 to P6_3
void servo_gpio_init()
{
    cy_rslt_t rslt;

    rslt = cyhal_gpio_init(
        PIN_SERVO,
        CYHAL_GPIO_DIR_OUTPUT,
        CYHAL_GPIO_DRIVE_STRONG,
        0);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("ERROR: Failed to initialize Servo - GPIO\n\r");
    }
}

// Control the linear actuator
//      - delay: ms
void control_lin(int action)
{
    switch (action)
    {
        case FORWARD:
            cyhal_gpio_write(PIN_LIN_FORE, 1);
            cyhal_gpio_write(PIN_LIN_BACK, 0);
            break;
        case BACKWARD:
            cyhal_gpio_write(PIN_LIN_FORE, 0);
            cyhal_gpio_write(PIN_LIN_BACK, 1);
            break;
        case STOP:
            cyhal_gpio_write(PIN_LIN_FORE, 0);
            cyhal_gpio_write(PIN_LIN_BACK, 0);
            break;
    }
}

// Deposit a piece - move linear actuator and servo


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

void pbs_init()
{
    cy_rslt_t rslt;

    rslt = cyhal_gpio_init(
        PIN_PASS_TURN_PB,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("ERROR: Failed to initialize pass-turn PB - GPIO\n\r");
    }

    rslt = cyhal_gpio_init(
        PIN_START_PB,
        CYHAL_GPIO_DIR_INPUT,
        CYHAL_GPIO_DRIVE_NONE,
        false);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("ERROR: Failed to initialize start PB - GPIO\n\r");
    }
}

void leds_init()
{
    // TODO: Add serial indication if an error occurred for a specific LED
    cy_rslt_t rslt;

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