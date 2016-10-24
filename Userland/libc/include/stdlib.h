#ifndef _INC_STDLIB_
	#define _INC_STDLIB_

	#define MEM_START 0xF000

	void* malloc(int size);
	void free(void* ptr);

#endif