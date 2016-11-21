#include "include/process.h"
#include "include/scheduler.h"
#include "../../include/atom.h"

#include "../../drivers/include/video.h"
#include "../../utils/include/clock.h"
#include "../ipc/include/mutex.h"
#include "../../utils/include/string.h"
#include "../include/mmu.h"

extern void _timerTickHandler();

static process *schedule();
static void killProcess(process *p);

static list waiting_list;
static process *current_process;

static mutex s_mutex;

bool buildScheduler() {
	waiting_list = listBuild(&equal);

	if(waiting_list == NULL)
		return false;

	s_mutex = lockBuild();
	if(s_mutex == NULL) 
		return false;

	newProcess("Master of the Puppets", NULL, 0, NULL);
	
	return true;
}

bool addProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	lock(s_mutex);
	if(p->id == 0)
		current_process = p;

	res = listAdd(waiting_list, p);
	unlock(s_mutex);

	return res;
}

bool removeProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;
			
	res = listRemove(waiting_list, p);

	killProcess(p);

	return res;
}

void blockProcess(process *p) {
	if(p == NULL)
		return;

	p->state = BLOCKED;
}

void unBlockProcess(process * p) {
	if(p == NULL)
		return;

	p->state = WAITING;
}

void setForeground(uint64_t pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	process *current;

	do {
		current = listGet(waiting_list);
		current->foreground = false;
	} while(current != NULL);

	p->foreground = true;	
}

process *getForeground() {
	process *current;

	do {
		current = listGet(waiting_list);
		if(current->foreground == true)
			break;
	} while(current != NULL);

	listResetCursor(waiting_list);

	return current;
}

process *getCurrentProcess() {
	return current_process;
}

uint64_t contextSwitch(uint64_t stack) {
	if(current_process == NULL )
		return 0;

	current_process->rsp = stack;
	if(current_process->state != BLOCKED)
		current_process->state = WAITING;

	current_process = schedule();

	if(current_process == NULL)
		return 0;

	current_process->state = RUNNING;

	return current_process->rsp;
}

uint64_t getCurrentPid(){
	return current_process->id;
}

static void killProcess(process *p) {
	process *child;

	if(p == NULL)
		return;

	child = getFirstWaitProcess(p);
	while(child != NULL) {
		freeProcess(child->id);
		child = getFirstWaitProcess(p);
	}
}

static process *schedule() {
	process *p;

	if(waiting_list == NULL || listIsEmpty(waiting_list) == true)
		return NULL;

	do {
		p = listPeekFirst(waiting_list);
	} while(p->state == BLOCKED);

	return p;
}

psContext *processesStatus() {
	char buffer[10] = {0};
	int listSize = listGetSize(waiting_list);

	int i, aux;

	psContext * res = k_malloc(sizeof(psContext));
	res->numbProcess = listSize;
	res->processes = k_malloc(sizeof(char)* listSize);
	res->separateChar = '&';

	list wProcessList = waiting_list;
	listResetCursor(wProcessList);

	process * p = listGet(wProcessList);

	for(i = 0; i < listSize ; i++ ){

		res->processes[i] = k_malloc(sizeof(char ) * 100);
		k_itoa(p->id, buffer);
		k_strcat(res->processes[i],  buffer);
		k_strcat(res->processes[i], "&");
		k_strcat(res->processes[i], p->name);
		k_strcat(res->processes[i], "&");
		k_itoa(p->state, buffer);
		k_strcat(res->processes[i], buffer);
		k_strcat(res->processes[i], "&");
		k_itoa(p->foreground, buffer);
		k_strcat(res->processes[i], buffer);
		k_strcat(res->processes[i], "&");
		aux = p->mallocsUsed * 4;
		k_itoa(aux, buffer);
		k_strcat(res->processes[i], buffer);
		p = listGet(wProcessList);
	}
	return res;
}