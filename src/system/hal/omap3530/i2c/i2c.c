/*
 * i2c.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "i2c.h"
#include "../../common/hal.h"
#include "../clock/clock.h"

#define I2C_RETRIES 100000

void i2c_read(uint32_t i2c, uint8_t slave_address, uint8_t addr, uint8_t *buffer, size_t length) {
	if (i2c_wait(i2c)) {
		// Send address with no stop
		i2c_send_cmd(i2c, 0x8601, slave_address, &addr, 1);

		// Send rest with stop
		i2c_send_cmd(i2c, 0x8403, slave_address, buffer, length);
	}
}

void i2c_write(uint32_t i2c, uint8_t slave_address, uint8_t address, uint8_t data) {
	uint8_t buffer[2];
	buffer[0] = address;
	buffer[1] = data;

	if (i2c_wait(i2c)) {
		i2c_send_cmd(i2c, I2C_CON1, slave_address, buffer, 2);
	}
}

static void i2c_send_cmd(uint32_t i2c, uint16_t con, uint8_t slave_address, uint8_t* data, size_t length) {
	int retries;
	int i = 0;

	bool_t loop = TRUE;
	while(loop) {
		retries = I2C_RETRIES;

		hal_bitmask_write(i2c, I2C_SA, slave_address, 16);
		hal_bitmask_write(i2c, I2C_CNT, length, 16);
		hal_bitmask_write(i2c, I2C_CON, con, 16);

		// Send all data
	    while (i < length) {
	    	uint16_t st = hal_get_address_value(i2c, I2C_STAT);

	    	if (st & I2C_STAT_NACK) {
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_NACK, 16);
	    		break;

	    	} else if (st & I2C_STAT_AL) {
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_AL, 16);
	    		break;

	    	} else if (st & I2C_STAT_ARDY) {
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_ARDY, 16);
	    		continue;

	    	} else if (st & I2C_STAT_RDR) {
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_XDR, 16);
	    		loop = FALSE;

	    	} else if (st & I2C_STAT_XRDY) {
	    		// Ready to send
	    		hal_bitmask_write(i2c, I2C_DATA, data[i++], 16);
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_XRDY, 16);
	    		loop = FALSE;

	    	} else if (st & I2C_STAT_RRDY) {
	    		// Ready to receive
	    		data[i++] = hal_get_address_value(i2c, I2C_DATA);
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_RRDY, 16);
	    		loop = FALSE;

	    	} else if (retries-- == 0) {
	    		return;
	    	}
	    }
	}

	// Did the transmission complete?
	retries = I2C_RETRIES;
	while ((hal_get_address_value(i2c, I2C_STAT) & I2C_STAT_ARDY) == 0) {
		if (!retries--) {
			return;
	    }
	}

	// Send ARDY
	hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_ARDY, 16);
}

static bool_t i2c_wait(uint32_t i2c) {
	int retries = I2C_RETRIES;

	while (hal_get_address_value(i2c, I2C_STAT) & I2C_STAT_BB) {
		if (!retries--) {
			return FALSE;
	    }
	}
	return TRUE;
}
