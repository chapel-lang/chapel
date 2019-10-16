/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_common.c $
 * Description: GASNet Extended API Common code
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

GASNETI_IDENT(gasnete_IdentString_Version, "$GASNetExtendedLibraryVersion: " GASNET_EXTENDED_VERSION_STR " $");
GASNETI_IDENT(gasnete_IdentString_ExtendedName, "$GASNetExtendedLibraryName: " GASNET_EXTENDED_NAME_STR " $");

/* ------------------------------------------------------------------------------------ */
/*
  Thread Management
  =================
*/

#ifndef GASNETE_THREADING_CUSTOM /* top-level disable for all threading-related code */

#if GASNETI_MAX_THREADS <= 256
  gasneti_threaddata_t *gasnete_threadtable[GASNETI_MAX_THREADS] = { 0 };
#else
  #define GASNETI_DYNAMIC_THREADTABLE 1
  gasneti_threaddata_t **gasnete_threadtable = NULL;
#endif
static int gasnete_numthreads = 0; /* current thread count */
int gasnete_maxthreadidx = 0; /* high-water mark of thread indexes issued */
static gasneti_mutex_t threadtable_lock = GASNETI_MUTEX_INITIALIZER;
#if GASNETI_MAX_THREADS > 1
  /* pthread thread-specific ptr to our threaddata (or NULL for a thread never-seen before) */
  GASNETI_THREADKEY_DEFINE(gasnete_threaddata);
#endif

#ifndef GASNETI_DEFAULT_MAX_THREADS
#define GASNETI_DEFAULT_MAX_THREADS 1024
#endif

extern uint64_t gasneti_max_threads(void) {
  static uint64_t val = 0;
  if (!val) {
    gasneti_mutex_lock(&threadtable_lock);
      if (!val) {
        val = MIN(GASNETI_MAX_THREADS, GASNETI_DEFAULT_MAX_THREADS);
        val = gasneti_getenv_int_withdefault("GASNET_MAX_THREADS", val, 0);
        if (val > GASNETI_MAX_THREADS) {
          fprintf(stderr,"WARNING: GASNET_MAX_THREADS value exceeds permissable limit (%i), "
                         "lowering it to match. %s\n", GASNETI_MAX_THREADS, GASNETI_MAX_THREADS_REASON);
        }
        val = MIN(GASNETI_MAX_THREADS, val);
      }
    gasneti_mutex_unlock(&threadtable_lock);
  }
  gasneti_sync_reads();
  gasneti_assert_uint(val ,<=, GASNETI_MAX_THREADS);
  return val;
}

extern void gasneti_fatal_threadoverflow(const char *subsystem) {
  uint64_t maxthreads = gasneti_max_threads();
  const char *reason;
  if (maxthreads < GASNETI_MAX_THREADS) 
    reason = "To raise this limit, set environment variable GASNET_MAX_THREADS.";
  else
    reason = GASNETI_MAX_THREADS_REASON;
  gasneti_fatalerror("GASNet %s: Too many simultaneous local client threads (limit=%"PRIu64"). %s",
                      subsystem, maxthreads, reason);
}

/* ------------------------------------------------------------------------------------ */
/* initing a thread's data upon thread discovery */

#ifndef GASNETE_INIT_THREADDATA
#define GASNETE_INIT_THREADDATA(thread) gasnete_init_threaddata(thread)
static void gasnete_init_threaddata(gasneti_threaddata_t *threaddata) {

  // TODO-EX: need an override?
  gasneti_mutex_init(&(threaddata->foreign_lock));
  threaddata->foreign_eops = NULL;
  threaddata->foreign_iops = NULL;

  #ifndef GASNETE_NEW_THREADDATA_EOP_INIT
  #define GASNETE_NEW_THREADDATA_EOP_INIT(threaddata) \
          (threaddata)->eop_free = NULL
  #endif
  GASNETE_NEW_THREADDATA_EOP_INIT(threaddata);

  #ifndef GASNETE_NEW_THREADDATA_IOP_INIT
  #define GASNETE_NEW_THREADDATA_IOP_INIT(threaddata) \
          (threaddata)->current_iop = gasnete_iop_new(threaddata)
  #endif
  GASNETE_NEW_THREADDATA_IOP_INIT(threaddata);

  /* give the conduit a chance to setup thread context via callbacks
     note gasneti_threaddata_t is zero-init, so only non-zero field inits are required
   */
  #ifdef GASNETC_NEW_THREADDATA_CALLBACK
    GASNETC_NEW_THREADDATA_CALLBACK(threaddata);
  #endif
  #ifdef GASNETE_NEW_THREADDATA_CALLBACK
    GASNETE_NEW_THREADDATA_CALLBACK(threaddata);
  #endif
}
#endif

/* ------------------------------------------------------------------------------------ */
/* freeing a thread's data upon thread exit */

#ifndef GASNETE_FREE_THREADDATA
#define GASNETE_FREE_THREADDATA(thread) gasnete_free_threaddata(thread)

// Return zero on success
static int gasnete_free_threaddata(gasneti_threaddata_t *thread) {
  int leak = 0;

  #ifndef GASNETE_IOP_ISDONE
  #define GASNETE_IOP_ISDONE(iop) gasnete_iop_isdone(iop)
  #endif

  #ifndef GASNETE_FREE_IOPS
  // TODO-EX: checks related to foreign iops?
  #define GASNETE_FREE_IOPS(thread) {                                           \
    int missing = thread->iop_num;                                              \
                                                                                \
    /* active iop */                                                            \
    gasnete_iop_t *iop = thread->current_iop;                                   \
    gasneti_assert(iop->next == NULL); /* not inside an NBI access region */    \
    gasneti_assert(GASNETE_IOP_ISDONE(iop)); /* no outstanding NBI ops */       \
    gasneti_free(iop);                                                          \
    missing--;                                                                  \
                                                                                \
    /* iop free lists */                                                        \
    for (int i = 0; i < 2; ++i) {                                               \
      if (i) {                                                                  \
        gasneti_mutex_lock(&thread->foreign_lock);                              \
        iop = thread->foreign_iops;                                             \
        thread->foreign_iops = NULL;                                            \
        gasneti_mutex_unlock(&thread->foreign_lock);                            \
      } else {                                                                  \
        iop = thread->iop_free;                                                 \
      }                                                                         \
      while (iop) {                                                             \
        gasnete_iop_t *next = iop->next;                                        \
        gasneti_assert(GASNETE_IOP_ISDONE(iop)); /* active in free list == bad */ \
        gasneti_free(iop);                                                      \
        missing--;                                                              \
        iop = next;                                                             \
      }                                                                         \
    }                                                                           \
    if (missing) {                                                              \
      /* TODO: handle this better? */                                           \
      GASNETI_TRACE_PRINTF(I, ("%d iops leaked", missing));                     \
      leak = 1;                                                                 \
    }                                                                           \
  }
  #endif
  GASNETE_FREE_IOPS(thread);

  #ifndef GASNETE_FREE_EOPS
  // TODO-EX: checks related to foreign eops?
  #define GASNETE_FREE_EOPS(thread) {            \
    size_t num_bufs = thread->eop_num_bufs;      \
    size_t total_eops = num_bufs * GASNETE_EOP_CHUNKCNT; \
    size_t missing = total_eops;                 \
    gasnete_eop_t *eop;                          \
    eop = thread->eop_free;                      \
    while (eop) { --missing; eop = eop->next; }  \
    gasneti_mutex_lock(&thread->foreign_lock);   \
    eop = thread->foreign_eops;                  \
    while (eop) { --missing; eop = eop->next; }  \
    gasneti_mutex_unlock(&thread->foreign_lock); \
    if (missing) {                               \
      /* TODO: handle this better? */            \
      GASNETI_TRACE_PRINTF(I, ("%"PRIuPTR" eops missing at thread destruction, leaking %"PRIuPTR" eops", \
                               (uintptr_t)missing, (uintptr_t)total_eops)); \
      leak = 1;                                  \
    } else {                                     \
      void **eopbuf = (void **)thread->eop_bufs; \
      while (eopbuf) {                           \
        /* TODO: check for in-flight eops */     \
        gasneti_assert(num_bufs-- > 0);          \
        void **next = *eopbuf;                   \
        gasneti_free(eopbuf);                    \
        eopbuf = next;                           \
      }                                          \
      gasneti_assert_uint(num_bufs ,==, 0);      \
    }                                            \
  }
  #endif
  GASNETE_FREE_EOPS(thread);

  /* conduits needing additional cleanups should use gasnete_register_threadcleanup */

  /* Must leak the threaddata if any iops or eops are unaccounted for */
  if (leak) return 1;

  /* threaddata itself */
  gasneti_free(thread);
  return 0;
}
#endif

/* ------------------------------------------------------------------------------------ */
/* thread table and cleanup callback management */

#ifndef GASNETE_THREAD_CLEANUP_DELAY
#define GASNETE_THREAD_CLEANUP_DELAY 1 /* number of times to postpone pthread_setspecific callback */
#endif

#if GASNETI_MAX_THREADS > 1
  /* pthread_key_t are used explicitly to get callback on thread termination */
  static pthread_key_t gasnete_threaddata_cleanup;
  static pthread_key_t gasnete_threadless_cleanup;
  static void gasnete_threadless_cleanup_fn(void *);
  static void gasnete_threaddata_cleanup_fn(void *);
  static void gasnete_threadkey_init(void) { 
    static int keycreated = 0;
    gasneti_mutex_lock(&threadtable_lock);
    if (!keycreated) {
      pthread_key_create(&gasnete_threaddata_cleanup, &gasnete_threaddata_cleanup_fn);
      pthread_key_create(&gasnete_threadless_cleanup, &gasnete_threadless_cleanup_fn);
      keycreated = 1;
    }
    gasneti_mutex_unlock(&threadtable_lock);
  }
#else
  gasnete_thread_cleanup_t *gasnete_threadless_cleanup;
#endif

extern void gasnete_register_threadcleanup(void (*cleanupfn)(void *), void *context) {
  gasneti_threaddata_t *thread = NULL;
  gasnete_thread_cleanup_t *newcleanup = gasneti_malloc(sizeof(gasnete_thread_cleanup_t));
  gasneti_leak(newcleanup);
  newcleanup->_cleanupfn = cleanupfn;
  newcleanup->_context = context;

  #if GASNETI_MAX_THREADS > 1
    thread = gasneti_threadkey_get(gasnete_threaddata);
  #endif
  if (thread) { /* usual case - extended API thread init before register */
    newcleanup->_next = thread->thread_cleanup;
    thread->thread_cleanup = newcleanup;
  } else { /* save away the cleanups for now */
    #if GASNETI_MAX_THREADS > 1
      gasnete_threadkey_init();
      newcleanup->_next = pthread_getspecific(gasnete_threadless_cleanup);
      pthread_setspecific(gasnete_threadless_cleanup, newcleanup);
    #else
      newcleanup->_next = gasnete_threadless_cleanup;
      gasnete_threadless_cleanup = newcleanup;
    #endif
  }
}

static void gasnete_threadless_cleanup_fn(void *_lifo) {
  #if GASNETI_MAX_THREADS > 1
    if (pthread_getspecific(gasnete_threaddata_cleanup)) { /* thread exists - delay for thread cleanup */
      pthread_setspecific(gasnete_threadless_cleanup, _lifo);
      return;
    }
  #endif
  { /* extended API thread never created - run destructors here */
    gasnete_thread_cleanup_t *cleanuplist = (gasnete_thread_cleanup_t *)_lifo;
    gasnete_thread_cleanup_t *nextcleanup;
    while ((nextcleanup = cleanuplist) != NULL) {
      cleanuplist = nextcleanup->_next;
      nextcleanup->_cleanupfn(nextcleanup->_context);
      gasneti_free(nextcleanup);
    }
  }
}

static void gasnete_threaddata_cleanup_fn(void *_thread) {
  gasneti_threaddata_t *thread = _thread;
  int idx = thread->threadidx;

  #if GASNETI_MAX_THREADS > 1
    /* ensure gasnete_threaddata remains valid for this thread while we run destructors */
    if (!gasneti_threadkey_get(gasnete_threaddata))
      gasneti_threadkey_set(gasnete_threaddata, thread);

    /* postpone destructor for specified iterations, to support GASNet client destructors */
    if (thread->thread_cleanup_delay < GASNETE_THREAD_CLEANUP_DELAY) {
      thread->thread_cleanup_delay++;
      pthread_setspecific(gasnete_threaddata_cleanup, thread);
      return;
    }
  #endif

  GASNETI_TRACE_PRINTF(C,("thread %i exiting\n", idx));

  { gasnete_thread_cleanup_t *cleanuplist;
    gasnete_thread_cleanup_t *nextcleanup;
    /* run cleanups in LIFO order, with multiple passes to catch registrations within cleanupfns */
    while (1) {
      if ((cleanuplist = thread->thread_cleanup) != NULL) 
          thread->thread_cleanup = NULL;
      #if GASNETI_MAX_THREADS > 1
      else if ((cleanuplist = pthread_getspecific(gasnete_threadless_cleanup)) != NULL) 
          pthread_setspecific(gasnete_threadless_cleanup, NULL);
      #else
      else if ((cleanuplist = gasnete_threadless_cleanup) != NULL) 
          gasnete_threadless_cleanup = NULL;
      #endif
      else break;
      
      while ((nextcleanup = cleanuplist) != NULL) {
        cleanuplist = nextcleanup->_next;
        nextcleanup->_cleanupfn(nextcleanup->_context);
        gasneti_free(nextcleanup);
      }
    }
  }

  if (! GASNETE_FREE_THREADDATA(thread)) {
    gasneti_mutex_lock(&threadtable_lock);
      gasnete_threadtable[idx] = NULL;
      gasnete_numthreads--;
    gasneti_mutex_unlock(&threadtable_lock);
  }
}

GASNETI_NEVER_INLINE(gasnete_new_threaddata,
extern void * gasnete_new_threaddata(void)) {
  gasneti_threaddata_t *threaddata = (gasneti_threaddata_t *)gasneti_calloc(1,sizeof(gasneti_threaddata_t));
  int idx;
  uint64_t maxthreads = gasneti_max_threads();
  gasneti_assert_uint(maxthreads ,<=, (((uint64_t)1)<<(sizeof(gasnete_threadidx_t)*8)));
  gasneti_leak(threaddata);

  gasneti_mutex_lock(&threadtable_lock);
    #if GASNETI_DYNAMIC_THREADTABLE
      if (!gasnete_threadtable) {
        gasneti_assert(gasnete_numthreads == 0);
        gasnete_threadtable = (gasneti_threaddata_t **)gasneti_calloc(maxthreads, sizeof(gasneti_threaddata_t*));
      }
    #endif
    gasnete_numthreads++;
    if (gasnete_numthreads > maxthreads) gasneti_fatal_threadoverflow("Extended API");
    /* find a free slot */
    if (gasnete_threadtable[gasnete_numthreads-1] == NULL) idx = gasnete_numthreads-1;
    else { /* keep table somewhat compacted */
      for (idx = 0; idx < maxthreads; idx++) {
        gasneti_assume(idx < GASNETI_MAX_THREADS); // silence a buggy array-bounds warning from gcc-5
        if (gasnete_threadtable[idx] == NULL) break;
      }
    }
    gasneti_assert(idx < GASNETI_MAX_THREADS && idx < maxthreads && gasnete_threadtable[idx] == NULL);
    if (idx > gasnete_maxthreadidx) gasnete_maxthreadidx = idx;

    gasnete_threadtable[idx] = threaddata;
    threaddata->threadidx = idx;
  gasneti_mutex_unlock(&threadtable_lock);

  #if GASNETI_MAX_THREADS > 1
    /* setup TLS identification */
    gasneti_assert(gasneti_threadkey_get(gasnete_threaddata) == NULL);
    gasneti_threadkey_set(gasnete_threaddata, threaddata);

    /* register pthread destructor */
    gasnete_threadkey_init();
    pthread_setspecific(gasnete_threaddata_cleanup, threaddata);
  #endif

  GASNETE_INIT_THREADDATA(threaddata);

  GASNETI_TRACE_PRINTF(C,("gasnete_new_threaddata: idx=%i, numthreads=%i", threaddata->threadidx, gasnete_numthreads));
  return threaddata;
}
/* PURE function (returns same value for a given thread every time) 
*/
#if (GASNETI_MAX_THREADS > 1) && !defined(_GASNETI_MYTHREAD_SLOW)
  // THIS FUNCTION SHOULD NEVER BE CALLED DIRECTLY
  // Most code should use GASNETI_MYTHREAD, or as a last resort _gasneti_mythread_slow()
  extern gasneti_threaddata_t *_gasnete_mythread_slow_slow(void) {
    gasneti_threaddata_t *threaddata = gasneti_threadkey_get(gasnete_threaddata);
    #ifdef GASNETI_RECORD_DYNAMIC_THREADLOOKUP
       GASNETI_RECORD_DYNAMIC_THREADLOOKUP();
    #endif
    if_pf (!threaddata) {
      /* first time we've seen this thread - need to set it up */
      threaddata = gasnete_new_threaddata();
    }
    gasneti_memcheck(threaddata);
    return threaddata;
  }
#endif

#endif /* GASNETE_THREADING_CUSTOM  */

/* ------------------------------------------------------------------------------------ */
