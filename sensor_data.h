#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>

typedef void (*callback_t)(void);

void sensor_update_bme680_data(uint8_t bsec_status, uint16_t iaq, uint8_t iaq_accuracy,
                               uint16_t static_iaq, int8_t temperature, uint32_t pressure,
                               uint8_t humidity);

void sensor_update_vbat(uint16_t mv);

const char* sensor_get_last_bsec_status(uint8_t width);
const char* sensor_get_last_iaq(uint8_t width);
const char* sensor_get_last_iaq_accuracy(uint8_t width);
const char* sensor_get_last_static_iaq(uint8_t width);
const char* sensor_get_last_temperature(uint8_t width);
const char* sensor_get_last_pressure(uint8_t width);
const char* sensor_get_last_humidity(uint8_t width);

const char* sensor_get_last_vbat(uint8_t width);

void sensor_register_callback(callback_t callback, uint8_t order);

#endif
