#include <syscall.h>

void syscall(int id, int type, uintptr_t* addr, char size, uint64_t aux1, uint64_t aux2) {
	_syscall(id, type, (uintptr_t) addr, size, aux1, aux2);
}

void write(int type, char* str, int size) {
	_syscall(SYSWRITE, type, (uintptr_t) str, size, NULL, NULL);
}

void read(int type, char* str, char aux) {
	_syscall(SYSREAD, type, (uintptr_t) str, aux, NULL, NULL);
}

void setColor(char color) {
	_syscall(SYSWRITE, COLOR, (uintptr_t) &color, NULL, NULL, NULL);
}

void setBgColor(char color) {
	_syscall(SYSWRITE, BGCOLOR, (uintptr_t) &color, NULL, NULL, NULL);
}

void getColor(char* color) {
	_syscall(SYSREAD, COLOR, (uintptr_t) color, NULL, NULL, NULL);
}

void clear() {
	_syscall(CLEAR, NULL, NULL, NULL, NULL, NULL);
}

void getTime(timeStruct* timeSystem) {
	_syscall(GETTIME, (uintptr_t) timeSystem, NULL, NULL, NULL, NULL);
}

void getDate(dateStruct* date) {
	_syscall(GETDATE, (uintptr_t) date, NULL, NULL, NULL, NULL);
}

void sleep(int seconds){
	_syscall(SLEEP, seconds, NULL, NULL, NULL, NULL);
}

void putPixel(uint16_t x, uint16_t y, ColorRGB* color) {
	_syscall(DRAWPIXEL, x, (uintptr_t) color, y, NULL, NULL);
}

void* malloc(size_t size) {
	uintptr_t res = NULL;
	_syscall(MALLOC, size, (uintptr_t) &res, NULL, NULL, NULL);

	return (void *) res;
}

void free(void *ptr) {
	_syscall(FREE, (uintptr_t) ptr, NULL, NULL, NULL, NULL);
}

uint64_t newProcess(char * name, func f, int argc, char** argv) {
	uint64_t res;
    _syscall(NEWPROCESS, (uintptr_t) name, (uintptr_t) f, argc, (uintptr_t) argv, (uintptr_t) &res);
    return res;
}

void kill(uint64_t pid) {
	_syscall(KILLPROCESS, pid, NULL, NULL, NULL, NULL);
}

psContext* ps() {
	uintptr_t res = NULL;
	_syscall(PS, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return (psContext *) res;
}

uint64_t pid() {
	uint64_t res;
	_syscall(PID,(uintptr_t) &res, NULL, NULL, NULL, NULL);
	return res;
}

void yield() {
	_syscall(YIELD, NULL, NULL, NULL, NULL, NULL);
}

mutex sysMutexInit() {
	mutex res = NULL;
	_syscall(MINIT, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return res;
}

void sysMutexLock(mutex m) {
	_syscall(MLOCK, (uintptr_t) m, NULL, NULL, NULL, NULL);
}

void sysMutexUnlock(mutex m) {
	_syscall(MUNLOCK, (uintptr_t) m, NULL, NULL, NULL, NULL);
}

void sysMutexDestroy(mutex m) {
	_syscall(MDESTROY, (uintptr_t) m, NULL, NULL, NULL, NULL);
}

void sysMutexIsLocked(mutex m, bool *res) {
	_syscall(MISLOCK, (uintptr_t) m, (uintptr_t) res, NULL, NULL, NULL);
}

void blockMyself() {
	_syscall(BLOCKSELF, NULL, NULL, NULL, NULL, NULL);
}

void wPid(uint64_t pid) {
	_syscall(WAITPID, pid, NULL, NULL, NULL, NULL);
}

/*void drawSquare(uint16_t x, uint16_t y, uint16_t l, char colorCode) {
	_syscall(DRAWSQUARE, x, y, l, colorCode, NULL);

}*/

semaphore semBuild(int value) {
	semaphore res = NULL;
	_syscall(SEMOPEN, value, (uintptr_t) &res, NULL, NULL, NULL);
	return res;
}

void semDestroy(semaphore s) {
	_syscall(SEMCLOSE, (uintptr_t) s, NULL, NULL, NULL, NULL);
}

void semWait(semaphore s) {
	_syscall(SEMWAIT, (uintptr_t) s, NULL, NULL, NULL, NULL);
}

void semPost(semaphore s) {
	_syscall(SEMPOST, (uintptr_t) s, NULL, NULL, NULL, NULL);
}

void *syscvInit(){
	uintptr_t res = NULL;
	_syscall(CVINIT, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return (void *) res;
}

void syscvWait(void * cv, mutex m){
	_syscall(CVWAIT,(uintptr_t) cv, (uintptr_t) m, NULL, NULL, NULL);
}

uint64_t syscvSignal(void * cv){
	uint64_t res;
	_syscall(CVSIG,(uintptr_t) cv, (uintptr_t) &res, NULL, NULL, NULL);
	return res;
}

void syscvBroadcast(void * cv) {
	_syscall(CVBROAD, (uintptr_t) cv, NULL, NULL, NULL, NULL);
}

void syscvDestroy(void * cv) {
	_syscall(CVDESTROY, (uintptr_t) cv, NULL, NULL, NULL, NULL);
}