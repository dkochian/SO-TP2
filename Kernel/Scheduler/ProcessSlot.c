//BASED ON Wyrm/ProcessSlot
#include "ProcessSlot.h"

ProcessSlot * newProcessSlot() {
    ProcessSlot * processSlot = malloc(sizeof(ProcessSlot));
    return processSlot;
}

void removeProcessSlot(ProcessSlot * processSlot) {
    //TODO: Decidir mas adelante si aca deberia removerse el process.
    free(processSlot);
}