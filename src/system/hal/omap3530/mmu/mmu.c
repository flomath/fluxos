/*
 * mmu.c
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 *
 *     © Page Frames - Marko Petrovic
 */

#include <string.h>
#include "../../../scheduler/scheduler.h"
#include "mmu.h"
#include "../../../scheduler/process.h"

/**
 * Array of page tables and page frames
 * for matching physical addresses
 * (process + page tables)
 *
 * Each entry consisting of one byte (8 bits)
 */
static char translationByteArray[TRANSLATION_ARRAY_MAX];

/**
 * Master OS page table
 */
static mmu_pageTableP_t masterPageTable;

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
 */
static void mmu_setTTBR1(mmu_pageTableP_t osPageTable);

/**
 * Sets the page table for the Process region
 */
static void mmu_setTTBR0(mmu_pageTableP_t processPageTable);

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
 * Maps a kernel master page table to its region
 * (direct mapping)
 */
static void mmu_mapRegionToMasterPageTable(unsigned int memRegionNumber, mmu_pageTableP_t pageTable);

/**
 * Gets the correct page table index from a virtual address
 * dependent on the page table type (L1, L2 or page frame)
 * and
 * dependent on the ttbr (OS or process)
 */
static uint32_t mmu_getPageTableIndex(uint32_t virtualAddress, unsigned int pageTableType, unsigned int ttbr);

/**
 * Creates a L1 entry based on a given L1 section template
 *
 * Sections implemented
 * Coarses implemented
 */
static uint32_t mmu_createL1PageTableEntry(mmu_l1_section_t L1_entry);

/**
 * Creates a L2 entry based on a given L2 page table template
 *
 * Small pages implemented
 * TODO: Large pages have to be checked
 */
static uint32_t mmu_createL2PageTableEntry(mmu_l2_pageTable_t L2_entry);


/**
 * Get address of free page table
 * Page tables is directly reserved
 */
static uint32_t mmu_getFreePageTable(unsigned int numEntriesReserve);

/**
 * Check if a given entry(bit) in translation byte array is used or not
 */
static bool_t mmu_isEntryUsed(unsigned int entry);

/**
 * Set an entry's status in translation byte array on used/free
 *
 * true for used
 * false for free
 */
static void mmu_setEntryUsed(unsigned int entry, bool_t used);

/**
 * Get physical address of page frame
 *
 * physical address = page table (direct mapped) + page frame * page size
 */
static uint32_t mmu_getPhysicalAddressEntry(unsigned int entry);

/**
 * Get address of L2 from
 * a given address in L1
 *
 * Only works if l1 page table is the real process l1
 */
static mmu_pageTableP_t mmu_getL2PageTable(uint32_t virtualAddress, mmu_pageTableP_t pageTableL1);

/**
 * Create a L2 in a L1 from a process
 */
static void mmu_createL2PageTable(uint32_t virtualAddress, PCB_t* process);

/**
 * Free all page entries (L1, L2, Frames) of a process
 */
static void mmu_freeProcess(mmu_pageTableP_t pageTableL1);

/**
 * Free all L2 page entries and their frames
 */
static void mmu_freePageTableL2(mmu_pageTableP_t pageTableL2);

/**
 * Get the page frame number for
 * the given physical address
 */
static unsigned int mmu_getPageFrameNum(uint32_t physicalAddress);

/**
 * Create a page frame in a L2
 */
static void mmu_createPageFrame(uint32_t virtualAddress, mmu_pageTableP_t pageTableL2);

/**
 * Get a free page frame in
 * process space
 */
static uint32_t mmu_getFreePageFrame();

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
//    mmu_setDomain(DOMAIN_M);
    mmu_setDomain(0xFFFFFFFF);

    // enable MMU
    mmu_enable();
}

void mmu_dabt_handler(void)
{
    // load dabt details via asm
    uint32_t dataFaultAddress = __mmu_load_dabt_addr();
    uint32_t dataFaultStatusRegister = __mmu_load_dabt_status();
    // Bit 10 + 3-0 for fault status
    // shift bit 10 to 4 to get one value
    unsigned int dataFaultStatus = ((dataFaultStatusRegister & 0x400) >> 6) | (dataFaultStatusRegister & 0xF);

    PCB_t* currentProcess = scheduler_getCurrentProcess();
    if (currentProcess == NULL) {
        return;
    }
    //TODO: check dataFault stuff

    // check
    switch(dataFaultStatus) {
        case DABT_ALIGN_FAULT:
            printf("Alignment fault!\n");
            break;
        case DABT_TRANS_SECTION_FAULT:
            // L2 needed
            mmu_createL2PageTable(dataFaultAddress, currentProcess);
            break;
        case DABT_TRANS_PAGE_FAULT:
            // Page frame needed
            mmu_createPageFrame(dataFaultAddress, mmu_getL2PageTable(dataFaultAddress, currentProcess->pageTable));
            printf("Page frame created!\n");
            break;
        case DABT_PERM_SECTION_FAULT:
            // kill process
            scheduler_killProcess(currentProcess->processID);
            break;
        case DABT_PERM_PAGE_FAULT:
            // kill process
            scheduler_killProcess(currentProcess->processID);
            break;
        default:
            scheduler_killProcess(currentProcess->processID);
            printf("PID %d killed\n", currentProcess->processID);
            printf("Data Abort Exception! 0x%x\n", dataFaultStatus);
            break;
    }
}

void mmu_create_process(PCB_t* process)
{
    mmu_pageTableP_t pageTable = mmu_createPageTable(PT_L1);
    mmu_mapRegionToMasterPageTable(BOOT_ROM_EXCEPTIONS, pageTable);
    process->pageTable = pageTable;
//    printf("L1 created at 0x%x\n", pageTable);
}

void mmu_switch_process(PCB_t* process)
{
    if(process->pageTable == NULL) {
        mmu_create_process(process);
    }

    mmu_setTTBR0(process->pageTable);
}

void mmu_kill_process(PCB_t* process)
{
    mmu_freeProcess(process->pageTable);
}

/**
 * static
 */

static void mmu_disable(void)
{
    // call asm function
    __mmu_disable();
}

static void mmu_enable(void)
{
    // call asm function
    __mmu_enable();
}

static void mmu_setTTBR1(mmu_pageTableP_t osPageTable)
{
    // call asm function
    __mmu_set_ttbr1((uint32_t)osPageTable);
}

static void mmu_setTTBR0(mmu_pageTableP_t pageTable)
{
    // call asm function
    __mmu_set_ttbr0((uint32_t)pageTable);
    __mmu_tlb_flush();
}

static void mmu_setTTBCR(uint32_t address)
{
    // call asm function
    __mmu_set_ttbcr((address & BOUNDARY_BIT_MASK));
}

static void mmu_setDomain(uint32_t address)
{
    // call asm function
//    __mmu_set_domain((address & DOMAIN_BIT_MASK));
    __mmu_set_domain(address);
}

/**
 * Page Tables
 */

static mmu_pageTableP_t mmu_createMasterPageTable()
{
    // check for first time a master page table
    // is available, if not create one
    if( masterPageTable != NULL ) {
        return masterPageTable;
    }

    masterPageTable = mmu_createPageTable(PT_L1);
    // map all os regions to master page table
    mmu_mapRegionToMasterPageTable(BOOT_ROM_REGION     , masterPageTable);
    mmu_mapRegionToMasterPageTable(INTERNAL_SRAM_REGION, masterPageTable);
    mmu_mapRegionToMasterPageTable(MMIO_REGION         , masterPageTable);
    mmu_mapRegionToMasterPageTable(KERNEL_REGION       , masterPageTable);
    mmu_mapRegionToMasterPageTable(PAGE_TABLES_REGION  , masterPageTable);
    mmu_mapRegionToMasterPageTable(BOOT_ROM_EXCEPTIONS , masterPageTable);

    return masterPageTable;
}

static mmu_pageTableP_t mmu_createPageTable(unsigned char pageTableType)
{
    // set page table
    unsigned int numPagesReserve = 0;
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

    mmu_pageTableP_t pageTable = (mmu_pageTableP_t) mmu_getFreePageTable(numPagesReserve);

    if( pageTable == NULL ) {
        return FAULT;
    }

    // fill all pages with page faults and return page table
    memset(pageTable, FAULT_PAGE_HIT, SMALL_PAGE_SIZE_4KB * numPagesReserve);
    return pageTable;
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
        L1_entry.domain 		    = DOMAIN_M;

        uint32_t pageTableOffset = mmu_getPageTableIndex(physicalAddress, PT_L1, TTBR1);
        uint32_t* newAddress = pageTable + (pageTableOffset << 2)/sizeof(uint32_t);
        *newAddress = mmu_createL1PageTableEntry(L1_entry);
    }
}

static uint32_t mmu_getPageTableIndex(uint32_t virtualAddress, unsigned int pageTableType, unsigned int ttbr)
{
    switch(pageTableType) {
        case PT_L1:
            return ((virtualAddress & (ttbr == TTBR1 ? PT_L1_BIT_MASK : PT_L1_BIT_MASK_N)) >> PT_L1_BIT_SHIFT);
        case PT_L2:
            return ((virtualAddress & PT_L2_BIT_MASK) >> PT_L2_BIT_SHIFT);
        case PAGE_FRAME:
            return (virtualAddress & PAGE_FRAME_BIT_MASK);
        default:
            return PT_OFFSET_INVALID;
    }
}

static mmu_pageTableP_t mmu_getL2PageTable(uint32_t virtualAddress, mmu_pageTableP_t pageTableL1)
{
    uint32_t pageTableOffsetL1 = mmu_getPageTableIndex(virtualAddress, PT_L1, TTBR0);
    uint32_t pageTableOffsetL2 = mmu_getPageTableIndex(virtualAddress, PT_L2, TTBR0);

    if( pageTableOffsetL2 == PT_OFFSET_INVALID ) {
        return mmu_createPageTable(PT_L2);
    } else {
        // get l2 page ref which
        // should be in l1 entry
        uint32_t pageTableL2Ref = ((uint32_t)pageTableL1 + (pageTableOffsetL1 << 2));

        // get actual l2 page table address
        uint32_t pageTableL2_base = (uint32_t)(*((uint32_t*)pageTableL2Ref)) & SMALL_PAGE_BIT_MASK;
        return (mmu_pageTableP_t) pageTableL2_base;
    }
}

static void mmu_createL2PageTable(uint32_t virtualAddress, PCB_t* process)
{
    mmu_pageTableP_t pageTable = mmu_createPageTable(PT_L2);
//    printf("L2 created at 0x%x\n", pageTable);

    mmu_l1_section_t L1_entry;
    L1_entry.sectionAddress 	= ((uint32_t)pageTable & COARSE_BIT_MASK);
    L1_entry.type 		        = COARSE;
    L1_entry.domain 		    = DOMAIN_M;
    // not needed for page table on l1
    L1_entry.CB 		        = 0x0;
    L1_entry.AP                 = 0x0;

    uint32_t pageTableOffset = mmu_getPageTableIndex(virtualAddress, PT_L1, TTBR0);
    // get address of actual l1 entry and
    // write l1 entry in it
    uint32_t* newAddress = process->pageTable + (pageTableOffset << 2)/sizeof(uint32_t);
    *newAddress = mmu_createL1PageTableEntry(L1_entry);

    // create a page frame after l2
    // has been created, otherwise
    // an data abort occurs for creating
    // a page frame
    mmu_createPageFrame(virtualAddress, pageTable);
}

static void mmu_freeProcess(mmu_pageTableP_t pageTableL1)
{
    // loop through l1 page table entries of process
    // and find all references to l2
    unsigned int pageTableEntry;
    for (pageTableEntry = 0; pageTableEntry < PT_L1_ENTRIES; pageTableEntry++) {
        uint32_t pageTableEntryAddr = (uint32_t)(*(pageTableL1 + pageTableEntry));

        // get type of entry (first 2 bits)
        // if type of entry is a coarse
        // clear frames and l2 itself
        if ((pageTableEntryAddr & 0x3) == COARSE) {
            mmu_pageTableP_t pageTableL2 = (mmu_pageTableP_t)(pageTableEntryAddr & COARSE_BIT_MASK);
            mmu_freePageTableL2(pageTableL2);

            // clear l2
            // clear entry in translation byte array
            mmu_setEntryUsed(mmu_getPageFrameNum((uint32_t)pageTableL2), FALSE);
            *(pageTableL1 + pageTableEntry) = FAULT;
        }
    }

    // clear l1
    // clear entry in translation byte array
    unsigned int pageFrameNum = mmu_getPageFrameNum((uint32_t)pageTableL1);
    mmu_setEntryUsed(pageFrameNum, FALSE);
    mmu_setEntryUsed(pageFrameNum + 1, FALSE);
    mmu_setEntryUsed(pageFrameNum + 2, FALSE);
    mmu_setEntryUsed(pageFrameNum + 3, FALSE);
    *pageTableL1 = FAULT;
}

static void mmu_freePageTableL2(mmu_pageTableP_t pageTableL2)
{
    // loop through l2 page table entries
    // and find all references to frames
    unsigned int pageTableEntry;
    for (pageTableEntry = 0; pageTableEntry < PT_L2_ENTRIES; pageTableEntry++) {
        uint32_t pageTableEntryAddr = (uint32_t)(*(pageTableL2 + pageTableEntry));

        // get type of entry (first 2 bits)
        // if type of entry is a small page
        // clear frame
        if ((pageTableEntryAddr & 0x3) == SMALL_PAGE) {
            uint32_t pageFrameAddr = pageTableEntryAddr & SMALL_PAGE_BIT_MASK;

            // clear frame
            // clear entry in translation byte array
            mmu_setEntryUsed(mmu_getPageFrameNum(pageFrameAddr), FALSE);
            *(pageTableL2 + pageTableEntry) = FAULT;
        }
    }
}

/**
 * Page Frames
 */

static unsigned int mmu_getPageFrameNum(uint32_t physicalAddress)
{
    return (physicalAddress - PAGE_TABLES_START_ADDRESS) / SMALL_PAGE_SIZE;
}

static void mmu_createPageFrame(uint32_t virtualAddress, mmu_pageTableP_t pageTableL2)
{
    uint32_t pageFrame = mmu_getFreePageFrame();

    mmu_l2_pageTable_t L2_entry;
    L2_entry.pageTableAddress 	= pageFrame & SMALL_PAGE_BIT_MASK;
    L2_entry.type  	            = SMALL_PAGE;
    L2_entry.AP                 = AP_RWRW;
    L2_entry.CB 	            = CB_cb;

    uint32_t pageTableOffset = mmu_getPageTableIndex(virtualAddress, PT_L2, TTBR0);
    mmu_pageTableP_t pageTableL2_base = (mmu_pageTableP_t)((uint32_t)pageTableL2 & SMALL_PAGE_BIT_MASK);
    // get address of actual l2 entry and
    // write l2 entry in it
    uint32_t* newAddress = pageTableL2_base + (pageTableOffset << 2)/sizeof(uint32_t);
    *newAddress = mmu_createL2PageTableEntry(L2_entry);
}

static uint32_t mmu_getFreePageTable(unsigned int numEntriesReserve)
{
    unsigned int translationArrayByte;
    unsigned int translationBit;

    // loop through byte array
    for(translationArrayByte = 0; translationArrayByte < PAGE_TABLES_MAX; translationArrayByte++) {
        // loop through byte
        for(translationBit = 0; translationBit < 8; translationBit++) {
            unsigned int pageTableEntry = (translationArrayByte * TRANSLATION_ARRAY_DATATYPE) + translationBit;

            // check if page table entry exceeds byte array
            if( pageTableEntry > PAGE_TABLES_MAX ) {
                return FAULT;
            }

            // actually no usage of other page types
            if( numEntriesReserve != 1 && numEntriesReserve != 4 ) {
                return FAULT;
            }

            // check if page frame is used
            if(mmu_isEntryUsed(pageTableEntry) == FALSE ) {
                // if numPageFrames == 1 -> L2 needed
                if( numEntriesReserve == 1 ) {
                    // set page frame
                    mmu_setEntryUsed(pageTableEntry, TRUE);

                    // return physical address
                    return mmu_getPhysicalAddressEntry(pageTableEntry);
                }
                // TODO: 16 times repeated(alignment) for Large Pages (L2)

                // if numEntriesReserve == 4 -> L1 needed
                // check for 16kB alignment
                if(mmu_getPhysicalAddressEntry(pageTableEntry) % PT_L1_SIZE != 0 ) {
                    continue;
                }

                // check if 3 further entries are free (because L1 needs 4 entries)
                // otherwise go 3 entries further and continue searching
                if (mmu_isEntryUsed(pageTableEntry + 1) == FALSE &&
                        mmu_isEntryUsed(pageTableEntry + 2) == FALSE &&
                        mmu_isEntryUsed(pageTableEntry + 3) == FALSE ) {
                    // set page frames
                    mmu_setEntryUsed(pageTableEntry, TRUE);
                    mmu_setEntryUsed(pageTableEntry + 1, TRUE);
                    mmu_setEntryUsed(pageTableEntry + 2, TRUE);
                    mmu_setEntryUsed(pageTableEntry + 3, TRUE);

                    // return physical address
                    return mmu_getPhysicalAddressEntry(pageTableEntry);
                } else {
                    translationBit += 3;
                }
            }

        }
    }

    return FAULT;
}

static uint32_t mmu_getFreePageFrame()
{
    unsigned int translationArrayByte;
    unsigned int translationBit;

    // loop through byte array
    unsigned int pageFrame;
    for(translationArrayByte = PAGE_FRAMES_START; translationArrayByte < sizeof(translationByteArray); translationArrayByte++) {
        // loop through byte
        for(translationBit = 0; translationBit < 8; translationBit++) {
            pageFrame = (translationArrayByte * TRANSLATION_ARRAY_DATATYPE) + translationBit;

            //TODO: check if pageFrame exceeds

            // check if page frame is used
            if (mmu_isEntryUsed(pageFrame) == FALSE) {
                break;
            } else {
                pageFrame = 0;
            }
        }

        if( pageFrame != 0 ) {
            break;
        }
    }

    // check if frame is in use
    if( pageFrame == 0 ) {
        return FAULT;
    }

    mmu_setEntryUsed(pageFrame, TRUE);
    return mmu_getPhysicalAddressEntry(pageFrame);
}

static bool_t mmu_isEntryUsed(unsigned int entry)
{
    // check if on position of entry is 1 or 0 (used or free)
    return (translationByteArray[entry / TRANSLATION_ARRAY_DATATYPE] >> (entry % TRANSLATION_ARRAY_DATATYPE)) & 0x1;
}

static void mmu_setEntryUsed(unsigned int entry, bool_t used)
{
    char pageFrameByte = translationByteArray[entry / TRANSLATION_ARRAY_DATATYPE];

    switch(used)
    {
        case TRUE:
            pageFrameByte |= (used << (entry % TRANSLATION_ARRAY_DATATYPE));
            break;
        case FALSE:
            pageFrameByte &= ~(1 << (entry % TRANSLATION_ARRAY_DATATYPE));
            break;
        default:
            break;
    }

    translationByteArray[entry / TRANSLATION_ARRAY_DATATYPE] = pageFrameByte;
}

static uint32_t mmu_getPhysicalAddressEntry(unsigned int entry)
{
    if( entry + SMALL_PAGE_SIZE_4KB > ((PAGE_TABLES_END_ADDRESS - PAGE_TABLES_START_ADDRESS) + (PROCESS_END_ADDRESS - PROCESS_START_ADDRESS)) ) {
        return FAULT;
    }

    return PAGE_TABLES_START_ADDRESS + (entry * SMALL_PAGE_SIZE_4KB);
}

/**
 * Page Table Entries
 */

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

static uint32_t mmu_createL2PageTableEntry(mmu_l2_pageTable_t L2_entry)
{
    uint32_t entry = (L2_entry.AP << AP_L2_BIT_SHIFT) |
                     (L2_entry.CB << CB_BIT_SHIFT) |
                     L2_entry.type;

    switch(L2_entry.type) {
        case SMALL_PAGE:
            entry |= (L2_entry.pageTableAddress & SMALL_PAGE_BIT_MASK);
            break;
        case LARGE_PAGE:
            entry |= (L2_entry.pageTableAddress & LARGE_PAGE_BIT_MASK);
            break;
        default:
            return FAULT_PAGE_HIT;
    }

    return entry;
}
