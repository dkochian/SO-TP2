//BASED ON Wyrm/Scheduler
#include "Scheduler.h"

//TODO: Static?
Scheduler * scheduler;

Scheduler * newScheduler() {
    Scheduler * aux = malloc(sizeof(Scheduler));
    return aux;
}

void removeScheduler(Scheduler * scheduler) {
    //TODO: Ver de remover todos los ProcessSlot con removeProcessSlot()
    free(scheduler);
}

void schedule() {
    scheduler->current = scheduler->current->next;
}

void addProcess(Process * process) {
    ProcessSlot * aux = newProcessSlot();
    aux->process = process;
    aux->next = scheduler->current->next;
    scheduler->current->next = aux;
}

void removeProcess(Process * process) {

    int found = 0;

    if (process == NULL) {
        //TODO: log de errores.
        //fprintf(stderr, "Failed to remove process: Process is NULL\n");
    }

    ProcessSlot * first = scheduler->current;
    ProcessSlot * prev = first;
    ProcessSlot * aux = prev->next;

    do {
        if (aux->process == process) {
            prev->next = aux->next;
            removeProcessSlot(aux);
            found = 1;
            break;
        }
        prev = aux;
        aux = aux->next;
    } while (prev != first);

    if (!found) {
        //TODO: log de errores.
        //fprintf(stderr, "Failed to remove process: Process not found on Scheduler list\n");
    }

}

void * switchUserToKernel(void * rsp) {
    Process * process = scheduler->current->process;
    process->userStack = rsp;
    return process->kernelStack;
}

void * switchKernelToUser() {
    scheduler->schedule();
    return scheduler->current->process->userStack;
}