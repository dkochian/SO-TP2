#ifndef __MUTEX_H__
    #define __MUTEX_H__

    #include "../../../include/common.h"
    #include "../../scheduler/include/process.h"
    #include "../../scheduler/include/scheduler.h"
    #include "../../../utils/include/queue.h"
    
    typedef struct mutex *mutex;

    mutex lockBuild();
    void lockDestroy(mutex l);
    void lock(mutex l);
    void unlock(mutex l);
    bool isLocked(mutex l);//TODO: If it's only used by the test command (userland), then remove it (and the command), no need for a test command

#endif