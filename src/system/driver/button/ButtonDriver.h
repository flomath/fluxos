/*
 * pushbutton.h
 *
 *  Created on: 14.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_DRIVER_BUTTON_BUTTONDRIVER_H_
#define SRC_SYSTEM_DRIVER_BUTTON_BUTTONDRIVER_H_

#include<stdio.h>

/**
 * initialise the simple button driver
 */
void button_driver_init();

/**
 * interrupt handler when button is pressed
 */
void button_driver_interrupt();

#endif /* SRC_SYSTEM_DRIVER_BUTTON_BUTTONDRIVER_H_ */
