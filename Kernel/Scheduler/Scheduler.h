//BASED ON Wyrm/Scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "ProcessSlot.h"

typedef struct Scheduler {
    ProcessSlot * current;
};

void newScheduler();

void removeScheduler();

void schedule();

void addProcess(Process * process);

void removeProcess(Process * process);

void * switchUserToKernel(void * rsp);

void * switchKernelToUser();

#endif