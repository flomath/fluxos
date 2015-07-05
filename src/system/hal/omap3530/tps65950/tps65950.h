/*
 * tps65950.h
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 *
 *	TPS65950 OMAP Power Management and System Companion Device Silicon Revision 1.2 (version G)
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS65950_H_
#define SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS65950_H_

#include "../../../system.h"

/**
 * setup audio on TPS
 */
void tps_audio_setup(void);

void tps_audio_volume_down();

void tps_audio_volume_up();

/**
 * write to i2c
 */
void tps_i2c_set(uint32_t address, uint32_t value);

// defines

#define SCD_AUDIO_VOICE		0x49

#define TPS_ARXR2_APGA_CTL 	0x0000001C
#define TPS_ARXR2PGA		0x00000012
#define TPS_AUDIO_IF 		0x0000000E
#define TPS_BOOST_CTL 		0x0000002E
#define TPS_HS_POPN_SET		0x00000024
#define TPS_ATXL1PGA 		0x0000000A
#define TPS_ATXR1PGA 		0x0000000B
#define TPS_AVADC_CTL 		0x00000007
#define TPS_ADCMICSEL 		0x00000008
#define TPS_ANAMICL 		0x00000005
#define TPS_ANAMICR 		0x00000006
#define TPS_ANAMIC_GAIN 	0x00000048

#define TPS_CODEC_MODE 		0x00000001
#define APPL_RATE_8			(0x0 << 4)
#define APPL_RATE_48 		(0xA << 4)
#define CODEECPDZ_ON 		(0x1 << 1)	//< Codec is on
#define OPT_MODE			1			//< 2 RX and TX stereo audio paths

#define TPS_OPTION 			0x00000002
#define ARXR2_EN 			(0x1 << 7)
#define ARXL2_EN 			(0x1 << 6)
#define ARXR1_EN 			(0x1 << 5)
#define ARXL1_VRX_EN 		(0x1 << 4)
#define ATXR2_VTXR_EN 		(0x1 << 3)
#define ATXL2_VTXL_EN 		(0x1 << 2)
#define ATXR1_EN 			(0x1 << 1)
#define ATXL1_EN 			(0x1 << 0)

#define TPS_RX_PATH_SEL 		0x00000043
#define RXL1_SEL 			(0x0 << 2)
#define RXR1_SEL 			(0x0 << 0)

#define TPS_APLL_CTL 		0x0000003A
#define APLL_EN 			(0x1 << 4)
#define APLL_INFREQ_26MHZ 	6

#define TPS_AVDAC_CTL 		0x00000017
#define ADACL2_EN 			(0x1 << 3)
#define ADACR2_EN 			(0x1 << 2)

#define TPS_ARXL2_APGA_CTL 	0x0000001B
#define B_ARX1_GAIN_SET 	3
#define ARXL2_GAIN_SET_MAX 	0xf
#define ARXL2_FM_EN 		(0x1 << 2)
#define ARXL2_DA_EN 		(0x1 << 1)
#define ARXL2_PDZ 			(0x1 << 0)
#define ARXL2_GAIN_0DB		(0x06 << 3)	// 0 dB gain

#define TPS_ARXL2PGA 		0x00000013
#define	TPS_ARXL2PGA_GAIN0DB	(0x30 | (3 << 6))	// 0dB gain

#define TPS_HS_GAIN_SET 		0x00000023

#define TPS_HS_SEL 			0x00000022
#define HSOR_AR2_EN 		(0x1 << 5)
#define HSOL_AL2_EN 		(0x1 << 2)

#define DATA_WIDTH_16 		(0x0 << 5)
#define AIF_FORMAT_CODEC 	(0x0 << 3)
#define AIF_EN 				(0x1 << 0)
#define VMID_EN 			(0x1 << 6)

#define ADCL_EN 			(0x1<<3)
#define ADCR_EN 			(0x1<<1)

#define CNCL_OFFSET_START 		(0x1<<7)
#define OFFSET_CNCL_SEL_RXALL 	(0x3<<5)
#define AUXL_EN 				(0x1<<2)

#define TPS_SOFTVOL_CTL		0x0000002F
#define	SOFTVOL_CTL_0		(0x1 << 5)
#define	SOFTVOL_CTL_7		(0x7 << 5)

#define AUXR_EN				(0x1<<2)

#endif /* SRC_SYSTEM_HAL_OMAP3530_TPS65950_TPS65950_H_ */
