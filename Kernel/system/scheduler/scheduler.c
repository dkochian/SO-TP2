//BASED ON Wyrm/Scheduler
#include "scheduler.h"
#include "../drivers/include/video.h"
#include "../mutex.h"
#include "../../drivers/include/video.h"

/**********************
**  Current Process  **
**********************/

static int processCounter = 0;
static ProcessSlot* current = NULL;
static bool volatile mutex;

int strcmp(char *str1, char *str2)//ver despues donde meter la funcion
{
	while((*str1 == *str2) && (*str1 != '\0')) {
		str1++;
		str2++;
	}

	if(*str1 > *str2)
		return 1;

	if(*str1 < *str2)
		return -1;

	return 0;
}

void schedule() {
	ProcessSlot* aux = current->next;
	bool found = FALSE;

	while(!found){
		if (strcmp(aux->process->state, "WAITING") == 0){
			current = aux;
			found = TRUE;
		}else
			aux = aux->next;
	}
}

void addProcess(Process* process) {
	ProcessSlot* aux = newProcessSlot();
	lock(&mutex);
	aux->process = process;
	if(processCounter==0) {
		aux->next = aux;
		current = aux;
	} else {
		aux->next = current->next;
		current->next = aux;
	}
	processCounter++;
	unlock(&mutex);
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
			if (aux == current){
				current = aux->next;
			}
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



Process *getProcess(uint64_t pid){
	int found = 0;
	ProcessSlot* resp = current;

	if(resp->process == NULL){
		return NULL;
	}

	do{
		if(resp->process->pid == pid)
			found = 1;
		else
			resp = resp->next;
	}while(!found && current != resp);

	if(!found)
		resp->process = NULL;

	return resp->process;
}

Process *getCurrProcess(){
	return current->process;
}

uint64_t getPID(){
	return current->process->pid;
}

bool killProcess(uint64_t pid){
	if(pid < 1)
		return FALSE;
	lock(&mutex);
	Process *process = getProcess(pid);
	if (process == NULL){
		unlock(&mutex);
		return FALSE;
	}

	removeProcess(process);
	unlock(&mutex);
	return TRUE;
}

bool blockProcess(uint64_t pid){
	lock(&mutex);
	Process *process = getProcess(pid);
	if(process == NULL){
		unlock(&mutex);
		return FALSE;
	} else if (current->process == process){
		schedule();
	}
	process->state = "BLOCKED";
	unlock(&mutex);
	return TRUE;
}

bool unblockProcess(uint64_t pid){
	lock(&mutex);
	Process *process = getProcess(pid);
	if(process == NULL){
		unlock(&mutex);
		return FALSE;
	}
	process->state = "WAITING";
	unlock(&mutex);
	return TRUE;
}

void printProcesses(){
	lock(&mutex);
	ProcessSlot* aux = current;

	if(aux->process == NULL){
		return;
	}
	print("PID -------------Process Name",-1);
	do{
		printDec(aux->process->pid,-1);
		print(aux->process->name,-1);
		//putchar('\n',-1);
		
		aux = aux->next;
	}while(current != aux);
	
	unlock(&mutex);
}

