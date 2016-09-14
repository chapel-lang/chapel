#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>

#include "argparsing.h"

typedef struct {
    aligned_t n;
    aligned_t result;
} f_arg_t;

typedef struct {
    f_arg_t    fargs[2];
    aligned_t *target;
} fr_arg_t;

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

static aligned_t fib_result(void *arg)
{
    aligned_t *target = ((fr_arg_t *)arg)->target;
    aligned_t  r0     = ((fr_arg_t *)arg)->fargs[0].result;

    r0 += ((fr_arg_t *)arg)->fargs[1].result;

    if (validation[((fr_arg_t *)arg)->fargs[0].n] != ((fr_arg_t *)arg)->fargs[0].result) {
	fprintf(stdout, "%u:%u(%p) should be %u (0)\n", (unsigned)((fr_arg_t *)arg)->fargs[0].n, (unsigned)((fr_arg_t *)arg)->fargs[0].result, &((fr_arg_t *)arg)->fargs[0].result, (unsigned)validation[((fr_arg_t *)arg)->fargs[0].n]);
	fflush(stdout);
	assert(validation[((fr_arg_t *)arg)->fargs[0].n] == ((fr_arg_t *)arg)->fargs[0].result);
    }
    if (validation[((fr_arg_t *)arg)->fargs[1].n] != ((fr_arg_t *)arg)->fargs[1].result) {
	fprintf(stdout, "%u:%u(%p) should be %u (1)\n", (unsigned)((fr_arg_t *)arg)->fargs[1].n, (unsigned)((fr_arg_t *)arg)->fargs[1].result, &((fr_arg_t *)arg)->fargs[1].result, (unsigned)validation[((fr_arg_t *)arg)->fargs[1].n]);
	fflush(stdout);
	assert(validation[((fr_arg_t *)arg)->fargs[1].n] == ((fr_arg_t *)arg)->fargs[1].result);
    }

    qthread_writeEF(target, &r0);
    free(arg);

    return 0;
}

static aligned_t fib_(void *arg)
{
    aligned_t  n      = ((f_arg_t *)arg)->n;
    aligned_t *result = &((f_arg_t *)arg)->result;

    if (n < 2) {
        qthread_writeEF_const(result, n);
        return 0;
    }

    fr_arg_t *fibs = malloc(sizeof(fr_arg_t));
    f_arg_t  *f1   = &fibs->fargs[0];
    f_arg_t  *f2   = &fibs->fargs[1];

    f1->n = n - 1;
    f2->n = n - 2;
    qthread_empty(&f1->result);
    qthread_empty(&f2->result);

    fibs->target = result;

    // Collect results of sub-actions
    qthread_fork_precond(fib_result, fibs, NULL, 2, &f1->result, &f2->result);

    // Fork off recursive actions
    qthread_fork(fib_, f1, NULL);
    qthread_fork(fib_, f2, NULL);

    return 0;
}

inline static aligned_t fib(aligned_t value)
{
    f_arg_t args = { value, 0 };

    qthread_empty(&args.result);

    qthread_fork(fib_, &args, NULL);
    qthread_readFF(&args.result, &args.result);

    return args.result;
}

int main(int   argc,
         char *argv[])
{
    aligned_t n = 20;
    aligned_t r = 0;

    assert(qthread_initialize() == 0);
    CHECK_VERBOSE();
    NUMARG(n, "FIB_INPUT");

    r = fib(n);
    iprintf("fib(%3lu) =            %lu\n", (unsigned long)n, (unsigned long)r);
    if (n < (sizeof(validation) / sizeof(aligned_t))) {
        iprintf("known correct answer: %lu\n", validation[n]);
        assert(r == validation[n]);
    }

    return 0;
}

/* vim:set expandtab */
