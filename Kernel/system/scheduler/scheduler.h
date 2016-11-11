//BASED ON Wyrm/Scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "processSlot.h"

/**********************
**  Current Process  **
**********************/
//  Used by switchAtomic() and switchKernelToUser(). Don't use externaly!
//  static void schedule();

//  Used by scheduler/process.c/newProcess(). Don't use externaly!  //
void addProcessToScheduler(Process* process, bool foreground);

//  Use this!  //
void removeProcessFromScheduler(Process* process);

/***********************
**  Helper Functions  **
***********************/
void* switchUserToKernel(void* rsp);
void* switchKernelToUser();
void* switchAtomic(void* rsp);

bool unblockProcess(uint64_t pid);
bool blockProcess(uint64_t pid);
bool killProcess(uint64_t pid);
Process *getProcess(uint64_t pid);
Process *getCurrProcess();
uint64_t getPID();
void printProcesses();

// void yield();

#endif