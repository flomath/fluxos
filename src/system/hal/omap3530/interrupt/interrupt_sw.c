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
			//TODO: wait till everything in buffer was written
			break;
		default:
			break;
	}

	atom_end();
}

static void sys_print(char* message, unsigned int length)
{
	uart_driver_write(message, length);
//	int i = 0;
//	char buffer[8];
//	while (i < length) {
//		int bufferCount = i % 8;
//		buffer[bufferCount] = message[i++];
//
//		if ( (bufferCount == 0 && i != 1) || length - i == 0) {
//			uart_driver_write(buffer, bufferCount > 0 ? bufferCount : 8);
//		}
//	}
}

static void sys_read()
{
	/*int count = 0;

	char buffer[8];
	while (count < length) {
		//int count = uart_driver_count();
		if ( count > 0 ) {
			//uart_driver_read(buffer, 8);

			uart_driver_write(buffer, count < 8 ? count : 8);
		}
	}*/
	return;
}
