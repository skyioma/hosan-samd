/**
 * \file
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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

#include "hal_spi.h"

#include <asf.h>

static struct spi_module spi_master_instance;

void hal_spi_init(void)
{
  struct spi_config config_spi_master;

  /* Configure, initialize and enable SERCOM SPI module */
  spi_get_config_defaults(&config_spi_master);
  config_spi_master.mux_setting = SPI_MUX_SETTING;
  config_spi_master.pinmux_pad0 = SPI_PINMUX_PAD0;
  config_spi_master.pinmux_pad1 = SPI_PINMUX_PAD1;
  config_spi_master.pinmux_pad2 = SPI_PINMUX_PAD2;
  config_spi_master.pinmux_pad3 = SPI_PINMUX_PAD3;

  spi_init(&spi_master_instance, SPI_MODULE, &config_spi_master);

  spi_enable(&spi_master_instance);
}

void hal_spi_write_byte(uint8_t byte)
{
  (void)spi_write_buffer_wait(&spi_master_instance, &byte, 1);
}

int8_t hal_spi_transceive_data(const uint8_t *write_buf, uint8_t *read_buf, uint8_t size)
{
  (void)spi_transceive_buffer_wait(&spi_master_instance, write_buf, read_buf, size);
  return 0;
}
