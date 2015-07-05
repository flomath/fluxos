/*
 * tps_led.c
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#include "tps_led.h"

void tps_led_setup(void) {
	uint8_t buffer[5] = {0};
	i2c_read(I2C1, SCD_LED, LED_EN, buffer, 5);

	int i;
	for (i=0;i<5;i++) {
		printf("Before: %d\n", buffer[i]);
	}

	// enable PWM
	i2c_write(I2C1, SCD_LED, VIBRA_CTL, 0);
	i2c_write(I2C1, SCD_LED, LED_EN, BV(0) | BV(5));

	i2c_read(I2C1, SCD_LED, LED_EN, buffer, 5);

	for (i=0;i<5;i++) {
		printf("After: %d\n", buffer[i]);
	}
}
