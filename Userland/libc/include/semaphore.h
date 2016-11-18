#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../../../Common/common.h"


typedef void* sem_u_t;

sem_u_t sem_open(char * name, int value);
void sem_close(sem_u_t sem);
void sem_wait(sem_u_t sem);
void sem_post(sem_u_t sem);

#endif