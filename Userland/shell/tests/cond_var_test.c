#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <syscall.h>
#include "include/cond_var_test.h"

static int processEntry(int argc, char **argv);

static volatile mutex m;
static volatile cond_t cond;
static volatile int done = 0;

static void mysleep(long int num) {
    int i = num * 10000000;
    while(i-- > 0);
}

static int processEntry(int argc, char **argv) {   
    //printNum(done);
    //print("&");
    char* id = (char *)argv; 
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print(id);
        printn(" is working....");
        mysleep(1);  
    }

    print((char *)argv);
    print(" checking condition.\n");

    lock(m);
    print("in lock");
    done++;
    printNum(done);
    if(done>=4)
        syscvSignal(cond);
    unlock(m);


    while(true) {}

    return 0;
}

int varaibleConditionTestCommand(int argc, char **argv){
    done =0;
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
    uint64_t pA = newProcess("1", processEntry, 1, (char **) "1");
    uint64_t pB = newProcess("2", processEntry, 1, (char **) "2");
    uint64_t pC = newProcess("3", processEntry, 1, (char **) "3");
    uint64_t pD = newProcess("4", processEntry, 1, (char **) "4");

    lock(m);

    syscvWait(cond, m);     
    printColor( "[process main] done == 4 so everyone is done\n", LIGHT_BLUE);

    unlock(m);

    kill(pA);
    kill(pB);
    kill(pC);
    kill(pD);
    mutexDestroy(m);
    syscvDestroy(cond);
    done = 0;

    return 0;
}