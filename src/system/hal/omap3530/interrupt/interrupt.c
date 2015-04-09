/*
 * interrupt.c
 *
 *  Created on: 25.03.2015
 *      Author: Philip
 */

#include <stdio.h>
#include "../../common/hal.h"
#include "interrupt.h"


#include "../timer/timer.h"

static interrupt_callback* irq_callbacks[IRQ_NUMBER] = { NULL };

void interrupt_init(void) {

	// 1. Program the MPU_INTC.INTCPS_SYSCONFIG register:
	// If necessary, enable the interface clock autogating by setting the AUTOIDLE bit

	hal_bitmask_set(MPU_INTC, MPU_INTC_INTCPS_SYSCONFIG, BV(1)); 	// Softreset
	hal_bitmask_clear(MPU_INTC, MPU_INTC_INTCPS_SYSCONFIG, BV(0)); 	// No AUTOIDLE

	// 2. Program the MPU_INTC.INTCPS_IDLE register:
	// If necessary, disable functional clock autogating
	// or enable synchronizer autogating by setting the FUNCIDLE bit or TURBO bit accordingly

	hal_bitmask_clear(MPU_INTC, MPU_INTC_INTCPS_IDLE, BV(1) + BV(0)); // free running syncrhonizer clock (default) and functional clock gating strategy is applied (default)

	// 3. Program the MPU_INTC.INTCPS_ILRm register for each interrupt line:
	// Assign a priority level and set the FIQNFIQ bit for an FIQ interrupt
	// (by default, interrupts are mapped to IRQ and priority is 0x0 [highest]).

	// Use default values

	// 4. Program the MPU_INTC.INTCPS_MIRn register:
	// Enable interrupts (by default, all interrupt lines are masked).

	// Unmask
	//hal_bitmask_clear(MPU_INTC, MPU_INTC_INTCPS_MIR(0), BV(29)); // Enable GPIO1
	//hal_bitmask_clear(MPU_INTC, MPU_INTC_INTCPS_MIR(1), BV(14)); // Enable GPT10
}

void interrupt_enable(void) {
	_enable_interrupts();
	_enable_IRQ();
}

void interrupt_disable(void) {
	_disable_IRQ();
	_disable_interrupts();
}

void interrupt_add_listener(uint32_t irq, interrupt_callback* listener) {
	irq_callbacks[irq] = listener;

	// Enable IRQ
	hal_bitmask_clear(MPU_INTC, MPU_INTC_INTCPS_MIR((uint8_t)irq / 32), BV(irq % 32));
}

#pragma INTERRUPT(dabt_handler, DABT)
interrupt void dabt_handler(void) {
	// Data abort exception. Read LR ans subtract 8 bits for getting the MemoryAddress of the cause
	printf("Data Abort Exception!\n");
}

#pragma INTERRUPT(fiq_handler, FIQ)
interrupt void fiq_handler(void) {
	printf("Not implemented: FIQ!\n");
}


#pragma INTERRUPT(irq_handler, IRQ)
void irq_handler(void) {
	interrupt_disable();

	// Get the rightmost 6 bits: Active IRQ
	mmio_t address = hal_get_register(MPU_INTC, MPU_INTC_INTCPS_SIR_IRQ);
	uint8_t irq = BIT_TRIM_LEFT(*address, 7);
	printf("Interrupt: %d\n", irq);

	// Call the callback
	if ( irq_callbacks[irq] != NULL ) {
		irq_callbacks[irq]();
	}

	// Clear the IRQ
	*((mmio_t)(MPU_INTC + MPU_INTC_INTCPS_CONTROL)) |= 0x01;
}

#pragma INTERRUPT(pabt_handler, PABT)
interrupt void pabt_handler(void) {
	printf("Not implemented: PABT\n");
}

#pragma INTERRUPT(swi_handler, SWI)
interrupt void swi_handler(void) {
	printf("Not implemented: SWI\n");
}

#pragma INTERRUPT(udef_handler, UDEF)
interrupt void udef_handler(void) {
	printf("Not implemented: UDEF\n");
}
