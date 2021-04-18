/**
 * \file
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <asf.h>

#include <BSEC/bsec_iot_example.h>

#include "drv_keyboard.h"
#include "drv_nrf24l01p.h"

#include "hal_adc.h"
#include "hal_delay.h"
#include "hal_extint.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_nvm.h"
#include "hal_rtc.h"
#include "hal_spi.h"
#include "sensor_data.h"
#include "views.h"

#include "EPD/EPD_Test.h"

static void keyboard_task_init(void);
static void keyboard_task(void *params);

static void main_task(void *params)
{
  vTaskDelay(5000 / portTICK_RATE_MS);

  hal_gpio_set(EN_33VA_PIN, EN_33VA_ACTIVE);
  hal_delay_ms(1);

#if 0
  EPD_2in13_test();
  hal_gpio_set(EN_33VA_PIN, !EN_33VA_ACTIVE);
#else
  views_init();
  views_paint();
#endif

  while (1) {
    vTaskDelay(10000 / portTICK_RATE_MS);
  }
}

void keyboard_task(void *params)
{
  EventGroupHandle_t event_group = (EventGroupHandle_t)params;
  uint32_t timeout = 1000;

  const struct button_handler handler = { .button_1 = view_handle_button_1 };

  drv_keyboard_set_handlers(&handler);

  while (1) {
    const EventBits_t event_bits = xEventGroupWaitBits(event_group, // xEventGroup
                                                       0x0F,        // uxBitsToWaitFor
                                                       pdTRUE,      // xClearOnExit
                                                       pdFALSE,     // xWaitForAllBits
                                                       timeout);    // xTicksToWait

    drv_keyboard_event_bits(event_bits, xTaskGetTickCount(), &timeout);
  }
}

void keyboard_task_init()
{
  EventGroupHandle_t event_group = xEventGroupCreate();

  hal_extint_set_event_group(event_group);

  xTaskCreate(&keyboard_task,
    (const char *)"KBD task",
    configMINIMAL_STACK_SIZE + 512,
    (void *)event_group,
    configMAX_PRIORITIES - 1,
    NULL);
}

int main(void)
{
  system_init();
  dbg_init();
  hal_adc_init();
  hal_delay_init();
  hal_extint_init();
  hal_gpio_init();
  hal_i2c_init();
  hal_nvm_init();
  hal_rtc_init();
  hal_spi_init();

  drv_nrf24_init();

  xTaskCreate(&main_task,
    (const char *)"Main task",
    configMINIMAL_STACK_SIZE + 100,
    NULL,
    tskIDLE_PRIORITY + 2,
    NULL);

  xTaskCreate(&bsec_iot_task,
    (const char *)"BSEC task",
    configMINIMAL_STACK_SIZE + 1024,
    NULL,
    tskIDLE_PRIORITY + 2,
    NULL);

  keyboard_task_init();

  vTaskStartScheduler();
}
