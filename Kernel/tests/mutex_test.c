#include "../include/clock.h"
#include "../include/mutex.h"
#include "include/mutex_test.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int var;
static mutex *m;

void startLockTest() {
	uint64_t pC;
	uint64_t pD;

	m = initLock();

	if(m == NULL) {
		print("Couldn't create the lock.", -1);
		printNewline();
		return;
	}

	print("Creating process C", -1);
	printNewline();
	pC = newProcess("Process C", processC, 0, NULL);
	if(pC == INVALID_PROCESS_ID) {
		print("Couldn't create process C", -1);
		printNewline();
	}
	else {
		print("Process C id: ", -1);
		printDec(pC, -1);
		printNewline();
	}

	print("Creating process D", -1);
	printNewline();
	pD = newProcess("Process D", processD, 0, NULL);
	if(pD == INVALID_PROCESS_ID) {
		print("Couldn't create process D", -1);
		printNewline();
	}
	else {
		print("Process D id: ", -1);
		printDec(pD, -1);
		printNewline();
	}
}

int processC(int argc, char **argv) {
	while(true) {
		lock(m);
		print("lock 1 passed", -1);
		printNewline();
		var = 1;
		print("processC: ", -1);
		printDec(var, -1);
		printNewline();
		unlock(m);
	}

	return 0;
}

int processD(int argc, char **argv) {
	while(true) {
		lock(m);
		var = 2;
		print("processD: ", -1);
		printDec(var, -1);
		printNewline();
		unlock(m);
	}

	return 0;
}