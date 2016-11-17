#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../../../Common/common.h"
#include <mutex.h>

typedef struct sem_t {
	mutex_u_t lock;
	char * name;
	int value;
}sem_t;

sem_t * sem_open(char * name, int value);
void sem_close(sem_t * sem);
void sem_wait(sem_t * sem);
void sem_post(sem_t * sem);

#endif