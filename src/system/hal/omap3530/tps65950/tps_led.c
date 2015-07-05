/*
 * tps_led.c
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#include "tps_led.h"

void tps_led_setup(void) {

	// enable PWM
	i2c_write(I2C1, SCD_LED, VIBRA_CTL, 0);
	i2c_write(I2C1, SCD_LED, LED_EN, BV(0) | BV(5));

}
