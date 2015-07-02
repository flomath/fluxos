/*
 * mmcsd.h
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_

#include "../../common/hal.h"
#include "../clock/clock.h"
#include "../../../../utils/sleep.h"
#include <stdio.h>
#include <stdbool.h>

#define MMCHS1      0x4809C000
#define MMCHS2      0x480B4000
#define MMCHS3      0x480AD000

#define MMCHS_SYSCONFIG     0x010
#define MMCHS_SYSSTATUS     0x014
#define MMCHS_CSRE          0x024
#define MMCHS_SYSTEST       0x028
#define MMCHS_CON           0x02C
#define MMCHS_PWCNT         0x030
#define MMCHS_SDMASA        0x100
#define MMCHS_BLK           0x104
#define MMCHS_ARG           0x108
#define MMCHS_CMD           0x10C
#define MMCHS_DATA          0x120
#define MMCHS_PSTATE        0x124
#define MMCHS_HCTL          0x128
#define MMCHS_SYSCTL        0x12C
#define MMCHS_STAT          0x130
#define MMCHS_IE            0x134
#define MMCHS_ISE           0x138
#define MMCHS_AC12          0x13C
#define MMCHS_CAPA          0x140
#define MMCHS_CUR_CAPA      0x148
#define MMCHS_FE            0x150
#define MMCHS_ADMAES        0x154
#define MMCHS_ADMASAL       0x158
#define MMCHS_REV           0x1FC

#define MMCHS_RSP(n)        (0x210 + (n * 4))
#define MMCHS_RSP10         0x110
#define MMCHS_RSP32         0x114
#define MMCHS_RSP54         0x118
#define MMCHS_RSP76         0x11C

#define CONTROL_PADCONF_MMC1_CLK	0x48002144	// mmc1_clk and mmc1_cmd
#define CONTROL_PADCONF_MMC1_DAT0	0x48002148
#define CONTROL_PADCONF_MMC1_DAT1	0x48002148
#define CONTROL_PADCONF_MMC1_DAT2	0x4800214C
#define CONTROL_PADCONF_MMC1_DAT4	0x48002150
#define CONTROL_PADCONF_MMC1_DAT6	0x48002154

#define MMCHS_CLK_FRQCY400		0xF0	// 400 kHz
#define MMCHS_CLK_FRQCY80		0x258	// 80 kHz

#define MMCHS_CMD0      0
#define MMCHS_CMD2      2
#define MMCHS_CMD3      3
#define MMCHS_CMD5      5
#define MMCHS_CMD6      6
#define MMCHS_CMD7      7
#define MMCHS_CMD8      8
#define MMCHS_CMD9      9
#define MMCHS_CMD16     16
#define MMCHS_CMD55     55
#define MMCHS_CMD41     41

#define MAX_RETRY		500
// (BADA_EN | CERR_EN | DEB_EN | DCRC_EN | DTO_EN | CIE_EN | CEB_EN | CCRC_EN | CTO_EN | BRR_EN | BWR_EN | TC_EN | CC_EN)
#define MMCHS_IDENTIFY	((0x01 << 29) | (0x01 << 28) |  (0x01 << 22) | (0x01 << 21) | (0x01 << 20) | (0x01 << 19) | (0x01 << 18) | (0x01 << 17) | (0x01 << 16) | (0x01 << 5) | (0x01 << 4) | (0x01 << 1) | (0x01 << 0))

#define MMCHS_SUCCESS	0
#define MMCHS_ERROR		1

/**
 * initialize mmc/sd controller
 */
void mmcsd_card_detect();

/**
 * soft-reset the mmc/sd controller
 */
void mmcsd_softreset();

/**
 * pre card identifaction procedure
 */
uint32_t mmcsd_precard_identification();

/**
 * get card specific data
 */
void mmcsd_card_data();

/**
 * configure card
 */
void mmcsd_card_config();

/**
 * send a command
 * a command starts an operation (serially transfered from sd host ctrl to the card on the mmci_cmd line)
 */
uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg);

/**
 * change the mmc/sd output clock frequency
 */
void mmcsd_change_clockfrequency(uint32_t clockfrequency);

/*
 * following definitions (defines only) by apple inc.
 * https://github.com/vathpela/edk2/
 */
#define CMD8_ARG          (0x0UL << 12 | 0x00000100 | 0xCEUL << 0) // bit8
#define INDX(CMD_INDX)    ((CMD_INDX & 0x3F) << 24)
#define ACMD41            (INDX(41) | (0x2UL << 16)) // RSP_TYPE_48BITS
#define ACMD41_INT_EN     (0x10000000 | 0x00080000 | 0x00020000 | 0x00000001 | 0x00040000 | 0x00010000) // CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN
#define ACMD6             (INDX(6) | (0x2UL << 16)) // RSP_TYPE_48BITS
#define ACMD6_INT_EN      (0x10000000 | 0x00080000 | 0x00020000 | 0x00000001 | 0x00040000 | 0x00010000)

#define HCS (0x01<< 30)

typedef enum {
  UNKNOWN_CARD,
  MMC_CARD,              //MMC card
  MMC_CARD_HIGH,         //MMC Card with High capacity
  EMMC_CARD,             //eMMC 4.41 card
  SD_CARD,               //SD 1.1 card
  SD_CARD_2,             //SD 2.0 or above standard card
  SD_CARD_2_HIGH         //SD 2.0 or above high capacity card
} CARD_TYPE;

typedef struct {
  uint32_t  Reserved0:   7; // 0
  uint32_t  V170_V195:   1; // 1.70V - 1.95V
  uint32_t  V200_V260:   7; // 2.00V - 2.60V
  uint32_t  V270_V360:   9; // 2.70V - 3.60V
  uint32_t  RESERVED_1:  5; // Reserved
  uint32_t  AccessMode:  2; // 00b (byte mode), 10b (sector mode)
  //uint32_t  PowerUp:     1; // This bit is set to LOW if the card has not finished the power up routine
  uint32_t  Busy:        1; // This bit is set to LOW if the card has not finished the power up routine
} OCR;

typedef struct {
  uint32_t  NOT_USED;   // 1 [0:0]
  uint32_t  CRC;        // CRC7 checksum [7:1]
  uint32_t  MDT;        // Manufacturing date [19:8]
  uint32_t  RESERVED_1; // Reserved [23:20]
  uint32_t  PSN;        // Product serial number [55:24]
  uint8_t   PRV;        // Product revision [63:56]
  uint8_t   PNM[5];     // Product name [64:103]
  uint16_t  OID;        // OEM/Application ID [119:104]
  uint8_t   MID;        // Manufacturer ID [127:120]
} CID;

typedef struct {
  uint8_t   NOT_USED:           1; // Not used, always 1 [0:0]
  uint8_t   CRC:                7; // CRC [7:1]

  uint8_t   RESERVED_1:         2; // Reserved [9:8]
  uint8_t   FILE_FORMAT:        2; // File format [11:10]
  uint8_t   TMP_WRITE_PROTECT:  1; // Temporary write protection [12:12]
  uint8_t   PERM_WRITE_PROTECT: 1; // Permanent write protection [13:13]
  uint8_t   COPY:               1; // Copy flag (OTP) [14:14]
  uint8_t   FILE_FORMAT_GRP:    1; // File format group [15:15]

  uint16_t  RESERVED_2:         5; // Reserved [20:16]
  uint16_t  WRITE_BL_PARTIAL:   1; // Partial blocks for write allowed [21:21]
  uint16_t  WRITE_BL_LEN:       4; // Max. write data block length [25:22]
  uint16_t  R2W_FACTOR:         3; // Write speed factor [28:26]
  uint16_t  RESERVED_3:         2; // Reserved [30:29]
  uint16_t  WP_GRP_ENABLE:      1; // Write protect group enable [31:31]

  uint32_t  WP_GRP_SIZE:        7; // Write protect group size [38:32]
  uint32_t  SECTOR_SIZE:        7; // Erase sector size [45:39]
  uint32_t  ERASE_BLK_EN:       1; // Erase single block enable [46:46]
  uint32_t  C_SIZE_MULT:        3; // Device size multiplier [49:47]
  uint32_t  VDD_W_CURR_MAX:     3; // Max. write current @ VDD max [52:50]
  uint32_t  VDD_W_CURR_MIN:     3; // Max. write current @ VDD min [55:53]
  uint32_t  VDD_R_CURR_MAX:     3; // Max. read current @ VDD max [58:56]
  uint32_t  VDD_R_CURR_MIN:     3; // Max. read current @ VDD min [61:59]
  uint32_t  C_SIZELow2:         2; // Device size [63:62]

  uint32_t  C_SIZEHigh10:       10;// Device size [73:64]
  uint32_t  RESERVED_4:         2; // Reserved [75:74]
  uint32_t  DSR_IMP:            1; // DSR implemented [76:76]
  uint32_t  READ_BLK_MISALIGN:  1; // Read block misalignment [77:77]
  uint32_t  WRITE_BLK_MISALIGN: 1; // Write block misalignment [78:78]
  uint32_t  READ_BL_PARTIAL:    1; // Partial blocks for read allowed [79:79]
  uint32_t  READ_BL_LEN:        4; // Max. read data block length [83:80]
  uint32_t  CCC:                12;// Card command classes [95:84]

  uint8_t   TRAN_SPEED          ;  // Max. bus clock frequency [103:96]
  uint8_t   NSAC                ;  // Data read access-time 2 in CLK cycles (NSAC*100) [111:104]
  uint8_t   TAAC                ;  // Data read access-time 1 [119:112]

  uint8_t   RESERVED_5:         6; // Reserved [125:120]
  uint8_t   CSD_STRUCTURE:      2; // CSD structure [127:126]
} CSD;

typedef struct  {
  uint16_t  RCA;
  uint32_t  BlockSize;
  uint32_t  NumOfBlocks;
  uint32_t  ClockFrequencySelect;
  CARD_TYPE CardType;
  OCR       OCRData;
  CID       CIDData;
  CSD       CSDData;
} CARD_INFO;

typedef struct {
  uint32_t  MediaId;
  bool    	RemovableMedia;
  bool 		MediaPresent;

  bool 		LogicalPartition;
  bool 		ReadOnly;
  bool 		WriteCaching;

  uint32_t  BlockSize;
  uint32_t  IoAlign;

  //EFI_LBA LastBlock;

  //EFI_LBA LowestAlignedLba;
  uint32_t  LogicalBlocksPerPhysicalBlock;
} EFI_BLOCK_MEDIA;

#endif /* SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_ */
