#ifndef __MUTEX_H_U_
	#define __MUTEX_H_U_

	#include "../../../Common/common.h"

	typedef void* mutex_u_t;

	/*
	** Initialize the lock.
	*/
	mutex_u_t initLock();

	/*
	** Acquires the lock.
	*/
	void lock(mutex_u_t lock);

	/*
	** Releases the lock.
	*/
	void unlock(mutex_u_t lock);

	void destroyLock(mutex_u_t lock);

#endif