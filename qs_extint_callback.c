/**
 * \file
 *
 * \brief SAM External Interrupt Driver Quick Start
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

#include <asf.h>

void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

void configure_extint_channel(void)
{
  struct extint_chan_conf config_extint_chan;
  extint_chan_get_config_defaults(&config_extint_chan);

  config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
  config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
  config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
  config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
  extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
}

void configure_extint_callbacks(void)
{
  extint_register_callback(extint_detection_callback, BUTTON_0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
  extint_chan_enable_callback(BUTTON_0_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT);
}

void extint_detection_callback(void)
{
  bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
  port_pin_set_output_level(LED_0_PIN, pin_state);
}

int main(void)
{
  system_init();

  configure_extint_channel();
  configure_extint_callbacks();

  system_interrupt_enable_global();

  while (true) {
    /* Do nothing - EXTINT will fire callback asynchronously */
  }
}
