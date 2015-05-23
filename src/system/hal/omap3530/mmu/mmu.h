/*
 * mmu.h
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_

#include "../../common/mmu/mmu.h"
#include "memoryManager.h"

/**
 * Translation Table Base
 */
#define TLB_SIZE            0x4000      ///< 4096 Entries * 4 = 16kB (p.962)
#define TTBR0			    0x0
#define TTBR1               0x1

/**
 * Page tables, sizes
 * and
 * bit masks for
 * page table index from address
 */
#define PT_L1               0x0
#define PT_L2               0x1
#define PAGE_FRAME          0x2

#define PT_L1_SIZE          0x4000      ///< 4096 Entries * 4 = 16kB (p.962)

#define SECTION             0x02        ///< 1 MB block - 1024 Entries
#define COARSE              0x01        ///< 4 KB block - 256  Entries
#define SECTION_SIZE        0x100000    ///< 0x100000 = 1048576 Bytes = 1 MB
#define COARSE_SIZE         0x400       ///< 0x400 = 256 Entries
#define SECTION_BIT_MASK    0xFFF00000  ///< Upper 12 bits
#define COARSE_BIT_MASK     0xFFFFFC00  ///< Upper 22 bits

#define PT_L1_BIT_MASK	    0xFFF00000  ///< Upper 12 bits
#define PT_L1_BIT_SHIFT     20          ///< Upper 12 bits important
#define PT_L2_BIT_MASK	    0xFF000     ///< 8 bits after Upper 12
#define PT_L2_BIT_SHIFT     12          ///< 8 bits after lower 12 important
#define PAGE_FRAME_BIT_MASK 0xFFF       ///< Lower 12 bits

/**
 * Table 3.8. Interpreting page table entry bits
 */
#define SMALL_PAGE          0x02        ///< 4  KB page
#define LARGE_PAGE          0x01        ///< 64 KB page
#define SMALL_PAGE_SIZE     0x1000      ///< 0x01000 = 4096  Bytes = 4  KB
#define LARGE_PAGE_SIZE     0x10000     ///< 0x10000 = 65536 Bytes = 64 KB
#define SMALL_PAGE_BIT_MASK 0xFFFFF000  ///< Upper 20 bits
#define LARGE_PAGE_BIT_MASK 0xFFF00000  ///< Upper 12 bits

/**
 * Table D4-36 MemAttr[3:2] encoding, stage 2 translation
 *
 * cb = noncached nonbuffered
 * cB = noncached buffered
 * WT = write-through cacheable
 * WB = write-back cacheable
 */
#define CB_BIT_SHIFT		2
#define	CB_cb	            0x0         ///< CB noncached nonbuffered
#define	CB_cB	            0x1         ///< CB noncached buffered
#define	CB_WT	            0x2         ///< CB write-through cacheable
#define	CB_WB	            0x3         ///< CB write-back cacheable

/**
 * Table 3.11. Domain access control register, access control bits
 *
 * NO = No access
 * C = Client
 * R = Reserved
 * M = Manager
 */
#define DOMAIN_BIT_MASK	    0x3         ///< Lower 2 bits important
#define DOMAIN_BIT_SHIFT    5
#define DOMAIN_NO           0x00        ///< No access - Any access generates a domain fault
#define DOMAIN_C            0x01        ///< Client - Accesses are checked against the access permission bits in the section or page descriptor
#define DOMAIN_R            0x02        ///< Reserved - Currently behaves like the no access mode
#define DOMAIN_M            0x03        ///< Manager - Accesses are not checked against the access permission bits so a permission fault cannot be generated

/**
 * Table 3.12. Interpreting access permission (AP) bits
 *
 * Privileged permissions | User permissions
 *
 * NA = No access
 * RO = Read-only
 * RW = Read/write
 */
#define AP_L1_BIT_SHIFT     10
#define AP_L2_BIT_SHIFT     4
#define AP_NANA             0x00        ///< AP No access
#define AP_RWNA             0x01        ///< AP Read/write | No access
#define AP_RWRO             0x02        ///< AP Read/write | Read-only
#define AP_RWRW             0x03        ///< AP Read/write | Read/write

/**
 * Boundaries for TTBR1 and TTBR0
 * Table B3-1
 */
#define BOUNDARY_BIT_MASK   0x7         ///< Lower 3 bits important
#define BOUNDARY_HALF	    0x1
#define BOUNDARY_QUARTER    0x2         ///< One Quarter for ttbr0

/**
 * Page frames
 */
#define FAULT_PAGE_HIT      0x0         ///< Fault entry hit on a page table

#define PAGE_FRAME_ARRAY_DATATYPE   sizeof(char)
#define PAGE_FRAMES_MAX     (DDR0_END_ADDRESS - PAGE_TABLES_START_ADDRESS) / SMALL_PAGE_SIZE_4KB / PAGE_FRAME_ARRAY_DATATYPE
#define PAGE_TABLES_MAX     (PAGE_TABLES_END_ADDRESS - PAGE_TABLES_START_ADDRESS) / SMALL_PAGE_SIZE_4KB

/**
 * Data abort fault status
 */
#define DABT_ALIGN_FAULT            0x1 ///< Alignment fault
#define DABT_DEBUG                  0x2 ///< Debug event
#define DABT_ACCESS_SECTION_FAULT   0x3 ///< Access flag fault, section
#define DABT_INSTR_FAULT            0x4 ///< Instruction cache maintenance fault
#define DABT_TRANS_SECTION_FAULT    0x5 ///< Translation fault, section
#define DABT_ACCESS_PAGE_FAULT      0x6 ///< Access flag fault, page
#define DABT_TRANS_PAGE_FAULT       0x7 ///< Translation fault, page
#define DABT_SYNC_EXT_TRANS_NO      0x8 ///< Synchronous external abort, non-translation
#define DABT_DOMAIN_SECTION_FAULT   0x9 ///< Domain fault, section
#define DABT_DOMAIN_PAGE_FAULT      0xB ///< Domain fault, page
#define DABT_SYNC_EXT_TRANS_L1      0xC ///< Synchronous external abort on translation table walk, first level
#define DABT_PERM_SECTION_FAULT     0xD ///< Permission fault, section
#define DABT_SYNC_EXT_TRANS_L2      0xE ///< Synchronous external abort on translation table walk, second level
#define DABT_PERM_PAGE_FAULT        0xF ///< Permission fault, second level

/**
 * Asm functions
 */
extern void __mmu_enable_write_buffer(void);
extern void __mmu_set_domain(uint32_t domain);
extern void __mmu_set_ttbcr(uint32_t ttbrc_address);
extern void __mmu_set_ttbr0(uint32_t ttbr_address);//TODO: implement ASID (context)
extern void __mmu_set_ttbr1(uint32_t ttbr_address);
extern void __mmu_enable(void);
extern void __mmu_disable(void);
extern void __mmu_load_dabt(uint32_t dataFaultAddress, uint32_t dataFaultStatusRegister);
extern void __mmu_tlb_flush(void);


#endif //SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
