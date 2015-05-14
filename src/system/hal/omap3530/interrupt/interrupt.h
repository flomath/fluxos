/*
 * interrupt.h
 *
 *  Created on: 25.03.2015
 *      Author: Philip
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_INTERRUPT_INTERRUPT_H_
#define SRC_SYSTEM_HAL_OMAP3530_INTERRUPT_INTERRUPT_H_

#include "../../common/interrupt.h"
#include "../../../scheduler/process.h"

#define MPU_INTC					0x48200000				//< The MPU Instance [10-5]
#define MPU_INTC_INTCPS_SYSCONFIG	0x010					//< This register controls various parameters of the module interface [10-10]
#define MPU_INTC_INTCPS_IDLE		0x050					//< This register controls the functional clock auto-idle and the synchronizer clock auto-gating [10-22]
#define MPU_INTC_INTCPS_ILR(m)		(0x100 + (0x4 * (m)))		//< These registers contain the priority for the interrupts and the FIQ/IRQ steering [10-46] m = 0...95
#define MPU_INTC_INTCPS_MIR(n)		(0x084 + (0x20 * (n)))	//< This register contains the interrupt mask [10-32] n = 0...2
#define MPU_INTC_INTCPS_CONTROL		0x048					//< This register contains the new interrupt agreement bits
#define MPU_INTC_INTCPS_SIR_IRQ		0x040					//<	This register supplies the currently active IRQ interrupt number [10-14]

#define IRQ_NUMBER	96

/**
 * Temporary Current Registers
 *
 * This variable will be used for saving the context, when the IRQ Handler is processing.
 */
Registers_t __context_current;

/**
 * Temporarly save the registers in the variable
 */
extern void __context_tmp_save(Registers_t* registers);

#endif /* SRC_SYSTEM_HAL_OMAP3530_INTERRUPT_INTERRUPT_H_ */
