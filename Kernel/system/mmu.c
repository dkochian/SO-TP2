#include "../../Common/common.h"
#include "include/mmu.h"

static uintptr_t **free_mem_table = (uintptr_t **) PAGE_TABLE;
static int current;
static char init = FALSE;

void k_initialize() {
	for(current = 0; current < MEM_BASE-PAGE_TABLE; current++)
		free_mem_table[current] = (uintptr_t *) MEM_BASE + current*PAGE_SIZE;

	current--;
}

void *k_malloc(size_t size) {
	if(!init) {
		k_initialize();
		init = TRUE;
	}
	if(current == -1)
		return NULL;

	return (void *) &free_mem_table[current--];
}

void k_free(void *ptr) {
	free_mem_table[++current] = (uintptr_t *) ptr;
}