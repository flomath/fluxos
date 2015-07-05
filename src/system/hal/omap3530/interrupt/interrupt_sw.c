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
#include "../../../driver/mcbsp/McbspDriver.h"
#include "../../../filesystem/ff.h"
#include <string.h>

static void sys_print(char* message);
static void sys_read(uint32_t* c);
static void sys_load_proc(uint32_t* address, size_t size);
static void sys_read_file(char* fileName, char* buffer, uint32_t* bufferSize);

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
			sys_print((char*) params);
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

		// Filesystem
		case SYS_READ_FILE:
			sys_read_file((char*)params[0], (char*)params[1], (uint32_t*)params[2]);
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

static void sys_read_file(char* fileName, char* buffer, uint32_t* bufferSize)
{
	//TODO: actually using file with path

	// Read binary file at once
	FATFS fs;
	fs.drv = 0;

	//TODO: check if we have to close mounted one?
	f_mount(&fs, "", 1);

	FILINFO fno;

	DIR dp;
	//TODO: add RESULT
	f_opendir(&dp, "/\0");
	f_readdir(&dp, &fno);
	FIL file;
	//TODO: only on root path!
	f_open(&file, fileName, FA_READ);

	unsigned ptr;

	ptr = f_tell(&file); // get read-pointer
	f_read(&file, buffer, (uint32_t)bufferSize, &ptr); // read 4 byte
	*bufferSize = ptr;
	uart_driver_write(buffer, strlen(buffer));
	ptr = f_eof(&file); // check if eof is reached - ptr != 0, if it is so go back
	f_lseek(&file, 0); // set read-pointer to the beginning of the file
	ptr = f_tell(&file);

	f_close(&file);
	f_closedir(&dp);
}
