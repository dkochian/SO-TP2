#include <stdio.h>
#include <integer.h>
#include <syscall.h>
#include "../include/common.h"
#include "../extra/include/commands.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);
static int processC(int argc, char **argv);

static int var;
static mutex m;
static bool locked;

int mutexTestCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "mTest");
		return 1;
	}
	uint64_t pA;
	uint64_t pB;
	uint64_t pC;

	m = mutexInit();

	if(m == NULL) {
		print("Couldn't create the lock.\n");
		return 1;
	}

	locked = strint((char *) argv);

	print("Creating \"Mutex process A\"\n");
	pA = newProcess("Mutex process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process A\"\n");
		mutexDestroy(m);
		return 1;
	}

	print("Mutex process A id: ");
	printNum(pA);
	printNewLine();

	print("Creating \"Mutex process B\"\n");
	pB = newProcess("Mutex process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process B\"\n");
		kill(pA);
		mutexDestroy(m);
		return 1;
	}

	print("Mutex process B id: ");
	printNum(pB);
	printNewLine();

	print("Creating \"Mutex process C\"\n");
	pC = newProcess("Mutex process C", processC, 0, NULL);
	if(pC == INVALID_PROCESS_ID) {
		print("Couldn't create \"Mutex process C\"\n");
		kill(pA);
		kill(pB);
		mutexDestroy(m);
		return 1;
	}

	print("Mutex process C id: ");
	printNum(pC);
	printNewLine();

	wPid(pA);
	wPid(pB);
	wPid(pC);

	mutexDestroy(m);

	return 0;
}

static int processA(int argc, char **argv) {
	int counter = CICLES;
	print("Mutex process A OK\n");
	
	while(counter > 0) {
		lock(m);

		if(locked == true) {
			lock(m);
			print("You shouldn't see this message.\n");
		}
		
		var = 2;
		print("Mutex processA: ");
		printNum(var);
		printNewLine();
		sleep(2);
		unlock(m);

		counter--;
	}

	return 0;
}

static int processB(int argc, char **argv) {
	int counter = CICLES;
	print("Mutex process B OK\n");

	while(counter > 0) {
		lock(m);
		var = 3;
		print("Mutex processB: ");
		printNum(var);
		printNewLine();
		unlock(m);
		sleep(3);

		counter--;
	}

	return 0;
}

static int processC(int argc, char **argv) {
	int counter = CICLES;
	print("Mutex process C OK\n");

	while(counter > 0) {
		lock(m);
		var = 5;
		print("Mutex processC: ");
		printNum(var);
		printNewLine();
		unlock(m);
		sleep(5);

		counter --;
	}

	return 0;
}