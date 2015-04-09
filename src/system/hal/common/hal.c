/*
 * hal.c
 *
 *  Created on: 19.03.2015
 *      Author: Nino Schoch
 */

#include "hal.h"

inline mmio_t hal_get_register(uint32_t port, uint8_t offset)
{
	return (mmio_t)(port + offset);
}


uint32_t hal_get_address_value(uint32_t port, uint8_t offset)
{
	mmio_t reg = hal_get_register(port, offset);
	return *reg;
}


uint32_t hal_bitmask_write(uint32_t port, uint8_t offset, uint32_t mask, uint8_t size)
{
	mmio_t reg = hal_get_register(port, offset);
	uint32_t value = *reg;

	switch (size) {
	case 32:
		*reg = mask;
		break;
	case 16:
		*reg = (uint16_t) mask;
		break;
	case 8:
		*reg = (uint8_t) mask;
		break;
	default:
		// Todo: Exception or something
		break;
	}
	return value;
}

void hal_bitmask_set(uint32_t port, uint8_t offset, uint32_t mask)
{
	mmio_t reg = hal_get_register(port, offset);
	*reg |= mask;
}


void hal_bitmask_clear(uint32_t port, uint8_t offset, uint32_t mask)
{
	mmio_t reg = hal_get_register(port, offset);
	*reg &= ~mask;
}
