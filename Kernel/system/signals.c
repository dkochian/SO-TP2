#include "include/signals.h"
#include "scheduler/include/scheduler.h"
#include "scheduler/include/process.h"
#include "../drivers/include/video.h"


void ctrlCHandler(){

	print("^C",-1);
	uint64_t pid = getCurrentPid();

	while(pid > 2) {
		pid = getProcessFromId(pid)->father;
	}
	processesStatus();
	printNewLine();
	printDec(pid, CYAN);
	printNewLine();
	freeProcess(pid);
	print("afuera\n",-1);
	processesStatus();
}