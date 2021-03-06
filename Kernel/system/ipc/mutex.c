#include "include/mutex.h"
#include "../include/mmu.h"
#include "../scheduler/include/scheduler.h"

//TODO: remove next, it's just for testing and should never be used
#include "../../drivers/include/video.h"
#include "../scheduler/include/process.h"

extern int _lock(int *lock);
extern void _unlock(int *lock);

struct mutex {
    int lock;
    queue m_queue;
};

mutex lockBuild() {
    mutex l = (mutex) k_malloc(sizeof(mutex));

    if(l == NULL)
        return NULL;

    l->m_queue = queueBuild(&equal);
    
    if(l->m_queue == NULL) {
        k_free(l);
        return NULL;
    }

    l->lock = false;

    return l;
}

void lockDestroy(mutex l) {
    queueDestroy(l->m_queue);
    k_free(l);
}

void lock(mutex l) {
    if(_lock(&l->lock) == 1) {
        process *p = getCurrentProcess();
        queuePush(l->m_queue, p);
        blockProcess(p);

        _yield();
    }
}

void unlock(mutex l) {
    if(queueIsEmpty(l->m_queue) == false) {
        process *p = queuePop(l->m_queue);
        unBlockProcess(p);
        return;
    }

	_unlock(&l->lock);
}

bool isLocked(mutex l) {
    return l->lock;
}