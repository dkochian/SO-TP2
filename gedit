#include "include/mutex.h"
#include "../include/mmu.h"
#include "../scheduler/include/scheduler.h"

#include "../../drivers/include/video.h"
#include "../scheduler/include/process.h"
extern mutex v_mutex;
//extern mutex m;

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
    process *p = getCurrentProcess();
    /*if(l == m) {
        print("[lock] ", 1);
        print(p->name, 1);
        printNewLine();
    }*/

    if(_lock(&l->lock) == 1) {
        queuePush(l->m_queue, p);
        blockProcess(p);

        _yield();
    }
}

void unlock(mutex l) {
    process *p = getCurrentProcess();
    /*if(l == m) {
        print("[unlock] ", 3);
        print(p->name, 3);
        printNewLine();
    }*/

    if(queueIsEmpty(l->m_queue) == false) {
        p = queuePop(l->m_queue);
        unBlockProcess(p);
        return;
    }

	_unlock(&l->lock);
}

bool isLocked(mutex l) {
    return l->lock;
}