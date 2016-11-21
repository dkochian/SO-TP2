#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

int startSchedulerTest(int argc, char **argv) {
	uint64_t pA;
	uint64_t pB;
	
	print("Creating \"Scheduler process A\"\n", -1);
	pA = newProcess("Scheduler process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process A\"\n", -1);
		return 1;
	}
	
	print("Scheduler process A id: ", -1);
	printNum(pA, -1);
	printNewLine();

	print("Creating \"Scheduler process B\"\n", -1);
	pB = newProcess("Scheduler process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process B\"\n", -1);
		freeProcess(pA);
		return 1;
	}
	
	print("Scheduler process B id: ", -1);
	printNum(pB, -1);
	printNewLine();

	return 0;
}

static int processA(int argc, char **argv) {
	static int counter = CICLES;

	print("Scheduler process A is running\n", -1);

	while(counter > 0) {
		print("Scheduler process A: row ", -1);
		printNum(counter++, -1);
		printNewLine();

		sleep(1);
		counter--;
	}

	return 0;
}

static int processB(int argc, char **argv) {
	static int counter = CICLES;

	print("Scheduler process B is running\n", -1);

	while(cicles > 0) {
		print("Scheduler process B: row ", -1);
		printNum(counter++, -1);
		printNewLine();

		sleep(1);
		counter--;
	}

	return 0;
}