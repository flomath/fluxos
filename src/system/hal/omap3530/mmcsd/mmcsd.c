/*
 * mmcsd.c
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#include "mmcsd.h"

uint32_t cardMediaChanged = TRUE;
CARD_INFO cardInfo;

EFI_BLOCK_MEDIA mmchsMedia = {
	0, //1703, // wolfy media id
	true,
	false,

	false,
	false,
	false,

	512,
	0,

	0,
	0
};

/************************************/

uint32_t mmcsd_initialize() {
	uint32_t status;

	// configure interface and functional clocks
	// p. 3161
	hal_bitmask_set(CORE_CM, CM_ICLKEN1_CORE, BV(EN_MMC1));
	hal_bitmask_set(CORE_CM, CM_FCLKEN_CORE, BV(EN_MMC1));

	memset(&cardInfo, 0, sizeof(cardInfo));

	if (cardMediaChanged == TRUE) {
		status = mmcsd_card_detect();

		if (status != 0) {
			mmchsMedia.BlockSize = 512;
			mmchsMedia.MediaPresent = FALSE;
			mmchsMedia.LastBlock = 0;

			return status;
		} else {
			cardMediaChanged = FALSE;
			mmchsMedia.MediaPresent = TRUE;
		}
	}
	/*else if (mmchsMedia.MediaPresent == FALSE) {
		return MMCHS_NOMEDIA;
	}*/

	return MMCHS_SUCCESS;
}

uint32_t mmcsd_card_detect() {
	// p. 3161;

	// softreset mmc/sd controller
	// set softreset bit to 1
	hal_bitmask_write(MMCHS1, MMCHS_SYSCONFIG, BV(SOFTRESET), 32);
	// wait for reset done
	while ((hal_get_address_value(MMCHS1, MMCHS_SYSSTATUS) & BV(RESETDONE)) != 1);

	// Softreset MMC controller
	/*hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));
	uint32_t softResetMMC;
	do {
		// wait until it returns 0x0
		softResetMMC = BIT_TRIM_RIGHT(
				(hal_get_address_value(MMCHS1, MMCHS_SYSCONFIG) & BV(1)), 1);
	} while (softResetMMC != 0);*/

	// Softreset for all
	/*hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(24)); // set to 1
	uint32_t softResetAllDone;
	do {
		// wait until it returns 0x0
		softResetAllDone = BIT_TRIM_RIGHT(
				(hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(24)), 24);
	} while (softResetAllDone != 0);*/

	// set default capabilities [22.5.1.3]
	// all settings are for MMCHS1 [p. 3206]
	hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(VS30)); // Voltage Support 3.0V must be set to 1
	hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(VS18)); // Voltage Support 1.8V must be set to 1

	// Maximum current capabilities (p. 3208)
	//hal_bitmask_clear(MMCHS1, MMCHS_CUR_CAPA, 0xFF);

	// wakeup configuration [22.5.1.4]
	hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(ENAWAKEUP)); 	// enwakup
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(IWE)); 				// (IWE) wakeup event on sd card interrupt
	//hal_bitmask_set(MMCHS1, MMCHS_IE, BV(8));       			// enable card interrupt (sdio card only!)

	//TODO: check if interrupts are needed?

	// mmc host/bus configuration [22.5.1.5]
	// MMCHS_CON to configure specific data/cmd transfer
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(OD));			// set OD to 1
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(DW8)); 		// clear to 0 - set to 1 to 4 bit data
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(CEATA)); 	// clear to 0 - CEATA mode to standard

	// configure card voltage, pwr mode and data bus width
	hal_bitmask_write(MMCHS1, MMCHS_HCTL, 0x0, 32);		//TODO: clear needed?
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, SDVS_3V);		// 0x6 3V
	hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(SDBP));	// SDBP_OFF
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(DTW));		// DTW_1_BIT

	// Enable the input buffer of the clock output.
	// As a result, there is a loopback through the
	// output and input buffers. The goal is to
	// support the synchronization of the mmci_clk.
	// see page 777

	// enable internal clock by setting bit 0
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(ICE));

	// update clock
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY80);

	// SD bus power on (SDBP)
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(SDBP));

	// read back the SDBP bit (wait until sd bus power is on)
	while (BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_HCTL) & BV(SDBP)), SDBP) != 1);

	uint32_t status;
	// precard identification procedure (p. 3162)
	status = mmcsd_precard_identification();
	if (status != 0) {
		printf("Failed to identify card. try again\n");
		return status;
	}

	// read card specific data
	status = mmcsd_card_data();
	if (status != 0) {
		return status;
	}

	// configure card for transfer mode
	status = mmcsd_card_config();
	if (status != 0) {
		return status;
	}

	// config media data
	mmchsMedia.LastBlock    = (cardInfo.NumOfBlocks - 1);
	mmchsMedia.BlockSize 	= cardInfo.BlockSize;
	//mmchsMedia.ReadOnly     = (MmioRead32 (GPIO1_BASE + GPIO_DATAIN) & BIT23) == BIT23;
	mmchsMedia.MediaPresent = true;
	mmchsMedia.MediaId++;

	return MMCHS_SUCCESS;
}

void mmcsd_change_clockfrequency(uint32_t clockfrequency) {
	// see p. 3157
	// set CEN bit to 0x0 to not provide the clock to card
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, BV(CEN));

	// set clock divider
	// MMCi.MMCHS_SYSCTL[15:6]CLKD
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, (0x3FF << 6)); // clear bit 6 to 15
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, (clockfrequency));

	// wait until clk is set and stable
	while (BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(ICS)), ICS) != 1);

	// provide the clock to the card
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(CEN));
}

uint32_t mmcsd_precard_identification() {
	// p. 3147
	int cmdArg = 0;
	uint32_t cmdStatus;
	uint8_t cmd8Supported = 0;

	// activate all interrupts
	hal_bitmask_write(MMCHS1, MMCHS_IE, 0x0, 32);		//TODO: clear needed?
	hal_bitmask_set(MMCHS1, MMCHS_IE, BV(BADA_ENABLE) + BV(CERR_ENABLE) +
										BV(DEB_ENABLE) + BV(DCRC_ENABLE) +
										BV(DTO_ENABLE) + BV(CIE_ENABLE) +
										BV(CEB_ENABLE) + BV(CCRC_ENABLE) +
										BV(CTO_ENABLE) + BV(BRR_ENABLE) +
										BV(BWR_ENABLE) + BV(TC_ENABLE) +
										BV(CC_ENABLE));

	// begin initialization
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(INIT)); 	// send init sequence
	hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x0, 32); 	// send dummy command

	// wait for 1ms
	util_ksleep(1);

	// set 1 to clear command complete flag
	hal_bitmask_set(MMCHS1, MMCHS_STAT, BV(CC));

	// end initialization
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(INIT)); 	// stop sending init sequence
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, SDVS_3V);		// 0x6 3V
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(SDBP));		// SDBP_ON
	hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(DTW));		// DTW 1-bit

	// clear status register
	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);

	// change clock frequency to fit protocol
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY400);
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(OD)); // set OD

	// send cmd0
	cmdStatus = mmcsd_sendcmd(CMD0, 0x0);
	if (cmdStatus != 0) {
		return cmdStatus;
	}

	// send cmd5 (fails if no sdio card found)
	cmdStatus = mmcsd_sendcmd(CMD5, 0x0);
	if (cmdStatus == 0) {
		printf("found sdio card. not yet implemented!\n");
		return MMCHS_ERROR_UNSUPPORTED;
	}

	// Found SDIO-Card
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(SRC)); // set to 1
	uint32_t softResetDone;
	do {
		// wait until it returns 0x0
		softResetDone = BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(SRC)), SRC);
	} while (softResetDone != 0);

	// send cmd8
	uint32_t cmd8Arg = (0x0 << 12 | (0x1 << 8) | 0xCE << 0);
	cmdStatus = mmcsd_sendcmd(CMD8, cmd8Arg);
	if (cmdStatus == MMCHS_SUCCESS) {
		uint32_t value_rsp10 = hal_get_address_value(MMCHS1, MMCHS_RSP10);
		if (value_rsp10 != cmd8Arg) {
			printf("MMCSD: EFI Device error! Possible no card available?\n");
			return MMCHS_ERROR_DEVICE;
		}

		// supports sector mode, high capacity flag
		cmd8Supported = 1;
	} else {
		//printf("CMD8 fails. Not an sd2.0 card.\n");
	}

	// soft reset
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(SRC)); // set to 1
	do {
		// wait until it returns 0x0
		softResetDone = BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(SRC)), SRC);
	} while (softResetDone != 0);

	// do until card is not busy anymore
	int retryCount = 0;
	do {
		// send cmd55
		cmdStatus = mmcsd_sendcmd(CMD55, 0x0);
		if (cmdStatus == MMCHS_SUCCESS) {
			//printf("succesfully detected sdcard. cmd5 response: %x\n", response);
			cardInfo.CardType = SD_CARD;

			// send ACMD41 command
			uint32_t acmd41Arg = ((uint32_t *) &(cardInfo.OCRData))[0];
			if (cmd8Supported) {
				cmdArg |= BV(HCS);
			}

			uint32_t cmd41Status = mmcsd_sendcmd(ACMD41, acmd41Arg);
			if (cmd41Status != 0) {
				return cmd41Status;
			}

			((uint32_t *) &(cardInfo.OCRData))[0] = hal_get_address_value(MMCHS1, MMCHS_RSP10);
			//printf("SD card detected: OCR: %x\n", cardInfo.OCRData);
		} else {
			cardInfo.CardType = MMC_CARD;
			// todo: not supported yet
			printf("detected mmc card. not supported yet\n");
			return MMCHS_ERROR_UNSUPPORTED;
		}

		// verify that card is not busy
		if (cardInfo.OCRData.Busy == 1) {
			if (cmd8Supported) {
				cardInfo.CardType = SD_CARD_2;
			}

			// card is ready, check capacity modes
			if (cardInfo.OCRData.AccessMode & BV(1)) {
				cardInfo.CardType = SD_CARD_2_HIGH;
				printf("High capacity card\n");
			} else {
				printf("Standard capacity card\n");
			}

			// card is ready to use
			break;
		}

		retryCount++;
		util_ksleep(1);

	} while (retryCount < MAX_RETRY);

	// send cmd2 to gather information about how to read sd card
	cmdStatus = mmcsd_sendcmd(CMD2, 0x0);
	if (cmdStatus != 0) {
		return cmdStatus;
	}

	// parse card information
	mmcsd_card_parse_cid();

	// send cmd3
	cmdStatus = mmcsd_sendcmd(CMD3, 0x0);
	if (cmdStatus != 0) {
		return cmdStatus;
	}

	// get lower 16 bits
	cardInfo.RCA = (hal_get_address_value(MMCHS1, MMCHS_RSP10) >> 16);

	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(OD)); 		// OD
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, SDVS_3V);		// sdvs_3v
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY400);

	// TODO: handle mmc card
	// if mmc card -> more than one card connected? if only 1 or all are identified, go on then

	return MMCHS_SUCCESS;
}

uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg) {
	// see p.3151 resp. 3155
	// read/write transfer flow with polling and without DMA

	// until cmd lines are not in use anymore
	while (BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_PSTATE) & BV(CMDI)), CMDI) != 0);

	// Set Blocksize and number of Blocks
	hal_bitmask_write(MMCHS1, MMCHS_BLK, BLEN_512, 32); // 512 bytes (block length)
	//hal_bitmask_write(MMCHS1, MMCHS_BLK, BV(16)); // possible TODO: 0 no block transferred, 1 block (block count)

	// reset timeout counter
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, DTO_FULL);
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, DTO_27);

	// Set Bitfields in MMCHS_CON
	// set MIT, STR, but only for mmc cards (not gonna happen)

	// Clear status register
	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);

	// write MMCHS_CSRE if response type permits
	// set bit for error code card status
	//hal_bitmask_write(MMCHS1, MMCHS_CSRE, 0x00000000);

	// MMCHS_ARG default
	hal_bitmask_write(MMCHS1, MMCHS_ARG, arg, 32);

	// set ie + cmd
	// Write command
	// MMCHS_CMD and MMCHS_ISE
	switch(cmd) {
		case CMD0:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00040001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x00000000, 32);
			break;
		case CMD1:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00050001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x01020000, 32);
			break;
		case CMD2:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x02090000, 32);
			break;
		case CMD3:
			//TODO: check if 0x0 arg is really correct? -> 0x00010000
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x031a0000, 32);
			break;
		case CMD5:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00050001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x05020000, 32);
			break;
		case CMD7:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x071a0000, 32);
			break;
		case CMD8:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x81a0000, 32);
			break;
		case CMD9:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x09090000, 32);
			break;
		case CMD16:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x101a0000, 32);
			break;
		case CMD17:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x107F0013, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x111a0000, 32);
			break;
		case CMD24:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x107F0012, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x181a0000, 32);
			break;
		case CMD55:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x371a0000, 32);
			break;
		case ACMD6:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x06020000, 32);
			break;
		case ACMD41:
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x29020000, 32);
			break;
		default:
			printf("WARNING: tried to send unknown cmd to MMCSD\n");
			return MMCHS_ERROR;
	}

	int retryCount = 0;
	while (retryCount < MAX_RETRY) {
		// read MMCHS_STAT register
		uint32_t mmcStatus;
		do {
			mmcStatus = hal_get_address_value(MMCHS1, MMCHS_STAT);
		} while (mmcStatus == 0);

		// Ignore CRC errors on CMD2 and ACMD47, per relevant standards
		// https://code.google.com/p/beagleboard-freebsd/source/browse/trunk/sys/arm/cortexa8/omap3/omap3_mmc.c?r=15#774
		if ((cmd == CMD2) && (BIT_TRIM_RIGHT( (mmcStatus & BV(CCRC)), CCRC) != 0)) {
			mmcStatus &= ~(BV(CCRC));
			// clear ERRI if no other error is set
			if ((mmcStatus & (0xFFFF0000)) == 0) {
				mmcStatus &= ~(BV(ERRI));
			}
		}

		// check error interrupt
		if ( BIT_TRIM_RIGHT((mmcStatus & BV(CTO)), CTO) != 0 && BIT_TRIM_RIGHT((mmcStatus & BV(CTO)), CTO) != 1) {
			// Soft reset
			hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(SRC));
			uint32_t softResetDone;
			do {
				// wait until it returns 0x0
				softResetDone = BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(SRC)), SRC);
			} while (softResetDone != 0);

			printf("error sending command: (CTO: %i)\n", BIT_TRIM_RIGHT((mmcStatus & BV(CTO)), CTO) );
			return MMCHS_ERROR;
		}

		// check if cmd is completed (CC)
		if (BIT_TRIM_RIGHT((mmcStatus & BV(CC)), CC) == 1) {
			hal_bitmask_write(MMCHS1, MMCHS_STAT, BV(CC), 32);
			break;
		}

		retryCount++;
	}

	if (retryCount == MAX_RETRY) {
		//printf("MMCHS send command timeout\n");
		return MMCHS_ERROR_TIMEOUT;
	}

	return MMCHS_SUCCESS;
}

uint32_t mmcsd_card_data() {
	uint32_t CardSize;

	uint32_t arg = cardInfo.RCA << 16;
	uint32_t status = mmcsd_sendcmd(CMD9, arg);
	if (status != 0) {
		return status;
	}

	// Populate 128-bit card specific data.
	((uint32_t *) &(cardInfo.CSDData))[0] = hal_get_address_value(MMCHS1, MMCHS_RSP10);
	((uint32_t *) &(cardInfo.CSDData))[1] = hal_get_address_value(MMCHS1, MMCHS_RSP32);
	((uint32_t *) &(cardInfo.CSDData))[2] = hal_get_address_value(MMCHS1, MMCHS_RSP54);
	((uint32_t *) &(cardInfo.CSDData))[3] = hal_get_address_value(MMCHS1, MMCHS_RSP76);

	// get card configuration
	uint32_t BlockSize;
	uint32_t NumBlocks;
	uint32_t ClockFrequencySelect;

	//Calculate BlockSize and Total number of blocks in the detected card.
	CSD_SDV2 *CsdSDV2Data;
	if (cardInfo.CardType == SD_CARD_2_HIGH) {
		CsdSDV2Data = (CSD_SDV2 *) &cardInfo.CSDData;

		//Populate BlockSize.
		BlockSize = (0x1UL << CsdSDV2Data->READ_BL_LEN);

		//Calculate Total number of blocks.
		CardSize = CsdSDV2Data->C_SIZELow16 | (CsdSDV2Data->C_SIZEHigh6 << 2);
		NumBlocks = ((CardSize + 1) * 1024);
	} else {
		//Populate BlockSize.
		BlockSize = (0x1UL << cardInfo.CSDData.READ_BL_LEN);

		//Calculate Total number of blocks.
		CardSize = cardInfo.CSDData.C_SIZELow2 | (cardInfo.CSDData.C_SIZEHigh10 << 2);
		//TODO: right calculation?
		NumBlocks = (CardSize + 1) * (1 << (cardInfo.CSDData.C_SIZE_MULT + 2));
	}

	//For >=2G card, BlockSize may be 1K, but the transfer size is 512 bytes.
	if (BlockSize > 512) {
		// TODO
		//NumBlocks = MultU64x32(NumBlocks, BlockSize/2);
		BlockSize = 512;
	}

	cardInfo.BlockSize = BlockSize;
	cardInfo.NumOfBlocks = NumBlocks;

	//Calculate Card clock divider value.
	mmcsd_calculate_card_clk(&ClockFrequencySelect);
	cardInfo.ClockFrequencySelect = ClockFrequencySelect;

	return MMCHS_SUCCESS;
}

uint32_t mmcsd_card_config() {
	uint32_t arg = cardInfo.RCA << 16;
	uint32_t status = mmcsd_sendcmd(CMD7, arg);
	if (status != 0) {
		return status;
	}

	if (cardInfo.CardType != UNKNOWN_CARD && cardInfo.CardType != MMC_CARD) {
		//TODO: arg right?
		status = mmcsd_sendcmd(CMD55, arg);
		if (status == 0) {
			status = mmcsd_sendcmd(ACMD6, 0x2);
			if (status == 0) {
				// set controller to 4-bit
				hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(DTW));
			}
		}
	}

	// send cmd16 (block length)
	//TODO: check if blocksize == 0x200
	arg = cardInfo.BlockSize;
	status = mmcsd_sendcmd(CMD16, arg);
	if (status != 0) {
		return status;
	}

	mmcsd_change_clockfrequency(cardInfo.ClockFrequencySelect);
	return MMCHS_SUCCESS;
}

void mmcsd_card_parse_cid() {
	uint32_t r0 = hal_get_address_value(MMCHS1, MMCHS_RSP10);
	uint32_t r1 = hal_get_address_value(MMCHS1, MMCHS_RSP32);
	uint32_t r2 = hal_get_address_value(MMCHS1, MMCHS_RSP54);
	uint32_t r3 = hal_get_address_value(MMCHS1, MMCHS_RSP76);

	cardInfo.CIDData.MDT = ((r0 >> 8) & 0xFFF);
	cardInfo.CIDData.PSN = (((r0 >> 24) & 0xFF) | ((r1 & 0xFFFFFF) << 8));
	cardInfo.CIDData.PRV = ((r1 >> 24) & 0xFF);
	cardInfo.CIDData.PNM[4] = ((r2) & 0xFF);
	cardInfo.CIDData.PNM[3] = ((r2 >> 8) & 0xFF);
	cardInfo.CIDData.PNM[2] = ((r2 >> 16) & 0xFF);
	cardInfo.CIDData.PNM[1] = ((r2 >> 24) & 0xFF);
	cardInfo.CIDData.PNM[0] = ((r3) & 0xFF);
	cardInfo.CIDData.OID = ((r3 >> 8) & 0xFFFF);
	cardInfo.CIDData.MID = ((r3 >> 24) & 0xFF);
}

uint32_t mmcsd_read(uint32_t lba, void* buffer, size_t bufferSize) {
	return mmcsd_read_write(lba, buffer, bufferSize, MMCHS_READ);
}

uint32_t mmcsd_write(uint32_t lba, void* buffer, size_t bufferSize) {
	return mmcsd_read_write(lba, buffer, bufferSize, MMCHS_WRITE);
}

uint32_t mmcsd_read_write(uint32_t lba, void* buffer, size_t bufferSize, uint8_t operationType) {
	uint32_t status = MMCHS_SUCCESS;
	uint32_t BlockCount;
	uint32_t BytesToBeTranferedThisPass = 0;
	uint32_t BytesRemainingToBeTransfered;

	if (mmchsMedia.MediaPresent == FALSE) {
		return MMCHS_ERROR_NO_MEDIA;
	}

	// detect errors and invalid parameters
	if (buffer == NULL) {
		return MMCHS_ERROR_INVALID_PARAMETER;
	}
	if (lba > mmchsMedia.LastBlock) {
		return MMCHS_ERROR_INVALID_PARAMETER;
	}
	if ((bufferSize % mmchsMedia.BlockSize) != 0) {
		return MMCHS_ERROR_BAD_BUFFER_SIZE;
	}

	// datalines are not in use anymore?
	int retryCount = 0;
	while (retryCount++ < MAX_RETRY && (BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_PSTATE) & BV(DATI)), DATI) != 0));

	if (retryCount == MAX_RETRY) {
		return MMCHS_ERROR_TIMEOUT;
	}

	BytesRemainingToBeTransfered = bufferSize;
	while (BytesRemainingToBeTransfered > 0) {
		if (cardMediaChanged) {
			return MMCHS_ERROR_NO_MEDIA;
		}

		BytesToBeTranferedThisPass = mmchsMedia.BlockSize;
		BlockCount = BytesToBeTranferedThisPass / mmchsMedia.BlockSize;

		status = mmcsd_transfer_block(lba, buffer, operationType);
		if (status != 0) {
			return status;
		}

		BytesRemainingToBeTransfered -= BytesToBeTranferedThisPass;
		lba += BlockCount;
		buffer = (uint8_t *) buffer + mmchsMedia.BlockSize;
	}

	return status;
}

uint32_t mmcsd_transfer_block(uint32_t lba, void* buffer, uint8_t operationType) {
	uint32_t cmdStatus;
	uint32_t mmcStatus;
	uint32_t cmd = 0;
	uint32_t cmdInterruptEnable = 0;
	uint32_t cmdArgument = 0;

	switch (operationType) {
		// Single block read
		case MMCHS_READ:
			cmd = CMD17;
			break;
		// Single block Write
		case MMCHS_WRITE:
			cmd = CMD24;
			break;
	}

	// Set command argument based on the card access mode (Byte mode or Block mode)
	if (cardInfo.OCRData.AccessMode & (0x01 << 1)) {
		cmdArgument = lba;
	} else {
		cmdArgument = lba * mmchsMedia.BlockSize;
	}

	cmdStatus = mmcsd_sendcmd(cmd, cmdArgument);
	if (cmdStatus != 0) {
		return cmdStatus;
	}

	// Read or Write data.
	if (operationType == MMCHS_READ) {
		cmdStatus = mmcsd_read_block_data(buffer);
		if (cmdStatus != 0) {
			return cmdStatus;
		}
	} else if (operationType == MMCHS_WRITE) {
		//TODO
	}

	uint32_t retryCount = 0;
	while (retryCount < MAX_RETRY) {
		do {
			mmcStatus = hal_get_address_value(MMCHS1, MMCHS_STAT);
		} while (mmcStatus == 0);

		// If the transfer is complete (TC)
		if (BIT_TRIM_RIGHT( (mmcStatus & BV(TC)), TC) == 1) {
			break;
		}

		// Handle errors
		// DEB, DCRC, DTO
		if (BIT_TRIM_RIGHT((mmcStatus & BV(DEB)), DEB) == 1 ||
				BIT_TRIM_RIGHT((mmcStatus & BV(DCRC)), DCRC) == 1 ||
				BIT_TRIM_RIGHT((mmcStatus & BV(DTO)), DTO) == 1) {
			hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(SRD)); // SRD
			while (BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(SRD)), SRD) != 0); // SRD

			return MMCHS_ERROR_DEVICE;
		}
		retryCount++;
	}

	if (retryCount == MAX_RETRY) {
		return MMCHS_ERROR_TIMEOUT;
	}

	return MMCHS_SUCCESS;
}

uint32_t mmcsd_read_block_data(void *buffer)
{
	uint32_t mmcStatus;
	uint32_t* dataBuffer = buffer;
	uint32_t dataSize = mmchsMedia.BlockSize / 4;
	uint32_t count;
	uint32_t retryCount = 0;

	while (retryCount < MAX_RETRY)
	{
		do {
			mmcStatus = hal_get_address_value(MMCHS1, MMCHS_STAT);
		} while (mmcStatus == 0);

		// Wait till buffer is ready (BRR)
		if (BIT_TRIM_RIGHT((mmcStatus & BV(BRR)), BRR) == 1) {
			// clear ready
			hal_bitmask_set(MMCHS1, MMCHS_STAT, BV(BRR));

			// Read the data
			for (count = 0; count < dataSize; count++) {
				*dataBuffer++ = hal_get_address_value(MMCHS1, MMCHS_DATA);
			}
			break;
		}
		retryCount++;
	}

	if (retryCount == MAX_RETRY) {
		return MMCHS_ERROR_TIMEOUT;
	}

	return MMCHS_SUCCESS;
}

void mmcsd_calculate_card_clk(uint32_t *ClockFrequencySelect) {
	uint8_t MaxDataTransferRate;
	uint32_t TransferRateValue = 0;
	uint32_t TimeValue = 0;
	uint32_t Frequency = 0;

	MaxDataTransferRate = cardInfo.CSDData.TRAN_SPEED;

	// For SD Cards  we would need to send CMD6 to set
	// speeds abouve 25MHz. High Speed mode 50 MHz and up

	// Calculate Transfer rate unit (Bits 2:0 of TRAN_SPEED)
	switch (MaxDataTransferRate & 0x7) {
		case 0:
			TransferRateValue = 100 * 1000;
			break;
		case 1:
			TransferRateValue = 1 * 1000 * 1000;
			break;
		case 2:
			TransferRateValue = 10 * 1000 * 1000;
			break;
		case 3:
			TransferRateValue = 100 * 1000 * 1000;
			break;
		default:
			// invalid rate
			break;
	}

	// calculate Time value (Bits 6:3 of TRAN_SPEED)
	switch ((MaxDataTransferRate >> 3) & 0xF) {
		case 1:
			TimeValue = 10;
			break;
		case 2:
			TimeValue = 12;
			break;
		case 3:
			TimeValue = 13;
			break;
		case 4:
			TimeValue = 15;
			break;
		case 5:
			TimeValue = 20;
			break;
		case 6:
			TimeValue = 25;
			break;
		case 7:
			TimeValue = 30;
			break;
		case 8:
			TimeValue = 35;
			break;
		case 9:
			TimeValue = 40;
			break;
		case 10:
			TimeValue = 45;
			break;
		case 11:
			TimeValue = 50;
			break;
		case 12:
			TimeValue = 55;
			break;
		case 13:
			TimeValue = 60;
			break;
		case 14:
			TimeValue = 70;
			break;
		case 15:
			TimeValue = 80;
			break;
		default:
			// invalid transfer speed
			break;
	}

	Frequency = TransferRateValue * TimeValue / 10;

	//Calculate Clock divider value to program in MMCHS_SYSCTL[CLKD] field.
	*ClockFrequencySelect = ((MMC_REFERENCE_CLK / Frequency) + 1);
}
