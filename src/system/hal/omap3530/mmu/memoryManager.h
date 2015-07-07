/*
 * memoryManager.h
 *
 * Created on: 17.04.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMU_MEMORYMANAGER_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMU_MEMORYMANAGER_H_

#include "../../common/mmu/memoryManager.h"

/**
 * DDR0
 */
#define DDR0_START_ADDRESS                  0x80000000
#define DDR0_END_ADDRESS                    0xC0000000  ///< Start address + 1 GB (0x40000000)

/**
 * Memory regions
 */
#define MEMORY_REGIONS		                7			///< Number of regions

#define BOOT_ROM_REGION						0
#define BOOT_ROM_START_ADDRESS				0x40000000
#define BOOT_ROM_END_ADDRESS				0x4001BFFF
static page_t regionBootROM[27];            ///< ((BOOT_ROM_END_ADDRESS-BOOT_ROM_START_ADDRESS)/4096)

#define INTERNAL_SRAM_REGION				1
#define INTERNAL_SRAM_START_ADDRESS			0x40200000
#define INTERNAL_SRAM_END_ADDRESS			0x4020FFFF
static page_t regionInternalSRAM[15];       ///< ((INTERNAL_SRAM_END_ADDRESS-INTERNAL_SRAM_START_ADDRESS)/4096)

#define MMIO_REGION				            2
#define MMIO_START_ADDRESS 		            0x48000000
#define MMIO_END_ADDRESS 		            0x490FFFFF
static page_t regionMMIO[4351];             ///< ((MMIO_END_ADDRESS-MMIO_START_ADDRESS)/4096)

#define KERNEL_REGION						3
#define KERNEL_START_ADDRESS				0x80000000
#define KERNEL_END_ADDRESS					0x80FFFFFF
static page_t regionKernel[4095];           ///< ((KERNEL_END_ADDRESS-KERNEL_START_ADDRESS)/4096)

#define PAGE_TABLES_REGION					4
#define PAGE_TABLES_START_ADDRESS			0x81000000
#define PAGE_TABLES_END_ADDRESS				0x814FFFFF
static page_t regionPageTables[1279];       ///< ((PAGE_TABLES_END_ADDRESS-PAGE_TABLES_START_ADDRESS)/4096)

#define PROCESS_REGION						5
#define PROCESS_START_ADDRESS			    0x0001C000
#define PROCESS_END_ADDRESS			        0x3EAFFFFF
static page_t regionProcess[256739];        ///< ((PROCESS_END_ADDRESS-PROCESS_START_ADDRESS)/4096)

#define BOOT_ROM_EXCEPTIONS                 6
#define BOOT_ROM_EXCEPTIONS_START_ADDRESS	0x00014000
#define BOOT_ROM_EXCEPTIONS_END_ADDRESS		0x0001BFFF
static page_t regionBootROMExceptions[7];   ///< ((BOOT_ROM_EXCEPTIONS_END_ADDRESS-BOOT_ROM_EXCEPTIONS_START_ADDRESS)/4096)

/**
 * Page sizes
 */
#define SMALL_PAGE_SIZE_4KB                 0x1000      ///< 4 KB size

/**
 * Initialize the different memory regions
 */
void memoryManager_init(void);

/**
 * Get the memory region for the
 * given base number
 */
memoryRegion_t* memoryManger_getRegion(unsigned int memRegionNumber);

#endif //SRC_SYSTEM_HAL_OMAP3530_MMU_MEMORYMANAGER_H_
