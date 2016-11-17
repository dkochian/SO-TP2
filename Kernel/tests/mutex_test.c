#include "../include/mutex.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

static int var;
static mutex *m;

void startLockTest() {
	uint64_t pA;
	uint64_t pB;

	m = initLock();

	if(m == NULL) {
		print("Couldn't create the lock.", -1);
		printNewline();
		return;
	}

	print("Creating \"Mutex process A\"", -1);
	printNewline();
	pA = newProcess("Mutex process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process A\"", -1);
		printNewline();
		return;
	}

	print("Mutex process A id: ", -1);
	printDec(pA, -1);
	printNewline();

	print("Creating \"Mutex process B\"", -1);
	printNewline();
	pB = newProcess("Mutex process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process B\"", -1);
		printNewline();
		freeProcess(pA);
		return;
	}

	print("Mutex process B id: ", -1);
	printDec(pB, -1);
	printNewline();
}

static int processA(int argc, char **argv) {
	int counter = 0;
	print("Mutex process A OK", -1);
	printNewline();
	
	while(true) {
		lock(m);
		var = 1;
		print("Mutex processA: ", -1);
		printDec(var, -1);
		printNewline();
		unlock(m);
		if(counter == 3){
			counter = 0;
			clear();
		}
		else
			counter++;
	}

	return 0;
}

static int processB(int argc, char **argv) {
	int counter = 0;
	print("Mutex process B OK", -1);
	printNewline();

	while(true) {
		lock(m);
		var = 2;
		print("Mutex processB: ", -1);
		printDec(var, -1);
		printNewline();
		unlock(m);
		if(counter == 2) {
			counter = 0;
			clear();
		}
		else
			counter++;
	}

	return 0;
}