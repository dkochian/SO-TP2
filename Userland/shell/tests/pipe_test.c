#include <stdio.h>
#include <syscall.h>

static uint64_t pipeId;
static bool writing;
static mutex p_mutex;

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

int pipeTestCommand(int argc, char **argv) {
	uint64_t pA;
	uint64_t pB;

	writing = true;

	print("Creating the pipe...\n");
	pipeId = pipeBuild();
	if(pipeId == INVALID_PIPE_ID) {
		print("Couldn't create the pipe.\n");
		return 1;
	}

	print("Creating the mutex...\n");
	p_mutex = mutexInit();
	if(p_mutex == NULL) {
		print("Couldn't create the mutex.\n");
		pipeDestroy(pipeId);
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

	return 0;
}

static int processA(int argc, char **argv) {
	char c;
	print("ProcessA read: ");
	while(writing == true) {
		read(STDPIPE, &c, pipeId);
		if(c != EMPTY)
			putchar(c);
	}
	printNewLine();

	return 0;
}

static int processB(int argc, char **argv) {
	write(STDPIPE, "h", NULL, pipeId);
	sleep(2);
	write(STDPIPE, "o", NULL, pipeId);
	sleep(1);
	write(STDPIPE, "l", NULL, pipeId);
	sleep(5);
	write(STDPIPE, "a", NULL, pipeId);

	lock(p_mutex);
	writing = false;
	unlock(p_mutex);

	return 1;
}