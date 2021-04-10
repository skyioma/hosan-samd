/**
 * \file
 *
 * \brief SAM D21 Non Volatile Memory Driver
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

#include <asf.h>
#include <string.h>

#include "hal_nvm.h"

void hal_nvm_init()
{
  struct nvm_config config_nvm;

  nvm_get_config_defaults(&config_nvm);

  config_nvm.manual_page_write = false;

  nvm_set_config(&config_nvm);
}

uint32_t hal_nvm_bsec_state_save(const uint8_t *state_buffer, uint32_t n_buffer)
{
  if (n_buffer > NVMCTRL_ROW_SIZE - 12)
    return 0;

  uint32_t address = FLASH_SIZE - NVMCTRL_ROW_SIZE;
  uint8_t buffer[NVMCTRL_ROW_SIZE];
  enum status_code error_code;

  buffer[0] = 0xDE;
  buffer[1] = 0xAD;
  buffer[2] = 0xBE;
  buffer[3] = 0xEF;

  buffer[4] = (n_buffer >>  0) & 0xFF;
  buffer[5] = (n_buffer >>  8) & 0xFF;
  buffer[6] = (n_buffer >> 16) & 0xFF;
  buffer[7] = (n_buffer >> 24) & 0xFF;

  memcpy(&buffer[8], state_buffer, n_buffer);

  // TODO crc32

  do {
    error_code = nvm_erase_row(address);
  } while (error_code == STATUS_BUSY);

  for (int i = 0; i < NVMCTRL_ROW_PAGES; ++i) {
    do {
      error_code = nvm_write_buffer(address, &buffer[i * NVMCTRL_PAGE_SIZE], NVMCTRL_PAGE_SIZE);
    } while (error_code == STATUS_BUSY);

    do {
      error_code = nvm_read_buffer(address, &buffer[i * NVMCTRL_PAGE_SIZE], NVMCTRL_PAGE_SIZE);
    } while (error_code == STATUS_BUSY);

    address += NVMCTRL_PAGE_SIZE;
  }

  return n_buffer;
}

uint32_t hal_nvm_bsec_state_load(uint8_t *state_buffer, uint32_t n_buffer)
{
  uint32_t address = FLASH_SIZE - NVMCTRL_ROW_SIZE;
  uint8_t* ptr = (uint8_t*)address;

  if (ptr[0] != 0xDE || ptr[1] != 0xAD || ptr[2] != 0xBE || ptr[3] != 0xEF)
    return 0;

  const uint32_t length = (((uint32_t)ptr[4]) <<  0) |
                          (((uint32_t)ptr[5]) <<  8) |
                          (((uint32_t)ptr[6]) << 16) |
                          (((uint32_t)ptr[7]) << 24);

  if (length > NVMCTRL_ROW_SIZE - 12 || length > n_buffer)
    return 0;

  memcpy(state_buffer, ptr + 8, length);
  return length;
}
