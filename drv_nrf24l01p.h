#ifndef NRF_UTILS_H
#define NRF_UTILS_H

#include <stdint.h>

void drv_nrf24_init(void);
void drv_nrf24_send_noack(const uint8_t *data, uint8_t len);

#endif
