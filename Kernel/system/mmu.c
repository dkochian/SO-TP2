#include "../../Common/common.h"
#include "include/mmu.h"


static void *memory_stack[PAGE_TABLE_ENTRIES];
static int current = 0;

void k_initialize() {
	for(current = 0; current < PAGE_TABLE_ENTRIES; current++) {
		memory_stack[current] = (void *) (DATA_ADDRESS + current * PAGE_SIZE + (void *) 1);
	}
}

void * k_malloc(size_t size) {
	if(current == -1)
		return NULL;

	return memory_stack[current--];
}

void k_free(void * page) {
	memory_stack[++current] = page;
}