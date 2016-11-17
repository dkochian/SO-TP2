#include "../include/clock.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

static int processA(int argc, char **argv);

void startWaitpidTest() {
	uint64_t pA;

	print("Creating \"Waitpid process A\"", -1);
	printNewline();
	pA = newProcess("Waitpid process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Waitpid process A\"", -1);
		printNewline();
		return;
	}

	print("I'll wait for \"Waitpid process A\" (id: ", -1);
	printDec(pA, -1);
	print(")", -1);
	printNewline();
	waitPid(pA);
	print("Hey I can run again!", -1);
	printNewline();
}

static int processA(int argc, char **argv) {
	int counter = 0;
	print("Waitpid process A OK", -1);
	printNewline();
	
	while(counter < 100) {
		if(counter % 10 == 0) {
			print("processE: ", -1);
			printDec(counter, -1);
			printNewline();
			sleep(1);
		}

		counter++;
	}

	return 0;
}