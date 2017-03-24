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

static aligned_t say_hello(void *arg)
{
    int const rank = qthread_multinode_rank();

    iprintf("Hello from locale %03d!\n", rank);

    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_VERBOSE();

    qthread_f funcs[2] = {say_hello, NULL};
    assert(spr_init(SPR_SPMD, funcs) == SPR_OK);

    int const here = spr_locale_id();
    int const there = (here+1 == spr_num_locales()) ? 0 : here+1;

    iprintf("[%03d] Single Program Multiple Hello!\n", here);

    aligned_t ret;
    qthread_fork_remote(say_hello, NULL, &ret, there, 0);
    qthread_readFF(NULL, &ret);

    spr_fini();

    return 0;
}

