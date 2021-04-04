/**
 * \file
 *
 * \brief SAM RTC Count Callback Quick Start
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>

void rtc_overflow_callback(void);
void configure_rtc_count(void);
void configure_rtc_callbacks(void);

struct rtc_module rtc_instance;

void rtc_overflow_callback(void)
{
  /* Do something on RTC overflow here */
  port_pin_toggle_output_level(LED_0_PIN);
}

void configure_rtc_count(void)
{
  struct rtc_count_config config_rtc_count;
  rtc_count_get_config_defaults(&config_rtc_count);

  config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1;
  config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
#ifdef FEATURE_RTC_CONTINUOUSLY_UPDATED
  config_rtc_count.continuously_update = true;
#endif
  rtc_count_init(&rtc_instance, RTC, &config_rtc_count);

  rtc_count_enable(&rtc_instance);
}

void configure_rtc_callbacks(void)
{
  rtc_count_register_callback(&rtc_instance, rtc_overflow_callback, RTC_COUNT_CALLBACK_OVERFLOW);
  rtc_count_enable_callback(&rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW);
}

int main(void)
{
  /* Initialize system. Must configure conf_clocks.h first. */
  system_init();

  /* Configure and enable RTC */
  configure_rtc_count();

  /* Configure and enable callback */
  configure_rtc_callbacks();

  /* Set period */
  rtc_count_set_period(&rtc_instance, 2000);

  while (true) {
    /* Infinite while loop */
  }
}
