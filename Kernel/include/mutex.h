#ifndef __MUTEX_H__
    #define __MUTEX_H__

    #include "../../Common/common.h"
    #include "../system/scheduler/include/process.h"
    #include "../system/scheduler/include/scheduler.h"
    #include "../system/scheduler/include/processList.h"
    
    typedef struct mutex {
        bool volatile lock;
        list q_list;
    } mutex;

    mutex *initLock();
    void destroyLock(mutex *l);
    void lock(mutex *l, process *p);
    void unlock(mutex *l);

#endif