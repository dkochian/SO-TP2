#include "../../Common/common.h"
#include "include/mmu.h"

/*void **free_mem_table = (void *) PAGE_TABLE;
int current = 0;

void k_initialize() {
	//for(uintptr_t current_addr = (uintptr_t) MEM_BASE; current_addr <= MEM_FINISH; current_addr += (uintptr_t) PAGE_SIZE)
	//	free_mem_table[current++] = current_addr;
	uintptr_t *current_addr = (uintptr_t *) MEM_FINISH;

	while(current_addr >= MEM_BASE) {
		free_mem_table[current++] = (void *) current_addr;
		current_addr -= PAGE_SIZE;
	}
}

void *k_malloc(size_t size) {
	if(current == -1)
		return NULL;

	print("[MMU] table[c]: 0x", -1);
	printHex((uint64_t) free_mem_table[current], -1);
	printNewline();

	print("[MMU] &table[c]: 0x", -1);
	printHex((uint64_t) &free_mem_table[current], -1);
	printNewline();

	print("[MMU] *table[c]: 0x", -1);
	printHex((uint64_t) *free_mem_table[current], -1);
	printNewline();

	return (void *) &free_mem_table[current--];
}

void k_free(void *ptr) {
	free_mem_table[++current] = (char) ptr;
}*/

static uintptr_t *
	mem = (uintptr_t*) 0xF000;

void *k_malloc(size_t size) {
	void *result = (void *) mem;
	mem += size;

	/*print("[MMU] result: 0x", -1);
	printHex((uint64_t) result, -1);
	printNewline();

	print("[MMU] &result: 0x", -1);
	printHex((uint64_t) &result, -1);
	printNewline();

	print("[MMU] *result: 0x", -1);
	printHex((uint64_t) *((uintptr_t *) result), -1);
	printNewline();*/

	return result;
}

void k_free(void *ptr) {
	return;
}