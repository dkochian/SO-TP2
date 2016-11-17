#include "include/mmu.h"
#include "include/syscalls.h"
#include "../include/common.h"
#include "../drivers/include/video.h"
#include "../drivers/include/keyboard.h"
#include "../include/clock.h"
#include "../include/mutex.h"
#include "scheduler/include/scheduler.h"
#include "scheduler/include/process.h"

static void write(int out, char* str, int size);
static void read(int in, char* buffer, char aux);

void sysCallHandler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
	switch(arg1) {
		case SYSWRITE:
			write(arg2, (char*) arg3, (int) arg4);
			break;
		case SYSREAD:
			read(arg2, (char*) arg3, (char) arg4);
			break;
		case CLEAR:
			clear();
			break;
		case GETTIME:
			getTime((timeStruct*) arg2);
			break;
		case GETDATE:{
			print("adentro DAte",-1);
			getDate(arg2);
		}
			break;
		case SLEEP:
			sleep((int) arg2);
			break;
		case DRAWPIXEL:
			printPixel((int) arg2, (int) arg4, (ColorRGB*) arg3);
			break;
		case MALLOC:
			*((uintptr_t *) arg3) = (uint64_t) k_malloc((size_t) arg2);
			break;
		case FREE:
			k_free((void *) arg2);
			break;
		case NEWPROCESS:
			newProcess((char *) arg2, (func) arg3, (int)arg4, (char**)arg5);
			break;
		case KILLPROCESS:
			removeProcess(getProcessFromId((uint64_t)arg2));
			break;
		case PS:
			*((uintptr_t *) arg2) = (uint64_t) processesStatus();
			break;
		case PID:
			*((uintptr_t *) arg2) = getCurrentProcess()->id;
			break;
		case YIELD:
			_yield();
			break;
		case MINIT:
			*((uintptr_t *) arg3) = (uint64_t) initLock();
			break;
		case MLOCK:
			lock((mutex*) arg3);
			break;
		case MUNLOCK:
			unlock((mutex*) arg3);
			break;
		case MDESTROY:
			destroyLock((mutex*) arg3);
			break;
		case BLOCKSELF:
			blockProcess(getCurrentProcess()->id);
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
	if(in == STDIN) {
		process *p = getForeground();
		*buffer = getKey(aux, p);
	}
	else if(in == COLOR)
		*buffer = getDefaultColor();
}