//https://gist.github.com/rtv/4989304

#include <stdio.h>
#include <string.h>
#include <integer.h>
#include "include/cvtest.h"
#include "../../libc/include/syscall.h"
#include "../../libc/include/condVarU.h"


int NUMTHREADS = 2;

/* a global count of the number of threads finished working. It will
   be protected by mutex and changes to it will be signalled to the
   main thread via cond */

int done = 0;
mutex m;
cond_u_t cond;

/* Note: error checking on pthread_X calls ommitted for clarity - you
   should always check the return values in real code. */

/* Note: passing the thread id via a void pointer is cheap and easy,
 * but the code assumes pointers and long ints are the same size
 * (probably 64bits), which is a little hacky. */

/*void processEntry(char * id) {
    //int myid = (long)id; // force the pointer to be a 64bit integer
  
    int workloops = 5;

    for( int i = 0; i < workloops; i++ ) {
        print( "[thread ");
        print(id);
        print("] working ( ");
        printNum(i);
        print(" / ");
        printNum(workloops);
        printn(" )");
        sleep(1); // simulate doing some costly work
    }

    // we're going to manipulate done and use the cond, so we need the mutex
    lock(m);

    // increase the count of threads that have finished their work.
    done++;
    print( "[thread ");
    print(id);
    print("] done is now ");
    printNum(done);
    printn(". Signalling cond.");

    // wait up the main thread (if it is sleeping) to test the value of done  
    cvSignal(cond); 
    unlock(m);
}

int cvTestDep(int argc, char ** argv){
    printColor( "Condition Variable Test Starting" , YELLOW);
    printNewLine();
    m = mutexInit();
    if(m == NULL) {
        printn("Couldn't create the mutex");
        printColor( "Test failed" , RED);
        printNewLine();
        return 0;
    }

    cond = cvInitialize();
    
    if(cond == NULL) {
        printn("Couldn't create the condition variable");
        printColor( "Test failed" , RED);
        printNewLine();
        mutexDestroy(m);
        return 0;
    }

    printColor( "[thread main] starting" , LIGHT_BLUE);
    printNewLine();

    //uint64_t threads[NUMTHREADS];
    char buff[2] = {0};

    for( int t = 0; t < NUMTHREADS; t++ ) {
        char * name = malloc(sizeof(char) * 11);
        strcat(name, "[Thread] ");
        itoa(t,buff);
        strcat(name, buff);
        //threads[t] = newProcess(name, processEntry, 1, buff);
        newProcess(name, processEntry, 1, buff);
        free(name);
    }

    // we're going to test "done" so we need the mutex for safety
    lock(m);

    // are the other threads still busy?
    while( done < NUMTHREADS ) {
        print( "[thread main] done is ");
        printNum(done);
        print(" which is < ");
        printNum(NUMTHREADS);
        printn(" so waiting on condition");

        //block this thread until another thread signals cond. While blocked, the mutex is released, then re-aquired before this thread is woken up and the call returns.
        cvWait(cond, m); 

        printn( "[thread main] wake - cond was signalled." ); 

        //we go around the loop with the lock held
    }

    print( "[thread main] done == ");
    printNum(NUMTHREADS);
    printn(" so everyone is done");

    unlock(m);

    mutexDestroy(m);
    cvDestroy(cond);

    return 1;
}*/