#include <syscall.h>

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

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static void printBase(uint64_t value, uint32_t base, char color);
static void printHex(uint64_t value, char color);

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

static void printBase(uint64_t value, uint32_t base, char color) {
	char
		buffer[64] = {'0'};

	uintToBase(value, buffer, base);
	print(buffer);
}

static void printHex(uint64_t value, char color) {
	printBase(value, 16, color);
}

void* malloc(size_t size) {
	uintptr_t res = NULL;
	_syscall(MALLOC, size, (uintptr_t) &res, NULL);

	/*print("[SYS] res: 0x", -1);
	printHex((uint64_t) res, -1);
	printNewline();

	print("[SYS] &res: 0x", -1);
	printHex((uint64_t) &res, -1);
	printNewline();*/

	/*print("[SYS] *res: 0x", -1);
	printHex((uint64_t) *res, -1);
	printNewline();*/

	return (void *) res;
}

void free(void *ptr) {
	_syscall(FREE, (uintptr_t) ptr, NULL, NULL);
}