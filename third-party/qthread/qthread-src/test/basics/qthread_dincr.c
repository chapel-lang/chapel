#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "argparsing.h"

double master = 0.0;
double retvals[30];

static aligned_t incr(void *arg)
{
    retvals[(intptr_t)arg] = qthread_dincr(&master, 1.0);
    return 0;
}

static aligned_t incr5(void *arg)
{
    qthread_dincr(&master, 5.0);
    return 0;
}

int main(int argc,
         char *argv[])
{
    int i;
    aligned_t rets[30];
    double ret_test;

    CHECK_VERBOSE();
    qthread_initialize();
    iprintf("%i shepherds\n", qthread_num_shepherds());

    ret_test = qthread_dincr(&master, 1);
    if (master != 1.0) {
        fprintf(stderr, "master = %f\n", master);
    }
    assert(master == 1.0);
    if (ret_test != 0.0) {
        fprintf(stderr, "ret_test = %f\n", ret_test);
    }
    assert(ret_test == 0.0);
    iprintf("basic increment succeeded\n");
    master = 2;
    for (i = 0; i < 30; i++) {
        qthread_fork(incr, (void *)(intptr_t)i, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
        qthread_readFF(NULL, rets + i);
    }
    if (master != 32.0) {
        int j;

        fprintf(stderr, "master is %f rather than 32\n", master);
        for (j = 0; j < 30; j++) {
            fprintf(stderr, "retvals[%i] = %f\n", j, retvals[j]);
        }
    }
    assert(master == 32.0);
    iprintf("30 concurrent threads successfully incremented by 1\n");
    master = 0.0;
    for (i = 0; i < 30; i++) {
        qthread_fork(incr5, NULL, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
        qthread_readFF(NULL, rets + i);
    }
    if (master != 150.0) {
        fprintf(stderr, "master is %f rather than 150\n", master);
    }
    assert(master == 150.0);
    iprintf("30 concurrent threads successfully incremented by 5\n");

    return 0;
}

/* vim:set expandtab */
