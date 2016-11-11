//BASED ON Wyrm/Process
#ifndef PROCESS_H
#define PROCESS_H

#include "stackFrame.h"
#include "../system/include/mmu.h"

typedef int (*process_entry_t) (int argc, char **argv);

typedef enum {
	P_WAIT, P_RUN, P_BLOCK
}ProcessState;

typedef struct {
	process_entry_t process_entry;

	void* userStack;
	void* kernelStack;

	void* userStackPage;
	void* kernelStackPage;

	uint64_t pid;
	char name[32];
	ProcessState state;
}Process;

//  Use this!  //
Process* newProcess(process_entry_t process_entry, char* name, bool foreground);

//  Used by scheduler/scheduler.c/removeProcessFromScheduler(). Don't use externaly!  //
void freeProcess(Process* process);

#endif
