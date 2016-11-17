#include "include/semaphore.h"


sem_t * sem_open(char * name, int value) {
	sem_t * sem = malloc(sizeof(sem_t));
	sem->name = name;
	sem->value = value;
	return sem;
}

void sem_close(sem_t * sem) {
	free(sem);
}

void sem_wait(sem_t * sem) {
	while(true) {
		lock(sem->lock);
		if(sem->value > 0) {
			sem->value--;
			unlock(sem->lock);
			return;
		} else {
			unlock(sem->lock);
			yield();
		}
	}
}

void sem_post(sem_t * sem) {
	lock(sem->lock);
	sem->value++;
	unlock(sem->lock);
}