/*
 * mmu.h
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_
#define SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_

#include "../hal.h"

#define MAX_PROCESSES       128         ///< Max proccess count
#define MAX_L2_PT           1024        ///< Max L2 page entries

typedef uint32_t* mmu_pageTableP_t;

/**
 * Table 3.2. First-level descriptor bits + address
 */
typedef struct {
    unsigned int type;                  ///< Page size and validity
    unsigned int CB;                    ///< Cached and buffered
    unsigned int domain;                ///< Domain control
    unsigned int AP;                    ///< Access permission
    uint32_t sectionAddress;            ///< Address of the section
} mmu_l1_section_t;

/**
 * Table 3.7. Second-level descriptor bits + address
 */
typedef struct {
    unsigned int type;                  ///< Page size and validity
    unsigned int CB;                    ///< Cached and buffered
    unsigned int AP;                    ///< Access permission
    uint32_t pageTableAddress;          ///< Address of page table
} mmu_l2_pageTable_t;

/**
 * Needed functions for upper layer
 * Initialize mmu
 * Create/Switch/Kill a process
 */
void mmu_init(void);
//TODO: add process structs
void mmu_create_process(void);
void mmu_switch_process(void);
void mmu_kill_process(void);

#endif //SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_

