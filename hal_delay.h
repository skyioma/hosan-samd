#ifndef HAL_DELAY_H
#define HAL_DELAY_H

#include <stdint.h>

void hal_delay_init(void);
void hal_delay_ms(uint32_t ms);

#endif
