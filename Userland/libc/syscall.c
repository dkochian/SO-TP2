#include <syscall.h>

void syscall(int id, int type, uintptr_t* addr, char size, uint64_t aux1, uint64_t aux2) {
	_syscall(id, type, (uintptr_t) addr, size, aux1, aux2);
}

void write(int type, const char *str, int size, uint64_t aux) {
	_syscall(SYSWRITE, type, (uintptr_t) str, size, aux, NULL);
}

void read(int type, char *str, uint64_t aux) {
	_syscall(SYSREAD, type, (uintptr_t) str, aux, NULL, NULL);
}

void setColor(char color) {
	_syscall(SYSWRITE, STDCOLOR, (uintptr_t) &color, NULL, NULL, NULL);
}

void setBgColor(char color) {
	_syscall(SYSWRITE, STDBGCOLOR, (uintptr_t) &color, NULL, NULL, NULL);
}

void getColor(char* color) {
	_syscall(SYSREAD, STDCOLOR, (uintptr_t) color, NULL, NULL, NULL);
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
	_syscall(DRAWPIXEL, x, y, (uintptr_t) color, NULL, NULL);
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

mutex mutexInit() {
	mutex res = NULL;
	_syscall(MINIT, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return res;
}

void mutexDestroy(mutex m) {
	_syscall(MDESTROY, (uintptr_t) m, NULL, NULL, NULL, NULL);
}

void lock(mutex m) {
	_syscall(MLOCK, (uintptr_t) m, NULL, NULL, NULL, NULL);
}

void unlock(mutex m) {
	_syscall(MUNLOCK, (uintptr_t) m, NULL, NULL, NULL, NULL);
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

cond_t syscvInit(){
	cond_t res = NULL;
	_syscall(CVINIT, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return res;
}

void syscvWait(cond_t cv, mutex m){
	_syscall(CVWAIT,(uintptr_t) cv, (uintptr_t) m, NULL, NULL, NULL);
}

void syscvSignal(cond_t cv){
	_syscall(CVSIG,(uintptr_t) cv, NULL, NULL, NULL, NULL);
}

void syscvBroadcast(cond_t cv) {
	_syscall(CVBROAD, (uintptr_t) cv, NULL, NULL, NULL, NULL);
}

void syscvDestroy(cond_t cv) {
	_syscall(CVDESTROY, (uintptr_t) cv, NULL, NULL, NULL, NULL);
}

uint64_t pipeBuild() {
	uint64_t res;
	_syscall(PINIT, (uintptr_t) &res, NULL, NULL, NULL, NULL);
	return res;
}

void pipeDestroy(uint64_t id) {
	_syscall(PDESTROY, id, NULL, NULL, NULL, NULL);
}