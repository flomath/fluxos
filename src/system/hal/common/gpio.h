/*
 * gpio.h
 *
 *  Created on: 13.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_GPIO_H_
#define SRC_SYSTEM_HAL_GPIO_H_

#include <stdint.h>

/**
 * enable input GPIO
 */
void gpio_enable_input(uint32_t port, uint8_t pin);

/**
 * enable GPIO interrupt
 */
void gpio_enable_irq(uint32_t port, uint32_t irq, uint8_t pin);

/*
 * enable output GPIO
 */
void gpio_enable_output(uint32_t port, uint8_t pin);

/*
 * disable GPIO
 */
void gpio_disable(uint32_t port);

/*
 * reset GPIO
 */
void gpio_reset(uint32_t port);

/*
 * read data from gpio input
 */
uint32_t gpio_read(uint32_t port, uint8_t pin);

/*
 * write to gpio output
 */
void gpio_write(uint32_t port, uint8_t pin, int32_t value);

#endif /* SRC_SYSTEM_HAL_GPIO_H_ */
