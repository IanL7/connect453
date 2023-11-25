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

// NOTE: PLAYER 1 ASSUMED TO BE YELLOW!
// NOTE: PLAYER 2 ASSUMED TO BE BLUE!


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Plain Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////

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
        for (sample = 0; sample < 34078; sample++)
        {
            /* Write the 16 bit value as DAC input */
            cyhal_dac_write(&my_dac_obj, 0xFF * startup[sample]);
            /* Return the 16 bit DAC register value */
            dac_read = cyhal_dac_read(&my_dac_obj);
            if (dac_read == sample)
            {
                /* Insert code */
            }
            // TODO: is this enough? Do I need a timer instead?
            // Delay for ~1/8000ths of a second, (sample rate of audio array is 8khz)
            cyhal_system_delay_us(125);
        }
    }
    /* Release DAC object after use by calling cyhal_dac_free, this de-initializes the DAC (including its output) */
    cyhal_dac_free(&my_dac_obj);
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

    printf("Playing startup sound\n\r");

    // Startup sound
    //mcu_startup_sound();
    cyhal_pwm_t pwm_obj;
    /* Initialize PWM on the supplied pin and assign a new clock */
    rslt = cyhal_pwm_init(&pwm_obj, P9_6, NULL);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 50, 600);
    /* Start the PWM output */
    rslt = cyhal_pwm_start(&pwm_obj);

    cyhal_system_delay_ms(10000);

    rslt = cyhal_pwm_stop(&pwm_obj);


    printf("Done with startup sound\n\r");
    
    for (;;)
    {
    }
}