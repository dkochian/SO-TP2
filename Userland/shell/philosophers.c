#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <mutex.h>
#include "include/philosophers.h"
#include "../../Common/common.h"

// WHY U TROLL ME SO, PHILOSOPHER?!?!

//	http://pseudomuto.com/development/2014/03/01/dining-philosophers-in-c/

typedef enum {
	NO_ACTION, INC, DEC, KILL
} edit_t;

/*****************
**  Global Data **
*****************/
int total;
mutex_t forks[MAX_PHILOSOPHERS];

mutex_t semLock;
volatile int sem;

mutex_t editLock;
volatile edit_t edit[MAX_PHILOSOPHERS];

/***************
**  Semaphore **
***************/
static void grabSem(mutex_t* lock_v, volatile int* value) {
	while(true) {
		lock(lock_v);
		if( (*value)>0 ) {
			(*value)--;
			unlock(lock_v);
			return;
		} else {
			unlock(lock_v);
			yield();
		}
	}
}

static void releaseSem(mutex_t* lock_v, volatile int* value) {
	lock(lock_v);
	(*value)++;
	unlock(lock_v);
}

/******************
**  Philosopher  **
******************/
static int philosopher(int argc, char** argv) {
	int pos = strintPos(argv[0]);
	int left = strintPos(argv[1]);

	print("Generated Philosopher N°: ");
	printNum(pos);
	printNewline();

	int right = pos;
	bool alive = true;
	while(alive) {
		// THINKING
			printNum(pos);
			printn(" is thinking...");
		sleep(5);
			printNum(pos);
			printn(" is hungry.");
		grabSem(&semLock, &sem);
		lock(& (forks[right]) );
			printNum(pos);
			printn(" grabbed right fork.");
		lock(& (forks[left]) );
			printNum(pos);
			printn(" grabbed left fork.");
		// EAT
			printNum(pos);
			printn(" is eating...");
		sleep(2);		

		unlock(& (forks[right]) );
		unlock(& (forks[left]) );
			printNum(pos);
			printn(" dropped both forks.");
		releaseSem(&semLock, &sem);

		lock(&editLock);
		switch( edit[pos] ) {
			case INC: {
				left++;
				break;
			}
			case DEC: {
				left--;
				break;
			}
			case KILL: {
				alive = false;
				break;
			}
			case NO_ACTION:
			default:
				;
		}
		edit[pos] = NO_ACTION;
		unlock(&editLock);
	}
	return 0;
}

/**************
**  Control  **
**************/
static void init() {
	total = INIT_PHILOSOPHERS;
	sem = total-1;
	initMutex(&semLock);
	initMutex(&editLock);
	for(int i=0; i<MAX_PHILOSOPHERS; i++) {
		initMutex(& (forks[i]) );
		edit[i] = NO_ACTION;
	}
}

static void launchPhilosopher(int pos, int left) {
	int argc = 2;
	char arg1[8];
	char arg2[8];
	itoa(pos, arg1);
	itoa(left, arg2);
	char* argv[2] = {arg1, arg2};
	newProcess("pChild", philosopher, argc, argv);
}

static void action(int pos, edit_t value) {
	lock(&editLock);
	edit[pos] = value;
	unlock(&editLock);
	bool done = false;
	do {
		lock(&editLock);
		edit_t temp = edit[pos];
		unlock(&editLock);
		if(temp == NO_ACTION) {
			done = true;
		} else {
			yield();
		}
	} while(!done);
	return;
}

static bool addPhilosopher() {
	if(total>=MAX_PHILOSOPHERS)
		return false;
	launchPhilosopher(total, total-1);
	action(0, INC);
	grabSem(&semLock, &sem);
	sem++;
	releaseSem(&semLock, &sem);
	total++;
	return true;
}

static bool removePhilosopher() {
	if(total<=MIN_PHILOSOPHERS)
		return false;
	grabSem(&semLock, &sem);
	sem--;
	releaseSem(&semLock, &sem);
	action(0, DEC);
	action(total-1, KILL);
	total--;
	return true;
}

static void exitNicely() {
	for(int i = total-1; i>=0; i--) {
		action(i, KILL);
	}
}

/***********
**  Main  **
***********/
void philosophers() {
	printn("Loading Philosophers...");
	init();
	for(int i=0; i<total; i++) {
		launchPhilosopher(i, (i+1)%total );
	}
	bool exitFlag = false;
	char c;
	while(!exitFlag) {
		c = getchar(false);
		if(c=='w' || c=='W') {
			
			if(addPhilosopher()) {
				// success
			} else {
				// cant add. max is MAX_PHILOSOPHERS
			} 

		} else if(c=='s' || c=='S') {

			if(removePhilosopher()) {
				// sucess
			} else {
				// can't remove. min is MIN_PHILOSOPHERS
			}

		} else {
			exitNicely();
			exitFlag = true;
		}
	}
}