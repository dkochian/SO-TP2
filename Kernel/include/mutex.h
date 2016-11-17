#ifndef __MUTEX_H__
    #define __MUTEX_H__

    #include "../../Common/common.h"
    #include "../system/scheduler/include/process.h"
    #include "../system/scheduler/include/scheduler.h"
    #include "../system/scheduler/include/processList.h"
    
    typedef struct mutex {
        int lock;
        list q_list;
    } mutex;

    mutex *initLock();
    void destroyLock(mutex *l);
    void lock(mutex *l);
    void unlock(mutex *l);
    bool isLock(mutex *l);

#endif