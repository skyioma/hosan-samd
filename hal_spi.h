#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdint.h>

void hal_spi_init(void);
void hal_spi_write_byte(uint8_t byte);
int8_t hal_spi_transceive_data(const uint8_t *write_buf, uint8_t *read_buf, uint8_t size);

void hal_spi_take(void);
void hal_spi_give(void);

#endif
