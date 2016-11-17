#include "../include/clock.h"
#include "../include/mutex.h"
#include "include/waitpid_test.h"
#include "../drivers/include/video.h"
#include "../system/scheduler/include/process.h"
#include "../system/scheduler/include/scheduler.h"

void startWaitpidTest() {
	uint64_t pE;

	print("Creating process E", -1);
	printNewline();
	pE = newProcess("Process E", processE, 0, NULL);
	if(pE == INVALID_PROCESS_ID) {
		print("Couldn't create process E", -1);
		printNewline();
		return;
	}

	print("I'll wait for process E (id: ", -1);
	printDec(pE, -1);
	print(")", -1);
	printNewline();
	waitPid(pE);
	print("Hey I can run again!", -1);
	printNewline();
}

int processE(int argc, char **argv) {
	int counter = 0;
	print("process E OK", -1);
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