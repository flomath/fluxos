/*
 * memoryManager.h
 *
 *  Created on: 07.05.2015
 *      Author: Nino Schoch
 */

#ifndef SRC_SYSTEM_HAL_COMMON_MMU_MEMORYMANAGER_H_
#define SRC_SYSTEM_HAL_COMMON_MMU_MEMORYMANAGER_H_

#include "../hal.h"

/**
 * Status of a page
 */
typedef struct
{
	bool_t reserved;						///< Status
	unsigned int processID;					///< ID of process
} page_t;

/**
 * Region table
 */
typedef struct {
	uint32_t addressStart;             		///< Virtual address start
	uint32_t addressEnd;               		///< Virtual address end
	unsigned int pageSize;     	     		///< Page size and validity
	unsigned int numPages;               	///< Number of pages
	unsigned int AP;	                  	///< Access permission
	unsigned int CB;                      	///< Cached and buffered
	unsigned int numPagesReserved;			///< Number of reserved pages
	page_t* pages;							///< Pages
} memoryRegion_t;


#endif /* SRC_SYSTEM_HAL_COMMON_MMU_MEMORYMANAGER_H_ */
