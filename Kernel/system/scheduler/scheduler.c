//BASED ON Wyrm/Scheduler
#include "scheduler.h"

//TODO: Static?
//Scheduler * scheduler;

/**********************
**  Current Process  **
**********************/

static ProcessSlot* current = NULL;

void initScheduler() {
	if(current!=NULL)
		return;
	current = newProcessSlot();
	current->next = current;
}

void schedule() {
	current = current->next;
}

void addProcess(Process* process) {
	ProcessSlot* aux = newProcessSlot();
	aux->process = process;
	aux->next = current->next;
	current->next = aux;
}

void removeProcess(Process* process) {
	int found = 0;
	if (process == NULL) {
		return;
		//TODO: log de errores.
		//fprintf(stderr, "Failed to remove process: Process is NULL\n");
	}
	ProcessSlot* first = current;
	ProcessSlot* prev = first;
	ProcessSlot* aux = prev->next;
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
	return;
}

/******************
**  Round Robin  **
******************/
/*
int timeSlice = TIME_SLICE;
int currentTime = TIME_SLICE;

void roundRobin() {
	currentTime--;
	if(currentTime<=0) {
		schedule();
		currentTime = timeSlice;
	}
}
*/
/***********************
**  Helper Functions  **
***********************/

void* switchAtomic(void* rsp) {
	current->process->userStack = rsp;
	schedule();
	return current->process->userStack;
}

void* switchUserToKernel(void* rsp) {
	Process* process = current->process;
	process->userStack = rsp;
	return process->kernelStack;
}

void* switchKernelToUser() {
	schedule();
	return current->process->userStack;
}