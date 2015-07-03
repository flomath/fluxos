/*
 * semaphore.c
 *
 *  Created on: 03.07.2015
 *      Author: Philip
 */

#include "semaphore.h"
#include "../scheduler/scheduler.h"

int sem_wait(sem_t* sem) {
	atom_begin();

	sem--;
	if (sem < 0) {
		int process = scheduler_getCurrentProcessId();
		sem_enqueue(sem, process);

		scheduler_suspend(process);

		// Todo: Schedule away
	}

	atom_end();

	return 0;
}

int sem_post(sem_t* sem) {
	atom_begin();

	sem++;
	if (sem <= 0) {
		int process = sem_dequeue(sem);

		scheduler_continue(process);
	}

	atom_end();

	return 0;
}

void sem_enqueue(sem_t* sem, int pid) {
	node_t* node = (node_t*)malloc(sizeof(node_t));
	node->pid = pid;
	node->next = NULL;

	if (sem->queue == NULL) {
		// Empty queue
		sem->queue = node;
		return;
	}

	// Enqueue to the very last node element
	node_t* current = sem->queue;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = node;
}

int sem_dequeue(sem_t* sem) {
	if(sem->queue == NULL) {
		return -1;
	}

	node_t* node = sem->queue;
	sem->queue = sem->queue->next;

	int pid = node->pid;
	free(node);

	return pid;
}
