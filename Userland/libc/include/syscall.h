#ifndef _INC_SYSCALL_
	#define _INC_SYSCALL_

	//TODO: Remove all the system calls that we are not using. ex: blockMyself(), and all the kernel code related to it.

	#include "../../../Common/common.h"

	extern void _syscall(int, int, int, int, int, int);

	void syscall(int id, int type, uintptr_t* addr, char size, uint64_t aux1, uint64_t aux2);
	void write(int type, const char *str, int size, uint64_t aux);
	void read(int type, char* str, uint64_t aux);
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
	
	mutex mutexInit();
	void mutexDestroy(mutex m);
	void lock(mutex m);
	void unlock(mutex m);
	void sysMutexIsLocked(mutex m, bool *res);

	void blockMyself();

	semaphore semBuild(int value);
	void semDestroy(semaphore s);
	void semWait(semaphore s);
	void semPost(semaphore s);

	cond_t syscvInit();
	void syscvWait(cond_t cv, mutex m);
	void syscvSignal(cond_t cv);
	void syscvBroadcast(cond_t cv);
	void syscvDestroy(cond_t cv);

	uint64_t pipeBuild();
	void pipeDestroy(uint64_t id);
	
#endif