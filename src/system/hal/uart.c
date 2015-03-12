/*
 * uart.c
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#include "uart.h"

/**
 * enable and configure rx/tx FIFO and DMA mode
 * and set protocol and baude rate
 */
void init_uart(uint32_t port)
{
	// FIFO and DMA settings

	// Switch to config mode B
	uint8_t lcrValue = uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// Enable register submode 1/2
	uint8_t efrValue = uart_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);

	// Switch to config mode A
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// enable register submode 2/2
	uint8_t mcrValue = uart_set(port, UART_MCR_REG, UART_MCR_TCR_TLR);

	// Enable FIFO, load new FIFO Triggers 1/3 and set DMA-Mode 1/2
	uint8_t fifoMask = 0b01011001; // 16 char Trigger (RX/TX), DMA-Mode 1, FIFO-Enable
	uart_set(port, UART_FCR_REG, fifoMask);

	// Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// load new FIFO triggers 2/3
	uart_clear(port, UART_TLR_REG, 0xFF); // reset RX_FIFO_TRIG_DMA and TX_FIFO_TRIG_DMA to 0

	// load new FIFO triggers 3/3 and set DMA-MODE 2/2
	uart_set(port, UART_SCR_REG, UART_SCR_RX_TRIG_GRAN);
	uart_set(port, UART_SCR_REG, UART_SCR_TX_TRIG_GRAN);
	uart_clear(port, UART_SCR_REG, BV(2)); 	// DMA-Mode 1 (RX and TX)
	uart_set(port, UART_SCR_REG, BV(1));	// DMA-Mode 1 (RX and TX)
	uart_set(port, UART_SCR_REG, UART_SCR_DMA_MODE_CTL);

	// restore enable register
	uart_write(port, UART_EFR_REG, efrValue);

	// Switch to config mode A
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// Restore MCR Reg
	uart_write(port, UART_MCR_REG, mcrValue);

	// Restore LCR Reg
	uart_write(port, UART_LCR_REG, lcrValue);


	// Protocol, Baud Rate, and Interrupt Settings
	// 1. Disable UART
	uart_set(port, UART_MDR1_REG, BV(2) + BV(1) + BV(0)); // Mode-Select to 0x7 (Disable)

	// 2. Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 3. Enable access to IER_REG
	uint8_t efrValue2 = uart_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);

	// 4. Switch to register operational mode
	uart_set(port, UART_LCR_REG, 0x00);

	// 5. Clear IER_REG
	uart_write(port, UART_IER_REG, 0x00);

	// 6. Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 7. Load new divisor value

	// 8. Switch to register operational mode
	uart_set(port, UART_LCR_REG, 0x00);

	// 9. Load new interrupt configuration

	// 10. Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 11. Restore Enhanced_Enable saved in step 3
	uart_write(port, UART_EFR_REG, efrValue2);

	// 12. Load new protocol formatting

	// 13. Load new UART mode
}

mmio_t uart_get_register(uint32_t port, uint8_t offset)
{
	return (mmio_t)(port + offset);
}

/**
 * Write mask into address
 */
uint8_t uart_write(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg = mask;

	return value;
}

/**
 * Enable a bit(mask) of given port and offset
 */
uint8_t uart_set(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg |= mask;

	return value;
}

/**
 * Clear a bit(mask) of given port and offset
 */
uint8_t uart_clear(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg &= ~mask;

	return value;
}
