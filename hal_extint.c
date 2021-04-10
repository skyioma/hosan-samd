/**
 * \file
 *
 * \brief SAM D21 External Interrupt Driver
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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

#include "hal_extint.h"

static void button_1_callback(void);
static void button_2_callback(void);

static EventGroupHandle_t event_group;

void hal_extint_init()
{
  struct extint_chan_conf config_extint_chan;

  extint_chan_get_config_defaults(&config_extint_chan);

  config_extint_chan.gpio_pin           = BUTTON_1_EIC_PIN;
  config_extint_chan.gpio_pin_mux       = BUTTON_1_EIC_MUX;
  config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
  config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
  extint_chan_set_config(BUTTON_1_EIC_LINE, &config_extint_chan);

  extint_register_callback(button_1_callback, BUTTON_1_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
  extint_chan_enable_callback(BUTTON_1_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);

  config_extint_chan.gpio_pin           = BUTTON_2_EIC_PIN;
  config_extint_chan.gpio_pin_mux       = BUTTON_2_EIC_MUX;
  config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
  config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
  extint_chan_set_config(BUTTON_2_EIC_LINE, &config_extint_chan);

  extint_register_callback(button_2_callback, BUTTON_2_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
  extint_chan_enable_callback(BUTTON_2_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
}

void button_1_callback()
{
  if (event_group != NULL) {
    const bool pin_state = port_pin_get_input_level(BUTTON_1_PIN);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    const BaseType_t xResult = xEventGroupSetBitsFromISR(event_group,
                                                         pin_state == BUTTON_1_ACTIVE ? 0x01 : 0x02,
                                                         &xHigherPriorityTaskWoken);

    // TODO Handle xResult != pdPASS
    (void)xResult;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

void button_2_callback()
{
  if (event_group != NULL) {
    const bool pin_state = port_pin_get_input_level(BUTTON_1_PIN);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    const BaseType_t xResult = xEventGroupSetBitsFromISR(event_group,
                                                         pin_state == BUTTON_1_ACTIVE ? 0x04 : 0x08,
                                                         &xHigherPriorityTaskWoken);

    // TODO Handle xResult != pdPASS
    (void)xResult;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

void hal_extint_set_event_group(EventGroupHandle_t event_group_)
{
  event_group = event_group_;
}
