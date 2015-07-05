/*
 * McbspDriver.h
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_DRIVER_MCBSP_MCBSPDRIVER_H_
#define SRC_SYSTEM_DRIVER_MCBSP_MCBSPDRIVER_H_

#include "../../system.h"
#include "../../hal/omap3530/mcbsp/mcbsp.h"

void mcbsp_driver_play_left(uint32_t data);
void mcbsp_driver_play_right(uint32_t data);

#endif /* SRC_SYSTEM_DRIVER_MCBSP_MCBSPDRIVER_H_ */
