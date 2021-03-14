#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static unsigned int target = 10;
static aligned_t x = 0;

static aligned_t alldone;

static Q_NOINLINE void thread2(size_t left,
                               size_t depth)
{
    size_t foo = qthread_stackleft();

    iprintf("leveli%i: %zu bytes left\n", (int)depth, foo);
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    assert(foo <= left);
#else
    assert(foo < left);
#endif
    /*if (depth < 5) {
     * thread2(t, foo, depth+1);
     * } */
}

static aligned_t thread(void *arg)
{
    int me = qthread_id();

    // printf("thread(%p): me %i\n", (void*) t, me);
    size_t foo = qthread_stackleft();

    iprintf("%zu bytes left\n", foo);
    thread2(foo, 2);

    assert(qthread_lock(&x) == 0);
    // printf("thread(%i): x=%d\n", me, x);
    x++;
    if (x == target) {
        qthread_unlock(&alldone);
    }
    assert(qthread_unlock(&x) == 0);
    return foo + me;                   /* to force them to be used */
}

int main(int argc,
         char *argv[])
{
    long int i;

    assert(qthread_initialize() == 0);

    NUMARG(target, "TEST_TARGET");
    CHECK_VERBOSE();

    qthread_lock(&alldone);

    for (i = 0; i < target; i++) {
        int res = qthread_fork(thread, NULL, NULL);
        if (res != 0) {
            printf("res = %i\n", res);
        }
        assert(res == 0);
    }

    qthread_lock(&alldone);

    iprintf("Final value of x=%lu (expecting %lu)\n", (unsigned long)x, (unsigned long)target);

    qthread_unlock(&alldone);

    if (x == target) {
        return 0;
    } else {
        return -1;
    }
}

/* vim:set expandtab */
