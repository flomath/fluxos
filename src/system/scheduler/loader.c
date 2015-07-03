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

void loader_load_process(uint32_t* address, size_t size) {
	//TODO: check size of process, to check if too big?

	atom_begin();

	// get current process to switch afterwards back
	PCB_t* currentProcess = scheduler_getCurrentProcess();

	// add process to get process space
	// code start + offset
	PCB_t* process = scheduler_addProcess((ProcFunc)(CODE_START + CODE_MAIN_OFFSET));

	// switch to new process
	mmu_switch_process(process);

	// load process into process space
	memcpy((uint32_t*)CODE_START, address, size);

	// switch back to old process
	mmu_switch_process(currentProcess);

	atom_end();
}
