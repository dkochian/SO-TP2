//BASED ON Wyrm/Process
#include "process.h"
#include "scheduler.h"

static uint64_t currentPid = 50;

extern void _yield(void);
static void initProcess(process_entry_t process_entry, uint64_t argc, void *argv);

static void * toStackAddress(void * page) {
	return (uint8_t *) page + PAGE_SIZE - 0x10;
}

static void * fillStackFrame(void * userStack, process_entry_t process_entry, uint64_t argc, char* argv[] ) {

	StackFrame * frame = (StackFrame *) userStack - 1;

	frame->gs = 0x001;
	frame->fs = 0x002;
	frame->r15 = 0x003;
	frame->r14 = 0x004;
	frame->r13 = 0x005;
	frame->r12 = 0x006;
	frame->r11 = 0x007;
	frame->r10 = 0x008;
	frame->r9 = 0x009;
	frame->r8 = 0x00A;
	frame->rsi = argc;
	frame->rdi = (uint64_t) process_entry;
	frame->rbp = 0x00D;
	frame->rdx = (uint64_t) argv;
	frame->rcx = 0x00F;
	frame->rbx = 0x010;
	frame->rax = 0x011;
	
	frame->rip = (uint64_t) &initProcess;//entryPoint;
	frame->cs = 0x008;
	frame->eflags = 0x202;
	frame->rsp = (uint64_t) &(frame->base);
	frame->ss = 0x000;
	frame->base = 0x000;

	return frame;
}

static void initProcess(process_entry_t process_entry, uint64_t argc, void *argv) {
	process_entry(argc, argv);
	removeProcessFromScheduler(getCurrProcess());
	_yield();
	return;
}

static void strcpy(char* to, char* from) {
	while( *from != '\0' ) {
		*to = *from;
		to++;
		from++;
	}
	*to = '\0';
}

Process* newProcess(void* entryPoint, char* name, bool foreground){//, uint64_t argc, void *argv) {
	Process* process = k_malloc(sizeof(Process));
	strcpy(process->name, name);
	process->entryPoint = entryPoint;
	process->userStackPage = newStackFrame();
	process->kernelStackPage = newStackFrame();
	process->userStack = toStackAddress(process->userStackPage);
	process->kernelStack = toStackAddress(process->kernelStackPage);
	process->pid = currentPid++;
	process->state = P_WAIT;

	process->userStack = fillStackFrame(process->userStack, process->entryPoint, 0, NULL);

	addProcessToScheduler(process, foreground);

	return process;
}

void freeProcess(Process* process) {
	removeStackFrame(process->userStack);
	removeStackFrame(process->kernelStack);
	k_free(process);
}
