#ifndef __INC_MMU__

	#define __INC_MMU__

	#include "../../Common/common.h"
	
	#define PAGE_TABLE			0x0000F000
	#define PAGE_SIZE			4*1024
	#define MEM_BASE			(0x0000F000 + PAGE_TABLE_ENTRIES)
	#define PAGE_TABLE_ENTRIES	256

	void k_initialize();
	void *k_malloc(size_t size);
	void k_free(void* ptr);

#endif