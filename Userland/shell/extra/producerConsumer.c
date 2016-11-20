//AUTHOR: RODRIGO REARDEN - SO

#include <stdio.h>
#include <random.h>
#include <string.h>
#include <syscall.h>

#include "include/producerConsumer.h"


extern void insertItem(char c);
extern char removeItem();

static int producer(int argc, char **argv);
static int consumer(int argc, char **argv);
static void control();

extern int bufferSize;

static semaphore itemMutex;
static semaphore emptyCount;
static semaphore fullCount;

static int prodSleepTime = 0;
static int consSleepTime = 0;


//TEST
void mysleep(long int num);

void mysleep(long int num) {
	int i = num * 10000000;
	while(i-- > 0);
}

int producerConsumer(int argc, char **argv) {

	int i = 0;

	//Semaphore creation

	//Mutexes buffer access
	itemMutex = semBuild(0);

	if (itemMutex == NULL) {
		printn("Failed to create itemMutex");
		return 1;
	}

	//Counts full buffer slots
	fullCount = semBuild(0);

	if (fullCount == NULL) {
		printn("Failed to create fullCount");
		return 1;
	}

	//Counts empty buffer slots
	emptyCount = semBuild(0);

	if (emptyCount == NULL){
		printn("Failed to create emptyCount");
		return 1;
	}

	//Semaphore initialization
	semPost(itemMutex);

	for(i = 0; i < bufferSize; i++)
		semPost(emptyCount);

	printn("Press enter to start");
	while(getchar(true) != '\n');

	//Create processes
	uint64_t prod = newProcess("producer", producer, 0, 0);
	uint64_t cons = newProcess("consumer", consumer, 0, 0);

	//Control speed
	control();

	kill(prod);
	kill(cons);

	semDestroy(itemMutex);
	semDestroy(emptyCount);
	semDestroy(fullCount);
	return 0;
}

//static int a = 10;

static int producer(int argc, char **argv) {
	int item;

	while (1) {

		mysleep(prodSleepTime);

		item = rand()%100;
		print("Produce ");
		printNum(item);
		printNewLine();


		//Decrement the count of empty slots in the buffer (semaphore goes down)
		//Locks when the remaining empty slots are zero
		semWait(emptyCount);
		semWait(itemMutex);

		insertItem(item);
		semPost(itemMutex);

		//Increment the count of full slots in the buffer (semaphore goes up)
		semPost(fullCount);
	}

	return 0;
}

static int consumer(int argc, char **argv) {
	int item;

	while (1) {

		mysleep(consSleepTime);

		//Decrement the count of full slots in the buffer (semaphore goes down)
		//Locks when there is no more information in the buffer
		semWait(fullCount);
		semWait(itemMutex);

		item = removeItem();
		semPost(itemMutex);

		//Increment the count of empty slots in the buffer (semaphore goes up)
		semPost(emptyCount);

		print("Consume ");
		printNum(item);
		printNewLine();
	}

	return 0;
}

static void control() {
	int end = 0;

	while(!end) {
		int c = getchar(true);

		switch(c) {
			case 'a':
				printn(" Decreasing producer speed..");
				prodSleepTime++;
				break;

			case 'z':
				printn(" Increasing producer speed..");
				prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime;
				break;

			case 's':
				printn(" Decreasing consumer speed..");
				consSleepTime++;
				break;

			case 'x':
				printn(" Increasing consumer speed..");
				consSleepTime = --consSleepTime < 0? 0 : consSleepTime;
				break;

			case 'q':
				end = 1;
				break;
		}
	}
}
