#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <qthread/qthread.h>

#include "qt_asserts.h"
#include "qthread_innards.h"
#include "qt_profiling.h"

extern unsigned int QTHREAD_LOCKING_STRIPES;
#define QTHREAD_CHOOSE_STRIPE(addr) (((size_t)addr >> 4) & (QTHREAD_LOCKING_STRIPES - 1))

#if defined(QTHREAD_MUTEX_INCREMENT) ||             \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
uint32_t qthread_incr32_(uint32_t     *op,
                         const int32_t incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint32_t     retval;

    QTHREAD_FEB_TIMER_DECLARATION(incr);

    assert(qthread_library_initialized);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_FEB_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_FEB_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_FEB_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}                      /*}}} */

uint64_t qthread_incr64_(uint64_t     *op,
                         const int64_t incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint64_t     retval;

    QTHREAD_FEB_TIMER_DECLARATION(incr);

    assert(qthread_library_initialized);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_FEB_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_FEB_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_FEB_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}                      /*}}} */

double qthread_dincr_(double      *op,
                      const double incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    double       retval;

    assert(qthread_library_initialized);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

float qthread_fincr_(float      *op,
                     const float incr)
{                      /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    float        retval;

    assert(qthread_library_initialized);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op   += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

uint32_t qthread_cas32_(uint32_t      *operand,
                        const uint32_t oldval,
                        const uint32_t newval)
{                      /*{{{ */
    uint32_t     retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    assert(qthread_library_initialized);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
        *operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

uint64_t qthread_cas64_(uint64_t      *operand,
                        const uint64_t oldval,
                        const uint64_t newval)
{                      /*{{{ */
    uint64_t     retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    assert(qthread_library_initialized);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
        *operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}                      /*}}} */

#else /* if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) */
# error Building this file erroneously.
#endif /* if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) */
/* vim:set expandtab: */
