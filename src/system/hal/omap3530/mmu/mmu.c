/*
 * mmu.c
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */
//TODO: define unsigned char as bool && NULL as void ptr

//#include <stdint.h>
//#include <stdlib.h>
#include "mmu.h"

uint32_t* masterPageTable;

/**
 * Disable mmu
 */
static void mmu_disable(void);

/**
 * Enable mmu
 */
static void mmu_enable(void);

/**
 * Sets the page table for the OS/Kernel region
 * TODO: check if uint32_t or pointer
 */
static void mmu_setTTBR1(uint32_t* osPageTable);

/**
 * Sets the page table for the Process region
 * TODO: check if uint32_t or pointer
 */
static void mmu_setTTBR0(uint32_t* processPageTable);

/**
 * Sets the ttbcr register
 *
 * Basically used for size of ttbr1 and ttbr0
 * TODO: check if uint32_t or smaller
 */
static void mmu_setTTBCR(uint32_t address);

/**
 * Sets the domain
 * TODO: check if uint32_t or smaller
 */
static void mmu_setDomain(uint32_t address);

/**
 * Creates the master page table
 * for the OS region
 */
static mmu_pageTableP_t mmu_createMasterPageTable();

/**
 * Creates a l1 or l2 page table
 */
static mmu_pageTableP_t mmu_createPageTable(unsigned char pageTableType);

/**
 * Initializes a page table
 */
static void mmu_initPageTable(mmu_pageTableP_t pageTable);

/**
 * Maps a kernel master page table to its region
 * (direct mapping)
 */
static void mmu_mapRegionToMasterPageTable(unsigned int memRegionNumber, mmu_pageTableP_t pageTable);

/**
 * Gets the correct page table index from a virtual address
 * depended on the page table (L1, L2 or page frame)
 */
static uint32_t mmu_getPageTableIndex(uint32_t virtualAddress, unsigned int pageTableType);

/**
 * Creates a L1 address based on a given L1 section template
 *
 * Sections implemented
 * TODO: Coarses have to be checked
 */
static uint32_t mmu_createL1PageTableEntry(mmu_l1_section_t L1_entry);

/**
 * Creates a L2 address based on a given L2 page table template
 *
 * Small pages implemented
 * TODO: Large pages have to be checked
 */
static uint32_t mmu_createL2PageTableEntry(mmu_l2_pageTable_t* L2_entry);

void mmu_init(void)
{
    // initialize memory regions
    memoryManager_init();

    // disable mmu, to configure it
    mmu_disable();

    // create OS master page table
    // and has always a size of 16kB + alignment
    mmu_createMasterPageTable();

    // set ttbr1 and ttbr0 for
    // OS and Process page table
    mmu_setTTBR1(masterPageTable);
    mmu_setTTBR0(masterPageTable);

    // set boundary (size of ttbr1 and ttbr0)
    mmu_setTTBCR(BOUNDARY_QUARTER);

    // set domain
    mmu_setDomain(DOMAIN_M);

    // enable MMU
    mmu_enable();
}

static void mmu_disable(void) {
    // call asm function
    __mmu_disable();
}

static void mmu_enable(void) {
    // call asm function
    __mmu_enable();
}

static void mmu_setTTBR1(uint32_t* osPageTable)
{
    // call asm function
    __mmu_set_ttbr1((uint32_t)osPageTable);
}

static void mmu_setTTBR0(uint32_t* pageTable)
{
    // call asm function
    __mmu_set_ttbr0((uint32_t)pageTable);
}

static void mmu_setTTBCR(uint32_t address)
{
    // call asm function
    __mmu_set_ttbcr((address & BOUNDARY_BIT_MASK));
}

static void mmu_setDomain(uint32_t address)
{
    // call asm function
    __mmu_set_domain((address & DOMAIN_BIT_MASK));
}

static mmu_pageTableP_t mmu_createMasterPageTable()
{
    // check for first time a master page table
    // is available, if not create one
    if( masterPageTable != NULL ) {
        return masterPageTable;
    }

    masterPageTable = mmu_createPageTable(PT_L1);
    // map all os regions to page table
    mmu_mapRegionToMasterPageTable(BOOT_ROM_REGION     , masterPageTable);
    mmu_mapRegionToMasterPageTable(INTERNAL_SRAM_REGION, masterPageTable);
    mmu_mapRegionToMasterPageTable(MMIO_REGION         , masterPageTable);
    mmu_mapRegionToMasterPageTable(KERNEL_REGION       , masterPageTable);
    mmu_mapRegionToMasterPageTable(PAGE_TABLES_REGION  , masterPageTable);

    return masterPageTable;
}

static mmu_pageTableP_t mmu_createPageTable(unsigned char pageTableType)
{
    // set page table
    unsigned int numPagesReserve = 0;
    //TODO: introduce constants
    switch(pageTableType) {
        case PT_L1:
            // 4096 Entries = 4 * 4096 = 16 kB Alignment
            numPagesReserve = 4;
            break;
        case PT_L2:
            // 256  Entries = 1 * 4096 = 4  kB Alignment
            numPagesReserve = 1;
            break;
        default:
            return NULL;
    }

    return (mmu_pageTableP_t)memoryManager_getFreePages(PAGE_TABLES_REGION, numPagesReserve);
}

static void mmu_mapRegionToMasterPageTable(unsigned int memRegionNumber, mmu_pageTableP_t pageTable)
{
    memoryRegion_t* memRegion = memoryManger_getRegion(memRegionNumber);
    uint32_t physicalAddress;
    mmu_l1_section_t L1_entry;

    for(physicalAddress = memRegion->addressStart; physicalAddress < memRegion->addressEnd; physicalAddress += SECTION_SIZE)
    {
        L1_entry.sectionAddress 	= (physicalAddress & PT_L1_BIT_MASK);
        L1_entry.type 		        = SECTION;
        L1_entry.CB 		        = CB_WB;
        L1_entry.AP 	            = AP_RWRW;
        L1_entry.domain 		    = DOMAIN_NO;

        uint32_t pageTableOffset = mmu_getPageTableIndex(physicalAddress, PT_L1);
        uint32_t* newAddress = pageTable + (pageTableOffset << 2)/sizeof(uint32_t);
        *newAddress = mmu_createL1PageTableEntry(L1_entry);
    }
}

static uint32_t mmu_getPageTableIndex(uint32_t virtualAddress, unsigned int pageTableType)
{
    switch(pageTableType) {
        case PT_L1:
            return ((virtualAddress & PT_L1_BIT_MASK) >> PT_L1_BIT_SHIFT);
        case PT_L2:
            return ((virtualAddress & PT_L2_BIT_MASK) >> PT_L2_BIT_SHIFT);
        case PAGE_FRAME:
            return (virtualAddress & PAGE_FRAME_BIT_MASK);
        default:
            return 0;
    }

}

static uint32_t mmu_createL1PageTableEntry(mmu_l1_section_t L1_entry)
{
    uint32_t entry = (L1_entry.AP << AP_L1_BIT_SHIFT) |
                     (L1_entry.domain << DOMAIN_BIT_SHIFT) |
                     (L1_entry.CB << CB_BIT_SHIFT) |
                     L1_entry.type;

    switch(L1_entry.type) {
        case SECTION:
            entry |= (L1_entry.sectionAddress & SECTION_BIT_MASK);
            break;
        case COARSE:
            entry |= (L1_entry.sectionAddress & COARSE_BIT_MASK);
            break;
        default:
            return FAULT_PAGE_HIT;
    }

    return entry;
}

static uint32_t mmu_createL2PageTableEntry(mmu_l2_pageTable_t* L2_entry)
{
    uint32_t entry = (L2_entry->AP << AP_L2_BIT_SHIFT) |
                     (L2_entry->CB << CB_BIT_SHIFT) |
                     L2_entry->type;

    switch(L2_entry->type) {
        case SMALL_PAGE:
            entry |= (L2_entry->pageTableAddress & SMALL_PAGE_BIT_MASK);
            break;
        case LARGE_PAGE:
            entry |= (L2_entry->pageTableAddress & LARGE_PAGE_BIT_MASK);
            break;
        default:
            return FAULT_PAGE_HIT;
    }

    return entry;
}


//TODO: implement creation l1, l2, page frames and data abort handler
