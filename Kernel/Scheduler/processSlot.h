//BASED ON Wyrm/ProcessSlot
#ifndef PROCESS_SLOT_H
#define PROCESS_SLOT_H

#include "process.h"

typedef struct {
    struct ProcessSlot* next;
    Process* process;
}ProcessSlot;

ProcessSlot * newProcessSlot();

void removeProcessSlot(ProcessSlot * processSlot);

#endif