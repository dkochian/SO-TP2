#include "include/mutex.h"
#include "system/scheduler/include/scheduler.h"
#include "system/include/mmu.h"

#include "drivers/include/video.h"

extern int _lock(int *lock);
extern void _unlock(int *lock);

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

void lock(mutex *l) {
    while(_lock(&l->lock) == 1) {
        process *p = getCurrentProcess();
        if(exists(l->q_list, p) == false) {
            add(l->q_list, p);
            blockProcess(p->id);
        }

        _yield();
    }
}

void unlock(mutex *l) {//review
    process *p;
    _unlock(&l->lock);
    p = getFirst(l->q_list);
    unBlockProcess(p->id);
}