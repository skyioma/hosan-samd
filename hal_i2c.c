/**
 * \file
 *
 * \brief SAM SERCOM I2C Master Quick Start Guide
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

#include "hal_i2c.h"

#include <asf.h>

#include <string.h>

#define MAX_I2C_BUF_SIZE 32
/* Number of times to try to send packet if failed. */
#define TIMEOUT 1000

/* Init software module. */
struct i2c_master_module i2c_master_instance;

void hal_i2c_init()
{
  /* Initialize config structure and software module. */
  struct i2c_master_config config_i2c_master;

  i2c_master_get_config_defaults(&config_i2c_master);

  /* Change buffer timeout to something longer. */
  config_i2c_master.buffer_timeout = 10000;
  config_i2c_master.pinmux_pad0 = I2C_SERCOM_PINMUX_PAD0;
  config_i2c_master.pinmux_pad1 = I2C_SERCOM_PINMUX_PAD1;

  /* Initialize and enable device with config. */
  i2c_master_init(&i2c_master_instance, I2C_MODULE, &config_i2c_master);

  i2c_master_enable(&i2c_master_instance);
}

int8_t hal_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len)
{
  uint8_t buffer[MAX_I2C_BUF_SIZE];

  if (data_len > MAX_I2C_BUF_SIZE - 1)
    return -1;

  buffer[0] = reg_addr;

  if (reg_data_ptr != NULL)
    memcpy(&buffer[1], reg_data_ptr, data_len);

  struct i2c_master_packet packet = {
    .address = dev_addr,
    .data_length = data_len + 1,
    .data = buffer,
    .ten_bit_address = false,
    .high_speed = false,
    .hs_master_code  = 0x0,
  };

  /* Timeout counter. */
  uint16_t timeout = 0;

  while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
    /* Increment timeout counter and check if timed out. */
    if (timeout++ == TIMEOUT) {
      break;
    }
  }

  return timeout <= TIMEOUT ? 0 : -1;
}

int8_t hal_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len)
{
  if (hal_i2c_write(dev_addr, reg_addr, NULL, 0) != 0)
    return -1;

  struct i2c_master_packet packet = {
    .address = dev_addr,
    .data_length = data_len,
    .data = reg_data_ptr,
    .ten_bit_address = false,
    .high_speed = false,
    .hs_master_code = 0x0,
  };

  /* Timeout counter. */
  uint16_t timeout = 0;

  while (i2c_master_read_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
    /* Increment timeout counter and check if timed out. */
    if (timeout++ == TIMEOUT) {
      break;
    }
  }

  return timeout <= TIMEOUT ? 0 : -1;
}
