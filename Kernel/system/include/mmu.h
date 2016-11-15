#ifndef __INC_MMU__

	#define __INC_MMU__

	#include "../../Common/common.h"
	
	#define PAGE_SIZE			4*1024
	#define DATA_ADDRESS 		0x500000
	#define PAGE_TABLE_ENTRIES	255

	void k_initialize();
	void *k_malloc(size_t size);
	void k_free(void* ptr);

#endif