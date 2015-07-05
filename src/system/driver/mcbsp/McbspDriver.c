/*
 * McbspDriver.c
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#include "McbspDriver.h"

void mcbsp_driver_play_left(uint32_t data)
{
	// Send signal on both channels
	while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, data, 32);

	//while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	//hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 2 ? data : 0, 32);
}

void mcbsp_driver_play_right(uint32_t data)
{
	// Send signal on both channels
	//while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	//hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 1 ? data : 0, 32);

	while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, data, 32);
}
