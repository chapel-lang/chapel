#include "qt_mpool.h"
#include "qt_shepherd_innards.h"
#include "qt_profiling.h"

typedef struct qthread_queue_s qthread_queue_t;

struct qthread_lock_s
{
    qthread_queue_t *waiting;
    qthread_shepherd_t *creator_ptr;
#ifdef QTHREAD_DEBUG
    unsigned owner;
#endif
    QTHREAD_FASTLOCK_TYPE lock;
#ifdef QTHREAD_LOCK_PROFILING
    qtimer_t hold_timer;
#endif
};

typedef struct qthread_addrres_s
{
    aligned_t *addr;		/* ptr to the memory NOT being blocked on */
    qthread_t *waiter;
    qthread_shepherd_t *creator_ptr;
    struct qthread_addrres_s *next;
} qthread_addrres_t;

typedef struct qthread_addrstat_s
{
    QTHREAD_FASTLOCK_TYPE lock;
    qthread_addrres_t *EFQ;
    qthread_addrres_t *FEQ;
    qthread_addrres_t *FFQ;
    qthread_shepherd_t *creator_ptr;
#ifdef QTHREAD_LOCK_PROFILING
    qtimer_t empty_timer;
#endif
    unsigned int full:1;
} qthread_addrstat_t;

#ifdef UNPOOLED
# define UNPOOLED_ADDRSTAT
# define UNPOOLED_ADDRRES
#endif

#ifdef UNPOOLED_ADDRSTAT
#define ALLOC_ADDRSTAT(shep) (qthread_addrstat_t *) malloc(sizeof(qthread_addrstat_t))
#define FREE_ADDRSTAT(t) free(t)
#else
extern qt_mpool generic_addrstat_pool;
static QINLINE qthread_addrstat_t *ALLOC_ADDRSTAT(qthread_shepherd_t * shep)
{				       /*{{{ */
    qthread_addrstat_t *tmp =
	(qthread_addrstat_t *) qt_mpool_alloc(shep ? (shep->addrstat_pool) :
					      generic_addrstat_pool);
    if (tmp != NULL) {
	tmp->creator_ptr = shep;
    }
    return tmp;
}				       /*}}} */

static QINLINE void FREE_ADDRSTAT(qthread_addrstat_t * t)
{				       /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->addrstat_pool) :
		  generic_addrstat_pool, t);
}				       /*}}} */
#endif

#ifdef UNPOOLED_ADDRRES
#define ALLOC_ADDRRES(shep) (qthread_addrres_t *) malloc(sizeof(qthread_addrres_t))
#define FREE_ADDRRES(t) free(t)
#else
static QINLINE qthread_addrres_t *ALLOC_ADDRRES(qthread_shepherd_t * shep)
{				       /*{{{ */
    qthread_addrres_t *tmp =
	(qthread_addrres_t *) qt_mpool_alloc(shep->addrres_pool);
    if (tmp != NULL) {
	tmp->creator_ptr = shep;
    }
    return tmp;
}				       /*}}} */

static QINLINE void FREE_ADDRRES(qthread_addrres_t * t)
{				       /*{{{ */
    qt_mpool_free(t->creator_ptr->addrres_pool, t);
}				       /*}}} */
#endif

/* This allocates a new, initialized addrstat structure, which is used for
 * keeping track of the FEB status of an address. It expects a shepherd pointer
 * to use to find the right memory pool to use. */
static QINLINE qthread_addrstat_t *qthread_addrstat_new(qthread_shepherd_t *
							shepherd)
{				       /*{{{ */
    qthread_addrstat_t *ret = ALLOC_ADDRSTAT(shepherd);

    if (ret != NULL) {
	QTHREAD_FASTLOCK_INIT(ret->lock);
	ret->full = 1;
	ret->EFQ = NULL;
	ret->FEQ = NULL;
	ret->FFQ = NULL;
	QTHREAD_EMPTY_TIMER_INIT(ret);
    }
    return ret;
}				       /*}}} */

/* this function is for maintenance of the FEB hashtables. SHOULD only be
 * necessary for things left over when qthread_finalize is called */
static void qthread_addrstat_delete(qthread_addrstat_t * m)
{				       /*{{{ */
#ifdef QTHREAD_LOCK_PROFILING
    qtimer_destroy(m->empty_timer);
#endif
    QTHREAD_FASTLOCK_DESTROY(m->lock);
    FREE_ADDRSTAT(m);
}				       /*}}} */

