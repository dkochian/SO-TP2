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
    if(_lock(&l->lock) == 1) {
        process *p = getCurrentProcess();
        if(exists(l->q_list, p) == false) {
            add(l->q_list, p);
            print("lock list:", -1);
            printNewline();
            printList(l->q_list);
            blockProcess(p->id);
        }

        _yield();
    }
}

void unlock(mutex* l) {
    print("unlock called", -1);
    printNewline();
    printSize(l->q_list);
    if(isEmpty(l->q_list) == false) {
        print("unlock list:", -1);
        printNewline();
        printList(l->q_list);
        process* p = getFirst(l->q_list);
        unBlockProcess(p->id);
    } else {
        print("shit", -1);
        printNewline();
        _unlock(&l->lock);
    }
}