//BASED ON Wyrm/ProcessSlot
#include "processSlot.h"

ProcessSlot * newProcessSlot() {
    ProcessSlot * processSlot = k_malloc(sizeof(ProcessSlot));
    return processSlot;
}

void removeProcessSlot(ProcessSlot * processSlot) {
    //TODO: Decidir mas adelante si aca deberia removerse el process.
    k_free(processSlot);
}