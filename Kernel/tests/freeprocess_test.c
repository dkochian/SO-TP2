#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

#include "../utils/include/clock.h"

static int processA(int argc, char **argv);

void startFreeProcessTest() {
	uint64_t pA;

	print("Creating \"FreeProcess process A\"\n", -1);
	pA = newProcess("FreeProcess process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"FreeProcess process A\"\n", -1);
		return;
	}

	print("I'll wait for \"FreeProcess process A\" (id: ", -1);
	printDec(pA, -1);
	print(")\n", -1);
	waitPid(pA);
	print("Hey I can run again!\n", -1);

	print("Creating \"FreeProcess process A\" again\n", -1);
	pA = newProcess("FreeProcess process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"FreeProcess process A\"\n", -1);
		return;
	}

	print("I'll again wait for \"FreeProcess process A\" (id: ", -1);
	printDec(pA, -1);
	print(")\n", -1);
}

static int processA(int argc, char **argv) {
	print("FreeProcess process A running\n", -1);
	sleep(3);

	return 0;
}