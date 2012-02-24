#ifndef FIFTY_SIX_RWLOCK_H
#define FIFTY_SIX_RWLOCK_H

/**
 * This reader-writer lock is derived from
 * "TLRW: Return of the Read-Write Lock"
 * Dice and Shavit, SPAA '10
 *
 * It supports at most 56 readers and an
 * unlimited number of writers. If the lock is used
 * with more than 56 readers, then unpredictable
 * behavior will happen.
 **/

#include <stdint.h>
#include <qt_atomics.h>
#include <assert.h>

#ifndef NOINLINE
# define NOINLINE __attribute__((noinline))
#endif

struct tlrw_lock {
    uint64_t owner;
    uint8_t  readers[128 - sizeof(uint64_t)];
};

typedef struct tlrw_lock rwlock_t;

static QINLINE void rwlock_init(rwlock_t *l)
{
    unsigned int i;

    l->owner = 0;
    for (i = 0; i < sizeof l->readers; i++) l->readers[i] = 0;
}

static QINLINE void rwlock_rdlock(rwlock_t *l,
                                  int       id)
{
    assert(id >= 0);
    for (;;) {
        l->readers[id] = 1;
        MACHINE_FENCE;

        if (l->owner == 0) {
            break;
        } else {
            l->readers[id] = 0;

            while (l->owner != 0) SPINLOCK_BODY();
        }
    }
}

static QINLINE void rwlock_rdunlock(rwlock_t *l,
                                    int       id)
{
    assert(id >= 0);
    l->readers[id] = 0;
}

static QINLINE void rwlock_wrlock(rwlock_t *l,
                                  int       id)
{
    assert(id >= 0);
    id = id + 1;

    uint64_t *readers = (void *)l->readers;
    while(qthread_cas32(&(l->owner), 0, id) != id) SPINLOCK_BODY();

    MACHINE_FENCE;
    for (int i = 0; i < sizeof(l->readers) / sizeof(uint64_t); i++) {
        while (readers[i] != 0) SPINLOCK_BODY();
    }
}

static QINLINE void rwlock_wrunlock(rwlock_t *l)
{
    l->owner = 0;
    MACHINE_FENCE;
}

#endif // ifndef FIFTY_SIX_RWLOCK_H
/* vim:set expandtab: */
