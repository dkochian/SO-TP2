
#ifndef _INC_SEMAPHORE_
	#define _INC_SEMAPHORE_

	#include "../../../Common/common.h"
	//#include "../include/mutex.h"
	#include "../scheduler/include/simpleProcessList.h"

	typedef struct sem_t {
		//mutex *s_mutex;
		//char * name;
		int value;
		sList q_list;
	}sem_t;

	sem_t *semOpen(char * name, int value);
	void semClose(sem_t * sem);
	void semWait(sem_t * sem);
	void semPost(sem_t * sem);

#endif