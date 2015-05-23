/*
 * memoryManager.c
 *
 * Created on: 17.04.15
 *     Author: Nino Schoch
 */

#include "memoryManager.h"

memoryRegion_t memoryRegions[MEMORY_REGIONS];

/**
 * Initialize the region with addresses
 * and based on the used page size
 *
 * Reserve directed mapped regions directly
 */
static void memoryManager_initRegion(memoryRegion_t* memRegion, uint32_t addressStart, uint32_t addressEnd, page_t* pages, bool_t directMapped);

/**
 * Reserve a page in a memory region
 */
static void memoryManager_reservePage(memoryRegion_t* memRegion, unsigned int pageNumber);

/**
 * Reserve a number of pages in a memory region
 *
 * Returns -1 for success - pages reserved
 * Returns >= 0 for failure - already a reserved page was found
 */
static int memoryManager_reservePages(memoryRegion_t* memRegion, unsigned int pageNumber, unsigned int numPagesReserve);

/**
 * Check if a given number of pages in a region
 * at a specific page number are free
 *
 * Returns -1 for success - free pages available
 * Returns >= 0 for failure - already a reserved page was found
 */
static int memoryManager_areFreePages(memoryRegion_t* memRegion, unsigned int pageNumber, unsigned int numPages);

/**
 * Get address of given page number
 * based on the region
 */
static uint32_t* memoryManager_getPageAddress(memoryRegion_t* memRegion, unsigned int pageNumber);

void memoryManager_init(void)
{
	// init regions
	memoryManager_initRegion(&memoryRegions[BOOT_ROM_REGION     ], BOOT_ROM_START_ADDRESS     , BOOT_ROM_END_ADDRESS     , regionBootROM     , TRUE);
	memoryManager_initRegion(&memoryRegions[INTERNAL_SRAM_REGION], INTERNAL_SRAM_START_ADDRESS, INTERNAL_SRAM_END_ADDRESS, regionInternalSRAM, TRUE);
	memoryManager_initRegion(&memoryRegions[MMIO_REGION         ], MMIO_START_ADDRESS         , MMIO_END_ADDRESS         , regionMMIO        , TRUE);
	memoryManager_initRegion(&memoryRegions[KERNEL_REGION       ], KERNEL_START_ADDRESS       , KERNEL_END_ADDRESS       , regionKernel      , TRUE);
	memoryManager_initRegion(&memoryRegions[PAGE_TABLES_REGION  ], PAGE_TABLES_START_ADDRESS  , PAGE_TABLES_END_ADDRESS  , regionPageTables  , TRUE);
	memoryManager_initRegion(&memoryRegions[PROCESS_REGION      ], PROCESS_START_ADDRESS      , PROCESS_END_ADDRESS      , regionProcess     , FALSE); //TODO: physical address of process region?
	memoryManager_initRegion(&memoryRegions[BOOT_ROM_EXCEPTIONS ], BOOT_ROM_EXCEPTIONS_START_ADDRESS, BOOT_ROM_EXCEPTIONS_END_ADDRESS, regionBootROMExceptions, TRUE);
}

memoryRegion_t* memoryManger_getRegion(unsigned int memRegionNumber)
{
	return &memoryRegions[memRegionNumber];
}

/**
 * Static functions
 */

static void memoryManager_initRegion(memoryRegion_t* memRegion, uint32_t addressStart, uint32_t addressEnd, page_t* pages, bool_t directMapped)
{
	memRegion->addressStart 		= addressStart;
	memRegion->addressEnd 			= addressEnd;
	memRegion->pageSize				= SMALL_PAGE_SIZE_4KB;
	memRegion->numPages				= (addressStart-addressEnd) / memRegion->pageSize;
	memRegion->numPagesReserved		= 0;
	memRegion->pages 				= pages;

	// reserve all pages in a region
	// that is directly mapped vA-pA
	if( directMapped == 1 ) {
		memoryManager_reservePages(memRegion, 0, memRegion->numPages);
	}
}

static void memoryManager_reservePage(memoryRegion_t* memRegion, unsigned int pageNumber)
{
	// check if region can address
	// number of page
	if( pageNumber > memRegion->numPages ) {
		return;
	}

	memRegion->pages[pageNumber].reserved = 1;
	memRegion->numPagesReserved++;
}

static int memoryManager_reservePages(memoryRegion_t* memRegion, unsigned int pageNumber, unsigned int numPagesReserve)
{
	// check if free pages are available
	// if status is not -1 (free pages available)
	// return index of first reserved page found
	int status = memoryManager_areFreePages(memRegion, pageNumber, numPagesReserve);
	if( status != -1 ) {
		return status;
	}

	// reserve given pages
	unsigned int i;
	for(i = pageNumber; i < numPagesReserve; i++) {
		memoryManager_reservePage(memRegion, i);
	}

	return status;
}

static int memoryManager_areFreePages(memoryRegion_t* memRegion, unsigned int pageNumber, unsigned int numPages)
{
	// if region has not enough pages,
	// don't reserve
	if( (pageNumber + numPages) > (memRegion->numPages - memRegion->numPagesReserved) ) {
		return pageNumber;
	}

	unsigned int i;
	for(i = pageNumber; i < (pageNumber + numPages); i++) {
		// if a page is found, which is reserved,
		// return index - better performance
		if(memRegion->pages[i].reserved == 1) {
			return i;
		}
	}

	return -1;
}

static uint32_t* memoryManager_getPageAddress(memoryRegion_t* memRegion, unsigned int pageNumber)
{
	// check if region can address
	// number of page
	if( pageNumber > memRegion->numPages ) {
		return NULL;
	}

	return ((uint32_t*)(memRegion->addressStart + (pageNumber * memRegion->pageSize)));
}
