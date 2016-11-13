#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include "include/philosophers.h"
#include "../../Common/common.h"

//	http://pseudomuto.com/development/2014/03/01/dining-philosophers-in-c/

typedef enum {
	NO_ACTION, INC, DEC, KILL
} edit_t;

typedef volatile bool mutex;

#define INIT_MUTEX 0;

/**********************
**  FALTAN DEFINIR!  **			// esto esta para que no rompa la compilación
**********************/
void lock(mutex* lock_v) {}
void unlock(mutex* lock_v) {}
void yield() {}

/*****************
**  Global Data **
*****************/
int total;
mutex forks[MAX_PHILOSOPHERS];

mutex semLock;
volatile int sem;

mutex editLock;
volatile edit_t edit[MAX_PHILOSOPHERS];

/***************
**  Semaphore **
***************/
void grabSem(mutex* lock_v, volatile int* value) {
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

void releaseSem(mutex* lock_v, volatile int* value) {
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
	semLock = INIT_MUTEX;
	editLock = INIT_MUTEX;
	for(int i=0; i<MAX_PHILOSOPHERS; i++) {
		forks[i] = INIT_MUTEX;
		edit[i] = NO_ACTION;
	}
}
void launchPhilosopher(int pos, int left) {
	// new process stuff here
	philosopher(pos, left);
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
