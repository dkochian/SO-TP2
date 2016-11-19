
#ifndef _INC_SEMAPHORE_
	#define _INC_SEMAPHORE_

	#include "../../../include/common.h"
	#include "mutex.h"

	struct sem_t {
		mutex s_mutex;
		char * name;
		int value;
	};

	typedef struct sem_t *sem_t;

	sem_t semOpen(char * name, int value);
	void semClose(sem_t sem);
	void semWait(sem_t sem);
	void semPost(sem_t sem);

#endif