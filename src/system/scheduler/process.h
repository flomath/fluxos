/*
 * process.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_SCHEDULER_PROCESS_H_
#define SRC_SYSTEM_SCHEDULER_PROCESS_H_

#include <stdint.h>
#include <stdlib.h>

typedef enum {
	PROCESS_CREATED = 0,
	PROCESS_READY,
	PROCESS_RUNNING,
	PROCESS_BLOCKED,
	PROCESS_TERMINATED
} ProcessState_t;

typedef struct {
	uint8_t id;
	ProcessState_t state;
	void (*task)(void); // ptr to task function
} Process_t;


/**
 * Get next unique process id
 */
uint8_t process_next_id();

/**
 * Create a new process
 */
Process_t* process_create();

#endif /* SRC_SYSTEM_SCHEDULER_PROCESS_H_ */
