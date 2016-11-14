#ifndef _INC_PROCESS_

	#define _INC_PROCESS_

	#include "processList.h"

	#define MAX_PROCESSES	16

	#define STACK_SIZE		0x800000

	extern void _yield();

	typedef int (*func) (int, char **);

	typedef enum process_state {
		RUNNING,
		BLOCKED,
		WAITING
	} process_state;

	typedef struct stack_frame {
		//Registers restore context
		uint64_t gs;
		uint64_t fs;
		uint64_t r15;
		uint64_t r14;
		uint64_t r13;
		uint64_t r12;
		uint64_t r11;
		uint64_t r10;
		uint64_t r9;
		uint64_t r8;
		uint64_t rsi;
		uint64_t rdi;
		uint64_t rbp;
		uint64_t rdx;
		uint64_t rcx;
		uint64_t rbx;
		uint64_t rax;

		//iretq hook
		uint64_t rip;
		uint64_t cs;
		uint64_t eflags;
		uint64_t rsp;
		uint64_t ss;
		uint64_t base;
	} stack_frame;

	typedef struct process {
		char *name;
		uint64_t id;
		uint64_t father;
		uint64_t s_frame;
		uint64_t rsp;

		list wait_list;

		process_state state;
	} process;

	void freeProcess(int pid);
	bool buildProcessManager();
	void addWaitProcess(process *child);
	bool equal(process *p1, process *p2);
	void removeWaitProcess(process *child);
	process *getFirstWaitProcess(process *father);
	uint64_t newProcess(char* name, func f, int argc, char **argv);
	

#endif