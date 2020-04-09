#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t checkres(void *arg)
{
    qthread_shepherd_id_t myshep = qthread_shep();

    assert(myshep == 1 || myshep == 0);

    iprintf("myshep = %u\n", (unsigned)myshep);
    iprintf("arg = %u\n", (unsigned)(uintptr_t)arg);
    assert(myshep == (qthread_shepherd_id_t)(intptr_t)arg);

    return 0;
}

static aligned_t migrant(void *arg)
{
    int myshep = qthread_shep();

    assert(myshep == 1 || myshep == 0);

    iprintf("migrant running on shep %i\n", myshep);
    if (myshep == 1) {
        qthread_migrate_to(0);
	iprintf("migrant now running on shep %i\n", (int)qthread_shep());
        assert(qthread_shep() == 0);
    } else {
        qthread_migrate_to(1);
	if (qthread_shep() != 1) {
	    fprintf(stderr, "Expected to be on shepherd 1, actually on shepherd %i\n", qthread_shep());
	    assert(qthread_shep() == 1);
	    abort();
	}
    }

    return 0;
}

#ifdef __INTEL_COMPILER
int setenv(const char *name,
           const char *value,
           int overwrite);
#endif

int main(int argc,
         char *argv[])
{
    aligned_t ret;

    qthread_init(2);

    CHECK_VERBOSE();

    assert(qthread_num_shepherds() == 2);
    iprintf("now to fork to shepherd 0...\n");
    qthread_fork_to(checkres, (void *)0, &ret, 0);
    qthread_readFF(&ret, &ret);
    iprintf("success in forking to shepherd 0!\n");
    iprintf("now to fork to shepherd 1...\n");
    qthread_fork_to(checkres, (void *)1, &ret, 1);
    qthread_readFF(&ret, &ret);
    iprintf("success in forking to shepherd 1!\n");
    iprintf("now to fork the migrant...\n");
    qthread_fork(migrant, NULL, &ret);
    iprintf("success in forking migrant!\n");
    qthread_readFF(&ret, &ret);
    iprintf("migrant returned successfully!\n");

    return 0;
}

/* vim:set expandtab */
