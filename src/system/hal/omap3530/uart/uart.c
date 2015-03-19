/*
 * uart.c
 *
 *  Created on: 12.03.2015
 *      Author: florian
 */

#include "uart.h"

void uart_init(uint32_t port, UARTConfiguration_t* conf)
{
	// Reset the UART
	uart_software_reset(port);

	// FIFO and DMA settings
	// 1. Switch to config mode B
	uint8_t lcrValue = hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 2. Enable register submode 1/2
	uint8_t efrValue = hal_bitmask_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);

	// 3. Switch to config mode A
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// 4. enable register submode 2/2
	uint8_t mcrValue = hal_bitmask_set(port, UART_MCR_REG, UART_MCR_TCR_TLR);

	// 5. Enable FIFO, load new FIFO Triggers 1/3 and set DMA-Mode 1/2
	uint8_t fifoMask = 0b00001001; // 8 char Trigger (RX/TX), DMA-Mode 1, FIFO-Enable
	hal_bitmask_write(port, UART_FCR_REG, fifoMask);

	// 6. Switch to config mode B
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 7. load new FIFO triggers 2/3
	hal_bitmask_clear(port, UART_TLR_REG, 0xFF); // reset RX_FIFO_TRIG_DMA and TX_FIFO_TRIG_DMA to 0

	// 8. load new FIFO triggers 3/3 and set DMA-MODE 2/2
	//hal_bitmask_set(port, UART_SCR_REG, UART_SCR_RX_TRIG_GRAN);
	hal_bitmask_set(port, UART_SCR_REG, UART_SCR_TX_TRIG_GRAN);
	//hal_bitmask_clear(port, UART_SCR_REG, BV(2)); 	// DMA-Mode 1 (RX and TX)
	//hal_bitmask_set(port, UART_SCR_REG, BV(1));	// DMA-Mode 1 (RX and TX)
	//hal_bitmask_set(port, UART_SCR_REG, UART_SCR_DMA_MODE_CTL);

	// 9. restore enable register
	if(efrValue & UART_EFR_ENHANCED_EN == 1) {
		hal_bitmask_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);
	} else {
		hal_bitmask_clear(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);
	}

	// 10. Switch to config mode A
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_A);

	// 11. Restore MCR Reg
	if(mcrValue & UART_MCR_TCR_TLR) {
		hal_bitmask_set(port, UART_MCR_REG, UART_MCR_TCR_TLR);
	} else {
		hal_bitmask_clear(port, UART_MCR_REG, UART_MCR_TCR_TLR);
	}

	// 12. Restore LCR Reg
	hal_bitmask_write(port, UART_LCR_REG, lcrValue);

	// Protocol, Baud Rate, and Interrupt Settings
	// 1. Disable UART
	hal_bitmask_set(port, UART_MDR1_REG, BV(2) + BV(1) + BV(0)); // Mode-Select to 0x7 (Disable)

	// 2. Switch to config mode B
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 3. Enable access to IER_REG
	uint8_t efrValue2 = hal_bitmask_set(port, UART_EFR_REG, UART_EFR_ENHANCED_EN);

	// 4. Switch to register operational mode
	hal_bitmask_set(port, UART_LCR_REG, 0x00);

	// 5. Clear IER_REG
	hal_bitmask_write(port, UART_IER_REG, 0x00);

	// 6. Switch to config mode B
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 7. Load new divisor value
	// DLL_REG.CLOCK_LSB and DLH_REG.CLOCK_MSB
	uint16_t divisor = CLK_UART / (16 * conf->baude_rate);
	hal_bitmask_write(port, UART_DLL_REG, divisor); // LSB
	hal_bitmask_write(port, UART_DLH_REG, (divisor >> 8)); // MSB

	// 8. Switch to register operational mode
	hal_bitmask_set(port, UART_LCR_REG, 0x00);

	// 9. Load new interrupt configuration
	// no other interrupts are enabled at this time
	hal_bitmask_set(port, UART_IER_REG, BV(1)); // THR_IT enable (Data ready)

	// 10. Switch to config mode B
	hal_bitmask_write(port, UART_LCR_REG, UART_LCR_MODE_CONF_B);

	// 11. Restore Enhanced_Enable saved in step 3
	if(efrValue2 & UART_EFR_ENHANCED_EN == 1) {
		hal_bitmask_set(port, UART_EFR_REG, efrValue2);
	} else {
		hal_bitmask_clear(port, UART_EFR_REG, efrValue2);
	}

	// 12. Load new protocol formatting
	// DIV_EN to operational mode
	// BREAK_EN to normal operating condition
	hal_bitmask_clear(port, UART_LCR_REG, 0xFF); // clear register

	// set stop bit
	if ( conf->stop_bit ) hal_bitmask_set(port, UART_LCR_REG, BV(2));
	
	// set parity type
	switch ( conf->parity_type ) 
	{
		case UART_PARITY_NONE:
			break;

		case UART_PARITY_0:
			hal_bitmask_set(port, UART_LCR_REG, BV(3)); // Parity enable
			hal_bitmask_set(port, UART_LCR_REG, BV(4)); // PARITY_TYPE_1 (Even)
			hal_bitmask_set(port, UART_LCR_REG, BV(5)); // PARITY_TYPE_2
			break;

		case UART_PARITY_1:
			hal_bitmask_set(port, UART_LCR_REG, BV(3)); // Parity enable
			hal_bitmask_set(port, UART_LCR_REG, BV(5)); // PARITY_TYPE_2
			break;
	}

	// set char length
	hal_bitmask_write(port, UART_LCR_REG, hal_get_address_value(port, UART_LCR_REG) | conf->char_length);

	// 13. Load new UART mode (set to UART 16x mode)
	hal_bitmask_clear(port, UART_MDR1_REG, 0x7);
}

void uart_software_reset(uint32_t port)
{
	hal_bitmask_set(port, UART_SYSC_REG, BV(1));

	// wait for reset
	while(hal_get_address_value(port, UART_SYSS_REG) & BV(0) != 1) {}
}


void uart_write(uint32_t port, uint8_t* buffer)
{
	mmio_t reg = hal_get_register(port, UART_THR_REG);
	*reg = *buffer;
}

void uart_read(uint32_t port, uint8_t* buffer)
{
	mmio_t reg = hal_get_register(port, UART_RHR_REG);
	*buffer = *reg;
}
