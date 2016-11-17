#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);

void startFreeProcessTest() {
	uint64_t pA;

	print("Creating \"FreeProcess process A\"", -1);
	printNewline();
	pA = newProcess("FreeProcess process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"FreeProcess process A\"", -1);
		printNewline();
		return;
	}

	print("I'll wait for \"FreeProcess process A\" (id: ", -1);
	printDec(pA, -1);
	print(")", -1);
	printNewline();
	waitPid(pA);
	print("Hey I can run again!", -1);

	print("Creating \"FreeProcess process A\" again", -1);
	printNewline();
	pA = newProcess("FreeProcess process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"FreeProcess process A\"", -1);
		printNewline();
		return;
	}

	print("I'll again wait for \"FreeProcess process A\" (id: ", -1);
	printDec(pA, -1);
	print(")", -1);
	printNewline();
	waitPid(pA);
	print("Hey I can run again!", -1);
}

static int processA(int argc, char **argv) {
	print("FreeProcess process A OK", -1);
	printNewline();

	return 0;
}