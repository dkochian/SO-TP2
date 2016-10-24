#ifndef _INC_SYSCALLS_
	#define _INC_SYSCALLS_

	#include "../include/common.h"

	void sysCallHandler(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);
	//void read(int in, char* buffer);

#endif
