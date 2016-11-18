#include "../include/atom.h"
#include "include/process.h"
#include "include/scheduler.h"

#include "../../drivers/include/video.h"
#include "../include/clock.h"
#include "../include/mutex.h"
#include "../include/string.h"
#include "../include/mmu.h"

extern void _timerTickHandler();

static process *schedule();
static void killProcess(process *p);

static list waiting_list;
static process *current_process;

static mutex *s_mutex;

int puppetMaster(int argc, char** argv) {
	while(true) {}
	return 0;
}

bool buildScheduler() {
	waiting_list = buildList(&equal);

	if(waiting_list == NULL)
		return false;

	newProcess("Puppet Master", puppetMaster, 0, NULL);

	s_mutex = initLock();
	if (s_mutex == NULL) 
		return false;
	
	return true;
}

bool addProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	lock(s_mutex);
	if(isEmpty(waiting_list) == true)
		current_process = p;

	res = add(waiting_list, p);
	unlock(s_mutex);

	return res;
}

bool removeProcess(process *p) { //should have locks but can't maybe caller (freeProcess also can't be locked)
	bool res = false;

	if(p == NULL)
		return res;

	if(p->id == 0){//You can't kill dummy
		print("Master of the Puppets is indestructible....", RED);
		printNewline();
		return false;
	} 
		
	res = remove(waiting_list, p);
	/*if(!k_strcmp(p->name, "Shell")) {
		clear();
		print("See you on the other side....", RED);
	}*/
	killProcess(p);

	return res;
}

void blockProcess(uint64_t pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	p->state = BLOCKED;
}

void unBlockProcess(uint64_t pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	p->state = WAITING;

	lock(s_mutex);
	setNext(waiting_list, p);
	//printList(waiting_list);
	unlock(s_mutex);
}

void setForeground(uint64_t pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	process *current;

	do {
		current = get(waiting_list);
		current->foreground = false;
	} while(current != NULL);

	resetCursor(waiting_list);

	p->foreground = true;	
}

process *getForeground() {
	process *current;

	do {
		current = get(waiting_list);
	} while(current != NULL && current->foreground == false);

	resetCursor(waiting_list);

	return current;
}

process *getCurrentProcess() {
	return current_process;
}

uint64_t contextSwitch(uint64_t stack) {
	if(current_process == NULL)
		return 0;

	current_process->rsp = stack;
	if(current_process->state != BLOCKED)
		current_process->state = WAITING;

	/*print("-|", -1);
	print(current_process->name, -1);
	print(" -> ", -1);*/
	current_process = schedule();

	/*print(current_process->name, -1);
	print("|-", -1);*/
	//printNewline();
	if(current_process == NULL)
		return 0;

	current_process->state = RUNNING;

	return current_process->rsp;
}

uint64_t getCurrentPid(){
	return current_process->id;
}

static void killProcess(process *p) {			//We should have to run sheduler again if the killed process is the one that's running right now
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
	if(waiting_list == NULL)
		return NULL;

	if(isEmpty(waiting_list) == true)
		return NULL;

	process *p;

	do {
		p = peekFirst(waiting_list);
		if(p == NULL)
			break;
	} while(p->state == BLOCKED);

	return p;
}

psContext *processesStatus() {

	char buffer[10] = {0};
	int aux = getSize(waiting_list);

	int i;

	psContext * res = k_malloc(sizeof(psContext));
	res->numbProcess = aux;
	res->processes = k_malloc(sizeof(char)* aux);
	res->separateChar = '&';
/*
	res->processes[0] = k_malloc(sizeof(char) * 60);
	k_itoa(current_process->id, buffer);
	k_strcat(res->processes[0], buffer);
	k_strcat(res->processes[0], "&");
	k_strcat(res->processes[0], current_process->name);
	k_strcat(res->processes[0], "&");
	k_itoa(current_process->state, buffer);
	k_strcat(res->processes[0], buffer);
	k_strcat(res->processes[0], "&");
	k_itoa(current_process->foreground, buffer);
	k_strcat(res->processes[0], buffer);
*/
	list wProcessList = waiting_list;
	resetCursor(wProcessList);

	process * p = get(wProcessList);

	for(i = 0; i < aux ; i++ ){

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
		p= get(wProcessList);
	}
	return res;
}
