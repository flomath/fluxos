/*
 * hal.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_HAL_H_
#define SRC_SYSTEM_HAL_HAL_H_

#include <stdint.h>

/**
 * Bitshift
 */
#define	BV(x) (0x01 << x)

/**
 * Memory Mapped IO address
 */
typedef volatile uint32_t* mmio_t;

/**
 * Write a bitmask into address
 */
uint8_t hal_bitmask_write(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Enable a bit(mask) of given port and offset
 */
uint8_t hal_bitmask_set(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Clear a bit(mask) of given port and offset
 */
uint8_t hal_bitmask_clear(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Get value of given address
 */
uint8_t hal_get_address_value(uint32_t port, uint8_t offset);

/**
 * Returns the register address of given port and offset
 *
 * @internal
 */
mmio_t hal_get_register(uint32_t port, uint8_t offset);

#endif /* SRC_SYSTEM_HAL_HAL_H_ */
