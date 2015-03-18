/*
 * uart.c
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#include "uart.h"

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

#define EN_UART1		BV(13)
#define EN_UART2		BV(14)

// UART_LCR_REG
#define UART_LCR_MODE_CONF_A	0x80
#define UART_LCR_MODE_CONF_B	0xBF
#define UART_LCR_DIV_EN			BV(7) ///< also used for conf mode a/b
#define UART_LCR_BREAK_EN		BV(6)

// UART_EFR_REG
#define UART_EFR_ENHANCED_EN	BV(4)

// UART_MCR_REG
#define UART_MCR_TCR_TLR		BV(6)

// UART_SCR_REG
#define UART_SCR_RX_TRIG_GRAN	BV(7)
#define UART_SCR_TX_TRIG_GRAN	BV(6)
#define UART_SCR_DMA_MODE_CTL	BV(0)

void uart_enable_uart1(void)
{
	// Todo
}

void uart_disable_uart1(void)
{
	// Todo
}

void uart_init(uint32_t port, UARTConfiguration_t* conf)
{
	// Reset the UART
	uart_software_reset(port);

	// FIFO and DMA settings
	// 1. Switch to config mode B
	uint8_t lcrValue = uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 2. Enable register submode 1/2
	uint8_t efrValue = uart_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);

	// 3. Switch to config mode A
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// 4. enable register submode 2/2
	uint8_t mcrValue = uart_set(port, UART_MCR_REG, UART_MCR_TCR_TLR);

	// 5. Enable FIFO, load new FIFO Triggers 1/3 and set DMA-Mode 1/2
	uint8_t fifoMask = 0b01011001; // 16 char Trigger (RX/TX), DMA-Mode 1, FIFO-Enable
	uart_set(port, UART_FCR_REG, fifoMask);

	// 6. Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 7. load new FIFO triggers 2/3
	uart_clear(port, UART_TLR_REG, 0xFF); // reset RX_FIFO_TRIG_DMA and TX_FIFO_TRIG_DMA to 0

	// 8. load new FIFO triggers 3/3 and set DMA-MODE 2/2
	uart_set(port, UART_SCR_REG, UART_SCR_RX_TRIG_GRAN);
	uart_set(port, UART_SCR_REG, UART_SCR_TX_TRIG_GRAN);
	uart_clear(port, UART_SCR_REG, BV(2)); 	// DMA-Mode 1 (RX and TX)
	uart_set(port, UART_SCR_REG, BV(1));	// DMA-Mode 1 (RX and TX)
	uart_set(port, UART_SCR_REG, UART_SCR_DMA_MODE_CTL);

	// 9. restore enable register
	uart_write(port, UART_EFR_REG, efrValue);

	// 10. Switch to config mode A
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// 11. Restore MCR Reg
	uart_write(port, UART_MCR_REG, mcrValue);

	// 12. Restore LCR Reg
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
	// DLL_REG.CLOCK_LSB and DLH_REG.CLOCK_MSB
	uint16_t divisor = CLK_UART / conf->baude_rate;
	uart_write(port, UART_DLL_REG, divisor); // LSB
	uart_write(port, UART_DLH_REG, (divisor >> 8)); // MSB

	// 8. Switch to register operational mode
	uart_set(port, UART_LCR_REG, 0x00);

	// 9. Load new interrupt configuration
	// no other interrupts are enabled at this time
	uart_set(port, UART_IER_REG, BV(0)); // RHR_IT enable (Data ready)

	// 10. Switch to config mode B
	uart_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 11. Restore Enhanced_Enable saved in step 3
	uart_write(port, UART_EFR_REG, efrValue2);

	// 12. Load new protocol formatting
	// DIV_EN to operational mode
	// BREAK_EN to normal operating condition
	uart_write(port, UART_LCR_REG, 0x00); // clear register

	// set stop bit
	if ( conf->stop_bit ) uart_set(port, UART_LCR_REG, BV(2));
	
	// set parity type
	switch ( conf->parity_type ) 
	{
		case UART_PARITY_NONE:
			break;

		case UART_PARITY_0:
			uart_set(port, UART_LCR_REG, BV(3)); // Parity enable
			uart_set(port, UART_LCR_REG, BV(4)); // PARITY_TYPE_1 (Even)
			uart_set(port, UART_LCR_REG, BV(5)); // PARITY_TYPE_2
			break;

		case UART_PARITY_1:
			uart_set(port, UART_LCR_REG, BV(3)); // Parity enable
			uart_set(port, UART_LCR_REG, BV(5)); // PARITY_TYPE_2
			break;
	}

	// set char length
	uart_write(port, UART_LCR_REG, uart_get_value(port, UART_LCR_REG) | conf->char_length);

	// 13. Load new UART mode (set to UART 16x mode)
	uart_clear(port, UART_MDR1_REG, 0x7);
}

void uart_software_reset(uint32_t port)
{
	uart_set(port, UART_SYSC_REG, BV(1));

	// wait for reset
	while(uart_get_value(port, UART_SYSS_REG) & BV(0) != 1) {}
}

mmio_t uart_get_register(uint32_t port, uint8_t offset)
{
	return (mmio_t)(port + offset);
}


uint8_t uart_get_value(uint32_t port, uint8_t offset)
{
	mmio_t reg = uart_get_register(port, offset);
	return *reg;
}


uint8_t uart_write(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg = mask;

	return value;
}


uint8_t uart_set(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg |= mask;

	return value;
}


uint8_t uart_clear(uint32_t port, uint8_t offset, uint8_t mask)
{
	mmio_t reg = uart_get_register(port, offset);
	uint8_t value = *reg;
	*reg &= ~mask;

	return value;
}
