#include <stdio.h>
#include <syscall.h>
#include "../include/common.h"
#include "../extra/include/commands.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

int schedulerTestCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "scTest");
		return 1;
	}
	uint64_t pA;
	uint64_t pB;
	
	print("Creating \"Scheduler process A\"\n");
	pA = newProcess("Scheduler process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process A\"\n");
		return 1;
	}
	
	print("Scheduler process A id: ");
	printNum(pA);
	printNewLine();

	print("Creating \"Scheduler process B\"\n");
	pB = newProcess("Scheduler process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Scheduler process B\"\n");
		kill(pA);
		return 1;
	}
	
	print("Scheduler process B id: ");
	printNum(pB);
	printNewLine();

	wPid(pA);
	wPid(pB);

	return 0;
}

static int processA(int argc, char **argv) {
	static int counter = CICLES;

	print("Scheduler process A is running\n");

	while(counter > 0) {
		print("Scheduler process A: row ");
		printNum(counter);
		printNewLine();

		sleep(1);
		counter--;
	}

	return 0;
}

static int processB(int argc, char **argv) {
	static int counter = CICLES;

	print("Scheduler process B is running\n");

	while(counter > 0) {
		print("Scheduler process B: row ");
		printNum(counter);
		printNewLine();

		sleep(1);
		counter--;
	}

	return 0;
}