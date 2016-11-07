#include "../../Common/common.h"
#include "include/mmu.h"

static uintptr_t **free_mem_table = (uintptr_t **) PAGE_TABLE;
static int current;

void k_initialize() {
	for(current = 0; current < MEM_BASE-PAGE_TABLE; current++)
		free_mem_table[current] = (uintptr_t *) MEM_BASE + current*PAGE_SIZE;

	current--;
}

void *k_malloc(size_t size) {
	if(current == -1)
		return NULL;

	print("[MMU] Malloc:", -1);
	printNewline();
	print("[MMU] &table[c]: 0x", -1);
	printHex((uint64_t) &free_mem_table[current], -1);
	printNewline();

	return (void *) &free_mem_table[current--];
}

void k_free(void *ptr) {
	print("[MMU] Free:", -1);
	printNewline();
	print("[MMU] current: ", -1);
	printDec(current, -1);
	printNewline();
	print("[MMU] ptr: 0x", -1);
	printHex((uint64_t) ptr, -1);
	printNewline();

	print("[MMU] &ptr: 0x", -1);
	printHex((uint64_t) &ptr, -1);
	printNewline();

	free_mem_table[++current] = (uintptr_t *) ptr;
}