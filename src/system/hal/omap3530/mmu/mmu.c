/*
 * mmu.c
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 *
 *     © Page Frames - Marko Petrovic
 */

#include <stdio.h>
#include <string.h>
#include "mmu.h"

/**
 * Array of page frames
 * for matching physical addresses
 * (process + page tables)
 *
 * Each entry consisting of one byte (8 bits)
 */
static char pageFramesByteArray[PAGE_FRAMES_MAX];

/**
 * Master OS page table
 */
static uint32_t* masterPageTable;

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
static void mmu_setTTBR1(uint32_t osPageTable);

/**
 * Sets the page table for the Process region
 * TODO: check if uint32_t or pointer
 */
static void mmu_setTTBR0(uint32_t processPageTable);

/**
 * Sets the ttbcr register
 *
 * Basically used for size of ttbr1 and ttbr0
 */
static void mmu_setTTBCR(uint32_t address);

/**
 * Sets the domain
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


/**
 * Get address of free page frame
 * Page frames are directly reserved
 */
static uint32_t mmu_getFreePageFrames(unsigned int numPageFramesReserve);

/**
 * Check if a given page frame is used or not
 */
static bool_t mmu_isPageFrameUsed(unsigned int pageFrame);

/**
 * Set a page frame status on used/free
 *
 * true for used
 * false for free
 */
static void mmu_setPageFrame(unsigned int pageFrame, bool_t used);

/**
 * Get physical address of page frame
 *
 * physical address = page table (direct mapped) + page frame * page size
 */
static uint32_t mmu_getPhysicalAddressPageFrame(unsigned int pageFrame);

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
    //TODO: check if & is working?
    mmu_setTTBR1(&masterPageTable);
    mmu_setTTBR0(&masterPageTable);

    // set boundary (size of ttbr1 and ttbr0)
    mmu_setTTBCR(BOUNDARY_QUARTER);

    // set domain
    mmu_setDomain(DOMAIN_M);

    // enable MMU
    mmu_enable();
}

void mmu_dabt_handler(void)
{
    // load dabt details via asm
    uint32_t dataFaultStatusRegister;
    __mmu_load_dabt(dataFaultStatusRegister);
    // Bit 10 + 3-0 for fault status
    // shift bit 10 to 4 to get one value
    unsigned int dataFaultStatus = ((dataFaultStatusRegister & 0x400) >> 6) | (dataFaultStatusRegister & 0xF);

    //TODO: check current process - scheduler needed

    // check
    switch(dataFaultStatus) {
        case DABT_ALIGN_FAULT:
            printf("Alignment fault!\n");
            break;
        case DABT_TRANS_SECTION_FAULT:
            // L2 needed
            break;
        case DABT_TRANS_PAGE_FAULT:
            // Page frame needed
            break;
        case DABT_PERM_SECTION_FAULT:
            // kill process
            break;
        case DABT_PERM_PAGE_FAULT:
            // kill process
            break;
        default:
            printf("Data Abort Exception!\n");
            break;
    }
}

static void mmu_disable(void) {
    // call asm function
    __mmu_disable();
}

static void mmu_enable(void) {
    // call asm function
    __mmu_enable();
}

static void mmu_setTTBR1(uint32_t osPageTable)
{
    // call asm function
    __mmu_set_ttbr1(osPageTable);
}

static void mmu_setTTBR0(uint32_t pageTable)
{
    // call asm function
    __mmu_set_ttbr0(pageTable);
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

    uint32_t pageTable = mmu_getFreePageFrames(numPagesReserve);

    if( pageTable == NULL ) {
        return 0;
    }

    // fill all pages with page faults and return page table
    memset(pageTable, FAULT_PAGE_HIT, SMALL_PAGE_SIZE_4KB * numPagesReserve);
    return (mmu_pageTableP_t)pageTable;
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

static uint32_t mmu_getFreePageFrames(unsigned int numPageFramesReserve)
{
    unsigned int pageFrameByte;
    unsigned int pageFrameBit;

    // loop through byte array
    for(pageFrameByte = 0; pageFrameByte < (PAGE_TABLES_MAX / PAGE_FRAME_ARRAY_DATATYPE); pageFrameByte++) {
        // loop through byte
        for(pageFrameBit = 0; pageFrameBit < 8; pageFrameBit++) {
            unsigned int pageFrame = (pageFrameByte * PAGE_FRAME_ARRAY_DATATYPE) + pageFrameBit;

            // check if page frame exceeds byte array
            if( pageFrame > PAGE_TABLES_MAX ) {
                return 0;
            }

            // actually no usage of other page types
            if( numPageFramesReserve != 1 && numPageFramesReserve != 4 ) {
                return 0;
            }

            // check if page frame is used
            if( mmu_isPageFrameUsed(pageFrame) == 0 ) {
                // if numPageFrames == 1 -> L2 needed
                if( numPageFramesReserve == 1 ) {
                    // set page frame
                    mmu_setPageFrame(pageFrame, TRUE);

                    // return physical address
                    return mmu_getPhysicalAddressPageFrame(pageFrame);
                }
                // TODO: 16 times repeated(alignment) for L2? I think just for large pages?!

                // if numPageFrame == 4 -> L1 needed
                // check for 16kB alignment
                if( mmu_getPhysicalAddressPageFrame(pageFrame) % PT_L1_SIZE != 0 ) {
                    continue;
                }

                // check if 3 further page frames are free (because L1 needs 4 pages)
                // otherwise go 3 page frames further and continue searching
                if ( mmu_isPageFrameUsed(pageFrame + 1) == 0 &&
                        mmu_isPageFrameUsed(pageFrame + 2) == 0 &&
                        mmu_isPageFrameUsed(pageFrame + 3) == 0 ) {
                    // set page frames
                    mmu_setPageFrame(pageFrame    , TRUE);
                    mmu_setPageFrame(pageFrame + 1, TRUE);
                    mmu_setPageFrame(pageFrame + 2, TRUE);
                    mmu_setPageFrame(pageFrame + 3, TRUE);

                    // return physical address
                    return mmu_getPhysicalAddressPageFrame(pageFrame);
                } else {
                    pageFrameBit += 3;
                }
            }

        }
    }

    return NULL;
}

static bool_t mmu_isPageFrameUsed(unsigned int pageFrame)
{
    // check if on position of pageFrame is 1 or 0 (used or free)
    return (pageFramesByteArray[pageFrame / PAGE_FRAME_ARRAY_DATATYPE] >> (pageFrame % PAGE_FRAME_ARRAY_DATATYPE)) & 0x1;
}

static void mmu_setPageFrame(unsigned int pageFrame, bool_t used)
{
    char pageFrameByte = pageFramesByteArray[pageFrame / PAGE_FRAME_ARRAY_DATATYPE];

    switch(used)
    {
        case TRUE:
            pageFrameByte |= (used << (pageFrame % PAGE_FRAME_ARRAY_DATATYPE));
            break;
        case FALSE:
            pageFrameByte &= (used << (pageFrame % PAGE_FRAME_ARRAY_DATATYPE));
            break;
        default:
            break;
    }

    pageFramesByteArray[pageFrame / PAGE_FRAME_ARRAY_DATATYPE] = pageFrameByte;
}

static uint32_t mmu_getPhysicalAddressPageFrame(unsigned int pageFrame)
{
    if( pageFrame + SMALL_PAGE_SIZE_4KB > ((PAGE_TABLES_END_ADDRESS - PAGE_TABLES_START_ADDRESS) + (PROCESS_END_ADDRESS - PROCESS_START_ADDRESS)) ) {
        return 0;
    }

    return PAGE_TABLES_START_ADDRESS + (pageFrame * SMALL_PAGE_SIZE_4KB);
}


//TODO: implement creation l1, l2, page frames and data abort handler
