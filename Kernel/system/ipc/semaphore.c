#include "include/semaphore.h"
#include "../include/mmu.h"


sem_t semOpen(char * name, int value) {
	sem_t sem = k_malloc(sizeof(sem_t));

	if (sem == NULL)
		return NULL;

	sem->s_mutex = initLock();

	if (sem->s_mutex == NULL) {
		k_free(sem);
		return NULL;
	}

	sem->name = name;
	sem->value = value;
	return sem;
}

void semClose(sem_t sem) {
	destroyLock(sem->s_mutex);
	k_free(sem);
}

void semWait(sem_t sem) {
	lock(sem->s_mutex);
	if (sem->value > 0) {
		sem->value--;
		unlock(sem->s_mutex);
		return;
	} else {
		unlock(sem->s_mutex);
		_yield();
	}
}

void semPost(sem_t sem) {
	lock(sem->s_mutex);
	sem->value++;
	unlock(sem->s_mutex);
}