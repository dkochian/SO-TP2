#include "../utils/include/clock.h"
#include "../drivers/include/video.h"
#include "../system/ipc/include/semaphore.h"
#include "../system/scheduler/include/process.h"

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);
static int processC(int argc, char **argv);

static semaphore s;

void startSemaphoreTest() {
	uint64_t pA;
	uint64_t pB;
	uint64_t pC;

	s = semBuild(0);

	if(s == NULL) {
		print("Couldn't create the semaphore.\n", -1);
		return;
	}

	print("Creating \"Semaphore process A\"\n", -1);
	pA = newProcess("Semaphore process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process A\"\n", -1);
		semDestroy(s);
		return;
	}

	print("Semaphore process A id: ", -1);
	printDec(pA, -1);
	printNewLine();


	print("Creating \"Semaphore process B\"\n", -1);
	pB = newProcess("Semaphore process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process B\"\n", -1);
		freeProcess(pA);
		semDestroy(s);
		return;
	}

	print("Semaphore process B id: ", -1);
	printDec(pB, -1);
	printNewLine();


	print("Creating \"Semaphore process C\"\n", -1);
	pC = newProcess("Semaphore process C", processC, 0, NULL);
	if(pC == INVALID_PROCESS_ID) {
		print("Couldn't create \"Semaphore process C\"\n", -1);
		freeProcess(pA);
		freeProcess(pB);
		semDestroy(s);
		return;
	}

	print("Semaphore process C id: ", -1);
	printDec(pC, -1);
	printNewLine();
}

static int processA(int argc, char **argv) {
	print("Semaphore process A OK\n", -1);
	sleep(5);
	semPost(s);
	semPost(s);
	sleep(5);
	semPost(s);
	
	return 0;
}

static int processB(int argc, char **argv) {
	print("Semaphore process B OK\n", -1);

	semWait(s);
	print("[B] Hey the waiting is over!\n", -1);
	semWait(s);
	print("[B] The waiting is over again!\n", -1);
	
	return 0;
}

static int processC(int argc, char **argv) {
	print("Semaphore process C OK\n", -1);

	semWait(s);
	print("[C] The waiting is over!\n", -1);
	
	return 0;
}