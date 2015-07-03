/*
 * i2c.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "i2c.h"
#include "../../common/hal.h"
#include "../clock/clock.h"

#define TIMEOUT 1000

void i2c1_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, BV(EN_I2C1));
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, BV(EN_I2C1));
}

void i2c2_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, BV(EN_I2C2));
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, BV(EN_I2C2));
}

void i2c3_enable(void) {
	hal_bitmask_set(CM_ICLKEN1_CORE, 0, BV(EN_I2C3));
	hal_bitmask_set(CM_FCLKEN1_CORE, 0, BV(EN_I2C3));
}


void i2c_init(uint32_t i2c) {
	// 18.5.1.1.1
	hal_bitmask_write(i2c, I2C_PSC, 0x7, 8);
	hal_bitmask_write(i2c, I2C_SCLL, 0x0d, 16);
	hal_bitmask_write(i2c, I2C_SCLH, 0x0f, 16);

	hal_bitmask_set(i2c, I2C_CON, BV(15));
}

void i2c_read(uint32_t i2c, uint8_t slave_address, uint8_t addr, uint8_t *buffer, size_t count) {
	if (i2c_wait(i2c)) {
		// Send address with no stop
		i2c_execute(i2c, 0x8601, slave_address, &addr, 1);

		// Send rest with stop
		i2c_execute(i2c, 0x8403, slave_address, buffer, count);
	}
}

void i2c_write8(uint32_t i2c, uint8_t slave_address, uint8_t address, uint8_t data) {
	uint8_t buffer[2];
	buffer[0] = address;
	buffer[1] = data;

	if (i2c_wait(i2c)) {
		i2c_write(i2c, slave_address, buffer, 2);
	}
}

void i2c_write(uint32_t i2c, uint8_t slave_address, uint8_t* data, size_t length) {
	if (i2c_wait(i2c)) {
		i2c_execute(i2c, I2C_CON1, slave_address, data, length);
	}
}

static void i2c_execute(uint32_t i2c, uint16_t con, uint8_t slave_address, uint8_t* data, size_t length) {
	int timeout;
	uint16_t st;
	int i = 0;
	bool_t redo = TRUE;

	do {
		hal_bitmask_write(i2c, I2C_SA, slave_address, 16);
		hal_bitmask_write(i2c, I2C_CNT, length, 16);
		hal_bitmask_write(i2c, I2C_CON, con, 16);

	    timeout = TIMEOUT * 10;

	    while (i < length) {
	    	st = hal_get_address_value(i2c, I2C_STAT);

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
	    		redo = FALSE;

	    	} else if (st & I2C_STAT_XRDY) {
	    		// Ready to send
	    		hal_bitmask_write(i2c, I2C_DATA, data[i++], 16);
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_XRDY, 16);
	    		redo = FALSE;

	    	} else if (st & I2C_STAT_RRDY) {
	    		// Ready to receive
	    		data[i++] = hal_get_address_value(i2c, I2C_DATA);
	    		hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_RRDY, 16);
	    		redo = FALSE;

	    	} else if (timeout-- == 0) {
	    		printf("Timeout!");
	    		return;
	    	}
	    }
	} while (redo);

	// Is the transfer completed?
	timeout = TIMEOUT;
	while ((hal_get_address_value(i2c, I2C_STAT) & I2C_STAT_ARDY) == 0) {
		if (!timeout--) {
			return;
	    }
	}

	hal_bitmask_write(i2c, I2C_STAT, I2C_STAT_ARDY, 16);
}

static bool_t i2c_wait(uint32_t i2c) {
	int timeout = TIMEOUT * 10;

	while (hal_get_address_value(i2c, I2C_STAT) & I2C_STAT_BB) {
		if (!timeout--) {
			return FALSE;
	    }
	}
	return TRUE;
}
