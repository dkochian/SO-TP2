#include "../include/mmu.h"
#include "../include/atom.h"
#include "include/process.h"
#include "../include/mutex.h"
#include "include/scheduler.h"

#include "../../drivers/include/video.h"
#include "../../include/lib.h"

extern void _timerTickHandler();

static void freeProcessId(uint64_t id);
static uint64_t getNewProcessId(process *p);
static int startProcess(func f, int argc, char **argv);
static void *buildStacKFrame(void *rsp, func f, int argc, char **argv);

static process **process_table;

bool buildProcessManager() {
	process_table = (process **) k_malloc(sizeof(process*)*MAX_PROCESSES);

	if(process_table == NULL)
		return false;

	for(int i=0; i<MAX_PROCESSES; i++)
		process_table[i] = NULL;

	return true;
}

uint64_t newProcess(char* name, func f, int argc, char **argv) {
	process *p;
	uint64_t rsp;

	p = (process *) k_malloc(sizeof(process));
	if(p == NULL)
		return INVALID_PROCESS_ID;

	p->id = getNewProcessId(p);
	if(p->id == INVALID_PROCESS_ID) {
		k_free(p);

		return INVALID_PROCESS_ID;
	}

	rsp = (uint64_t) k_malloc(STACK_SIZE);
	if(rsp == NULL) {
		freeProcessId(p->id);
		k_free(p);

		return INVALID_PROCESS_ID;
	}

	p->wait_list = buildList(&equal);
	if(p->wait_list == NULL) {
		freeProcessId(p->id);
		k_free((void *) rsp);
		k_free(p);

		return INVALID_PROCESS_ID;
	}

	p->name = name;
	p->s_frame = rsp;
	p->state = WAITING;
	p->foreground = false;

	if(p->id == 1)//it's hardcoded for testing only
		setForeground(p->id);

	rsp += STACK_SIZE - 1 - sizeof(stack_frame);
	p->rsp = (uint64_t) buildStacKFrame((void *)rsp, f, argc, argv);

	process *father = getCurrentProcess();
	if(father == NULL)
		father = 0;

	p->father = father->id;

	addProcess(p);	//Add the process to the scheduler

	return p->id;
}

void freeProcess(int pid) {
	process *p = getProcessFromId(pid);

	if(p == NULL)
		return;

	removeProcess(p);	//Remove the process from the scheduler
	freeProcessId(pid);	//Free the process id

	destroyList(p->wait_list);
	k_free((void *) p->s_frame);
	k_free(p);
}

void addWaitProcess(process *child) {
	if(child == NULL)
		return;

	process *father = getProcessFromId(child->father);

	if(father == NULL)
		return;

	add(father->wait_list, child);

	if(father->state != BLOCKED)
		father->state = BLOCKED;
}

void removeWaitProcess(process *child) {
	if(child == NULL)
		return;

	process *father = getProcessFromId(child->father);

	if(father == NULL)
		return;

	remove(father->wait_list, child);

	if(isEmpty(father->wait_list) == true)
		father->state = WAITING;
}

process *getFirstWaitProcess(process *father) {
	if(father == NULL)
		return NULL;

	return getFirst(father->wait_list);
}

bool equal(process *p1, process *p2) {
	return p1->id == p2->id;
}

process *getProcessFromId(uint64_t id) {
	if(id == INVALID_PROCESS_ID || id < 0 || id > MAX_PROCESSES - 1)
		return NULL;

	process *p = NULL;

	_cli();
	p = process_table[id];
	_sti();

	return p;
}

static int startProcess(func f, int argc, char **argv) {
	if(f != NULL)
		f(argc, argv);

	removeProcess(getCurrentProcess());
	_timerTickHandler();
	return 0;
}

static void *buildStacKFrame(void *rsp, func f, int argc, char **argv) {
	stack_frame *s_frame = (stack_frame *) rsp;

	s_frame->gs = 0x001;
	s_frame->fs = 0x002;
	s_frame->r15 = 0x003;
	s_frame->r14 = 0x004;
	s_frame->r13 = 0x005;
	s_frame->r12 = 0x006;
	s_frame->r11 = 0x007;
	s_frame->r10 = 0x008;
	s_frame->r9 = 0x009;
	s_frame->r8 = 0x00A;
	s_frame->rsi = argc;
	s_frame->rdi = (uint64_t) f;
	s_frame->rbp = 0x00D;
	s_frame->rdx = (uint64_t) argv;
	s_frame->rcx = 0x00F;
	s_frame->rbx = 0x010;
	s_frame->rax = 0x011;
	
	s_frame->rip = (uint64_t) &startProcess;
	s_frame->cs = 0x008;
	s_frame->eflags = 0x202;
	s_frame->rsp = (uint64_t) &(s_frame->base);
	s_frame->ss = 0x000;
	s_frame->base = 0x000;

	return s_frame;
}

static uint64_t getNewProcessId(process *p) {
	if(p == NULL)
		return INVALID_PROCESS_ID;

	uint64_t id = INVALID_PROCESS_ID;

	_cli();
	for(int i=0; i<MAX_PROCESSES; i++) {
		if(process_table[i] == NULL) {
			id = i;
			process_table[i] = p;
			break;
		}
	}
	_sti();

	return id;
}

static void freeProcessId(uint64_t id) {
	if(id == INVALID_PROCESS_ID || id < 0 || id > MAX_PROCESSES - 1)
		return;

	_cli();
	process_table[id] = NULL;
	_sti();
}