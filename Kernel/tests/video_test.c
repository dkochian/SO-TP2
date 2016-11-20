#include "include/video_test.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

void startVideoTest() {
	uint64_t pA;
	uint64_t pB;

	print("Creating \"Scheduler process A\"\n", -1);
	pA = newProcess("Scheduler process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process A\"\n", -1);
		return;
	}
	
	print("Scheduler process A id: ", -1);
	printDec(pA, -1);
	printNewLine();

	print("Creating \"Scheduler process B\"\n", -1);
	pB = newProcess("Scheduler process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process B\"\n", -1);
		freeProcess(pA);
		return;
	}
	
	print("Scheduler process B id: ", -1);
	printDec(pB, -1);
	printNewLine();
}

static int processA(int argc, char **argv) {
	static int counter = 0;

	print("Scheduler process A is running\n", -1);

	while(true) {
		print("Scheduler process A: row ", -1);
		printDec(counter++, -1);
		printNewLine();
	}

	return 0;
}

static int processB(int argc, char **argv) {
	static int counter = 0;

	print("Scheduler process B is running\n", -1);

	while(true) {
		print("Scheduler process B: row ", -1);
		printDec(counter++, -1);
		printNewLine();
	}

	return 0;
}