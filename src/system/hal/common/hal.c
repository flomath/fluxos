/*
 * hal.c
 *
 *  Created on: 19.03.2015
 *      Author: Nino Schoch
 */

#include "hal.h"

uint32_t hal_get_address_value(uint32_t port, uint8_t offset)
{
	mmio_t reg = hal_get_register(port, offset);
	return *reg;
}

uint32_t hal_bitmask_write(uint32_t port, uint8_t offset, uint32_t mask, uint8_t size)
{
	if(size == 32) {
		mmio_t reg = hal_get_register(port, offset);
		uint32_t value = *reg;
		*reg = mask;
		return value;
	}

	if(size == 16) {
		volatile uint16_t* reg = (volatile uint16_t *)hal_get_register(port, offset);
		uint16_t value = *reg;
		*reg = mask;
		return value;
	}

	if(size == 8) {
		// TODO: note! not working properly!
		mmio_t reg = hal_get_register(port, offset);
		uint8_t value = *reg;
		*reg = mask;
		return value;
	}

	return 0;
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
