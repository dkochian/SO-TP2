#include "../include/clock.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

void startSchedulerTest() {
	uint64_t pA;
	uint64_t pB;
	
	print("Creating \"Scheduler process A\"", -1);
	printNewline();
	pA = newProcess("Scheduler process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process A\"", -1);
		printNewline();
		return;
	}
	
	print("Scheduler process A id: ", -1);
	printDec(pA, -1);
	printNewline();

	print("Creating \"Scheduler process B\"", -1);
	printNewline();
	pB = newProcess("Scheduler process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process B\"", -1);
		printNewline();
		freeProcess(pA);
		return;
	}
	
	print("Scheduler process B id: ", -1);
	printDec(pB, -1);
	printNewline();
}

static int processA(int argc, char **argv) {
	static int counter = 0;

	print("Scheduler process A is running", -1);
	printNewline();

	while(true) {
		print("Scheduler process A: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}

static int processB(int argc, char **argv) {
	static int counter = 0;

	print("Scheduler process B is running", -1);
	printNewline();

	while(true) {
		print("Scheduler process B: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}