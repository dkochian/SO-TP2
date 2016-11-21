#include <stdio.h>
#include <syscall.h>

static int processA(int argc, char **argv);

int freeProcessTestCommand(int argc, char **argv) {
	uint64_t pA;

	print("Creating \"kill process A\"\n");
	pA = newProcess("kill process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"kill process A\"\n");
		return 1;
	}

	print("I'll wait for \"kill process A\" (id: ");
	printNum(pA);
	print(")\n");
	wPid(pA);
	print("Hey I can run again!\n");

	print("Creating \"kill process A\" again\n");
	pA = newProcess("kill process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"kill process A\"\n");
		return 1;
	}

	print("I'll again wait for \"kill process A\" (id: ");
	printNum(pA);
	print(")\n");

	return 0;
}

static int processA(int argc, char **argv) {
	print("kill process A running\n");
	sleep(3);

	return 0;
}