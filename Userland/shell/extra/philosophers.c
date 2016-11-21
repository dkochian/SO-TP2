//	http://pseudomuto.com/development/2014/03/01/dining-philosophers-in-c/
#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <draw.h>

#include "include/philosophers.h"
#include "include/commands.h"
#include "../include/common.h"

#define LEFT false
#define RIGHT true
#define SIZE (64)
#define SPACER (16)
#define TOPGAP (48)

static void colorLegend();
static void control();

typedef enum {
	NO_ACTION, ZERO, PLUS, KILL
} edit_t;

/*****************
**  Global Data **
*****************/
volatile static int speed;

uint64_t childs[MAX_PHILOSOPHERS];

volatile static int total;
mutex forks[MAX_PHILOSOPHERS];

semaphore sem;

mutex editLock;
volatile edit_t edit[MAX_PHILOSOPHERS];

/**************
**  Display  **
**************/
static void updateSquare(int pos, char color) {
	int x = SPACER + (SIZE+SPACER)*(pos);
	int y = TOPGAP;
	int l = SIZE;
	drawSquare(x,y,l,color);
}

static void updateState(int pos, bool hand, char color) {
	if(hand==LEFT) {
		int x = SPACER+ (SIZE+SPACER)*(pos);
		int y = TOPGAP;
		int l = SIZE/2;
		drawSquare(x,y,l,color);
		y += SIZE/2;
		drawSquare(x,y,l,color);
	} else {
		int x = SPACER+SIZE/2+ (SIZE+SPACER)*(pos);
		int y = TOPGAP;
		int l = SIZE/2;
		drawSquare(x,y,l,color);
		y += SIZE/2;
		drawSquare(x,y,l,color);
	}
}

static void mysleep(long int num) {
	int i = num * 10000000;
	while(i-- > 0);
}

static void sleep2() {
	/* 
	** I tried being nice... sleep syscall hangs everything.
	** Don't believe me?
	** Switch to commented code and try it out...
	*/

	/*for(int i=0; i<speed; i++)
		sleep(1);*/
	mysleep(speed);
}

/******************
**  Philosopher  **
******************/
static int philosopher(int argc, char** argv) {
	int pos = strintPos(argv[0]);
	int right = strintPos(argv[1]);
	free(argv[0]);
	free(argv[1]);
	free(argv);

	int left = pos;
	bool alive = true;
	while(alive) {
		// THINKING
		updateSquare(pos, BLUE);
		sleep2();
		
		// HUNGRY
		updateSquare(pos, MAGENTA);
		//sleep2();

		semWait(sem);
		
		// CAN GRAB
		updateSquare(pos, RED);
		sleep2();

		lock( (forks[right]) );

		// GRABBED RIGHT
		updateState(pos, RIGHT, GREEN);
		//sleep2();

		lock( (forks[left]) );
		
		// GRABBED LEFT - EATING
		updateState(pos, LEFT, GREEN);
		sleep2();

		unlock( (forks[right]) );

		// DROPPED RIGHT
		updateState(pos, RIGHT, LIGHT_GREEN);
		//sleep2();

		unlock( (forks[left]) );
		
		// DROPPED LEFT
		updateState(pos, LEFT, LIGHT_GREEN);
		//sleep2();

		semPost(sem);
		
		// OUT EAT
		updateSquare(pos, LIGHT_BLUE);
		sleep2();

		
		// CHECK MESSAGE
		lock(editLock);
		updateSquare(pos, DARK_GRAY);
		switch( edit[pos] ) {
			case NO_ACTION:
				break;
			case ZERO: {
				right = 0;
				edit[pos] = NO_ACTION;
				break;
			}
			case PLUS: {
				right = pos+1;
				edit[pos] = NO_ACTION;
				break;
			}
			case KILL: {
				alive = false;
				edit[pos] = NO_ACTION;
				break;
			}
		}
		unlock(editLock);
	}

	updateSquare(pos, BLACK);
	return 0;
}

/**************
**  Control  **
**************/
static int init() {
	speed = INIT_SPEED;
	total = INIT_PHILOSOPHERS;
	sem = semBuild(total-1);
	if(sem==NULL) {
		return 1;
	}
	editLock = mutexInit();
	if(editLock ==NULL) {
		semDestroy(sem);
		return 1;
	}
	for(int i=0; i<MAX_PHILOSOPHERS; i++) {
		forks[i] = mutexInit();
		if(forks[i] == NULL) {	//ERROR
			semDestroy(sem);
			mutexDestroy(editLock);
			for(int j=0; j<i; j++)
				mutexDestroy(forks[j]);
			return 1;
		}
		edit[i] = NO_ACTION;
		childs[i] = NULL;
	}
	return 0;
}

static void launchPhilosopher(int pos, int right) {
	int argc = 2;

	char** argv = (char**) malloc(sizeof(char*)*argc);
	char* arg1 = (char*) malloc(sizeof(char*)*64);
	char* arg2 = (char*) malloc(sizeof(char*)*64);
	itoa(pos, arg1);
	itoa(right, arg2);
	argv[0] = arg1;
	argv[1] = arg2;
	childs[pos] = newProcess("phiChild", philosopher, argc, argv);
}

static void action(int pos, edit_t value) {
	lock(editLock);
	edit[pos] = value;
	unlock(editLock);
	bool done = false;
	do {
		lock(editLock);
		edit_t temp = edit[pos];
		unlock(editLock);
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
	launchPhilosopher(total, 0);
	action(total-1, PLUS);
	semPost(sem);
	total++;
	return true;
}

static bool removePhilosopher() {
	if(total<=MIN_PHILOSOPHERS)
		return false;
	semWait(sem);
	action(total-2, ZERO);
	action(total-1, KILL);
	total--;
	return true;
}

static void exitNicely() {
	for(int i=0; i< MAX_PHILOSOPHERS; i++) {
		if(childs[i]!=NULL) {
			kill(childs[i]);
		}
	}
	semDestroy(sem);
	mutexDestroy(editLock);
	for(int i=0; i<MAX_PHILOSOPHERS; i++) {
		mutexDestroy(forks[i]);
	}
}

/***********
**  Main  **
***********/
int philosophers(int argc, char **argv) {
	clear();
	printn("Loading Dining Philosophers...");
	if( init()==1) {
		printn("Error loading Philosophers");
		return 1;
	}
	for(int i=0; i<total; i++) {
		launchPhilosopher(i, (i+1)%total );
	}
	colorLegend();
	printNewLine();
	printNewLine();
	printNewLine();
	printNewLine();
	printNewLine();
	printNewLine();

	control();

	printn("Exiting Philosophers...");
	exitNicely();
	clear();
	return 0;
}

/*****************************
**  Helpers - Main Process  **
*****************************/
static void colorLegend() {
	print("    Colors:  ");
	printColor("Thinking  ", BLUE);
	printColor("Hungry  ", MAGENTA);
	printColor("Enabled  ", RED);
	printColor("Eating  ", GREEN);
	printColor("Done Eating  ", LIGHT_GREEN);
	printColor("Disabled  ", LIGHT_BLUE);
	printnColor("Checking  ", DARK_GRAY);
}
static void control() {
	bool end = false;
	int c;
	while(!end) {
		print("Input: ");
		do {
			c = getchar(true);
		} while(c==0);
		printNewLine();
		switch(c) {
			case 'w':
			case 'W':
				print("    Adding philosopher...      ");
				if(addPhilosopher()) {
					printnColor("Done.", GREEN);
				} else {
					printColor("Can't add. ", RED);
					print("Max is ");
					printNum(MAX_PHILOSOPHERS);
					printn(".");
				}
				break;
			case 's':
			case 'S':
				print("    Removing philosopher...    ");
				if(removePhilosopher()) {
					printnColor("Done.", GREEN);
				} else {
					printColor("Can't remove. ", RED);
					print("Min is ");
					printNum(MIN_PHILOSOPHERS);
					printn(".");
				}
				break;
			case 'a':
			case 'A':
				if(speed < MAX_SPEED) {
					print("    Speed decreased");
					speed++;
					if(speed == MAX_SPEED) {
						printnColor(" to TURTLE SPEED!", GREEN);
					} else {
						printn(".");
					}
				} else {
					printnColor("    Snorlax is your spirit Pokemon. ", LIGHT_GRAY);
				}
				break;
			case 'd':
			case 'D':
				if(speed > MIN_SPEED) {
					print("    Speed increased");
					speed--;
					if(speed==MIN_SPEED) {
						printnColor(" to TURBO!", RED);
					} else {
						printn(".");
					}
				} else {
					printnColor("    2 FAST 2 FURIOUS 4 ME", RED);
				}
				break;
			case 'q':
			case 'Q':
				end = true;
				break;
			default:
				printn("    Press: 'w' to add, 's' to remove, 'd' to speed up, 'a' to slow down, & 'q' to quit.");
				colorLegend();
		}
	}
}
