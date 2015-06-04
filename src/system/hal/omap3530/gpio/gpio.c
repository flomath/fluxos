
#include "gpio.h"

void gpio_enable_input(uint32_t port, uint8_t pin)
{
  // enable clock
  //clock_enable_gpio(port);

  // enable GPIO to input
  hal_bitmask_set(port, GPIO_OE, BV(pin));

  // enable module
  //hal_bitmask_clear(port, GPIO_CTRL, BV(0));

  // Set detection
  // TODO: make it more configurable
  hal_bitmask_set(port, GPIO_FALLINGDETECT, BV(pin));

  // enable wakeup
  hal_bitmask_set(port, GPIO_WAKEUPENABLE, BV(pin));
  hal_bitmask_set(port, GPIO_SYSCONFIG, BV(2)); ///< ENWAKEUP
}

void gpio_enable_irq(uint32_t port, uint32_t irq, uint8_t pin)
{
  hal_bitmask_set(port, irq, BV(pin));
}

void gpio_enable_output(uint32_t port, uint8_t pin)
{
  // enable clock
  //clock_enable_gpio(port);

  // enable GPIO to input
  hal_bitmask_clear(port, GPIO_OE, BV(pin));

  // enable module
  hal_bitmask_clear(port, GPIO_CTRL, BV(0));

  // Set detection
  // TODO: make it more configurable
  hal_bitmask_set(port, GPIO_FALLINGDETECT, BV(pin));

  // enable wakeup
  hal_bitmask_set(port, GPIO_WAKEUPENABLE, BV(pin));
  hal_bitmask_set(port, GPIO_SYSCONFIG, BV(2)); ///< ENWAKEUP
}

void gpio_disable(uint32_t port)
{
  hal_bitmask_set(port, GPIO_CTRL, BV(0));
}

void gpio_reset(uint32_t port)
{
  // software reset [24-11]
  hal_bitmask_set(port, GPIO_SYSCONFIG, BV(1));

  // wait for reset
  while(hal_get_address_value(port, GPIO_SYSSTATUS) & BV(0) != 1) {}
}

uint32_t gpio_read(uint32_t port, uint8_t pin)
{
  if((hal_get_address_value(port, GPIO_OE) & BV(pin)) == 1)
    return (hal_get_address_value(port, GPIO_DATAIN) & BV(pin));

  return 0;
}

void gpio_write(uint32_t port, uint8_t pin, int32_t value)
{
	// check if output
	if((hal_get_address_value(port, GPIO_OE) & BV(pin)) == 1)
		return;

	hal_bitmask_write(port, GPIO_DATAOUT, value, 32);
}
