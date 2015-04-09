/*
 * mmu.c
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */

#include "mmu.h"
#include "../../common/mmu/mmu.h"

static void initPTables(void);
static void writeSectionsToMemory(mmu_l1_pt_t* section, uint32_t numOfSections);

void mmu_init(void)
{
    // needed?
    __mmu_enable_write_buffer();
    //set_ddr_memory(SDRAM_RAM_SIZE_256);

    // set size of ttbr1
    __mmu_set_ttbcr(0x2);

    initPTables();
    /* the master-pagetable base-address is referenced in the ttbr1 and has always a size of 16kB */
    uint32_t ttbr1 = PT_L1_OS_STARTADDR & 0xFFFFC000;
    ttbr1 |= (CB_WB & 0x3) << 3;
    ttbr1 |= 0x1; //caching activate
    __mmu_set_ttbr1(ttbr1);

    // set manager access
    __mmu_set_domain(0x3);

    __mmu_enable();

    // set domain to user?
}

/**
 * Initialize page tables
 */
static void initPTables(void)
{
    mmu_pt_t masterTable;
    masterTable.ptAddress = PT_L1_OS_STARTADDR;

    mmu_l1_pt_t hw_section;
    hw_section.type = SECTION;
    hw_section.CB = CB_cb;
    hw_section.domain = DOMAIN_NO;
    hw_section.AP = AP_RWRW;
    hw_section.vAddress = HW_STARTADDR;
    hw_section.pAddress = HW_STARTADDR;
    hw_section.pt = &masterTable;
    writeSectionsToMemory(&hw_section, (HW_SIZE / SECTION_SIZE));

    mmu_l1_pt_t int_vec_section;
    int_vec_section.type = SECTION;
    int_vec_section.CB = CB_cb;
    int_vec_section.domain = DOMAIN_NO;
    int_vec_section.AP = AP_RWRW;
    int_vec_section.vAddress = INT_VEC_STARTADDR;
    int_vec_section.pAddress = INT_VEC_STARTADDR;
    int_vec_section.pt = &masterTable;
    writeSectionsToMemory(&int_vec_section, 1); //TODO int_vec size?

    mmu_l1_pt_t kernel_section;
    kernel_section.type = SECTION;
    kernel_section.CB = CB_WB;
    kernel_section.domain = DOMAIN_NO;
    kernel_section.AP = AP_RWNA;
    kernel_section.vAddress = KERNEL_STARTADDR;
    kernel_section.pAddress = KERNEL_STARTADDR;
    kernel_section.pt = &masterTable;
    writeSectionsToMemory(&kernel_section, (KERNEL_SIZE / SECTION_SIZE));

    mmu_l1_pt_t pt_l1_section;
    pt_l1_section.type = SECTION;
    pt_l1_section.CB = CB_WB;
    pt_l1_section.domain = DOMAIN_NO;
    pt_l1_section.AP = AP_RWNA;
    pt_l1_section.vAddress = PT_L1_P_STARTADDR;
    pt_l1_section.pAddress = PT_L1_P_STARTADDR;
    pt_l1_section.pt = &masterTable;
    //writeSectionsToMemory(&pt_l1_section, (PAGE_TABLE_REGION_SIZE / TASK_L1_PT_SIZE));


    mmu_l1_pt_t pt_l2_section;
    pt_l2_section.type = SECTION;
    pt_l2_section.CB = CB_WB;
    pt_l2_section.domain = DOMAIN_NO;
    pt_l2_section.AP = AP_RWNA;
    pt_l2_section.vAddress = PT_L2_P_STARTADDR;
    pt_l2_section.pAddress = PT_L2_P_STARTADDR;
    pt_l2_section.pt = &masterTable;
    //writeSectionsToMemory(&pt_l2_section, (PAGE_TABLE_REGION_SIZE / SECTION_PAGE_SIZE));
}

/*
 * Write PT to memory (only sections)
 *
 * TODO: to define
 */
static void writeSectionsToMemory(mmu_l1_pt_t* section, uint32_t numOfSections) {
    uint32_t* tablePos = (uint32_t*) section->pt->ptAddress;
    tablePos += section->vAddress >> 20;
    tablePos += numOfSections - 1;
    tablePos = (uint32_t*)((uint32_t)tablePos & 0xFFFFFFFC);

    uint32_t entry = section->pAddress & 0xFFFF0000;
    entry |= 0x2; //section entry
    entry |= (section->CB & 0x3) << 2;
    entry |= section->domain << 5;
    entry |= (section->AP & 0x3) << 10;
    int i;
    for (i = numOfSections - 1; i >= 0; i--) {
        *tablePos-- = entry + (i << 20);
    }
}
