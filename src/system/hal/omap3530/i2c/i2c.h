/*
 * i2c.h
 *
 *  Created on: 10.07.2015
 *      Author: philip
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_I2C_I2C_H_
#define SRC_SYSTEM_HAL_OMAP3530_I2C_I2C_H_

#include "../../../system.h"

/*
 * BeagleBoard System Reference 8.4.3 page 49
 *
 * OMAP3530 I2C Control
 * The various components in the TPS65950 are controlled from the OMAP3530 via the
 * I2C interface. I2C_0 is used to control the TPS65950 device.
 *
 * page: 50
 * I2C interface seems to be I2C4
 */

#define EN_I2C1 15		///< Bit of CM_ICLKEN1_CORE and CM_FCLKEN1_CORE to enable I2C1
#define EN_I2C2 16		///< Bit of CM_ICLKEN1_CORE and CM_FCLKEN1_CORE to enable I2C2
#define EN_I2C3 17		///< Bit of CM_ICLKEN1_CORE and CM_FCLKEN1_CORE to enable I2C3

#define I2C1 0x48070000 ///< I2C1 Device Address
#define I2C2 0x48072000 ///< I2C2 Device Address
#define I2C3 0x48060000 ///< I2C3 Device Address

#define I2C_PSC		0x30	///< This register is used to specify the internal clocking of the I2C peripheral core. The core logic is sampled at the clock rate of the functional clock for the module divided by (PSC+1). [18-40]
#define I2C_SCLL	0x34	///< This register is used to determine the SCL low time value when master. [18-42]
#define I2C_SCLH	0x38	///< This register is used to determine the SCL low time value when master. [18-44]

#define I2C_STAT    0x08
#define I2C_CNT     0x18
#define I2C_DATA    0x1C
#define I2C_CON     0x24
#define I2C_SA      0x2C

#define I2C_STAT_XDR 	BV(14)
#define I2C_STAT_RDR 	BV(13)
#define I2C_STAT_BB 	BV(12)
#define I2C_STAT_XRDY 	BV(4)
#define I2C_STAT_RRDY 	BV(3)
#define I2C_STAT_ARDY 	BV(2)
#define I2C_STAT_NACK 	BV(1)
#define I2C_STAT_AL 	BV(0)
#define I2C_STAT_BB		BV(12)

#define I2C_CON1	0x8603
#define I2C_CON2	0x8403

/**
 * Enable the I²C device 1
 */
void i2c1_enable(void);

/**
 * Enable the I²C device 2
 */
void i2c2_enable(void);

/**
 * Enable the I²C device 3
 */
void i2c3_enable(void);

/**
 * Initialize the I²C device
 * @param i2c The I²C device to initialize
 */
void i2c_init(uint32_t i2c);

/**
 * Write the 8 bit data to the i2c device
 * @param i2c The I2C module
 * @param slave_address The slave address
 * @param data The data to write
 * @param address
 */
void i2c_write8(uint32_t i2c, uint8_t slave_address, uint8_t address, uint8_t data);

/**
 * Write the data to the i2c device
 * @param i2c The I2C module
 * @param slave_address The slave address
 * @param data The data to write
 * @param length The length of data
 */
void i2c_write(uint32_t i2c, uint8_t slave_address, uint8_t* data, size_t length);

/**
 * Sends the data
 * @param i2c The I2C module
 * @param con
 * @param slave_address The slave address
 * @param data The data to write
 * @param length The length of data
 */
static void i2c_execute(uint32_t i2c, uint16_t con, uint8_t slave_address, uint8_t* data, size_t length);

/**
 * Wait until device is idle
 * @param i2c The I2C module
 */
static void i2c_wait(uint32_t i2c);

#endif
