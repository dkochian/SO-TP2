//BASED ON Wyrm/Process
#ifndef PROCESS_H
#define PROCESS_H

#include "stackFrame.h"
#include "../system/include/mmu.h"

typedef struct {
    void * entryPoint;

    void * userStack;
    void * kernelStack;

    void * userStackPage;
    void * kernelStackPage;
}Process;

Process * newProcess(void * entryPoint);

void removeProcess2(Process * process);

#endif
