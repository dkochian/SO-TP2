#ifndef _INC_SYSCALL_
	#define _INC_SYSCALL_

	#include "../../../Common/common.h"

	extern void _syscall(int, int, int, int);

	void syscall(int id, int type, uintptr_t* addr, char size);
	void write(int type, char* str, int size);
	void read(int type, char* str, char aux);
	void setColor(char color);
	void setBgColor(char color);
	void getColor(char* color);
	void getDate(dateStruct* date);
	void getTime(timeStruct* timeSystem);
	void clear();
	void sleep(int seconds);
	void putPixel(uint16_t x, uint16_t y, ColorRGB* color);
	void* malloc(size_t size);
	void free(void *ptr);
	void newProcess();
	void kill(uint64_t pid);
	void block(uint64_t pid);
	void unblock(uint64_t pid);
	void ps();
	int pid();

#endif