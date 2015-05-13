
#include "gpio.h"

void gpio_enable_input(uint32_t port, uint8_t pin)
{
  // enable clock
  clock_enable_gpio(port);

  // enable GPIO to input
  hal_bitmask_set(port, GPIO_OE, BV(pin));

  // enable module
  hal_bitmask_clear(port, GPIO_CTRL, BV(0));
}

void gpio_enable_output(uint32_t port, uint8_t pin)
{
  // enable clock
  clock_enable_gpio(port);

  // enable GPIO to input
  hal_bitmask_clear(port, GPIO_OE, BV(pin));

  // enable module
  hal_bitmask_clear(port, GPIO_CTRL, BV(0));
}

void gpio_disable(uint32_t port)
{
  hal_bitmask_set(port, GPIO_CTRL, BV(0));
}

void gpio_reset(uint32_t port)
{
  // software reset [24-11]
  halt_bitmask_set(port, GPIO_SYSCONFIG, BV(1));

  // wait for reset
  while(hal_get_address_value(port, GPIO_SYSSTATUS) & BV(0) != 1) {}
}

uint32_t gpio_read(uint32_t port, uint8_t pin)
{
  if(hal_get_address_value(port, GPIO_OE, pin) == 1)
    return hal_get_address_value(port, GPIO_DATAIN, pin);
}

void gpio_write(uint32_t port, uint8_t pin)
{
  if(hal_get_address_value(port, GPIO_OE, pin) == 0)
    // TODO: set pin value
    //hal_bitmask_set(port, GPIO_DATAOUT)
}
