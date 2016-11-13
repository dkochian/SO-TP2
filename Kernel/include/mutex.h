#ifndef __MUTEX_H__
  #define __MUTEX_H__

  #include "../../Common/common.h"

  #define INIT_MUTEX 0x0
  #define LOCKED_MUTEX 0x01

  
  #define CAS_LOCK(sl)     __sync_lock_test_and_set(sl, 1)
  #define CLEAR_LOCK(sl) __sync_lock_release(sl)
  /*ESTO PUEDE SERVIR PARA USERLAND*/
  #define TRY_LOCK(sl)        !CAS_LOCK(sl)
  #define RELEASE_LOCK(sl)    CLEAR_LOCK(sl)
  #define ACQUIRE_LOCK(sl)    (CAS_LOCK(sl)? spin_acquire_lock(sl) : 0)
  #define INITIAL_LOCK(sl)    (*sl = 0)

  #define LOCK(lock) while (__sync_lock_test_and_set(&lock, 0x01)) { syscall_yield(); };
  #define UNLOCK(lock) __sync_lock_release(&lock);
  /**/

  #define SPINS_PER_YIELD 63
  #define SPIN_LOCK_YIELD


   /**
    * Acquires the lock.
    */
  void lock(bool volatile * lock);

   /**
    * Releases the lock.
    */
  void unlock(bool volatile * lock);

   /**
    * Check the lock. Use only within interrupt, guaranteing uninterrupted execution.
    */
  bool isLockOpenRightThisInstant(bool volatile * lock);

#endif