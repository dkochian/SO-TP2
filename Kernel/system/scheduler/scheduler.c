//BASED ON Wyrm/Scheduler
#include "scheduler.h"
#include "../drivers/include/video.h"

/**********************
**  Current Process  **
**********************/

static int processCounter = 0;
static ProcessSlot* current = NULL;

void schedule() {
	current = current->next;
}

void addProcess(Process* process) {
	ProcessSlot* aux = newProcessSlot();
	aux->process = process;
	if(processCounter==0) {
		aux->next = aux;
		current = aux;
	} else {
		aux->next = current->next;
		current->next = aux;
	}
	processCounter++;
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
	} else {
		processCounter--;
	}
	return;
}

/***********************
**  Helper Functions  **
***********************/

void* switchAtomic(void* rsp) {
	if(processCounter==0)
		return rsp;
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