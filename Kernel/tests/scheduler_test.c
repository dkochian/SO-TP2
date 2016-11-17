#include "../include/clock.h"
#include "include/scheduler_test.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

void startSchedulerTest() {
	uint64_t pA;
	uint64_t pB;
	
	print("Creating process A", -1);
	printNewline();
	pA = newProcess("Process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create process A", -1);
		printNewline();
		return;
	}
	
	print("Process A id: ", -1);
	printDec(pA, -1);
	printNewline();

	print("Creating process B", -1);
	printNewline();
	pB = newProcess("Process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create process B", -1);
		printNewline();
		freeProcess(pA);
		return;
	}
	
	print("Process B id: ", -1);
	printDec(pB, -1);
	printNewline();
}

int processA(int argc, char **argv) {
	static int counter = 0;

	print("Process A is running", -1);
	printNewline();

	while(true) {
		print("Process A: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}

int processB(int argc, char **argv) {
	static int counter = 0;

	print("Process B is running", -1);
	printNewline();

	while(true) {
		print("Process B: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}