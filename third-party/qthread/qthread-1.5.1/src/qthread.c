#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>		       /* for malloc() and abort() */
#ifdef HAVE_MALLOC_H
# include <malloc.h>		       /* for memalign() */
#endif
#include <limits.h>		       /* for INT_MAX */
#include <qthread/qthread-int.h>       /* for UINT8_MAX */
#include <string.h>		       /* for memset() */
#if !HAVE_MEMCPY
# define memcpy(d, s, n) bcopy((s), (d), (n))
# define memmove(d, s, n) bcopy((s), (d), (n))
#endif
#include <sys/time.h>
#include <sys/resource.h>
#if (defined(QTHREAD_SHEPHERD_PROFILING) || defined(QTHREAD_LOCK_PROFILING))
# include <qthread/qtimer.h>
#endif
#ifdef QTHREAD_USE_PTHREADS
# include <pthread.h>
#endif
#ifdef HAVE_SCHED_H
# include <sched.h>
#endif
#ifdef QTHREAD_USE_VALGRIND
# include <valgrind/memcheck.h>
#endif
#ifdef QTHREAD_GUARD_PAGES
#include <sys/types.h>
#include <sys/mman.h>
#endif
#include <errno.h>
#ifdef SST
# include <ppcPimCalls.h>
#endif
#ifdef HAVE_SYSCTL
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# ifdef HAVE_SYS_SYSCTL_H
#  include <sys/sysctl.h>
# endif
#endif
#if defined(HAVE_SYSCONF) && ! defined(QTHREAD_HAVE_MACHTOPO) && defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif

#include "qt_mpool.h"
#include "qt_atomics.h"

#include "qthread/qthread.h"
#include "qthread/futurelib.h"
#include "qthread_innards.h"
#include "qthread_prefetch.h"
#include "futurelib_innards.h"
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# include "qt_barrier.h"
# include "qthread/feb_barrier.h"   /* for barrier in parallel region defination */
# include "qt_arrive_first.h"
#endif

#ifndef QTHREAD_NOALIGNCHECK
#define QALIGN(d, s) do { \
    s = (aligned_t *) (((size_t) d) & (~(sizeof(aligned_t)-1))); \
    if (s != d) { \
	fprintf(stderr, \
		"WARNING: %s(): unaligned address %p ... assuming %p\n", \
		__FUNCTION__, (void *) d, (void *) s); \
    } \
} while(0)
#else /* QTHREAD_NOALIGNCHECK */
#define QALIGN(d, s) (s)=(d)
#endif

#if !(defined(HAVE_GCC_INLINE_ASSEMBLY) && \
    (QTHREAD_SIZEOF_ALIGNED_T == 4 || \
     (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32 && \
      QTHREAD_ASSEMBLY_ARCH != QTHREAD_SPARCV9_32))) && \
    ! defined(QTHREAD_MUTEX_INCREMENT)
# warn QTHREAD_MUTEX_INCREMENT not defined. It probably should be.
# define QTHREAD_MUTEX_INCREMENT 1
#endif

/* internal data structures */
typedef struct qthread_lock_s qthread_lock_t;
#ifndef QTHREAD_SHEPHERD_TYPEDEF
#define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

#include "qt_qthread_struct.h"

struct qthread_queue_s
{
    qthread_t *head;
    qthread_t *tail;
    qthread_shepherd_t *creator_ptr;
    pthread_mutex_t lock;
    pthread_cond_t notempty;
};

#include "qt_shepherd_innards.h"
#include "qt_blocking_structs.h"
#include "qt_threadqueues.h"
#include "qt_affinity.h"

pthread_key_t shepherd_structs;

/* shared globals (w/ futurelib) */
qlib_t qlib = NULL;
int qaffinity = 1;

struct qt_cleanup_funcs_s {
    void(*func)(void);
    struct qt_cleanup_funcs_s *next;
} *qt_cleanup_funcs = NULL;

#ifdef QTHREAD_COUNT_THREADS
static aligned_t threadcount;
static aligned_t maxconcurrentthreads;
static double avg_concurrent_threads;
static aligned_t avg_count;
static aligned_t concurrentthreads;
static QTHREAD_FASTLOCK_TYPE concurrentthreads_lock;

#define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) qthread_internal_incr(&qlib->TYPE##_stripes[(BIN)], &qlib->TYPE##_stripes_locks[(BIN)], 1)
#else
#define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) do{ }while(0)
#endif

/* Internal functions */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high, unsigned int low);
#else
static void qthread_wrapper(void *ptr);
#endif

static QINLINE void qthread_makecontext(ucontext_t * const, void * const, const size_t,
					void (*)(void), const void * const,
					ucontext_t * const);
static QINLINE qthread_t *qthread_thread_new(const qthread_f f,
					     const void *arg,
					     size_t arg_size,
					     void * ret,
					     const qthread_shepherd_id_t shepherd);
static QINLINE void qthread_thread_free(qthread_t * t);
static qthread_shepherd_t* qthread_find_active_shepherd(qthread_shepherd_id_t *l, unsigned int *d);


static QINLINE qthread_queue_t *qthread_queue_new(qthread_shepherd_t *
				  shepherd);
static QINLINE void qthread_queue_free(qthread_queue_t * q);
static QINLINE void qthread_enqueue(qthread_queue_t * q, qthread_t * t);

static QINLINE qthread_t *qthread_dequeue(qthread_queue_t * q);

static QINLINE void qthread_exec(qthread_t * t, ucontext_t * c);
static QINLINE void qthread_gotlock_fill(qthread_shepherd_t * shep,
					 qthread_addrstat_t * m, void *maddr,
					 const char recursive);
static QINLINE void qthread_gotlock_empty(qthread_shepherd_t * shep,
					  qthread_addrstat_t * m, void *maddr,
					  const char recursive);

#define QTHREAD_INITLOCK(l) do { if (pthread_mutex_init(l, NULL) != 0) { return QTHREAD_PTHREAD_ERROR; } } while(0)
#define QTHREAD_LOCK(l) qassert(pthread_mutex_lock(l), 0)
#define QTHREAD_UNLOCK(l) qassert(pthread_mutex_unlock(l), 0)
//#define QTHREAD_DESTROYLOCK(l) do { int __ret__ = pthread_mutex_destroy(l); if (__ret__ != 0) fprintf(stderr, "pthread_mutex_destroy(%p) returned %i (%s)\n", l, __ret__, strerror(__ret__)); assert(__ret__ == 0); } while (0)
#define QTHREAD_DESTROYLOCK(l) qassert(pthread_mutex_destroy(l), 0)
#define QTHREAD_DESTROYCOND(l) qassert(pthread_cond_destroy(l), 0)
#define QTHREAD_SIGNAL(l) qassert(pthread_cond_signal(l), 0)
#define QTHREAD_CONDWAIT(c, l) qassert(pthread_cond_wait(c, l), 0)

#if defined(UNPOOLED_QTHREAD_T) || defined(UNPOOLED)
#define ALLOC_QTHREAD(shep) (qthread_t *) malloc(sizeof(qthread_t))
#define FREE_QTHREAD(t) free(t)
#else
static qt_mpool generic_qthread_pool = NULL;
static QINLINE qthread_t *ALLOC_QTHREAD(qthread_shepherd_t * shep)
{				       /*{{{ */
    qthread_t *tmp =
	(qthread_t *) qt_mpool_alloc(shep ? (shep->qthread_pool) :
				     generic_qthread_pool);
    if (tmp != NULL) {
	tmp->creator_ptr = shep;
    }
    return tmp;
}				       /*}}} */

static QINLINE void FREE_QTHREAD(qthread_t * t)
{				       /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->qthread_pool) :
		  generic_qthread_pool, t);
}				       /*}}} */
#endif

#if defined(UNPOOLED_STACKS) || defined(UNPOOLED)
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(qthread_shepherd_t * shep)
{				       /*{{{ */
    char *tmp = valloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()));

    assert(tmp != NULL);
    if (tmp == NULL) {
	return NULL;
    }
    if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
	perror("mprotect in ALLOC_STACK (1)");
    }
    if (mprotect
	(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
	 PROT_NONE) != 0) {
	perror("mprotect in ALLOC_STACK (2)");
    }
    return tmp + getpagesize();
}				       /*}}} */

static QINLINE void FREE_STACK(qthread_shepherd_t * shep, void *t)
{				       /*{{{ */
    char *tmp = t;

    assert(t);
    tmp -= getpagesize();
    if (mprotect(tmp, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
	perror("mprotect in FREE_STACK (1)");
    }
    if (mprotect
	(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
	 PROT_READ | PROT_WRITE) != 0) {
	perror("mprotect in FREE_STACK (2)");
    }
    free(tmp);
}				       /*}}} */
# else
#  define ALLOC_STACK(shep) malloc(qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s))
#  define FREE_STACK(shep, t) free(t)
# endif
#else
static qt_mpool generic_stack_pool = NULL;
# ifdef QTHREAD_GUARD_PAGES
static QINLINE void *ALLOC_STACK(qthread_shepherd_t * shep)
{				       /*{{{ */
    char *tmp =
	qt_mpool_alloc(shep ? (shep->stack_pool) : generic_stack_pool);
    assert(tmp);
    if (tmp == NULL) {
	return NULL;
    }
    if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
	perror("mprotect in ALLOC_STACK (1)");
    }
    if (mprotect
	(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
	 PROT_NONE) != 0) {
	perror("mprotect in ALLOC_STACK (2)");
    }
    return tmp + getpagesize();
}				       /*}}} */

static QINLINE void FREE_STACK(qthread_shepherd_t * shep, void *t)
{				       /*{{{ */
    char *tmp = t;

    assert(t);
    tmp -= getpagesize();
    if (mprotect(tmp, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
	perror("mprotect in FREE_STACK (1)");
    }
    if (mprotect
	(tmp + qlib->qthread_stack_size + getpagesize(), getpagesize(),
	 PROT_READ | PROT_WRITE) != 0) {
	perror("mprotect in FREE_STACK (2)");
    }
    qt_mpool_free(shep ? (shep->stack_pool) : generic_stack_pool, tmp);
}				       /*}}} */
# else
#  define ALLOC_STACK(shep) qt_mpool_alloc(shep?(shep->stack_pool):generic_stack_pool)
#  define FREE_STACK(shep, t) qt_mpool_free(shep?(shep->stack_pool):generic_stack_pool, t)
# endif
#endif

#if defined(UNPOOLED_QUEUES) || defined(UNPOOLED)
# define ALLOC_QUEUE(shep) (qthread_queue_t *) malloc(sizeof(qthread_queue_t))
# define FREE_QUEUE(t) free(t)
#else
static qt_mpool generic_queue_pool = NULL;
qt_mpool generic_threadqueue_pool = NULL;
qt_mpool generic_threadqueue_node_pool = NULL;
static QINLINE qthread_queue_t *ALLOC_QUEUE(qthread_shepherd_t * shep)
{				       /*{{{ */
    qthread_queue_t *tmp =
	(qthread_queue_t *) qt_mpool_alloc(shep ? (shep->queue_pool) :
					   generic_queue_pool);
    if (tmp != NULL) {
	tmp->creator_ptr = shep;
    }
    return tmp;
}				       /*}}} */

static QINLINE void FREE_QUEUE(qthread_queue_t * t)
{				       /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->queue_pool) :
		  generic_queue_pool, t);
}				       /*}}} */

#endif

#if defined(UNPOOLED_LOCKS) || defined(UNPOOLED)
#define ALLOC_LOCK(shep) (qthread_lock_t *) malloc(sizeof(qthread_lock_t))
#define FREE_LOCK(t) free(t)
#else
static qt_mpool generic_lock_pool = NULL;
static QINLINE qthread_lock_t *ALLOC_LOCK(qthread_shepherd_t * shep)
{				       /*{{{ */
    qthread_lock_t *tmp =
	(qthread_lock_t *) qt_mpool_alloc(shep ? (shep->lock_pool) :
					  generic_lock_pool);
    if (tmp != NULL) {
	tmp->creator_ptr = shep;
    }
    return tmp;
}				       /*}}} */

static QINLINE void FREE_LOCK(qthread_lock_t * t)
{				       /*{{{ */
    qt_mpool_free(t->creator_ptr ? (t->creator_ptr->lock_pool) :
		  generic_lock_pool, t);
}				       /*}}} */
#endif

#if ! defined(UNPOOLED_ADDRSTAT) && ! defined(UNPOOLED)
qt_mpool generic_addrstat_pool = NULL;
#endif

/* guaranteed to be between 0 and 128, using the first parts of addr that are
 * significant */
unsigned int QTHREAD_LOCKING_STRIPES = 128;

#ifdef QTHREAD_DEBUG
enum qthread_debug_levels debuglevel = 0;
QTHREAD_FASTLOCK_TYPE output_lock;

int qthread_debuglevel(int d)
{
    if (d >= 0) debuglevel = d;
    return debuglevel;
}
#else
int qthread_debuglevel(int d)
{
    return 0;
}
#endif

#include "qt_profiling.h"

#ifndef QTHREAD_NO_ASSERTS
void * shep0arg = NULL;
#endif
/* the qthread_shepherd() is the pthread responsible for actually
 * executing the work units
 *
 * this function is the workhorse of the library: this is the function that
 * gets spawned several times and runs all the qthreads. */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void *qthread_shepherd(void *arg);
static void *qthread_shepherd_wrapper(unsigned int high, unsigned int low)
{				       /*{{{ */
    qthread_shepherd_t *me =
	(qthread_shepherd_t *) ((((uintptr_t) high) << 32) | low);
    qthread_debug(ALL_DETAILS, "high(%x), low(%x): me = %p\n",
		  high, low, me);
    return qthread_shepherd(me);
}
#endif

/* pulled from qthread_shepherd in case needed by stealing/run specific code */
static QINLINE void alloc_rdata(qthread_shepherd_t *me, qthread_t *t)
{
  void *stack = ALLOC_STACK(me);
  assert(stack);
#ifdef QTHREAD_GUARD_PAGES
  t->rdata = (struct qthread_runtime_data_s*)(((char*)stack) + getpagesize() + qlib->qthread_stack_size);
#else
  t->rdata = (struct qthread_runtime_data_s*)(((char*)stack) + qlib->qthread_stack_size);
#endif
  t->rdata->stack = stack;
  t->rdata->shepherd_ptr = me;
  t->rdata->blockedon = NULL;
#ifdef QTHREAD_USE_VALGRIND
  t->rdata->valgrind_stack_id = VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
#endif
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
  t->rdata->openmpTaskRetVar = NULL;
  //	t->rdata->taskWaitLock.u.w = 0;
  qthread_syncvar_empty(&t->rdata->taskWaitLock);

#endif
}

static void *qthread_shepherd(void *arg)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *me_worker = (qthread_worker_t *) arg;
    qthread_shepherd_t *me = (qthread_shepherd_t *) me_worker->shepherd;
#else
    qthread_shepherd_t *me = (qthread_shepherd_t *) arg;
#endif
    ucontext_t my_context;
    qthread_t *t;
    int done = 0;

#ifdef QTHREAD_SHEPHERD_PROFILING
    me->total_time = qtimer_create();
    qtimer_t idle = qtimer_create();
#endif

    qthread_debug(ALL_DETAILS, "alive! me = %p\n", me);
    assert(me != NULL);
    assert(me->shepherd_id <= qlib->nshepherds);
    qthread_debug(ALL_FUNCTIONS, "id(%u): forked with arg %p\n",
		  me->shepherd_id, arg);
#ifndef QTHREAD_NO_ASSERTS
    if (shep0arg != NULL && me->shepherd_id == 0) {
	if (arg != shep0arg) {
	    fprintf(stderr, "arg = %p, shep0arg = %p\n", arg, shep0arg);
	}
	assert(arg == shep0arg);
	shep0arg = NULL;
    }
#endif

    /* Initialize myself */
    pthread_setspecific(shepherd_structs, arg);

    if (qaffinity & (me->node != -1)) {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	qt_affinity_set(me_worker);
#else
	qt_affinity_set(me);
#endif
    }
    /* workhorse loop */
    while (!done) {
#ifdef QTHREAD_SHEPHERD_PROFILING
	qtimer_start(idle);
#endif
	qthread_debug(ALL_DETAILS, "id(%i): fetching a thread from my queue...\n", me->shepherd_id);
	assert(me->ready);
	t = qt_threadqueue_dequeue_blocking(me->ready QMS_ARG(me_worker->active));
	assert(t);
#ifdef QTHREAD_SHEPHERD_PROFILING
	qtimer_stop(idle);
	me->idle_count++;
	me->idle_time += qtimer_secs(idle);
	if (me->idle_maxtime < qtimer_secs(idle)) {
	    me->idle_maxtime = qtimer_secs(idle);
	}
#endif

	qthread_debug(THREAD_DETAILS,
		      "id(%u): dequeued thread %p: id %d/state %d\n",
		      me->shepherd_id, t, t->thread_id, t->thread_state);

	if (t->thread_state == QTHREAD_STATE_TERM_SHEP) {
#ifdef QTHREAD_SHEPHERD_PROFILING
	    qtimer_stop(me->total_time);
#endif
	    done = 1;
	    qthread_thread_free(t);
	} else {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	  if(!*(volatile size_t*)&me_worker->active){
	    qt_threadqueue_enqueue(me->ready,t,me);
	    // short stall to prevent worker from monoplizing queue
	    struct timespec req={0};
	    req.tv_sec = 0;
	    req.tv_nsec= 10000000L;
	    nanosleep(&req,&req);
	  }
	  else {
#endif
	    /* yielded only happens for the first thread */
	    assert((t->thread_state == QTHREAD_STATE_NEW) ||
		   (t->thread_state == QTHREAD_STATE_RUNNING) ||
		   (t->thread_state == QTHREAD_STATE_YIELDED &&
		    t->flags & QTHREAD_REAL_MCCOY));

	    assert(t->f != NULL || t->flags & QTHREAD_REAL_MCCOY);
	    if (t->rdata == NULL) alloc_rdata(me, t);
	    assert(t->rdata->shepherd_ptr != NULL);
	    if (t->rdata->shepherd_ptr != me) {
		//fprintf(stderr, "shepherd_ptr = %p, me = %p\n", t->rdata->shepherd_ptr, me);
		fflush(stderr);
		t->rdata->shepherd_ptr = me;
	    }
	    assert(t->rdata->shepherd_ptr == me);

	    if ((t->target_shepherd != NULL) && (t->target_shepherd != me) &&
		QTHREAD_CASLOCK_READ_UI(t->target_shepherd->active)) {
		/* send this thread home */
		qthread_debug(THREAD_DETAILS,
			      "id(%u): thread %u going back home to shep %u\n",
			      me->shepherd_id, t->thread_id,
			      t->target_shepherd->shepherd_id);
		t->rdata->shepherd_ptr = t->target_shepherd;
		assert(t->rdata->shepherd_ptr->ready != NULL);
		qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
	    } else if (!QTHREAD_CASLOCK_READ_UI(me->active)) {
		qthread_debug(ALL_DETAILS,
			      "id(%u): skipping thread exec because I've been disabled!\n",
			      me->shepherd_id);
		if (t->target_shepherd == NULL || t->target_shepherd == me) {
		    /* send to the closest shepherd */
		    t->rdata->shepherd_ptr =
			qthread_find_active_shepherd(me->sorted_sheplist,
						     me->shep_dists);
		} else {
		    /* find a shepherd somewhere near the preferred shepherd
		     *
		     * Note: if the preferred shep was active, we would have sent
		     * this thread home above */
		    t->rdata->shepherd_ptr =
			qthread_find_active_shepherd(t->target_shepherd->
						     sorted_sheplist,
						     t->target_shepherd->
						     shep_dists);
		}
		assert(t->rdata->shepherd_ptr);
		if (t->rdata->shepherd_ptr == NULL) {
		    qthread_debug(THREAD_DETAILS,
				  "id(%u): a new home for thread %i could not be found!\n",
				  me->shepherd_id, t->thread_id);
		    t->rdata->shepherd_ptr = me;
		}
		qthread_debug(THREAD_DETAILS,
			      "id(%u): rescheduling thread %i on %i\n",
			      me->shepherd_id, t->thread_id,
			      t->rdata->shepherd_ptr->shepherd_id);
		assert(t->rdata->shepherd_ptr->ready != NULL);
		qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
	    } else {		       /* me->active */
#ifdef QTHREAD_SHEPHERD_PROFILING
		if (t->thread_state == QTHREAD_STATE_NEW) {
		    me->num_threads++;
		}
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
		me_worker->current = t;
#else
		me->current = t;
#endif
		getcontext(&my_context);
		qthread_debug(THREAD_DETAILS, "id(%u): shepherd context is %p\n", me->shepherd_id, &my_context);
		/* note: there's a good argument that the following should
		 * be: (*t->f)(t), however the state management would be
		 * more complex
		 */
		qthread_exec(t, &my_context);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
		me_worker->current = NULL;
#else
		me->current = NULL;
#endif
		qthread_debug(ALL_DETAILS,
			      "id(%u): back from qthread_exec\n",
			      me->shepherd_id);
		/* now clean up, based on the thread's state */
		switch (t->thread_state) {
		    case QTHREAD_STATE_MIGRATING:
			qthread_debug(THREAD_DETAILS,
				      "id(%u): thread %u migrating to shep %u\n",
				      me->shepherd_id, t->thread_id,
				      t->target_shepherd->shepherd_id);
			t->thread_state = QTHREAD_STATE_RUNNING;
			t->rdata->shepherd_ptr = t->target_shepherd;
			assert(t->rdata->shepherd_ptr->ready != NULL);
			qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t, me);
			break;
		    default:
			qthread_debug(THREAD_DETAILS, "id(%u): thread in state %i; that's illegal!\n", me->shepherd_id, t->thread_state);
			assert(0);
		    case QTHREAD_STATE_YIELDED:	/* reschedule it */
			t->thread_state = QTHREAD_STATE_RUNNING;
			qthread_debug(THREAD_DETAILS,
				      "id(%u): thread %i yielded; rescheduling\n",
				      me->shepherd_id, t->thread_id);
			assert(me->ready != NULL);
			qt_threadqueue_enqueue_yielded(me->ready, t, me);
			break;

		    case QTHREAD_STATE_FEB_BLOCKED:	/* unlock the related FEB address locks, and re-arrange memory to be correct */
			qthread_debug(LOCK_DETAILS,
				      "id(%u): thread %i blocked on FEB\n",
				      me->shepherd_id, t->thread_id);
			t->thread_state = QTHREAD_STATE_BLOCKED;
			QTHREAD_FASTLOCK_UNLOCK(&
				       (((qthread_addrstat_t *) (t->
								 rdata->blockedon))->
					lock));
			break;

		    case QTHREAD_STATE_BLOCKED:	/* put it in the blocked queue */
			qthread_debug(LOCK_DETAILS,
				      "id(%u): thread %i blocked on LOCK\n",
				      me->shepherd_id, t->thread_id);
			qthread_enqueue((qthread_queue_t *) t->rdata->blockedon->
					waiting, t);
			QTHREAD_FASTLOCK_UNLOCK(&(t->rdata->blockedon->lock));
			break;

		    case QTHREAD_STATE_TERMINATED:
			qthread_debug(THREAD_DETAILS,
				      "id(%u): thread %i terminated\n",
				      me->shepherd_id, t->thread_id);
			t->thread_state = QTHREAD_STATE_DONE;
			/* we can remove the stack and the context... */
			qthread_thread_free(t);
			break;
		}
	    }
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	  }
#endif
	}
    }

#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_destroy(idle);
#endif
    qthread_debug(ALL_DETAILS, "id(%u): finished\n",
		  me->shepherd_id);
    pthread_exit(NULL);
    return NULL;
}				       /*}}} */

static qthread_shepherd_t *qthread_find_active_shepherd(qthread_shepherd_id_t
							* l, unsigned int *d)
{				       /*{{{ */
    qthread_shepherd_id_t target = 0;
    qthread_shepherd_t *sheps = qlib->shepherds;
    const qthread_shepherd_id_t nsheps =
	(qthread_shepherd_id_t) qlib->nshepherds;

    qthread_debug(ALL_FUNCTIONS, "l(%p): from %i sheps\n", l, (int)nsheps);
    if (l == NULL) {
	/* if l==NULL, there's no locality info, so just find the least-busy active shepherd */
	saligned_t busyness = 0;
	int found = 0;

	for (size_t i = 0; i < nsheps; i++) {
	    if (QTHREAD_CASLOCK_READ_UI(sheps[i].active)) {
		ssize_t shep_busy_level = 
		    qthread_internal_atomic_read_s(&sheps[i].ready->advisory_queuelen,
			    &sheps[i].ready->advisory_queuelen_m);

		if (found == 0) {
		    found = 1;
		    qthread_debug(ALL_FUNCTIONS,
				  "l(%p): shep %i is the least busy (%i) so far\n",
				  l, (int)i, shep_busy_level);
		    busyness = shep_busy_level;
		    target = i;
		} else if (shep_busy_level < busyness ||
			   (shep_busy_level == busyness &&
			    random() % 2 == 0)) {
		    qthread_debug(ALL_FUNCTIONS,
				  "l(%p): shep %i is the least busy (%i) so far\n",
				  l, (int)i, shep_busy_level);
		    busyness = shep_busy_level;
		    target = i;
		}
	    }
	}
	assert(found);
	if (found == 0) {
	    qthread_debug(ALL_FUNCTIONS,
			  "l(%p): DID NOT FIND ANY ACTIVE SHEPHERDS!!!\n",
			  l);
	    return NULL;
	} else {
	    qthread_debug(ALL_FUNCTIONS,
			  "l(%p): found bored target %i\n",
			  l, (int)target);
	    return &(sheps[target]);
	}
    } else {
	/* if we have locality info, use it to identify the closest shepherd(s)
	 * and if there's more than one that is equidistant, pick the least busy
	 */
	qthread_shepherd_id_t alt;
	saligned_t busyness;

	while (target < (nsheps-1) && QTHREAD_CASLOCK_READ_UI(sheps[l[target]].active) == 0) {
	    target++;
	}
	if (target >= (nsheps-1)) {
	    return NULL;
	}
	qthread_debug(ALL_FUNCTIONS,
		      "l(%p): nearest active shepherd (%i) is %i away\n",
		      l, (int)l[target], (int)d[l[target]]);
	busyness =
	    qthread_internal_atomic_read_s(&sheps[l[target]].ready->advisory_queuelen,
		    &sheps[l[target]].ready->advisory_queuelen_m);
	for (alt = target + 1; alt < (nsheps-1) && d[l[alt]] == d[l[target]];
	     alt++) {
	    saligned_t shep_busy_level =
		qthread_internal_atomic_read_s(&sheps[l[alt]].ready->advisory_queuelen,
			&sheps[l[alt]].ready->advisory_queuelen_m);
	    if (shep_busy_level < busyness ||
		(shep_busy_level == busyness && random() % 2 == 0)) {
		qthread_debug(ALL_FUNCTIONS,
			      "l(%p): shep %i is the least busy (%i) so far\n",
			      l, (int)d[l[alt]], shep_busy_level);
		busyness = shep_busy_level;
		target = alt;
	    }
	}
	qthread_debug(ALL_FUNCTIONS,
		      "l(%p): found target %i\n",
		      l, (int)target);
	return &(sheps[l[target]]);
    }
}				       /*}}} */

int qthread_init(qthread_shepherd_id_t nshepherds)
{				       /*{{{ */
    char newenv[100] = {0};
    snprintf(newenv, 99, "QTHREAD_NUM_SHEPHERDS=%i", (int)nshepherds);
    putenv(newenv);
    return qthread_initialize();
}				       /*}}} */


int qthread_initialize(void)
{				       /*{{{ */
    int r;
    size_t i;
    int need_sync = 1;
    qthread_shepherd_id_t nshepherds = 0;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_id_t nworkerspershep = 0;
#endif

#ifdef QTHREAD_DEBUG
    QTHREAD_FASTLOCK_INIT(output_lock);
    {
	char *qdl = getenv("QTHREAD_DEBUG_LEVEL");
	char *qdle = NULL;

	if (qdl) {
	    debuglevel = strtol(qdl, &qdle, 0);
	    if (qdle == NULL || qdle == qdl) {
		fprintf(stderr, "unparseable debug level (%s)\n", qdl);
		debuglevel = 0;
	    }
	} else {
	    debuglevel = 0;
	}
    }
# ifdef SST
    debuglevel = 7;
# endif
#endif

    qthread_debug(ALL_CALLS, "began.\n");
    if (qlib != NULL) {
	qthread_debug(ALL_DETAILS, "redundant call\n");
	return QTHREAD_SUCCESS;
    }
    qlib = (qlib_t) malloc(sizeof(struct qlib_s));
    qassert_ret(qlib, QTHREAD_MALLOC_ERROR);

#ifdef QTHREAD_USE_PTHREADS
    {
	char *qsh = getenv("QTHREAD_NUM_SHEPHERDS");
	char *qshe = NULL;

	if (qsh) {
	    nshepherds = strtol(qsh, &qshe, 0);
	    if (qshe == NULL || qshe == qsh) {
		fprintf(stderr, "unparsable number of shepherds (%s)\n", qsh);
		nshepherds = 0;
	    } else if (nshepherds > 0) {
		fprintf(stderr, "Forced %i Shepherds\n", (int)nshepherds);
	    }
	} else {
	    nshepherds = 0;
	}
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	qsh = getenv("QTHREAD_NUM_WORKERS_PER_SHEPHERD");
	qshe = NULL;

	if (qsh) {
	    nworkerspershep = strtol(qsh, &qshe, 0);
	    if (qshe == NULL || qshe == qsh) {
		fprintf(stderr, "unparsable number of workers (%s)\n", qsh);
		nworkerspershep = 1;
	    } else if (nworkerspershep > 0) {
		fprintf(stderr, "Forced %i Workers per Shepherd\n", (int)nworkerspershep);
		if (nshepherds == 0) {
		    fprintf(stderr, "Number of shepherds not specified - number of workers may be ignored\n");
		}
	    }
	}
#endif
    }
    qt_affinity_init(&nshepherds
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	    , &nworkerspershep
#endif
	    );

    if (nshepherds == 1
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        && nworkerspershep == 1
#endif
       ) {
	need_sync = 0;
    }
#else /* i.e. not using pthreads aka all serial. */
    nshepherds = 1;
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    nworkerspershep = 1;
# endif
    need_sync = 0;
#endif
    qthread_debug(THREAD_BEHAVIOR,"there will be %u shepherd(s)\n", (unsigned)nshepherds);

    QTHREAD_LOCKING_STRIPES = 1<<((unsigned int)(log2(nshepherds)) + 1);

#ifdef QTHREAD_COUNT_THREADS
    threadcount = 0;
    maxconcurrentthreads = 0;
    concurrentthreads = 0;
    avg_concurrent_threads = 0;
    avg_count = 0;
    QTHREAD_FASTLOCK_INIT(concurrentthreads_lock);
#endif

    /* initialize the FEB-like locking structures */
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    qlib->atomic_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->atomic_locks, QTHREAD_MALLOC_ERROR);
#endif
#ifdef QTHREAD_COUNT_THREADS
    qlib->locks_stripes = malloc(sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks_stripes, QTHREAD_MALLOC_ERROR);
    qlib->febs_stripes = malloc(sizeof(aligned_t) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes, QTHREAD_MALLOC_ERROR);
# ifdef QTHREAD_MUTEX_INCREMENT
    qlib->locks_stripes_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks_stripes_locks, QTHREAD_MALLOC_ERROR);
    qlib->febs_stripes_locks = malloc(sizeof(QTHREAD_FASTLOCK_TYPE) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->febs_stripes_locks, QTHREAD_MALLOC_ERROR);
# endif
#endif
    qlib->locks = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->locks, QTHREAD_MALLOC_ERROR);
    qlib->FEBs = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->FEBs, QTHREAD_MALLOC_ERROR);
    qlib->syncvars = malloc(sizeof(qt_hash) * QTHREAD_LOCKING_STRIPES);
    qassert_ret(qlib->syncvars, QTHREAD_MALLOC_ERROR);
    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
	QTHREAD_FASTLOCK_INIT(qlib->atomic_locks[i]);
#endif
#ifdef QTHREAD_COUNT_THREADS
	qlib->locks_stripes[i] = 0;
	qlib->febs_stripes[i] = 0;
# ifdef QTHREAD_MUTEX_INCREMENT
	QTHREAD_FASTLOCK_INIT(qlib->locks_stripes_locks[i]);
	QTHREAD_FASTLOCK_INIT(qlib->febs_stripes_locks[i]);
# endif
#endif
	qlib->locks[i] = qt_hash_create(need_sync);
	qassert_ret(qlib->locks[i], QTHREAD_MALLOC_ERROR);
	qlib->FEBs[i] = qt_hash_create(need_sync);
	qassert_ret(qlib->FEBs[i], QTHREAD_MALLOC_ERROR);
	qlib->syncvars[i] = qt_hash_create(need_sync);
	qassert_ret(qlib->syncvars[i], QTHREAD_MALLOC_ERROR);
    }

    /* initialize the kernel threads and scheduler */
    qassert(pthread_key_create(&shepherd_structs, NULL), 0);
    qlib->nshepherds = nshepherds;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qlib->nworkerspershep = nworkerspershep;
#endif
    qlib->nshepherds_active = nshepherds;
    qlib->shepherds = (qthread_shepherd_t *)
	 calloc(nshepherds, sizeof(qthread_shepherd_t));
    qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);

    {
	char *stacksize = getenv("QTHREAD_STACK_SIZE");
	size_t ss = 0;

	if (stacksize && *stacksize) {
	    char *eptr;

	    ss = strtoul(stacksize, &eptr, 0);
	    if (*eptr != 0) {
		ss = 0;
	    }
	}
	if (ss != 0) {
	    qlib->qthread_stack_size = ss;
	} else {
	    qlib->qthread_stack_size = QTHREAD_DEFAULT_STACK_SIZE;
	}
	qthread_debug(THREAD_DETAILS, "qthread stack size: %u\n", qlib->qthread_stack_size);
    }
#ifdef QTHREAD_GUARD_PAGES
    {
	size_t pagesize = getpagesize();

	/* round stack size to nearest page */
	if (qlib->qthread_stack_size % pagesize) {
	    qlib->qthread_stack_size +=
		pagesize - (qlib->qthread_stack_size % pagesize);
	}
    }
#endif
    qlib->max_thread_id = 0;
    qlib->sched_shepherd = 0;
    QTHREAD_FASTLOCK_INIT(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->sched_shepherd_lock);
    {
	struct rlimit rlp;

	qassert(getrlimit(RLIMIT_STACK, &rlp), 0);
	qthread_debug(THREAD_DETAILS, "stack sizes ... cur: %u max: %u\n",
		      rlp.rlim_cur, rlp.rlim_max);
	if (rlp.rlim_cur == RLIM_INFINITY) {
	    qlib->master_stack_size = 8 * 1024 * 1024;
	} else {
	    qlib->master_stack_size = (unsigned int)(rlp.rlim_cur);
	}
	qlib->max_stack_size = rlp.rlim_max;

    }

    /* initialize the shepherds as having no affinity */
    for (i = 0; i < nshepherds; i++) {
	qlib->shepherds[i].node = -1;
	qlib->shepherds[i].shep_dists = NULL;
	qlib->shepherds[i].sorted_sheplist = NULL;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	qlib->shepherds[i].workers = (qthread_worker_t *)
	  calloc(nworkerspershep, sizeof(qthread_worker_t));
	qassert_ret(qlib->shepherds[i].workers, QTHREAD_MALLOC_ERROR);
#endif
    }
    {
	char *aff = getenv("QTHREAD_AFFINITY");

	if (aff && !strncmp(aff, "no", 3))
	    qaffinity = 0;
	else
	    qaffinity = 1;
    }
    qthread_debug(ALL_DETAILS, "qaffinity = %i\n", qaffinity);
    if (qaffinity == 1 &&
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
	    nshepherds * nworkerspershep > 1
#else
	    nshepherds > 1
#endif
	    ) {
	int ret = qt_affinity_gendists(qlib->shepherds, nshepherds);
	if (ret != QTHREAD_SUCCESS) {
	    qthread_debug(ALL_DETAILS, "gendists returned non-success (%i)\n", (int)ret);
	    return ret;
	}
    }
#ifndef UNPOOLED
    /* set up the memory pools */
    qthread_debug(ALL_DETAILS, "shepherd pools sync = %i\n", need_sync);
    for (i = 0; i < nshepherds; i++) { /*{{{ */
	/* the following SHOULD only be accessed by one thread at a time, so
	 * should be quite safe unsynchronized. If things fail, though...
	 * resynchronize them and see if that fixes it. */
	qlib->shepherds[i].qthread_pool =
	    qt_mpool_create(need_sync, sizeof(qthread_t));
	qlib->shepherds[i].stack_pool =
#ifdef QTHREAD_GUARD_PAGES
	    qt_mpool_create_aligned(need_sync,
				    qlib->qthread_stack_size +
				    sizeof(struct qthread_runtime_data_s) + (2 * getpagesize()),
				    getpagesize());
#else
	    qt_mpool_create(need_sync, qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s));
#endif
	qlib->shepherds[i].queue_pool =
	    qt_mpool_create(need_sync, sizeof(qthread_queue_t));
	qlib->shepherds[i].threadqueue_pool =
	    qt_mpool_create(need_sync, sizeof(qt_threadqueue_t));
	qlib->shepherds[i].threadqueue_node_pool =
	    qt_mpool_create_aligned(need_sync, sizeof(qt_threadqueue_node_t), 16);
	qlib->shepherds[i].lock_pool =
	    qt_mpool_create(need_sync, sizeof(qthread_lock_t));
	qlib->shepherds[i].addrres_pool =
	    qt_mpool_create(need_sync, sizeof(qthread_addrres_t));
	qlib->shepherds[i].addrstat_pool =
	    qt_mpool_create(need_sync, sizeof(qthread_addrstat_t));
    }				       /*}}} */
    /* these are used when qthread_fork() is called from a non-qthread. */
    generic_qthread_pool = qt_mpool_create(need_sync, sizeof(qthread_t));
    generic_stack_pool =
#ifdef QTHREAD_GUARD_PAGES
	qt_mpool_create_aligned(need_sync,
				qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
				(2 * getpagesize()), getpagesize());
#else
	qt_mpool_create(need_sync, sizeof(struct qthread_runtime_data_s) + qlib->qthread_stack_size);
#endif
    generic_queue_pool =
	qt_mpool_create(need_sync, sizeof(qthread_queue_t));
    generic_threadqueue_pool =
	qt_mpool_create(need_sync, sizeof(qt_threadqueue_t));
    generic_threadqueue_node_pool =
	qt_mpool_create_aligned(need_sync, sizeof(qt_threadqueue_node_t), 16);
    generic_lock_pool =
	qt_mpool_create(need_sync, sizeof(qthread_lock_t));
    generic_addrstat_pool =
	qt_mpool_create(need_sync, sizeof(qthread_addrstat_t));
#endif

    /* initialize the shepherd structures */
    for (i = 0; i < nshepherds; i++) {
	qthread_debug(ALL_DETAILS,
		      "setting up shepherd %i (%p)\n", i,
		      &qlib->shepherds[i]);
	qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t) i;
	QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
	qlib->shepherds[i].ready =
	     qt_threadqueue_new(&(qlib->shepherds[i]));
	qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_LOCK_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
	qlib->shepherds[i].uniqueincraddrs = qt_hash_create(0);
# endif
	qlib->shepherds[i].uniquelockaddrs = qt_hash_create(0);
	qlib->shepherds[i].uniquefebaddrs = qt_hash_create(0);
#endif

	qthread_debug(ALL_DETAILS,
		      "shepherd %i set up (%p)\n", i,
		      &qlib->shepherds[i]);

    }
    qthread_debug(ALL_DETAILS, "done setting up shepherds.\n");

    /* now, transform the current main context into a qthread,
     * and make the main thread a shepherd (shepherd 0).
     * What will happen is this:
     * qlib->mccoy_thread represents the original execution thread, and so will
     * receive a context based on the current execution state.
     * qlib->master_context is a context for the new shepherd that will be
     * created (shepherd 0).
     * qlib->master_stack is a stack for that shepherd, and is huge, because
     * the shepherd expects a "standard" size stack. The mccoy_thread, as it is
     * for the *current* thread, also expects a full-size stack. The point of
     * this weirdness is so that the current thread can block the same way that
     * a qthread can. */
    qthread_debug(ALL_DETAILS, "allocating shep0\n");
    qlib->mccoy_thread = qthread_thread_new(NULL, NULL, 0, NULL, 0);
    qthread_debug(ALL_DETAILS, "mccoy thread = %p\n", qlib->mccoy_thread);
    qassert_ret(qlib->mccoy_thread, QTHREAD_MALLOC_ERROR);

    qthread_debug(ALL_DETAILS, "master_context = %p\n", &(qlib->master_context));
    qlib->master_stack = calloc(1, qlib->master_stack_size);
    qassert_ret(qlib->master_stack, QTHREAD_MALLOC_ERROR);
    qthread_debug(ALL_DETAILS, "master_stack = %p\n", qlib->master_stack);
#ifdef QTHREAD_USE_VALGRIND
    qlib->valgrind_masterstack_id =
	VALGRIND_STACK_REGISTER(qlib->master_stack, qlib->master_stack_size);
#endif

    /* the context will have its own stack ptr */
    qlib->mccoy_thread->thread_state = QTHREAD_STATE_YIELDED;	/* avoid re-launching */
    qlib->mccoy_thread->flags = QTHREAD_REAL_MCCOY | QTHREAD_UNSTEALABLE;	/* i.e. this is THE parent thread */
    assert(qlib->mccoy_thread->rdata == NULL);

    qlib->mccoy_thread->rdata = malloc(sizeof(struct qthread_runtime_data_s));

    assert(qlib->mccoy_thread->rdata != NULL);
    qlib->mccoy_thread->rdata->shepherd_ptr = &(qlib->shepherds[0]);
    qlib->mccoy_thread->rdata->stack = NULL;

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
   qthread_syncvar_empty(&qlib->mccoy_thread->rdata->taskWaitLock);
#endif

    qthread_debug(ALL_DETAILS, "enqueueing mccoy thread\n");
    qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread,
		       &(qlib->shepherds[0]));
    qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
    qassert(getcontext(&(qlib->master_context)), 0);
    /* now build the context for the shepherd 0 */
    qthread_debug(ALL_DETAILS, "calling qthread_makecontext\n");
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qlib->shepherds[0].workers[0].shepherd = &qlib->shepherds[0];
    qlib->shepherds[0].workers[0].active = 1;
    qlib->shepherds[0].workers[0].worker_id = 0;
#endif
    qthread_makecontext(&(qlib->master_context), qlib->master_stack,
			qlib->master_stack_size,
#ifdef QTHREAD_MAKECONTEXT_SPLIT
			(void (*)(void))qthread_shepherd_wrapper,
#else
			(void (*)(void))qthread_shepherd,
#endif
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
			&(qlib->shepherds[0].workers[0]),
#else
			&(qlib->shepherds[0]),
#endif
			&(qlib->mccoy_thread->rdata->context));
#ifndef QTHREAD_NO_ASSERTS
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    shep0arg = &(qlib->shepherds[0].workers[0]);
# else
    shep0arg = &(qlib->shepherds[0]);
# endif
#endif
    /* this launches shepherd 0 */
    qthread_debug(ALL_DETAILS, "launching shepherd 0\n");
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&qlib->mccoy_thread->rdata->context,
				      sizeof(ucontext_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&(qlib->master_context),
				      sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(&qlib->mccoy_thread->rdata->context,
			      sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(&(qlib->master_context), sizeof(ucontext_t));
#endif
    qthread_debug(ALL_DETAILS, "calling swapcontext\n");
    qassert(swapcontext(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)),
	    0);

    /* spawn the shepherds */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < nshepherds; ++i) {
        qthread_worker_id_t j;
	qthread_debug(ALL_DETAILS,
		      "forking workers for shepherd %i (%p)\n", i,
		      &qlib->shepherds[i]);
        for (j = 0; j < nworkerspershep; ++j) {
	    if ((i == 0) && (j == 0)) continue; // original pthread becomes shep 0 worker 0
            qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
            qlib->shepherds[i].workers[j].worker_id = j;
            qlib->shepherds[i].workers[j].packed_worker_id = j+(i*nworkerspershep);
            qlib->shepherds[i].workers[j].active = 1;
	    if ((r =
	        pthread_create(&qlib->shepherds[i].workers[j].worker, NULL,
		    qthread_shepherd, &qlib->shepherds[i].workers[j])) != 0) {
		fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n",
		        r);
	        perror("qthread_init spawning worker");
	        return r;
	    }
	    qthread_debug(ALL_DETAILS, "spawned shep %i worker %i\n", (int)i, (int)j);
        }
    }
    qlib->nworkers_active = nshepherds*nworkerspershep;
#else
    for (i = 1; i < nshepherds; ++i) {
	qthread_debug(ALL_DETAILS,
		      "forking shepherd %i (%p)\n", i,
		      &qlib->shepherds[i]);
	if ((r =
	     pthread_create(&qlib->shepherds[i].shepherd, NULL,
			    qthread_shepherd, &qlib->shepherds[i])) != 0) {
	    fprintf(stderr, "qthread_init: pthread_create() failed (%d)\n",
		    r);
	    perror("qthread_init spawning shepherd");
	    return r;
	}
    }
#endif

    qthread_debug(ALL_DETAILS, "calling atexit\n");
    atexit(qthread_finalize);

    qthread_debug(ALL_DETAILS, "calling component init functions\n");
    qt_feb_barrier_internal_init();
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qt_global_barrier_init(qlib->nshepherds*qlib->nworkerspershep, 0);
    qt_global_arrive_first_init(qthread_num_workers()-1, 0);
# else
    qt_global_barrier_init(nshepherds, 0);
    qt_global_arrive_first_init(nshepherds-1, 0);
# endif
#endif

    qthread_debug(ALL_DETAILS, "finished.\n");
    return QTHREAD_SUCCESS;
}				       /*}}} */

/* This initializes a context (c) to run the function (func) with a single
 * argument (arg). This is just a wrapper around makecontext that isolates some
 * of the portability garbage. */
static QINLINE void qthread_makecontext(ucontext_t * const c, void * const stack,
					const size_t stacksize, void (*func) (void),
					const void * const arg, ucontext_t * const returnc)
{				       /*{{{ */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
    const unsigned int high = ((uintptr_t) arg) >> 32;
    const unsigned int low = ((uintptr_t) arg) & 0xffffffff;
#endif

    /* Several other libraries that do this reserve a few words on either end
     * of the stack for some reason. To avoid problems, I'll also do this (even
     * though I have no idea why they do this). */
#ifdef INVERSE_STACK_POINTER
    c->uc_stack.ss_sp = (char *)(stack) + stacksize - 8;
#else
    c->uc_stack.ss_sp = (char *)(stack) + 8;
#endif
    c->uc_stack.ss_size = stacksize - 64;
#ifdef UCSTACK_HAS_SSFLAGS
    c->uc_stack.ss_flags = 0;
#endif
#ifdef HAVE_NATIVE_MAKECONTEXT
    /* the makecontext man page (Linux) says: set the uc_link FIRST.
     * why? no idea */
    c->uc_link = returnc;	       /* NULL pthread_exit() */
#endif
#ifdef QTHREAD_MAKECONTEXT_SPLIT
# ifdef EXTRA_MAKECONTEXT_ARGC
    makecontext(c, func, 3, high, low);
# else
    makecontext(c, func, 2, high, low);
# endif /* EXTRA_MAKECONTEXT_ARGC */
#else /* QTHREAD_MAKECONTEXT_SPLIT */
# ifdef EXTRA_MAKECONTEXT_ARGC
    makecontext(c, func, 2, arg);
# else
    makecontext(c, func, 1, arg);
# endif /* EXTRA_MAKECONTEXT_ARGC */
#endif /* QTHREAD_MAKECONTEXT_SPLIT */
#ifdef HAVE_NATIVE_MAKECONTEXT
    assert((void*)c->uc_link == (void*)returnc);
#endif
}				       /*}}} */

/* this adds a function to the list of cleanup functions to call at finalize */
void qthread_internal_cleanup(void (*function)(void))
{
    struct qt_cleanup_funcs_s *ng = malloc(sizeof(struct qt_cleanup_funcs_s));
    assert(ng);
    ng->func = function;
    ng->next = qt_cleanup_funcs;
    qt_cleanup_funcs = ng;
}

void qthread_finalize(void)
{				       /*{{{ */
    int r;
    qthread_shepherd_id_t i;
    qthread_t *t;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t* worker;
#endif

    if (qlib == NULL)
	return;

    qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    worker = qthread_internal_getworker();
    if (worker->packed_worker_id != 0) {    /* Only run finalize on shepherd 0 worker 0*/
        worker->current->thread_state = QTHREAD_STATE_YIELDED;  /* Otherwise, put back */
        //	    qt_threadqueue_enqueue(shep0->ready, worker->current,
        //			   shep0);
        return;
    }
#endif

    qthread_debug(ALL_CALLS, "began.\n");

    /* rcm - probably need to put a "turn off the library flag" here, but,
     * the programmer can ensure that no further threads are forked for now
     */

    /* enqueue the termination thread sentinal */
#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_stop(shep0->total_time);
#endif

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < qlib->nshepherds; i++) {
        qthread_worker_id_t j;
	for (j = 0; j < qlib->nworkerspershep; j++) {
	    if (i == 0 && j == 0)
	        continue;   /* None for shepard 0's worker 0 */
	    qthread_debug(ALL_DETAILS, "terminating shepherd %i worker %i\n", (int)i, j);
	    t = qthread_thread_new(NULL, NULL, 0, NULL, i);
	    assert(t != NULL);	       /* what else can we do? */
	    t->thread_state = QTHREAD_STATE_TERM_SHEP;
	    t->thread_id = (unsigned int)-1;
	    qt_threadqueue_enqueue(qlib->shepherds[i].ready, t,
	    	    shep0);
	}
    }
#else
    for (i = 1; i < qlib->nshepherds; i++) {
	qthread_debug(ALL_DETAILS, "terminating shepherd %i\n", (int)i);
	t = qthread_thread_new(NULL, NULL, 0, NULL, i);
	assert(t != NULL);	       /* what else can we do? */
	t->thread_state = QTHREAD_STATE_TERM_SHEP;
	t->thread_id = (unsigned int)-1;
	qt_threadqueue_enqueue(qlib->shepherds[i].ready, t,
			   shep0);
    }
#endif

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_debug(ALL_DETAILS, "destroying the global barrier\n");
    qt_global_barrier_destroy();
#endif
#ifdef QTHREAD_SHEPHERD_PROFILING
    printf
	("QTHREADS: Shepherd 0 spent %f%% of the time idle, handling %lu threads\n",
	 shep0->idle_time / qtimer_secs(shep0->total_time) * 100.0,
	 (unsigned long)shep0->num_threads);
    printf
	("QTHREADS: Shepherd 0 averaged %g secs to find a new thread, max %g secs\n",
	 shep0->idle_time / shep0->idle_count,
	 shep0->idle_maxtime);
    qtimer_destroy(shep0->total_time);
#endif
    /* wait for each SPAWNED shepherd to drain it's queue
     * (note: not shepherd 0, because that one wasn't spawned) */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    for (i = 0; i < qlib->nshepherds; i++) {
        /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker 0 */
        qthread_worker_id_t j;
	    qthread_shepherd_t *shep = &(qlib->shepherds[i]);
	    qthread_debug(ALL_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
        for (j = 0; j < qlib->nworkerspershep; j++) {
	  if (i == 0 && j == 0)
                continue;  /* This leaves out shepard 0's worker 0 */
	    if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
	        fprintf(stderr,
		        "qthread_finalize: pthread_join() of shep %i worker %i failed (%d, or \"%s\")\n",
		        (int)i, (int)j, r, strerror(r));
		fflush(stderr);
	        abort();
	    }
        }
	if (i == 0) continue;
#else
    for (i = 1; i < qlib->nshepherds; i++) {
	qthread_shepherd_t *shep = &(qlib->shepherds[i]);
	qthread_debug(ALL_DETAILS, "waiting for shepherd %i to exit\n", (int)i);
	if ((r = pthread_join(shep->shepherd, NULL)) != 0) {
	    fprintf(stderr,
		    "qthread_finalize: pthread_join() of shep %i failed (%d, or \"%s\")\n",
		    (int)i, r, strerror(r));
	    abort();
	}
#endif
	QTHREAD_CASLOCK_DESTROY(shep->active);
	qt_threadqueue_free(shep->ready);
#ifdef QTHREAD_SHEPHERD_PROFILING
	printf
	    ("QTHREADS: Shepherd %i spent %f%% of the time idle, handling %lu threads\n",
	     i, shep->idle_time / qtimer_secs(shep->total_time) *
	     100.0, (unsigned long)shep->num_threads);
	qtimer_destroy(shep->total_time);
	printf
	    ("QTHREADS: Shepherd %i averaged %g secs to find a new thread, max %g secs\n",
	     i, shep->idle_time / shep->idle_count, shep->idle_maxtime);
#endif
#ifdef QTHREAD_LOCK_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
	QTHREAD_ACCUM_MAX(shep0->incr_maxtime, shep->incr_maxtime);
	shep0->incr_time  += shep->incr_time;
	shep0->incr_count += shep->incr_count;
# endif
	QTHREAD_ACCUM_MAX(shep0->aquirelock_maxtime,
			  shep->aquirelock_maxtime);
	shep0->aquirelock_time += shep->aquirelock_time;
	shep0->aquirelock_count += shep->aquirelock_count;
	QTHREAD_ACCUM_MAX(shep0->lockwait_maxtime,
			  shep->lockwait_maxtime);
	shep0->lockwait_time += shep->lockwait_time;
	shep0->lockwait_count += shep->lockwait_count;
	QTHREAD_ACCUM_MAX(shep0->hold_maxtime, shep->hold_maxtime);
	shep0->hold_time += shep->hold_time;
	QTHREAD_ACCUM_MAX(shep0->febblock_maxtime,
			  shep->febblock_maxtime);
	shep0->febblock_time += shep->febblock_time;
	shep0->febblock_count += shep->febblock_count;
	QTHREAD_ACCUM_MAX(shep0->febwait_maxtime,
			  shep->febwait_maxtime);
	shep0->febwait_time += shep->febwait_time;
	shep0->febwait_count += shep->febwait_count;
	QTHREAD_ACCUM_MAX(shep0->empty_maxtime, shep->empty_maxtime);
	shep0->empty_time += shep->empty_time;
	shep0->empty_count += shep->empty_count;
	qthread_debug(ALL_DETAILS, "destroying hashes\n");
# ifdef QTHREAD_MUTEX_INCREMENT
	qt_hash_callback(shep->uniqueincraddrs,
			 qthread_unique_collect, shep0->uniqueincraddrs);
	qt_hash_destroy(shep->uniqueincraddrs);
# endif
	qt_hash_callback(shep->uniquelockaddrs,
			 qthread_unique_collect, shep0->uniquelockaddrs);
	qt_hash_destroy(shep->uniquelockaddrs);
	qt_hash_callback(shep->uniquefebaddrs,
			 qthread_unique_collect, shep0->uniquefebaddrs);
	qt_hash_destroy(shep->uniquefebaddrs);
#endif
    }
    qthread_debug(ALL_DETAILS, "freeing shep0's threadqueue\n");
    qt_threadqueue_free(shep0->ready);

    qthread_debug(ALL_DETAILS, "calling cleanup functions\n");
    while (qt_cleanup_funcs != NULL) {
	struct qt_cleanup_funcs_s *tmp = qt_cleanup_funcs;
	qt_cleanup_funcs = tmp->next;
	tmp->func();
	free(tmp);
    }

#ifdef QTHREAD_LOCK_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
    printf
	("QTHREADS: %llu increments performed (%ld unique), average %g secs, max %g secs\n",
	 (unsigned long long)shep0->incr_count, qt_hash_count(shep0->uniqueincraddrs),
	 (shep0->incr_count == 0) ? 0 : (shep0->incr_time / shep0->incr_count),
	 shep0->incr_maxtime);
    qt_hash_destroy(shep0->uniqueincraddrs);
# endif
    printf
	("QTHREADS: %llu locks aquired (%ld unique), average %g secs, max %g secs\n",
	 (unsigned long long)shep0->aquirelock_count, qt_hash_count(shep0->uniquelockaddrs),
	 (shep0->aquirelock_count == 0) ? 0 : (shep0->aquirelock_time /
	     shep0->aquirelock_count), shep0->aquirelock_maxtime);
    qt_hash_destroy(shep0->uniquelockaddrs);
    printf
	("QTHREADS: Blocked on a lock %llu times, average %g secs, max %g secs\n",
	 (unsigned long long)shep0->lockwait_count,
	 (shep0->lockwait_count == 0) ? 0 : (shep0->lockwait_time / shep0->lockwait_count),
	 shep0->lockwait_maxtime);
    printf("QTHREADS: Locks held an average of %g seconds, max %g seconds\n",
	   (shep0->aquirelock_count == 0) ? 0 : (shep0->hold_time / shep0->aquirelock_count),
	   shep0->hold_maxtime);
    printf("QTHREADS: %ld unique addresses used with FEB, blocked %g secs\n",
	   qt_hash_count(shep0->uniquefebaddrs),
	   (shep0->febblock_count == 0) ? 0 : shep0->febblock_time);
    qt_hash_destroy(shep0->uniquefebaddrs);
    printf
	("QTHREADS: %llu potentially-blocking FEB operations, average %g secs, max %g secs\n",
	 (unsigned long long)shep0->febblock_count,
	 (shep0->febblock_count == 0) ? 0 : (shep0->febblock_time / shep0->febblock_count),
	 shep0->febblock_maxtime);
    printf
	("QTHREADS: %llu FEB operations blocked, average wait %g secs, max %g secs\n",
	 (unsigned long long)shep0->febwait_count,
	 (shep0->febwait_count == 0) ? 0 : (shep0->febwait_time / shep0->febwait_count),
	 shep0->febwait_maxtime);
    printf
	("QTHREADS: %llu FEB bits emptied, stayed empty average %g secs, max %g secs\n",
	 (unsigned long long)shep0->empty_count,
	 (shep0->empty_count == 0) ? 0 : (shep0->empty_time /
	     shep0->empty_count), shep0->empty_maxtime);
#endif

    for (i = 0; i < QTHREAD_LOCKING_STRIPES; i++) {
	qthread_debug(ALL_DETAILS, "destroying lock infrastructure of shep %i\n", (int)i);
	qt_hash_destroy(qlib->locks[i]);
	qt_hash_destroy_deallocate(qlib->FEBs[i],
				   (qt_hash_deallocator_fn)
				   qthread_addrstat_delete);
	qt_hash_destroy_deallocate(qlib->syncvars[i],
				   (qt_hash_deallocator_fn)
				   qthread_addrstat_delete);
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
	QTHREAD_FASTLOCK_DESTROY(qlib->atomic_locks[i]);
#endif
#ifdef QTHREAD_COUNT_THREADS
	printf("QTHREADS: bin %i used %u times for locks, %u times for FEBs\n", i,
	       (unsigned int)qlib->locks_stripes[i], (unsigned int)qlib->febs_stripes[i]);
# ifdef QTHREAD_MUTEX_INCREMENT
	QTHREAD_FASTLOCK_DESTROY(qlib->locks_stripes_locks[i]);
	QTHREAD_FASTLOCK_DESTROY(qlib->febs_stripes_locks[i]);
# endif
#endif
    }
    qthread_debug(ALL_DETAILS, "destroy lock infrastructure arrays\n");
    free(qlib->locks);
    free(qlib->FEBs);
#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    free(qlib->atomic_locks);
#endif
#ifdef QTHREAD_COUNT_THREADS
    free(qlib->locks_stripes);
    free(qlib->febs_stripes);
# ifdef QTHREAD_MUTEX_INCREMENT
    free(qlib->locks_stripes_locks);
    free(qlib->febs_stripes_locks);
# endif
#endif

#ifdef QTHREAD_COUNT_THREADS
    printf("QTHREADS: spawned %lu threads, max concurrency %lu, avg concurrency %g\n",
	   (unsigned long)threadcount, (unsigned long)maxconcurrentthreads,
	   avg_concurrent_threads);
    QTHREAD_FASTLOCK_DESTROY(concurrentthreads_lock);
#endif

    qthread_debug(ALL_DETAILS, "destroy scheduling locks\n");
    QTHREAD_FASTLOCK_DESTROY(qlib->max_thread_id_lock);
    QTHREAD_FASTLOCK_DESTROY(qlib->sched_shepherd_lock);

#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(qlib->mccoy_thread->valgrind_stack_id);
    VALGRIND_STACK_DEREGISTER(qlib->valgrind_masterstack_id);
#endif
    assert(qlib->mccoy_thread->rdata->stack == NULL);
    qthread_debug(ALL_DETAILS, "destroy mccoy thread structure\n");
    free(qlib->mccoy_thread->rdata);
    FREE_QTHREAD(qlib->mccoy_thread);
    qthread_debug(ALL_DETAILS, "destroy master stack\n");
    free(qlib->master_stack);
    for (i = 0; i < qlib->nshepherds; ++i) {
	qthread_debug(ALL_DETAILS, "destroy topology information on shep %i\n", (int)i);
	if (qlib->shepherds[i].shep_dists) {
	    free(qlib->shepherds[i].shep_dists);
	}
	if (qlib->shepherds[i].sorted_sheplist) {
	    free(qlib->shepherds[i].sorted_sheplist);
	}
    }
#ifndef UNPOOLED
    for (i = 0; i < qlib->nshepherds; ++i) {
	qthread_debug(ALL_DETAILS, "destroy shep %i qthread pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].qthread_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i queue pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].queue_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i threadqueue pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].threadqueue_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i threadqueue_node pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].threadqueue_node_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i lock pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].lock_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i addrres pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].addrres_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i addrstat pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].addrstat_pool);
	qthread_debug(ALL_DETAILS, "destroy shep %i stack pool\n", (int)i);
	qt_mpool_destroy(qlib->shepherds[i].stack_pool);
    }
    qthread_debug(ALL_DETAILS, "destroy global memory pools\n");
    qt_mpool_destroy(generic_qthread_pool);
    generic_qthread_pool = NULL;
    qt_mpool_destroy(generic_stack_pool);
    generic_stack_pool = NULL;
    qt_mpool_destroy(generic_queue_pool);
    generic_queue_pool = NULL;
    qt_mpool_destroy(generic_threadqueue_pool);
    generic_threadqueue_pool = NULL;
    qt_mpool_destroy(generic_threadqueue_node_pool);
    generic_threadqueue_node_pool = NULL;
    qt_mpool_destroy(generic_lock_pool);
    generic_lock_pool = NULL;
    qt_mpool_destroy(generic_addrstat_pool);
    generic_addrstat_pool = NULL;
#endif
    qthread_debug(ALL_DETAILS, "destroy global shepherd array\n");
    free(qlib->shepherds);
    qthread_debug(ALL_DETAILS, "destroy global data\n");
    free(qlib);
    qlib = NULL;
    qthread_debug(ALL_DETAILS, "destroy shepherd thread-local data\n");
    qassert(pthread_key_delete(shepherd_structs), 0);

    qthread_debug(ALL_DETAILS, "finished.\n");
}				       /*}}} */

    
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void qthread_pack_workerid(const qthread_worker_id_t w, const qthread_worker_id_t newId)
{
  int shep = w%qlib->nshepherds;
  int worker = w/qlib->nshepherds;
  assert((shep < qlib->nshepherds));
  assert((worker < qlib->nworkerspershep));
  qlib->shepherds[shep].workers[worker].packed_worker_id = newId;
}


int qthread_disable_worker(const qthread_worker_id_t w)
{

  int shep = w%qlib->nshepherds;
  int worker = w/qlib->nshepherds;
   
  qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
  qassert_ret((worker < qlib->nworkerspershep), QTHREAD_BADARGS);
  if ((worker == 0) & (shep == 0)) {
    /* currently, the "real mccoy" original thread cannot be migrated
     * (because I don't know what issues that could cause on all
     * architectures). For similar reasons, therefore, the original
     * shepherd cannot be disabled. One of the nice aspects of this is that
     * therefore it is impossible to disable ALL shepherds.
     *
     * ... it's entirely possible that I'm being overly cautious. This is a
     * policy based on gut feeling rather than specific issues. */
    return QTHREAD_NOT_ALLOWED;
  }
  qthread_debug(ALL_CALLS, "began on worker(%i-%i)\n", shep, worker);
  
  (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 1,0);
  qlib->nworkers_active--; // decrement active count
  
  if (worker == 0) qthread_disable_shepherd(shep);
  
  return QTHREAD_SUCCESS;
}

int qthread_enable_worker(const qthread_worker_id_t w)
{				       /*{{{ */

  int shep = w%qlib->nshepherds;
  int worker = w/qlib->nshepherds;

  assert(shep < qlib->nshepherds);
  
  if (worker == 0) qthread_enable_shepherd(shep);
  qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
  qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
  (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 0,1);
  return QTHREAD_SUCCESS;
}				       /*}}} */
#endif


int qthread_disable_shepherd(const qthread_shepherd_id_t shep)
{				       /*{{{ */
    qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
    if (shep == 0) {
	/* currently, the "real mccoy" original thread cannot be migrated
	 * (because I don't know what issues that could cause on all
	 * architectures). For similar reasons, therefore, the original
	 * shepherd cannot be disabled. One of the nice aspects of this is that
	 * therefore it is impossible to disable ALL shepherds.
	 *
	 * ... it's entirely possible that I'm being overly cautious. This is a
	 * policy based on gut feeling rather than specific issues. */
	return QTHREAD_NOT_ALLOWED;
    }
    qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), -1);
    (void)QT_CAS(qlib->shepherds[shep].active, 1, 0);
    return QTHREAD_SUCCESS;
}				       /*}}} */

void qthread_enable_shepherd(const qthread_shepherd_id_t shep)
{				       /*{{{ */
    assert(shep < qlib->nshepherds);
    qthread_debug(ALL_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
    (void)QT_CAS(qlib->shepherds[shep].active, 0, 1);
}				       /*}}} */

qthread_t *qthread_internal_self(void)
{				       /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_t *worker = qthread_internal_getworker();
    return worker ? worker->current : NULL;
#else
    qthread_shepherd_t *shep = qthread_internal_getshep();
    return shep ? shep->current : NULL;
#endif
}				       /*}}} */

qthread_t *qthread_self(void)
{				       /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_t *worker = qthread_internal_getworker();
    return worker ? worker->current : NULL;
#else
    qthread_shepherd_t *shep = qthread_internal_getshep();
    return shep ? shep->current : NULL;
#endif
}				       /*}}} */

size_t qthread_stackleft(void)
{				       /*{{{ */
    const qthread_t * f = qthread_internal_self();
    if (f != NULL && f->rdata->stack != NULL) {
	assert((size_t) & f > (size_t) f->rdata->stack &&
	       (size_t) & f < ((size_t) f->rdata->stack + qlib->qthread_stack_size));
#ifdef STACK_GROWS_DOWN
	/* not tested */
	assert(((size_t) (f->rdata->stack) + qlib->qthread_stack_size) -
	       (size_t) (&f) < qlib->qthread_stack_size);
	return ((size_t) (f->rdata->stack) + qlib->qthread_stack_size) -
	    (size_t) (&f);
#else
	assert((size_t) (&f) - (size_t) (f->rdata->stack) <
	       qlib->qthread_stack_size);
	return (size_t) (&f) - (size_t) (f->rdata->stack);
#endif
    } else {
	return 0;
    }
}				       /*}}} */

size_t qthread_readstate(
    const enum introspective_state type)
{				       /*{{{ */
    switch (type) {
	case STACK_SIZE:
	    return qlib->qthread_stack_size;
	case BUSYNESS:
	{
	    qthread_shepherd_t *shep = qthread_internal_getshep();
	    if (shep == NULL) {
		return (size_t) (-1);
	    } else {
		return
		    qthread_internal_atomic_read_s(&
						   (shep->
						    ready->advisory_queuelen),
						   &(shep->
						     ready->advisory_queuelen_m));
	    }
	}
	case ACTIVE_SHEPHERDS:
	    return (size_t) (qlib->nshepherds_active);
	case TOTAL_SHEPHERDS:
	    return (size_t) (qlib->nshepherds);
	default:
	    return (size_t) (-1);
    }
}				       /*}}} */

aligned_t *qthread_retloc(void)
{				       /*{{{ */
    qthread_t *me = qthread_internal_self();

    if (me) {
	return me->ret;
    } else {
	return NULL;
    }
}				       /*}}} */

/************************************************************/
/* functions to manage thread stack allocation/deallocation */
/************************************************************/
static QINLINE qthread_t *qthread_thread_new(const qthread_f f,
					     const void *arg,
					     size_t arg_size,
					     void * ret,
					     const qthread_shepherd_id_t
					     shepherd)
{				       /*{{{ */
    qthread_t *t;

#ifndef UNPOOLED
    qthread_shepherd_t *myshep = &(qlib->shepherds[shepherd]);
#endif

    t = ALLOC_QTHREAD(myshep);
    qthread_debug(ALL_DETAILS, "t = %p\n", t);
    qassert_ret(t, NULL);

#ifdef QTHREAD_NONLAZY_THREADIDS
    /* give the thread an ID number */
    t->thread_id =
	qthread_internal_incr(&(qlib->max_thread_id),
			      &qlib->max_thread_id_lock, 1);
#else
    t->thread_id = (unsigned int)-1;
#endif

    t->thread_state = QTHREAD_STATE_NEW;
    t->flags = 0;
    t->target_shepherd = NULL;
    t->f = f;
    t->arg = (void *)arg;
    t->ret = ret;
    t->rdata = NULL;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->currentParallelRegion = NULL;
#endif
    // should I use the builtin block for args?
    t->free_arg = NO;
    if (arg_size > 0) {
	if (arg_size <= ARGCOPY_MAX) {
	    t->arg = (void*)(&t->argcopy_data);
	} else {
	    t->arg = malloc(arg_size);
	    t->free_arg = YES;
	}
	memcpy(t->arg, arg, arg_size);
    }

    qthread_debug(ALL_DETAILS, "returning\n");
    return t;
}				       /*}}} */

static QINLINE void qthread_thread_free(qthread_t * t)
{				       /*{{{ */
    assert(t != NULL);

    qthread_debug(ALL_FUNCTIONS, "t(%p): destroying thread id %i\n", t, t->thread_id);
    if (t->rdata != NULL) {
#ifdef QTHREAD_USE_VALGRIND
	VALGRIND_STACK_DEREGISTER(t->valgrind_stack_id);
#endif
	qthread_debug(ALL_DETAILS, "t(%p): releasing stack %p to %p\n", t, t->rdata->stack, t->creator_ptr);
	FREE_STACK(t->creator_ptr, t->rdata->stack);
    }
    qthread_debug(ALL_DETAILS, "t(%p): releasing thread handle %p\n", t, t);
    FREE_QTHREAD(t);
}				       /*}}} */

static QINLINE qthread_queue_t *qthread_queue_new(qthread_shepherd_t *
						  shepherd)
{				       /*{{{ */
    qthread_queue_t *q;

    q = ALLOC_QUEUE(shepherd);
    if (q != NULL) {
	q->head = NULL;
	q->tail = NULL;
	if (pthread_mutex_init(&q->lock, NULL) != 0) {
	    FREE_QUEUE(q);
	    return NULL;
	}
	if (pthread_cond_init(&q->notempty, NULL) != 0) {
	    QTHREAD_DESTROYLOCK(&q->lock);
	    FREE_QUEUE(q);
	    return NULL;
	}
    }
    return q;
}				       /*}}} */

static QINLINE void qthread_queue_free(qthread_queue_t * q)
{				       /*{{{ */
    assert((q->head == NULL) && (q->tail == NULL));
    QTHREAD_DESTROYLOCK(&q->lock);
    QTHREAD_DESTROYCOND(&q->notempty);
    FREE_QUEUE(q);
}				       /*}}} */

static QINLINE void qthread_enqueue(qthread_queue_t * q, qthread_t * t)
{				       /*{{{ */
    assert(t != NULL);
    assert(q != NULL);

    qthread_debug(ALL_FUNCTIONS, "q(%p), t(%p): started\n", q, t);

    QTHREAD_LOCK(&q->lock);

    t->next = NULL;

    if (q->head == NULL) {	       /* surely then tail is also null; no need to check */
	q->head = t;
	q->tail = t;
	QTHREAD_SIGNAL(&q->notempty);
    } else {
	q->tail->next = t;
	q->tail = t;
    }

    qthread_debug(ALL_DETAILS, "q(%p), t(%p): finished\n", q, t);
    QTHREAD_UNLOCK(&q->lock);
}				       /*}}} */

static QINLINE qthread_t *qthread_dequeue(qthread_queue_t * q)
{				       /*{{{ */
    qthread_t *t = NULL;

    /* NOTE: it's up to the caller to lock/unlock the queue! */
    qthread_debug(ALL_FUNCTIONS,
		  "q(%p), t(%p): started\n", q, t);

    if (q->head == NULL) {
	qthread_debug(ALL_DETAILS,
		      "q(%p), t(%p): finished (nobody in list)\n",
		      q, t);
	return (NULL);
    }

    t = q->head;
    if (q->head != q->tail) {
	q->head = q->head->next;
    } else {
	q->head = NULL;
	q->tail = NULL;
    }
    t->next = NULL;

    qthread_debug(ALL_DETAILS,
		  "q(%p), t(%p): finished\n", q, t);
    return (t);
}				       /*}}} */

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
// in Rose only code -- call function with rose argument list
// pulled from HPCToolkit externals
 
int in_qthread_fence(void* addr);
extern void *qthread_fence1;
extern void *qthread_fence2;

#define MONITOR_ASM_LABEL(name)         \
  asm volatile (".globl " #name );	\
  asm volatile ( #name ":" )
#endif

/* this function runs a thread until it completes or yields */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high, unsigned int low)
{				       /*{{{ */
    qthread_t *t = (qthread_t *) ((((uintptr_t) high) << 32) | low);
#else
static void qthread_wrapper(void *ptr)
{
    qthread_t *t = (qthread_t *) ptr;
#endif
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
    MONITOR_ASM_LABEL(qthread_fence1); // add label for HPCToolkit stack unwind
#endif

    qthread_debug(THREAD_BEHAVIOR,
		  "tid %u executing f=%p arg=%p...\n",
		  t->thread_id, t->f, t->arg);
    assert((size_t)&t > (size_t)t->rdata->stack &&
	   (size_t)&t < ((size_t)t->rdata->stack + qlib->qthread_stack_size));
#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    threadcount++;
    concurrentthreads++;
    if (concurrentthreads > maxconcurrentthreads)
	maxconcurrentthreads = concurrentthreads;
    avg_concurrent_threads =
	(avg_concurrent_threads*(double)(threadcount-1.0)/threadcount)
	+((double)concurrentthreads/threadcount);
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif
    if (t->ret) {
	/* XXX: if this fails, we should probably do something */
	if (t->flags & QTHREAD_RET_IS_SYNCVAR) {
	    /* this should avoid problems with irresponsible return values */
	    qassert(qthread_syncvar_writeEF_const((syncvar_t*)t->ret,
			INT64TOINT60((t->f) (t->arg))), QTHREAD_SUCCESS);
	    if (t->free_arg) {
		assert(&t->argcopy_data != t->arg);
		free(t->arg);
	    }
	} else {
	    qassert(qthread_writeEF_const((aligned_t*)t->ret, (t->f) (t->arg)), QTHREAD_SUCCESS);
	}
    } else {
	assert(t->f);
	(t->f) (t->arg);
    }
    t->thread_state = QTHREAD_STATE_TERMINATED;

#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    concurrentthreads--;
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif
    if (t->flags & QTHREAD_FUTURE) {
	future_exit();
    }
    /* theoretically, we could rely on the uc_link pointer to bring us back to
     * the parent shepherd. HOWEVER, this doesn't work in lots of situations,
     * so we do it manually. A brief list of situations:
     *  1. if we're using the portable make/get/swapcontext
     *  2. if the context switch requires a stack-size modification
     *  3. if the thread has migrated (i.e. uc_link points to the original
     *  shepherd, not the current parent... theoretically, that could be
     *  changed, but getting a good uc_link is finicky)
     *
     * Thus, since doing it manually isn't a performance problem, we do it
     * manually.
     */
    qthread_debug(THREAD_BEHAVIOR, "tid %u exiting.\n",
		  t->thread_id);
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
    MONITOR_ASM_LABEL(qthread_fence2); // add label for HPCToolkit stack unwind
#endif
    qthread_back_to_master(t);
}				       /*}}} */
 
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
int in_qthread_fence(void* addr)
{
  return (qthread_fence1 <= addr) && (addr >= qthread_fence2);
}
#endif

/* This function means "run thread t". The second argument (c) is a pointer
 * to the current context. */
static QINLINE void qthread_exec(qthread_t * t, ucontext_t * c)
{				       /*{{{ */
#ifdef NEED_RLIMIT
    struct rlimit rlp;
#endif

    assert(t != NULL);
    assert(c != NULL);

    if (t->thread_state == QTHREAD_STATE_NEW) {

	qthread_debug(ALL_DETAILS,
		      "t(%p), c(%p): type is QTHREAD_THREAD_NEW!\n",
		      t, c);
	t->thread_state = QTHREAD_STATE_RUNNING;

	qassert(getcontext(&t->rdata->context), 0);	/* puts the current context into t->rdata->context */
	qthread_makecontext(&t->rdata->context, t->rdata->stack, qlib->qthread_stack_size,
			    (void (*)(void))qthread_wrapper, t, c);
#ifdef HAVE_NATIVE_MAKECONTEXT
    } else {
	t->rdata->context.uc_link = c;       /* NULL pthread_exit() */
#endif
    }

    t->rdata->return_context = c;

#ifdef NEED_RLIMIT
    qthread_debug(ALL_DETAILS,
		  "t(%p): setting stack size limits... hopefully we don't currently exceed them!\n",
		  t);
    if (t->flags & QTHREAD_REAL_MCCOY) {
	rlp.rlim_cur = qlib->master_stack_size;
    } else {
	rlp.rlim_cur = qlib->qthread_stack_size;
    }
    rlp.rlim_max = qlib->max_stack_size;
    qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
#endif

    qthread_debug(ALL_DETAILS,
		  "t(%p): executing swapcontext(%p, %p)...\n", t, t->rdata->return_context, &t->rdata->context);
    /* return_context (aka "c") is being written over with the current context */
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(ucontext_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(ucontext_t));
#endif
    qassert(swapcontext(t->rdata->return_context, &t->rdata->context), 0);
#ifdef NEED_RLIMIT
    qthread_debug(ALL_DETAILS,
		  "t(%p): setting stack size limits back to normal...\n",
		  t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
	rlp.rlim_cur = qlib->master_stack_size;
	qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif

    assert(t != NULL);
    assert(c != NULL);

    qthread_debug(ALL_DETAILS, "t(%p): finished\n", t);
}				       /*}}} */

/* this function yields thread t to the master kernel thread */
void qthread_yield(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    if (t != NULL) {
	qthread_debug(THREAD_DETAILS,
		      "tid %u yielding...\n", t->thread_id);
	t->thread_state = QTHREAD_STATE_YIELDED;
	qthread_back_to_master(t);
	qthread_debug(THREAD_DETAILS, "tid %u resumed.\n",
		      t->thread_id);
    }
}				       /*}}} */

/***********************************************
 * FORKING                                     *
 ***********************************************/
/* fork a thread by putting it in somebody's work queue
 * NOTE: scheduling happens here
 */
int qthread_fork(const qthread_f f, const void *arg, aligned_t * ret)
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_id_t shep;
    qthread_shepherd_t *myshep = qthread_internal_getshep();

    qthread_debug(THREAD_BEHAVIOR, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    assert(qlib);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (myshep) {
	shep = myshep->shepherd_id; // rely on work-stealing
    } else {
	shep = 0;
    }
#else
    int loopctr = 0;
    if (myshep) {
	do {
	    shep = (qthread_shepherd_id_t) (myshep->sched_shepherd++);
	    if (myshep->sched_shepherd == qlib->nshepherds) {
		myshep->sched_shepherd = 0;
	    }
	    loopctr++;
	} while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
		loopctr <= qlib->nshepherds);
    } else {
	do {
	    shep = (qthread_shepherd_id_t) qthread_internal_incr_mod(&qlib->sched_shepherd, qlib->nshepherds, &qlib->sched_shepherd_lock);
	    loopctr++;
	} while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
		loopctr <= qlib->nshepherds);
    }
    if (loopctr > qlib->nshepherds) {
	qthread_debug(THREAD_BEHAVIOR, "could not find an active shepherd\n");
	return QTHREAD_NOT_ALLOWED;
    }
#endif
    t = qthread_thread_new(f, arg, 0, ret, shep);
    if (t) {
	qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n",
		      t->thread_id, shep);

	if (ret) {
	    int test = qthread_empty(ret);

	    if (test != QTHREAD_SUCCESS) {
		qthread_thread_free(t);
		return test;
	    }
	}
	qt_threadqueue_enqueue(qlib->shepherds[shep].ready, t, myshep);
	return QTHREAD_SUCCESS;
    }
    qthread_debug(THREAD_BEHAVIOR, "malloc error\n");
    return QTHREAD_MALLOC_ERROR;
}				       /*}}} */

int qthread_fork_syncvar_copyargs_to(const qthread_f f,
				     const void *const arg,
				     const size_t arg_size,
				     syncvar_t * const ret,
				     const qthread_shepherd_id_t preferred_shep
				     )
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_id_t target_shep;
    qthread_shepherd_t *myshep = qthread_internal_getshep();

    qthread_debug(THREAD_BEHAVIOR, "f(%p), arg(%p), ret(%p)\n", f, arg, ret);
    assert(qlib);
    assert(f);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (preferred_shep != NO_SHEPHERD) {
	target_shep = preferred_shep % qthread_num_shepherds();
    } else if (myshep != NULL) {
	target_shep = myshep->shepherd_id;
    } else {
	target_shep = 0;
    }
#else
    if (preferred_shep != NO_SHEPHERD) {
	target_shep = preferred_shep % qthread_num_shepherds();
    } else {
	int loopctr = 0;
	if (myshep) {		       /* note: for forking from a qthread, NO LOCKS! */
	    do {
		target_shep =
		    (qthread_shepherd_id_t) (myshep->sched_shepherd++);
		if (myshep->sched_shepherd == qlib->nshepherds) {
		    myshep->sched_shepherd = 0;
		}
		loopctr++;
	    } while (QTHREAD_CASLOCK_READ_UI
		     (qlib->shepherds[target_shep].active) != 1 &&
		     loopctr <= qlib->nshepherds);
	} else {
	    do {
		target_shep = (qthread_shepherd_id_t)
		    qthread_internal_incr_mod(&qlib->sched_shepherd,
					      qlib->nshepherds,
					      &qlib->sched_shepherd_lock);
		loopctr++;
	    } while (QTHREAD_CASLOCK_READ_UI
		     (qlib->shepherds[target_shep].active) != 1 &&
		     loopctr <= qlib->nshepherds);
	}
	if (loopctr > qlib->nshepherds) {
	    qthread_debug(THREAD_BEHAVIOR,
			  "could not find an active shepherd\n");
	    return QTHREAD_NOT_ALLOWED;
	}
    }
#endif
    t = qthread_thread_new(f, arg, arg_size, (aligned_t *) ret, target_shep);
    if (!t) {
	qthread_debug(THREAD_BEHAVIOR, "malloc error\n");
	return QTHREAD_MALLOC_ERROR;
    }
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
		  target_shep);
    t->flags |= QTHREAD_RET_IS_SYNCVAR;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    t->currentParallelRegion = myshep->currentParallelRegion; // saved in shepherd
#endif
    t->id = preferred_shep;  // used in barrier and arrive_first, NOT the thread-id
                             // may be extraneous in both when parallel region 
                             // barriers in place (not will to pull it now
                             // maybe late) akp

    if (ret) {
	int test = qthread_syncvar_empty(ret);

	if (test != QTHREAD_SUCCESS) {
	    qthread_thread_free(t);
	    return test;
	}
    }
    qt_threadqueue_enqueue(qlib->shepherds[target_shep].ready, t, myshep);
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_fork_syncvar_copyargs(const qthread_f f, const void *const arg,
				  const size_t arg_size,
				  syncvar_t * const ret)
{				       /*{{{ */
    return qthread_fork_syncvar_copyargs_to(f, arg, arg_size, ret,
					    NO_SHEPHERD);
}				       /*}}} */


int qthread_fork_syncvar(const qthread_f f, const void *const arg, syncvar_t * const ret)
{				       /*{{{ */
    return qthread_fork_syncvar_copyargs(f, arg, 0, ret);
}				       /*}}} */

int qthread_fork_to(const qthread_f f, const void *arg, aligned_t * ret,
		    const qthread_shepherd_id_t shepherd)
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if (shepherd >= qlib->nshepherds || f == NULL) {
	return QTHREAD_BADARGS;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep = &(qlib->shepherds[shepherd]);
    t->target_shepherd = shep;
    t->flags |= QTHREAD_UNSTEALABLE;
    qthread_debug(THREAD_BEHAVIOR,
	    "new-tid %u shep %u\n", t->thread_id,
	    shepherd);

    if (ret) {
	int test = qthread_empty(ret);

	if (test != QTHREAD_SUCCESS) {
	    qthread_thread_free(t);
	    return test;
	}
    }
    qt_threadqueue_enqueue(shep->ready, t,
	    qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}				       /*}}} */


int qthread_fork_syncvar_to(
    const qthread_f f,
    const void *const arg,
    syncvar_t * ret,
    const qthread_shepherd_id_t s)
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_t *shep;

    const qthread_shepherd_id_t shepherd = s % qthread_num_shepherds();
    if (shepherd >= qlib->nshepherds || f == NULL) {
      return QTHREAD_BADARGS;
    }
    assert(f != NULL);
    assert(shepherd < qlib->nshepherds);
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    t->target_shepherd = shep = &(qlib->shepherds[shepherd]);
    t->flags |= QTHREAD_UNSTEALABLE;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    t->id = s % (qthread_num_workers());
#else
    t->id = shepherd;
#endif
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
		  shepherd);

    if (ret) {
	int test = qthread_syncvar_empty(ret);

	if (test != QTHREAD_SUCCESS) {
	    qthread_thread_free(t);
	    return test;
	}
	t->flags |= QTHREAD_RET_IS_SYNCVAR;
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
	shep =
	    qthread_find_active_shepherd(shep->sorted_sheplist,
					 shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_fork_future_to(const qthread_f f,
			   const void *arg, aligned_t * ret,
			   const qthread_shepherd_id_t shepherd)
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if (shepherd >= qlib->nshepherds || f == NULL) {
	return QTHREAD_BADARGS;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep = &(qlib->shepherds[shepherd]);
    t->flags |= QTHREAD_FUTURE;
    t->target_shepherd = &(qlib->shepherds[shepherd]);
    qthread_debug(THREAD_BEHAVIOR,
	    "new-tid %u shep %u\n",
	    t->thread_id, shepherd);

    if (ret) {
	int test = qthread_empty(ret);

	if (test != QTHREAD_SUCCESS) {
	    qthread_thread_free(t);
	    return test;
	}
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
	shep =
	    qthread_find_active_shepherd(shep->sorted_sheplist,
		    shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_fork_syncvar_future_to(
    const qthread_f f,
    const void *arg,
    syncvar_t * ret,
    const qthread_shepherd_id_t shepherd)
{				       /*{{{ */
    qthread_t *t;
    qthread_shepherd_t *shep;

    assert(shepherd < qlib->nshepherds);
    assert(f != NULL);
    if (shepherd >= qlib->nshepherds || f == NULL) {
	return QTHREAD_BADARGS;
    }
    if (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shepherd].active) != 1) {
	return QTHREAD_NOT_ALLOWED;
    }
    t = qthread_thread_new(f, arg, 0, ret, shepherd);
    qassert_ret(t, QTHREAD_MALLOC_ERROR);
    shep = &(qlib->shepherds[shepherd]);
    t->flags |= QTHREAD_FUTURE;
    t->target_shepherd = &(qlib->shepherds[shepherd]);
    qthread_debug(THREAD_BEHAVIOR, "new-tid %u shep %u\n", t->thread_id,
		  shepherd);
    if (ret) {
	int test = qthread_syncvar_empty(ret);

	if (test != QTHREAD_SUCCESS) {
	    qthread_thread_free(t);
	    return test;
	}
	t->flags |= QTHREAD_RET_IS_SYNCVAR;
    }
    if (QTHREAD_CASLOCK_READ_UI(shep->active) == 0) {
	shep =
	    qthread_find_active_shepherd(shep->sorted_sheplist,
					 shep->shep_dists);
    }
    qt_threadqueue_enqueue(shep->ready, t, qthread_internal_getshep());
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_fork_syncvar_future(const qthread_f f,
				const void *arg, syncvar_t * ret)
{
    qthread_shepherd_id_t shep;
    qthread_shepherd_t *myshep = qthread_internal_getshep();

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (myshep) {
	shep = myshep->shepherd_id;    // rely on work-stealing
    } else {
	shep = 0;
    }
#else
    int loopctr = 0;
    if (myshep) {		       /* note: for forking from a qthread, NO LOCKS! */
	do {
	    shep = (qthread_shepherd_id_t) (myshep->sched_shepherd++);
	    if (myshep->sched_shepherd == qlib->nshepherds) {
		myshep->sched_shepherd = 0;
	    }
	    loopctr++;
	} while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
		 loopctr <= qlib->nshepherds);
    } else {
	do {
	    shep = (qthread_shepherd_id_t)
		qthread_internal_incr_mod(&qlib->sched_shepherd,
					  qlib->nshepherds,
					  &qlib->sched_shepherd_lock);
	    loopctr++;
	} while (QTHREAD_CASLOCK_READ_UI(qlib->shepherds[shep].active) != 1 &&
		 loopctr <= qlib->nshepherds);
    }
    if (loopctr > qlib->nshepherds) {
	qthread_debug(THREAD_BEHAVIOR, "could not find an active shepherd\n");
	return QTHREAD_NOT_ALLOWED;
    }
#endif
    return qthread_fork_syncvar_future_to(f, arg, ret, shep);
}

void qthread_back_to_master(qthread_t * t)
{				       /*{{{ */
#ifdef NEED_RLIMIT
    struct rlimit rlp;

    qthread_debug(ALL_DETAILS,
		  "t(%p): setting stack size limits for master thread...\n",
		  t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
	rlp.rlim_cur = qlib->master_stack_size;
	rlp.rlim_max = qlib->max_stack_size;
	qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif
    /* now back to your regularly scheduled master thread */
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(ucontext_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(t->rdata->return_context, sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(ucontext_t));
    VALGRIND_MAKE_MEM_DEFINED(t->rdata->return_context, sizeof(ucontext_t));
#endif
    qassert(swapcontext(&t->rdata->context, t->rdata->return_context), 0);
#ifdef NEED_RLIMIT
    qthread_debug(ALL_DETAILS,
		  "t(%p): setting stack size limits back to qthread size...\n",
		  t);
    if (!(t->flags & QTHREAD_REAL_MCCOY)) {
	rlp.rlim_cur = qlib->qthread_stack_size;
	qassert(setrlimit(RLIMIT_STACK, &rlp), 0);
    }
#endif
}				       /*}}} */

/* function to move a qthread from one shepherd to another */
int qthread_migrate_to(const qthread_shepherd_id_t shepherd)
{				       /*{{{ */
    qthread_t *me = qthread_internal_self();
    if (me->rdata->shepherd_ptr->shepherd_id == shepherd) {
	me->target_shepherd = me->rdata->shepherd_ptr;
	return QTHREAD_SUCCESS;
    }
    if (me->flags & QTHREAD_REAL_MCCOY) {
	return QTHREAD_NOT_ALLOWED;
    }
    if (me && shepherd < qlib->nshepherds) {
	qthread_debug(THREAD_BEHAVIOR,
		      "tid %u from shep %u to shep %u\n",
		      me->thread_id, me->rdata->shepherd_ptr->shepherd_id, shepherd);
	me->target_shepherd = &(qlib->shepherds[shepherd]);
	me->thread_state = QTHREAD_STATE_MIGRATING;
	me->rdata->blockedon = (struct qthread_lock_s *)(intptr_t) shepherd;
	qthread_back_to_master(me);

	qthread_debug(THREAD_DETAILS,
		      "tid %u awakes on shepherd %u!\n",
		      me->thread_id, me->rdata->shepherd_ptr->shepherd_id);
	return QTHREAD_SUCCESS;
    } else {
	return QTHREAD_BADARGS;
    }
}				       /*}}} */

/* functions to implement FEB locking/unlocking */

/* The lock ordering in these functions is very particular, and is designed to
 * reduce the impact of having only one hashtable. Don't monkey with it unless
 * you REALLY know what you're doing! If one hashtable becomes a problem, we
 * may need to move to a new mechanism.
 */

/* This is just a little function that should help in debugging */
int qthread_feb_status(const aligned_t * addr)
{				       /*{{{ */
    const aligned_t *alignedaddr;
#ifndef SST
    int status = 1;		/* full */
    qthread_addrstat_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(addr);

    QALIGN(addr, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       (void *)alignedaddr);
	if (m) {
	    QTHREAD_FASTLOCK_LOCK(&m->lock);
	    status = m->full;
	    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	}
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_BEHAVIOR, "addr %p is %i", addr,
		  status);
    return status;
#else
    QALIGN(addr, alignedaddr);
    return PIM_feb_is_full((void*)alignedaddr);
#endif
}				       /*}}} */

/* this function removes the FEB data structure for the address maddr from the
 * hash table */
static QINLINE void qthread_FEB_remove(void *maddr)
{				       /*{{{ */
    qthread_addrstat_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(maddr);

    qthread_debug(LOCK_DETAILS,
		  "attempting removal %p\n", maddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin], maddr);
	if (m) {
	    QTHREAD_FASTLOCK_LOCK(&(m->lock));
	    if (m->FEQ == NULL && m->EFQ == NULL && m->FFQ == NULL &&
		m->full == 1) {
		qthread_debug(LOCK_DETAILS,
			      "all lists are empty, and status is full\n");
		qt_hash_remove_locked(qlib->FEBs[lockbin], maddr);
	    } else {
		QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
		qthread_debug(LOCK_DETAILS,
			      "address cannot be removed; in use\n");
		m = NULL;
	    }
	}
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    if (m != NULL) {
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	qthread_addrstat_delete(m);
    }
}				       /*}}} */

static QINLINE void qthread_gotlock_empty(qthread_shepherd_t * shep,
					  qthread_addrstat_t * m, void *maddr,
					  const char recursive)
{				       /*{{{ */
    qthread_addrres_t *X = NULL;
    int removeable;

    m->full = 0;
    QTHREAD_EMPTY_TIMER_START(m);
    if (m->EFQ != NULL) {
	/* dQ */
	X = m->EFQ;
	m->EFQ = X->next;
	/* op */
	if (maddr && maddr != X->addr) {
	    *(aligned_t*)maddr = *(X->addr);
	}
	/* requeue */
	X->waiter->thread_state = QTHREAD_STATE_RUNNING;
	qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter, shep);
	FREE_ADDRRES(X);
	qthread_gotlock_fill(shep, m, maddr, 1);
    }
    if (m->full == 1 && m->EFQ == NULL && m->FEQ == NULL && m->FFQ == NULL)
	removeable = 1;
    else
	removeable = 0;
    if (recursive == 0) {
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	if (removeable) {
	    qthread_FEB_remove(maddr);
	}
    }
}				       /*}}} */

static QINLINE void qthread_gotlock_fill(qthread_shepherd_t * shep,
					 qthread_addrstat_t * m, void *maddr,
					 const char recursive)
{				       /*{{{ */
    qthread_addrres_t *X = NULL;
    int removeable;

    qthread_debug(LOCK_DETAILS, "m(%p), addr(%p)\n", m, maddr);
    m->full = 1;
    QTHREAD_EMPTY_TIMER_STOP(m);
    /* dequeue all FFQ, do their operation, and schedule them */
    qthread_debug(LOCK_DETAILS, "dQ all FFQ\n");
    while (m->FFQ != NULL) {
	/* dQ */
	X = m->FFQ;
	m->FFQ = X->next;
	/* op */
	if (X->addr && X->addr != maddr) {
	    memcpy(X->addr, maddr, sizeof(aligned_t));
	}
	/* schedule */
	X->waiter->thread_state = QTHREAD_STATE_RUNNING;
	qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter, shep);
	FREE_ADDRRES(X);
    }
    if (m->FEQ != NULL) {
	/* dequeue one FEQ, do their operation, and schedule them */
	qthread_t *waiter;

	qthread_debug(LOCK_DETAILS, "dQ 1 FEQ\n");
	X = m->FEQ;
	m->FEQ = X->next;
	/* op */
	if (X->addr && X->addr != maddr) {
	    memcpy(X->addr, maddr, sizeof(aligned_t));
	}
	waiter = X->waiter;
	waiter->thread_state = QTHREAD_STATE_RUNNING;
	qt_threadqueue_enqueue(waiter->rdata->shepherd_ptr->ready, waiter, shep);
	FREE_ADDRRES(X);
	qthread_gotlock_empty(shep, m, maddr, 1);
    }
    if (m->EFQ == NULL && m->FEQ == NULL && m->full == 1)
	removeable = 1;
    else
	removeable = 0;
    if (recursive == 0) {
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	/* now, remove it if it needs to be removed */
	if (removeable) {
	    qthread_FEB_remove(maddr);
	}
    }
}				       /*}}} */

int qthread_empty(const aligned_t * dest)
{				       /*{{{ */
    const aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m;
    qt_hash FEBbin;
    qthread_t *me = qthread_internal_self();
    {
	const int lockbin = QTHREAD_CHOOSE_STRIPE(dest);
	FEBbin = qlib->FEBs[lockbin];

	QALIGN(dest, alignedaddr);
	QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    }
    qt_hash_lock(FEBbin);
    {				       /* BEGIN CRITICAL SECTION */
	m = (qthread_addrstat_t *) qt_hash_get_locked(FEBbin,
					       (void *)alignedaddr);
	if (!m) {
	    /* currently full, and must be added to the hash to empty */
	    m = qthread_addrstat_new(me ? (me->rdata->shepherd_ptr) :
				     qthread_internal_getshep());
	    if (!m) {
		qt_hash_unlock(FEBbin);
		return QTHREAD_MALLOC_ERROR;
	    }
	    m->full = 0;
	    QTHREAD_EMPTY_TIMER_START(m);
	    qt_hash_put_locked(FEBbin, (void *)alignedaddr, m);
	    m = NULL;
	} else {
	    /* it could be either full or not, don't know */
	    QTHREAD_FASTLOCK_LOCK(&m->lock);
	}
    }				       /* END CRITICAL SECTION */
    qt_hash_unlock(FEBbin);
    qthread_debug(LOCK_BEHAVIOR, "%p is now empty\n", dest);
    if (m) {
	qthread_gotlock_empty(me->rdata->shepherd_ptr, m, (void *)alignedaddr, 0);
    }
#else
    QALIGN(dest, alignedaddr);
    PIM_feb_empty((void*)alignedaddr);
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_fill(const aligned_t * dest)
{				       /*{{{ */
    const aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t *me = qthread_internal_self();

    QALIGN(dest, alignedaddr);
    /* lock hash */
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {				       /* BEGIN CRITICAL SECTION */
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       (void *)alignedaddr);
	if (m) {
	    QTHREAD_FASTLOCK_LOCK(&m->lock);
	}
    }				       /* END CRITICAL SECTION */
    qt_hash_unlock(qlib->FEBs[lockbin]);	/* unlock hash */
    qthread_debug(LOCK_BEHAVIOR, "%p is now full\n", dest);
    if (m) {
	/* if dest wasn't in the hash, it was already full. Since it was,
	 * we need to fill it. */
	qthread_gotlock_fill(me->rdata->shepherd_ptr, m, (void *)alignedaddr, 0);
    }
#else
    QALIGN(dest, alignedaddr);
    PIM_feb_fill((unsigned int*)alignedaddr);
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

/* the way this works is that:
 * 1 - data is copies from src to destination
 * 2 - the destination's FEB state gets changed from empty to full
 */

int qthread_writeF(aligned_t * restrict const dest,
		   const aligned_t * restrict const src)
{				       /*{{{ */
    aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t *me = qthread_internal_self();

    qthread_debug(LOCK_BEHAVIOR,
		  "tid %u dest=%p src=%p...\n",
		  me->thread_id, dest, src);
    QALIGN(dest, alignedaddr);
    QTHREAD_LOCK_UNIQUERECORD(feb, dest, me);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {	/* lock hash */
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       (void *)alignedaddr);
	if (!m) {
	    m = qthread_addrstat_new(me->rdata->shepherd_ptr);
	    if (!m) {
		qt_hash_unlock(qlib->FEBs[lockbin]);
		return QTHREAD_MALLOC_ERROR;
	    }
	    qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m);
	}
	QTHREAD_FASTLOCK_LOCK(&m->lock);
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);	/* unlock hash */
    /* we have the lock on m, so... */
    if (dest && dest != src) {
	memcpy(dest, src, sizeof(aligned_t));
    }
    qthread_debug(LOCK_BEHAVIOR,
		  "tid %u succeeded on %p=%p\n",
		  me->thread_id, dest, src);
    qthread_gotlock_fill(me->rdata->shepherd_ptr, m, alignedaddr, 0);
#else
    QALIGN(dest, alignedaddr);
    PIM_feb_empty((void*)alignedaddr);
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_writeF_const(aligned_t * const dest,
			 const aligned_t src)
{				       /*{{{ */
    return qthread_writeF(dest, &src);
}				       /*}}} */

/* the way this works is that:
 * 1 - destination's FEB state must be "empty"
 * 2 - data is copied from src to destination
 * 3 - the destination's FEB state gets changed from empty to full
 */

int qthread_writeEF(aligned_t * restrict const dest,
		    const aligned_t * restrict const src)
{				       /*{{{ */
    aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m;
    qthread_addrres_t *X = NULL;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t *me = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    qthread_debug(LOCK_BEHAVIOR,
		  "tid %u dest=%p src=%p...\n",
		  me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, dest, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(dest, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       (void *)alignedaddr);
	if (!m) {
	    m = qthread_addrstat_new(me->rdata->shepherd_ptr);
	    if (!m) {
		qt_hash_unlock(qlib->FEBs[lockbin]);
		return QTHREAD_MALLOC_ERROR;
	    }
	    qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m);
	}
	QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    qthread_debug(LOCK_DETAILS, "m->full == %i\n",
		  m->full);
    if (m->full == 1) {		       /* full, thus, we must block */
	QTHREAD_WAIT_TIMER_DECLARATION;
	X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
	if (X == NULL) {
	    QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
	    return QTHREAD_MALLOC_ERROR;
	}
	X->addr = (aligned_t *) src;
	X->waiter = me;
	X->next = m->EFQ;
	m->EFQ = X;
	qthread_debug(LOCK_DETAILS, "back to parent\n");
	me->thread_state = QTHREAD_STATE_FEB_BLOCKED;
	me->rdata->blockedon = (struct qthread_lock_s *)m;
	QTHREAD_WAIT_TIMER_START();
	qthread_back_to_master(me);
	QTHREAD_WAIT_TIMER_STOP(me, febwait);
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p after waiting\n",
		      me->thread_id, dest, src);
    } else {
	if (dest && dest != src) {
	    memcpy(dest, src, sizeof(aligned_t));
	}
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p\n",
		      me->thread_id, dest, src);
	qthread_gotlock_fill(me->rdata->shepherd_ptr, m, alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else
    QALIGN(dest, alignedaddr);
    while (PIM_feb_try_writeef(alignedaddr, src) == 1) {
	qthread_yield(me);
    }
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_writeEF_const(aligned_t * const dest, const aligned_t src)
{				       /*{{{ */
    return qthread_writeEF(dest, &src);
}				       /*}}} */

/* the way this works is that:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 */

int qthread_readFF(aligned_t * restrict const dest,
		   const aligned_t * restrict const src)
{				       /*{{{ */
    const aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m = NULL;
    qthread_addrres_t *X = NULL;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t *me = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    qthread_debug(LOCK_BEHAVIOR,
		  "tid %u dest=%p src=%p...\n",
		  me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       (void *)alignedaddr);
	if (!m) {
	    if (dest && dest != src) {
		memcpy(dest, src, sizeof(aligned_t));
	    }
	} else {
	    QTHREAD_FASTLOCK_LOCK(&m->lock);
	}
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* now m, if it exists, is locked - if m is NULL, then we're done! */
    if (m == NULL) {		       /* already full! */
	if (dest && dest != src) {
	    memcpy(dest, src, sizeof(aligned_t));
	}
    } else if (m->full != 1) {	       /* not full... so we must block */
	QTHREAD_WAIT_TIMER_DECLARATION;
	X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
	if (X == NULL) {
	    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	    return QTHREAD_MALLOC_ERROR;
	}
	X->addr = (aligned_t *) dest;
	X->waiter = me;
	X->next = m->FFQ;
	m->FFQ = X;
	qthread_debug(LOCK_DETAILS, "back to parent\n");
	me->thread_state = QTHREAD_STATE_FEB_BLOCKED;
	me->rdata->blockedon = (struct qthread_lock_s *)m;
	QTHREAD_WAIT_TIMER_START();
	qthread_back_to_master(me);
	QTHREAD_WAIT_TIMER_STOP(me, febwait);
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p after waiting\n",
		      me->thread_id, dest, src);
    } else {			       /* exists AND is empty... weird, but that's life */
	if (dest && dest != src) {
	    memcpy(dest, src, sizeof(aligned_t));
	}
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p\n",
		      me->thread_id, dest, src);
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else
    QALIGN(src, alignedaddr);
    while (PIM_feb_try_readff(dest, alignedaddr) == 1) {
	qthread_yield(me);
    }
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

/* the way this works is that:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 * 3 - the src's FEB bits get changed from full to empty
 */

int qthread_readFE(aligned_t * restrict const dest,
		   const aligned_t * restrict const src)
{				       /*{{{ */
    const aligned_t *alignedaddr;
#ifndef SST
    qthread_addrstat_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t *me = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    qthread_debug(LOCK_BEHAVIOR,
		  "tid %u dest=%p src=%p...\n",
		  me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
	m = (qthread_addrstat_t *) qt_hash_get_locked(qlib->FEBs[lockbin],
					       alignedaddr);
	if (!m) {
	    m = qthread_addrstat_new(me->rdata->shepherd_ptr);
	    if (!m) {
		qt_hash_unlock(qlib->FEBs[lockbin]);
		return QTHREAD_MALLOC_ERROR;
	    }
	    qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m);
	}
	QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    if (m->full == 0) {		       /* empty, thus, we must block */
	QTHREAD_WAIT_TIMER_DECLARATION;
	qthread_addrres_t *X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);

	if (X == NULL) {
	    QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	    return QTHREAD_MALLOC_ERROR;
	}
	X->addr = (aligned_t *) dest;
	X->waiter = me;
	X->next = m->FEQ;
	m->FEQ = X;
	qthread_debug(LOCK_DETAILS, "back to parent\n");
	me->thread_state = QTHREAD_STATE_FEB_BLOCKED;
	/* so that the shepherd will unlock it */
	me->rdata->blockedon = (struct qthread_lock_s *)m;
	QTHREAD_WAIT_TIMER_START();
	qthread_back_to_master(me);
	QTHREAD_WAIT_TIMER_STOP(me, febwait);
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p after waiting\n",
		      me->thread_id, dest, src);
    } else {			       /* full, thus IT IS OURS! MUAHAHAHA! */
	if (dest && dest != src) {
	    memcpy(dest, src, sizeof(aligned_t));
	}
	qthread_debug(LOCK_BEHAVIOR,
		      "tid %u succeeded on %p=%p\n",
		      me->thread_id, dest, src);
	qthread_gotlock_empty(me->rdata->shepherd_ptr, m, (void*)alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else
    QALIGN(src, alignedaddr);
    while (PIM_feb_try_readfe(dest, alignedaddr) == 1) {
	qthread_yield(me);
    }
#endif
    return QTHREAD_SUCCESS;
}				       /*}}} */

/* functions to implement FEB-ish locking/unlocking
 *
 * These are atomic and functional, but do not have the same semantics as full
 * FEB locking/unlocking (for example, unlocking cannot block)
 *
 * NOTE: these have not been profiled, and so may need tweaking for speed
 * (e.g. multiple hash tables, shortening critical section, etc.)
 */

/* The lock ordering in these functions is very particular, and is designed to
 * reduce the impact of having a centralized hashtable. Don't monkey with it
 * unless you REALLY know what you're doing!
 */

int qthread_lock(const aligned_t * a)
{				       /*{{{ */
    qthread_lock_t *m;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(a);
    qthread_t *me = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(aquirelock);

    qthread_debug(LOCK_BEHAVIOR, "tid(%u), a(%p): starting...\n",
		  me->thread_id, a);
    QTHREAD_LOCK_UNIQUERECORD(lock, a, me);
    QTHREAD_LOCK_TIMER_START(aquirelock);

    QTHREAD_COUNT_THREADS_BINCOUNTER(locks, lockbin);
    qt_hash_lock(qlib->locks[lockbin]);
    m = (qthread_lock_t *) qt_hash_get_locked(qlib->locks[lockbin], (void *)a);
    if (m == NULL) {
	m = ALLOC_LOCK(me->rdata->shepherd_ptr);
	if (m == NULL) {
	    qt_hash_unlock(qlib->locks[lockbin]);
	    return QTHREAD_MALLOC_ERROR;
	}

	assert(me->rdata->shepherd_ptr == qthread_internal_getshep());
	m->waiting = qthread_queue_new(me->rdata->shepherd_ptr);
	if (m->waiting == NULL) {
	    FREE_LOCK(m);
	    qt_hash_unlock(qlib->locks[lockbin]);
	    return QTHREAD_MALLOC_ERROR;
	}
	QTHREAD_FASTLOCK_INIT(m->lock);
	QTHREAD_HOLD_TIMER_INIT(m);
	qt_hash_put_locked(qlib->locks[lockbin], (void *)a, m);
	/* since we just created it, we own it */
	QTHREAD_FASTLOCK_LOCK(&m->lock);
	/* can only unlock the hash after we've locked the address, because
	 * otherwise there's a race condition: the address could be removed
	 * before we have a chance to add ourselves to it */
	qt_hash_unlock(qlib->locks[lockbin]);

#ifdef QTHREAD_DEBUG
	m->owner = me->thread_id;
#endif
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	qthread_debug(LOCK_BEHAVIOR,
		      "tid(%u), a(%p): returned (wasn't locked)\n",
		      me->thread_id, a);
    } else {
	QTHREAD_WAIT_TIMER_DECLARATION;
	/* success==failure: because it's in the hash, someone else owns
	 * the lock; dequeue this thread and yield. NOTE: it's up to the
	 * master thread to enqueue this thread and unlock the address
	 */
	QTHREAD_FASTLOCK_LOCK(&m->lock);
	/* for an explanation of the lock/unlock ordering here, see above */
	qt_hash_unlock(qlib->locks[lockbin]);

	me->thread_state = QTHREAD_STATE_BLOCKED;
	me->rdata->blockedon = m;

	QTHREAD_WAIT_TIMER_START();

	qthread_back_to_master(me);

	QTHREAD_WAIT_TIMER_STOP(me, lockwait);

	/* once I return to this context, I own the lock! */
	/* conveniently, whoever unlocked me already set up everything too */
	qthread_debug(LOCK_BEHAVIOR,
		      "tid(%u), a(%p): returned (was locked)\n",
		      me->thread_id, a);
    }
    QTHREAD_LOCK_TIMER_STOP(aquirelock, me);
    QTHREAD_HOLD_TIMER_START(m);
    return QTHREAD_SUCCESS;
}				       /*}}} */

int qthread_unlock(const aligned_t * a)
{				       /*{{{ */
    qthread_lock_t *m;
    qthread_t *u;
    const int lockbin = QTHREAD_CHOOSE_STRIPE(a);
    qthread_t *me = qthread_internal_self();

    qthread_debug(LOCK_BEHAVIOR, "tid(%u), a(%p)\n", me->thread_id,
		  a);

    QTHREAD_COUNT_THREADS_BINCOUNTER(locks, lockbin);
    qt_hash_lock(qlib->locks[lockbin]);
    m = (qthread_lock_t *) qt_hash_get_locked(qlib->locks[lockbin], (void *)a);
    if (m == NULL) {
	/* unlocking an address that's already unlocked */
	qt_hash_unlock(qlib->locks[lockbin]);
	return QTHREAD_REDUNDANT;
    }
    QTHREAD_FASTLOCK_LOCK(&m->lock);

    QTHREAD_HOLD_TIMER_STOP(m, me->rdata->shepherd_ptr);

    /* unlock the address... if anybody's waiting for it, give them the lock
     * and put them in a ready queue.  If not, delete the lock structure.
     */

    QTHREAD_LOCK(&m->waiting->lock);
    u = qthread_dequeue(m->waiting);
    if (u == NULL) {
	qthread_debug(LOCK_DETAILS,
		      "tid(%u), a(%p): deleting waiting queue\n",
		      me->thread_id, a);
	qt_hash_remove_locked(qlib->locks[lockbin], (void *)a);
	qt_hash_unlock(qlib->locks[lockbin]);
	QTHREAD_HOLD_TIMER_DESTROY(m);
	QTHREAD_UNLOCK(&m->waiting->lock);
	qthread_queue_free(m->waiting);
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
	QTHREAD_FASTLOCK_DESTROY(m->lock);
	FREE_LOCK(m);
    } else {
	qt_hash_unlock(qlib->locks[lockbin]);
	qthread_debug(LOCK_DETAILS,
		      "tid(%u), a(%p): pulling thread from queue (%p)\n",
		      me->thread_id, a, u);
	u->thread_state = QTHREAD_STATE_RUNNING;
#ifdef QTHREAD_DEBUG
	m->owner = u->thread_id;
#endif

	/* NOTE: because of the use of getcontext()/setcontext(), threads
	 * return to the shepherd that setcontext()'d into them, so they
	 * must remain in that queue.
	 */
	qt_threadqueue_enqueue(u->rdata->shepherd_ptr->ready, u, me->rdata->shepherd_ptr);

	QTHREAD_UNLOCK(&m->waiting->lock);
	QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    }

    return QTHREAD_SUCCESS;
}				       /*}}} */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
/* These are just accessor functions */
#ifdef QTHREAD_LOG_BARRIER
qt_barrier_t *qt_thread_barrier()            // get barrier active for this thread
#else
qt_feb_barrier_t *qt_thread_barrier()            // get barrier active for this thread
#endif
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    return t->currentParallelRegion->barrier;
}				       /*}}} */

void qt_set_unstealable(void);
void qt_set_unstealable()
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    t->flags |= QTHREAD_UNSTEALABLE;
}				       /*}}} */

/* These are just accessor functions */
qthread_parallel_region_t *qt_parallel_region()  // get active parallel region
{				       /*{{{ */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_t *worker = qthread_internal_getworker();
  return worker->current->currentParallelRegion;
#else
  qthread_shepherd_t * shep = qthread_internal_getshep();
  return shep->currentParallelRegion;
#endif

}				       /*}}} */

int qt_omp_parallel_region_create()
{				       /*{{{ */
  qthread_shepherd_t *myshep = qthread_internal_getshep();
 
  qthread_parallel_region_t *pr = malloc(sizeof(qthread_parallel_region_t));
  qassert_ret(pr, QTHREAD_MALLOC_ERROR);

#ifdef QTHREAD_LOG_BARRIER
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qt_barrier_t *gb = qt_barrier_create(qthread_num_workers(), REGION_BARRIER,0); // allocate barrier for region (workers)
#else
  qt_barrier_t *gb = qt_barrier_create(qthread_num_shepherds(), REGION_BARRIER,0); // allocate barrier for region (shepherds)
#endif
#else
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qt_feb_barrier_t *gb = qt_feb_barrier_create(qthread_num_workers()); // allocate barrier for region (workers)
#else
  qt_feb_barrier_t *gb = qt_feb_barrier_create(qthread_num_shepherds()); // allocate barrier for region (sheperds)
#endif
#endif

  pr->last = myshep->currentParallelRegion;
  myshep->currentParallelRegion = pr;
  myshep->currentParallelRegion->barrier = gb; 
  myshep->currentParallelRegion->forLoop = NULL; 
  myshep->currentParallelRegion->loopList = NULL; 

  return 0;
}  		                       /*}}} */

void * qt_free_loop(void *lp);

void qt_omp_parallel_region_destroy()
{				       /*{{{ */
  qthread_shepherd_t *myshep = qthread_internal_getshep();
  qthread_parallel_region_t *pr = myshep->currentParallelRegion;
  if (!pr) return;

#if 0 // race condition on cleanup - commented out until found - akp 3/16/11
      // it looks like one thread reaches cleanup code before completing loop
      // I thought it was related to threads moving (affinity reduces the 
      // likelyhood of the hang -- but does not make it go away) but have not 
      // found it.
  qt_free_loop(pr->loopList);

  if (pr->barrier){
#ifdef QTHREAD_LOG_BARRIER
    qt_barrier_destroy(pr->barrier);
#else
    qt_feb_barrier_destroy(pr->barrier);
#endif
  }
  myshep->currentParallelRegion = pr->last;
  free(pr);
  
#endif
}  		                       /*}}} */

#endif

/* These are just accessor functions */
unsigned qthread_id(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    qthread_debug(ALL_CALLS, "tid(%u)\n",
		  t ? t->thread_id : (unsigned)-1);
#ifdef QTHREAD_NONLAZY_THREADIDS
    return t ? t->thread_id : (unsigned int)-1;
#else
    if (!t) {
	return (unsigned int)-1;
    }
    if (t->thread_id != (unsigned int)-1) {
	return t->thread_id;
    }
    ((qthread_t *) t)->thread_id =
	qthread_internal_incr(&(qlib->max_thread_id),
			      &qlib->max_thread_id_lock, 1);
    return t->thread_id;
#endif
}				       /*}}} */
unsigned qthread_barrier_id(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    qthread_debug(ALL_CALLS, "tid(%u)\n",
		  t ? t->id : (unsigned)-1);
    return t ? t->id : (unsigned int)-1;
}				       /*}}} */

qthread_shepherd_id_t qthread_shep(void)
{				       /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();
    if (ret == NULL) {
	return NO_SHEPHERD;
    } else {
	return ret->shepherd_id;
    }
}				       /*}}} */
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
qthread_worker_id_t qthread_worker(qthread_shepherd_id_t *shepherd_id)
{                                      /*{{{ */
    qthread_worker_t *worker = (qthread_worker_t *)pthread_getspecific(shepherd_structs);
    if(shepherd_id != NULL) {
	*shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker?(worker->packed_worker_id):NO_WORKER;
}                                      /*}}} */
#endif

int qthread_shep_ok(void)
{				       /*{{{ */
    qthread_shepherd_t *ret = qthread_internal_getshep();
    if (ret == NULL) {
	return QTHREAD_PTHREAD_ERROR;
    } else {
	return QTHREAD_CASLOCK_READ_UI(ret->active);
    }
}				       /*}}} */

unsigned int qthread_internal_shep_to_node(const qthread_shepherd_id_t shep)
{				       /*{{{ */
    return qlib->shepherds[shep].node;
}				       /*}}} */

/* returns the distance between two shepherds */
int qthread_distance(const qthread_shepherd_id_t src,
		     const qthread_shepherd_id_t dest)
{				       /*{{{ */
    assert(src < qlib->nshepherds);
    assert(dest < qlib->nshepherds);
    if (src >= qlib->nshepherds || dest >= qlib->nshepherds) {
	return QTHREAD_BADARGS;
    }
    if (qlib->shepherds[src].shep_dists == NULL) {
	return 0;
    } else {
	return qlib->shepherds[src].shep_dists[dest];
    }
}				       /*}}} */

/* returns a list of shepherds, sorted by their distance from this qthread;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t *qthread_sorted_sheps(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    if (t == NULL) {
	return NULL;
    }
    assert(t->rdata);
    assert(t->rdata->shepherd_ptr);
    return t->rdata->shepherd_ptr->sorted_sheplist;
}				       /*}}} */

/* returns a list of shepherds, sorted by their distance from the specified shepherd;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t *qthread_sorted_sheps_remote(const
							 qthread_shepherd_id_t
							 src)
{				       /*{{{ */
    assert(src < qlib->nshepherds);
    if (src >= qlib->nshepherds) {
	return NULL;
    }
    return qlib->shepherds[src].sorted_sheplist;
}				       /*}}} */

/* returns the number of shepherds actively scheduling work */
qthread_shepherd_id_t qthread_num_shepherds(void)
{				       /*{{{ */
    return (qthread_shepherd_id_t) (qlib->nshepherds_active);
}				       /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
/* returns the number of shepherds actively scheduling work */
qthread_worker_id_t qthread_num_workers(void)
{				       /*{{{ */
    return (qthread_worker_id_t) (qlib->nworkers_active);
}				       /*}}} */
#endif

/* these two functions are helper functions for futurelib
 * (nobody else gets to have 'em!) */
unsigned int qthread_isfuture(void)
{				       /*{{{ */
    qthread_t * t = qthread_internal_self();
    return t ? (t->flags & QTHREAD_FUTURE) : 0;
}				       /*}}} */

void qthread_assertfuture(qthread_t * t)
{				       /*{{{ */
    t->flags |= QTHREAD_FUTURE;
}				       /*}}} */

void qthread_assertnotfuture(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    t->flags &= ~QTHREAD_FUTURE;
}				       /*}}} */

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
# ifdef __INTEL_COMPILER
#  pragma warning (disable:1418)
# endif

int qthread_forCount(int inc)
{                                    /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    return (t->rdata->forCount += inc);
}                                    /*}}} */

void qthread_getTaskListLock(void)
{                                      /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    qthread_syncvar_writeEF_const(&t->rdata->taskWaitLock, 1);
}                                      /*}}} */

void qthread_releaseTaskListLock(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    qthread_syncvar_readFE(NULL, &t->rdata->taskWaitLock);
}				       /*}}} */

taskSyncvar_t * qthread_getTaskRetVar(void)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    return t->rdata->openmpTaskRetVar;
}				       /*}}} */

void qthread_setTaskRetVar(taskSyncvar_t *v)
{				       /*{{{ */
    qthread_t *t = qthread_internal_self();
    assert(t);
    t->rdata->openmpTaskRetVar = v;
}				       /*}}} */
#endif

#if defined(QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
uint32_t qthread_incr32_(volatile uint32_t * op, const int32_t incr)
{				       /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint32_t retval;
    QTHREAD_LOCK_TIMER_DECLARATION(incr);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_LOCK_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_LOCK_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_LOCK_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}				       /*}}} */

uint64_t qthread_incr64_(volatile uint64_t * op, const int64_t incr)
{				       /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    uint64_t retval;
    QTHREAD_LOCK_TIMER_DECLARATION(incr);

    QTHREAD_COUNT_THREADS_BINCOUNTER(atomic, stripe);
    QTHREAD_LOCK_UNIQUERECORD(incr, op, qthread_internal_self());
    QTHREAD_LOCK_TIMER_START(incr);
    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    QTHREAD_LOCK_TIMER_STOP(incr, qthread_internal_self());
    return retval;
}				       /*}}} */

double qthread_dincr_(volatile double *op, const double incr)
{				       /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    double retval;

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}				       /*}}} */

float qthread_fincr_(volatile float *op, const float incr)
{				       /*{{{ */
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(op);
    float retval;

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *op;
    *op += incr;
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}				       /*}}} */

uint32_t qthread_cas32_(volatile uint32_t * operand, const uint32_t oldval,
			const uint32_t newval)
{				       /*{{{ */
    uint32_t retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
	*operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}				       /*}}} */

uint64_t qthread_cas64_(volatile uint64_t * operand, const uint64_t oldval,
			const uint64_t newval)
{				       /*{{{ */
    uint64_t retval;
    unsigned int stripe = QTHREAD_CHOOSE_STRIPE(operand);

    QTHREAD_FASTLOCK_LOCK(&(qlib->atomic_locks[stripe]));
    retval = *operand;
    if (retval == oldval) {
	*operand = newval;
    }
    QTHREAD_FASTLOCK_UNLOCK(&(qlib->atomic_locks[stripe]));
    return retval;
}				       /*}}} */
#endif
