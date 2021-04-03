#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

void hal_gpio_init(void);
void hal_gpio_set(uint8_t pin, uint8_t value);
uint8_t hal_gpio_get(uint8_t pin);

#endif
