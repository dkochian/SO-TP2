#include "mutex.h"


void lock(bool volatile * lock) {
    while(__sync_lock_test_and_set(lock, 0x01)) {
        //switch_task(1);
    }
}

void unlock(bool volatile * lock) {
    __sync_lock_release(lock);
}

static int spin_acquire_lock(int *sl) {
  int spins = 0;
  while (*(volatile int *)sl != 0 || CAS_LOCK(sl)) {
    if ((++spins & SPINS_PER_YIELD) == 0) {
      SPIN_LOCK_YIELD;
    }
  }
  return 0;
}


/*
typedef struct LockStruct{
    bool 
        m_locked;
}LockStruct;

static LockStruct *mutexVar = NULL;

void initMutex(){
    mutexVar = k_malloc(sizeof(LockStruct));
    mutexVar->m_locked = FALSE;
}*/

/**
 * Locks the lock.
 */
/*void lock() {
  while (_testAndSetLock(&lock->held));
}*/
/**
 * Unlocks the lock. 
 */
/*void unlock() {
    mutexVar->m_locked = FALSE;
}*/
/*
static bool compareAndSet(bool expectedValue, bool newValue){
    if(m_locked == expectedValue) {
        m_locked = newValue;
    }
    return m_locked;
}
*/
/*
https://github.com/stevej/osdev
*/