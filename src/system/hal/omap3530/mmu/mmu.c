/*
 * mmu.c
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */
//TODO: define unsigned char as bool && NULL as void ptr

#include "mmu.h"
#include "memoryManager.h"
#include "../../common/mmu/memoryManager.h"
#include "../../common/mmu/mmu.h"

static void initPT(void);
//static void writeSectionToMemory(mmu_l1_pt_t* section);
//static void mmu_set_kernel_table(mmu_ptp table);
//static void mmu_set_process_table(mmu_ptp table);




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
static void mmu_mapRegionToMasterPageTable(memoryRegion_t* memoryRegion, mmu_pageTableP_t pageTable);

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
static uint32_t mmu_createL1PageTableEntry(mmu_l1_section_t* L1_entry);

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
    __mmu_set_ttbr1(osPageTable);
}

static void mmu_setTTBR0(uint32_t* pageTable)
{
    // call asm function
    __mmu_set_ttbr0(pageTable);
}

static void mmu_setTTBCR(uint32_t address)
{
    // call asm function
    __mmu_set_ttbcr(address);
}

static void mmu_setDomain(uint32_t address)
{
    // call asm function
    __mmu_set_domain(address);
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
    mmu_mapRegionToMasterPageTable(PAGE_TABLE_REGION   , masterPageTable);

    return masterPageTable;
}

static mmu_pageTableP_t mmu_createPageTable(unsigned char pageTableType)
{
    // set page table
    unsigned int numPagesReserve = 0;
    //TODO: introduce constants
    switch(pageTableType) {
        case PT_L1:
            // 16 kB Alignment
            numPagesReserve = 4;
            break;
        case PT_L2:
            // 4  kB Alignment
            numPagesReserve = 1;
            break;
        default:
            return NULL;
    }

    return (mmu_pageTableP_t)memoryManager_getFreePagesInRegion(PAGE_TABLE_REGION, numPagesReserve);
}

static void mmu_mapRegionToMasterPageTable(memoryRegion_t* memRegion, mmu_pageTableP_t pageTable)
{
    uint32_t physicalAddress;
    mmu_l1_section_t* L1_entry;

    for(physicalAddress = memRegion->addressStart; physicalAddress < memRegion->addressEnd; physicalAddress += SECTION_SIZE)
    {
        L1_entry->sectionAddress 	= physicalAddress & PT_L1_BIT_MASK;
        L1_entry->type 		        = SECTION;
        L1_entry->CB 		        = CB_WB;
        L1_entry->AP 	            = AP_RWRW;
        L1_entry->domain 		    = DOMAIN_M;

        uint32_t pageTableOffset = mmu_getPageTableIndex(physicalAddress, PT_L1);
        uint32_t* newAddress = pageTable + (pageTableOffset << 2)/sizeof(uint32_t);
        *newAddress = mmu_createL1PageTableEntry(L1_entry);
    }
}

static uint32_t mmu_getPageTableIndex(uint32_t virtualAddress, unsigned int pageTableType)
{
    //TODO: shifting dependent on L1/L2/PageFrame and type of page table
    switch(pageTableType) {
        case PT_L1:
            return ((virtualAddress & PT_L1_BIT_MASK) >> PT_L1_BIT_SHIFT);
        case PT_L2:
            return ((virtualAddress & PT_L2_BIT_MASK) >> PT_L2_BIT_SHIFT);
        case PAGE_FRAME:
            return (virtualAddress & PAGE_FRAME_BIT_MASK);
        default:
            return NULL;
    }

}

static uint32_t mmu_createL1PageTableEntry(mmu_l1_section_t* L1_entry)
{
    uint32_t entry = (L1_entry->AP << AP_L1_BIT_SHIFT) |
                     (L1_entry->domain << DOMAIN_BIT_SHIFT) |
                     (L1_entry->CB << CB_BIT_SHIFT) |
                     L1_entry->type;

    switch(L1_entry->type) {
        case SECTION:
            entry |= (L1_entry->sectionAddress & SECTION_BIT_MASK);
            break;
        case COARSE:
            entry |= (L1_entry->sectionAddress & COARSE_BIT_MASK);
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


/*static void mmu_set_kernel_table(mmu_ptp table) {
    uint32_t tableAddr = (uint32_t) table & 0xFFFFC000;
    __mmu_set_ttbr1(tableAddress);
}

static void mmu_set_process_table(mmu_ptp table) {
    uint32_t tableAddr = (uint32_t) table & 0xFFFFC000;
    __mmu_set_ttbr0(tableAddress);
}*/

/**
 * Initialize page tables
 */
/*static void initPT(void)
{
    mmu_l1_pt_t l1_os_table;
    l1_table.ptAddress = PT_L1_OS_STARTADDR;
    l1_table.vAddress = PT_L1_OS_STARTADDR;

    mmu_l1_section_t hw_section;
    hw_section.type = SECTION;
    hw_section.CB = CB_cb;
    hw_section.domain = DOMAIN_NO;
    hw_section.AP = AP_RWRW;
    hw_section.vAddress = HW_STARTADDR;
    hw_section.pAddress = HW_STARTADDR;
    hw_section.pageSize = SECTION_SIZE;
    hw_section.numPages = HW_SIZE / hw_section.pageSize;
    hw_section.l1_pt = &l1_os_table;
    writeSectionToMemory(&hw_section);

    mmu_l1_section_t kernel_section;
    kernel_section.type = SECTION;
    kernel_section.CB = CB_WB;
    kernel_section.domain = DOMAIN_NO;
    kernel_section.AP = AP_RWNA;
    kernel_section.vAddress = KERNEL_STARTADDR;
    kernel_section.pAddress = KERNEL_STARTADDR;
    hw_section.pageSize = SECTION_SIZE;
    hw_section.numPages = KERNEL_SIZE / kernel_section.pageSize;
    kernel_section.l1_pt = &l1_os_table;
    writeSectionToMemory(&kernel_section);

    mmu_l1_section_t pt_l1_section;
    pt_l1_section.type = SECTION;
    pt_l1_section.CB = CB_WB;
    pt_l1_section.domain = DOMAIN_NO;
    pt_l1_section.AP = AP_RWNA;
    pt_l1_section.vAddress = PT_L1_P_STARTADDR;
    pt_l1_section.pAddress = PT_L1_P_STARTADDR;
    hw_section.pageSize = PT_L1_P_SIZE;
    hw_section.numPages = PT_SECTION_SIZE / pt_l1_section.pageSize;
    pt_l1_section.l1_pt = &l1_os_table;
    writeSectionToMemory(&pt_l1_section);

    mmu_l1_section_t pt_l2_section;
    pt_l2_section.type = SECTION;
    pt_l2_section.CB = CB_WB;
    pt_l2_section.domain = DOMAIN_NO;
    pt_l2_section.AP = AP_RWNA;
    pt_l2_section.vAddress = PT_L2_P_STARTADDR;
    pt_l2_section.pAddress = PT_L2_P_STARTADDR;
    hw_section.pageSize = PT_L2_P_SIZE;
    hw_section.numPages = PT_SECTION_SIZE / pt_l2_section.pageSize;
    pt_l2_section.l1_pt = &l1_os_table;
    writeSectionToMemory(&pt_l2_section);
}*/
/*
void mmu_create_process(void* processId)
{

    mmu_l1_pt_t l1_process;
    l1_process.vAddress = VM_PROC_STARTADDR;
    l1_process.ptAddress = PT_L1_P_STARTADDR + processId * PT_L1_P_SIZE;

    // 1024 l2 pagetables are possible --> 1024 * 1mb
    uint32_t nr_of_pages = processSize / SMALL_PAGE_SIZE;

    if (nr_of_pages <= 256) {
        mmu_l1_section_t process_section;
        process_section.type = SECTION;
        process_section.CB = CB_cb;
        process_section.domain = DOMAIN_NO;
        process_section.AP = AP_RWRW;
        process_section.vAddress = PROC_STARTADDR;
        process_section.pAddress = PROC_STARTADDR;
        process_section.pageSize = SMALL_PAGE_SIZE;
        process_section.numPages = PROC_SECTION_SIZE / PROC_SIZE;
        process_section.l1_pt = &l1_process;

        mmu_l2_pt_t l2_process;
        l2_process.type = COARSE;
        l2_process.numPages = nr_of_pages;
        l2_process.ptAddress = PT_L2_P_STARTADDR + processId * PT_L2_P_SIZE;
        l2_process.l1_ptAddress = l1_process.ptAddress;
        l1_process.mmu_l2_pt[0] = l2_process;

        writeTableToMemory(&process_section);
        writeSmallPagesToMemory(&l2_process);
    } else {

    }
}
*/
/*void mmu_kill_process(void*)
{

}
*/
/*
 * Write section to memory
 */
/*
static void writeSectionToMemory(mmu_l1_section_t* section)
{
    uint32_t* tablePos = (uint32_t*) section->pt->ptAddress;
    tablePos += section->vAddress >> 20;
    tablePos += section->numPages - 1;
    // first 2 bits irrelevant
    tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

    // last 12 bits
    uint32_t entry = section->pAddress & 0xFFFC0000;
    // Bits 10+11 AP
    entry |= (section->AP & 0x3) << 10;
    // Bits 5-8 Domain
    entry |= (section->domain & 0xF) << 5;
    // Bits 2+3 CB
    entry |= (section->CB & 0x3) << 2;
    // Bits 0+1 Section
    entry |= section->type & 0x3;
    int i;
    for (i = section->numPages - 1; i >= 0; i--) {
        *tablePos-- = entry + (i << 20);
    }
}
*/
