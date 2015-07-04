/*
 * semaphore.c
 *
 *  Created on: 03.07.2015
 *      Author: Philip
 */

#include "semaphore.h"
#include "../scheduler/scheduler.h"
#include <string.h>

static sem_node_t* sem_list;

sem_t* sem_create(const char* name) {
	sem_t* sem = (sem_t*) malloc(sizeof(sem_t));
	strncpy(sem->name, name, 16);
	sem->queue = NULL;
	sem->value = 0;

	sem_node_t* node = (sem_node_t*) malloc(sizeof(sem_node_t));

	node->sem = sem;
	node->next = NULL;

	if (sem_list == NULL) {
		// Empty list
		sem_list = node;
		return sem;
	}

	// Enqueue to the very last node element
	sem_node_t* current = sem_list;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = node;

	return sem;
}

void sem_destroy(const char* name) {
	if (sem_list == NULL) {
		// Empty list
		return;
	}

	sem_node_t* prev = NULL;
	sem_node_t* current = sem_list;
	while (current->next != NULL && strcmp(name, current->sem->name) != 0) {
		prev = current;
		current = current->next;
	}

	if(strcmp(name, current->sem->name) == 0) {
		if(prev == NULL) {
			sem_list = sem_list->next;
		} else {
			prev = current->next;
		}
	}

	free(current->sem);
	free(current);
}

sem_t* sem_get(const char* name) {
	if (sem_list == NULL) {
		// Empty list
		return NULL;
	}

	sem_node_t* current = sem_list;
	while (current->next != NULL && strcmp(name, current->sem->name) != 0) {
		current = current->next;
	}

	if(strcmp(name, current->sem->name) == 0) {
		return current->sem;
	}

	return NULL;
}

int sem_wait(sem_t* sem) {
	atom_begin();

	sem->value--;
	if (sem->value < 0) {
		int process = scheduler_getCurrentProcessId();
		sem_enqueue(sem, process);

		scheduler_suspend(process);

		atom_end();

		// Wait until state has changed again
		while (scheduler_getCurrentProcess()->state != PROCESS_RUNNING);

		return 0;
	}

	atom_end();

	return 0;
}

int sem_post(sem_t* sem) {
	atom_begin();

	sem->value++;
	if (sem->value <= 0) {
		int process = sem_dequeue(sem);

		scheduler_continue(process);
	}

	atom_end();

	return 0;
}

static void sem_enqueue(sem_t* sem, int pid) {
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

static int sem_dequeue(sem_t* sem) {
	if(sem->queue == NULL) {
		return -1;
	}

	node_t* node = sem->queue;
	sem->queue = sem->queue->next;

	int pid = node->pid;
	free(node);

	return pid;
}
