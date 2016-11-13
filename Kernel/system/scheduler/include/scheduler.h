#ifndef _INC_SCHEDULER_

	#define _INC_SCHEDULER_

	#include "process.h"

	bool buildScheduler();
	bool addProcess(process *pid);
	bool removeProcess(process *pid);
	uint64_t contextSwitch(uint64_t stack);
	process *getCurrentProcess();

#endif