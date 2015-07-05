/*
 * McbspDriver.c
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#include "McbspDriver.h"

void mcsbp_driver_init()
{
	mcbsp2_enable();
	mcbsp_init_master2(MCBSP2);
	tps_led_setup();
	tps_audio_setup();
}

void mcbsp_driver_play_left(uint32_t data)
{
	// Send signal on left channel
	while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, data, 32);
}

void mcbsp_driver_play_right(uint32_t data)
{
	// Send signal on right channel
	while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
	hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, data, 32);
}

