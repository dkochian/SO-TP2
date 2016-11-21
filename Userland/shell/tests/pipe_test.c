#include <stdio.h>
#include <syscall.h>

static uint64_t pipeId;
static mutex p_mutex;

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

int pipeTestCommand(int argc, char **argv) {
	uint64_t pA;
	uint64_t pB;

	print("Creating the pipe...\n");
	pipeId = pipeBuild();
	if(pipeId == INVALID_PIPE_ID) {
		print("Couldn't create the pipe.\n");
		return 1;
	}

	print("Creating \"pipe process A\"...\n");
	pA = newProcess("pipe process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		pipeDestroy(pipeId);
		mutexDestroy(p_mutex);
		print("Couldn't create \"pipe process A\"\n");
		return 1;
	}

	print("Creating \"pipe process B\"...\n");
	pB = newProcess("pipe process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		pipeDestroy(pipeId);
		mutexDestroy(p_mutex);
		kill(pA);
		print("Couldn't create \"pipe process B\"\n");
		return 1;
	}

	wPid(pA);
	wPid(pB);

	pipeDestroy(pipeId);

	return 0;
}

static int processA(int argc, char **argv) {
	char buffer[MAX_BUFFER];
	print("ProcessA read: ");
	read(STDPIPE, buffer, pipeId);
	print(buffer);
	printNewLine();

	return 0;
}

static int processB(int argc, char **argv) {
	sleep(1);
	write(STDPIPE, "hola", NULL, pipeId);

	return 1;
}