#ifndef __INC_MMU__

	#define __INC_MMU__

	#include "../../Common/common.h"
	
	#define PAGE_TABLE		0xF0000
	#define PAGE_SIZE		0x00001	//4*1024=4096

	#define MEM_BASE		0xF1000
	#define MEM_FINISH		0x20000- MEM_BASE//512*1024*1024 - MEM_BASE

	//void k_initialize();
	void *k_malloc(size_t size);
	void k_free(void* ptr);

#endif