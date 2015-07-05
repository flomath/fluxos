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
#include "../../../driver/mcbsp/McbspDriver.h"

static void sys_print(char* message, uint32_t length);
static void sys_read(uint32_t* c);
static void sys_load_proc(uint32_t* address, size_t size);

void handle_interrupt_sw(uint32_t swiID, uint32_t params[], uint32_t paramLength)
{
	atom_begin();

	PCB_t* process;

	switch(swiID)
	{
		// Helper/Debug methods
		case SYS_DEBUG:
			printf("debug swi called\n");
			break;
		case SYS_PRINT:
			sys_print((char*) params, paramLength);
			break;
		case SYS_READ:
			sys_read((uint32_t*) params);
			break;

		// Process Management
		case SYS_EXIT:
			// get current process for id
			// and kill it
			process = scheduler_getCurrentProcess();
			if (process != NULL) {
				scheduler_killProcess(process->processID);
			}
			break;
		case SYS_LOAD_PROC:
			if (paramLength == 2) {
				sys_load_proc((uint32_t*)params[0], (size_t)params[1]);
			}
			break;
		case SYS_START_PROC:
			scheduler_addProcess((ProcFunc)params);
			break;

		// Audio Driver
		case SYS_AUDIO_PLAYL:
			mcbsp_driver_play_left((uint32_t)params);
			break;
		case SYS_AUDIO_PLAYR:
			mcbsp_driver_play_right((uint32_t)params);
			break;

		default:
			break;
	}

	atom_end();
}

static void sys_print(char* message, uint32_t length)
{
	uart_driver_write(message, length);
}

static void sys_read(uint32_t* c)
{
	// read only one char
	int count = uart_driver_count();
	if (count > 0) {
		char buffer[1];
		uart_driver_read(buffer, 1);
		uart_driver_write(buffer, 1);
		*c = buffer[0];
	} else {
		*c = '\0';
	}
}

static void sys_load_proc(uint32_t* address, size_t size)
{
	loader_load_process(address, size);
}