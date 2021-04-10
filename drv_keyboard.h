#ifndef DRV_KEYBOARD_H
#define DRV_KEYBOARD_H

#include <stdint.h>

enum button_press_kind { BPK_CLICK, BPK_DOUBLE_CLICK, BPK_PUSH };

typedef void (*button_press_callback_t)(enum button_press_kind kind);

struct button_handler {
  button_press_callback_t button_1;
  button_press_callback_t button_2;
};

void drv_keyboard_init(void);
void drv_keyboard_set_handlers(const struct button_handler *handler);
void drv_keyboard_event_bits(uint8_t event_bits, uint32_t ticks, uint32_t *wait_time);


#endif
