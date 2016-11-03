#include "../../Common/common.h"
#include "include/mmu.h"

char *free_mem_table = (char *) PAGE_TABLE;
int current = 0;

void k_initialize() {
	for(char current_addr = (char) MEM_BASE; current_addr <= MEM_FINISH; current_addr += (char) PAGE_SIZE)
		free_mem_table[current++] = current_addr;
}

void *k_malloc(size_t size) {
	if(current == -1)
		return NULL;

	return (void *) free_mem_table[current--];
}

void k_free(void *ptr) {
	free_mem_table[++current] = (char) ptr;
}