/*
 * memoryManager.h
 *
 * Created on: 17.04.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_

#include "../../common/mmu/memoryManager.h"

/**
 * Memory regions
 */
#define MEMORY_REGIONS		                6			///< Number of regions

#define BOOT_ROM_REGION						0
#define BOOT_ROM_START_ADDRESS				0x40000000
#define BOOT_ROM_END_ADDRESS				0x4002BFFF

#define INTERNAL_SRAM_REGION				1
#define INTERNAL_SRAM_START_ADDRESS			0x402F0400
#define INTERNAL_SRAM_END_ADDRESS			0x402FFFFF

#define MMIO_REGION				            2
#define MMIO_START_ADDRESS 		            0x40300000
#define MMIO_END_ADDRESS 		            0x7FFFFFFF

#define KERNEL_REGION						3
#define KERNEL_START_ADDRESS				0x80000000
#define KERNEL_END_ADDRESS					0x80FFFFFF

#define PAGE_TABLE_REGION					4
#define PAGE_TABLES_START_ADDRESS			0x81000000
#define PAGE_TABLES_END_ADDRESS				0x814FFFFF

#define PROCESS_REGION						5
#define PROCESS_PAGES_START_ADDRESS			0x81500000
#define PROCESS_PAGES_END_ADDRESS			0xBFFFFFFF

/**
 * TODO: define name
 */
#define SMALL_PAGE_SIZE_4KB                 0x1000      ///< 4 KB size
#define FAULT_PAGE_HIT                      0x0         ///< Fault entry hit on a page table

/**
 * Initialize the different memory regions
 */
void memoryManager_init(void);

/**
 * Get the memory region for the
 * given base number
 */
memoryRegion_t* memoryManger_getRegion(unsigned int memRegionNumber);

/**
 * Look up memory for free pages, reserve them
 * and return the address of the first page
 */
uint32_t* memoryManager_getFreePagesInRegion(unsigned int memRegionNumber, unsigned int numPagesReserve);

#endif //SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
