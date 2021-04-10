#include "drv_keyboard.h"

#include <stddef.h>

#define MAX_WAIT_TICKS 65535
#define DOUBLE_CLICK_TICKS 500
#define PUSH_TICKS 750

struct button_state {
  uint8_t is_down;
  uint32_t down_time;
};

static struct button_state button_1_state;
static struct button_state button_2_state;

static button_press_callback_t button_1_callback;
static button_press_callback_t button_2_callback;

static void handle_button_event_bits(struct button_state *state,
                                     uint8_t event_bits,
                                     uint32_t ticks,
                                     button_press_callback_t handler,
                                     uint32_t *timeout);

void drv_keyboard_init()
{
}

void drv_keyboard_set_handlers(const struct button_handler *handler)
{
  button_1_callback = handler->button_1;
  button_2_callback = handler->button_2;
}

void drv_keyboard_event_bits(uint8_t event_bits, uint32_t ticks, uint32_t *wait_time)
{
  uint32_t button_1_timeout = MAX_WAIT_TICKS;
  uint32_t button_2_timeout = MAX_WAIT_TICKS;

  handle_button_event_bits(&button_1_state, (event_bits & 0x3), ticks, button_1_callback, &button_1_timeout);
  handle_button_event_bits(&button_2_state, ((event_bits >> 2) & 0x3), ticks, button_2_callback, &button_2_timeout);

  *wait_time = button_1_timeout <= button_2_timeout ? button_1_timeout : button_2_timeout;
}

void handle_button_event_bits(struct button_state *state,
                              uint8_t event_bits,
                              uint32_t ticks,
                              button_press_callback_t handler,
                              uint32_t *timeout)
{
  const uint32_t delta_ticks = ticks - state->down_time;

  if (event_bits == 0) {
    // Wait is timed out.

    if (state->down_time != 0) {
      // Some action is pending.

      if (state->is_down != 0) {
        // Button is currently down.

        if (delta_ticks >= PUSH_TICKS) {
          // Long enough "down" -> push.

#if 0
          dbg_print_str("push\r\n");
#endif
          if (handler != NULL)
            handler(BPK_PUSH);

          state->down_time = 0;
          *timeout = MAX_WAIT_TICKS;
        } else {
          // Woke up to early for "push" timeout.
          *timeout = PUSH_TICKS - delta_ticks;
        }
      } else {
        // Button is currently up.

        if (delta_ticks >= DOUBLE_CLICK_TICKS) {
          // Double click time is passed since first "down" -> click.

#if 0
          dbg_print_str("click\r\n");
#endif
          if (handler != NULL)
            handler(BPK_CLICK);

          state->down_time = 0;
          *timeout = MAX_WAIT_TICKS;
        } else {
          // Woke up to early for "double click" timeout.
          *timeout = DOUBLE_CLICK_TICKS - delta_ticks;
        }
      }
    }
  } else if (event_bits & 0x01) {
    // Button went down.

    if (state->down_time != 0 && delta_ticks <= DOUBLE_CLICK_TICKS) {
#if 0
      dbg_print_str("double\r\n");
#endif
      if (handler != NULL)
        handler(BPK_DOUBLE_CLICK);

      state->down_time = 0;
      *timeout = MAX_WAIT_TICKS;
    } else {
      state->down_time = ticks;
      *timeout = PUSH_TICKS;
    }

    state->is_down = 1;
  } else if (event_bits & 0x02) {
    // Button went up.

    if (state->down_time == 0) {
      // Do nothing. Released earlier.
      *timeout = MAX_WAIT_TICKS;
    } else if (delta_ticks < PUSH_TICKS) {
      // Could be a double click. Wait.
      *timeout = DOUBLE_CLICK_TICKS - delta_ticks;
    } else {
      // Long enough "down" -> push. Indeed this code shouldn't be reached!
#if 0
      dbg_print_str("push\r\n");
#endif
      if (handler != NULL)
        handler(BPK_PUSH);

      state->down_time = 0;
      *timeout = MAX_WAIT_TICKS;
    }

    state->is_down = 0;
  }
}
