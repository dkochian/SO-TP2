#include "include/mutex.h"
#include "include/syscall.h"


mutex_u_t initLock() {
	return (mutex_u_t) sysMutexInit();
}

void destroyLock(mutex_u_t lock) {
	sysMutexDestroy(lock);
}

void lock(mutex_u_t lock) {
	sysMutexLock(lock);
}

void unlock(mutex_u_t lock) {
	sysMutexUnlock(lock);
}