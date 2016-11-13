#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <mutex.h>
#include "include/philosophers.h"
#include "../../Common/common.h"

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
void grabSem(mutex_t* lock_v, volatile int* value) {
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

void releaseSem(mutex_t* lock_v, volatile int* value) {
	lock(lock_v);
	(*value)++;
	unlock(lock_v);
}

/******************
**  Philosopher  **
******************/
void philosopher(int pos, int left) {
	int right = pos;
	bool alive = true;
	while(alive) {
		// THINKING
		grabSem(&semLock, &sem);
		lock(& (forks[right]) );
		lock(& (forks[left]) );
		// EAT
		unlock(& (forks[right]) );
		unlock(& (forks[left]) );
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
}

/**************
**  Control  **
**************/
void init() {
	total = INIT_PHILOSOPHERS;
	sem = total-1;
	initMutex(&semLock);
	initMutex(&editLock);
	for(int i=0; i<MAX_PHILOSOPHERS; i++) {
		initMutex(& (forks[i]) );
		edit[i] = NO_ACTION;
	}
}
void launchPhilosopher(int pos, int left) {
	// new process stuff here

	//newProcess(void * entryPoint, char* name, bool foreground);
	//philosopher(pos, left);
	
	// new process stuff here
}
void action(int pos, edit_t value) {
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

bool addPhilosopher() {
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

bool removePhilosopher() {
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
void exitNicely() {
	for(int i = total-1; i>=0; i--) {
		action(i, KILL);
	}
}

/***********
**  Main  **
***********/
void philosophers() {
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
