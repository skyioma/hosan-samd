#include "hal_delay.h"

#include <asf.h>

void hal_delay_init()
{
  delay_init();
}

void hal_delay_ms(uint32_t ms)
{
  vTaskDelay(ms);
}
