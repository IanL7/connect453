/*
 * rpi_task.c
 *
 *  Created on: Nov 30, 2023
 *      Authors: Billy Begos and Ian Lodes
 */

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "main.h"
#include "rpi_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "console.h"

char prev_board_state[500];

void task_rpi(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    init_prev_board_state();

    for(;;)
    {
        vTaskDelay(100);

        if(ALERT_CONSOLE_RX)
        {
            // send board state
            if (!equal(pcInputString, prev_board_state))
            {
                xQueueOverwrite(xBoardQueue, pcInputString);
            }
            
            // copy current board state to previous board state
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                prev_board_state[i] = pcInputString[i];
            } 

            //printf("Rx'd: %s\n", pcInputString);
            
            memset(pcInputString, 0, 500);
            cInputIndex = 0;
            ALERT_CONSOLE_RX = false;
        }
    }
}	

static bool equal(char board1[], char board2[])
{
    bool equal = true;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board1[i] != board2[i])
        {
            equal = false;
        }
    }
    return equal;
}

static void init_prev_board_state()
{
    for (int i = 0; i < 500; i++)
    {
        // put in garbage
        prev_board_state[i] = '4';
    }
}