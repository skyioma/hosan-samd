#ifndef HAL_EXTINT_H
#define HAL_EXTINT_H

#include <asf.h>

void hal_extint_init(void);
void hal_extint_set_event_group(EventGroupHandle_t event_group_);

#endif
