/*
 * uart.h
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_ARCH_OMAP3530_UART_H_
#define SRC_SYSTEM_HAL_ARCH_OMAP3530_UART_H_

#include "../../common/uart.h"

#define UART1			0x4806A000 ///< UART 1 address [17-38]
#define UART2			0x4806C000 ///< UART 2 address [17-38]
#define UART3			0x49020000 ///< UART 3 address [17-38]

#define CLK_UART		48000000

#define UART_LCR_REG	0x00C ///< line control register [17-64]
#define UART_EFR_REG	0x008 ///< enhanced feature register [17-62]
#define UART_MCR_REG	0x010 ///< modem control register [17-66]
#define	UART_FCR_REG	0x008 ///< FIFO control register [17-54]
#define UART_TLR_REG	0x01C ///< trigger level register [17-88]
#define UART_SCR_REG	0x040 ///< supplementary control register [17-116]
#define UART_MDR1_REG	0x020 ///< Mode definition register [17-90]
#define UART_IER_REG	0x004 ///< Interrupt enable register [17-46]
#define UART_DLL_REG	0x000 ///< Divisor latches low [17-40
#define UART_DLH_REG	0x004 ///< Divisor latches high [17-52]
#define UART_SYSC_REG	0x054 ///< System configuration register [17-124]
#define UART_SYSS_REG	0x058 ///< System status register [17-126]
#define UART_THR_REG 	0x000 ///< Transmit holding register [17-44]
#define UART_RHR_REG 	0x000 ///< Receive holding register [17-42]
#define UART_IIR_REG	0x008 ///< Interrupt identification register [17-56]

#define EN_UART1		BV(13)
#define EN_UART2		BV(14)

// UART_LCR_REG
#define UART_LCR_MODE_CONF_A	0x80
#define UART_LCR_MODE_CONF_B	0xBF
#define UART_LCR_DIV_EN			BV(7) ///< also used for conf mode a/b
#define UART_LCR_BREAK_EN		BV(6)

// UART_EFR_REG
#define UART_EFR_ENHANCED_EN	BV(4)

// UART_UFR_REG
#define UART_IER_ENHANCED_EN	BV(4)

// UART_MCR_REG
#define UART_MCR_TCR_TLR		BV(6)

// UART_SCR_REG
#define UART_SCR_RX_TRIG_GRAN	BV(7)
#define UART_SCR_TX_TRIG_GRAN	BV(6)
#define UART_SCR_DMA_MODE_CTL	BV(0)

#endif /* SRC_SYSTEM_HAL_ARCH_OMAP3530_UART_H_ */
