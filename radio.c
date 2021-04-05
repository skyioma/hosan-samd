#include "radio.h"

#include <string.h>

#include <radio-proto.h>

#include "drv_nrf24l01p.h"

#define DATA_PACKET_SIZE_MAX 32
// Radio protocol version.
#define RADIO_PROTO_VERSION  0
// This device unique id.
#define RADIO_DEVICE_ID      0x11

// extern uint16_t status_battery_voltage;

// static uint16_t status_word = 0x24; // CM1S_BAT_VOLTAGE | CM1S_APHT_DATA
static uint16_t status_word = 0x20; // CM1S_APHT_DATA;
static uint8_t last_sent_counter;

// Packet direction: Device -> Controller.
//
// Byte 0:
//    Bits 7..5: protocol version
//    Bits 4..0: sender device id (up to 30 devices + 1 controller + 1 reserved)
// Byte 1: 8-bit wrapping counter (starts from zero, per direction)
// Byte 2: Command from Communication Model 1 Set: mandatory CM1C_STATUS
// Bute 3+: Command dependent...

void radio_send_bme680_data(uint8_t bsec_status, uint16_t iaq, uint8_t iaq_accuracy,
                            uint16_t static_iaq, int8_t temperature, uint32_t pressure,
                            uint8_t humidity)
{
  uint8_t pkt[DATA_PACKET_SIZE_MAX];
  uint8_t len = 0;

  pkt[len] = (RADIO_PROTO_VERSION << 5) | RADIO_DEVICE_ID;
  len += 1;

  pkt[len] = last_sent_counter;
  len += 1;

  ++last_sent_counter;

  pkt[len] = CM1C_STATUS;
  len += 1;

#if 0
  if (cmd == CM1C_STATUS)
#endif
  {
    // Take into account previous cycle errors also!

#if 0
    if (any_error())
      set_status_bit(CM1S_ERROR, 1);
#endif

    memcpy(&pkt[len], &status_word, sizeof(status_word));
    len += 2;

#if 0
    if (get_status_bit(CM1S_ERROR))
    {
      uint16_t error = any_error();

      memcpy(&pkt[len], &error, sizeof(error));
      len += 2;
    }

    if (get_status_bit(CM1S_RUNTIME))
    {
      uint32_t runtime = 0; // millis();

      memcpy(&pkt[len], &runtime, sizeof(runtime));
      len += 4;

      memcpy(&pkt[len], &loop_counter, sizeof(loop_counter));
      len += 4;
    }

    if (get_status_bit(CM1S_BAT_VOLTAGE))
    {
      memcpy(&pkt[len], &status_battery_voltage, sizeof(status_battery_voltage));
      len += 2;
    }
#endif

#if 0
    if (get_status_bit(CM1S_APHT_DATA))
#endif
    {
      pkt[len + 0] = bsec_status;
      pkt[len + 1] = iaq_accuracy;
      pkt[len + 2] = iaq / 2;
      pkt[len + 3] = static_iaq / 2;

      const uint32_t p = pressure - 50000;

      pkt[len + 4] = p & 0x000000ff;
      pkt[len + 5] = (p & 0x0000ff00) >> 8;
      pkt[len + 6] = humidity;
      pkt[len + 7] = temperature;

      len += 8;
    }
  }
#if 0
  else if (cmd == CM1C_PONG)
  {
    memcpy(&pkt[len], data, data_len);
    len += data_len;
  }
  else
  {
    set_error_bit(CM1E_INT_1);
    return 1;
  }
#endif

drv_nrf24_send_noack(pkt, len);

#if 0

  delay_ms(1);

  was_nrf_irq = 0;
  nrf_send_data(pkt, len);

  uint8_t status;

  // XXX WDT RESET does not work here!

  while (nrf_sending(&status) != 0 && was_nrf_irq == 0)
    power_down();

  status = nrf_sent_ok(status);

  if (status == 0)
    set_error_bit(CM1E_CANT_DELIVER);

  return !status;
#endif
}
