/*
 * interrupt_sw.c
 *
 *  Created on: 23.05.2015
 *      Author: florian
 */

#include "interrupt_sw.h"
#include "../../../scheduler/scheduler.h"
#include "../../../driver/uart/UartDriver.h"
#include "../../../scheduler/loader.h"

static void sys_print(char* message, unsigned int length);
static void sys_read(uint32_t* c);
static void sys_load_proc(uint32_t* address, size_t size);

void handle_interrupt_sw(uint32_t swiID, uint32_t params[], unsigned int paramLength)
{
	atom_begin();

	PCB_t* process;

	switch(swiID)
	{
		case SYS_DEBUG:
			printf("debug swi called\n");
			break;
		case SYS_EXIT:
			// get current process for id
			// and kill it
			process = scheduler_getCurrentProcess();
			if (process != NULL) {
				scheduler_killProcess(process->processID);
			}
			break;
		case SYS_PRINT:
			sys_print((char*) params, paramLength);
			break;
		case SYS_READ:
			sys_read((uint32_t*) params);
			break;
		case SYS_LOAD_PROC:
			if (paramLength == 2) {
				sys_load_proc((uint32_t*)params[0], (size_t)params[1]);
			}
		default:
			break;
	}

	atom_end();
}

static void sys_print(char* message, unsigned int length)
{
	uart_driver_write(message, length);
}

static void sys_read(uint32_t* c)
{
	// read only one char
	while (1) {
		int count = uart_driver_count();
		if (count > 0) {
			char buffer[1];
			uart_driver_read(buffer, 1);
			uart_driver_write(buffer, 1);
			*c = buffer[0];
			break;
		}
	}
}

static void sys_load_proc(uint32_t* address, size_t size)
{
	loader_load_process(address, size);
}
