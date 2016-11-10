//BASED ON Wyrm/Process
#include "process.h"
#include "scheduler.h"

static uint64_t currentPid = 50;

void removeProcess2(Process * process) {
    removeStackFrame(process->userStack);
    removeStackFrame(process->kernelStack);
    k_free(process);
}

static void * toStackAddress(void * page) {
    return (uint8_t *) page + PAGE_SIZE - 0x10;
}

static void * fillStackFrame(void * entryPoint, void * userStack) {

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
    frame->rsi = 0x00B;
    frame->rdi = 0x00C;
    frame->rbp = 0x00D;
    frame->rdx = 0x00E;
    frame->rcx = 0x00F;
    frame->rbx = 0x010;
    frame->rax = 0x011;
    frame->rip = (uint64_t) entryPoint;
    frame->cs = 0x008;
    frame->eflags = 0x202;
    frame->rsp = (uint64_t) & (frame->base);
    frame->ss = 0x000;
    frame->base = 0x000;

    return frame;
}

Process * newProcess(void * entryPoint, char* name) {
    Process * process = k_malloc(sizeof(Process));

    process->name = name;
    process->entryPoint = entryPoint;
    process->userStackPage = newStackFrame();
    process->kernelStackPage = newStackFrame();
    process->userStack = toStackAddress(process->userStackPage);
    process->kernelStack = toStackAddress(process->kernelStackPage);
    process->pid = currentPid++;
    process->state = "WAITING";

    process->userStack = fillStackFrame(process->entryPoint, process->userStack);

    addProcess(process);

    return process;
}