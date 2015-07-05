/*
 * tps_led.h
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS_LED_H_
#define SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS_LED_H_

#include "../../common/hal.h"
#include "../i2c/i2c.h"

#define SCD_LED				0x4a
#define VIBRA_CTL			0x00000045
#define LED_EN				0x000000EE

/**
 * init tps LED
 */
void tps_led_setup(void);

#endif /* SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS_LED_H_ */
