/*
 * i2c.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "i2c.h"
#include "../../common/hal.h"
#include "../clock/clock.h"

void i2c1_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, EN_I2C1);
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, EN_I2C1);
}

void i2c2_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, EN_I2C2);
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, EN_I2C2);
}

void i2c3_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, EN_I2C3);
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, EN_I2C3);
}


void i2c_init(uint32_t i2c) {
	// Sets I2C to 100Khz F/S mode
	hal_bitmask_write(i2c, I2C_PSC, 0x17, 16);
	hal_bitmask_write(i2c, I2C_SCLL, 0x0d, 16);
	hal_bitmask_write(i2c, I2C_SCLH, 0x0f, 16);
}
