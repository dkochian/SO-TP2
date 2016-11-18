//AUTHOR: RODRIGO REARDEN - SO

#include <stdio.h>
#include <random.h>
#include <string.h>
#include <syscall.h>
#include "include/producerConsumer.h"
#include "../libc/include/semaphore.h"


extern void insertItem(char c);
extern char removeItem();

static int producer(int argc, char **argv);
static int consumer(int argc, char **argv);
static void control();

extern int bufferSize;

static sem_u_t itemMutex;
static sem_u_t emptyCount;
static sem_u_t fullCount;

static int prodSleepTime = 0;
static int consSleepTime = 0;

int producerConsumer(int argc, char ** argv) {

	int i = 0;

	//Semaphore creation

	//Mutexes buffer access
	itemMutex = sem_open("itemMutex", 700);

	if (itemMutex == NULL) {
		printn("Failed to create itemMutex");
	}

	//Counts full buffer slots
	fullCount = sem_open("fullCount", 700);

	if (fullCount == NULL) {
		printn("Failed to create fullCount");
	}

	//Counts empty buffer slots
	emptyCount = sem_open("emptyCount", 700);

	if (emptyCount == NULL){
		printn("Failed to create emptyCount");
	}

	//Semaphore initialization
	sem_post(itemMutex);

	for(i = 0; i < bufferSize; i++)
		sem_post(emptyCount);

	printn("Press enter to start");
	while(getchar(true) != '\n');

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

	return 1;
}

//static int a = 10;

static int producer(int argc, char **argv) {
	int item;

	while (1) {
//		printn("ANTES SLEEP");
		sleep(prodSleepTime);
//		printn("DESPUES SLEEP");

		item = rand()%100;
		print("Produce ");
		printNum(item);
		printNewline();


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
	int item;

	while (1) {
		sleep(consSleepTime);

		//Decrement the count of full slots in the buffer (semaphore goes down)
		//Locks when there is no more information in the buffer
		sem_wait(fullCount);
		sem_wait(itemMutex);

		item = removeItem();
		sem_post(itemMutex);

		//Increment the count of empty slots in the buffer (semaphore goes up)
		sem_post(emptyCount);
		print("Consume ");
		printNum(item);
		printNewline();
	}

	return 0;
}

static void control() {
	int end = 0;

	while(!end) {
		int c = getchar(true);

		switch(c) {
			case 'a':
				printn("Decreasing producer speed..");
				prodSleepTime++;
				break;

			case 'z':
				printn("Increasing producer speed..");
				prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime;
				break;

			case 's':
				printn("Decreasing consumer speed..");
				consSleepTime++;
				break;

			case 'x':
				printn("Increasing consumer speed..");
				consSleepTime = --consSleepTime < 0? 0 : consSleepTime;
				break;

			case 'q':
				end = 1;
				break;
		}
	}
}
