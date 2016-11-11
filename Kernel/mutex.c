#include "mutex.h"


void lock(bool volatile * lock) {
	while(__sync_lock_test_and_set(lock, 0x01)) {
		//switch_task(1);	//yield??
	}
}

void unlock(bool volatile * lock) {
	__sync_lock_release(lock);
}

// Use only within interrupt, guaranteing uninterrupted execution.
bool isLockOpenRightThisInstant(bool volatile * lock) {
	if(__sync_lock_test_and_set(lock, 0x01)) {
		// sync returned true, therefore wasn't grabbed
		return FALSE;
	} else {
		// sync returned false, therefore was grabbed
		__sync_lock_release(lock);
		return TRUE;
	}
}

/*static int spin_acquire_lock(int *sl) {
  int spins = 0;
  while (*(volatile int *)sl != 0 || CAS_LOCK(sl)) {
    if ((++spins & SPINS_PER_YIELD) == 0) {
      SPIN_LOCK_YIELD;
    }
  }
  return 0;
}*/


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