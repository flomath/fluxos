/*
 * mmu.h
 *
 * Created on: 26.03.15
 *     Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_
#define SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_

#include "../hal.h"

typedef struct {
    uint32_t ptAddress;
    uint32_t masterPtAddress;
} mmu_pt_t;

/**
 * Table 3.2. First-level descriptor bits
 */
typedef struct {
    uint32_t type;              ///< Page size and validity
    uint32_t CB;                ///< Cacheable and buffered
    uint32_t domain;            ///< Domain control
    uint32_t AP;                ///< Access permission
    uint32_t vAddress;          ///< Virtual address
    uint32_t pAddress;          ///< Physical address
    mmu_pt_t* pt;               ///< Current page table description
} mmu_l1_pt_t;

/**
 * Table 3.7. Second-level descriptor bits
 */
typedef struct {
    uint32_t type;              ///< Page size and validity
    uint32_t CB;                ///< Cacheable and buffered
    uint32_t AP0;               ///< Access permission
    uint32_t AP1;               ///< Access permission
    uint32_t AP2;               ///< Access permission
    uint32_t AP3;               ///< Access permission
    mmu_pt_t* pt;               ///< Current page table description
    mmu_l1_pt_t* l1_pt;         ///< L1 page table
} mmu_l2_pt_t;

void mmu_init(void);

// input param task/process
void mmu_create_process(void*);
void mmu_kill_process(void*);

#endif //SRC_SYSTEM_HAL_COMMON_MMU_MMU_H_

