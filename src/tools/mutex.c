/*
 * mutex.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include "mutex.h"

int mutex_lock() {
	// _disable_interrupts();
	return 1;
}

int mutex_release() {
	// _enable_interrupts();
	return 0;
}
