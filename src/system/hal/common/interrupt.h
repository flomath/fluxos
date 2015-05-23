/*
 * interrupt.h
 *
 *  Created on: 25.03.2015
 *      Author: Philip
 */

#ifndef SRC_SYSTEM_HAL_COMMON_INTERRUPT_H_
#define SRC_SYSTEM_HAL_COMMON_INTERRUPT_H_

#include "interrupt_sw.h"

typedef void interrupt_callback(void);

/**
 * Initializes the IRQ environment.
 */
void interrupt_init();

/**
 * Enables the IRQ environment
 */
void interrupt_enable();

/**
 * Disables the IRQ environment
 */
void interrupt_disable();

/**
 * Registers a new listener to an IRQ
 *
 * @param irq The IQR to listen on
 * @param callback The callback function, which will be executed when an interrupt occurs
 */
void interrupt_add_listener(uint32_t irq, interrupt_callback* callback);

#endif /* SRC_SYSTEM_HAL_COMMON_INTERRUPT_H_ */
