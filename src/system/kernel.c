/*
 * kernel.c
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#include "kernel.h"

//interrupt_callback timer_irq;

void kernel_boot()
{
	// initialise MMU
	mmu_init();

	// Set up interrupts
	interrupt_init();

	// Console
	uart_driver_init(9600);
	scheduler_addProcess(console_init);

	// initialise onboard button
	button_driver_init();

	// initialise LED
	gpio_driver_init();

	// Enable sound
	mcsbp_driver_init();

	// Add IRQ handler
	interrupt_add_listener(40, &timer_irq);

	gpt_timer_init(GPT_TIMER4, 500);
	gpt_timer_start(GPT_TIMER4);

	syscall(SYS_PRINT, (uint32_t*)"system booted ...\r\n", 1);

	// lets sleep for a while
	util_ksleep(1000);

	// Enable interrupts globally
	interrupt_enable();


}

static void timer_irq(Registers_t* context)
{
	//button_driver_interrupt(context);
	scheduler_run(context);

	gpt_timer_reset(GPT_TIMER4);
	gpt_timer_start(GPT_TIMER4);
}
