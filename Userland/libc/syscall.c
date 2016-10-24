#include <syscall.h>
#include <stdio.h>

void syscall(int id, int type, uintptr_t* addr, char size) {
	_syscall(id, type, (uintptr_t) addr, size);
}

void write(int type, char* str, int size) {
	_syscall(SYSWRITE, type, (uintptr_t) str, size);
}

void read(int type, char* str, char aux) {
	_syscall(SYSREAD, type, (uintptr_t) str, aux);
}

void setColor(char color) {
	_syscall(SYSWRITE, COLOR, (uintptr_t) &color, NULL);
}

void setBgColor(char color) {
	_syscall(SYSWRITE, BGCOLOR, (uintptr_t) &color, NULL);
}

void getColor(char* color) {
	_syscall(SYSREAD, COLOR, (uintptr_t) color, NULL);
}

void clear() {
	_syscall(CLEAR, NULL, NULL, NULL);
}

void getTime(timeStruct* timeSystem) {
	_syscall(GETTIME, (uintptr_t) timeSystem, NULL, NULL);
}

void getDate(dateStruct* date) {
	_syscall(GETDATE, (uintptr_t) date, NULL, NULL);
}

void sleep(int seconds){
	_syscall(SLEEP, seconds, NULL, NULL);
}

void putPixel(uint16_t x, uint16_t y, ColorRGB* color) {
	_syscall(DRAWPIXEL, x, (uintptr_t) color, y);
}