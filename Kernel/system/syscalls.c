#include "include/mmu.h"
#include "include/syscalls.h"
#include "../include/common.h"
#include "../drivers/include/video.h"
#include "../drivers/include/keyboard.h"
#include "../include/clock.h"
#include "scheduler/scheduler.h"
#include "scheduler/process.h"

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
		case NEWPROCESS:
			//newProcess();
			break;
		case KILLPROCESS:
			killProcess((uint64_t) rbx);
			break;
		case BLOCKPROCESS:
			blockProcess((uint64_t) rbx);
			break;
		case UNBLOCKPROCESS:
			unblockProcess((uint64_t) rbx);
			break;
		case PS:
			printProcesses();
			break;
		case PID:
			*((uintptr_t *) rcx) = (uint64_t) getPID();
			break;
		default:
			write(STDERR, "Error: Invalid system call.", 28);
			break;
	}
}
/*
uint64_t syscall_ps(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
	printProcesses();
	return 1;
}
*/

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