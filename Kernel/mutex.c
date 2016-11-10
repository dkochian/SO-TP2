#include "mutex.h"
#define CAS_LOCK(sl)     __sync_lock_test_and_set(sl, 1)
#define CLEAR_LOCK(sl) __sync_lock_release(sl)

#define TRY_LOCK(sl)        !CAS_LOCK(sl)
#define RELEASE_LOCK(sl)    CLEAR_LOCK(sl)
#define ACQUIRE_LOCK(sl)    (CAS_LOCK(sl)? spin_acquire_lock(sl) : 0)
#define INITIAL_LOCK(sl)    (*sl = 0)


#define SPINS_PER_YIELD 63
#define SPIN_LOCK_YIELD

#define SPINS_PER_YIELD 63
#define SPIN_LOCK_YIELD



void spin_lock(uint8_t volatile * lock) {
    while(__sync_lock_test_and_set(lock, 0x01)) {
        switch_task(1);
    }
}

void spin_unlock(uint8_t volatile * lock) {
    __sync_lock_release(lock);
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


/* Plain spin locks use single word (embedded in malloc_states) */

static int spin_acquire_lock(int *sl) {
  int spins = 0;
  while (*(volatile int *)sl != 0 || CAS_LOCK(sl)) {
    if ((++spins & SPINS_PER_YIELD) == 0) {
      SPIN_LOCK_YIELD;
    }
  }
  return 0;
}






/*USERLAND
#define LOCK(lock) while (__sync_lock_test_and_set(&lock, 0x01)) { syscall_yield(); };
#define UNLOCK(lock) __sync_lock_release(&lock);*/

