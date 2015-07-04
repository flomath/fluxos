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
#include "../../../ipc/semaphore.h"
#include <string.h>

static void sys_print(char* message);
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
			sys_print((char*) params);
			break;
		case SYS_LOAD_PROC:
			if (paramLength == 2) {
				sys_load_proc((uint32_t*)params[0], (size_t)params[1]);
			}
			break;
		default:
			break;
	}

	atom_end();
}

static void sys_print(char* message)
{
	uart_driver_write(message, strlen(message));
}

static void sys_load_proc(uint32_t* address, size_t size)
{
	loader_load_process(address, size);
}
