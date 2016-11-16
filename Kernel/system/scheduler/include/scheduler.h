#ifndef _INC_SCHEDULER_

	#define _INC_SCHEDULER_

	#include "process.h"

	bool buildScheduler();
	process *getCurrentProcess();
	bool addProcess(process *pid);
	void blockProcess(uint64_t pid);
	bool removeProcess(process *pid);
	void unBlockProcess(uint64_t pid);
	uint64_t contextSwitch(uint64_t stack);
	void setForeground(uint64_t pid);
	process *getForeground();
	uint64_t getCurrentPid();
	uint64_t getNumerProcess();
	psContext * processesStatus();

#endif