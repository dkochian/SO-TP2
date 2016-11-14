#ifndef __MUTEX_H_U_
	#define __MUTEX_H_U_

	#include "../../../Common/common.h"

	typedef volatile bool mutex_t;

	/*
	** Initialize the lock.
	*/
	void initMutex(mutex_t * lock);

	/*
	** Acquires the lock.
	*/
	void lock(mutex_t * lock);

	/*
	** Releases the lock.
	*/
	void unlock(mutex_t * lock);

#endif