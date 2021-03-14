#include <stdio.h>                     /* for printf() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

static aligned_t validation[] = {
    0,        // 0
    1,        // 1
    1,        // 2
    2,        // 3
    3,        // 4
    5,        // 5
    8,        // 6
    13,       // 7
    21,       // 8
    34,       // 9
    55,       // 10
    89,       // 11
    144,      // 12
    233,      // 13
    377,      // 14
    610,      // 15
    987,      // 16
    1597,     // 17
    2584,     // 18
    4181,     // 19
    6765,     // 20
    10946,    // 21
    17711,    // 22
    28657,    // 23
    46368,    // 24
    75025,    // 25
    121393,   // 26
    196418,   // 27
    317811,   // 28
    514229,   // 29
    832040,   // 30
    1346269,  // 31
    2178309,  // 32
    3524578,  // 33
    5702887,  // 34
    9227465,  // 35
    14930352, // 36
    24157817, // 37
    39088169  // 38
};

static aligned_t fib(void *arg_)
{
    unsigned int n = *(unsigned int*)arg_;

    if (n < 2) {
	return n;
    }

    aligned_t ret1 = 0;
    aligned_t ret2 = 0;
    unsigned int n1 = n - 1;
    unsigned int n2 = n - 2;

    qthread_fork(fib, &n1, &ret1);
    qthread_fork(fib, &n2, &ret2);

    qthread_readFF(NULL, &ret1);
    qthread_readFF(NULL, &ret2);

    return ret1 + ret2;
}

int main(int   argc,
         char *argv[])
{
    qtimer_t  timer = qtimer_create();
    aligned_t n     = 20;
    aligned_t ret   = 0;

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(n, "FIB_INPUT");

    qtimer_start(timer);
    qthread_fork(fib, &n, &ret);
    qthread_readFF(NULL, &ret);
    qtimer_stop(timer);

    if (validation[n] == ret) {
        LOG_FIB_YAML(n, ret, qtimer_secs(timer))
        LOG_ENV_QTHREADS_YAML()
    } else {
        iprintf("Fail %lu (== %lu) in %f sec\n", (unsigned long)ret, (unsigned long)validation[n], qtimer_secs(timer));
    }

    qtimer_destroy(timer);

    return 0;
}

/* vim:set expandtab */
