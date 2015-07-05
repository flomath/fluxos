/*
 * mcbsp.c
 *
 *  Created on: 14.05.2015
 *      Author: florian
 *      modified: philip (03.06.2015)
 */


/*
 * notes:
 *
 * CS4231A audio codec
 * stereo-mode: configure for a one phase frame containing one 32 bit word
 * 				upper 16 bit (left ch.), lower 16 bit (right ch.)
 *
 * mono-mode: 	configure for a one phase frame containing one 16 bit word
 * 				input: codec only samples left ch.
 * 				output: codec sends sample to both channels
 */

#include "mcbsp.h"
#include "../clock/clock.h"

void mcbsp2_enable(void) {
	// Activate PER_96M_FCLK (Internal Clock)
	// Activate Clocks (page 2974)
	hal_bitmask_clear(CONTROL, CONTROL_DEVCONF0, BV(6));

	hal_bitmask_set(PER_CM, CM_FCLKEN_PER, BV(0));
	hal_bitmask_set(PER_CM, CM_ICLKEN_PER, BV(0));
}

void mcbsp_init_master2(uint32_t mcbsp) {
	// Hardware and Software Reset
	// McBSP2,3 and 4 belong to the PER Domain and their reset signal is the PER_RST from the RRCM module

	// Set the transmitter in reset mode
	hal_bitmask_clear(mcbsp, MCBSPLP_SPCR2_REG, BV(7));
	hal_bitmask_clear(mcbsp, MCBSPLP_SPCR2_REG, BV(0));

	// Set the receiver in reset mode
	hal_bitmask_clear(mcbsp, MCBSPLP_SPCR1_REG, BV(0));

	// disable gpio-mode
	hal_bitmask_clear(mcbsp, MCBSPLP_PCR_REG, BV(13));

	// xmit format
	// XCR1
	hal_bitmask_set(mcbsp, MCBSPLP_XCR1_REG, BV(6)); 	// 16-bit word length for phase 1
	hal_bitmask_set(mcbsp, MCBSPLP_XCR1_REG, BV(8));	// framelength = 1 word

	// XCR2
	hal_bitmask_set(mcbsp, MCBSPLP_XCR2_REG, BV(15)); 	// 2 phase
	hal_bitmask_set(mcbsp, MCBSPLP_XCR2_REG, BV(0));	// delay of 1 ms for i2s page
	hal_bitmask_clear(mcbsp, MCBSPLP_XCR2_REG, BV(8));	// framelength = 1 word
	hal_bitmask_set(mcbsp, MCBSPLP_XCR2_REG, BV(6));	// 16-bit word length for phase 2

	// receive format
	// RCR1
	hal_bitmask_set(mcbsp, MCBSPLP_RCR1_REG, BV(6)); 	// 16-bit word length for phase 1
	hal_bitmask_set(mcbsp, MCBSPLP_RCR1_REG, BV(8));	// framelength = 1 word

	// RCR2
	hal_bitmask_set(mcbsp, MCBSPLP_RCR2_REG, BV(15)); 	// 2 phase
	hal_bitmask_set(mcbsp, MCBSPLP_RCR2_REG, BV(0));	// delay of 1 ms for i2s page
	hal_bitmask_clear(mcbsp, MCBSPLP_RCR2_REG, BV(8));	// framelength = 1 word
	hal_bitmask_set(mcbsp, MCBSPLP_RCR2_REG, BV(6));	// 16-bit word length for phase 2

	// disable DMA
	//hal_bitmask_set(MCBSP1_BASE, MCBSPLP_XCCR_REG, BV(0));

	// clock polarity
	// receiver
	//hal_bitmask_set(MCBSP1, MCBSPLP_PCR_REG, BV(0)); // rising edge
	//hal_bitmask_set(MCBSP1, MCBSPLP_PCR_REG, BV(1)); // falling edge

	// Wait to allow settings to sync
	int loop;
	for (loop = 0; loop < 0XFF; loop++);

	// Wake up the transmitter of reset mode (needed to configure)
	hal_bitmask_set(mcbsp, MCBSPLP_SPCR2_REG, BV(7));
	hal_bitmask_set(mcbsp, MCBSPLP_SPCR2_REG, BV(0));

	// Wake up the receiver of reset mode (needed to configure)
	hal_bitmask_set(mcbsp, MCBSPLP_SPCR1_REG, BV(0));
}

void mcbsp_init_master(uint32_t mcbsp) {
	// See OMAP35x Reference Guide page 3026

	// STEP 1
	// Coming out after a global reset? We assume that we won't call this method a second time after booting up. Therefore we can ignore this check

	// Set either McBSPi.MCBSPLP_SPCR1_REG[0], McBSPi.MCBSPLP_SPCR2_REG[7] or McBSPi.MCBSPLP_SPCR2_REG[0] bit to 0
	hal_bitmask_clear(mcbsp, MCBSPLP_SPCR2_REG, BV(0)); // XRST -> Disable Transmitter

	// STEP 2
	// Program the configuration registers as required

	// McBSPi.MCBSPLP_SPCR1_REG
	// McBSPi.MCBSPLP_SPCR2_REG
	// McBSPi.MCBSPLP_PCR_REG
	hal_bitmask_set(mcbsp, MCBSPLP_PCR_REG,
			BV(11) +	// FSXM: Frame synchronization is determined by SRG in SRGR2 (FSGM)
			BV(9) + 	// CLKXM is an output pin and is driven by the internal SRG
			BV(7) +		// The frequency of the CLKG is CLKG frequency = (Input clock frequency) / (CLKGDV + 1)
						// SCLKME is used in conjunction with the CLKSM bit to select the input clock:
						// 0x0:
						// CLKSM = 0: Signal on CLKS pin
						// CLKSM = 1: McBSPi_ICLK clock
						// 0x1:
						// CLKSM = 0: Signal on CLKR pin
						// CLKSM = 1: Signal on CLKX pin

			BV(3) + 	// FSXP: Transmit Frame Synchronization Polarity; Frame synchronization pulse FSX is active high (0) / low (1)
			BV(2) + 	// FSRP: Receive Frame Synchronization Polarity; FSR is high (0) / low (1)
			BV(1));		// CLKXP: Transmit Clock Polarity; Transmit data driven on rising edge (0) / falling edge (1)

	// McBSPi.MCBSPLP_RCR1_REG
	hal_bitmask_set(mcbsp, MCBSPLP_RCR1_REG, BV(7) + BV(5)); // RWDLEN1: Receive Word Length: 32 Bit

	// McBSPi.MCBSPLP_RCR2_REG
	hal_bitmask_set(mcbsp, MCBSPLP_RCR2_REG, BV(7) + BV(5)); // RWDLEN2: Receive Word Length: 32 Bit

	// McBSPi.MCBSPLP_XCR1_REG
	hal_bitmask_set(mcbsp, MCBSPLP_XCR1_REG, BV(7) + BV(5)); // XWDLEN1: Transmit Word Length: 32 Bit

	// McBSPi.MCBSPLP_XCR2_REG
	hal_bitmask_set(mcbsp, MCBSPLP_XCR2_REG,
			BV(15) + 		// XPHASE: Transmit Phases; Single-phase frame (0) / Dual-phase frame (1)
			BV(7) + BV(5)	// XWDLEN2: Transmit Word Length: 32 Bit
			//BV(3) + 		// XREVERSE: Transmit Reverse Mode MSB (0) / LSB (1)
							// XDATLY: Transmit Data Delay
			);

	// McBSPi.MCBSPLP_SRGR1_REG
	//hal_bitmask_set(mcbsp, MCBSPLP_SRGR1_REG,);

	// McBSPi.MCBSPLP_SRGR2_REG
	hal_bitmask_set(mcbsp, MCBSPLP_SRGR2_REG,
			BV(13) 			// CLKSM: SRG Clock Mode
			//BV(12) + 		// FSGM: (used when FSXM is set) Sample Rate Generator Transmit Frame-Synchronization Mode
							// FPER: Frame Period
			);

	// Set CONTROL.CONTROL_PADCONF_<module>_<signal>[x] INPUTENABLE bits to 1
	// mcbsp2_fsx, mcbsp2_clkx
	hal_bitmask_set(CONTROL_PADCONF_MCBSP2_FSX, 0, BV(24) + BV(8));

	// mcbsp2_dr, mcbsp2_dx
	hal_bitmask_set(CONTROL_PADCONF_MCBSP2_DR, 0, BV(24) + BV(8));

	// Wait 2 SRG Clock Cycles (to ensure proper internal synchronization)
	int x = 0;
	for ( x = 0; x < 10; x++);

	// STEP 3
	// Set up data aquisition as required (such as writing to McBSPi.MCBSPLP_DXR_REG)

	// STEP 4
	// Set either McBSPi.MCBSPLP_SPCR1_REG[0] or McBSPi.MCBSPLP_SPCR2_REG[0] to 1
	hal_bitmask_set(mcbsp, MCBSPLP_SPCR2_REG, BV(0));

	// STEP 5
	// Is internally generated frame sync required?
	// Set McBSPi.MCBSPLP_SPCR2_REG[7] to 1
	hal_bitmask_set(mcbsp, MCBSPLP_SPCR2_REG, BV(7));

	// Wait 2 SRG Clock Cycles
	for ( x = 0; x < 10; x++);

	// Receiver and transmitter become active
}

