#include "mutex.h"
/*
typedef struct {
    bool 
        m_locked = FALSE;
}lockStruct;*/

//static lockStruct *lock = k_malloc(sizeof(lockStruct));

/**
 * Locks the lock.
 */
void lock() {
  //  while (_testAndSetLock(&lock->held));
}
/**
 * Unlocks the lock. 
 */
void unlock() {
    //lock->m_locked = FALSE;
}
/*
static bool compareAndSet(bool expectedValue, bool newValue){
    if(m_locked == expectedValue) {
        m_locked = newValue;
    }
    return m_locked;
}
*/