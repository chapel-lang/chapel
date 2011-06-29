#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

void qt_blocking_subsystem_begin_blocking_action(void);
void qt_blocking_subsystem_end_blocking_action(void);

static int foo = 0;

static aligned_t user_func(void *arg)
{
    iprintf("in user_func function\n");
    qt_blocking_subsystem_begin_blocking_action();
    foo = 1;
    iprintf("Mwa. Hah. Hah.\n");
    qt_blocking_subsystem_end_blocking_action();
    iprintf("exiting user_func function\n");

    return 0;
}

int main(int argc,
         char *argv[])
{
    aligned_t t;

    user_func(NULL);
    assert(foo == 1);

    foo = 0;

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();

    qthread_fork(user_func, NULL, &t);
    qthread_readFF(NULL, &t);
    assert(foo == 1);

    return (foo == 1)?0:EXIT_FAILURE;
}

/* vim:set expandtab */
