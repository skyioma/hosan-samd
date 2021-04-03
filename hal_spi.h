#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdint.h>

void hal_spi_init(void);
void hal_spi_write_byte(uint8_t byte);

#endif
