#ifndef _INC_SEMAPHORE_
#define _INC_SEMAPHORE_

#include "../../../include/common.h"
#include "mutex.h"
#include "../../../utils/include/queue.h"

struct sem_t {
	mutex s_mutex;
	char * name;
	int value;
	queue s_queue;
};

typedef struct sem_t * sem_t;

sem_t semOpen(char * name, int value);

void semClose(sem_t sem);

void semWait(sem_t sem);

void semPost(sem_t sem);

#endif