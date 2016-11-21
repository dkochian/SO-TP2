#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);

int startWaitpidTest(int argc, char **argv) {
	uint64_t pA;

	print("Creating \"Waitpid process A\"\n", -1);
	pA = newProcess("Waitpid process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Waitpid process A\"\n", -1);
		return 1;
	}

	print("I'll wait for \"Waitpid process A\" (id: ", -1);
	printNum(pA, -1);
	print(")", -1);
	printNewLine();
	waitPid(pA);
	print("Hey I can run again!\n", -1);

	return 0;
}

static int processA(int argc, char **argv) {
	int counter = 0;
	print("Waitpid process A OK\n", -1);
	
	while(counter < 100) {
		if(counter % 10 == 0) {
			print("processE: ", -1);
			printNum(counter, -1);
			printNewLine();
			sleep(1);
		}

		counter++;
	}

	return 0;
}