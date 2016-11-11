//BASED ON Wyrm/Scheduler
#include "scheduler.h"
#include "../drivers/include/video.h"
#include "../mutex.h"
#include "../../drivers/include/video.h"

/**********************
**  Current Process  **
**********************/

static bool volatile mutex;			// grab this before touching any other variable
static int processCounter = 0;
static ProcessSlot* current = NULL;
static ProcessSlot* foreground;

static ProcessSlot* foregroundDefault;	// set with shell on first add

//ver despues donde meter la funcion. klib?
int strcmp(char *str1, char *str2) {
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

static void schedule() {
	if( !isLockOpenRightThisInstant(&mutex) )
		return;
	ProcessSlot* aux = current;
	bool found = FALSE;
	while(!found){
		aux = aux->next;
		switch(aux->process->state) {
			case P_WAIT: {
				// next in line
				current->process->state = P_WAIT;
				aux->process->state = P_RUN;
				current = aux;
				found = TRUE;
			}
			break;
			case P_RUN: {
				// merry go round
				found = TRUE;
			}
			break;
			case P_BLOCK:
			default: {
				// keep going round
				;
			}
		}
	}
}

// Used by scheduler/process.c/newProcess(). Don't use externaly!
void addProcessToScheduler(Process* process, bool f) {
	ProcessSlot* aux = newProcessSlot();
	process->state = P_WAIT;
	aux->process = process;
	lock(&mutex);
	if(processCounter==0) {
		foregroundDefault = aux;
		aux->next = aux;
		current = aux;
	} else {
		aux->next = current->next;
		current->next = aux;
	}
	if(f) {
		foreground = aux;
	}
	processCounter++;
	unlock(&mutex);
}

void removeProcessFromScheduler(Process* process) {
	if( strcmp(process->name, "SHELL")==0 ) {
		// can't kill the shell
		return;
	}
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
			if( foregroundDefault == aux) {
				// can't kill foregroundDefault (i.e. SHELL)
				break;
			}
			if (aux == current){
				current = aux->next;
			}
			prev->next = aux->next;
			if(aux == foreground)
				foreground = foregroundDefault;
			freeProcess(aux->process);
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



Process* getProcess(uint64_t pid){
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

Process* getCurrProcess(){
	return current->process;
}

uint64_t getPID(){
	printDec(processCounter,-1);
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

	removeProcessFromScheduler(process);
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
	process->state = P_BLOCK;
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
	process->state = P_WAIT;
	unlock(&mutex);
	return TRUE;
}

void printProcesses(){
	lock(&mutex);
	ProcessSlot* aux = current;

	if(aux->process == NULL){
		print("NULL",-1);
		unlock(&mutex);
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
/*
void yield() {
	lock(&mutex);
	//	make call to timerTickHandler (ASM) 
	unlock(&mutex);
}
*/