#include "include/mmu.h"
#include "include/syscalls.h"
#include "../include/common.h"
#include "ipc/include/pipe.h"
#include "ipc/include/mutex.h"
#include "ipc/include/condVar.h"
#include "ipc/include/semaphore.h"
#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "scheduler/include/process.h"
#include "../drivers/include/keyboard.h"
#include "scheduler/include/scheduler.h"

#include "../utils/include/string.h"

static void write(int out, char *str, int size, uint64_t aux);
static void read(int in, char *buffer, uint64_t aux);

void sysCallHandler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
	switch(arg1) {
		case SYSWRITE:
			write(arg2, (char *) arg3, (int) arg4, arg5);
			break;
		case SYSREAD:
			read(arg2, (char *) arg3, arg4);
			break;
		case CLEAR:
			clear();
			break;
		case GETTIME:
			getTime((timeStruct *) arg2);
			break;
		case GETDATE:
			getDate((dateStruct *)arg2);
			break;
		case SLEEP:
			sleep((int) arg2);
			break;
		case DRAWPIXEL:
			printPixel((int) arg2, (int) arg3, (ColorRGB*) arg4);
			break;
		case MALLOC:
			*((uintptr_t *) arg3) = (uint64_t) k_malloc((size_t) arg2);
			break;
		case FREE:
			k_free((void *) arg2);
			break;
		case NEWPROCESS: {
			*((uint64_t *) arg6) = newProcess((char *) arg2, (func) arg3, (int) arg4, (char **) arg5);
		}break;
		case KILLPROCESS:
			freeProcess(arg2);
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
			*((mutex *) arg2) = lockBuild();
			break;
		case MLOCK:
			lock((mutex) arg2);
			break;
		case MUNLOCK:
			unlock((mutex) arg2);
			break;
		case MDESTROY: 
			lockDestroy((mutex) arg2);
			break;
		case MISLOCK:
			*((bool *) arg3) = isLocked((mutex) arg2);
			break;
		case BLOCKSELF:
			blockProcess(getCurrentProcess());
			break;
		case WAITPID:
			waitPid((uint64_t) arg2);
			break;
		case DRAWSQUARE:
			drawSquare((uint16_t) arg2, (uint16_t) arg3, (uint16_t) arg4, (char) arg5);
			break;
		case SEMOPEN:
			*((semaphore *) arg3) = semBuild((int) arg2);
			break;
		case SEMCLOSE:
			semDestroy((semaphore) arg2);
			break;
		case SEMWAIT:
			semWait((semaphore) arg2);
			break;
		case SEMPOST:
			semPost((semaphore) arg2);
			break;
		case CVINIT:
			*((cond_t *) arg2) = cvInitialize();
			break;
		case CVWAIT:
			cvWait((cond_t)arg2,(mutex) arg3);
			break;
		case CVSIG:
			cvSignal((cond_t)arg2);
			break;
		case CVBROAD:
			cvBroadcast((cond_t)arg2);
			break;	
		case CVDESTROY:
			cvDestroy((cond_t)arg2);
			break;
		case PINIT:
			*((uint64_t *) arg2) = pipeBuild();
			break;
		case PDESTROY:
			pipeDestroy(arg2);
			break;
		default:
			write(STDERR, "Error: Invalid system call.", 28, NULL);
			break;
	}
}

static void write(int out, char *str, int size, uint64_t aux) {
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
		case STDCOLOR:
			setDefaultColor(str[0]);
			break;
		case STDBGCOLOR:
			setBgDefaultColor(str[0]);
			break;
		case STDTOAST:
			printToast(str, size);
			break;
		case STDPIPE:
			pipeWrite(aux, str);
			break;
	}
}

static void read(int in, char *buffer, uint64_t aux) {
	if(in == STDIN)
		*buffer = getKey((bool) aux);
	else if(in == STDCOLOR)
		*buffer = getDefaultColor();
	else if(in == STDPIPE)
		pipeRead(aux, buffer);
}