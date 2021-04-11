#ifndef VIEWS_H
#define VIEWS_H

#include "drv_keyboard.h"

void views_init(void);
void views_paint(void);
void views_update(void);

void view_handle_button_1(enum button_press_kind kind);
void view_handle_button_2(enum button_press_kind kind);

#endif
