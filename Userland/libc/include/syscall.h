#ifndef _INC_SYSCALL_
	#define _INC_SYSCALL_

	#include "../../../Common/common.h"

	extern void _syscall(int, int, int, int, int, int);

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

	uint64_t newProcess(char * name, func f, int argc, char** argv);
	void kill(uint64_t pid);
	void block(uint64_t pid);
	void unblock(uint64_t pid);
	psContext *ps();
	uint64_t pid();
	void wPid(uint64_t pid);
	void rPid(uint64_t pid);

	void yield();
	void* sysMutexInit();
	void sysMutexLock(void* m);
	void sysMutexUnlock(void* m);
	void sysMutexDestroy(void* m);
	void sysMutexIsLocked(void *m, bool *res);

	void blockMyself();
	void drawSquare(uint16_t x, uint16_t y, uint16_t l, char color);

#endif