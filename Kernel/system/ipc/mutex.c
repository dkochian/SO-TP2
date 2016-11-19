#include "include/mutex.h"
#include "../include/mmu.h"
#include "../scheduler/include/scheduler.h"

#include "../../drivers/include/video.h"

extern int _lock(int *lock);
extern void _unlock(int *lock);

struct mutex {
    int lock;
    queue m_queue;
    bool blocked[MAX_PROCESSES];
};

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
    if(p == NULL) //Used only by master of the puppets
        _lock(&l->lock);
    else {
        if(_lock(&l->lock) == 1) {
            if(queueExists(l->m_queue, p) == false) {
                queuePush(l->m_queue, p);
                blockProcess(p->id);
            }

            _yield();
        }
        l->blocked[p->id] = (p->state == BLOCKED);
        p->state = LOCKED;
    }
}

void unlock(mutex l) {
    process *p = getCurrentProcess();
    if(p != NULL) {
        if(l->blocked[p->id] == true)
            p->state = BLOCKED;
        else
            p->state = WAITING;

        if(queueIsEmpty(l->m_queue) == false) {
            p = queuePop(l->m_queue);
            unBlockProcess(p->id);
            return;
        }
    }
    
    _unlock(&l->lock);
}

bool isLock(mutex l) {
    return l->lock;
}