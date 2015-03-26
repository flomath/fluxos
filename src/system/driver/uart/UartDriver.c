/*
 * UartDriver.c
 *
 *  Created on: 18.03.2015
 *      Author: Philip
 */

#include "UartDriver.h"
#include "../../hal/omap3530/uart/uart.h"


void uart_driver_init(uint32_t baud_rate) {
	// Create configuration
	UARTConfiguration_t config;
	config.baude_rate = baud_rate;
	config.char_length = UART_LENGTH_8;
	config.parity_type = UART_PARITY_NONE;
	config.stop_bit = 0x0;

	uart_init(UART3, &config);
}

void uart_driver_write(char* data, size_t size) {
	uint8_t* current = (uint8_t*)data;
	while (current < (uint8_t*)data + size) {
		uart_write(UART3, current);
		++current;
	}
}

void uart_driver_read(char* buffer, size_t size) {
	uint8_t* current = (uint8_t*)buffer;
	while (current < (uint8_t*)buffer + size) {
		// Todo.. Break if there is no data
		uart_read(UART3, current);
		++current;
	}
}

void uart_driver_reset() {
	uart_software_reset(UART1);
}

void uart_driver_register_callback(void *callback(void)) {

}

void uart_driver_interrupt() {

}


