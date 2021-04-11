#include "drv_nrf24l01p.h"

#include <asf.h>

#include <stdio.h>
#include <string.h>

#include "hal_spi.h"

// SPI Commands (1st byte).
#define NRF24_COMMAND_R_REGISTER         0x00
#define NRF24_COMMAND_W_REGISTER         0x20
#define NRF24_COMMAND_FLUSH_TX           0xE1
#define NRF24_COMMAND_W_TX_PAYLOAD_NOACK 0xB0

// NRF24 Registers.
#define NRF24_REG_00_CONFIG              0x00
#define NRF24_REG_02_EN_RXADDR           0x02
#define NRF24_REG_04_SETUP_RETR          0x04
#define NRF24_REG_05_RF_CH               0x05
#define NRF24_REG_06_RF_SETUP            0x06
#define NRF24_REG_0A_RX_ADDR_P0          0x0A
#define NRF24_REG_10_TX_ADDR             0x10
#define NRF24_REG_1C_DYNPD               0x1C
#define NRF24_REG_1D_FEATURE             0x1D

#define NRF24_RADIO_CHANNEL              1
#define DATA_PACKET_SIZE_MAX             32

static void write_command(uint8_t cmd, const uint8_t *data, uint8_t data_len);
static void write_register_u8(uint8_t reg, uint8_t data);
static void write_register(uint8_t reg, const uint8_t *data, uint8_t data_len);

static const uint8_t address[] = { 0x89, 0x67, 0x45, 0x23, 0x01 };

void drv_nrf24_init()
{
  write_register_u8(NRF24_REG_06_RF_SETUP, 0x0E); // 2Mbps, 0dBm

  write_register_u8(NRF24_REG_02_EN_RXADDR, 0x01); // enable data pipe 0 only
  write_register_u8(NRF24_REG_04_SETUP_RETR, 0x3F); // ARD 1ms, ARC 15 times
  write_register_u8(NRF24_REG_05_RF_CH, NRF24_RADIO_CHANNEL); // channel

  write_register(NRF24_REG_0A_RX_ADDR_P0, address, sizeof(address)); // RX ADDR P0
  write_register(NRF24_REG_10_TX_ADDR, address, sizeof(address)); // TX ADDR

  write_register_u8(NRF24_REG_1D_FEATURE, 0x07); // EN_DPL, EN_ACK_PAY, EN_DYN_ACK
  write_register_u8(NRF24_REG_1C_DYNPD, 0x01); // DPL_P0

  write_register_u8(NRF24_REG_00_CONFIG, 0x0E); // RX_DR, TX_DS, MAX_RT, CRC16, PWR_UP, PTX
  write_command(NRF24_COMMAND_FLUSH_TX, NULL, 0);
}

void write_command(uint8_t cmd, const uint8_t *data, uint8_t data_len)
{
  uint8_t read_buf[DATA_PACKET_SIZE_MAX + 1];
  uint8_t write_buf[DATA_PACKET_SIZE_MAX + 1];

  write_buf[0] = cmd;

  if (data != NULL)
    memcpy(&write_buf[1], data, data_len);

  hal_spi_take();
  port_pin_set_output_level(NRF_CS_PIN, NRF_CS_ACTIVE);
  (void)hal_spi_transceive_data(write_buf, read_buf, data_len + 1);
  port_pin_set_output_level(NRF_CS_PIN, !NRF_CS_ACTIVE);
  hal_spi_give();
}

void write_register_u8(uint8_t reg, uint8_t data)
{
  write_register(reg, &data, 1);
}

void write_register(uint8_t reg, const uint8_t *data, uint8_t data_len)
{
  uint8_t read_buf[DATA_PACKET_SIZE_MAX + 1];
  uint8_t write_buf[DATA_PACKET_SIZE_MAX + 1];

  write_buf[0] = NRF24_COMMAND_W_REGISTER | reg;
  memcpy(&write_buf[1], data, data_len);

  hal_spi_take();
  port_pin_set_output_level(NRF_CS_PIN, NRF_CS_ACTIVE);
  (void)hal_spi_transceive_data(write_buf, read_buf, data_len + 1);
  port_pin_set_output_level(NRF_CS_PIN, !NRF_CS_ACTIVE);
  hal_spi_give();
}

void drv_nrf24_send_noack(const uint8_t *data, uint8_t len)
{
  write_command(NRF24_COMMAND_W_TX_PAYLOAD_NOACK, data, len);

  port_pin_set_output_level(NRF_CE_PIN, NRF_CE_ACTIVE);
  delay_us(11);
  port_pin_set_output_level(NRF_CE_PIN, NRF_CE_ACTIVE);
}
