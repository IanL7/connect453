#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
/* RTOS header files */
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <semphr.h>
#include <event_groups.h>
/*******************************************************************************
* External Global Variables
******************************************************************************/
void task_blink_led(void *param);

void mcu_all_leds_init()
{
  cy_rslt_t rslt;

  // LED1
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

  // LED 2
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

  // LED 3
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

int main(void)
{
  cy_rslt_t rslt;

  /* Initialize the device and board peripherals */
  rslt = cybsp_init();
  CY_ASSERT(CY_RSLT_SUCCESS == rslt);

  __enable_irq();

  // create a task to blink the onboard LED
  xTaskCreate(
    task_blink_led,
    "Blink LED Task",
    configMINIMAL_STACK_SIZE,
    NULL,
    3,
    NULL);
  
  // Start the scheduler
  vTaskStartScheduler();

  for (;;)
  {
  }
}

void task_blink_led(void *param)
{
  cy_rslt_t rslt;

  /* Suppress warning for unused parameter */
  (void)param;

  // Initialize LED pins
  mcu_all_leds_init();

  /* Repeatedly running part of the task */
  for (;;)
  {
    vTaskDelay(300);
    cyhal_gpio_toggle(P5_5);
  }
}
