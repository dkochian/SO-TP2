#ifndef __MUTEX_H__
  #define __MUTEX_H__

  #include "../Common/common.h"
  
  /**
  * Initializes Mutex.
  */
  void initMutex();

   /**
    * Acquires the lock.
    */
   void lock();

   /**
    * Releases the lock.
    */
   void unlock();

#endif