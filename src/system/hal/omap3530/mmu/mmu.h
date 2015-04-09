/*
 * mmu.h
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_

#include "../../common/mmu/mmu.h"

/**
 * Translation Table Base
 */
#define TLB_SIZE            0x4000      ///< 4096 Entries

/**
 * Table 3.3. Interpreting first-level descriptor bits
 */
#define SECTION             0x02        ///< 1 MB block - 1024 Entries
#define COARSE              0x01        ///< 4 KB block - 256  Entries
#define SECTION_SIZE        0x100000    ///< 0x100000 = 1048576 Bytes = 1 MB
#define COARSE_SIZE         0x400       ///< 0x400 = 256 Entries

/**
 * Table 3.8. Interpreting page table entry bits
 */
#define SMALL_PAGE          0x02        ///< 4  KB page
#define LARGE_PAGE          0x01        ///< 64 KB page
#define SMALL_PAGE_SIZE     0x1000      ///< 0x01000 = 4096  Bytes = 4  KB
#define LARGE_PAGE_SIZE     0x10000     ///< 0x10000 = 65536 Bytes = 64 KB

/**
 * Table D4-36 MemAttr[3:2] encoding, stage 2 translation
 *
 * cb = noncached nonbuffered
 * cB = noncached buffered
 * WT = write-through cacheable
 * WB = write-back cacheable
 */
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
#define AP_NANA             0x00        ///< AP No access
#define AP_RWNA             0x01        ///< AP Read/write | No access
#define AP_RWRO             0x02        ///< AP Read/write | Read-only
#define AP_RWRW             0x03        ///< AP Read/write | Read/write


#define MAX_PROCESSES       128         ///< Max proccess count
#define MAX_L2_PT           1024        ///< Max L2 page entries

/**
 * Hardware
 */
#define HW_STARTADDR        0x48000000  ///< Hardware start address
#define HW_SIZE 			0x2000000   ///< 32 MB
#define HW_PAGE_SIZE 		SMALL_PAGE_SIZE

#define INT_VEC_STARTADDR   0x4020FFC8  ///< Interrupt Vector address
#define INT_VEC_SIZE		LARGE_PAGE_SIZE

/**
 * OS
 */
#define KERNEL_STARTADDR    0x80000000  ///< Kernel start address - currently DDR0
#define KERNEL_SIZE         0x800000    ///< 8 MB //TODO: define size
#define KERNEL_SECTION_SIZE	SECTION_SIZE

/**
 * L1 table - OS
 */
#define PT_L1_OS_STARTADDR	(KERNEL_STARTADDR + KERNEL_SIZE)
#define PT_L1_OS_SIZE       TLB_SIZE    ///< 4096 Entries * 4 = 16kB (p.962)

/**
 * L1 + 2 table - Process
 */
#define PT_L1_P_STARTADDR   (PT_L1_OS_STARTADDR + PT_L1_OS_SIZE)
#define PT_L1_P_SIZE        TLB_SIZE    ///< 4096 Entries * 4 = 16kB (p.962)
#define PT_L2_P_STARTADDR   (PT_L1_P_STARTADDR + (PT_L1_P_SIZE * MAX_PROCESSES))
#define PT_L2_P_SIZE        COARSE_SIZE ///< 256  Entries
#define PT_L2_P_ENDADDR     (PT_L2_P_STARTADDR + (PT_L2_P_SIZE * MAX_L2_PT))

/**
 * Asm functions
 */
extern void __mmu_enable_write_buffer(void);
extern void __mmu_set_ttbr(void);
extern void __mmu_set_domain(uint32_t domain);
extern void __mmu_set_ttbcr(uint32_t ttbrc_address);
extern void __mmu_set_ttbr0(uint32_t ttbr_address, uint32_t context);
extern void __mmu_set_ttbr1(uint32_t ttbr_address);
extern void __mmu_enable(void);
extern void __mmu_disable(void);

#endif //SRC_SYSTEM_HAL_OMAP3530_MMU_MMU_H_
