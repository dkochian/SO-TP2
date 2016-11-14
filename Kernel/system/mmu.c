#include "../../Common/common.h"
#include "include/mmu.h"


static void * memory_stack[PAGE_TABLE_ENTRIES];
static int current = 0;

void k_initialize() {

	for(current = 0; current < PAGE_TABLE_ENTRIES; current++) {
		memory_stack[current] = (void *) (DATA_ADDRESS + current * PAGE_SIZE);
	}
}

void * k_malloc(size_t size) {
	if(current == -1) {
		return NULL;
	}
	return memory_stack[current--];
}

void k_free(void * page) {
	memory_stack[++current] = page;
}

//static char init = false;
//static uintptr_t free_mem_table[PAGE_TABLE_ENTRIES];
//static int current = 0;
//
//void k_initialize() {
//
//	for(current = 0; current < PAGE_TABLE_ENTRIES - 1; current++)
//		free_mem_table[current] = (uintptr_t )(current + 1);
//}
//
//void * k_malloc(size_t size) {
//	if(!init) {
//		k_initialize();
//		init = true;
//	}
//	if(current == -1)
//		return NULL;
//
//	return (void *) MEM_BASE + free_mem_table[current--]*PAGE_SIZE ;
//}
//
//void k_free(void *ptr) {
//	free_mem_table[++current] = (uintptr_t) ptr;
//}

/*static char*
	mem = (char*) 0x3C00;//0xF000;
void k_initialize() {
	return;
}
void *k_malloc(size_t size){
	void* result = mem;
	mem += size;
	return result;
}
void k_free(void *ptr) {
	return;
}*/