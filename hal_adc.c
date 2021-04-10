/**
 * \file
 *
 * \brief SAM D21 ADC
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

#include "hal_adc.h"

uint16_t status_battery_voltage;

static struct adc_module adc_instance;

void hal_adc_init()
{
  struct adc_config config_adc;

  adc_get_config_defaults(&config_adc);

  config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV8;
  config_adc.positive_input  = VBAT_ADC_INPUT;

  adc_init(&adc_instance, ADC, &config_adc);
}

uint16_t hal_adc_read_vbat_mv()
{
  adc_enable(&adc_instance);

  adc_start_conversion(&adc_instance);

  uint16_t result;

  do {
    /* Wait for conversion to be done and read out result */
  } while (adc_read(&adc_instance, &result) == STATUS_BUSY);

  adc_disable(&adc_instance);

  status_battery_voltage = (float)result * 1.0 / 4095 / 42.2 * (100 + 42.2) * 1000;
  return status_battery_voltage;
}
