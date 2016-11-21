#include <stdio.h>
#include <syscall.h>
#include "../extra/include/commands.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);
static int processC(int argc, char **argv);

static semaphore s;

int semaphoreTestCommand(int argc, char **argv) {	
	if(argc != 0) {
		helpCommand(1, (char **) "semTest");
		return 1;
	}
	uint64_t pA;
	uint64_t pB;
	uint64_t pC;

	s = semBuild(0);

	if(s == NULL) {
		print("Couldn't create the semaphore.\n");
		return 1;
	}

	print("Creating \"Semaphore process A\"\n");
	pA = newProcess("Semaphore process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process A\"\n");
		semDestroy(s);
		return 1;
	}

	print("Semaphore process A id: ");
	printNum(pA);
	printNewLine();


	print("Creating \"Semaphore process B\"\n");
	pB = newProcess("Semaphore process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process B\"\n");
		kill(pA);
		semDestroy(s);
		return 1;
	}

	print("Semaphore process B id: ");
	printNum(pB);
	printNewLine();


	print("Creating \"Semaphore process C\"\n");
	pC = newProcess("Semaphore process C", processC, 0, NULL);
	if(pC == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process C\"\n");
		kill(pA);
		kill(pB);
		semDestroy(s);
		return 1;
	}

	print("Semaphore process C id: ");
	printNum(pC);
	printNewLine();

	wPid(pA);
	wPid(pB);
	wPid(pC);

	semDestroy(s);
	
	return 0;
}

static int processA(int argc, char **argv) {
	print("Semaphore process A OK\n");
	sleep(5);
	semPost(s);
	sleep(3);
	semPost(s);
	sleep(10);
	semPost(s);
	
	return 0;
}

static int processB(int argc, char **argv) {
	print("Semaphore process B OK\n");

	semWait(s);
	print("[B] Hey the waiting is over!\n");
	semWait(s);
	print("[B] The waiting is over again!\n");
	
	return 0;
}

static int processC(int argc, char **argv) {
	print("Semaphore process C OK\n");

	semWait(s);
	print("[C] The waiting is over!\n");
	
	return 0;
}