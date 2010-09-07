/*   $Source: /var/local/cvs/gasnet/gasnet_diagnostic.c,v $
 *     $Date: 2010/04/26 16:07:24 $
 * $Revision: 1.28.2.4 $
 * Description: GASNet internal diagnostics
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

/* filthy hack to allow simultaneous use of gasnet-internal and test.h facilities */
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup
#undef strndup
#undef assert         

#define TEST_OMIT_CONFIGSTRINGS 1
#include <../tests/test.h>
#include <gasnet_handler.h>

/* this file should *only* contain symbols used for internal diagnostics,
   so that we can avoid needlessly linking it into production executables 
   everything except the main entry point should be static, to prevent namespace pollution
*/

GASNETT_IDENT(gasneti_IdentString_diagnostics, 
 "$GASNetDiagnostics: (<link>) INCLUDES gasnet_diagnostic.o $");

#if GASNET_PAR
  static void * thread_fn(void *arg);
#endif
static int num_threads = 1;
static int peer = -1;
static void * myseg = NULL;
static void * peerseg = NULL;
static void * peersegmid = NULL;
static int iters = 0;
static int iters0 = 0;
static int iters2 = 0;

#define gasneti_diag_hidx_base 128
#define gasnetc_diag_hidx_base 160
#define gasnete_diag_hidx_base 200
static int gasneti_diag_havehandlers = 0; /* true iff caller has registered our handler table */

static int id = 0;
#define TEST_HEADER(desc)                       \
  PTHREAD_BARRIER(num_threads);                 \
  if (!id) TEST_SECTION_BEGIN();                \
  PTHREAD_BARRIER(num_threads);                 \
  if (TEST_SECTION_ENABLED() &&                 \
      (MSG0("%c: %s %s...",TEST_SECTION_NAME(), \
           (num_threads > 1?"parallel":"sequential"),desc),1))

#ifdef GASNETC_DIAGNOSTICS_SETUP
  GASNETC_DIAGNOSTICS_SETUP /* can include helper source files, etc */
#endif
#ifdef GASNETE_DIAGNOSTICS_SETUP
  GASNETE_DIAGNOSTICS_SETUP /* can include helper source files, etc */
#endif

#ifndef GASNETC_RUN_DIAGNOSTICS_SEQ
#define GASNETC_RUN_DIAGNOSTICS_SEQ(iters) (0)
#endif
#ifndef GASNETC_RUN_DIAGNOSTICS_PAR
#define GASNETC_RUN_DIAGNOSTICS_PAR(iters,threadid,numthreads) (0)
#endif

#ifndef GASNETE_RUN_DIAGNOSTICS_SEQ
#define GASNETE_RUN_DIAGNOSTICS_SEQ(iters) (0)
#endif
#ifndef GASNETE_RUN_DIAGNOSTICS_PAR
#define GASNETE_RUN_DIAGNOSTICS_PAR(iters,threadid,numthreads) (0)
#endif


/* ------------------------------------------------------------------------------------ */
/*  misc sequential tests */

#if GASNET_DEBUG
  extern gasneti_auxseg_request_t gasneti_auxseg_dummy(gasnet_seginfo_t *auxseg_info);
  static void auxseg_test(void) {
    BARRIER();
    TEST_HEADER("auxseg test") {
      gasneti_auxseg_dummy((void *)(uintptr_t)-1); /* call self-test */
    }
  }
#else
  #define auxseg_test()   TEST_HEADER("auxseg test - SKIPPED")
#endif

static void mutex_test(int id);
static void spinlock_test(int id);
static void cond_test(int id);
static void semaphore_test(int id);
static void lifo_test(int id);
static void atomic128_test(int id);
static void malloc_test(int id);
static void progressfns_test(int id);
static void op_test(int id);

/* ------------------------------------------------------------------------------------ */
/* run iters iterations of diagnostics and return zero on success 
   must be called collectively by exactly one thread on each node
   in par mode, the test may internally spawn up to threadcnt threads
 */
extern int gasneti_run_diagnostics(int iter_cnt, int threadcnt, const char *testsections,
                                   gasnet_seginfo_t const *seginfo) { 
  int i;
  int partner = (gasnet_mynode() ^ 1);
  if (partner == gasnet_nodes()) partner = gasnet_mynode();
  test_errs = 0;
  iters = iter_cnt;
  iters2 = iters*100;
  iters0 = MAX(1,iters/100);
  peer = gasnet_mynode()^1;
  if (peer == gasnet_nodes()) peer = gasnet_mynode();
  assert_always(seginfo);
  _test_seginfo = (gasnet_seginfo_t *)seginfo;
  for (i=0; i < (int)gasnet_nodes(); i++) {
    assert_always(_test_seginfo[i].size >= TEST_SEGSZ);
    assert_always((((uintptr_t)_test_seginfo[i].addr) % PAGESZ) == 0);
  }
  myseg = TEST_MYSEG();
  peerseg = TEST_SEG(peer);
  peersegmid = (char *)peerseg + TEST_SEGSZ/2;
  if (testsections) TEST_SECTION_PARSE(testsections);

  TEST_GENERICS_WARNING();

  auxseg_test();

  BARRIER();
  TEST_HEADER("malloc test") malloc_test(0);

  BARRIER();
  TEST_HEADER("gasneti_mutex_t test") mutex_test(0);

  BARRIER();
  TEST_HEADER("gasneti_cond_t test") cond_test(0);

  BARRIER();
  spinlock_test(0);

  BARRIER();
  semaphore_test(0);

  BARRIER();
  atomic128_test(0);

  BARRIER();
  lifo_test(0);

  BARRIER();
  progressfns_test(0);

  BARRIER();

  op_test(0);

  BARRIER();
  TEST_HEADER("conduit tests") {
    BARRIER();
    test_errs += GASNETC_RUN_DIAGNOSTICS_SEQ(iters);
    BARRIER();
    test_errs += GASNETE_RUN_DIAGNOSTICS_SEQ(iters);
  }
  BARRIER();

  #if GASNET_PAR
    num_threads = threadcnt;
    MSG0("spawning %i threads...", num_threads);
    test_createandjoin_pthreads(num_threads, &thread_fn, NULL, 0);
  #endif

  BARRIER();
  MSG0("GASNet internal diagnostics complete.");
  return test_errs;
}

#undef MSG0
#undef ERR
#define MSG0 THREAD_MSG0(id)
#define ERR  THREAD_ERR(id)

/* ------------------------------------------------------------------------------------ */
/*  mixed parallel / sequential tests */
/* ------------------------------------------------------------------------------------ */

static void malloc_test(int id) { 
  int i, cnt = 0;
  int maxobjs;
  void **ptrs;
  gasneti_heapstats_t stats_before, stats_after;

  /* try to trigger any warm-up allocations potentially caused by barrier */
  if (id == 0) {
    /* each node takes a turn being a late arrival */
    for (i=0; i < gasneti_nodes; i++) {
      if (i == gasneti_mynode) {
        uint64_t goal = gasnett_ticks_to_us(gasnett_ticks_now()) + 100000; /* 0.1s */
        while (gasnett_ticks_to_us(gasnett_ticks_now()) < goal) {
          gasnett_sched_yield();
        }
      }
      BARRIER();
    }
  }
  for (i=0; i < num_threads; i++) {
    if (i == id) BARRIER(); /* each thread gets a chance */
    PTHREAD_LOCALBARRIER(num_threads);
  }

  if (!id) gasneti_getheapstats(&stats_before);
    
  PTHREAD_BARRIER(num_threads);

  gasneti_memcheck_all();
  ptrs = gasneti_malloc_allowfail(8);
  assert_always(ptrs);
  gasneti_free(ptrs);
  ptrs = gasneti_realloc(NULL,8);
  assert_always(ptrs);
  gasneti_free(ptrs);
  gasneti_free(NULL);

  PTHREAD_BARRIER(num_threads);
  
  maxobjs = MIN(iters,10000/num_threads);
  ptrs = gasneti_calloc(maxobjs,sizeof(void*));
  for (i = 0; i < maxobjs; i++) assert_always(ptrs[i] == NULL);
  for (i = 0; i < iters2/num_threads; i++) {
    gasneti_memcheck_one();
    if (cnt == maxobjs || (cnt > 0 && TEST_RAND_ONEIN(2))) {
      size_t idx = TEST_RAND(0,cnt-1);
      assert_always(ptrs[idx]);
      gasneti_memcheck(ptrs[idx]);
      if (TEST_RAND_ONEIN(2)) {
        gasneti_free(ptrs[idx]);
        cnt--;
        ptrs[idx] = ptrs[cnt];
        ptrs[cnt] = NULL;
      } else {
        ptrs[idx] = gasneti_realloc(ptrs[idx],TEST_RAND(1,16*1024));
      }
    } else {
      void *p;
      if (TEST_RAND_ONEIN(2)) {
        p = gasneti_malloc(TEST_RAND(1,1024));
      } else {
        p = gasneti_calloc(1,TEST_RAND(1,1024));
      }
      gasneti_memcheck(p);
      assert_always(p);
      assert_always(ptrs[cnt] == NULL);
      ptrs[cnt] = p;
      cnt++;
    }
  }
  gasneti_memcheck_all();
  for (i = 0; i < cnt; i++) {
    gasneti_free(ptrs[i]);
  }
  gasneti_free(ptrs);
  gasneti_memcheck_all();

  PTHREAD_BARRIER(num_threads);

  for (i = 0; i < iters/num_threads; i++) {
    int alignsz;
    for (alignsz = 1; alignsz < 64*1024; alignsz *= 2) {
      size_t sz = TEST_RAND(1,alignsz*2);
      char * p = gasnett_malloc_aligned(alignsz,sz);
      assert_always(p);
      assert_always((((uintptr_t)p) & (alignsz-1)) == 0);
      p[0] = 'x'; p[sz - 1] = 'y';
      gasnett_free_aligned(p);
    }
  }
  gasneti_memcheck_all();

  PTHREAD_BARRIER(num_threads);

  if (!id) {
    gasneti_getheapstats(&stats_after);
    #if GASNET_DEBUG
    {
    #if GASNETI_CONDUIT_THREADS
      float tol = 0.1; /* allow for some heap change if a conduit thread is around */
    #else
      float tol = 0; /* we have all the threads, and nothing else should be allocating */
    #endif
      if (abs(stats_before.live_bytes - stats_after.live_bytes)/(double)stats_after.live_bytes > tol ||
          abs(stats_before.live_objects - stats_after.live_objects)/(double)stats_after.live_objects > tol) 
        MSG("ERROR: unexpected heap size change:\n"
        "  stats_before.live_bytes=%llu stats_after.live_bytes=%llu\n"
        "  stats_before.live_objects=%llu stats_after.live_objects=%llu",
        (unsigned long long)stats_before.live_bytes,   (unsigned long long)stats_after.live_bytes,
        (unsigned long long)stats_before.live_objects, (unsigned long long)stats_after.live_objects);
    }
    #endif
  }
  PTHREAD_BARRIER(num_threads);
}
/* ------------------------------------------------------------------------------------ */
static void cond_test(int id) {
  static gasneti_cond_t cond1 = GASNETI_COND_INITIALIZER;
  static gasneti_cond_t cond2;
  static gasneti_mutex_t lock1 = GASNETI_MUTEX_INITIALIZER;
  static uint32_t done = 0;
  int i;

  PTHREAD_BARRIER(num_threads);

    if (!id) {
      gasneti_cond_init(&cond2);
      gasneti_cond_destroy(&cond2);
      gasneti_cond_init(&cond2);
      gasneti_mutex_lock(&lock1);
      gasneti_cond_signal(&cond1);
      gasneti_cond_signal(&cond2);
      gasneti_cond_broadcast(&cond1);
      gasneti_cond_broadcast(&cond2);
      gasneti_mutex_unlock(&lock1);
    }

  PTHREAD_BARRIER(num_threads);

    if (!id) { /* awake thread */
      for (i = 0; i < iters2; i++) {
        gasneti_mutex_lock(&lock1);
          if (i&1) {
            gasneti_cond_signal(&cond1);
          } else {
            gasneti_cond_broadcast(&cond1);
          }
        gasneti_mutex_unlock(&lock1);
        if (TEST_RAND_ONEIN(iters)) gasnett_sched_yield();
      }
      gasneti_mutex_lock(&lock1);
        done = 1;
        gasneti_cond_broadcast(&cond1);
      gasneti_mutex_unlock(&lock1);
    } else {
      gasneti_mutex_lock(&lock1);
      while (!done) {
        gasneti_mutex_assertlocked(&lock1);
        gasneti_cond_wait(&cond1, &lock1);
      }
      gasneti_mutex_unlock(&lock1);
      gasneti_mutex_assertunlocked(&lock1);
    }

  PTHREAD_BARRIER(num_threads);
}
/* ------------------------------------------------------------------------------------ */
static void mutex_test(int id) {
  static gasneti_mutex_t lock1 = GASNETI_MUTEX_INITIALIZER;
  static gasneti_mutex_t lock2;
  static unsigned int counter;
  int i;

  PTHREAD_BARRIER(num_threads);

    if (!id) {
      gasneti_mutex_assertunlocked(&lock1);
      gasneti_mutex_lock(&lock1);
      gasneti_mutex_assertlocked(&lock1);
      gasneti_mutex_unlock(&lock1);
      gasneti_mutex_assertunlocked(&lock1);

      assert_always(gasneti_mutex_trylock(&lock1) == GASNET_OK);
      gasneti_mutex_assertlocked(&lock1);
      gasneti_mutex_unlock(&lock1);

      gasneti_mutex_init(&lock2);
      gasneti_mutex_assertunlocked(&lock2);
      gasneti_mutex_lock(&lock2);
      gasneti_mutex_assertlocked(&lock2);
      gasneti_mutex_unlock(&lock2);
      gasneti_mutex_assertunlocked(&lock2);
      gasneti_mutex_destroy(&lock2);
      gasneti_mutex_init(&lock2);
      gasneti_mutex_assertunlocked(&lock2);

      counter = 0;
    }

  PTHREAD_BARRIER(num_threads);

    for (i=0;i<iters2;i++) {
      if (i&1) {
        gasneti_mutex_lock(&lock1);
      } else {
        int retval;
        while ((retval=gasneti_mutex_trylock(&lock1)) != 0) {
          assert_always(retval == EBUSY);
        }
      }
        counter++;
      gasneti_mutex_unlock(&lock1);
    }

  PTHREAD_BARRIER(num_threads);

    if (counter != (num_threads * iters2)) 
      ERR("failed mutex test: counter=%i expecting=%i", counter, (num_threads * iters2));

  PTHREAD_BARRIER(num_threads);
}
/* ------------------------------------------------------------------------------------ */
#if GASNETI_HAVE_SPINLOCK
static void spinlock_test(int id) {
  static gasneti_atomic_t lock1 = GASNETI_SPINLOCK_INITIALIZER;
  static gasneti_atomic_t lock2;
  static unsigned int counter;
  int i;

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("spinlock test"); else return;

    if (!id) {
      gasneti_spinlock_lock(&lock1);
      gasneti_spinlock_unlock(&lock1);

      assert_always(gasneti_spinlock_trylock(&lock1) == GASNET_OK);
      gasneti_spinlock_unlock(&lock1);

      gasneti_spinlock_init(&lock2);
      gasneti_spinlock_lock(&lock2);
      gasneti_spinlock_unlock(&lock2);
      gasneti_spinlock_destroy(&lock2);
      gasneti_spinlock_init(&lock2);

      counter = 0;
    }

  PTHREAD_BARRIER(num_threads);

    for (i=0;i<iters2;i++) {
      if (i&1) {
        gasneti_spinlock_lock(&lock1);
      } else {
        int retval;
        while ((retval=gasneti_spinlock_trylock(&lock1))) {
          assert_always(retval == EBUSY);
        }
      }
        counter++;
      gasneti_spinlock_unlock(&lock1);
    }

  PTHREAD_BARRIER(num_threads);

    if (counter != (num_threads * iters2)) 
      ERR("failed spinlock test: counter=%i expecting=%i", counter, (num_threads * iters2));

  PTHREAD_BARRIER(num_threads);
}
#else
static void spinlock_test(int id) {
  TEST_HEADER("spinlock test - SKIPPED");
}
#endif
/* ------------------------------------------------------------------------------------ */
static void semaphore_test(int id) {
  static gasneti_semaphore_t sema1 = GASNETI_SEMAPHORE_INITIALIZER(GASNETI_SEMAPHORE_MAX,0);
  static gasneti_semaphore_t sema2;
  static gasneti_atomic_t counter;
  gasneti_atomic_val_t limit = MIN(1000000, num_threads * iters2);
  int i;

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("semaphore test"); else return;

    if (!id) {
      if (!gasneti_semaphore_trydown(&sema1))
        ERR("failed semaphore test: 'down' from GASNETI_SEMAPHORE_MAX failed");
      gasneti_semaphore_up(&sema1);

      if (gasneti_semaphore_read(&sema1) != GASNETI_SEMAPHORE_MAX)
        ERR("failed semaphore test: 'up' to GASNETI_SEMAPHORE_MAX failed");

      gasneti_semaphore_init(&sema2, limit, limit);
      if (!gasneti_semaphore_trydown(&sema2))
        ERR("failed semaphore test: trydown failed");
      if (gasneti_semaphore_trydown_n(&sema2,4) != 4)
        ERR("failed semaphore test: trydown_n failed");
      if (gasneti_semaphore_trydown_partial(&sema2,5) != 5)
        ERR("failed semaphore test: trydown_partial failed");
      gasneti_semaphore_up_n(&sema2,10);
      if (gasneti_semaphore_read(&sema2) != limit)
        ERR("failed semaphore test: up/down test failed");
      gasneti_semaphore_destroy(&sema2);
      gasneti_semaphore_init(&sema2, limit, limit);

      gasneti_atomic_set(&counter, 0, 0);
    }

  PTHREAD_BARRIER(num_threads);

    for (i=0;i<iters2;i++) {
      if (gasneti_semaphore_trydown(&sema1))
        gasneti_semaphore_up(&sema1);
    }

  PTHREAD_BARRIER(num_threads);

    while (gasneti_semaphore_trydown(&sema2))
      gasneti_atomic_increment(&counter, 0);
    if (gasneti_semaphore_trydown(&sema2))
        ERR("failed semaphore test: trydown pounding test failed");

  PTHREAD_BARRIER(num_threads);

    if (gasneti_semaphore_read(&sema1) != GASNETI_SEMAPHORE_MAX)
      ERR("failed semaphore test: trydown/up pounding test failed");
    if (gasneti_atomic_read(&counter, 0) != limit)
      ERR("failed semaphore test: trydown pounding test failed");

  PTHREAD_BARRIER(num_threads);
    if (!id) gasneti_semaphore_destroy(&sema2);
}
/* ------------------------------------------------------------------------------------ */
#if GASNETI_HAVE_ATOMIC128_T
static void atomic128_test(int id) {
  char _var128[16 + GASNETI_HAVE_ATOMIC128_T - 1]; /* Space for var128 + alignment padding */
  gasneti_atomic128_t *var128 = (gasneti_atomic128_t *)
          (((uintptr_t)_var128 + GASNETI_HAVE_ATOMIC128_T - 1) & ~(GASNETI_HAVE_ATOMIC128_T - 1));
  uint64_t readhi, readlo;
  const uint64_t one64 = 1;
  uint64_t tmp64;
  int i;

  TEST_HEADER("128-bit atomic CAS test"); else return;

  gasneti_atomic128_set(var128, 0, 2*iters, 0);
  gasneti_atomic128_read(&readhi, &readlo, var128,0);
  if ((readlo != (uint64_t)(2*iters)) || readhi)
    ERR("gasneti_atomic128_set/gasneti_atomic128_read got wrong value (lo half)");

  gasneti_atomic128_set(var128, 2*iters, 0, 0);
  gasneti_atomic128_read(&readhi, &readlo, var128,0);
  if ((readhi != (uint64_t)(2*iters)) || readlo)
    ERR("gasneti_atomic128_set/gasneti_atomic128_read got wrong value (hi half)");

  /* single bit-marching tests */
  for (i=0;i<128;i++) {
    const uint64_t tmplo = (i < 64) ? (one64<<i) : 0;
    const uint64_t tmphi = (i >= 64) ? (one64<<(i-64)) : 0;

    gasneti_atomic128_set(var128, tmphi, tmplo, 0);
    gasneti_atomic128_read(&readhi, &readlo, var128, 0);
    if ((readlo != tmplo) || (readhi != tmphi))
      ERR("gasneti_atomic128_set/gasneti_atomic128_read got wrong value on bit %i", i);
    if (gasneti_atomic128_compare_and_swap(var128, 0, 0, tmphi, tmplo, 0))
      ERR("gasneti_atomic128_compare_and_swap succeeded at bit %i when it should have failed", i);
    if (!gasneti_atomic128_compare_and_swap(var128, tmphi, tmplo, 0, 0, 0))
      ERR("gasneti_atomic128_compare_and_swap failed at bit %i when it should have succeeded", i);
  }

  /* double bit-marching tests */
  for (i=0;i<128;i++) {
    int j;
    for (j=0;j<i;j++) {
      const uint64_t tmplo_i = (i < 64) ? (one64<<i) : 0;
      const uint64_t tmphi_i = (i >= 64) ? (one64<<(i-64)) : 0;
      const uint64_t tmplo_j = (j < 64) ? (one64<<j) : 0;
      const uint64_t tmphi_j = (j >= 64) ? (one64<<(j-64)) : 0;
      const uint64_t tmplo = tmplo_i | tmplo_j;
      const uint64_t tmphi = tmphi_i | tmphi_j;

      gasneti_atomic128_set(var128, tmphi, tmplo, 0);
      if (gasneti_atomic128_compare_and_swap(var128, tmphi_i, tmplo_i, tmphi, tmplo, 0) ||
          gasneti_atomic128_compare_and_swap(var128, tmphi_j, tmplo_j, tmphi, tmplo, 0))
        ERR("gasneti_atomic128_compare_and_swap succeeded at bits %i and %i when it should have failed", i, j);
    }
  }

  gasneti_atomic128_set(var128, iters, 0, 0);
  for (i=0;i<=iters;i++) {
    if (gasneti_atomic128_compare_and_swap(var128, iters+i-1, i-1, iters+i-2, i-2, 0))
      ERR("gasneti_atomic128_compare_and_swap succeeded at i=%i when it should have failed", i);
    if (gasneti_atomic128_compare_and_swap(var128, iters+i+1, i+1, iters+i-2, i-2, 0))
      ERR("gasneti_atomic128_compare_and_swap succeeded at i=%i when it should have failed", i);
    gasneti_atomic128_read(&readhi, &readlo,var128,0);
    if (((int)readhi != iters+i) || ((int)readlo != i))
      ERR("gasneti_atomic128_compare_and_swap altered value when it should not have at i=%i", i);
    if (!gasneti_atomic128_compare_and_swap(var128, iters+i, i, iters+i+1, i+1, 0))
      ERR("gasneti_atomic128_compare_and_swap failed at i=%i when it should have succeeded", i);
    gasneti_atomic128_read(&readhi, &readlo, var128,0);
    if (((int)readhi != iters+i+1) || ((int)readlo != i+1))
      ERR("gasneti_atomic128_compare_and_swap set wrong updated value at i=%i", i);
  }
}
#else
static void atomic128_test(int id) {
  TEST_HEADER("128-bit atomic test - SKIPPED");
}
#endif
/* ------------------------------------------------------------------------------------ */
static void lifo_test(int id) {
  static gasneti_lifo_head_t lifo1 = GASNETI_LIFO_INITIALIZER;
  static gasneti_lifo_head_t lifo2;
  static gasneti_atomic_t counter;
  int limit = MIN(1000000, num_threads * iters2);
  int i;

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("lifo test"); else return;

  { 
    void * tmp = test_malloc(sizeof(void *));
    for (i = 0; i < iters2; ++i) {
      gasneti_lifo_push(&lifo1, tmp);
      tmp = gasneti_lifo_pop(&lifo1);
      if (tmp == NULL)
        ERR("failed lifo test: 1-each pop/push test failed at iteration %d", i);
    }
    PTHREAD_BARRIER(num_threads); /* See bug 2711 */
    test_free(tmp);
  }

#if 0 /* Redundant due to barrier inserted for bug2711 */
  PTHREAD_BARRIER(num_threads);
#endif
    if (!id) {
      gasneti_lifo_init(&lifo2);

      for (i = 0; i < limit; ++i)
	gasneti_lifo_push(&lifo1, test_malloc(sizeof(void *)));

      for (i = 0; i < limit; ++i) {
	void * tmp = gasneti_lifo_pop(&lifo1);
	if (tmp == NULL)
          ERR("failed lifo test: push/pop test failed");
	gasneti_lifo_push(&lifo2, tmp);
      }
      if (gasneti_lifo_pop(&lifo1) != NULL)
        ERR("failed lifo test: push/pop test failed");

      for (i = 0; i < limit; ++i) {
	void * tmp = gasneti_lifo_pop(&lifo2);
	if (tmp == NULL)
          ERR("failed lifo test: push/pop test failed");
	gasneti_lifo_push(&lifo1, tmp);
      }
      if (gasneti_lifo_pop(&lifo2) != NULL)
        ERR("failed lifo test: push/pop test failed");

      gasneti_lifo_destroy(&lifo2);
      gasneti_lifo_init(&lifo2);

      gasneti_atomic_set(&counter, 0, 0);
    }

  PTHREAD_BARRIER(num_threads);

    for (i=0;i<iters2;i++) {
      void * tmp = gasneti_lifo_pop(&lifo1);
      if (tmp != NULL) {
        gasneti_lifo_push(&lifo2, tmp);
	gasneti_atomic_increment(&counter, 0);
      }
    }

  PTHREAD_BARRIER(num_threads);

    if ((gasneti_lifo_pop(&lifo1) != NULL) || (gasneti_atomic_read(&counter, 0) != limit))
      ERR("failed lifo test: push/pop pounding test failed");

  PTHREAD_BARRIER(num_threads);

  {
    void * head = NULL;

    for (i=0;i<iters2;i++) {
      void * tmp = gasneti_lifo_pop(&lifo2);
      if (tmp != NULL) {
	*(void **)tmp = head;
	head = tmp;
	gasneti_atomic_decrement(&counter, 0);
      }
    }

    PTHREAD_BARRIER(num_threads); /* Barrier before free() for bug 2711 */
    while (head != NULL) {
      void * next = *(void **)head;
      test_free(head);
      head = next;
    }
  }

#if 0 /* Redundant due to barrier inserted for bug2711 */
  PTHREAD_BARRIER(num_threads);
#endif

    if ((gasneti_lifo_pop(&lifo2) != NULL) || (gasneti_atomic_read(&counter, 0) != 0))
      ERR("failed lifo test: push/pop pounding test failed");

  PTHREAD_BARRIER(num_threads);
    if (!id) gasneti_lifo_destroy(&lifo2);
}
/* ------------------------------------------------------------------------------------ */
static int pf_cnt_boolean, pf_cnt_counted;
static gasnet_hsl_t pf_lock = GASNET_HSL_INITIALIZER;
static gasneti_weakatomic_t progressfn_req_sent = gasneti_weakatomic_init(0);
static gasneti_weakatomic_t progressfn_rep_rcvd = gasneti_weakatomic_init(0);
static void progressfn_reqh(gasnet_token_t token, void *buf, size_t nbytes) {
  GASNET_Safe(gasnet_AMReplyMedium0(token, gasneti_diag_hidx_base + 1, buf, nbytes));
}
static void progressfn_reph(gasnet_token_t token, void *buf, size_t nbytes) {
  gasneti_weakatomic_increment(&progressfn_rep_rcvd,0);
}
static void progressfn_tester(int *counter) {
  static int active = 0; /* protocol provides mutual exclusion & recursion protection */
  int iamactive = 0;
  gasnet_hsl_lock(&pf_lock);
    (*counter)++;
    if (!active) { active = 1; iamactive = 1; }
  gasnet_hsl_unlock(&pf_lock);
  if (!iamactive) return;

  /* do some work that should be legal inside a progress fn */
  { static int tmp = 47;
    int sz;
    gasnet_put_nbi(peer, peersegmid, &tmp, sizeof(tmp));
    gasnet_get_nbi(&tmp, peer, peersegmid, sizeof(tmp));
    for (sz = 1; sz <= MIN(128*1024,TEST_SEGSZ/2); sz = (sz < 64?sz*2:sz*8)) {
      gasnet_put_nbi_bulk(peer, peersegmid, myseg, sz);
      gasnet_get_nbi_bulk(myseg, peer, peersegmid, sz);
    }
    sz = gasnet_try_syncnbi_all();
    if (gasneti_diag_havehandlers) {
      for (sz = 1; sz <= MIN(gasnet_AMMaxMedium(),MIN(64*1024,TEST_SEGSZ/2)); sz = (sz < 64?sz*2:sz*8)) {
        gasneti_weakatomic_increment(&progressfn_req_sent,0);
        gasnet_AMRequestMedium0(peer, gasneti_diag_hidx_base + 0, myseg, sz);
        gasneti_weakatomic_increment(&progressfn_req_sent,0);
        gasnet_AMRequestLong0(peer, gasneti_diag_hidx_base + 0, myseg, sz, peersegmid);
      }
    }
  }

  gasneti_local_mb();
  active = 0;
}
static void progressfn_bool(void) { progressfn_tester(&pf_cnt_boolean); }
static void progressfn_counted(void) { progressfn_tester(&pf_cnt_counted); }
static void progressfns_test(int id) {
#if !GASNET_DEBUG
  TEST_HEADER("progress functions test - SKIPPED"); else return;
  return;
#else
  int iter;
  TEST_HEADER("progress functions test"); else return;
  for (iter=0; iter < iters0; iter++) {
    int i;
    int cnt_c = 0, cnt_b = 0;

    PTHREAD_BARRIER(num_threads);
    gasneti_debug_progressfn_bool = progressfn_bool;
    gasneti_debug_progressfn_counted = progressfn_counted;
    pf_cnt_boolean = 0;
    pf_cnt_counted = 0;
    PTHREAD_BARRIER(num_threads);

    if (!id) GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_debug_boolean,BOOLEAN);
    PTHREAD_BARRIER(num_threads);
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_debug_counted,COUNTED);
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_debug_counted,COUNTED);
    GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_debug_counted,COUNTED);

    /* do some work that should cause progress fns to run */
    for (i=0; i < 2; i++) {
      int tmp;
      gasnet_put(peer, peerseg, &tmp, sizeof(tmp));
      gasnet_get(&tmp, peer, peerseg, sizeof(tmp));
      gasnet_put_bulk(peer, peersegmid, myseg, 1024);
      gasnet_get_bulk(myseg, peer, peersegmid, 1024);
      gasnet_AMPoll();
    }

    /* ensure they did run */
    cnt_c = pf_cnt_counted; cnt_b = pf_cnt_boolean;
    assert_always(cnt_c > 0); assert_always(cnt_b > 0);

    /* disable progress fns and quiesce the system */
    PTHREAD_BARRIER(num_threads);
    if (!id) GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_debug_boolean,BOOLEAN);
    GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_debug_counted,COUNTED);
    PTHREAD_BARRIER(num_threads);
    for (i=0; i < 1000; i++) { gasnet_AMPoll(); gasneti_sched_yield(); }
    PTHREAD_BARRIER(num_threads);
    cnt_c = pf_cnt_counted; cnt_b = pf_cnt_boolean;
    PTHREAD_BARRIER(num_threads);

    /* do some work that might cause progress fns to run */
    for (i=0; i < 2; i++) {
      int tmp;
      gasnet_put(peer, peerseg, &tmp, sizeof(tmp));
      gasnet_get(&tmp, peer, peerseg, sizeof(tmp));
      gasnet_put_bulk(peer, peersegmid, myseg, 1024);
      gasnet_get_bulk(myseg, peer, peersegmid, 1024);
      gasnet_AMPoll();
    }

    PTHREAD_BARRIER(num_threads);
    /* ensure they did not run */
    assert_always(cnt_c == pf_cnt_counted); assert_always(cnt_b == pf_cnt_boolean);
  }
  GASNET_BLOCKUNTIL(gasneti_weakatomic_read(&progressfn_req_sent,0) ==
                    gasneti_weakatomic_read(&progressfn_rep_rcvd,0));
#endif
}
/* ------------------------------------------------------------------------------------ */
#if GASNETI_HAVE_EOP_INTERFACE
static void op_test(int id) {
  int iter;
  GASNET_BEGIN_FUNCTION();
  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("internal op interface test"); else return;
  for (iter=0; iter < iters0; iter++) {
    static const void **share = NULL;
    int peerid = ( id + 1 ) % num_threads;

    PTHREAD_BARRIER(num_threads);
    gasnet_wait_syncnbi_all();
    PTHREAD_BARRIER(num_threads);
    if (!id) share = test_malloc(sizeof(void *)*num_threads);
    PTHREAD_BARRIER(num_threads);

    { gasneti_eop_t *eop;
      gasnet_handle_t h;
      eop = gasneti_eop_create(GASNETE_THREAD_GET_ALONE);
      assert_always(eop);
      h = gasneti_eop_to_handle(eop);
      assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
      share[id] = eop; /* hand-off eop to neighbor thread */
      PTHREAD_BARRIER(num_threads);
      gasneti_eop_markdone(share[peerid]); /* mark right neighbor's eop done */
      PTHREAD_BARRIER(num_threads);
      assert_always(gasnet_try_syncnb(h) == GASNET_OK);
    }
    PTHREAD_BARRIER(num_threads);
    { /* inc the get and put counts on my iop */
      gasneti_iop_t *iop = gasneti_iop_register(1, 0 GASNETE_THREAD_GET);
        assert_always(iop);
        assert_always(gasnet_try_syncnbi_puts() == GASNET_ERR_NOT_READY);
        assert_always(gasnet_try_syncnbi_gets() == GASNET_OK);
        assert_always(gasnet_try_syncnbi_all() == GASNET_ERR_NOT_READY);
      assert_always(iop == gasneti_iop_register(2, 1 GASNETE_THREAD_GET));
        assert_always(gasnet_try_syncnbi_puts() == GASNET_ERR_NOT_READY);
        assert_always(gasnet_try_syncnbi_gets() == GASNET_ERR_NOT_READY);
        assert_always(gasnet_try_syncnbi_all() == GASNET_ERR_NOT_READY);

      share[id] = iop; /* hand-off iop to neighbor thread */
      PTHREAD_BARRIER(num_threads);
      gasneti_iop_markdone(share[peerid], 1, 0); /* mark right neighbor's iop puts done */
      PTHREAD_BARRIER(num_threads);
        assert_always(gasnet_try_syncnbi_puts() == GASNET_OK);
        assert_always(gasnet_try_syncnbi_gets() == GASNET_ERR_NOT_READY);
        assert_always(gasnet_try_syncnbi_all() == GASNET_ERR_NOT_READY);
      PTHREAD_BARRIER(num_threads);
      gasneti_iop_markdone(share[peerid], 2, 1); /* mark right neighbor's iop gets done */
      PTHREAD_BARRIER(num_threads);
        assert_always(gasnet_try_syncnbi_puts() == GASNET_OK);
        assert_always(gasnet_try_syncnbi_gets() == GASNET_OK);
        assert_always(gasnet_try_syncnbi_all() == GASNET_OK);
    }
    PTHREAD_BARRIER(num_threads);
    { int isget;
      for (isget = 0; isget <= 1; isget++) { 
        #define ASSERT_NBI_SYNCED() (                            \
          assert_always(gasnet_try_syncnbi_puts() == GASNET_OK), \
          assert_always(gasnet_try_syncnbi_gets() == GASNET_OK), \
          assert_always(gasnet_try_syncnbi_all() == GASNET_OK) )
        #define ASSERT_NBI_NOTSYNCED() (isget? (                            \
          assert_always(gasnet_try_syncnbi_puts() == GASNET_OK),            \
          assert_always(gasnet_try_syncnbi_gets() == GASNET_ERR_NOT_READY), \
          assert_always(gasnet_try_syncnbi_all() == GASNET_ERR_NOT_READY)   \
          ) : (                                                             \
          assert_always(gasnet_try_syncnbi_puts() == GASNET_ERR_NOT_READY), \
          assert_always(gasnet_try_syncnbi_gets() == GASNET_OK),            \
          assert_always(gasnet_try_syncnbi_all() == GASNET_ERR_NOT_READY)) )

        gasneti_iop_t *iop1, *iop2;
        gasneti_iop_t *peer_iop1, *peer_iop2;
        ASSERT_NBI_SYNCED();
        iop1 = gasneti_iop_register(5, isget GASNETE_THREAD_GET); /* iop1 = 5 */
        assert_always(iop1);
        ASSERT_NBI_NOTSYNCED();
        PTHREAD_BARRIER(num_threads);
        share[id] = iop1; /* hand-off iop1 to neighbor thread */
        PTHREAD_BARRIER(num_threads);
        peer_iop1 = share[peerid];
        gasneti_iop_markdone(peer_iop1, 2, isget); /* iop1 -= 2 */
        PTHREAD_BARRIER(num_threads);
        ASSERT_NBI_NOTSYNCED();

        { /* implicit access region */
          gasnet_handle_t h;
          gasneti_eop_t *eop;
          gasnet_handle_t h2;

          gasnet_begin_nbi_accessregion();
          iop2 = gasneti_iop_register(1, isget GASNETE_THREAD_GET); /* iop2 = 1 */
          assert_always(iop2);
          assert_always(iop2 != iop1);
          PTHREAD_BARRIER(num_threads);
          share[id] = iop2; /* hand-off iop2 to neighbor thread */
          PTHREAD_BARRIER(num_threads);
          peer_iop2 = share[peerid];
          gasneti_iop_markdone(peer_iop2, 1, isget);  /* iop2 -= 1 */
          PTHREAD_BARRIER(num_threads);
          assert_always(iop2 == gasneti_iop_register(2, isget GASNETE_THREAD_GET)); /* iop2 += 2 */

          eop = gasneti_eop_create(GASNETE_THREAD_GET_ALONE);
          assert_always(eop);
          h2 = gasneti_eop_to_handle(eop);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);


          h = gasnet_end_nbi_accessregion();
          assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          gasneti_iop_markdone(peer_iop1, 2, isget); /* iop1 -= 2 */
          PTHREAD_BARRIER(num_threads);
          ASSERT_NBI_NOTSYNCED();
          assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          gasneti_iop_markdone(peer_iop2, 1, isget); /* iop2 -= 1 */
          PTHREAD_BARRIER(num_threads);
          ASSERT_NBI_NOTSYNCED();
          assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          gasneti_iop_markdone(peer_iop1, 1, isget); /* iop1 -= 1 */
          PTHREAD_BARRIER(num_threads);
          ASSERT_NBI_SYNCED();
          assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          assert_always(iop1 == gasneti_iop_register(2, isget GASNETE_THREAD_GET)); /* iop1 += 2 */
          ASSERT_NBI_NOTSYNCED();
          assert_always(gasnet_try_syncnb(h) == GASNET_ERR_NOT_READY);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          gasneti_iop_markdone(peer_iop2, 1, isget); /* iop2 -= 1 */
          PTHREAD_BARRIER(num_threads);
          ASSERT_NBI_NOTSYNCED();
          assert_always(gasnet_try_syncnb(h) == GASNET_OK);
          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);

          PTHREAD_BARRIER(num_threads);
          gasneti_iop_markdone(peer_iop1, 2, isget); /* iop1 -= 2 */
          PTHREAD_BARRIER(num_threads);
          ASSERT_NBI_SYNCED();

          assert_always(gasnet_try_syncnb(h2) == GASNET_ERR_NOT_READY);
          share[id] = eop; /* hand-off eop to neighbor thread */
          PTHREAD_BARRIER(num_threads);
          gasneti_eop_markdone(share[peerid]); /* mark right neighbor's eop done */
          PTHREAD_BARRIER(num_threads);
          assert_always(gasnet_try_syncnb(h2) == GASNET_OK);
          ASSERT_NBI_SYNCED();

          PTHREAD_BARRIER(num_threads);
        }
      }
    }
    PTHREAD_BARRIER(num_threads);
    if (!id) { test_free(share); share = NULL; }
    PTHREAD_BARRIER(num_threads);
  }
}
#else
static void op_test(int id) { 
  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("internal op interface test - SKIPPED"); else return;
}
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNET_PAR

static void * thread_fn(void *arg) {
  int test_errs = 0;
  int id = (int)(uintptr_t)arg;

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("gasneti_mutex_t test") mutex_test(id);

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("gasneti_cond_t test") cond_test(id);

  PTHREAD_BARRIER(num_threads);
  spinlock_test(id);

  PTHREAD_BARRIER(num_threads);
  semaphore_test(id);

  PTHREAD_BARRIER(num_threads);
  lifo_test(id);

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("malloc test") malloc_test(id);
  
  PTHREAD_BARRIER(num_threads);
  progressfns_test(id);

  PTHREAD_BARRIER(num_threads);
  op_test(id);

  PTHREAD_BARRIER(num_threads);
  TEST_HEADER("conduit tests") {
    PTHREAD_BARRIER(num_threads);
    test_errs += GASNETC_RUN_DIAGNOSTICS_PAR(iters,id,num_threads);
    PTHREAD_BARRIER(num_threads);
    test_errs += GASNETE_RUN_DIAGNOSTICS_PAR(iters,id,num_threads);
  }
  PTHREAD_BARRIER(num_threads);

  return (void *)(uintptr_t)test_errs;
}
#endif

static gasnet_handlerentry_t gasneti_diag_handlers[] = {
  #ifdef GASNETC_DIAG_HANDLERS
    GASNETC_DIAG_HANDLERS(), /* should start at gasnetc_diag_hidx_base */
  #endif
  #ifdef GASNETE_DIAG_HANDLERS
    GASNETE_DIAG_HANDLERS(), /* should start at gasnete_diag_hidx_base */
  #endif

  { gasneti_diag_hidx_base + 0, (gasneti_handler_fn_t)progressfn_reqh },
  { gasneti_diag_hidx_base + 1, (gasneti_handler_fn_t)progressfn_reph }
};


void gasneti_diagnostic_gethandlers(gasnet_handlerentry_t **htable, int *htable_cnt) {
  assert(htable && htable_cnt);
  *htable = gasneti_diag_handlers;
  *htable_cnt = (int)(sizeof(gasneti_diag_handlers)/sizeof(gasnet_handlerentry_t));
  gasneti_diag_havehandlers = 1;
}

