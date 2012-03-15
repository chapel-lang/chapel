#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <limits.h>                    /* for INT_MAX */
#include <qthread/qthread-int.h>       /* for uint64_t */

/* Internal Headers */
#include "qt_hash.h"
#include "qthread/qthread.h"
#include "qthread_innards.h"
#include "qt_profiling.h"
#include "qt_blocking_structs.h"
#include "qt_addrstat.h"
#include "qt_qthread_struct.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"
#include "qt_internal_syncvar.h"

/* Internal Prototypes */
static QINLINE void qthread_syncvar_gotlock_fill(qthread_shepherd_t *shep,
                                                 qthread_addrstat_t *m,
                                                 syncvar_t          *maddr,
                                                 const uint64_t      ret);
static QINLINE void qthread_syncvar_gotlock_empty(qthread_shepherd_t *shep,
                                                  qthread_addrstat_t *m,
                                                  syncvar_t          *maddr,
                                                  const uint64_t      ret);

/* Internal Structs */
typedef struct {
    unsigned int cf : 1; // there was a timeout
    unsigned int zf : 1; // unused
    unsigned int of : 1;
    unsigned int pf : 1;
    unsigned int sf : 1;
} eflags_t;

typedef enum bt {
    WRITEEF,
    WRITEEF_NB,
    WRITEF,
    READFF,
    READFF_NB,
    READFE,
    READFE_NB,
    FILL,
    EMPTY,
    INCR
} blocker_type;
typedef struct {
    pthread_mutex_t lock;
    void           *a;
    void           *b;
    blocker_type    type;
    int             retval;
} qthread_syncvar_blocker_t;

/* Internal Macros */
#define BUILD_UNLOCKED_SYNCVAR(data, state) (((data) << 4) | ((state) << 1))

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
# define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked) do { \
        MACHINE_FENCE;                                       \
        (addr)->u.s.lock = 0;                                \
} while (0)
# define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state) do { \
        MACHINE_FENCE;                                       \
        (addr)->u.w = BUILD_UNLOCKED_SYNCVAR(val, state);    \
} while (0)
#elif ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) ||    \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) ||         \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) ||         \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64) ||   \
    defined(__tile__))
# define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked) do { \
        MACHINE_FENCE;                                       \
        (addr)->u.w = (unlocked);                            \
} while (0)
# define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state) do { \
        MACHINE_FENCE;                                       \
        (addr)->u.w = BUILD_UNLOCKED_SYNCVAR(val, state);    \
} while (0)
#else /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) */
# define UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked) do {                \
        /* this has its own pthread mutex, so does not need memory synch */ \
        qthread_cas64(&((addr)->u.w), (addr)->u.w, (unlocked));             \
} while (0)
# define UNLOCK_THIS_MODIFIED_SYNCVAR(addr, val, state) do {                            \
        /* this has its own pthread mutex, so does not need memory synch */             \
        qthread_cas64(&((addr)->u.w), (addr)->u.w, BUILD_UNLOCKED_SYNCVAR(val, state)); \
} while (0)
#endif /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) */

static uint64_t qthread_mwaitc(syncvar_t *const restrict addr,
                               unsigned char const       statemask,
                               unsigned int              timeout,
                               eflags_t *const restrict  err)
{                                      /*{{{ */
#if ((QTHREAD_ASSEMBLY_ARCH != QTHREAD_TILEPRO) && \
    (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32))
    syncvar_t unlocked;
#endif
    syncvar_t locked;
    eflags_t  e;

    assert(timeout > 0);
    assert(addr != NULL);
    assert(err != NULL);

    e    = *err;
    e.zf = 0;
    e.cf = 1;
    do {
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEPRO)
        uint32_t low, high;
        int32_t *addrptr = (int32_t *)addr;
        /* note that the tilera is little-endian, otherwise this would be
         * addrptr+1 */
        while ((low = __insn_tns(addrptr)) == 1) {
            if (timeout-- <= 0) { goto errexit; }
            SPINLOCK_BODY();
        }
        /* now addrptr[0] is 1 and low is the "real" (unlocked) addrptr[0]
         * value. */
        high       = addrptr[1];
        locked.u.w = (((uint64_t)high) << 32) | low;
        MACHINE_FENCE;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
        /* This applies for any 32-bit architecture with a valid 32-bit CAS
         * (though I'm making some big-endian assumptions at the moment) */
        uint32_t  low_unlocked, low_locked;
        uint32_t *addrptr = (uint32_t *)addr;
        do {
loop_start:
            low_unlocked = addrptr[1];  // atomic read
            if ((low_unlocked & 1) != 0) {
                if (timeout-- <= 0) { goto errexit; }
                SPINLOCK_BODY();
                goto loop_start;
            }
            low_locked = low_unlocked | 1;
            if (qthread_cas32(&(addrptr[1]), low_unlocked, low_locked) == low_unlocked) { break; }
            if (timeout-- <= 0) { goto errexit; }
        } while (1);
        locked.u.w = addr->u.w; // I locked it, so I can read it
#else                           /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEPRO) */
        do {
loop_start:
            unlocked = *addr;          // may be locked or unlocked, we don't know
            if (unlocked.u.s.lock == 1) {
                if (timeout-- <= 0) { goto errexit; }
                SPINLOCK_BODY();
                goto loop_start;
            }
            locked          = unlocked;
            locked.u.s.lock = 1;       // create the locked version
            if (qthread_cas64((uint64_t *)addr, unlocked.u.w, locked.u.w) == unlocked.u.w) { break; }
            if (timeout-- <= 0) { goto errexit; }
        } while (1);
#endif  /* if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEPRO) */
        /***************************************************
        * now locked == unlocked, and the lock bit is set *
        ***************************************************/
        if (statemask & (1 << locked.u.s.state)) {
            /* this is a state of interest, so fill the err struct */
            e.cf = 0;
            e.sf = (unsigned char)(locked.u.s.state & 1);
            e.pf = (unsigned char)((locked.u.s.state >> 1) & 1);
            e.of = (unsigned char)((locked.u.s.state >> 2) & 1);
            *err = e;
            qthread_debug(SYNCVAR_DETAILS, "returning locked... (%i)\n", (int)locked.u.s.data);
            return locked.u.s.data;
        } else {
            /* this is NOT a state of interest, so unlock the locked bit */
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_TILEPRO)
            MACHINE_FENCE;
            addrptr[0] = low;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
            MACHINE_FENCE;
            addrptr[1] = low_unlocked;
#else
            UNLOCK_THIS_UNMODIFIED_SYNCVAR(addr, unlocked.u.w);
#endif
        }
    } while (timeout-- > 0);
errexit:
    *err = e;
    return 0;
}                                      /*}}} */

const syncvar_t SYNCVAR_INITIALIZER       = SYNCVAR_STATIC_INITIALIZER;
const syncvar_t SYNCVAR_EMPTY_INITIALIZER = SYNCVAR_STATIC_EMPTY_INITIALIZER;

int qthread_syncvar_status(syncvar_t *const v)
{                                      /*{{{ */
    eflags_t     e = { 0, 0, 0, 0, 0 };
    unsigned int realret;

#ifdef __tile__
    uint64_t ret = qthread_mwaitc(v, 0xff, INT_MAX, &e);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    realret = (e.of << 2) | (e.pf << 1) | e.sf;
    MACHINE_FENCE;
    v->u.w = BUILD_UNLOCKED_SYNCVAR(ret, realret);
    return (realret & 0x2) ? 0 : 1;

#else
# if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||   \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) ||      \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64))
    {
        /* I'm being optimistic here; this only works if a basic 64-bit load is
         * atomic (on most platforms it is). Thus, if I've done an atomic read
         * and the syncvar is unlocked, then I figure I can trust
         * that state and do not need to do a locked atomic operation of any
         * kind (e.g. cas) */
        syncvar_t local_copy_of_v = *v;
        if (local_copy_of_v.u.s.lock == 0) {
            /* short-circuit */
            return (local_copy_of_v.u.s.state & 0x2) ? 0 : 1;
        }
    }
# endif /* if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)) */
    (void)qthread_mwaitc(v, 0xff, INT_MAX, &e);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    realret = v->u.s.state;
    UNLOCK_THIS_UNMODIFIED_SYNCVAR(v, BUILD_UNLOCKED_SYNCVAR(v->u.s.data, v->u.s.state));
    return (realret & 0x2) ? 0 : 1;
#endif /* __tile__ */
}                                      /*}}} */

static aligned_t qthread_syncvar_blocker_thread(void *arg)
{                                      /*{{{ */
    qthread_syncvar_blocker_t *const restrict a = (qthread_syncvar_blocker_t *)arg;

    switch (a->type) {
        case READFE: a->retval     = qthread_syncvar_readFE(a->a, a->b); break;
        case READFE_NB: a->retval  = qthread_syncvar_readFE_nb(a->a, a->b); break;
        case READFF: a->retval     = qthread_syncvar_readFF(a->a, a->b); break;
        case READFF_NB: a->retval  = qthread_syncvar_readFF_nb(a->a, a->b); break;
        case WRITEEF: a->retval    = qthread_syncvar_writeEF(a->a, a->b); break;
        case WRITEEF_NB: a->retval = qthread_syncvar_writeEF_nb(a->a, a->b); break;
        case WRITEF: a->retval     = qthread_syncvar_writeF(a->a, a->b); break;
        case FILL: a->retval       = qthread_syncvar_fill(a->a); break;
        case EMPTY: a->retval      = qthread_syncvar_empty(a->a); break;
        case INCR: a->retval       = qthread_syncvar_incrF(a->a, *(int64_t *)a->b); break;
    }
    pthread_mutex_unlock(&(a->lock));
    return 0;
}                                      /*}}} */

static int qthread_syncvar_blocker_func(void        *dest,
                                        void        *src,
                                        blocker_type t)
{   /*{{{*/
    qthread_syncvar_blocker_t args = { PTHREAD_MUTEX_INITIALIZER, dest, src, t, QTHREAD_SUCCESS };

    pthread_mutex_lock(&args.lock);
    qthread_fork(qthread_syncvar_blocker_thread, &args, NULL);
    pthread_mutex_lock(&args.lock);
    pthread_mutex_unlock(&args.lock);
    pthread_mutex_destroy(&args.lock);
    return args.retval;
} /*}}}*/

/* state 0: full, no waiters
 * state 1: full, queued waiters (who are waiting for it to be empty)
 * state 2: empty, no waiters
 * state 3: empty, queued waiters (who are waiting for it to be full)
 * state 4-7: undefined
 */
/* ops */
/* fff */
#define SYNCFEB_FULL_NOWAIT              0x1 /* 000 */
#define SYNCFEB_FULL_WAITERS             0x2 /* 001 */
#define SYNCFEB_FULL                     0x3 /* 00x */
#define SYNCFEB_EMPTY_NOWAIT             0x4 /* 010 */
#define SYNCFEB_EMPTY_WAITERS            0x8 /* 011 */
#define SYNCFEB_EMPTY                    0xc /* 01x */
#define SYNCFEB_ANY                      0xf /* 0xx */
#define INITIAL_TIMEOUT                  100
#define SYNCFEB_STATE_FULL_NO_WAITERS    0x0
#define SYNCFEB_STATE_FULL_WITH_WAITERS  0x1
#define SYNCFEB_STATE_EMPTY_NO_WAITERS   0x2
#define SYNCFEB_STATE_EMPTY_WITH_WAITERS 0x3

int API_FUNC qthread_syncvar_readFF(uint64_t *restrict const  dest,
                                    syncvar_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   ret;
    qthread_t *me = qthread_internal_self();

    assert(src);
    qthread_debug(SYNCVAR_CALLS, "me(%p), dest(%p), src(%p) = %x\n", me, dest, src, (uintptr_t)src->u.w);

    if (!me) {
        return qthread_syncvar_blocker_func(dest, src, READFF);
    }

#if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||    \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) ||      \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64))
    {
        /* I'm being optimistic here; this only works if a basic 64-bit load is
         * atomic (on most platforms it is). Thus, if I've done an atomic read
         * and the syncvar is both unlocked and full, then I figure I can trust
         * that state and do not need to do a locked atomic operation of any
         * kind (e.g. cas) */
        syncvar_t local_copy_of_src = *src;
        if ((local_copy_of_src.u.s.lock == 0) && ((local_copy_of_src.u.s.state & 2) == 0)) {        /* full and unlocked */
            /* short-circuit */
            if (dest) {
                *dest = local_copy_of_src.u.s.data;
            }
            return QTHREAD_SUCCESS;
        }
    }
#endif /* if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)) */
    ret = qthread_mwaitc(src, SYNCFEB_FULL, INITIAL_TIMEOUT, &e);
    qthread_debug(SYNCVAR_DETAILS, "2 src(%p) = %x, ret = %x\n", src,
                  (uintptr_t)src->u.w, ret);
    if (e.cf) {                        /* there was a timeout */
        QTHREAD_WAIT_TIMER_DECLARATION;
        const int           lockbin = QTHREAD_CHOOSE_STRIPE(src);
        qthread_addrstat_t *m;
        qthread_addrres_t  *X;

        ret = qthread_mwaitc(src, SYNCFEB_ANY, INT_MAX, &e);
        qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
        if (e.pf == 0) {                         /* it got full! */
            goto locked_full;
        }
        qt_hash_lock(qlib->syncvars[lockbin]);
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_WITH_WAITERS);
        qthread_debug(SYNCVAR_DETAILS,
                      "3 src(%p) = %x (queued waiter waiting for full)\n",
                      src, (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret, SYNCFEB_STATE_EMPTY_WITH_WAITERS));
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)src);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            assert(m);
            if (!m) {
                qt_hash_unlock(qlib->syncvars[lockbin]);
                return ENOMEM;
            }
            qassertnot(qt_hash_put_locked(qlib->syncvars[lockbin], (void *)src, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        qt_hash_unlock(qlib->syncvars[lockbin]);
        X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
        assert(X);
        if (!X) {
            qthread_addrstat_delete(m);
            return ENOMEM;
        }
        X->addr   = (aligned_t *)dest;
        X->waiter = me;
        X->next   = m->FFQ;
        m->FFQ    = X;
        qthread_debug(SYNCVAR_DETAILS, "back to parent\n");
        me->thread_state     = QTHREAD_STATE_FEB_BLOCKED;
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(SYNCVAR_DETAILS, "src(%p) woke up\n", src);
    } else {
        qthread_debug(SYNCVAR_DETAILS, "locked/full on the first try; word=%x, state = %x, ret=%x\n", (unsigned int)src->u.w, (int)src->u.s.state, (int)ret);
locked_full:
        /* at this point, the syncvar is locked and e.pf should be 0 */
        assert(e.pf == 0);
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, e.sf);
        if (dest) { *dest = ret; }
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int INTERNAL qthread_syncvar_readFF_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   ret;
    qthread_t *me = qthread_internal_self();

    assert(src);
    qthread_debug(SYNCVAR_CALLS, "me(%p), dest(%p), src(%p) = %x\n", me, dest, src, (uintptr_t)src->u.w);

    if (!me) {
        return qthread_syncvar_blocker_func(dest, src, READFF_NB);
    }

#if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) ||    \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) ||      \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64))
    {
        /* I'm being optimistic here; this only works if a basic 64-bit load is
         * atomic (on most platforms it is). Thus, if I've done an atomic read
         * and the syncvar is both unlocked and full, then I figure I can trust
         * that state and do not need to do a locked atomic operation of any
         * kind (e.g. cas) */
        syncvar_t local_copy_of_src = *src;
        if ((local_copy_of_src.u.s.lock == 0) && ((local_copy_of_src.u.s.state & 2) == 0)) {        /* full and unlocked */
            /* short-circuit */
            if (dest) {
                *dest = local_copy_of_src.u.s.data;
            }
            return QTHREAD_SUCCESS;
        }
    }
#endif /* if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)) */
    ret = qthread_mwaitc(src, SYNCFEB_FULL, 1, &e);
    qthread_debug(SYNCVAR_DETAILS, "2 src(%p) = %x, ret = %x\n", src,
                  (uintptr_t)src->u.w, ret);
    if (e.cf) {                        /* there was a timeout */
        qthread_debug(SYNCVAR_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        return QTHREAD_OPFAIL;
    } else {
        qthread_debug(SYNCVAR_DETAILS, "locked/full on the first try; word=%x, state = %x, ret=%x\n", (unsigned int)src->u.w, (int)src->u.s.state, (int)ret);
        /* at this point, the syncvar is locked and e.pf should be 0 */
        assert(e.pf == 0);
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, e.sf);
        if (dest) { *dest = ret; }
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int API_FUNC qthread_syncvar_fill(syncvar_t *restrict const addr)
{                                      /*{{{ */
    eflags_t            e = { 0, 0, 0, 0, 0 };
    uint64_t            ret;
    qthread_shepherd_t *shep = qthread_internal_getshep();

    assert(addr);

    qthread_debug(SYNCVAR_BEHAVIOR, "shep(%p), addr(%p) = %x\n", shep, addr,
                  (uintptr_t)addr->u.w);
    if (!shep) {
        return qthread_syncvar_blocker_func(addr, NULL, FILL);
    }
    ret = qthread_mwaitc(addr, SYNCFEB_ANY, INT_MAX, &e);
    qthread_debug(SYNCVAR_DETAILS, "shep(%p), addr(%p) = %x (b)\n", shep, addr,
                  (uintptr_t)addr->u.w);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if (e.pf == 1) {                         /* currently empty, so it needs to change state */
        if (e.sf == 1) {                     /* waiters! */
            const int           lockbin = QTHREAD_CHOOSE_STRIPE(addr);
            qthread_addrstat_t *m;

            e.sf = 0;                  // I'm releasing waiters
            e.pf = 0;                  // I'm going to mark this as full
            qt_hash_lock(qlib->syncvars[lockbin]);
            m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)addr);
            assert(m);                 // otherwise there weren't really any waiters
            QTHREAD_FASTLOCK_LOCK(&(m->lock));
            qt_hash_unlock(qlib->syncvars[lockbin]);
            if (m->FEQ) {
                e.pf = 1;                      // back to being empty
                if (m->FEQ->next) {
                    e.sf = 1;          // only one will be dequeued, so it'll still have waiters
                }
            }
            UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, (e.pf << 1) | e.sf);
            assert(m->FFQ || m->FEQ);      // otherwise there weren't really any waiters
            assert(m->EFQ == NULL);        // someone snuck in!
            qthread_syncvar_gotlock_fill(shep, m, addr, ret);
        } else {
            assert(e.sf == 0);         /* no waiters */
            UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, 0);
        }
    } else { /* already empty, so just release the lock */
        assert(e.pf == 0);
        UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, e.sf);
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int API_FUNC qthread_syncvar_empty(syncvar_t *restrict const addr)
{                                      /*{{{ */
    eflags_t            e = { 0, 0, 0, 0, 0 };
    uint64_t            ret;
    qthread_shepherd_t *shep = qthread_internal_getshep();

    assert(addr);

    qthread_debug(SYNCVAR_DETAILS, "shep(%p), addr(%p) = %x\n", shep, addr, (uintptr_t)addr->u.w);
    if (!shep) {
        return qthread_syncvar_blocker_func(addr, NULL, EMPTY);
    }
    ret = qthread_mwaitc(addr, SYNCFEB_ANY, INT_MAX, &e);
    qthread_debug(SYNCVAR_DETAILS, "shep(%p), addr(%p) = %x (b)\n", shep, addr, (uintptr_t)addr->u.w);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if (e.pf == 0) {                         /* currently full, so it needs to change state */
        if (e.sf == 1) {                     /* waiters! */
            const int           lockbin = QTHREAD_CHOOSE_STRIPE(addr);
            qthread_addrstat_t *m;

            e.sf = 0;                  // released!
            // wanted to mark it empty, but the waiters will fill it
            qt_hash_lock(qlib->syncvars[lockbin]);
            m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)addr);
            assert(m);                 // otherwise, there weren't any waiters
            QTHREAD_FASTLOCK_LOCK(&(m->lock));
            assert(m->EFQ);                           // otherwise there weren't really any waiters
            assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
            qt_hash_unlock(qlib->syncvars[lockbin]);
            if (m->EFQ->next) {
                e.sf = 1;
            }
            qthread_debug(SYNCVAR_DETAILS, "m->FEQ = %p, m->FFQ = %p, m->EFQ = %p\n",
                          m->FEQ, m->FFQ, m->EFQ);
            // addr->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by gotlock_empty so we know what value to write
            qthread_syncvar_gotlock_empty(shep, m, addr, e.sf);
            qthread_debug(SYNCVAR_DETAILS, "empty(%p) => %x\n", addr,
                          (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret, (e.pf << 1) | e.sf));
        } else {
            assert(e.sf == 0);         /* no waiters */
            UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
        }
    } else { /* already empty, so just release the lock */
        assert(e.pf == 1);
        UNLOCK_THIS_MODIFIED_SYNCVAR(addr, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS | e.sf);
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int API_FUNC qthread_syncvar_readFE(uint64_t *restrict const  dest,
                                    syncvar_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   ret;
    const int  lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t *me      = qthread_internal_self();

    assert(src);

    if (!me) {
        return qthread_syncvar_blocker_func(dest, src, READFE);
    }

    assert(me->rdata);
    assert(me->rdata->shepherd_ptr);
    assert(me->rdata->shepherd_ptr->ready);
    assert(me->rdata->shepherd_ptr->shepherd_id >= 0);
    assert(me->rdata->shepherd_ptr->shepherd_id < qthread_num_shepherds());

    qthread_debug(SYNCVAR_BEHAVIOR, "me(%p), dest(%p), src(%p) = %x\n", me, dest,
                  src, (uintptr_t)src->u.w);
    ret = qthread_mwaitc(src, SYNCFEB_FULL, INITIAL_TIMEOUT, &e);
    qthread_debug(SYNCVAR_DETAILS, "2 src(%p) = %x\n", src,
                  (uintptr_t)src->u.w);
    if (e.cf) {                        /* there was a timeout */
        QTHREAD_WAIT_TIMER_DECLARATION;
        qthread_addrstat_t *m;
        qthread_addrres_t  *X;

        ret = qthread_mwaitc(src, SYNCFEB_ANY, INT_MAX, &e);
        qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
        if (e.pf == 0) {                         /* it got full! */
            if (e.sf == 1) {                     /* it got full with waiters! */
                goto locked_full_waiters;
            } else {
                goto locked_full;
            }
        }
        qt_hash_lock(qlib->syncvars[lockbin]);
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_WITH_WAITERS);
        qthread_debug(SYNCVAR_DETAILS,
                      "3 src(%p) = %x (queued waiter waiting for full)\n",
                      src, (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret,
                                                             SYNCFEB_STATE_EMPTY_WITH_WAITERS));
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin],
                                                     (void *)src);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            assert(m);
            if (!m) {
                qt_hash_unlock(qlib->syncvars[lockbin]);
                return ENOMEM;
            }
            qassertnot(qt_hash_put_locked(qlib->syncvars[lockbin], (void *)src, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        qt_hash_unlock(qlib->syncvars[lockbin]);
        X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
        assert(X);
        if (!X) {
            qthread_addrstat_delete(m);
            return ENOMEM;
        }
        X->addr   = (aligned_t *)&ret;
        X->waiter = me;
        X->next   = m->FEQ;
        m->FEQ    = X;
        qthread_debug(SYNCVAR_DETAILS, "back to parent\n");
        me->thread_state     = QTHREAD_STATE_FEB_BLOCKED;
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(SYNCVAR_DETAILS, "src(%p) woke up\n", src);
    } else if (e.sf == 1) {            /* waiters! */
        qthread_addrstat_t *m;

locked_full_waiters:
        assert(e.pf == 0);             // otherwise we should have gotten a timeout
        e.sf = 0;                      // released!
        // wanted to mark it empty (pf=1), but the waiters will fill it
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)src);
        assert(m);                     // otherwise, there weren't any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        assert(m->EFQ);                           // otherwise there weren't really any waiters
        assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->EFQ->next) {            // there will be a waiter still waiting
            e.sf = 1;
        }
        qthread_debug(SYNCVAR_DETAILS, "m->FEQ = %p, m->FFQ = %p, m->EFQ = %p\n",
                      m->FEQ, m->FFQ, m->EFQ);
        // src->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by gotlock_empty so we know what value to write
        qthread_syncvar_gotlock_empty(me->rdata->shepherd_ptr, m, src, e.sf);
        qthread_debug(SYNCVAR_DETAILS, "src(%p) => %x\n", src,
                      (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret, e.sf));
    } else {
locked_full:
        assert(e.pf == 0);             // otherwise this isn't really full
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
    }
    if (dest) {
        *dest = ret;
    }
    qthread_debug(SYNCVAR_DETAILS, "src(%p) exiting\n", src);
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int INTERNAL qthread_syncvar_readFE_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   ret;
    const int  lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t *me      = qthread_internal_self();

    assert(src);

    if (!me) {
        return qthread_syncvar_blocker_func(dest, src, READFE_NB);
    }

    assert(me->rdata);
    assert(me->rdata->shepherd_ptr);
    assert(me->rdata->shepherd_ptr->ready);
    assert(me->rdata->shepherd_ptr->shepherd_id >= 0);
    assert(me->rdata->shepherd_ptr->shepherd_id < qthread_num_shepherds());

    qthread_debug(SYNCVAR_BEHAVIOR, "me(%p), dest(%p), src(%p) = %x\n", me, dest,
                  src, (uintptr_t)src->u.w);
    ret = qthread_mwaitc(src, SYNCFEB_FULL, 1, &e);
    qthread_debug(SYNCVAR_DETAILS, "2 src(%p) = %x\n", src,
                  (uintptr_t)src->u.w);
    if (e.cf) {                        /* there was a timeout */
        qthread_debug(SYNCVAR_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        return QTHREAD_OPFAIL;
    } else if (e.sf == 1) {            /* waiters! */
        qthread_addrstat_t *m;

        assert(e.pf == 0);             // otherwise we should have gotten a timeout
        e.sf = 0;                      // released!
        // wanted to mark it empty (pf=1), but the waiters will fill it
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)src);
        assert(m);                     // otherwise, there weren't any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        assert(m->EFQ);                           // otherwise there weren't really any waiters
        assert(m->FFQ == NULL && m->FEQ == NULL); // someone snuck in!
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->EFQ->next) {            // there will be a waiter still waiting
            e.sf = 1;
        }
        qthread_debug(SYNCVAR_DETAILS, "m->FEQ = %p, m->FFQ = %p, m->EFQ = %p\n",
                      m->FEQ, m->FFQ, m->EFQ);
        // src->u.w = BUILD_UNLOCKED_SYNCVAR(ret, e.sf); // this must be done by gotlock_empty so we know what value to write
        qthread_syncvar_gotlock_empty(me->rdata->shepherd_ptr, m, src, e.sf);
        qthread_debug(SYNCVAR_DETAILS, "src(%p) => %x\n", src,
                      (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret, e.sf));
    } else {
        assert(e.pf == 0);             // otherwise this isn't really full
        UNLOCK_THIS_MODIFIED_SYNCVAR(src, ret, SYNCFEB_STATE_EMPTY_NO_WAITERS);
    }
    if (dest) {
        *dest = ret;
    }
    qthread_debug(SYNCVAR_DETAILS, "src(%p) exiting\n", src);
    return QTHREAD_SUCCESS;
}                                      /*}}} */

static QINLINE void qthread_syncvar_gotlock_empty(qthread_shepherd_t *shep,
                                                  qthread_addrstat_t *m,
                                                  syncvar_t          *maddr,
                                                  const uint64_t      sf)
{                                      /*{{{ */
    qthread_addrres_t *X = NULL;
    int                removeable;

    qthread_debug(SYNCVAR_DETAILS, "m(%p), addr(%p)\n", m, maddr);
    m->full = 0;
    QTHREAD_EMPTY_TIMER_START(m);
    if (m->EFQ != NULL) {
        /* dequeue one EFQ, do its operation, and schedule the thread */
        qthread_debug(SYNCVAR_DETAILS, "dQ 1 EFQ\n");
        X      = m->EFQ;
        m->EFQ = X->next;
        /* op */
        if (maddr && (maddr != (syncvar_t *)X->addr)) {
            UNLOCK_THIS_MODIFIED_SYNCVAR(maddr, *((uint64_t *)X->addr), sf);
        }
        X->waiter->thread_state = QTHREAD_STATE_RUNNING;
        qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter);
        FREE_ADDRRES(X);
    }
    if ((m->EFQ == NULL) && (m->FEQ == NULL) && (m->FFQ == NULL)) {
        removeable = 1;
    } else {
        removeable = 0;
    }
    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    if (removeable) {
        const int lockbin = QTHREAD_CHOOSE_STRIPE(maddr);
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)maddr);
        if (m) {
            QTHREAD_FASTLOCK_LOCK(&(m->lock));
            if ((m->FEQ == NULL) && (m->EFQ == NULL) && (m->FFQ == NULL)) {
                qthread_debug(SYNCVAR_DETAILS, "all lists are empty\n");
                qassertnot(qt_hash_remove_locked(qlib->syncvars[lockbin], (void *)maddr), 0);
            } else {
                QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
                qthread_debug(SYNCVAR_DETAILS, "addr cannot be removed; in use\n");
                m = NULL;
            }
        }
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m != NULL) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            qthread_addrstat_delete(m);
        }
    }
}                                      /*}}} */

static QINLINE void qthread_syncvar_gotlock_fill(qthread_shepherd_t *shep,
                                                 qthread_addrstat_t *m,
                                                 syncvar_t          *maddr,
                                                 const uint64_t      ret)
{                                      /*{{{ */
    qthread_addrres_t *X = NULL;
    int                removeable;

    qthread_debug(SYNCVAR_DETAILS, "m(%p), addr(%p)\n", m, maddr);
    m->full = 1;
    QTHREAD_EMPTY_TIMER_STOP(m);
    /* dequeue all FFQ, do their operation, and schedule them */
    qthread_debug(SYNCVAR_DETAILS, "dQ all FFQ\n");
    while (m->FFQ != NULL) {
        /* dQ */
        X      = m->FFQ;
        m->FFQ = X->next;
        /* op */
        if (X->addr) {
            *(uint64_t *)X->addr = ret;
        }
        /* schedule */
        X->waiter->thread_state = QTHREAD_STATE_RUNNING;
        qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter);
        FREE_ADDRRES(X);
    }
    if (m->FEQ != NULL) {
        /* dequeue one FEQ, do their operation, and reschedule them */
        qthread_debug(SYNCVAR_DETAILS, "dQ 1 FEQ\n");
        X      = m->FEQ;
        m->FEQ = X->next;
        /* op */
        if (X->addr) {
            *(uint64_t *)X->addr = ret;
        }
        X->waiter->thread_state = QTHREAD_STATE_RUNNING;
        qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter);
        FREE_ADDRRES(X);
    }
    if ((m->EFQ == NULL) && (m->FEQ == NULL) && (m->FFQ == NULL)) {
        removeable = 1;
    } else {
        removeable = 0;
    }
    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    if (removeable) {
        const int lockbin = QTHREAD_CHOOSE_STRIPE(maddr);
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)maddr);
        if (m) {
            QTHREAD_FASTLOCK_LOCK(&(m->lock));
            if ((m->FEQ == NULL) && (m->EFQ == NULL) && (m->FFQ == NULL)) {
                qthread_debug(SYNCVAR_DETAILS, "all lists are empty\n");
                qt_hash_remove_locked(qlib->syncvars[lockbin], (void *)maddr);
            } else {
                QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
                qthread_debug(SYNCVAR_DETAILS, "address cannot be removed; in use\n");
                m = NULL;
            }
        }
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m != NULL) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            qthread_addrstat_delete(m);
        }
    }
}                                      /*}}} */

int API_FUNC qthread_syncvar_writeF(syncvar_t *restrict const      dest,
                                    const uint64_t *restrict const src)
{                                      /*{{{ */
    eflags_t            e    = { 0, 0, 0, 0, 0 };
    uint64_t            ret  = *src;
    qthread_shepherd_t *shep = qthread_internal_getshep();

    qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

    qthread_debug(SYNCVAR_BEHAVIOR, "shep(%p), dest(%p) = %x, src(%p) = %x\n", shep,
                  dest, (unsigned long)dest->u.w, src, *src);
    if (!shep) {
        return qthread_syncvar_blocker_func(dest, (void *)src, WRITEF);
    }
    qthread_mwaitc(dest, SYNCFEB_ANY, INT_MAX, &e);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if ((e.pf == 1) && (e.sf == 1)) {        /* there are waiters to release */
        const int           lockbin = QTHREAD_CHOOSE_STRIPE(dest);
        qthread_addrstat_t *m;

        e.sf = 0;                      // I'm releasing waiters
        e.pf = 0;                      // I'm going to mark this as full
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)dest);
        assert(m);                     // otherwise there weren't really any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->FEQ) {
            e.pf = 1;                  // back to being empty
            if (m->FEQ->next) {
                e.sf = 1;              // only one will be dequeued, so it'll still have waiters
            }
        }
        UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, (e.pf << 1) | e.sf);
        assert(m->FFQ || m->FEQ);      // otherwise there weren't really any waiters
        assert(m->EFQ == NULL);        // someone snuck in!
        qthread_syncvar_gotlock_fill(shep, m, dest, ret);
    } else {
        UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, 0);
    }

    return QTHREAD_SUCCESS;
}                                      /*}}} */

int API_FUNC qthread_syncvar_writeF_const(syncvar_t *restrict const dest,
                                          const uint64_t            src)
{                                      /*{{{ */
    return qthread_syncvar_writeF(dest, &src);
}                                      /*}}} */

int API_FUNC qthread_syncvar_writeEF(syncvar_t *restrict const      dest,
                                     const uint64_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   ret;
    const int  lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t *me      = qthread_internal_self();

    qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

    qthread_debug(SYNCVAR_DETAILS, "writeEF dest(%p) = %x\n", dest, (uintptr_t)dest->u.w);
    if (!me) {
        return qthread_syncvar_blocker_func(dest, (void *)src, WRITEEF);
    }
    ret = qthread_mwaitc(dest, SYNCFEB_EMPTY, INITIAL_TIMEOUT, &e);
    if (e.cf) {                        /* there was a timeout */
        QTHREAD_WAIT_TIMER_DECLARATION;
        qthread_addrstat_t *m;
        qthread_addrres_t  *X;

        ret = qthread_mwaitc(dest, SYNCFEB_ANY, INT_MAX, &e);
        qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
        if (e.pf == 1) {                         /* it got empty! */
            if (e.sf == 1) {                     /* not just empty, but with waiters! */
                goto locked_empty_waiters;
            } else {
                goto locked_empty;
            }
        }
        /* lock the hash, then mark the syncvar as "consult-hash" and release it */
        qt_hash_lock(qlib->syncvars[lockbin]);
        UNLOCK_THIS_MODIFIED_SYNCVAR(dest, ret, SYNCFEB_STATE_FULL_WITH_WAITERS);
        qthread_debug(SYNCVAR_DETAILS,
                      "writeEF(c) dest(%p) = %x (queued waiter waiting for empty)\n",
                      dest, (uintptr_t)BUILD_UNLOCKED_SYNCVAR(ret, SYNCFEB_STATE_FULL_WITH_WAITERS));
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)dest);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            assert(m);
            if (!m) {
                qt_hash_unlock(qlib->syncvars[lockbin]);
                return ENOMEM;
            }
            qassertnot(qt_hash_put_locked(qlib->syncvars[lockbin], (void *)dest, m), 0);
        }
        /* lock the addrstat (already within the hash) and then unlock the hash */
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        qt_hash_unlock(qlib->syncvars[lockbin]);
        X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
        assert(X);
        X->addr   = (aligned_t *)src;
        X->waiter = me;
        X->next   = m->EFQ;
        m->EFQ    = X;
        qthread_debug(SYNCVAR_DETAILS, ": back to parent\n");
        me->thread_state     = QTHREAD_STATE_FEB_BLOCKED;
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(SYNCVAR_DETAILS, "writeEF(%p) woke up\n", dest);
    } else if (e.sf == 1) {            /* there are waiters to release! */
        qthread_addrstat_t *m;

locked_empty_waiters:
        assert(e.pf == 1);             // otherwise it wasn't really empty
        e.pf = 0;                      // mark full
        e.sf = 0;                      // released!
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)dest);
        assert(m);                     // otherwise there weren't really any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        assert(m->FFQ || m->FEQ);      // otherwise there weren't really any waiters
        assert(m->EFQ == NULL);        // someone snuck in!
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->FEQ) {
            e.pf = 1;
            if (m->FEQ->next) {
                e.sf = 1;
            }
        }
        qthread_debug(SYNCVAR_DETAILS, "m->FEQ = %p, m->FFQ = %p, m->EFQ = %p\n",
                      m->FEQ, m->FFQ, m->EFQ);
        {
            uint64_t val = *src;
            UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, (e.pf << 1) | e.sf);
            qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, dest, val);
            qthread_debug(SYNCVAR_DETAILS, "writeEF(%p) => %x ...1\n", dest,
                          (uintptr_t)BUILD_UNLOCKED_SYNCVAR(val, (e.pf << 1) | e.sf));
        }
    } else {
        uint64_t val;
locked_empty:
        assert(e.pf == 1);             // otherwise it wasn't really empty
        assert(e.sf == 0);
        val = *src;
        // e.pf = 0;                    // now mark it full
        UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, SYNCFEB_STATE_FULL_NO_WAITERS);
        qthread_debug(SYNCVAR_DETAILS, "writeEF(%p) => %x ...2\n", dest,
                      (uintptr_t)BUILD_UNLOCKED_SYNCVAR(val, SYNCFEB_STATE_FULL_NO_WAITERS));
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int API_FUNC qthread_syncvar_writeEF_const(syncvar_t *restrict const dest,
                                           const uint64_t            src)
{                                      /*{{{ */
    return qthread_syncvar_writeEF(dest, &src);
}                                      /*}}} */

int INTERNAL qthread_syncvar_writeEF_nb(syncvar_t *restrict const      dest,
                                        const uint64_t *restrict const src)
{                                      /*{{{ */
    eflags_t   e       = { 0, 0, 0, 0, 0 };
    const int  lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t *me      = qthread_internal_self();

    qassert_ret((*src >> 60) == 0, QTHREAD_OVERFLOW);

    qthread_debug(SYNCVAR_DETAILS, "writeEF dest(%p) = %x\n", dest, (uintptr_t)dest->u.w);
    if (!me) {
        return qthread_syncvar_blocker_func(dest, (void *)src, WRITEEF_NB);
    }
    (void)qthread_mwaitc(dest, SYNCFEB_EMPTY, 1, &e);
    if (e.cf) {                        /* there was a timeout */
        qthread_debug(SYNCVAR_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        return QTHREAD_OPFAIL;
    } else if (e.sf == 1) {            /* there are waiters to release! */
        qthread_addrstat_t *m;

        assert(e.pf == 1);             // otherwise it wasn't really empty
        e.pf = 0;                      // mark full
        e.sf = 0;                      // released!
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)dest);
        assert(m);                     // otherwise there weren't really any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        assert(m->FFQ || m->FEQ);      // otherwise there weren't really any waiters
        assert(m->EFQ == NULL);        // someone snuck in!
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->FEQ) {
            e.pf = 1;
            if (m->FEQ->next) {
                e.sf = 1;
            }
        }
        qthread_debug(SYNCVAR_DETAILS, "m->FEQ = %p, m->FFQ = %p, m->EFQ = %p\n",
                      m->FEQ, m->FFQ, m->EFQ);
        {
            uint64_t val = *src;
            UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, (e.pf << 1) | e.sf);
            qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, dest, val);
            qthread_debug(SYNCVAR_DETAILS, "writeEF(%p) => %x ...1\n", dest,
                          (uintptr_t)BUILD_UNLOCKED_SYNCVAR(val, (e.pf << 1) | e.sf));
        }
    } else {
        uint64_t val;
        assert(e.pf == 1);             // otherwise it wasn't really empty
        assert(e.sf == 0);
        val = *src;
        // e.pf = 0;                    // now mark it full
        UNLOCK_THIS_MODIFIED_SYNCVAR(dest, val, SYNCFEB_STATE_FULL_NO_WAITERS);
        qthread_debug(SYNCVAR_DETAILS, "writeEF(%p) => %x ...2\n", dest,
                      (uintptr_t)BUILD_UNLOCKED_SYNCVAR(val, SYNCFEB_STATE_FULL_NO_WAITERS));
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int INTERNAL qthread_syncvar_writeEF_const_nb(syncvar_t *restrict const dest,
                                              const uint64_t            src)
{                                      /*{{{ */
    return qthread_syncvar_writeEF_nb(dest, &src);
}                                      /*}}} */

uint64_t API_FUNC qthread_syncvar_incrF(syncvar_t *restrict const operand,
                                        const uint64_t            inc)
{                                      /*{{{ */
    eflags_t   e = { 0, 0, 0, 0, 0 };
    uint64_t   newv;
    qthread_t *me = qthread_internal_self();

    assert(operand);
    qthread_debug(SYNCVAR_BEHAVIOR, "me(%p), operand(%p), inc(%lu) = %x\n", me,
                  operand, (unsigned long)inc);
    if (!me) {
        return qthread_syncvar_blocker_func(operand, (void *)&inc, INCR);
    }
    qthread_mwaitc(operand, SYNCFEB_ANY, INT_MAX, &e);
    qassert_ret(e.cf == 0, QTHREAD_TIMEOUT); /* there better not have been a timeout */
    if ((e.pf == 1) && (e.sf == 1)) {        /* there are waiters to release */
        const int           lockbin = QTHREAD_CHOOSE_STRIPE(operand);
        qthread_addrstat_t *m;

        e.sf = 0;                      // I'm releasing waiters
        e.pf = 0;                      // I'm going to mark this as full
        qt_hash_lock(qlib->syncvars[lockbin]);
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->syncvars[lockbin], (void *)operand);
        assert(m);                     // otherwise there weren't really any waiters
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
        qt_hash_unlock(qlib->syncvars[lockbin]);
        if (m->FEQ) {
            e.pf = 1;                  // back to being empty
            if (m->FEQ->next) {
                e.sf = 1;              // only one will be dequeued, so it'll still have waiters
            }
        }
        newv = operand->u.s.data + inc;
        UNLOCK_THIS_MODIFIED_SYNCVAR(operand, newv, (e.pf << 1) | e.sf);
        assert(m->FFQ || m->EFQ);      // otherwise there weren't really any waiters
        assert(m->FEQ == NULL);        // someone snuck in!
        qthread_syncvar_gotlock_fill(me->rdata->shepherd_ptr, m, operand, newv);
    } else {
        newv = operand->u.s.data + inc;
        UNLOCK_THIS_MODIFIED_SYNCVAR(operand, newv, (e.pf << 1) | e.sf);
    }

    return newv;
}                                      /*}}} */

/* vim:set expandtab: */
