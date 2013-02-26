#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

static aligned_t done;

static aligned_t ping(void *arg)
{
    int const size = spr_num_locales();
    int const rank = spr_locale_id();

    iprintf("Ping %03d\n", rank);

    if (rank == 0) {
        qthread_writeF_const(&done, 1);
    } else {
        int const next = (rank + 1) % size;
        qthread_fork_remote(ping, NULL, NULL, next, 0);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_VERBOSE();

    qthread_f funcs[2] = {ping, NULL};
    assert(spr_init(0, funcs) == SPR_OK);

    int const size = spr_num_locales();
    int const rank = spr_locale_id();

    int const next = (rank + 1) % size;

    qthread_empty(&done);
    qthread_fork_remote(ping, NULL, NULL, next, 0);
    qthread_readFF(NULL, &done);

    return 0;
}

