/*
 * light_sensor_task.c
 *
 *  Created on: Nov 30, 2023
 *      Authors: Christian VanIten and Ian Lodes
 */

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "main.h"
#include "light_sensor_task.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

cyhal_i2c_t light_sensor_i2c_obj;

// Could potentially turn this into a one shot function -
// Don't necessarily need to constantly check light value
void task_light_sensor(void *param)
{
    /* Suppress warning for unused parameter */
    (void)param;

    printf("entered light sensor\n\r");

    uint8_t Data0[1], Data1[1], Data2[1], Data3[1];
    uint8_t Write0[1], Write1[1], Write2[1], Write3[1];

    // Read Data_CH1 Lower bits
    Write0[0] = 0x88;
    Write1[0] = 0x89;
    Write2[0] = 0x8A;
    Write3[0] = 0x8B;

    // Read Data_CH1 upper bits
    for (;;)
    {
        vTaskDelay(500);
        printf("loop iter\n\r");
        /* Use cyhal_i2c_master_write to write the required data to the device. */
        /* Send the register address, do not generate a stop condition.  This will result in */
        /* a restart condition. */
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                    &light_sensor_i2c_obj,
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Write0,				 // Array of data to write
                                    1,					 // Number of bytes to write
                                    0,					 // Block until completed
                                    false))				 // Do NOT generate Stop Condition
        {
            /* Use cyhal_i2c_master_read to read the required data from the device. */
            // The register address has already been set in the write above, so read a single byte
            // of data.
            if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                        &light_sensor_i2c_obj,		 // I2C Object
                                        ALS_SUBORDINATE_ADDR, // I2C Address
                                        Data0,				 // Read Buffer
                                        1,					 // Number of bytes to read
                                        0,					 // Block until completed
                                        true))				 // Generate Stop Condition
            {
                // printf("int0: %d\n", Data0[0]);
            }
            else
            {
                printf("error reading 0\n\r");
                /* Disable all interrupts. */
                __disable_irq();
                CY_ASSERT(0);
                while (1)
                {
                };
            }
        }
        else
        {
            /* Disable all interrupts. */
            printf("error writing 0\n\r");
            __disable_irq();
            CY_ASSERT(0);
            while (1)
            {
            };
        }

        /* Use cyhal_i2c_master_write to write the required data to the device. */
        /* Send the register address, do not generate a stop condition.  This will result in */
        /* a restart condition. */
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                    &light_sensor_i2c_obj,
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Write1,				 // Array of data to write
                                    1,					 // Number of bytes to write
                                    0,					 // Block until completed
                                    false))				 // Do NOT generate Stop Condition
        {
            /* Use cyhal_i2c_master_read to read the required data from the device. */
            // The register address has already been set in the write above, so read a single byte
            // of data.
            if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                        &light_sensor_i2c_obj,		 // I2C Object
                                        ALS_SUBORDINATE_ADDR, // I2C Address
                                        Data1,				 // Read Buffer
                                        1,					 // Number of bytes to read
                                        0,					 // Block until completed
                                        true))				 // Generate Stop Condition
            {
                // printf("int1: %d\n", Data1[0]);
            }
            else
            {
                /* Disable all interrupts. */
                printf("error reading 1\n\r");
                __disable_irq();
                CY_ASSERT(0);
                while (1)
                {
                };
            }
        }
        else
        {
            /* Disable all interrupts. */
            printf("error writing 1\n\r");
            __disable_irq();
            CY_ASSERT(0);
            while (1)
            {
            };
        }

        // Read Data_CH0 lower bits

        /* Use cyhal_i2c_master_write to write the required data to the device. */
        /* Send the register address, do not generate a stop condition.  This will result in */
        /* a restart condition. */
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                    &light_sensor_i2c_obj,
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Write2,				 // Array of data to write
                                    1,					 // Number of bytes to write
                                    0,					 // Block until completed
                                    false))				 // Do NOT generate Stop Condition
        {
            /* Use cyhal_i2c_master_read to read the required data from the device. */
            // The register address has already been set in the write above, so read a single byte
            // of data.
            if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                        &light_sensor_i2c_obj,		 // I2C Object
                                        ALS_SUBORDINATE_ADDR, // I2C Address
                                        Data2,				 // Read Buffer
                                        1,					 // Number of bytes to read
                                        0,					 // Block until completed
                                        true))				 // Generate Stop Condition
            {
                // printf("int2: %d\n", Data2[0]);
            }
            else
            {
                /* Disable all interrupts. */
                printf("error reading 2\n\r");
                __disable_irq();
                CY_ASSERT(0);
                while (1)
                {
                };
            }
        }
        else
        {
            /* Disable all interrupts. */
            printf("error writing 2\n\r");
            __disable_irq();
            CY_ASSERT(0);
            while (1)
            {
            };
        }

        // Read Data_CH0 upper bits

        /* Use cyhal_i2c_master_write to write the required data to the device. */
        /* Send the register address, do not generate a stop condition.  This will result in */
        /* a restart condition. */
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                    &light_sensor_i2c_obj,
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Write3,				 // Array of data to write
                                    1,					 // Number of bytes to write
                                    0,					 // Block until completed
                                    false))				 // Do NOT generate Stop Condition
        {
            /* Use cyhal_i2c_master_read to read the required data from the device. */
            // The register address has already been set in the write above, so read a single byte
            // of data.
            if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                        &light_sensor_i2c_obj,		 // I2C Object
                                        ALS_SUBORDINATE_ADDR, // I2C Address
                                        Data3,				 // Read Buffer
                                        1,					 // Number of bytes to read
                                        0,					 // Block until completed
                                        true))				 // Generate Stop Condition
            {
                // printf("int3: %d\n", Data3[0]);
            }
            else
            {
                /* Disable all interrupts. */
                printf("error reading 3\n\r");
                __disable_irq();
                CY_ASSERT(0);
                while (1)
                {
                };
            }
        }
        else
        {
            /* Disable all interrupts. */
            printf("error writing 3\n\r");
            __disable_irq();
            CY_ASSERT(0);
            while (1)
            {
            };
        }

        uint8_t data1 = Data1[0];
        uint8_t data3 = Data3[0];

        printf("Channel 1: %d\n", (data1 << 8) | Data0[0]);
        printf("Channel 0: %d\n\n\n", (data3 << 8) | Data2[0]);

        xQueueOverwrite(xLightQueue, &data1);
        printf("light sensor loop\n\r");
    }
}

void light_sensor_init()
{
    /* Declare variables */
    cy_rslt_t rslt;
    cyhal_i2c_t light_sensor_i2c_obj;
    /* Define frequency */
    uint32_t I2C_MASTER_FREQUENCY = 100000u;
    /* Define the I2C master configuration structure */
    cyhal_i2c_cfg_t i2c_master_config = {CYHAL_I2C_MODE_MASTER, 0 /* address is not used for master mode */, I2C_MASTER_FREQUENCY};
    /* Initialize I2C master, set the SDA and SCL pins and assign a new clock */
    rslt = cyhal_i2c_init(&light_sensor_i2c_obj, PIN_I2C_SDA, PIN_I2C_SCL, NULL);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("error initializing light sensor 1 \n\r");
    }
    /* Configure the I2C resource to be master */
    rslt = cyhal_i2c_configure(&light_sensor_i2c_obj, &i2c_master_config);
    if (rslt != CY_RSLT_SUCCESS)
    {
        printf("error initializing light sensor 2 \n\r");
    }

    Cy_SysLib_Delay(200);

    // turn on the device by writing to config buffer
    uint8_t config_buffer[2];
    config_buffer[0] = 0x80;
    config_buffer[1] = 0x01;

    if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &light_sensor_i2c_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                config_buffer,		  // Array of data to write
                                2,					  // Number of bytes to write
                                0,					  // Block until completed
                                true) )				  // Do generate Stop Condition
    {
        printf("successfully initialized light sensor\n\r");
    }
    else 
    {
        printf("error initializing light sensor 3\n\r");
    }
}