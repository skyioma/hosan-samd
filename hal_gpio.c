#include "hal_gpio.h"

#include <asf.h>

void hal_gpio_init()
{
  struct port_config pin_conf;

  port_get_config_defaults(&pin_conf);

  pin_conf.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_output_level(E_CS_PIN, !E_CS_ACTIVE);
  port_pin_set_config(E_CS_PIN, &pin_conf);
  port_pin_set_config(E_DC_PIN, &pin_conf);
  port_pin_set_output_level(E_RST_PIN, !E_RST_ACTIVE);
  port_pin_set_config(E_RST_PIN, &pin_conf);

  port_pin_set_output_level(EN_33VA_PIN, !EN_33VA_ACTIVE);
  port_pin_set_config(EN_33VA_PIN, &pin_conf);

  pin_conf.direction = PORT_PIN_DIR_INPUT;
  port_pin_set_config(E_BUSY_PIN, &pin_conf);
}

void hal_gpio_set(uint8_t pin, uint8_t value)
{
  port_pin_set_output_level(pin, value);
}

uint8_t hal_gpio_get(uint8_t pin)
{
  return port_pin_get_input_level(pin);
}
