/*
 * hutex.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_TOOLS_MUTEX_H_
#define SRC_TOOLS_MUTEX_H_

typedef struct Mutex {
	int count;
} Mutex_t;

int mutex_lock(); // Mutex P()
int mutex_release(); 	// Mutex V()

#endif /* SRC_TOOLS_MUTEX_H_ */
