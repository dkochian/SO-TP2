#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <draw.h>

#include "include/philosophers.h"
#include "include/commands.h"
#include "../include/common.h"

//extern void putPixel(uint16_t x, uint16_t y, ColorRGB* color);

static void control();
/*
ColorRGB baseColors[16] = {
		{0,		0,		0},
		{0,		0,		255},
		{0,		128,	0},
		{0,		255,	255},
		{255,	0,		0},
		{255,	0,		255},
		{165,	42,		42},
		{211,	211,	211},
		{128,	128,	128},
		{0,		128,	255},
		{144,	238,	144},
		{224,	255,	255},
		{255,	77,		77},
		{237,	83,		147},
		{255,	255,	0},
		{255,	255,	255}
	};
*/


//	http://pseudomuto.com/development/2014/03/01/dining-philosophers-in-c/

typedef enum {
	NO_ACTION, INC, DEC, KILL
} edit_t;

/*****************
**  Global Data **
*****************/
volatile static int speed;
uint64_t childs[MAX_PHILOSOPHERS];

volatile static int total;
mutex forks[MAX_PHILOSOPHERS];

												//mutex_u_t semLock;
												//volatile int sem;
semaphore sem;

mutex editLock;
volatile edit_t edit[MAX_PHILOSOPHERS];

/***************
**  Semaphore **
***************/
/*static void grabSem(mutex_u_t lock_v, volatile int* value) {
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

static void releaseSem(mutex_u_t lock_v, volatile int* value) {
	print("AAA");
	lock(lock_v);
	print("BBB");
	(*value)++;
	print("CCC");
	unlock(lock_v);
	printn("dropped");
}
*/
/**************
**  Display  **
**************/

#define LEFT false
#define RIGHT true


/*void doA(int x, int y, int l, char color) {
	ColorRGB c = baseColors[(uint16_t)color];
	for(int i=0; i<l; i++) {
		for(int j=0; j<l; j++) {
			print("<");
			putPixel(x+i, y+j , &c);
			print(">");
		}
	}
}*/
#define SIZE (64)
#define SPACER (16)
#define TOPGAP (48)
static void updateSquare(int pos, char color) {
	//ColorRGB c = charToRGB(color);
	int x = SPACER + (SIZE+SPACER)*(pos);
	int y = TOPGAP;
	int l = SIZE;
	drawSquare(x,y,l,color);
	//doA(x, y, l, color);
}

static void updateState(int pos, bool hand, char color) {
	//ColorRGB c = charToRGB(color);
	if(hand==LEFT) {
		int x = SPACER+ (SIZE+SPACER)*(pos);
		int y = TOPGAP;
		int l = SIZE/2;
		drawSquare(x,y,l,color);
		//doA(x, y, l, color);
		y += SIZE/2;
		drawSquare(x,y,l,color);
		//doA(x, y, l, color);
	} else {
		int x = SPACER+SIZE/2+ (SIZE+SPACER)*(pos);
		int y = TOPGAP;
		int l = SIZE/2;
		drawSquare(x,y,l,color);
		//doA(x, y, l, color);
		y += SIZE/2;
		drawSquare(x,y,l,color);
		//doA(x, y, l, color);
	}
}

static void mysleep(long int num) {
	int i = num * 10000000;
	while(i-- > 0);
}

static void sleep2() {
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
	//print("Generated Philosopher N°: ");
	int left = pos;
	bool alive = true;
	
	/*printNum(pos);
	print(":");
	printNum(left);
	print("-");
	printNum(right);
	printNewLine();
	*/
	while(alive) {
		// THINKING
		updateSquare(pos, BLUE);
		sleep2();
		
		// HUNGRY
		updateSquare(pos, MAGENTA);
		//sleep2();

		semWait(sem);
												//grabSem(semLock, &sem);
		
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
												//releaseSem(semLock, &sem);
		
		// OUT EAT
		updateSquare(pos, LIGHT_BLUE);
		sleep2();

		lock(editLock);
		
		// CHECK MESSAGE
		updateSquare(pos, DARK_GRAY);

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
												//sem = total-1;
												//semLock = mutexInit();
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

static void launchPhilosopher(int pos, int left) {
	int argc = 2;

	char** argv = (char**) malloc(sizeof(char*)*argc);
	char* arg1 = (char*) malloc(sizeof(char*)*64);
	char* arg2 = (char*) malloc(sizeof(char*)*64);
	itoa(pos, arg1);
	itoa(left, arg2);
	argv[0] = arg1;
	argv[1] = arg2;
	childs[pos] = newProcess("Phi-Child", philosopher, argc, argv);
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
	launchPhilosopher(total, total-1);
	action(0, INC);

	semPost(sem);

										//grabSem(semLock, &sem);
										//sem++;
										//releaseSem(semLock, &sem);
	total++;
	return true;
}

static bool removePhilosopher() {
	if(total<=MIN_PHILOSOPHERS)
		return false;

	semWait(sem);
										//grabSem(semLock, &sem);
										//sem--;
										//releaseSem(semLock, &sem);
	action(0, DEC);
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
										//mutexDestroy(semLock);
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
	
	//color  code
	print("Colors:  ");
	printColor("Thinking  ", BLUE);
	printColor("Hungry  ", MAGENTA);
	printColor("Enabled  ", RED);
	printColor("Eating  ", GREEN);
	printColor("Done Eating  ", LIGHT_GREEN);
	printColor("Disabled  ", LIGHT_BLUE);
	printnColor("Checking  ", DARK_GRAY);

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
				print("Adding philosopher...      ");
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
				print("Removing philosopher...    ");
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
				if(speed < MAX_SPEED)
					speed++;
				break;
			case 'd':
			case 'D':
				if(speed > MIN_SPEED)
					speed--;
				break;
			case 'q':
			case 'Q':
				end = true;
				break;
			default:
				printn("Press: 'w' to add, 's' to remove, 'd' to speed up, 'a' to slow down, & 'q' to quit.");
				psCommand(0, NULL);
		}
	}
}
