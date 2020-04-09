#include <stdio.h>                     /* for printf() */
#include <assert.h>                    /* for assert() */
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

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
    aligned_t *n = (aligned_t *)arg_;

    if (*n < 2) {
        return *n;
    }

    aligned_t ret1, ret2;
    aligned_t n1 = *n - 1;
    aligned_t n2 = *n - 2;

    ret1 = _Cilk_spawn fib(&n1);
    ret2 = _Cilk_spawn fib(&n2);

    _Cilk_sync;

    return ret1 + ret2;
}

int main(int   argc,
         char *argv[])
{
    qtimer_t  timer = qtimer_create();
    aligned_t n     = 20;
    aligned_t ret   = 0;

    /* setup */
    CHECK_VERBOSE();
    NUMARG(n, "FIB_INPUT");

    qtimer_start(timer);
    ret = _Cilk_spawn fib(&n);
    _Cilk_sync;
    qtimer_stop(timer);

    if (validation[n] == ret) {
        fprintf(stdout, "%d %lu %lu %f\n", __cilkrts_get_nworkers(), (unsigned long)n, (unsigned long)ret, qtimer_secs(timer));
    } else {
        iprintf("Fail %lu (== %lu) in %f sec\n", (unsigned long)ret, (unsigned long)validation[n], qtimer_secs(timer));
    }

    qtimer_destroy(timer);

    return 0;
}

/* vim:set expandtab */
