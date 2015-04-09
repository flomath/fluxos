/*
 * UartDriver.c
 *
 *  Created on: 18.03.2015
 *      Author: Philip
 */

#include "UartDriver.h"
#include "../../hal/omap3530/uart/uart.h"
#include "../../hal/common/interrupt.h"

#define UART_DRIVER_DEVICE 			UART3
#define UART_DRIVER_IRQ 			74
#define UART_DRIVER_BUFFER_SIZE 	16

void (*uart_driver_callback_method)(void) = NULL;
static uint8_t uart_driver_buffer[UART_DRIVER_BUFFER_SIZE];
static int uart_driver_buffer_pos = 0;

void uart_driver_init(uint32_t baud_rate) {
	// Create configuration
	UARTConfiguration_t conf = {
			0x1,
			baud_rate,
			UART_PARITY_NONE,
			UART_LENGTH_8
	};
	uart_init(UART_DRIVER_DEVICE, &conf);

	// Enable interrupts
	interrupt_add_listener(UART_DRIVER_IRQ, &uart_driver_interrupt);
}

void uart_driver_write(char* data, size_t size) {
	uint8_t* current = (uint8_t*)data;
	while (current < (uint8_t*)data + size) {
		uart_write(UART_DRIVER_DEVICE, current);
		++current;
	}
}

void uart_driver_read(char* buffer, size_t size) {
	uint8_t* current = (uint8_t*)buffer;
	int buffer_pos = 0;
	while (current < ((uint8_t*)buffer + size) && buffer_pos < uart_driver_buffer_pos) {
		*current = uart_driver_buffer[buffer_pos];
		++current;
		++buffer_pos;
	}

	// If not all data was received, move the rest of the data to the beginning of the buffer
	int remainding = uart_driver_buffer_pos - buffer_pos;
	int i;
	for ( i = 0; i < remainding; i++ ) {
		uart_driver_buffer[i] = uart_driver_buffer[buffer_pos];
		++buffer_pos;
	}

	uart_driver_buffer_pos = remainding;
}

int uart_driver_count() {
	return uart_driver_buffer_pos;
}

void uart_driver_reset() {
	uart_software_reset(UART_DRIVER_DEVICE);
}

void uart_driver_register_callback(void (*callback)(void)) {
	uart_driver_callback_method = callback;
}

void uart_driver_interrupt() {
	uint8_t type = BIT_RANGE(hal_get_address_value(UART_DRIVER_DEVICE, UART_IIR_REG), 5, 1);
	uint8_t received = 0;
	switch (type) {
	case 0x2:
		// RHR
		received = hal_get_address_value(UART_DRIVER_DEVICE, UART_RHR_REG);

		// Clear RS-FIFO Trigger and reset counter logic
		hal_bitmask_set(UART_DRIVER_DEVICE, UART_FCR_REG, BV(1));

		break;
	case 0x6:
		// Timeout
		received = hal_get_address_value(UART_DRIVER_DEVICE, UART_RHR_REG);
		break;
	default:
		return;
	}

	if ( received != 0 ) {
		// Add to buffer
		if ( uart_driver_buffer_pos > UART_DRIVER_BUFFER_SIZE ) {
			uart_driver_buffer_pos = 0;
		}
		uart_driver_buffer[uart_driver_buffer_pos++] = received;

		// Call callback
		if ( uart_driver_callback_method != NULL ) {
			uart_driver_callback_method();
		}
	}
}


