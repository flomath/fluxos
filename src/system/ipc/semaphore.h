/*
 * semaphore.h
 *
 *  Created on: 03.07.2015
 *      Author: Philip
 */

#ifndef SRC_SYSTEM_IPC_SEMAPHORE_H_
#define SRC_SYSTEM_IPC_SEMAPHORE_H_

typedef struct sem_process_node node_t;
typedef struct sem_node sem_node_t;

struct sem_process_node {
	int pid;
	node_t* next;
};

typedef struct {
	int value;
	char* name;
	node_t* queue;
} sem_t;

struct sem_node {
	sem_t* sem;
	sem_node_t* next;
};

/**
 * Create a new semaphore and return it
 */
sem_t* sem_create(const char* name);

/**
 * Find the semaphore and destroy it
 */
void sem_destroy(const char* name);

/**
 * Find the semaphore and return it
 */
sem_t* sem_get(const char* name);

/**
 * Wait until semaphore is released
 */
int sem_wait(sem_t* sem);

/**
 * Release the semaphore
 */
int sem_post(sem_t* sem);

/**
 * Enqueue the process to the queue
 */
void sem_enqueue(sem_t* sem, int pid);

/**
 * Dequeue the next process
 */
int sem_dequeue(sem_t* sem);

#endif /* SRC_SYSTEM_IPC_SEMAPHORE_H_ */
