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
        addition = localmaster + 1;
        ret = qthread_cas(&master, ret, addition);
    } while (ret != localmaster);
    return 0;
}

volatile uint32_t four = 4;
volatile uint64_t eight = 8;
void *volatile ptr = NULL;

static uint32_t read_vol32(volatile uint32_t * ptr)
{
    return *ptr;
}

#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
static uint64_t read_vol64(volatile uint64_t * ptr)
{
    return *ptr;
}

#endif

static void *read_volptr(void *volatile *ptr)
{
    return *ptr;
}

int main(int argc,
         char *argv[])
{
    int i;
    aligned_t rets[30];

    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();

    rets[0] = qthread_cas(&master, master, 1);
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

    assert(qthread_cas(&four, 4, 5) == 4);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(qthread_cas(&eight, 8, 9) == 8);
#endif
    assert(read_vol32(&four) == 5);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
    assert(read_vol64(&eight) == 9);
#endif

    assert(qthread_cas_ptr(&ptr, NULL, &i) == NULL);
    assert(read_volptr(&ptr) == &i);
    iprintf("success!\n");

    return 0;
}

/* vim:set expandtab */
