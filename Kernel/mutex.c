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

    l->q_list = buildListS(&equal);
    
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
    destroyListS(l->q_list);
    k_free(l);
}

bool isLock(mutex *l){
    return l->lock;
}

void lock(mutex *l) {
    if(_lock(&l->lock) == 1) {
        process *p = getCurrentProcess();
        if(existsS(l->q_list, p) == false) {
            addS(l->q_list, p);
            blockProcess(p->id);
        }
        //print("in", -1);
        printNewline();
        printListS(l->q_list);
        printNewline();
        _yield();
        print("out", -1);
    }
}

void unlock(mutex* l) {
    if(isEmptyS(l->q_list) == false) {
        process* p = getFirstS(l->q_list);
        unBlockProcess(p->id);
        return;
    }
    
    _unlock(&l->lock);
}