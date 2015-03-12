/*
 * hal.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_HAL_H_
#define SRC_SYSTEM_HAL_HAL_H_

#define	BV(x)	(0x01 << x)	// generic bitshift

typedef volatile uint32_t* mmio_t; // Memory Mapped IO address

#endif /* SRC_SYSTEM_HAL_HAL_H_ */
