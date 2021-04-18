#include "sensor_data.h"

#include <stdio.h>

#define BUF_SIZE     16
#define CB_POOL_SIZE 16

struct sensor_data {
  uint8_t bsec_status;
  uint16_t iaq;
  uint8_t iaq_accuracy;
  uint16_t static_iaq;
  int8_t temperature;
  uint32_t pressure;
  uint8_t humidity;

  uint16_t vbat;
};

static struct sensor_data last_sensor_data;

struct callback_desc {
  callback_t callback;
  uint8_t order;
};

static struct callback_desc cb_pool[CB_POOL_SIZE];
static volatile uint8_t in_callback;

static void handle_callbacks(void);

void sensor_register_callback(callback_t callback, uint8_t order)
{
  for (unsigned i = 0; i < CB_POOL_SIZE; ++i) {
    if (cb_pool[i].callback == NULL || cb_pool[i].order > order) {
      for (unsigned l = CB_POOL_SIZE - 1; l > i; --l) {
        if (cb_pool[l - 1].callback != NULL)
          cb_pool[l] = cb_pool[l - 1];
      }

      cb_pool[i].callback = callback;
      cb_pool[i].order = order;
      break;
    }
  }
}

void handle_callbacks()
{
  if (in_callback != 0)
    return;

  ++in_callback;

  for (unsigned i = 0; i < CB_POOL_SIZE; ++i) {
    if (cb_pool[i].callback == NULL)
      break;

    cb_pool[i].callback();
  }

  --in_callback;
}

void sensor_update_bme680_data(uint8_t bsec_status, uint16_t iaq, uint8_t iaq_accuracy,
                               uint16_t static_iaq, int8_t temperature, uint32_t pressure,
                               uint8_t humidity)
{
  last_sensor_data.bsec_status = bsec_status;
  last_sensor_data.iaq = iaq;
  last_sensor_data.iaq_accuracy = iaq_accuracy;
  last_sensor_data.static_iaq = static_iaq;
  last_sensor_data.temperature = temperature;
  last_sensor_data.pressure = pressure;
  last_sensor_data.humidity = humidity;

  handle_callbacks();
}

void sensor_update_vbat(uint16_t mv)
{
  last_sensor_data.vbat = mv;
  handle_callbacks();
}

const char* sensor_get_last_bsec_status(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.bsec_status);
  return buf;
}

const char* sensor_get_last_iaq(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.iaq);
  return buf;
}

const char* sensor_get_last_iaq_accuracy(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.iaq_accuracy);
  return buf;
}

const char* sensor_get_last_static_iaq(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.static_iaq);
  return buf;
}

const char* sensor_get_last_temperature(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*d", width, last_sensor_data.temperature);
  return buf;
}

const char* sensor_get_last_pressure(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*lu", width, last_sensor_data.pressure);
  return buf;
}

const char* sensor_get_last_humidity(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.humidity);
  return buf;
}

const char* sensor_get_last_vbat(uint8_t width)
{
  static char buf[BUF_SIZE];

  sprintf(buf, "%*u", width, last_sensor_data.vbat);
  return buf;
}
