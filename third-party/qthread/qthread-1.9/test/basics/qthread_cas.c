#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "argparsing.h"

aligned_t master = 0;

static aligned_t incr(void *arg)
{
    aligned_t localmaster, addition, ret;

    ret = master;
    do {
        localmaster = ret;
        addition    = localmaster + 1;
        ret         = qthread_cas(&master, ret, addition);
    } while (ret != localmaster);
    return 0;
}

uint32_t four  = 4;
uint64_t eight = 8;
void    *ptr   = NULL;

int main(int   argc,
         char *argv[])
{
    int       i;
    aligned_t rets[30];

    memset(rets, 0, sizeof(aligned_t) * 30);
    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();

    rets[0] = qthread_cas(&master, 0, 1);
    assert(master == 1);
    assert(rets[0] == 0);
    master = 0;
    for (i = 0; i < 30; i++) {
        qthread_fork(incr, NULL, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
        qthread_readFF(NULL, rets + i);
    }
    if (master != 30) {
        fprintf(stderr, "master is %lu rather than 30\n",
                (long unsigned)master);
    }

    assert(qthread_cas32(&four, 4, 0xdeadbeef) == 4);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(qthread_cas64(&eight, (uint64_t)8, (uint64_t)0xdeadbeefc0d1f1edULL) == 8);
#endif
    assert(four == 0xdeadbeef);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(eight == (uint64_t)0xdeadbeefc0d1f1edULL);
#endif
    assert(qthread_cas32(&four, 0xdeadbeef, 5) == 0xdeadbeef);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(qthread_cas64(&eight, (uint64_t)0xdeadbeefc0d1f1edULL, (uint64_t)9) == (uint64_t)0xdeadbeefc0d1f1edULL);
#endif
    assert(four == 5);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(eight == 9);
#endif

    assert(qthread_cas_ptr(&ptr, NULL, &i) == NULL);
    assert(ptr == &i);
    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */
