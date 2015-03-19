/*
 * hal.c
 *
 *  Created on: 19.03.2015
 *      Author: Nino Schoch
 */

#include "hal.h"

mmio_t hal_get_register(uint32_t port, uint8_t offset)
{
	return (mmio_t)(port + offset);
}


uint8_t hal_get_address_value(uint32_t port, uint8_t offset)
{
	mmio_t reg = hal_get_register(port, offset);
	return *reg;
}


uint8_t hal_bitmask_write(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = hal_get_register(port, offset);
	uint8_t value = *reg;
	*reg = mask;

	return value;
}


uint8_t hal_bitmask_set(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = hal_get_register(port, offset);
	uint8_t value = *reg;
	*reg |= mask;

	return value;
}


uint8_t hal_bitmask_clear(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = hal_get_register(port, offset);
	uint8_t value = *reg;
	*reg &= ~mask;

	return value;
}
