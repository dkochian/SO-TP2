#ifndef _INC_SCHEDULER_
	#define _INC_SCHEDULER_

	#include "process.h"

	bool buildScheduler();
	process *getCurrentProcess();
	bool addProcess(process *p);
	void blockProcess(process *p);
	bool removeProcess(process *p);
	void unBlockProcess(process *p);
	uint64_t contextSwitch(uint64_t stack);
	void setForeground(uint64_t pid);
	process *getForeground();
	uint64_t getCurrentPid();
	uint64_t getNumerProcess();
	psContext * processesStatus();

#endif