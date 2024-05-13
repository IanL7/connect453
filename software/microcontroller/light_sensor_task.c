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
#include "drivers/i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>

// Could potentially turn this into a one shot function -
// Don't necessarily need to constantly check light value
uint16_t read_light_sensor()
{
    // turn on the device by writing to config buffer
    uint8_t config_buffer[2];
    config_buffer[0] = 0x80;
    config_buffer[1] = 0x01;

    if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &i2c_master_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                config_buffer,		  // Array of data to write
                                2,					  // Number of bytes to write
                                1000,					  // Block until completed
                                true) )				  // Do generate Stop Condition
    {
        printf("successfully initialized light sensor\n\r");
    }
    else 
    {
        printf("error initializing light sensor 3\n\r");
    }

    uint8_t Data0[1], Data1[1], Data2[1], Data3[1];
    uint8_t Write0[1], Write1[1], Write2[1], Write3[1];

    // Read Data_CH1 Lower bits
    Write0[0] = 0x88;
    Write1[0] = 0x89;
    Write2[0] = 0x8A;
    Write3[0] = 0x8B;

    uint16_t light_value;
    bool error = false;

    // Read Data_CH1 upper bits

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    /* Send the register address, do not generate a stop condition.  This will result in */
    /* a restart condition. */
    if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &i2c_master_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                Write0,				 // Array of data to write
                                1,					 // Number of bytes to write
                                1000,					 // Block until completed
                                false))				 // Do NOT generate Stop Condition
    {
        /* Use cyhal_i2c_master_read to read the required data from the device. */
        // The register address has already been set in the write above, so read a single byte
        // of data.
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                    &i2c_master_obj,		 // I2C Object
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Data0,				 // Read Buffer
                                    1,					 // Number of bytes to read
                                    1000,					 // Block until completed
                                    true))				 // Generate Stop Condition
        {
            // printf("int0: %d\n", Data0[0]);
        }
        else
        {
            printf("error reading 0\n\r");
            error = true;
        }
    }
    else
    {
        printf("error writing 0\n\r");
        error = true;
    }

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    /* Send the register address, do not generate a stop condition.  This will result in */
    /* a restart condition. */
    if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &i2c_master_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                Write1,				 // Array of data to write
                                1,					 // Number of bytes to write
                                1000,					 // Block until completed
                                false))				 // Do NOT generate Stop Condition
    {
        /* Use cyhal_i2c_master_read to read the required data from the device. */
        // The register address has already been set in the write above, so read a single byte
        // of data.
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                    &i2c_master_obj,		 // I2C Object
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Data1,				 // Read Buffer
                                    1,					 // Number of bytes to read
                                    1000,					 // Block until completed
                                    true))				 // Generate Stop Condition
        {
            // printf("int1: %d\n", Data1[0]);
        }
        else
        {
            printf("error reading 1\n\r");
            error = true;
        }
    }
    else
    {
        printf("error writing 1\n\r");
        error = true;
    }

    // Read Data_CH0 lower bits

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    /* Send the register address, do not generate a stop condition.  This will result in */
    /* a restart condition. */
    if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &i2c_master_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                Write2,				 // Array of data to write
                                1,					 // Number of bytes to write
                                1000,					 // Block until completed
                                false))				 // Do NOT generate Stop Condition
    {
        /* Use cyhal_i2c_master_read to read the required data from the device. */
        // The register address has already been set in the write above, so read a single byte
        // of data.
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                    &i2c_master_obj,		 // I2C Object
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Data2,				 // Read Buffer
                                    1,					 // Number of bytes to read
                                    1000,					 // Block until completed
                                    true))				 // Generate Stop Condition
        {
            // printf("int2: %d\n", Data2[0]);
        }
        else
        {
            error = true;
            printf("error reading 2\n\r");
        }
    }
    else
    {
        error = true;
        printf("error writing 2\n\r");
    }

    // Read Data_CH0 upper bits

    /* Use cyhal_i2c_master_write to write the required data to the device. */
    /* Send the register address, do not generate a stop condition.  This will result in */
    /* a restart condition. */
    if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(
                                &i2c_master_obj,
                                ALS_SUBORDINATE_ADDR, // I2C Address
                                Write3,				 // Array of data to write
                                1,					 // Number of bytes to write
                                1000,					 // Block until completed
                                false))				 // Do NOT generate Stop Condition
    {
        /* Use cyhal_i2c_master_read to read the required data from the device. */
        // The register address has already been set in the write above, so read a single byte
        // of data.
        if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(
                                    &i2c_master_obj,		 // I2C Object
                                    ALS_SUBORDINATE_ADDR, // I2C Address
                                    Data3,				 // Read Buffer
                                    1,					 // Number of bytes to read
                                    1000,					 // Block until completed
                                    true))				 // Generate Stop Condition
        {
            // printf("int3: %d\n", Data3[0]);
        }
        else
        {
            error = true;
            printf("error reading 3\n\r");
        }
    }
    else
    {
        error = true;
        printf("error writing 3\n\r");
    }

    if (!error)
    {
        light_value = (Data3[0] << 8) | Data2[0];
    }
    else 
    {
        light_value = 100;
    }

        //printf("Channel 0: %d\n\n\n", (Data3[0] << 8) | Data2[0]);

    return light_value;
}

void light_sensor_init()
{
    i2c_init();

    Cy_SysLib_Delay(200);
}