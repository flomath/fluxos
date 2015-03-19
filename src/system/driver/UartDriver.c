/*
 * UartDriver.c
 *
 *  Created on: 18.03.2015
 *      Author: Philip
 */

#include "UartDriver.h"
#include "../hal/arch/omap3530/uart/uart.h"


void uart_driver_init(uint32_t baud_rate) {
	// Enable the uart 1
	uart_enable_uart1();

	// Create configuration
	UARTConfiguration_t config;
	config.baude_rate = baud_rate;
	config.char_length = UART_LENGTH_5;
	config.parity_type = UART_PARITY_NONE;
	config.stop_bit = 0x0;

	uart_init(UART1, &config);
}

void uart_driver_write(char* data, size_t size) {

}

void uart_driver_read(char* buffer, size_t size) {

}

void uart_driver_reset() {
	uart_software_reset(UART1);
}

void uart_driver_register_callback(void *callback(void)) {

}

void uart_driver_interrupt() {

}


