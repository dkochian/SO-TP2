//BASED ON Wyrm/Process
#ifndef PROCESS_H
#define PROCESS_H

#include "StackFrame.h"
#include "memory_manager.h"

typedef struct Process {
    void * entryPoint;

    void * userStack;
    void * kernelStack;

    void * userStackPage;
    void * kernelStackPage;
};

Process * newProcess(void * entryPoint);

void removeProcess(Process * process);

static void * toStackAddress(void * page);

static void * fillStackFrame(void * entryPoint, void * userStack);

#endif
