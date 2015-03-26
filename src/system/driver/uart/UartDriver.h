/*
 * UartDriver.h
 *
 *  Created on: 18.03.2015
 *      Author: Philip
 */

#ifndef SRC_SYSTEM_DRIVER_UARTDRIVER_H_
#define SRC_SYSTEM_DRIVER_UARTDRIVER_H_

#include <inttypes.h>
#include <cstdlib>

/**
 * Initialize the uart with the given configuration
 *
 * @param baud_rate The baud rate to use
 */
void uart_driver_init(uint32_t baud_rate);

/**
 * Send data through the uart port
 *
 * @param data The data to send
 * @param size The size of the data
 */
void uart_driver_write(char* data, size_t size);

/**
 * Get data from the uart port. This method will stop as soon there is no data to receive or the buffer is full.
 *
 * @param buffer The data in which the data will be saved in
 * @param size The maximum size to write. All data after this will be omitted.
 */
void uart_driver_read(char* buffer, size_t size);

/**
 * Resets the uart
 */
void uart_driver_reset();

/**
 * Register a callback method t
 */
void uart_driver_register_callback(void *callback(void));

/**
 * Will be called by the Interrupt Service Routine (ISR).
 * Do not call this manually.
 *
 * @internal
 */
void uart_driver_interrupt();



#endif /* SRC_SYSTEM_DRIVER_UARTDRIVER_H_ */
