//BASED ON Wyrm/Scheduler
#include "scheduler.h"

//TODO: Static?
Scheduler * scheduler;

int timeSlice = TIME_SLICE;
int currentTime = TIME_SLICE;

void roundRobin() {
	currentTime--;
	if(currentTime<=0) {
		schedule();
		currentTime = timeSlice;
	}
}

Scheduler* newScheduler() {
	Scheduler * aux = k_malloc(sizeof(Scheduler));
	return aux;
}

void removeScheduler(Scheduler * scheduler) {
	ProcessSlot* first = scheduler->current;
	ProcessSlot* following = first->next;
	while( first!=following ) {
		ProcessSlot* temp = following->next;
		removeProcessSlot(following);
		following = temp;
	}
	removeProcessSlot(first);
	k_free(scheduler);
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
	schedule();
	return scheduler->current->process->userStack;
}