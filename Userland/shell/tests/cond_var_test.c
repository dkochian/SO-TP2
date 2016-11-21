#include <stdio.h>
#include <string.h>
#include <integer.h>
#include <syscall.h>
#include "include/cond_var_test.h"

static int processEntry(int argc, char **argv);

static volatile mutex m;
static volatile cond_t cond;
static volatile int done = 0;

static int processEntry(int argc, char **argv) {   
    char* id = (char *)argv; 
    int workloops = 5;
    int i;
    for( i = 0; i < workloops; i++ ) {
        print(id);
        printn(" is working....");
        //sleep(1);  
    }

    print((char *)argv);
    print(" checking condition.\n");

    done++;
    if(done >= MAX_PROCESS)
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
    uint64_t process[MAX_PROCESS] = {0};

    printColor( "[process main] starting\n" , LIGHT_BLUE);
    int i = 0;
    char arg[MAX_PROCESS][MAX_PROCESS];
    for(i = 0; i < MAX_PROCESS; i++){
        itoa(i+1, arg[i]);
        process[i] = newProcess(arg[i], processEntry, 1, (char **) arg[i]);
    }

    lock(m);

    syscvWait(cond, m);     
    printColor( "[process main] done == ", LIGHT_BLUE);
    printNum(MAX_PROCESS);
    printColor(" so everyone is done\n", LIGHT_BLUE);

    unlock(m);

    for (i = 0; i < MAX_PROCESS; i++) {
        kill(process[i]);
    }

    mutexDestroy(m);
    syscvDestroy(cond);

    return 0;
}