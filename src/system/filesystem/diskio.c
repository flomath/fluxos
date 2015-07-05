/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "../hal/omap3530/mmcsd/mmcsd.h"

/* Definitions of physical drive number for each drive */
#define MMC		0	/* Map MMC/SD card to physical drive 0 */

/* RTC function */
#if !_FS_READONLY && !_FS_NORTC
DWORD get_fattime (void){
	return ((35 << 25) | (7 << 21) | (4 << 16) | (17 << 11) | (32 << 5) | (32 << 0));
}
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	if(pdrv == MMC) {
		result = mmcsd_get_card_status();

		switch(result){
		case MMCHS_SUCCESS:
			stat = 0;
			break;
		case MMCHS_ERROR_LOAD:
			stat = STA_NODISK;
			break;
		case MMCHS_ERROR_TIMEOUT:
			stat = STA_NOINIT;
			break;
		}

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result = -1;
	int retryCount = 0;

	if(pdrv == MMC) {
		while(result != MMCHS_SUCCESS && retryCount <= DISKIO_MAX_RETRY_COUNT){
			result = mmcsd_initialize();
			retryCount++;
		}

		switch(result){
		case MMCHS_SUCCESS:
			stat = 0;
			break;
		case MMCHS_ERROR_LOAD:
			stat = STA_NODISK;
			break;
		case MMCHS_ERROR_TIMEOUT:
			stat = STA_NOINIT;
			break;
		}

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */ //TODO: how does this work?
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int result;
	int size;
	char* buffTmp = (void*)buff; //pointer that can be changed

	if(pdrv == MMC) {
		size = 512; //TODO: how to determine the size of a sector?

		int i;
		result = MMCHS_SUCCESS;
		for(i = 0; i < count && result == MMCHS_SUCCESS; i++){
			result = mmcsd_read(sector, (void*)buffTmp, size);

			buffTmp++;
		}

		switch(result){
		case MMCHS_SUCCESS:
			return RES_OK;
		case MMCHS_ERROR_TIMEOUT:
			return RES_NOTRDY;
		case MMCHS_ERROR_INVALID_PARAMETER:
			return RES_PARERR;
		default:
			return RES_ERROR;
		}
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int result;
	int size;
	char* buffTmp = (void*)buff; //pointer that can be changed

	if(pdrv == MMC) {
		size = 512; //TODO: how to determine the size of a sector?

		int i;
		result = MMCHS_SUCCESS;
		for(i = 0; i < count && result == MMCHS_SUCCESS; i++){
			result = mmcsd_write(sector, (void*)buffTmp, size);

			buffTmp++;
		}

		switch(result){
		case MMCHS_SUCCESS:
			return RES_OK;
		case MMCHS_ERROR_TIMEOUT:
			return RES_NOTRDY;
		case MMCHS_ERROR_INVALID_PARAMETER:
			return RES_PARERR;
		default:
			return RES_ERROR;
		}
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(pdrv == MMC) {
		CARD_INFO info = mmcsd_get_card_info();

		//TODO: other commands (in diskio.h)
		switch(cmd){
		case CTRL_SYNC:
			//TODO:
			break;
		case GET_SECTOR_COUNT:
			*((int*)buff) = info.NumOfBlocks;
			break;
		case GET_SECTOR_SIZE:
			*((int*)buff) = info.BlockSize;
			break;
		case GET_BLOCK_SIZE:
			//TODO: what is sector and what is block
			*((int*)buff) = info.BlockSize;
			break;
		case CTRL_TRIM:
			//TODO:
			break;
		case MMC_GET_TYPE:
			*((int*)buff) = info.CardType;
			break;
		case MMC_GET_CSD:
			*((CSD*)buff) = info.CSDData;
			break;
		case MMC_GET_CID:
			*((CID*)buff) = info.CIDData;
			break;
		case MMC_GET_OCR:
			*((OCR*)buff) = info.OCRData;
			break;
		case MMC_GET_SDSTAT:
			*((int*)buff) = mmcsd_get_card_status();
			break;
		}

		return RES_OK;
	}

	return RES_PARERR;
}
#endif
