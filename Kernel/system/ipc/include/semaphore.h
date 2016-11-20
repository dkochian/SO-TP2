#ifndef _INC_SEMAPHORE_
	#define _INC_SEMAPHORE_

	#include "../../../include/common.h"
	#include "mutex.h"
	#include "../../../utils/include/queue.h"

	typedef struct semaphore *semaphore;

	semaphore semBuild(int value);
	void semDestroy(semaphore s);
	void semWait(semaphore s);
	void semPost(semaphore s);

#endif