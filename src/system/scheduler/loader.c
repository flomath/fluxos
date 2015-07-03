/*
 * loader.c
 *
 *  Created on: 10.06.2015
 *      Author: Philip
 */


#include "loader.h"
#include "process.h"
#include "scheduler.h"
#include "../hal/omap3530/mmu/mmu.h"
#include "../system.h"

static uint32_t* proc_buffy;
static unsigned int proc_size;

static void start_process()
{
	printf("Test: %i", proc_size);
	uint32_t* proc_buffy_tmp = proc_buffy;
	unsigned int proc_size_tmp = proc_size;

	// load process into process space
	memcpy((uint32_t*)CODE_START, proc_buffy_tmp, proc_size_tmp);

	// get current process to switch afterwards back
	PCB_t* currentProcess = scheduler_getCurrentProcess();
	currentProcess->newPC = (uint32_t)(CODE_START + CODE_MAIN_OFFSET) + 4;

	//magic while
	while(1) {}
}

void loader_load_process(uint32_t* address, size_t size) {
	//TODO: check size of process, to check if too big?

	atom_begin();

	// add process to get process space
	// code start + offset
	proc_buffy = address;
	proc_size = size;
	scheduler_addProcess((ProcFunc)(&start_process));

	atom_end();
}
