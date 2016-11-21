#include <stdio.h>
#include <syscall.h>
#include "../include/common.h"

static int processA(int argc, char **argv);

int waitpidTestCommand(int argc, char **argv) {
	uint64_t pA;

	print("Creating \"Waitpid process A\"\n");
	pA = newProcess("Waitpid process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Waitpid process A\"\n");
		return 1;
	}

	print("I'll wait for \"Waitpid process A\" (id: ");
	printNum(pA);
	print(")");
	printNewLine();
	wPid(pA);
	print("Hey I can run again!\n");

	return 0;
}

static int processA(int argc, char **argv) {
	int counter = CICLES;
	print("Waitpid process A OK\n");
	
	while(counter > 0) {
		if(counter % 10 == 0) {
			print("processE: ");
			printNum(counter);
			printNewLine();
			sleep(1);
		}

		counter--;
	}

	return 0;
}