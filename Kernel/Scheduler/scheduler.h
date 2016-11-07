//BASED ON Wyrm/Scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "processSlot.h"
/*
typedef struct {
    ProcessSlot * current;
}Scheduler;*/

#define TIME_SLICE 5

void roundRobin();

/*Scheduler* newScheduler();

void removeScheduler();
*/
void schedule();

void addProcess(Process * process);

void removeProcess(Process * process);

void * switchUserToKernel(void * rsp);

void * switchKernelToUser();

#endif