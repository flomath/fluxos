/*
 * interrupt_sw.c
 *
 *  Created on: 23.05.2015
 *      Author: florian
 */

#include "interrupt_sw.h"
#include "../../../scheduler/scheduler.h"
#include "../../../driver/uart/UartDriver.h"

static void sys_print(char* message, unsigned int length);

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
		default:
			break;
	}

	atom_end();
}

static void sys_print(char* message, unsigned int length)
{
	uart_driver_write(message, length);
}

static void sys_read()
{
	//TODO
	return;
}
