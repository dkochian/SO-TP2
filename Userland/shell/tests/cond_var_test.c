#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <syscall.h>
#include "include/cond_var_test.h"

static int done = 0;
static int processEntry1(int argc, char **argv);
static int processEntry2(int argc, char **argv);
static int processEntry3(int argc, char **argv);
static int processEntry4(int argc, char **argv);
static mutex m;
static cond_t cond;

static int processEntry1(int argc, char **argv) {
  
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print( "[process ");
        print((char *)argv);
        print("] working ( ");
        printNum(i);
        print(" / ");
        printNum(workloops);
        printn(" )");
        //sleep(1);  
    }

    lock(m);

    done++;
    print( "[process ");
    print((char *)argv);
    print("] done is now ");
    printNum(done);
    print(". Signalling condition.\n");

    unlock(m);

    return 0;
}
static int processEntry2(int argc, char **argv) {
  
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print( "[process ");
        print((char *)argv);
        print("] working ( ");
        printNum(i);
        print(" / ");
        printNum(workloops);
        printn(" )");
        //sleep(1);  
    }

    lock(m);

    done++;
    print( "[process ");
    print((char *)argv);
    print("] done is now ");
    printNum(done);
    print(". Signalling condition.\n");

    syscvSignal(cond);
    unlock(m);

    return 0;
}
static int processEntry3(int argc, char **argv) {
  
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print( "[process ");
        print((char *)argv);
        print("] working ( ");
        printNum(i);
        print(" / ");
        printNum(workloops);
        printn(" )");
        //sleep(1);  
    }

    lock(m);

    done++;
    print( "[process ");
    print((char *)argv);
    print("] done is now ");
    printNum(done);
    print(". Signalling condition.\n");

    syscvSignal(cond);
    unlock(m);

    return 0;
}
static int processEntry4(int argc, char **argv) {
  
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print( "[process ");
        print((char *)argv);
        print("] working ( ");
        printNum(i);
        print(" / ");
        printNum(workloops);
        printn(" )");
        //sleep(1);  
    }

    lock(m);

    done++;
    print( "[process ");
    print((char *)argv);
    print("] done is now ");
    printNum(done);
    print(". Signalling condition.\n");

    syscvSignal(cond);
    unlock(m);

    return 0;
}


int varaibleConditionTestCommand(int argc, char **argv){
    printColor( "Condition Variable Test Starting\n" , YELLOW);
    m = mutexInit();
    if(m == NULL) {
        printn("Couldn't create the mutex");
        printColor( "Test failed\n" , RED);
        return 1;
    }

    cond = syscvInit();
    
    if(cond == NULL) {
        printn("Couldn't create the condition variable");
        printColor( "Test failed\n" , RED);
        mutexDestroy(m);
        return 1;
    }

    printColor( "[process main] starting\n" , LIGHT_BLUE);
    uint64_t pA = newProcess("1", processEntry1, 1, (char **) "1");
    uint64_t pB = newProcess("2", processEntry2, 1, (char **) "2");
    uint64_t pC = newProcess("3", processEntry3, 1, (char **) "3");
    uint64_t pD = newProcess("4", processEntry4, 1, (char **) "4");

    wPid(pA);
    wPid(pB);
    wPid(pC);
    wPid(pD);

    lock(m);

    while( done < 4 ) {
        print( "[process main] done is ");
        printNum(done);
        print(" which is < 4 so waiting on condition\n");

        syscvWait(cond, m); 

        printColor( "[process main] wake - cond was signalled.\n", LIGHT_GREEN ); 
    }

    printColor( "[process main] done == 4 so everyone is done\n", LIGHT_BLUE);

    unlock(m);

    mutexDestroy(m);
    syscvDestroy(cond);
    done = 0;

    return 1;
}