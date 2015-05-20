/*
 * GPIODriver.c
 *
 *  Created on: 14.05.2015
 *      Author: florian
 */

#include "GPIODriver.h"
#include "../../hal/omap3530/clock/clock.h"
#include "../../hal/omap3530/gpio/gpio.h"

#define LED1	0x002
#define LED2	0x004

void gpio_driver_init()
{
	// enable clock
	clock_enable_gpio(GPIO4);

	// beagleboard LED
	gpio_enable_output(GPIO4, LED1);

	gpio_write(GPIO4, LED1, 0);
}
