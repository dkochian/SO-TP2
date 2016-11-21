#include "include/semaphore.h"
#include "../include/mmu.h"

#include "../scheduler/include/scheduler.h"

#include "../../drivers/include/video.h"

//NUEVA IMPLEMENTACION, NO FUNCIONA. PROBAR PRODCON Y VER QUE DESBLOQUEA UN PROCESO DISTINTO AL QUE BLOQUEA. AGREGA MAL A LA QUEUE?
/*
sem_t semOpen(char * name, int value) {
	sem_t sem = k_malloc(sizeof(sem_t));

	if (sem == NULL)
		return NULL;

	sem->s_mutex = initLock();

	if (sem->s_mutex == NULL) {
		k_free(sem);
		return NULL;
	}

	sem->s_queue = queueBuild(&equal);

	if (sem->s_queue == NULL) {
		k_free(sem);
		return NULL;
	}

	sem->name = name;
	sem->value = value;
	return sem;
}

void semClose(sem_t sem) {
	destroyLock(sem->s_mutex);
	queueDestroy(sem->s_queue);
	k_free(sem);
}

void semWait(sem_t sem) {

	process *p = getCurrentProcess();

	lock(sem->s_mutex);
	if (sem->value > 0) {
		sem->value--;
		unlock(sem->s_mutex);
	} else {
		//if(queueExists(sem->s_queue, p) == false) {
			queuePush(sem->s_queue, p);
						print("bloqueo:", -1);
						printDec(p->id, -1);
						printNewline();
			blockProcess(p->id);
		//}
		unlock(sem->s_mutex);
		_yield();
		//semWait(sem);
	}
	return;
}

void semPost(sem_t sem) {
	lock(sem->s_mutex);
	sem->value++;
	if(queueIsEmpty(sem->s_queue) == false) {
		process *p = queuePop(sem->s_queue);
					print("desbloqueo:", -1);
					printDec(p->id, -1);
					printNewline();
		unBlockProcess(p->id);
	}
	unlock(sem->s_mutex);
	return;
}
*/


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
	while(true) {
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
}

void semPost(sem_t sem) {
	lock(sem->s_mutex);
	sem->value++;
	unlock(sem->s_mutex);
}