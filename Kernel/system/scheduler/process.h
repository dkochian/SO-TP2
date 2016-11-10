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

    uint64_t pid;
    char* name;
    char* state;
}Process;

Process * newProcess(void * entryPoint, char* name);

void removeProcess2(Process* process);

#endif
