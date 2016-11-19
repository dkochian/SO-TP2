#include "include/mutex.h"
#include "../scheduler/include/scheduler.h"
#include "../include/mmu.h"

#include "../../drivers/include/video.h"

extern int _lock(int *lock);
extern void _unlock(int *lock);

mutex initLock() {
    mutex l = (mutex) k_malloc(sizeof(mutex));

    if(l == NULL)
        return NULL;

    l->m_queue = queueBuild(&equal);
    
    if(l->m_queue == NULL) {
        k_free(l);
        return NULL;
    }

    l->lock = false;

    if(l->m_queue == NULL) {
        k_free(l);
        return NULL;
    }

    return l;
}

void destroyLock(mutex l) {
    queueDestroy(l->m_queue);
    k_free(l);
}

void lock(mutex l) {
    process *p = getCurrentProcess();
    if(_lock(&l->lock) == 1) {
        if(queueExists(l->m_queue, p) == false) {
            queuePush(l->m_queue, p);
            blockProcess(p->id);
        }

        _yield();
    }
    p->state = LOCKED;
}

void unlock(mutex l) {
    process *p = getCurrentProcess();
    p->state = RUNNING;

    if(queueIsEmpty(l->m_queue) == false) {
        p = queuePop(l->m_queue);
        unBlockProcess(p->id);
        return;
    }
    
    _unlock(&l->lock);
}

bool isLock(mutex l) {
    return l->lock;
}