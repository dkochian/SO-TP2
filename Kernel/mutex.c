#include "include/mutex.h"
#include "system/scheduler/include/scheduler.h"
#include "system/include/mmu.h"

#include "drivers/include/video.h"

extern void _lock(bool volatile *lock);
extern void _unlock(bool volatile *lock);

mutex *initLock() {
    mutex *l = (mutex *) k_malloc(sizeof(mutex));

    if(l == NULL)
        return NULL;

    l->q_list = buildList(&equal);
    
    if(l->q_list == NULL) {
        k_free(l);
        return NULL;
    }

    l->lock = false;

    if(l->q_list == NULL) {
        k_free(l);
        return NULL;
    }

    return l;
}

void destroyLock(mutex *l) {
    destroyList(l->q_list);
    k_free(l);
}

void lock(mutex *l, process *p) {
    add(l->q_list, p);
    blockProcess(p->id);
    _lock(&l->lock);
}

void unlock(mutex *l) {
    process *p;
    _unlock(&l->lock);
    p = getFirst(l->q_list);
    unBlockProcess(p->id);
}