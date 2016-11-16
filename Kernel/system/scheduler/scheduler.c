#include "../include/atom.h"
#include "include/process.h"
#include "include/scheduler.h"

#include "../../drivers/include/video.h"
#include "../include/clock.h"

extern void _timerTickHandler();

static process *schedule();
static void killProcess(process *p);

static list waiting_list;
static process *current_process;

bool buildScheduler() {
	waiting_list = buildList(&equal);

	if(waiting_list == NULL)
		return false;

	newProcess("PuppetMaster", NULL, 0, NULL);
	return true;
}

bool addProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	_cli();
	if(isEmpty(waiting_list) == true)
		current_process = p;

	res = add(waiting_list, p);
	_sti();

	return res;
}

bool removeProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	_cli();
	res = remove(waiting_list, p);
	killProcess(p);
	_sti();

	return res;
}

void blockProcess(uint64_t pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	p->state = BLOCKED;
}

void unBlockProcess(uint64_t pid) {
	print("LOL", -1);
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	p->state = WAITING;

	//_cli();
	setNext(waiting_list, p);
	//_sti();

	//_timerTickHandler();
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
/*
	print("-||", -1);
	print(current_process->name, -1);
	print(" -> ", -1);
*/	
	current_process = schedule();
/*	
	print(current_process->name, -1);
	print("||-", -1);
	printNewline();
*/	
	if(current_process == NULL)
		return 0;

	current_process->state = RUNNING;

	return current_process->rsp;
}

static void killProcess(process *p) {			//We should have to run sheduler again if the killed process is the one that's running right now
	process *child;

	if(p == NULL)
		return;
	if(p->id == 0) //You can't kill PuppetMaster
		return;

	child = getFirstWaitProcess(p);
	while(child != NULL) {
		killProcess(child);
		child = getFirstWaitProcess(p);
	}

	freeProcess(p->id);
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