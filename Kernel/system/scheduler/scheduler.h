//BASED ON Wyrm/Scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "processSlot.h"

/**********************
**  Current Process  **
**********************/
void schedule();
void addProcess(Process* process);
void removeProcess(Process* process);

/***********************
**  Helper Functions  **
***********************/
void* switchUserToKernel(void* rsp);
void* switchKernelToUser();
void* switchAtomic(void* rsp);

#endif