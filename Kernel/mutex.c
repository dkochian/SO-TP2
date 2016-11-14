#include "include/mutex.h"

void lock(bool volatile *lock) {
  while(__sync_lock_test_and_set(lock, LOCKED_MUTEX)) {
    //switch_task(1); //yield??
  }
}

void unlock(bool volatile *lock) {
  __sync_lock_release(lock);
}

// Use only within interrupt, guaranteing uninterrupted execution.
bool isLockOpenRightThisInstant(bool volatile *lock) {
  if(__sync_lock_test_and_set(lock, LOCKED_MUTEX)) {
    // sync returned true, therefore wasn't grabbed
    return false;
  } else {
    // sync returned false, therefore was grabbed
    __sync_lock_release(lock);
    return true;
  }
}