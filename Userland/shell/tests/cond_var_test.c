//https://gist.github.com/rtv/4989304

#include <strdio.h>
#include <string.h>
#include "include/cond_var_test.h"
#include "../../libc/include/syscall"
#include "../../libc/include/condVarU"

/*
static int done = 0;
static int processEntry(int argc, char **argv);
static mutex m;
static cond_t cond;

static int processEntry(int argc, char **argv) {
  
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
        //sleep(1); // simulate doing some costly work
    }

    lock(m);

    done++;
    print( "[process ");
    print((char *)argv);
    print("] done is now ");
    printNum(done);
    printn(". Signalling cond.");

    cvSignal(cond); 
    unlock(m);

    return 0;
}

int startVariableConditionTest(int argc, char **argv){
    printColor( "Condition Variable Test Starting\n" , YELLOW);
    m = lockBuild();
    if(m == NULL) {
        printn("Couldn't create the mutex");
        printColor( "Test failed\n" , RED);
        return 1;
    }

    cond = cvInitialize();
    
    if(cond == NULL) {
        printn("Couldn't create the condition variable");
        printColor( "Test failed\n" , RED);
        lockDestroy(m);
        return 1;
    }
    char name[MAXTESTP][11];
    char buff[MAXTESTP][2];
    printColor( "[process main] starting\n" , LIGHT_BLUE);


    for( int t = 0; t < MAXTESTP; t++ ) {
        k_strcat(name[MAXTESTP], "[process ");
        k_itoa(t,buff[MAXTESTP]);
        k_strcat(name[MAXTESTP], buff[MAXTESTP]);
        k_strcat(name[MAXTESTP], "]");

        newProcess(name[MAXTESTP], processEntry, 1, (char **) buff[MAXTESTP]);

    }

    lock(m);

    while( done < MAXTESTP ) {
        print( "[process main] done is ");
        printNum(done);
        print(" which is < ");
        printNum(MAXTESTP);
        print(" so waiting on condition\n");

        cvWait(cond, m); 

        printColor( "[process main] wake - cond was signalled.\n", LIGHT_GREEN ); 
    }

    print( "[process main] done == ", LIGHT_BLUE);
    printNum(MAXTESTP);
    printColor(" so everyone is done\n", LIGHT_BLUE);

    unlock(m);

    lockDestroy(m);
    cvDestroy(cond);
    done = 0;
    //k_free(buff);
    //k_free(name);
    return 1;
}*/