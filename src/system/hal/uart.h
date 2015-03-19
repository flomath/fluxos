/*
 * uart.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_UART_H_
#define SRC_SYSTEM_HAL_UART_H_

#include "hal.h"

/**
 * LCR_REG Parity Options
 */
typedef enum {
	UART_PARITY_NONE,	///< Parity disabled
	UART_PARITY_0,		///< Paritybit 0 forced
	UART_PARITY_1		///< Paritybit 1 forced
} uart_parity_type_t;

/**
 * LCR_REG character length
 */
typedef enum {
	UART_LENGTH_5 = 0x0,	///< 5 Bits
	UART_LENGTH_6 = 0x1,	///< 6 Bits
	UART_LENGTH_7 = 0x2,	///< 7 Bits
	UART_LENGTH_8 = 0x3		///< 8 Bits
} uart_char_length_t;

/**
 * Configuration struct
 */
typedef struct {
	uint8_t stop_bit;				///< If 0 one stop bit regardless of the word length, otherwise 1.5 stop bits for word length 5 and 2 stop bits for word length 6, 7, 8
	uint32_t baude_rate;			///< The baude rate
	uart_parity_type_t parity_type;	///< Disabled, odd or even parity bit
	uart_char_length_t char_length;	///< The word size of the data (5, 6, 7 or 8 bit)
} UARTConfiguration_t;

/**
 * Initialize the uart
 *
 * @param port The address of the port to configure. Possible params are UART1, UART2 and UART3
 * @param conf The configuration consisting of the stop_bit, baud_rate, parity_type and char_length
 */
void uart_init(uint32_t port, UARTConfiguration_t* conf);

/**
 * Software reset
 *
 * @param port The address of the port to reset. Possible params are UART1, UART2 and UART3
 */
void uart_software_reset(uint32_t port);

/**
 * Write character to the uart
 *
 * @param port The address of the port to write to. Possible params are UART1, UART2 and UART3
 * @param buffer Buffer which contains the data to write to the uart
 */
void uart_write(uint32_t port, uint8_t* buffer);

/**
 * Reads character from the uart
 *
 * @param port The address of the port to read from. Possible params are UART1, UART2 and UART3
 * @param buffer Buffer which is filled with data from the uart
 */
void uart_read(uint32_t port, uint8_t* buffer);

#endif /* SRC_SYSTEM_HAL_UART_H_ */
