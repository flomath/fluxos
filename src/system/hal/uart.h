/*
 * uart.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_UART_H_
#define SRC_SYSTEM_HAL_UART_H_

#include "hal.h"

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

#define EN_UART1		BV(13)
#define EN_UART2		BV(14)

// UART_LCR_REG
#define UART_LCR_MODE_CONF_A	0x80
#define UART_LCR_MODE_CONF_B	0xBF

// UART_EFR_REG
#define UART_EFR_ENHANCED_EN	BV(4)

// UART_MCR_REG
#define UART_MCR_TCR_TLR		BV(6)

// UART_SCR_REG
#define UART_SCR_RX_TRIG_GRAN	BV(7)
#define UART_SCR_TX_TRIG_GRAN	BV(6)
#define UART_SCR_DMA_MODE_CTL	BV(0)

void init_uart(uint32_t port);
void uart_enable_uart1(void);
void uart_disable_uart1(void);

uint8_t uart_write(uint32_t port, uint8_t offset, uint8_t mask);
uint8_t uart_set(uint32_t port, uint8_t offset, uint8_t mask);
uint8_t uart_clear(uint32_t port, uint8_t offset, uint8_t mask);

mmio_t uart_get_register(uint32_t port, uint8_t offset); // get register address

#endif /* SRC_SYSTEM_HAL_UART_H_ */
