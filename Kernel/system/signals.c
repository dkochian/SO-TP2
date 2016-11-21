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
	freeProcess(pid);
}