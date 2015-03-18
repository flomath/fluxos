/*
 * uart.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_UART_H_
#define SRC_SYSTEM_HAL_UART_H_

#include "hal.h"

#define UART1			0x4806A000 ///< UART 1 address [17-38]
#define UART2			0x4806C000 ///< UART 2 address [17-38]
#define UART3			0x49020000 ///< UART 3 address [17-38]

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
 * Enable the uart
 */
void uart_enable_uart1(void);

/**
 * Disable the uart
 */
void uart_disable_uart1(void);

/**
 * Write mask into address
 */
uint8_t uart_write(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Enable a bit(mask) of given port and offset
 */
uint8_t uart_set(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Clear a bit(mask) of given port and offset
 */
uint8_t uart_clear(uint32_t port, uint8_t offset, uint8_t mask);

/**
 * Get value of given address
 */
uint8_t uart_get_value(uint32_t port, uint8_t offset);

/**
 * Software reset
 */
void uart_software_reset(uint32_t port);

/**
 * Returns the register address
 *
 * @internal
 */
mmio_t uart_get_register(uint32_t port, uint8_t offset);

#endif /* SRC_SYSTEM_HAL_UART_H_ */
