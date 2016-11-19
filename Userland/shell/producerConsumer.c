//AUTHOR: RODRIGO REARDEN - SO

#include <stdio.h>
#include <random.h>
#include <string.h>
#include <syscall.h>
#include <semaphore.h>

#include "include/producerConsumer.h"


extern void insertItem(char c);
extern char removeItem();

static int producer(int argc, char **argv);
static int consumer(int argc, char **argv);
static void control();

extern int bufferSize;

//pthread_t producerThread;
//pthread_t consumerThread;

static sem_u_t itemMutex;
static sem_u_t emptyCount;
static sem_u_t fullCount;

static int prodSleepTime = 1;
static int consSleepTime = 1;

int producerConsumer(int argc, char ** argv) {
	int i = 0;


//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Semaphore creation

//	sem_unlink("itemMutex");
//	sem_unlink("fullCount");
//	sem_unlink("emptyCount");

	//Mutexes buffer access
//	itemMutex = sem_open("itemMutex", O_CREAT, O_EXCL, 700);

	itemMutex = semOpen("itemMutex", 700);

	if (itemMutex == NULL) {
		printn("Failed to create itemMutex");
		return 0;
	}

	//Counts full buffer slots
	fullCount = semOpen("fullCount", 700);

	if (fullCount == NULL) {
		printn("Failed to create fullCount");
		semClose(itemMutex);
		return 0;
	}

	//Counts empty buffer slots
	emptyCount = semOpen("emptyCount", 700);

	if (emptyCount == NULL){
		printn("Failed to create emptyCount");
		semClose(itemMutex);
		semClose(fullCount);
		return 0;
	}

	//Semaphore initialization
	semPost(itemMutex);

	for(i = 0; i < bufferSize; i++)
		semPost(emptyCount);

//	printn("Press enter to start");
//	//TODO: MODIFICAR PARA QUE USE GETCHAR NUESTRO
//	getchar(1);

	//Create threads
//	pthread_create(&producerThread, &attr, producer, NULL);
//	pthread_create(&consumerThread, &attr, consumer, NULL);
	newProcess("producer", producer, 0, 0);
	newProcess("consumer", consumer, 0, 0);

	//Control thread speed
	control();

//	pthread_attr_destroy(&attr);
//	pthread_exit(NULL);

	semClose(itemMutex);
	semClose(emptyCount);
	semClose(fullCount);

//	sem_unlink("itemMutex");
//	sem_unlink("emptyCount");
//	sem_unlink("fullCount");

	return 1;
}

//static int a = 10;

static int producer(int argc, char **argv) {
	int item;

	while (1) {
		sleep(prodSleepTime);

		//char stackChar = 'a';

//		char item = rand();
		item = rand()%100;
		print("Produce ");
		printNum(item);
		printNewline();


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
		sleep(consSleepTime);

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
		printNewline();
	}

	return 0;
}

static void control() {
	int end = 0;

	while(!end) {
//		int c = getchar(1);
		int c = 0;
		switch(c) {
			case 'a':
				prodSleepTime++;
			break;

			case 'z':
				prodSleepTime = --prodSleepTime < 0? 0 : prodSleepTime;
			break;

			case 's':
				consSleepTime++;
			break;

			case 'x':
				consSleepTime = --consSleepTime < 0? 0 : consSleepTime;
			break;

			case 'q':
				end = 1;
			break;
		}
	}
}
