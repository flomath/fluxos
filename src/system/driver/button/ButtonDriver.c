/*
 * ButtonDriver.c
 *
 *  Created on: 14.05.2015
 *      Author: florian
 */

#include "ButtonDriver.h"
#include "../../hal/omap3530/clock/clock.h"
#include "../../hal/omap3530/gpio/gpio.h"
#include "../../hal/omap3530/interrupt/interrupt.h"


#define BUTTON_DRIVER_IRQ 	29
#define BUTTON_GPIO_PIN		7

void button_driver_init()
{
	// enable clock
	clock_enable_gpio(GPIO1);

	// beagleboard user button (see beagleboard system ref on p. 64)
	gpio_enable_input(GPIO1, BUTTON_GPIO_PIN);

	// enable irq for gpio pin 7
	gpio_enable_irq(GPIO1, GPIO_IRQENABLE1, BUTTON_GPIO_PIN);

	// Enable interrupts
	interrupt_add_listener(BUTTON_DRIVER_IRQ, &button_driver_interrupt);
}

void button_driver_interrupt()
{
	printf("button pressed\n");

	hal_bitmask_clear(GPIO1, GPIO_WAKEUPENABLE, BV(BUTTON_GPIO_PIN));
	hal_bitmask_set(GPIO1, GPIO_IRQSTATUS1, BV(BUTTON_GPIO_PIN));
}
