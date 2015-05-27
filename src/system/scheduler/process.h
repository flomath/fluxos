/*
 * process.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_SCHEDULER_PROCESS_H_
#define SRC_SYSTEM_SCHEDULER_PROCESS_H_

#include <stdint.h>

typedef enum {
	PROCESS_CREATED = 0,
	PROCESS_READY,
	PROCESS_RUNNING,
	PROCESS_BLOCKED,
	PROCESS_TERMINATED
} ProcessState_t;

/**
 * Registers for OMAP3530
 */
typedef struct {
	uint32_t R0;	//< R0
	uint32_t R1;	//< R1
	uint32_t R2;	//< R2
	uint32_t R3;	//< R3
	uint32_t R4;	//< R4
	uint32_t R5;	//< R5
	uint32_t R6;	//< R6
	uint32_t R7;	//< R7
	uint32_t R8;	//< R8
	uint32_t R9;	//< R9
	uint32_t R10;	//< R10
	uint32_t R11;	//< R11
	uint32_t R12;	//< R12
	uint32_t SP; 	//< R13, Stack Pointer
	uint32_t LR; 	//< R14, Link Register
	uint32_t PC;	//< Program Counter
	uint32_t CPSR;	//< Mode
} Registers_t;

typedef struct {
	uint32_t size;
	uint32_t processID;
	ProcessState_t state;
	Registers_t registers;
	uint32_t* pageTable;
	void (*func)(void);
} PCB_t;

#endif /* SRC_SYSTEM_SCHEDULER_PROCESS_H_ */
