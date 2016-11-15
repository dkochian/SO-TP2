#include <syscall.h>

void syscall(int id, int type, uintptr_t* addr, char size) {
	_syscall(id, type, (uintptr_t) addr, size, NULL);
}

void write(int type, char* str, int size) {
	_syscall(SYSWRITE, type, (uintptr_t) str, size, NULL);
}

void read(int type, char* str, char aux) {
	_syscall(SYSREAD, type, (uintptr_t) str, aux, NULL);
}

void setColor(char color) {
	_syscall(SYSWRITE, COLOR, (uintptr_t) &color, NULL, NULL);
}

void setBgColor(char color) {
	_syscall(SYSWRITE, BGCOLOR, (uintptr_t) &color, NULL, NULL);
}

void getColor(char* color) {
	_syscall(SYSREAD, COLOR, (uintptr_t) color, NULL, NULL);
}

void clear() {
	_syscall(CLEAR, NULL, NULL, NULL, NULL);
}

void getTime(timeStruct* timeSystem) {
	_syscall(GETTIME, (uintptr_t) timeSystem, NULL, NULL, NULL);
}

void getDate(dateStruct* date) {
	_syscall(GETDATE, (uintptr_t) date, NULL, NULL, NULL);
}

void sleep(int seconds){
	_syscall(SLEEP, seconds, NULL, NULL, NULL);
}

void putPixel(uint16_t x, uint16_t y, ColorRGB* color) {
	_syscall(DRAWPIXEL, x, (uintptr_t) color, y, NULL);
}

void* malloc(size_t size) {
	uintptr_t res = NULL;
	_syscall(MALLOC, size, (uintptr_t) &res, NULL, NULL);

	return (void *) res;
}

void free(void *ptr) {
	_syscall(FREE, (uintptr_t) ptr, NULL, NULL, NULL);
}

void newProcess(char * name, func f, int argc, char** argv) {
    _syscall(NEWPROCESS, (uintptr_t) name, (uintptr_t) f, argc, (uintptr_t) argv);
}

void kill(uint64_t pid) {
	_syscall(KILLPROCESS, pid, NULL, NULL, NULL);
}

void block(uint64_t pid) {
	_syscall(BLOCKPROCESS, pid, NULL, NULL, NULL);
}

void unblock(uint64_t pid) {
	_syscall(UNBLOCKPROCESS, pid, NULL, NULL, NULL);
}

void ps() {
	_syscall(PS, NULL, NULL, NULL, NULL);
}

int pid() {
	int  res = -1;
	_syscall(PID, (uintptr_t) &res, NULL, NULL, NULL);

	return res;
}

void yield() {
	_syscall(YIELD, NULL, NULL, NULL, NULL);
}