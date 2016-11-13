#include "../include/atom.h"
#include "include/process.h"
#include "include/scheduler.h"

#include "../../drivers/include/video.h"
#include "../include/clock.h"

static process *schedule();
static void killProcess(process *p);

static list process_list;
static process* current_process;

static int dummyP(int argc, char **argv);

bool buildScheduler() {
	process_list = buildList(&equal);

	if(process_list == NULL)
		return false;

	newProcess("master of the puppets (ini)", NULL, 0, NULL);
	print("current: ", -1);
	print(current_process->name, -1);
	print(" || rsp: 0x", -1);
	printHex(current_process->rsp, -1);
	printNewline();
	return true;
}

bool addProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	_cli();
	if(isEmpty(process_list) == true)
		current_process = p;

	res = add(process_list, p);
	_sti();

	return res;
}

bool removeProcess(process *p) {
	bool res = false;

	if(p == NULL)
		return res;

	_cli();
	res = remove(process_list, p);
	killProcess(p);
	_sti();

	return res;
}

process *getCurrentProcess() {
	return current_process;
}

uint64_t contextSwitch(uint64_t stack) {
	if(current_process == NULL)
		return 0;

	current_process->rsp = stack;
	current_process->state = WAITING;

	current_process = schedule();
	if(current_process == NULL) {
		return 0;
	}

	current_process->state = RUNNING;

	return current_process->rsp;
}

static void killProcess(process *p) {			//We should have to run sheduler again if the killed process is the one that's running right now
	process *child;

	if(p == NULL)
		return;
	if(p->id == 0) //You can't kill dummy
		return;

	child = getFirstWaitProcess(p);
	while(child != NULL) {
		killProcess(child);
		child = getFirstWaitProcess(p);
	}

	freeProcess(p->id);
}

static process *schedule() {
	if(process_list == NULL)
		return NULL;

	if(isEmpty(process_list) == true)
		return NULL;

	process *p;

	do {
		p = peekFirst(process_list);
		if(p == NULL)
			break;
	} while(p->state == BLOCKED || p->id == 0);

	return p;//if it's NULL, it'll return NULL
}