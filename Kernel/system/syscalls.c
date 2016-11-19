#include "include/mmu.h"
#include "include/syscalls.h"
#include "../include/common.h"
#include "ipc/include/mutex.h"
#include "ipc/include/semaphore.h"
#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "scheduler/include/process.h"
#include "../drivers/include/keyboard.h"
#include "scheduler/include/scheduler.h"

static void write(int out, char* str, int size);
static void read(int in, char* buffer, char aux);

void sysCallHandler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
	switch(arg1) {
		case SYSWRITE:
			write(arg2, (char *) arg3, (int) arg4);
			break;
		case SYSREAD:
			read(arg2, (char *) arg3, (char) arg4);
			break;
		case CLEAR:
			clear();
			break;
		case GETTIME:
			getTime((timeStruct*) arg2);
			break;
		case GETDATE:
			getDate(arg2);
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
			*((uintptr_t *) arg6) = newProcess((char *) arg2, (func) arg3, (int)arg4, (char**)arg5);
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
			*((mutex *) arg2) = initLock();
			break;
		case MLOCK:
			lock((mutex) arg2);
			break;
		case MUNLOCK:
			unlock((mutex) arg2);
			break;
		case MDESTROY: 
			destroyLock((mutex) arg2);
			break;
		case MISLOCK:
			*((bool *) arg3) = isLocked((mutex) arg2);
			break;
		case BLOCKSELF:
			blockProcess(getCurrentProcess()->id);
			break;
		case WAITPID:
			waitPid((uint64_t) arg2);
			break;
		case DRAWSQUARE:
			drawSquare((uint16_t) arg2, (uint16_t) arg3, (uint16_t) arg4, (char) arg5);
			break;
		case SEMOPEN:
			*((uintptr_t *) arg2) = (uint64_t) semOpen((char *) arg3, (int)  arg4);
			break;
		case SEMCLOSE:
			semClose((sem_t) arg2);
			break;
		case SEMWAIT:
			semWait((sem_t) arg2);
			break;
		case SEMPOST:
			semPost((sem_t) arg2);
			break;
		/*case CVINIT:
			*((uintptr_t *) arg2) = cvInitialize();
			break;
		case CVWAIT:
			cvWait((cond_t)arg2,(sem_t) arg3);
			break;
		case CVSIG:
			*((uintptr_t *) arg3) = cvSignal((cond_t)arg2);
			break;
		case CVBROAD:
			cvBroadcast((cond_t)arg2);
			break;	
		case CVDESTROY:
			cvDestroy((cond_t)arg2);
			break;*/
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