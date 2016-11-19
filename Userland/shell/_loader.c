/* _loader.c */
#include <stdint.h>
#include <stdio.h>
#include <syscall.h>

#include "include/shell.h"

extern char bss;
extern char endOfBinary;

void * memset(void * destiny, int32_t c, uint64_t length);

int _start(int argc, char ** argv) {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	newProcess("&Shell", main, 0, NULL);

	while(true) {}
	
	return 0xDEADBEEF;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}