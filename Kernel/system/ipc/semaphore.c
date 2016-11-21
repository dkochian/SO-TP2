#include "../include/mmu.h"
#include "include/semaphore.h"
#include "../scheduler/include/process.h"

#include "../scheduler/include/scheduler.h"
#include "../../drivers/include/video.h"

struct semaphore {
	int value;
	mutex s_mutex;
	queue s_queue;
};

semaphore semBuild(int value) {
	semaphore s = k_malloc(sizeof(struct semaphore));

	if(s == NULL)
		return NULL;

	s->s_mutex = lockBuild();

	if(s->s_mutex == NULL) {
		k_free(s);
		return NULL;
	}

	s->s_queue = queueBuild(&equal);

	if(s->s_queue == NULL) {
		lockDestroy(s->s_mutex);
		k_free(s);
		return NULL;
	}

	s->value = value;

	return s;
}

void semDestroy(semaphore s) {
	lockDestroy(s->s_mutex);
	queueDestroy(s->s_queue);
	k_free(s);
}

void semWait(semaphore s) {
	lock(s->s_mutex);
	if(s->value > 0 && queueIsEmpty(s->s_queue) == true) {
		s->value--;
		unlock(s->s_mutex);
		return;
	}
	process *p = getCurrentProcess();
	queuePush(s->s_queue, p);
	blockProcess(p);
	unlock(s->s_mutex);

	_yield();

	lock(s->s_mutex);
	s->value--;
	unlock(s->s_mutex);
}

void semPost(semaphore s) {
	lock(s->s_mutex);
	s->value++;
	unlock(s->s_mutex);
	if(queueIsEmpty(s->s_queue) == false) {
		process *p = queuePop(s->s_queue);
		unBlockProcess(p);
	}
}