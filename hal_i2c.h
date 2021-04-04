#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>

void hal_i2c_init(void);
int8_t hal_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len);
int8_t hal_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data_ptr, uint16_t data_len);

#endif
