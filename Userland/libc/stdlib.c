#include <stdlib.h>

static char*
	mem = (char*) MEM_START;

void * malloc(int size){
	void* result = mem;
	mem += size;
	return result;
}

void free(void *ptr){
	return;
}