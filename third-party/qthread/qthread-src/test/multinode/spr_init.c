#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qthread/qthread.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

int my_id, world_size;

static aligned_t returner(void *arg)
{
    iprintf("(%03d) returner called, returning %ld\n",
            my_id, *((long *)arg));
    return *((long *)arg);
}

int main(int   argc,
         char *argv[])
{
    aligned_t tmp, ret = 0;
    int       retval;
    long      foobar   = 1234567890;
    qthread_f funcs[2] = { returner, NULL };

    CHECK_VERBOSE();

    retval = spr_init(SPR_SPMD, funcs);
    if (retval != SPR_OK) {
        fprintf(stderr, "(%03d) spr_init returned %d\n", 0, retval);
        return -1;
    }

    my_id      = spr_locale_id();
    world_size = spr_num_locales();

    iprintf("(%03d) Rank %d of %d is alive\n", my_id, my_id, world_size);

    spr_unify();

    if (my_id != 0) {
        return -2;
    }

    int target = (world_size > 1) ? 1 : 0;
    retval = qthread_fork_remote(returner, &foobar, &ret, target, sizeof(long));
    if (retval != 0) {
        fprintf(stderr, "(%03d) fork_remote returned %d\n", my_id, retval);
        return -3;
    }

    retval = qthread_readFE(&tmp, &ret);
    iprintf("(%03d) returner returned %ld\n", my_id, (long)tmp);
    if (retval != 0) {
        fprintf(stderr, "(%03d) readFE returned %d (%d)\n", my_id, retval, (int)tmp);
        return -4;
    }

    spr_fini();

    return (tmp == foobar) ? 0 : 5;
}

/* vim:set expandtab */
