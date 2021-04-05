#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

void radio_send_bme680_data(uint8_t bsec_status, uint16_t iaq, uint8_t iaq_accuracy,
                            uint16_t static_iaq, int8_t temperature, uint32_t pressure,
                            uint8_t humidity);

#endif
