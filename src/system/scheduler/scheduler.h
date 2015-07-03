/*
 * scheduler.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_
#define SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_

#include "process.h"
#include "../hal/omap3530/mmu/mmu.h"
#include <stdlib.h>
#include "../../tools/mutex.h"

#define		SCHEDULER_MAX_PROCESSES 8
#define		SCHEDULER_INVALID_ID	-1

typedef void (*ProcFunc)();

/**
 * Add a new process to the scheduler
 */
PCB_t* scheduler_addProcess(ProcFunc fct);

/**
 * Get a free process id slot
 */
int scheduler_getFreeProcessID();

/**
 * Get the next process by a scheduling algorithm
 */
int scheduler_getNextProcess();

/**
 * Run the scheduler
 */
void scheduler_run();

/**
 * Kill a process and release its resources
 */
void scheduler_killProcess(int processID);

/**
 * Get current process
 */
PCB_t* scheduler_getCurrentProcess();

/**
 * Disable scheduling
 */
void atom_begin(void);

/**
 * Enable scheduling
 */
void atom_end(void);

#endif /* SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_ */
