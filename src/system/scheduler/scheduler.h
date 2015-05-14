/*
 * scheduler.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_
#define SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_

#include "process.h"
#include <stdlib.h>
#include "../../tools/mutex.h"

#define		SCHEDULER_MAX_PROCESSES 64
#define		SCHEDULER_INVALID_ID	-1

typedef void (*ProcFunc)();

/**
 * Load the context
 */
extern void __context_load(PCB_t* context_address);

/**
 * Add a new process to the scheduler
 */
void scheduler_addProcess(ProcFunc fct);

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

#endif /* SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_ */
