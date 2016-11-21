//AUTHOR: RODRIGO REARDEN - SO

#include <stdio.h>
#include <random.h>
#include <string.h>
#include <syscall.h>
#include <semaphore.h>

#include "include/producerConsumer.h"
#include "include/visualProdCons.h"


extern void insertItem(char c);
extern char removeItem();

static int producer(int argc, char **argv);
static int consumer(int argc, char **argv);
static void control();

extern int bufferSize;

static sem_u_t itemMutex;
static sem_u_t emptyCount;
static sem_u_t fullCount;

static int prodSleepTime = 4;
static int consSleepTime = 5;


void mysleep(long int num);


void producerConsumer() {

	clear();

	int i = 0;

	//Semaphore creation

	//Mutexes buffer access
	itemMutex = sem_open("itemMutex", 0);

	if (itemMutex == NULL) {
		printn("Failed to create itemMutex");
	}

	//Counts full buffer slots
	fullCount = sem_open("fullCount", 0);

	if (fullCount == NULL) {
		printn("Failed to create fullCount");
	}

	//Counts empty buffer slots
	emptyCount = sem_open("emptyCount", 0);

	if (emptyCount == NULL){
		printn("Failed to create emptyCount");
	}

	//Semaphore initialization
	sem_post(itemMutex);

	for(i = 0; i < bufferSize; i++) {
		sem_post(emptyCount);
	}

	printn("Press enter to start");
	while(getchar(true) != '\n');

	initVisual();

	//Create processes
	uint64_t prod = newProcess("producer", producer, 0, 0);
	uint64_t cons = newProcess("consumer", consumer, 0, 0);

	//Control speed
	control();

	kill(prod);
	kill(cons);

	sem_close(itemMutex);
	sem_close(emptyCount);
	sem_close(fullCount);

	clear();
}

//static int a = 10;

static int producer(int argc, char **argv) {

	while (1) {

		mysleep(prodSleepTime);

		//We used a fixed value only for visual purposes
		int item = 50;
//		item = rand()%100;

		//Decrement the count of empty slots in the buffer (semaphore goes down)
		//Locks when the remaining empty slots are zero
		sem_wait(emptyCount);
		sem_wait(itemMutex);

		insertItem(item);
		sem_post(itemMutex);

		//Increment the count of full slots in the buffer (semaphore goes up)
		sem_post(fullCount);
	}

	return 0;
}

static int consumer(int argc, char **argv) {

	while (1) {

		mysleep(consSleepTime);

		//Decrement the count of full slots in the buffer (semaphore goes down)
		//Locks when there is no more information in the buffer
		sem_wait(fullCount);
		sem_wait(itemMutex);

		removeItem();
		sem_post(itemMutex);

		//Increment the count of empty slots in the buffer (semaphore goes up)
		sem_post(emptyCount);
	}

	return 0;
}


//The sleep we have doesnt work well for this program.
void mysleep(long int num) {
	int i = num * 10000000;
	while(i-- > 0);
}

static void control() {
	int end = 0;

	while(!end) {
		int c = getchar(false);

		switch(c) {
			case 'a':
//				printn(" Decreasing producer speed..");
				prodSleepTime++;
				break;

			case 'z':
//				printn(" Increasing producer speed..");
				prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime;
				break;

			case 's':
//				printn(" Decreasing consumer speed..");
				consSleepTime++;
				break;

			case 'x':
//				printn(" Increasing consumer speed..");
				consSleepTime = --consSleepTime < 0? 0 : consSleepTime;
				break;

			case 'q':
				end = 1;
				break;
		}
	}
}
