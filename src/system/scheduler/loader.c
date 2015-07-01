/*
 * loader.c
 *
 *  Created on: 10.06.2015
 *      Author: Philip
 */


#include "loader.h"
#include "process.h"
#include "scheduler.h"
#include "../system.h"

void loader_load_process(uint32_t address, size_t size) {
	// Load process into memory

	// WHAT NEEDS TO BE DONE BEFORE STARTING A PROCESS?

	// Find process entry point
	uint32_t entry_point = address;

	// Start Process
	scheduler_addProcess((ProcFunc)entry_point);
}
