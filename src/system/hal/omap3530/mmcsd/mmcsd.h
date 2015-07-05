/*
 * mmcsd.h
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_

#include "../../../system.h"
#include "../clock/clock.h"
#include "../../../../utils/sleep.h"

#define FALSE		        0
#define TRUE		        1
#define MAX_RETRY		    5000

/**
 * Functional clocks
 */
#define EN_MMC1             24
#define EN_MMC2             25
#define EN_MMC3             30

/**
 * Status codes (success + error)
 */
#define MMCHS_SUCCESS                 0
#define MMCHS_ERROR_LOAD              1
#define MMCHS_ERROR_TIMEOUT           2
#define MMCHS_ERROR_DEVICE            3
#define MMCHS_ERROR_UNSUPPORTED       4
#define MMCHS_ERROR_INVALID           5
#define MMCHS_ERROR_NO_MEDIA          6
#define MMCHS_ERROR_INVALID_PARAMETER 7
#define MMCHS_ERROR_BAD_BUFFER_SIZE   8
#define MMCHS_ERROR_MEDIA_CHANGED     9

/**
 * Actions
 */
#define MMCHS_READ 0
#define MMCHS_WRITE 1

/**
 * Different MultiMediaCard
 */
#define MMCHS1              0x4809C000
#define MMCHS2              0x480B4000
#define MMCHS3              0x480AD000

/**
 * System Configuration Register
 */
#define MMCHS_SYSCONFIG     0x010
#define AUTOIDLE            0         ///< Internal Clock gating strategy
#define SOFTRESET           1         ///< Software reset - During reset, it always returns 0
#define ENAWAKEUP           2         ///< Wake-up feature control

/**
 * System Status Register
 */
#define MMCHS_SYSSTATUS     0x014
#define RESETDONE           0         ///< Internal Reset Monitoring - 0x1 -> Reset completed

/**
 * Configuration register
 */
#define MMCHS_CON           0x02C
#define OD                  0         ///< Card open drain mode
#define INIT                1         ///< Send initialization stream - When this bit is set to 1, and the card is idle, an initialization sequence is sent to the card
#define DW8                 5         ///< 0x0 1-bit or 4-bit Data width - 0x1 8-bit Data width
#define CEATA               12        ///< CE-ATA control mode - 0x0 Standard MMC/SD/SDIO mode

/**
 * Transfer Length Configuration register
 */
#define MMCHS_BLK           0x104
#define BLEN                0         ///< Transfer Block Size
#define BLEN_512            (0x200 << BLEN)

/**
 * Command argument Register
 */
#define MMCHS_ARG           0x108
#define HCS                 30

/**
 * Command and transfer mode register
 */
#define MMCHS_CMD           0x10C
#define DE_ENABLE           0         ///< DMA Enable
#define BCE_ENABLE          1         ///< Block Count Enable
#define DDIR                4         ///< Data transfer Direction - 0x0 Data Write - 0x1 Data Read
#define MSBS                5         ///< Multi/Single block select - 0x0 Single - 0x1 Multi
#define RSP_TYPE            16        ///< Response type with length
#define RSP_TYPE_136BITS    (0x1 << RSP_TYPE)
#define RSP_TYPE_48BITS     (0x2 << RSP_TYPE)
#define CCCE_ENABLE         19        ///< Command CRC check enable
#define CICE_ENABLE         20        ///< Command Index check enable
#define DP_ENABLE           21        ///< Data present select
#define CMD_TYPE            22        ///< Command type
#define INDX                24        ///< Command index Binary encoded value from 0 to 63 specifying the command number send to card
#define INDX_FULL           (0x3F << INDX)

/**
 * Data Register
 */
#define MMCHS_DATA          0x120

/**
 * Present state register
 */
#define MMCHS_PSTATE        0x124
#define CMDI                0         ///< Command inhibit(mmci_cmd) - 0x0 allowed - 0x1 not allowed
#define DATI                1         ///< Command inhibit(mmci_dat) - 0x0 allowed - 0x1 not allowed

/**
 * Control register
 */
#define MMCHS_HCTL          0x128
#define DTW                 1         ///< Data transfer width - 0x0 1bit data - 0x1 4bit data
#define SDBP                8         ///< SD bus power
#define SDVS                9         ///< SD bus voltage select
#define SDVS_3V             (0x6 << SDVS)
#define IWE                 24        ///< Wake-up event enable on SD card interrupt

/**
 * SD system control register
 */
#define MMCHS_SYSCTL        0x12C
#define ICE                 0         ///< Internal clock enable
#define ICS                 1         ///< Internal clock stable
#define CEN                 2         ///< Clock enable
#define CLKD                6         ///< Clock frequency select
#define MMCHS_CLK_FRQCY80	(0x258 << CLKD) ///< 80 kHz
#define MMCHS_CLK_FRQCY400	(0xF0  << CLKD) ///< 400 kHz
#define DTO_COUNTER         16        ///< Data timeout counter value and busy timeout
#define DTO_FULL            (0xF << DTO_COUNTER)
#define DTO_27              (0xE << DTO_COUNTER) ///< TCF x 2^27
#define SRC                 25        ///< Software reset for mmci_cmd line. This bit is set to 1 for reset and released RW to 0 when completed
#define SRD                 26        ///< Software reset for mmci_dat line. This bit is set to 1 for reset and released RW to 0 when completed
#define MMC_REFERENCE_CLK   (96000000)

/**
 * Interrupt status register
 */
#define MMCHS_STAT          0x130
#define CC                  0         ///< Command complete - Read 0x1 command complete - Write 0x1 status is cleared
#define TC                  1         ///< Transfer completed - Read 0x1 transfer complete - Write 0x1 status is cleared
#define BRR                 5         ///< Buffer read ready - Read 0x1 ready to read buffer - Write 0x1 status is cleared
#define ERRI                15        ///< Error Interrupt
#define CTO                 16        ///< Command Timeout Error - Read 0x1 time out - Write 0x1 status is cleared
#define CCRC                17        ///< Command CRC Error - Read 0x1 crc error - Write 0x1 status is cleared
#define DTO                 20        ///< Data timeout error - Read 0x1 Time out - Write 0x1 status is cleared
#define DCRC                21        ///< Data CRC Error - Read 0x1 Data CRC error - Write 0x1 status is cleared
#define DEB                 22        ///< Data End Bit error - Read 0x1 Data end bit error - Write 0x1 status is cleared

/**
 * Interrupt SD enable register
 */
#define MMCHS_IE            0x134
#define CC_ENABLE           0         ///< Command completed Interrupt Enable
#define TC_ENABLE           1         ///< Transfer completed Interrupt Enable
#define BWR_ENABLE          4         ///< Buffer Write Ready Interrupt Enable
#define BRR_ENABLE          5         ///< Buffer Read Ready Interrupt Enable
#define CTO_ENABLE          16        ///< Command timeout error Interrupt Enable
#define CCRC_ENABLE         17        ///< Command CRC error Interrupt Enable
#define CEB_ENABLE          18        ///< Command end bit error Interrupt Enable
#define CIE_ENABLE          19        ///< Command index error Interrupt Enable
#define DTO_ENABLE          20        ///< Data timeout error Interrupt Enable
#define DCRC_ENABLE         21        ///< Data CRC error Interrupt Enable
#define DEB_ENABLE          22        ///< Data end bit error Interrupt Enable
#define CERR_ENABLE         28        ///< Card error interrupt Enable
#define BADA_ENABLE         29        ///< Bad access to data space interrupt Enable

/**
 * Capabilities register
 */
#define MMCHS_CAPA          0x140
#define VS30                25        ///< Voltage support 3.0V
#define VS18                26        ///< Voltage support 1.8V

/**
 * Registers not needed atm
 */
#define MMCHS_CSRE          0x024
#define MMCHS_SYSTEST       0x028
#define MMCHS_PWCNT         0x030
#define MMCHS_SDMASA        0x100
#define MMCHS_ISE           0x138
#define MMCHS_AC12          0x13C
#define MMCHS_CUR_CAPA      0x148
#define MMCHS_FE            0x150
#define MMCHS_ADMAES        0x154
#define MMCHS_ADMASAL       0x158
#define MMCHS_REV           0x1FC

/**
 * RSPs
 */
#define MMCHS_RSP10         0x110
#define MMCHS_RSP32         0x114
#define MMCHS_RSP54         0x118
#define MMCHS_RSP76         0x11C

/**
 * Commands
 */
#define CMD0      0
#define CMD1      1
#define CMD2      2
#define CMD3      3
#define CMD5      5
#define CMD7      7
#define CMD8      8
#define CMD9      9
#define CMD16     16
#define CMD17     17
#define CMD24     24
#define CMD55     55
#define ACMD6     6
#define ACMD41    41

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
uint32_t mmcsd_card_config();

/**
 * send a command
 * a command starts an operation (serially transfered from sd host ctrl to the card on the mmci_cmd line)
 */
uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg);

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

/**
 * Structures
 */

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
} CSD_SDV2;

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

#endif /* SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_ */
