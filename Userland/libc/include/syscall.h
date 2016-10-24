#ifndef _INC_SYSCALL_
	#define _INC_SYSCALL_

	#include "../../include/common.h"

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

#endif