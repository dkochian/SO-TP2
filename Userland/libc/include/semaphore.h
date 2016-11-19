#ifndef SEMAPHORE_H
	#define SEMAPHORE_H

	#include "../../../Common/common.h"

	typedef void* sem_u_t;

	sem_u_t semOpen(char * name, int value);
	void semClose(sem_u_t sem);
	void semWait(sem_u_t sem);
	void semPost(sem_u_t sem);

#endif