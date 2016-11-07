#include "include/mmu.h"
#include "include/syscalls.h"
#include "../include/common.h"
#include "../drivers/include/video.h"
#include "../drivers/include/keyboard.h"
#include "../include/clock.h"

static void write(int out, char* str, int size);
static void read(int in, char* buffer, char aux);

void sysCallHandler(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx) {
	switch(rax) {
		case SYSWRITE:
			write(rbx, (char*) rcx, (int) rdx);
			break;
		case SYSREAD:
			read(rbx, (char*) rcx, (char) rdx);
			break;
		case CLEAR:
			clear();
			break;
		case GETTIME:
			getTime((timeStruct*) rbx);
			break;
		case GETDATE:
			getDate(rbx);
			break;
		case SLEEP:
			sleep((int) rbx);
			break;
		case DRAWPIXEL:
			printPixel((int) rbx, (int) rdx, (ColorRGB*) rcx);
			break;
		case MALLOC:
			*((uintptr_t *) rcx) = (uint64_t) k_malloc((size_t) rbx);
			break;
		case FREE:
			k_free((void *) rbx);
			break;
		default:
			write(STDERR, "Error: Invalid system call.", 28);
			break;
	}
}

static void write(int out, char* str, int size) {
	switch(out) {
		case STDERR: {
			int
				i = 0;

			while(i < size)
				putChar(str[i++], COLOR_ERROR);
			break;
		}
		case STDOUT: {
			int
				i = 0;

			while(i < size)
				putChar(str[i++], -1);
			break;
		}
		case COLOR:
			setDefaultColor(str[0]);
			break;
		case BGCOLOR:
			setBgDefaultColor(str[0]);
			break;
		case TOAST:
			printToast(str, size);
			break;
	}
}

void read(int in, char* buffer, char aux) {
	if(in == STDIN)
		*buffer = getKey(aux);
	else if(in == COLOR)
		*buffer = getDefaultColor();
}