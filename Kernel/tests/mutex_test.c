#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "../system/ipc/include/mutex.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);
static int processC(int argc, char **argv);

static int var;
mutex m;
static bool locked;

void startLockTest(bool lock) {
	uint64_t pA;
	uint64_t pB;
	uint64_t pC;

	m = lockBuild();

	if(m == NULL) {
		print("Couldn't create the lock.\n", -1);
		return;
	}

	locked = lock;

	print("Creating \"Mutex process A\"\n", -1);
	pA = newProcess("Mutex process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process A\"\n", -1);
		lockDestroy(m);
		return;
	}

	print("Mutex process A id: ", -1);
	printDec(pA, -1);
	printNewLine();

	print("Creating \"Mutex process B\"\n", -1);
	pB = newProcess("Mutex process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process B\"\n", -1);
		freeProcess(pA);
		lockDestroy(m);
		return;
	}

	print("Mutex process B id: ", -1);
	printDec(pB, -1);
	printNewLine();

	print("Creating \"Mutex process C\"\n", -1);
	pC = newProcess("Mutex process C", processC, 0, NULL);
	if(pC == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process C\"\n", -1);
		freeProcess(pA);
		freeProcess(pB);
		lockDestroy(m);
		return;
	}

	print("Mutex process C id: ", -1);
	printDec(pC, -1);
	printNewLine();
}

static int processA(int argc, char **argv) {
	print("Mutex process A OK\n", -1);
	
	while(true) {
		lock(m);

		if(locked == true) {
			lock(m);
			print("You shouldn't see this message.\n", -1);
		}
		
		var = 2;
		print("Mutex processA: ", -1);
		printDec(var, -1);
		printNewLine();
		sleep(2);
		unlock(m);
	}

	return 0;
}

static int processB(int argc, char **argv) {
	print("Mutex process B OK\n", -1);

	while(true) {
		lock(m);
		var = 3;
		print("Mutex processB: ", -1);
		printDec(var, -1);
		printNewLine();
		unlock(m);
		sleep(3);
	}

	return 0;
}

static int processC(int argc, char **argv) {
	print("Mutex process C OK\n", -1);

	while(true) {
		lock(m);
		var = 5;
		print("Mutex processC: ", -1);
		printDec(var, -1);
		printNewLine();
		unlock(m);
		sleep(5);
	}

	return 0;
}