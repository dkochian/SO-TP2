#include "include/mutex.h"
#include "include/syscall.h"

#define INIT_MUTEX 0x0
#define LOCKED_MUTEX 0x01

void initMutex(mutex_t * lock) {
	*lock = INIT_MUTEX;
}

void lock(mutex_t * lock) {
	while(__sync_lock_test_and_set(lock, LOCKED_MUTEX)) {
		yield();
	}
}

void unlock(mutex_t * lock) {
	__sync_lock_release(lock);
}
