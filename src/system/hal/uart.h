/*
 * uart.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_UART_H_
#define SRC_SYSTEM_HAL_UART_H_

#include "hal.h"

#define CLK_UART		48000000

#define UART1			0x4806A000 // [17-38]
#define UART2			0x4806C000
#define UART3			0x49020000

#define UART_LCR_REG	0x00C // line control register [17-64]
#define UART_EFR_REG	0x008 // enhanced feature register [17-62]
#define UART_MCR_REG	0x010 // modem control register [17-66]
#define	UART_FCR_REG	0x008 // FIFO control register [17-54]
#define UART_TLR_REG	0x01C // trigger level register [17-88]
#define UART_SCR_REG	0x040 // supplementary control register [17-116]
#define UART_MDR1_REG	0x020 // Mode definition register [17-90]
#define UART_IER_REG	0x004 // Interrupt enable register [17-46]
#define UART_DLL_REG	0x000 // Divisor latches low [17-40
#define UART_DLH_REG	0x004 // Divisor latches high [17-52]
#define UART_SYSC_REG	0x054 // System configuration register [17-124]
#define UART_SYSS_REG	0x058 // System status register [17-126]

#define EN_UART1		BV(13)
#define EN_UART2		BV(14)

// UART_LCR_REG
#define UART_LCR_MODE_CONF_A	0x80
#define UART_LCR_MODE_CONF_B	0xBF
#define UART_LCR_DIV_EN			BV(7) // also used for conf mode a/b
#define UART_LCR_BREAK_EN		BV(6)

// UART_EFR_REG
#define UART_EFR_ENHANCED_EN	BV(4)

// UART_MCR_REG
#define UART_MCR_TCR_TLR		BV(6)

// UART_SCR_REG
#define UART_SCR_RX_TRIG_GRAN	BV(7)
#define UART_SCR_TX_TRIG_GRAN	BV(6)
#define UART_SCR_DMA_MODE_CTL	BV(0)

/**
 * LCR_REG Parity Options
 * UART_PARITY_NONE: Parity disabled
 * UART_PARITY_0: Paritybit 0 forced
 * UART_PARITY_1: Paritybit 1 forced
 */
typedef enum uart_parity_type {
	UART_PARITY_NONE,
	UART_PARITY_0,
	UART_PARITY_1
} uart_parity_type;
/*
 * LCR_REG character length
 */
typedef enum uart_char_length {
	UART_LENGTH_5 = 0x0,
	UART_LENGTH_6 = 0x1,
	UART_LENGTH_7 = 0x2,
	UART_LENGTH_8 = 0x3
} uart_char_length;

typedef struct {
	uint8_t stop_bit;
	uint32_t baude_rate;
	uart_parity_type parity_type;
	uart_char_length char_length;
} UARTConfiguration_t;

void init_uart(uint32_t port, UARTConfiguration_t* conf);
void uart_enable_uart1(void);
void uart_disable_uart1(void);

uint8_t uart_write(uint32_t port, uint8_t offset, uint8_t mask);
uint8_t uart_set(uint32_t port, uint8_t offset, uint8_t mask);
uint8_t uart_clear(uint32_t port, uint8_t offset, uint8_t mask);
uint8_t uart_get_value(uint32_t port, uint8_t offset);
void uart_software_reset(uint32_t port);

mmio_t uart_get_register(uint32_t port, uint8_t offset); // get register address

#endif /* SRC_SYSTEM_HAL_UART_H_ */
