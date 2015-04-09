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
 * Return the right-most N bits, meaning it trims the left-most 1-N bits
 */
#define BIT_TRIM_LEFT(value, N)	(value & ((1 << N) - 1))

/**
 * Return the left-most 1-N bits, meaning it trims the right-most N bits
 */
#define BIT_TRIM_RIGHT(value, N) (value >> N)

/**
 * Return the bit range
 */
#define BIT_RANGE(value, upper, lower) ((value >> lower) & ~(~0 << (upper - lower + 1)))

/**
 * Memory Mapped IO address
 */
typedef volatile uint32_t* mmio_t;

/**
 * Write a bitmask into address
 *
 * @param port		The register base address
 * @param offset	The offset of the register
 * @param mask		The value to write
 * @param size		The size of the register
 */
uint32_t hal_bitmask_write(uint32_t port, uint8_t offset, uint32_t mask, uint8_t size);

/**
 * Enable a bit(mask) of given port and offset
 */
void hal_bitmask_set(uint32_t port, uint8_t offset, uint32_t mask);

/**
 * Clear a bit(mask) of given port and offset
 */
void hal_bitmask_clear(uint32_t port, uint8_t offset, uint32_t mask);

/**
 * Get value of given address
 */
uint32_t hal_get_address_value(uint32_t port, uint8_t offset);

/**
 * Returns the register address of given port and offset
 *
 * @internal
 */
inline mmio_t hal_get_register(uint32_t port, uint8_t offset);

#endif /* SRC_SYSTEM_HAL_HAL_H_ */
