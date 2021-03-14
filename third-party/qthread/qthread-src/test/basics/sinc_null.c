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

static aligned_t submit_to_sinc(void *arg_)
{
    qt_sinc_submit((qt_sinc_t *)arg_, NULL);

    return 0;
}

static aligned_t wait_on_sinc(void *arg_)
{
    qt_sinc_t *sinc = (qt_sinc_t *)arg_;

    qt_sinc_wait(sinc, NULL);

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
        qt_sinc_submit(arg->sinc, NULL);
    }

    return 0;
}

// //////////////////////////////////////////////////////////////////////////////
int main(int   argc,
         char *argv[])
{
    size_t depth = 3;

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    NUMARG(depth, "TEST_DEPTH");

    // Test creating an empty sinc
    {
        qt_sinc_t zero_sinc;
	qt_sinc_init(&zero_sinc, 0, NULL, NULL, 0);
        qt_sinc_wait(&zero_sinc, NULL);
        qt_sinc_fini(&zero_sinc);

        qt_sinc_t *three_sinc = qt_sinc_create(0, NULL, NULL, 0);
        qt_sinc_expect(three_sinc, 3);
        qthread_fork(submit_to_sinc, three_sinc, NULL);
        qthread_fork(submit_to_sinc, three_sinc, NULL);
        qthread_fork(submit_to_sinc, three_sinc, NULL);
        qt_sinc_wait(three_sinc, NULL);
        qt_sinc_destroy(three_sinc);
    }

    qt_sinc_t *sinc = qt_sinc_create(0, NULL, NULL, 2);

    // Spawn additional waits
    aligned_t rets[3];
    {
        qthread_fork(wait_on_sinc, sinc, &rets[0]);
        qthread_fork(wait_on_sinc, sinc, &rets[1]);
        qthread_fork(wait_on_sinc, sinc, &rets[2]);
    }

    {
        v_args_t args = { depth, sinc };

        // These two spawns covered by qt_sinc_create(...,2)
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
    }

    qt_sinc_wait(sinc, NULL);
    for (int i = 0; i < 3; i++)
        qthread_readFF(NULL, &rets[i]);

    // Reset the sinc
    qt_sinc_reset(sinc, 2);

    // Second use
    {
        v_args_t args = { depth, sinc };

        // These two spawns covered by qt_sinc_reset(...,2)
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
    }

    qt_sinc_wait(sinc, NULL);
    qt_sinc_destroy(sinc);

    return 0;
}

/* vim:set expandtab */
