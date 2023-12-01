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


void task_rpi(void *pvParameters)
{
    int i = 0;
    led_mask = 0x01;
    
    while(1)
    {
        Cy_SysLib_Delay(100);
        io_expander_set_output_port(led_mask);

        if(ALERT_CONSOLE_RX)
        {
            if(strcmp(pcInputString,"temp")==0){
                temp = LM75_get_temp();
                printf("Temp: %.2f\r\n", temp);
            }
            else if(strstr(pcInputString, "led off") != NULL){
                printf("led off\r\n");
                int num = pcInputString[8]-48;
                if(num<0 || num>6){
                }
                if(num==0){
                    led_mask &= ~0x01;
                }
                else if(num==1){
                    led_mask &= ~0x02;
                }
                else if(num==2){
                    led_mask &= ~0x04;
                }
                else if(num==3){
                    led_mask &= ~0x08;
                }
                else if(num==4){
                    led_mask &= ~0x10;
                }
                else if(num==5){
                    led_mask &= ~0x20;
                }
                else if(num==6){
                    led_mask &= ~0x40;
                }
            }
            else if(strstr(pcInputString, "led on") != NULL){
                int num = pcInputString[7]-48;
                printf("led on\r\n");
                if(num<0 || num>6){
                    printf("\r\nInvalid command, please follow the format: led on <0-6>\r\n");
                }
                if(num==0){
                    led_mask |= 0x01;
                }
                else if(num==1){	
                    led_mask |= 0x02;
                }
                else if(num==2){
                    led_mask |= 0x04;
                }
                else if(num==3){
                    led_mask |= 0x08;
                }
                else if(num==4){
                    led_mask |= 0x10;
                }
                else if(num==5){
                    led_mask |= 0x20;
                }
                else if(num==6){
                    led_mask |= 0x40;
                }
                
            }
            else{
                printf("Rx'd: %s\n", pcInputString);
            }
            
            
            memset(pcInputString, 0, 500);
            cInputIndex = 0;
            ALERT_CONSOLE_RX = false;


    }
}