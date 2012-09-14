#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/sinc.h>
#include "argparsing.h"

typedef uint32_t my_value_t;

static void my_incr(void *tgt, const void *src) {
    *(my_value_t *)tgt += *(my_value_t *)src;
}

static aligned_t wait_on_sinc(void *arg_)
{
    qt_sinc_t *sinc = (qt_sinc_t *)arg_;

    my_value_t x;
    qt_sinc_wait(sinc, &x);

    return 0;
}

typedef struct v_args_s {
    size_t     depth;
    qt_sinc_t *sinc;
} v_args_t;

static aligned_t visit(void *arg_)
{
    v_args_t *arg = (v_args_t *)arg_;

    if (arg->depth > 2) {
        /* I'm an internal node. */
        v_args_t args = { arg->depth - 1, arg->sinc };

        qt_sinc_expect(arg->sinc, 2);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);

        qt_sinc_submit(arg->sinc, NULL);
    } else if (arg->depth == 2) {
        /* I'm going to spawn leaf nodes. */
        v_args_t args = { arg->depth - 1, arg->sinc };

        qt_sinc_expect(arg->sinc, 2);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);

        qt_sinc_submit(arg->sinc, NULL);
    } else {
        /* I'm a leaf node. */
        my_value_t value = 1;
        qt_sinc_submit(arg->sinc, &value);
    }

    return 0;
}

// //////////////////////////////////////////////////////////////////////////////
int main(int   argc,
         char *argv[])
{
    size_t total = 0;
    size_t depth = 3;

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    NUMARG(depth, "TEST_DEPTH");

    my_value_t initial_value = 0;
    qt_sinc_t sinc;
    qt_sinc_init(&sinc, sizeof(my_value_t), &initial_value, my_incr, 2);

    // Spawn additional waits
    iprintf("Spawning multiple waiters...\n");
    aligned_t rets[3];
    {
        qthread_fork(wait_on_sinc, &sinc, &rets[0]);
        qthread_fork(wait_on_sinc, &sinc, &rets[1]);
        qthread_fork(wait_on_sinc, &sinc, &rets[2]);
    }

    iprintf("Spawning two threads to fill the sinc...\n");
    {
        v_args_t args = { depth, &sinc };

        // These two spawns covered by qt_sinc_create(...,2)
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
    }

    iprintf("Waiting for the sinc...\n");
    my_value_t x = 0;
    qt_sinc_wait(&sinc, &x);
    for (int i = 0; i < 3; i++)
        qthread_readFF(NULL, &rets[i]);

    total += x;

    // Reset the sinc
    qt_sinc_reset(&sinc, 2);

    // Second use
    {
        v_args_t args = { depth, &sinc };

        // These two spawns covered by qt_sinc_reset(...,2)
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
    }

    x = 0;
    qt_sinc_wait(&sinc, &x);
    total += x;

    qt_sinc_fini(&sinc);

    if (total == (1UL << (depth+1))) {
        iprintf("SUCCEEDED with total = 2*(2^%lu) = %lu\n", 
            (unsigned long)depth,
            (unsigned long)total);
        return 0;
    } else {
        iprintf("FAILED with total = %lu\n", (unsigned long)total);
        return 1;
    }
}

/* vim:set expandtab */
