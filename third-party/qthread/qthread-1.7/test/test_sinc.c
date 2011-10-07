#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/qt_sinc.h>
#include "argparsing.h"

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

        qt_sinc_willspawn(arg->sinc, 2);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);

        qt_sinc_submit(arg->sinc, NULL);
    } else if (arg->depth == 2) {
        /* I'm going to spawn leaf nodes. */
        v_args_t args = { arg->depth - 1, arg->sinc };

        qt_sinc_willspawn(arg->sinc, 2);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);

        qt_sinc_submit(arg->sinc, NULL);
    } else {
        /* I'm a leaf node. */
        aligned_t value = 1;
        qt_sinc_submit(arg->sinc, &value);
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

    qt_sinc_t *sinc = qt_sinc_create();

    {
        v_args_t args = { depth, sinc };

        qt_sinc_willspawn(sinc, 2);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
        qthread_fork_syncvar_copyargs(visit, &args, sizeof(v_args_t), NULL);
    }

    aligned_t x = 0;
    qt_sinc_wait(sinc, &x);

    if (x == 1 << depth) {
        return 0;
    } else {
        return 1;
    }
}

/* vim:set expandtab */
