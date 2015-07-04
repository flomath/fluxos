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
#include <string.h>
#include <stdbool.h>

#define FALSE		0
#define TRUE		1

#define MMCHS1      0x4809C000
#define MMCHS2      0x480B4000
#define MMCHS3      0x480AD000

typedef volatile unsigned int * memory_mapped_io_t;
#define MMCHS_REG(reg) (*((memory_mapped_io_t)(MMCHS1 + reg)))

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
#define MMCHS_NOMEDIA	10
#define MMCHS_INVALIDPARAM	11
#define MMCHS_BADBFRSIZE	12
#define MMCHS_TIMEOUT	13

/**
 * configure functional clocks
 */
uint32_t mmcsd_initialize();

/**
 * initialize mmc/sd controller
 */
uint32_t mmcsd_card_detect();

/**
 * soft-reset the mmc/sd controller
 */
void mmcsd_softreset();

/**
 * parse card CID info
 */
void mmcsd_card_parse_cid();

/**
 * pre card identifaction procedure
 */
uint32_t mmcsd_precard_identification();

/**
 * get card specific data
 */
uint32_t mmcsd_card_data();

/**
 * configure card
 */
void mmcsd_card_config();

/**
 * send a command
 * a command starts an operation (serially transfered from sd host ctrl to the card on the mmci_cmd line)
 */
//uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg);
uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg, uint32_t ie);

/**
 * change the mmc/sd output clock frequency
 */
void mmcsd_change_clockfrequency(uint32_t clockfrequency);

void mmcsd_calculate_card_clk (uint32_t *ClockFrequencySelect);

uint32_t mmcsd_read_write(uint32_t lba, void* buffer, size_t bufferSize, uint8_t operationType);

uint32_t mmcsd_transfer_block(uint32_t lba, void* buffer, uint8_t operationType);

uint32_t mmcsd_read_block_data(void *buffer);

uint32_t mmcsd_write(uint32_t lba, void* buffer, size_t bufferSize);

uint32_t mmcsd_read(uint32_t lba, void* buffer, size_t bufferSize);

/*
 * following definitions (defines only) by apple inc.
 * https://github.com/vathpela/edk2/
 */

#define CMDI_MASK         (0x01 << 0)
#define CMDI_ALLOWED      (0x0UL << 0)
#define CMDI_NOT_ALLOWED  (0x01 << 0)
#define DATI_MASK         (0x01 << 1)
#define DATI_ALLOWED      (0x0UL << 1)
#define DATI_NOT_ALLOWED  (0x01 << 1)
#define BLEN_512BYTES     (0x200UL << 0)
#define DTO_MASK          (0xFUL << 16)
#define DTO_VAL           (0xEUL << 16)
#define CCRC              (0x01 << 17)
#define ERRI              (0x01 << 15)
#define SRC_MASK          (0x01 << 25)
#define SRC               (0x01 << 25)
#define CTO               (0x01 << 16)
#define CC                (0x01 << 0)
#define TC                (0x01 << 1)
#define BWR               (0x01 << 4)
#define BRR               (0x01 << 5)
#define ERRI              (0x01 << 15)
#define CTO               (0x01 << 16)
#define CCRC              (0x01 << 17)
#define DTO               (0x01 << 20)
#define DCRC              (0x01 << 21)
#define DEB               (0x01 << 22)

#define MMCHS_ERROR_DEVICE            3
#define MMCHS_ERROR_UNSUPPORTED       4
#define MMCHS_ERROR_INVALID           5
#define MMCHS_ERROR_NO_MEDIA          6
#define MMCHS_ERROR_INVALID_PARAMETER 7
#define MMCHS_ERROR_BAD_BUFFER_SIZE   8
#define MMCHS_ERROR_MEDIA_CHANGED     9

#define MMCHS_STATUS_SUCCESS     0
#define MMCHS_STATUS_LOAD_ERROR  1
#define MMCHS_STATUS_TIMEOUT     2

#define MMCHS_READ 0
#define MMCHS_WRITE 1

#define MMC_REFERENCE_CLK (96000000)
#define CC_EN             (0x01 << 0)
#define TC_EN             (0x01 << 1)
#define BWR_EN            (0x01 << 4)
#define BRR_EN            (0x01 << 5)
#define CTO_EN            (0x01 << 16)
#define CCRC_EN           (0x01 << 17)
#define CEB_EN            (0x01 << 18)
#define CIE_EN            (0x01 << 19)
#define DTO_EN            (0x01 << 20)
#define DCRC_EN           (0x01 << 21)
#define DEB_EN            (0x01 << 22)
#define CERR_EN           (0x01 << 28)
#define BADA_EN           (0x01 << 29)
#define SRD               (0x01 << 26)

#define DE_ENABLE         (0x01 << 0)
 #define BCE_ENABLE        (0x01 << 1)
 #define ACEN_ENABLE       (0x01 << 2)
 #define DDIR_READ         (0x01 << 4)
 #define DDIR_WRITE        (0x0UL << 4)
 #define MSBS_SGLEBLK      (0x0UL << 5)
 #define MSBS_MULTBLK      (0x01 << 5)
 #define RSP_TYPE_MASK     (0x3UL << 16)
 #define RSP_TYPE_136BITS  (0x01 << 16)
 #define RSP_TYPE_48BITS   (0x2UL << 16)
 #define CCCE_ENABLE       (0x01 << 19)
 #define CICE_ENABLE       (0x01 << 20)
 #define DP_ENABLE         (0x01 << 21)

//#define CMD8_ARG          (0x0 << 12 | (0x01 << 8) | 0xCE << 0) // bit8
#define INDX(CMD_INDX)    ((CMD_INDX & 0x3F) << 24)

#define CMD0              INDX(0)
#define CMD0_INT_EN       (CC_EN | CEB_EN)

#define CMD1              (INDX(1) | RSP_TYPE_48BITS)
#define CMD1_INT_EN       (CC_EN | CEB_EN | CTO_EN)

#define CMD2              (INDX(2) | CCCE_ENABLE | RSP_TYPE_136BITS)
#define CMD2_INT_EN       (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD3              (INDX(3) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD3_INT_EN       (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD5              (INDX(5) | RSP_TYPE_48BITS)
#define CMD5_INT_EN       (CC_EN | CEB_EN | CTO_EN)

#define CMD7              (INDX(7) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD7_INT_EN       (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD8              (INDX(8) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD8_INT_EN       (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)
#define CMD8_ARG          (0x0UL << 12 | (0x01 << 8) | 0xCEUL << 0)

#define CMD9              (INDX(9) | CCCE_ENABLE | RSP_TYPE_136BITS)
#define CMD9_INT_EN       (CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD16             (INDX(16) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD16_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD17             (INDX(17) | DP_ENABLE | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS | DDIR_READ)
#define CMD17_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | TC_EN | BRR_EN | CTO_EN | DTO_EN | DCRC_EN | DEB_EN | CEB_EN)

#define CMD18             (INDX(18) | DP_ENABLE | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS | MSBS_MULTBLK | DDIR_READ | BCE_ENABLE | DE_ENABLE)
#define CMD18_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | TC_EN | BRR_EN | CTO_EN | DTO_EN | DCRC_EN | DEB_EN | CEB_EN)

#define CMD23             (INDX(23) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD23_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define CMD24             (INDX(24) | DP_ENABLE | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS | DDIR_WRITE)
#define CMD24_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | TC_EN | BWR_EN | CTO_EN | DTO_EN | DCRC_EN | DEB_EN | CEB_EN)

#define CMD25             (INDX(25) | DP_ENABLE | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS | MSBS_MULTBLK | DDIR_READ | BCE_ENABLE | DE_ENABLE)
#define CMD25_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | TC_EN | BRR_EN | CTO_EN | DTO_EN | DCRC_EN | DEB_EN | CEB_EN)

#define CMD55             (INDX(55) | CICE_ENABLE | CCCE_ENABLE | RSP_TYPE_48BITS)
#define CMD55_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define ACMD41            (INDX(41) | RSP_TYPE_48BITS)
#define ACMD41_INT_EN     (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define ACMD6             (INDX(6) | RSP_TYPE_48BITS)
#define ACMD6_INT_EN      (CERR_EN | CIE_EN | CCRC_EN | CC_EN | CEB_EN | CTO_EN)

#define HCS (0x01<< 30)



uint32_t transferBlock(int lba, void* buffer, uint8_t operationType);
uint32_t readWrite(int lba, void* buffer, size_t bufferSize, uint8_t operationType);
uint32_t readBlockData(void* buffer);
uint32_t sendCommand(int cmd, int cmdInterrupts, int arg);


typedef enum {
  UNKNOWN_CARD,
  MMC_CARD,              //MMC card
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

  uint8_t   TRAN_SPEED: 8          ;  // Max. bus clock frequency [103:96]
  uint8_t   NSAC     : 8           ;  // Data read access-time 2 in CLK cycles (NSAC*100) [111:104]
  uint8_t   TAAC    : 8            ;  // Data read access-time 1 [119:112]

  uint8_t   RESERVED_5:         6; // Reserved [125:120]
  uint8_t   CSD_STRUCTURE:      2; // CSD structure [127:126]
} CSD;

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
	uint16_t  WP_GRP_SIZE:        7; // Write protect group size [38:32]
  uint16_t  SECTOR_SIZE:        7; // Erase sector size [45:39]
  uint16_t  ERASE_BLK_EN:       1; // Erase single block enable [46:46]
  uint16_t  RESERVED_4:         1; // Reserved [47:47]
  uint32_t  C_SIZELow16:        16;// Device size [69:48]
  uint32_t  C_SIZEHigh6:        6; // Device size [69:48]
  uint32_t  RESERVED_5:         6; // Reserved [75:70]
  uint32_t  DSR_IMP:            1; // DSR implemented [76:76]
  uint32_t  READ_BLK_MISALIGN:  1; // Read block misalignment [77:77]
  uint32_t  WRITE_BLK_MISALIGN: 1; // Write block misalignment [78:78]
  uint32_t  READ_BL_PARTIAL:    1; // Partial blocks for read allowed [79:79]
  uint16_t  READ_BL_LEN:        4; // Max. read data block length [83:80]
  uint16_t  CCC:                12;// Card command classes [95:84]
  uint8_t   TRAN_SPEED          ;  // Max. bus clock frequency [103:96]
  uint8_t   NSAC                ;  // Data read access-time 2 in CLK cycles (NSAC*100) [111:104]
  uint8_t   TAAC                ;  // Data read access-time 1 [119:112]
  uint8_t   RESERVED_6:         6; // 0 [125:120]
  uint8_t   CSD_STRUCTURE:      2; // CSD structure [127:126]
}CSD_SDV2;

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
  uint64_t	LastBlock;

  //EFI_LBA LowestAlignedLba;
  uint32_t  LogicalBlocksPerPhysicalBlock;
} EFI_BLOCK_MEDIA;

#endif /* SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_ */
