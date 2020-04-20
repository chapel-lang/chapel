/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testtools.c $
 * Description: helpers for GASNet tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#define TEST_GASNET_TOOLS_ONLY
#include "test.h"
#include <limits.h> /* For INT_MAX */

/* specifically omit gasnetex.h/test.h to test independence */
#if defined(_GASNETEX_H) || defined(TEST_GASNETEX_H)
#error testtools should *not* include gasnetex.h
#endif

#if GASNETT_THREAD_SAFE
  int NUM_THREADS = 0;
  gasnett_atomic_t thread_flag[TEST_MAXTHREADS];
  int valX[TEST_MAXTHREADS];
  int valY[TEST_MAXTHREADS];
  gasnett_atomic_t atomicX[TEST_MAXTHREADS];
  int32_t valX32[TEST_MAXTHREADS];
  int32_t valY32[TEST_MAXTHREADS];
  gasnett_atomic32_t atomicX32[TEST_MAXTHREADS];
  int64_t valX64[TEST_MAXTHREADS];
  int64_t valY64[TEST_MAXTHREADS];
  gasnett_atomic64_t atomicX64[TEST_MAXTHREADS];
#endif

#define DEFAULT_THREADS 10
#define DEFAULT_ITERS 100
int iters = 0;
#define TEST_HEADER_PREFIX() ((void)0)
#define TEST_HEADER(desc)             \
  TEST_HEADER_PREFIX();               \
  if (TEST_SECTION_BEGIN_ENABLED() && \
      (MSG0("%c: %s",TEST_SECTION_NAME(),desc),1))

TEST_BACKTRACE_DECLS();

void * thread_fn(void *arg);

/* test gasnet tools modifier convenience macros */
GASNETT_INLINE(test_dummy)
void test_dummy(void * GASNETT_RESTRICT p) {}

void test_dummy2(void) GASNETT_NORETURN;
GASNETT_NORETURNP(test_dummy2)
void test_dummy2(void) { gasnett_fatalerror("test_dummy2"); }

GASNETT_EXTERNC void test_dummy5(void);

GASNETT_BEGIN_EXTERNC
void *test_dummy3(void) GASNETT_MALLOC;
void *test_dummy3(void) { return malloc(1); }
GASNETT_INLINE(test_dummy4) GASNETT_MALLOC
void *test_dummy4(void) { return malloc(1); }
void test_dummy5(void) { }
GASNETT_END_EXTERNC
double volatile d_junk = 0;


GASNETT_THREADKEY_DECLARE(sertest_key1);
GASNETT_THREADKEY_DEFINE(sertest_key1);
GASNETT_THREADKEY_DEFINE(sertest_key2);
GASNETT_THREADKEY_DEFINE(partest_key1);
GASNETT_THREADKEY_DEFINE(partest_key2);
#define test_threadkeys(key1,key2) do {            \
  void *val = gasnett_threadkey_get(key1);         \
  assert_always(val == NULL);                      \
  gasnett_threadkey_set(key1,(void *)&val);        \
  val = gasnett_threadkey_get(key1);               \
  assert_always(val == &val);                      \
                                                   \
  gasnett_threadkey_init(key2);                    \
  val = gasnett_threadkey_get_noinit(key2);        \
  assert_always(val == NULL);                      \
  gasnett_threadkey_set_noinit(key2,(void *)&val); \
  val = gasnett_threadkey_get_noinit(key2);        \
  assert_always(val == &val);                      \
  gasnett_threadkey_init(key2);                    \
  val = gasnett_threadkey_get_noinit(key2);        \
  assert_always(val == &val);                      \
} while (0)

int main(int argc, char **argv) {
  /* avoid unused function warnings */
  uintptr_t test_dummies =
          (uintptr_t)&test_dummy  ^
          (uintptr_t)&test_dummy2 ^
          (uintptr_t)&test_dummy3 ^
          (uintptr_t)&test_dummy4 ^
          (uintptr_t)&test_dummy5 ^
          (uintptr_t)&test_dummies;

  test_init("testtools", 0,"(iters) (num_threads) (tests_to_run)");

  TEST_BACKTRACE_INIT(argv[0]);
  
  if (argc > 1) iters = atoi(argv[1]);
  if (iters < 1) iters = DEFAULT_ITERS;
  #if GASNETT_THREAD_SAFE
    if (argc > 2) NUM_THREADS = atoi(argv[2]);
    if (NUM_THREADS < 1) NUM_THREADS = DEFAULT_THREADS;
    NUM_THREADS = test_thread_limit(NUM_THREADS);
  #else
    if (argc > 2 && atoi(argv[2]) != 1) { ERR("no pthreads - only one thread available."); test_usage(); }
  #endif
  if (argc > 3) TEST_SECTION_PARSE(argv[3]);
  if (argc > 4) test_usage();

  TEST_GENERICS_WARNING();
  #if GASNETT_THREAD_SAFE
    MSG("Running testtools with %i iterations and %i threads", iters, NUM_THREADS);
  #else
    MSG("Running testtools with %i iterations", iters);
  #endif

  #if    PLATFORM_ARCH_32 && !PLATFORM_ARCH_64
    assert_always(sizeof(void*) == 4);
  #elif !PLATFORM_ARCH_32 &&  PLATFORM_ARCH_64
    assert_always(sizeof(void*) == 8);
  #else
    #error must #define exactly one of PLATFORM_ARCH_32 or PLATFORM_ARCH_64
  #endif

  { int smaj = GASNETT_SPEC_VERSION_MAJOR;
    int smin = GASNETT_SPEC_VERSION_MINOR;
    int rmaj = GASNETT_RELEASE_VERSION_MAJOR;
    int rmin = GASNETT_RELEASE_VERSION_MINOR;
    int rpat = GASNETT_RELEASE_VERSION_PATCH;
    assert_always(smaj > 0 && smin >= 0 && rmaj > 0 && rmin >= 0 && rpat >= 0);
    uint64_t ver = rmaj * (uint64_t)1000000 + rmin * (uint64_t)10000 + rpat;
    assert_always(ver == gasnett_release_version());
    const char *version_str = _STRINGIFY(GASNET_RELEASE_VERSION_MAJOR) "." 
                              _STRINGIFY(GASNET_RELEASE_VERSION_MINOR) "." 
                              _STRINGIFY(GASNET_RELEASE_VERSION_PATCH);
    assert_always(!strcmp(version_str, gasnett_release_version_str()));
  }

  #if defined(GASNETT_PAGESIZE) && defined(GASNETT_PAGESHIFT)
    if (0x1 << GASNETT_PAGESHIFT != GASNETT_PAGESIZE)
      ERR("bad pagesizes: GASNETT_PAGESHIFT=%i GASNETT_PAGESIZE=%i",
              GASNETT_PAGESHIFT, GASNETT_PAGESIZE);
    else 
      MSG("System page size is 2^%i == %i", GASNETT_PAGESHIFT, GASNETT_PAGESIZE);
  #endif

  { int cpucnt = gasnett_cpu_count();
    MSG("CPU count estimated to be: %i", cpucnt);
    assert_always(cpucnt >= 0);
  }

  MSG("Cache line size estimated to be: %i", GASNETT_CACHE_LINE_BYTES);
  if ((GASNETT_CACHE_LINE_BYTES & (GASNETT_CACHE_LINE_BYTES-1)) != 0)
        ERR("GASNETT_CACHE_LINE_BYTES not a power of two!");

  { uint64_t val = gasnett_getPhysMemSz(0);
    char sz_str[50];
    gasnett_format_number(val, sz_str, sizeof(sz_str), 1);
    if (val == 0) MSG("WARNING: gasnett_getPhysMemSz() failed to discover physical memory size.");
    else {
      MSG("Physical memory size estimated to be: %s", sz_str);
      if (val > (1ULL<<50) || val < (1ULL<<20)) 
        ERR("gasnett_getPhysMemSz() got a ridiculous result: %" PRIu64 " bytes", val);
    }
  }

  { char tmp_str[50];
    gasnett_format_number(0, tmp_str, sizeof(tmp_str), 1);
    assert_always(gasnett_parse_int(tmp_str, 1) == 0);
    gasnett_format_number(0, tmp_str, sizeof(tmp_str), 0);
    assert_always(gasnett_parse_int(tmp_str, 0) == 0);
    for (int i=0; i < 62; i++) {
      int64_t x = (((int64_t)1) << i);
      int64_t y;
      gasnett_format_number(x, tmp_str, sizeof(tmp_str), 1);
      y = gasnett_parse_int(tmp_str, 1);
      if (x != y) ERR("gasnett_format_number/gasnett_parse_int memsz mismatch: %" PRId64 " != %" PRId64 " (%s)",
                      x, y, tmp_str);
      gasnett_format_number(x, tmp_str, sizeof(tmp_str), 0);
      y = gasnett_parse_int(tmp_str, 0);
      if (x != y) ERR("gasnett_format_number/gasnett_parse_int mismatch: %" PRId64 " != %" PRId64 " (%s)",
                      x, y, tmp_str);
    }
  }

  gasnett_sched_yield();
  gasnett_flush_streams();
  gasnett_maximize_rlimits();
  TEST_TRACING_MACROS();

  TEST_HEADER("Testing high-performance timers and sleep...")
  { /* high performance timers */
    int timeiters = MAX(1,iters / 10);
    gasnett_tick_t ticktimemin = GASNETT_TICK_MIN;
    gasnett_tick_t ticktimemax = GASNETT_TICK_MAX;
    #if PLATFORM_OS_CYGWIN || PLATFORM_OS_WSL
      // bug 2410: avoid false negatives due to cygwin's high gettimeofday() reference timer granularity
      double default_slack = 0.05; // 50 ms for cygwin
    #else
      double default_slack = 0.01; // 10 ms, sufficient for most platforms
    #endif
    double slack = gasnett_getenv_dbl_withdefault("GASNET_TEST_TIME_SLACK", default_slack);

    double overhead = gasnett_tick_overheadus();
    double granularity = gasnett_tick_granularityus();

    /* Aiming for 'total' microseconds of busy waits, with 25% of it
       in the last iteration, but need a non-trivial number of ticks
       to pass int each iteration. */
    uint64_t total = 20 * 1000000;
    uint64_t us_delay = (uint64_t)((0.75*total) / timeiters);
    uint64_t min_delay = (uint64_t)MAX(1, 50*granularity);
    if (us_delay < min_delay) {
       us_delay = min_delay;
       timeiters = (int)((0.75*total) / min_delay);
    }

    if (!(ticktimemin < ticktimemax)) ERR("!(min < max)");
    if (!(gasnett_ticks_now() > ticktimemin)) ERR("!(now > min)");
    if (!(gasnett_ticks_now() < ticktimemax)) ERR("!(now < max)");

    if (granularity <= 0.0 || overhead <= 0.0) {
        ERR("nonsensical timer overhead/granularity measurements:\n"
             "  overhead: %.3fus  granularity: %.3fus\n",overhead, granularity);
    } else if ((granularity+10*slack) < 0.5*overhead) {
        /* allow some leeway for noise at granularities approaching cycle speed */
        // leeway is scaled by slack to allow disabling this test
        // on platforms where timers are unreliable (eg cpu emulator)
        MSG("WARNING: suspicious timer overhead/granularity measurements: (this can be caused by high system noise)\n"
             "  overhead: %.3fus  granularity: %.3fus\n",overhead, granularity);
    }

    gasnett_tick_t start, begin = gasnett_ticks_now();  /* outer time point */
    uint64_t startref, beginref = gasnett_gettimeofday_us();
    for (int i=0; i < timeiters; i++) {
      if (i == timeiters - 1) {
        start = begin; /* use outer time point for base of last iteration */
        startref = beginref;
        us_delay = total; /* consume the remainder (about 25%) of the total interval */
      } else {
        start = gasnett_ticks_now(); /* inner time point */
        startref = gasnett_gettimeofday_us();
      }
      if (i % MAX(1,timeiters/3) == 0) sleep(1); /* sleep wait */
      { /* busy wait */
        gasnett_tick_t last = start;
        do {
          gasnett_tick_t next = gasnett_ticks_now();
          if (next < last) 
            ERR("gasnett_ticks_to_us not monotonic! !(%" PRIu64 " <= %" PRIu64 ")",
                 (uint64_t)last, (uint64_t)next);
          if (next <= GASNETT_TICK_MIN) 
            ERR("gasnett_ticks_to_us()=%" PRIu64 " <= GASNETT_TICK_MIN=%" PRIu64,
                 (uint64_t)next, (uint64_t)GASNETT_TICK_MIN);
          if (next >= GASNETT_TICK_MAX) 
            ERR("gasnett_ticks_to_us()=%" PRIu64 " >= GASNETT_TICK_MAX=%" PRIu64,
                 (uint64_t)next, (uint64_t)GASNETT_TICK_MAX);
          d_junk *= 1.0001;
          last = next;
        } while (gasnett_ticks_to_us(last-start) < us_delay);
      }
      gasnett_tick_t end = gasnett_ticks_now();
      uint64_t endref = gasnett_gettimeofday_us();

      int time = gasnett_ticks_to_us(end) - gasnett_ticks_to_us(start);
      int timeref = endref - startref;

      if (abs(timeref - time) > (int)(slack * 1.e6))
        ERR("timer and reference differ by more than %g sec:\n"
               "\ttime=%i  timeref=%i  delta=%g sec\n",
               slack,time,timeref,1.e-6*abs(timeref - time));

      if (abs( (int)((gasnett_ticks_to_us(end) - gasnett_ticks_to_us(start)) - 
                      gasnett_ticks_to_us(end - start)) ) > 1)
        ERR("ticks_to_us(A) - ticks_to_us(B) != ticks_to_us(A-B)");

      if (abs( (int)(gasnett_ticks_to_ns(end - start)/1000 - 
                     gasnett_ticks_to_us(end - start)) ) > 1)
        ERR("ticks_to_ns(A)/1000 != ticks_to_us(A)");

    }

    for (uint64_t ns_delay = 10; ns_delay < (uint64_t)2e9; ns_delay *= 14) {
      gasnett_tick_t start = gasnett_ticks_now();
      int rc = gasnett_nsleep(ns_delay);
      gasnett_tick_t end = gasnett_ticks_now();
      if (rc) ERR("gasnett_nsleep returned non-zero");
      double elapsed_plus = gasnett_ticks_to_ns(end - start)
                            + 0.0005 * ns_delay
                            + 1000 * granularity;
      if (elapsed_plus < ns_delay)
        ERR("gasnett_nsleep(%" PRIu64 ") returned at least %" PRIu64 " nanoseconds too early",
            ns_delay, (uint64_t)(ns_delay - elapsed_plus));
    }
  }

  TEST_HEADER("Testing zero-byte counting...")
  { /* gasnett_count0s*() */
    static const char src[24] = { '\0', '1',  '\0', '2', '\0', '3', '\0', '4',
                                  '5',  '6',  '7',  '8', '\0', '9', '\0', 'a',
                                  'b',  '\0', 'c',  'd', 'e',  'f', 'g',  'h' };
    char dst_guarded[24+16];
    char *dst = dst_guarded+8;

    for (int i=0;i<8;++i) { /* src alignment */
      for (int j=0;j<8;++j) { /* dst alignment */
        for (int l=0;l<16;++l) { /* length */
          memset(dst_guarded, 0, sizeof(dst_guarded));
	  int z0 = gasnett_count0s_copy(dst+j, src+i, l);
	  if (memcmp(dst+j, src+i, l)) ERR("memory mismatch from gasnett_count0s_copy(dst+%i, src+%i, %i)",i,j,l);
	  int z1 = gasnett_count0s(dst+j, l);
          int z2,k;
	  for (z2=0,k=0;k<l;++k) { z2 += !dst[j+k]; }
	  if (z0 != z2) ERR("incorrect return value from gasnett_count0s_copy(dst+%i, src+%i, %i) (got %i want %i)",i,j,l,z0,z2);
	  if (z1 != z2) ERR("incorrect return value from gasnett_count0s(dst+%i, %i) src+%i (got %i want %i)",j,l,i,z1,z0);
	  if (dst[j-1] || dst[j+l])
	    ERR("memory clobbered by gasnett_count0s_copy(dst+%i, src+%i, %i)",i,j,l);
        }
      }
    }

    { /* Long strings of zeros and non-zeros */
      #define CNT0SMAX 4096
      char *src = (char *)test_malloc(CNT0SMAX);
      memset(src, 0, CNT0SMAX);
      for (int i=0;i<=CNT0SMAX;++i)
	if (gasnett_count0s(src, i) != (unsigned)i)
          ERR("incorrect return from gasnett_count0s(string-of-%i-zeros)", i);
      memset(src, 1, CNT0SMAX);
      for (int i=0;i<=CNT0SMAX;++i)
	if (gasnett_count0s(src, i) != 0)
          ERR("incorrect return from gasnett_count0s(string-of-%i-nonzeros)", i);
      test_free(src);
    }

    for (int i=0;i<8*(int)sizeof(uintptr_t);++i) {
      uintptr_t val = ((uintptr_t)1) << i;
      if (gasnett_count0s_uintptr_t(val) != (sizeof(uintptr_t) - 1))
        ERR("incorrect return from gasnett_count0s_uintptr_t(1<<%i)", i);
    }
    if (gasnett_count0s_uintptr_t(0) != sizeof(uintptr_t))
      ERR("incorrect return from gasnett_count0s_uintptr_t(0)");

    for (int i=0;i<32;++i) {
      uint32_t val = ((uint32_t)1) << i;
      if (gasnett_count0s_uint32_t(val) != 3)
        ERR("incorrect return from gasnett_count0s_uint32_t(1<<%i)", i);
    }
    if (gasnett_count0s_uint32_t(0) != 4)
      ERR("incorrect return from gasnett_count0s_uint32_t(0)");

    for (int i=0;i<64;++i) {
      uint64_t val = ((uint64_t)1) << i;
      if (gasnett_count0s_uint64_t(val) != 7)
        ERR("incorrect return from gasnett_count0s_uint64_t(1<<%i)", i);
    }
    if (gasnett_count0s_uint64_t(0) != 8)
      ERR("incorrect return from gasnett_count0s_uint64_t(0)");
  }

  TEST_HEADER("Testing local membars...")
  { /* local membar */
    for (int i=0;i<iters;i++) {
      gasnett_local_mb();
    }
    for (int i=0;i<iters;i++) {
      gasnett_weak_mb();
    }
  }

  TEST_HEADER("Testing local write membars...")
  { /* local membar */
    for (int i=0;i<iters;i++) {
      gasnett_local_wmb();
    }
    for (int i=0;i<iters;i++) {
      gasnett_weak_wmb();
    }
  }

  TEST_HEADER("Testing local read membars...")
  { /* local membar */
    for (int i=0;i<iters;i++) {
      gasnett_local_rmb();
    }
    for (int i=0;i<iters;i++) {
      gasnett_weak_rmb();
    }
  }

  TEST_HEADER("Testing threadkey (sequential)...")
  { /* serial threadkey test */
    test_threadkeys(sertest_key1,sertest_key2);
  }

  TEST_HEADER("Testing atomic ops (sequential)...")
  { /* we can't really test atomicity without spinning threads, 
       but we can at least test simple operations  */
    gasnett_atomic_t var = gasnett_atomic_init(10);

    if (gasnett_atomic_read(&var,0) != 10)
      ERR("gasnett_atomic_init/gasnett_atomic_read got wrong value");

    gasnett_atomic_set(&var, 2*iters, 0);
    if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters))
      ERR("gasnett_atomic_set/gasnett_atomic_read got wrong value");

    for (int i=0;i<iters;i++) {
      gasnett_atomic_increment(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters + (i+1)))
        ERR("gasnett_atomic_increment got wrong value");
    }

    for (int i=iters-1;i>=0;i--) {
      gasnett_atomic_decrement(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters + i))
        ERR("gasnett_atomic_decrement got wrong value");
    }

    for (int i=0;i<iters;i++) {
      gasnett_atomic_set(&var, i, 0);
      gasnett_atomic_increment(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i+1))
        ERR("gasnett_atomic_set/gasnett_atomic_increment got wrong value");
    }

    for (int i=0;i<iters;i++) {
      gasnett_atomic_set(&var, i, 0);
      gasnett_atomic_decrement(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i-1))
        ERR("gasnett_atomic_set/gasnett_atomic_decrement got wrong value");
    }

    gasnett_atomic_set(&var, iters, 0);
    for (int i=iters-1;i>=1;i--) {
      if (gasnett_atomic_decrement_and_test(&var,0))
        ERR("gasnett_atomic_decrement_and_test got wrong value");
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i))
        ERR("gasnett_atomic_decrement_and_test set wrong value");
    }
    if (!gasnett_atomic_decrement_and_test(&var,0))
      ERR("gasnett_atomic_decrement_and_test got wrong value at zero");
    if (gasnett_atomic_read(&var,0) != 0)
      ERR("gasnett_atomic_decrement_and_test set wrong value at zero");

    #if defined(GASNETT_HAVE_ATOMIC_CAS)
      gasnett_atomic_set(&var, 0, 0);
      for (int i=0;i<iters;i++) {
	if (gasnett_atomic_compare_and_swap(&var, i-1, i-2, 0))
          ERR("gasnett_atomic_compare_and_swap succeeded at i=%i when it should have failed", i);
	if (gasnett_atomic_compare_and_swap(&var, i+1, i-2, 0))
          ERR("gasnett_atomic_compare_and_swap succeeded at i=%i when it should have failed", i);
        if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i))
          ERR("gasnett_atomic_compare_and_swap altered value when it should not have at i=%i", i);
	if (!gasnett_atomic_compare_and_swap(&var, i, i+1, 0))
          ERR("gasnett_atomic_compare_and_swap failed at i=%i when it should have succeeded", i);
        if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i+1))
          ERR("gasnett_atomic_compare_and_swap set wrong updated value at i=%i", i);
      }

      gasnett_atomic_set(&var, 0, 0);
      for (int i=0;i<iters;i++) {
        if (gasnett_atomic_swap(&var,i+1,0) != (gasnett_atomic_val_t)(i))
          ERR("gasnett_atomic_swap test failed at iteration %i", i);
      }
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(iters))
        ERR("gasnett_atomic_swap test failed at iteration %i", iters);
    #endif

    #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      gasnett_atomic_set(&var, 1, 0);
      for (int i=1;i<iters;i++) {
        if ((gasnett_atomic_add(&var, i, 0) != (gasnett_atomic_val_t)(2*i)) ||
            (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*i)))
          ERR("gasnett_atomic_add got wrong value");
        if ((gasnett_atomic_subtract(&var, i-1, 0) != (gasnett_atomic_val_t)(i+1)) ||
            (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i+1)))
          ERR("gasnett_atomic_subtract got wrong value");
      }
    #endif

    /* Verify "reachability" of limit values */
    gasnett_atomic_set(&var, GASNETT_ATOMIC_MAX, 0);
    if (gasnett_atomic_read(&var,0) != GASNETT_ATOMIC_MAX)
        ERR("gasnett_atomic_set/read could not handle GASNETT_ATOMIC_MAX");
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_read(&var,0) != GASNETT_ATOMIC_MAX - 1)
        ERR("gasnett_atomic_decrement could not leave GASNETT_ATOMIC_MAX");
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_read(&var,0) != GASNETT_ATOMIC_MAX)
        ERR("gasnett_atomic_increment could not reach GASNETT_ATOMIC_MAX");

    gasnett_atomic_set(&var, (gasnett_atomic_val_t)(-1), 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != -1)
        ERR("gasnett_atomic_set/signed could not handle -1");
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_read(&var,0) != 0)
        ERR("gasnett_atomic_increment could not leave -1");
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != -1)
        ERR("gasnett_atomic_decrement could not reach -1");

    gasnett_atomic_set(&var, (gasnett_atomic_val_t)GASNETT_ATOMIC_SIGNED_MIN, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MIN)
        ERR("gasnett_atomic_set/signed could not handle GASNETT_ATOMIC_SIGNED_MIN");
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MIN + 1)
        ERR("gasnett_atomic_increment could not leave GASNETT_ATOMIC_SIGNED_MIN");
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MIN)
        ERR("gasnett_atomic_decrement could not reach GASNETT_ATOMIC_SIGNED_MIN");

    gasnett_atomic_set(&var, GASNETT_ATOMIC_SIGNED_MAX, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MAX)
        ERR("gasnett_atomic_set/signed could not handle GASNETT_ATOMIC_SIGNED_MAX");
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MAX - 1)
        ERR("gasnett_atomic_decrement could not leave GASNETT_ATOMIC_SIGNED_MAX");
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MAX)
        ERR("gasnett_atomic_increment could not reach GASNETT_ATOMIC_SIGNED_MAX");

   /* Verify expected two's-complement wrap-around properties */
    gasnett_atomic_set(&var, GASNETT_ATOMIC_MAX, 0);
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_read(&var,0) != 0)
        ERR("failed unsigned wrap-around at GASNETT_ATOMIC_MAX");
    gasnett_atomic_set(&var, 0, 0);
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_read(&var,0) != GASNETT_ATOMIC_MAX)
        ERR("failed unsigned wrap-around at 0");
    gasnett_atomic_set(&var, GASNETT_ATOMIC_SIGNED_MAX, 0);
    gasnett_atomic_increment(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MIN)
        ERR("failed signed wrap-around at GASNETT_ATOMIC_SIGNED_MAX");
    gasnett_atomic_set(&var, (gasnett_atomic_val_t)GASNETT_ATOMIC_SIGNED_MIN, 0);
    gasnett_atomic_decrement(&var, 0);
    if (gasnett_atomic_signed(gasnett_atomic_read(&var,0)) != GASNETT_ATOMIC_SIGNED_MAX)
        ERR("failed signed wrap-around at GASNETT_ATOMIC_SIGNED_MIN");

    #if defined(GASNETT_HAVE_ATOMIC_CAS)
    { /* Use a couple temporaries to avoid warnings
         about our intentional overflow/underflow. */
      gasnett_atomic_val_t utemp;
      gasnett_atomic_sval_t stemp;

      /* Verify expected wrap-around properties of "oldval" in c-a-s */
      gasnett_atomic_set(&var, GASNETT_ATOMIC_MAX, 0);
      utemp = 0;
      if (!gasnett_atomic_compare_and_swap(&var, utemp - 1, 0, 0))
        ERR("gasnett_atomic_compare_and_swap failed unsigned wrap-around at oldval=-1");

      gasnett_atomic_set(&var, 0, 0);
      utemp = GASNETT_ATOMIC_MAX;
      if (!gasnett_atomic_compare_and_swap(&var, utemp + 1, 0, 0))
        ERR("gasnett_atomic_compare_and_swap failed unsigned wrap-around at oldval=MAX+1");

      gasnett_atomic_set(&var, GASNETT_ATOMIC_SIGNED_MAX, 0);
      stemp = GASNETT_ATOMIC_SIGNED_MIN;
      if (!gasnett_atomic_compare_and_swap(&var, stemp - 1, 0, 0))
        ERR("gasnett_atomic_compare_and_swap failed signed wrap-around at oldval=SIGNED_MIN-1");

      gasnett_atomic_set(&var, (gasnett_atomic_val_t)GASNETT_ATOMIC_SIGNED_MIN, 0);
      stemp = GASNETT_ATOMIC_SIGNED_MAX;
      if (!gasnett_atomic_compare_and_swap(&var, stemp + 1, 0, 0))
        ERR("gasnett_atomic_compare_and_swap failed signed wrap-around at oldval=SIGNED_MAX+1");
    }
    #endif

    {
      gasnett_atomic32_t var32 = gasnett_atomic32_init(~(uint32_t)0);
      const uint32_t one32 = 1;

      if (~gasnett_atomic32_read(&var32,0) != 0)
        ERR("gasnett_atomic32_init/gasnett_atomic32_read got wrong value");

      gasnett_atomic32_set(&var32, 2*iters, 0);
      if (gasnett_atomic32_read(&var32,0) != (uint32_t)(2*iters))
        ERR("gasnett_atomic32_set/gasnett_atomic32_read got wrong value");

      /* single bit-marching tests */
      for (int i=0;i<32;i++) {
        gasnett_atomic32_set(&var32, one32<<i, 0);
	uint32_t tmp32 = gasnett_atomic32_read(&var32, 0);
	if (tmp32 != (one32<<i))
          ERR("gasnett_atomic32_set/gasnett_atomic32_read got wrong value on bit %i", i);
	if (gasnett_atomic32_compare_and_swap(&var32, 0, tmp32, 0))
          ERR("gasnett_atomic32_compare_and_swap succeeded at bit %i when it should have failed", i);
	if (!gasnett_atomic32_compare_and_swap(&var32, tmp32, 0, 0))
          ERR("gasnett_atomic32_compare_and_swap failed at bit %i when it should have succeeded", i);
      }

      /* double bit-marching tests */
      for (int i=0;i<32;i++) {
        for (int j=0;j<i;j++) {
          uint32_t tmp32 = (one32<<i) | (one32<<j);
          gasnett_atomic32_set(&var32, tmp32, 0);
          if (gasnett_atomic32_compare_and_swap(&var32, (one32<<i), tmp32, 0) ||
              gasnett_atomic32_compare_and_swap(&var32, (one32<<j), tmp32, 0))
            ERR("gasnett_atomic32_compare_and_swap succeeded at bits %i and %i when it should have failed", i, j);
        }
      }

      gasnett_atomic32_set(&var32, 0, 0);
      for (int i=0;i<iters;i++) {
	if (gasnett_atomic32_compare_and_swap(&var32, i-1, i-2, 0))
          ERR("gasnett_atomic32_compare_and_swap succeeded at i=%i when it should have failed", i);
	if (gasnett_atomic32_compare_and_swap(&var32, i+1, i-2, 0))
          ERR("gasnett_atomic32_compare_and_swap succeeded at i=%i when it should have failed", i);
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)i)
          ERR("gasnett_atomic32_compare_and_swap altered value when it should not have at i=%i", i);
	if (!gasnett_atomic32_compare_and_swap(&var32, i, i+1, 0))
          ERR("gasnett_atomic32_compare_and_swap failed at i=%i when it should have succeeded", i);
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+1))
          ERR("gasnett_atomic32_compare_and_swap set wrong updated value at i=%i", i);
      }

      /* TODO: Want more than this very simple test of SWAP and the arithmetic tests. */
      gasnett_atomic32_set(&var32, 0, 0);
      for (int i=0;i<iters;i++) {
        gasnett_atomic32_increment(&var32,0);
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+1))
          ERR("gasnett_atomic32_increment wrote wrong value");
        if (gasnett_atomic32_add(&var32,4,0)  != (uint32_t)(i+5))
          ERR("gasnett_atomic32_add returned wrong value");
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+5))
          ERR("gasnett_atomic32_add wrote wrong value");
        gasnett_atomic32_decrement(&var32,0);
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+4))
          ERR("gasnett_atomic32_decrement wrote wrong value");
        if (gasnett_atomic32_decrement_and_test(&var32,0))
          ERR("gasnett_atomic32_decrement_and_test succeeded when expecting failure");
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+3))
          ERR("gasnett_atomic32_decrement_and_test wrote wrong value on failure");
        if (gasnett_atomic32_subtract(&var32,(i+2),0) != (uint32_t)1)
          ERR("gasnett_atomic32_subtract returned wrong value");
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)1)
          ERR("gasnett_atomic32_subtract wrote wrong value");
        if (! gasnett_atomic32_decrement_and_test(&var32,0))
          ERR("gasnett_atomic32_decrement_and_test failed when expecting success");
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)0)
          ERR("gasnett_atomic32_decrement_and_test wrote wrong value on sucess");
        if (gasnett_atomic32_swap(&var32,i+1,0) != 0)
          ERR("gasnett_atomic32_swap returned wrong value");
        if (gasnett_atomic32_read(&var32,0) != (uint32_t)(i+1))
          ERR("gasnett_atomic32_swap wrote wrong value");
      }
    }

    {
      gasnett_atomic64_t var64 = gasnett_atomic64_init(~(uint64_t)0);
      const uint64_t one64 = 1;

      if (~gasnett_atomic64_read(&var64,0) != 0)
        ERR("gasnett_atomic64_init/gasnett_atomic64_read got wrong value");

      gasnett_atomic64_set(&var64, 2*iters, 0);
      if (gasnett_atomic64_read(&var64,0) != (uint64_t)(2*iters))
        ERR("gasnett_atomic64_set/gasnett_atomic64_read got wrong value");

      /* single bit-marching tests */
      for (int i=0;i<64;i++) {
        gasnett_atomic64_set(&var64, one64<<i, 0);
	uint64_t tmp64 = gasnett_atomic64_read(&var64, 0);
	if (tmp64 != (one64<<i))
          ERR("gasnett_atomic64_set/gasnett_atomic64_read got wrong value on bit %i", i);
	if (gasnett_atomic64_compare_and_swap(&var64, 0, tmp64, 0))
          ERR("gasnett_atomic64_compare_and_swap succeeded at bit %i when it should have failed", i);
	if (!gasnett_atomic64_compare_and_swap(&var64, tmp64, 0, 0))
          ERR("gasnett_atomic64_compare_and_swap failed at bit %i when it should have succeeded", i);
      }

      /* double bit-marching tests */
      for (int i=0;i<64;i++) {
        for (int j=0;j<i;j++) {
          uint64_t tmp64 = (one64<<i) | (one64<<j);
          gasnett_atomic64_set(&var64, tmp64, 0);
          if (gasnett_atomic64_compare_and_swap(&var64, (one64<<i), tmp64, 0) ||
              gasnett_atomic64_compare_and_swap(&var64, (one64<<j), tmp64, 0))
            ERR("gasnett_atomic64_compare_and_swap succeeded at bits %i and %i when it should have failed", i, j);
        }
      }

      gasnett_atomic64_set(&var64, 0, 0);
      for (int i=0;i<iters;i++) {
	if (gasnett_atomic64_compare_and_swap(&var64, i-1, i-2, 0))
          ERR("gasnett_atomic64_compare_and_swap succeeded at i=%i when it should have failed", i);
	if (gasnett_atomic64_compare_and_swap(&var64, i+1, i-2, 0))
          ERR("gasnett_atomic64_compare_and_swap succeeded at i=%i when it should have failed", i);
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)i)
          ERR("gasnett_atomic64_compare_and_swap altered value when it should not have at i=%i", i);
	if (!gasnett_atomic64_compare_and_swap(&var64, i, i+1, 0))
          ERR("gasnett_atomic64_compare_and_swap failed at i=%i when it should have succeeded", i);
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+1))
          ERR("gasnett_atomic64_compare_and_swap set wrong updated value at i=%i", i);
      }

      /* TODO: Want more than this very simple test of SWAP and the arithmetic tests. */
      gasnett_atomic64_set(&var64, 0, 0);
      for (int i=0;i<iters;i++) { /* Test in lo word */
        gasnett_atomic64_increment(&var64,0);
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+1))
          ERR("gasnett_atomic64_increment wrote wrong value");
        if (gasnett_atomic64_add(&var64,4,0) != (uint64_t)(i+5))
          ERR("gasnett_atomic64_add returned wrong value");
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+5))
          ERR("gasnett_atomic64_add wrote wrong value");
        gasnett_atomic64_decrement(&var64,0);
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+4))
          ERR("gasnett_atomic64_decrement wrote wrong value");
        if (gasnett_atomic64_decrement_and_test(&var64,0))
          ERR("gasnett_atomic64_decrement_and_test succeeded when expecting failure");
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+3))
          ERR("gasnett_atomic64_decrement_and_test wrote wrong value on failure");
        if (gasnett_atomic64_subtract(&var64,(i+2),0) != (uint64_t)1)
          ERR("gasnett_atomic64_subtract returned wrong value");
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)1)
          ERR("gasnett_atomic64_subtract wrote wrong value");
        if (! gasnett_atomic64_decrement_and_test(&var64,0))
          ERR("gasnett_atomic64_decrement_and_test failed when expecting success");
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)0)
          ERR("gasnett_atomic64_decrement_and_test wrote wrong value on sucess");
        if (gasnett_atomic64_swap(&var64,i+1,0) != 0)
          ERR("gasnett_atomic64_swap returned wrong value");
        if (gasnett_atomic64_read(&var64,0) != (uint64_t)(i+1))
          ERR("gasnett_atomic64_swap wrote wrong value");
      }
      gasnett_atomic64_set(&var64, 0, 0);
      for (int i=0;i<iters;i++) { /* Test in hi word */
        const uint64_t j = (uint64_t)i << 32;
        const uint64_t c1 = (uint64_t)1 << 32;
        const uint64_t c2 = (uint64_t)2 << 32;
        gasnett_atomic64_increment(&var64,0);
        if (gasnett_atomic64_read(&var64,0) != (j+1))
          ERR("gasnett_atomic64_increment wrote wrong value (hi)");
        if (gasnett_atomic64_add(&var64,c2,0) != (j+c2+1))
          ERR("gasnett_atomic64_add returned wrong value (hi)");
        if (gasnett_atomic64_read(&var64,0) != (j+c2+1))
          ERR("gasnett_atomic64_add wrote wrong value (hi)");
        gasnett_atomic64_decrement(&var64,0);
        if (gasnett_atomic64_read(&var64,0) != (j+c2))
          ERR("gasnett_atomic64_decrement wrote wrong value (hi)");
        if (gasnett_atomic64_decrement_and_test(&var64,0))
          ERR("gasnett_atomic64_decrement_and_test succeeded when expecting failure (hi)");
        if (gasnett_atomic64_read(&var64,0) != (j+c2-1))
          ERR("gasnett_atomic64_decrement_and_test wrote wrong value on failure (hi)");
        if (gasnett_atomic64_subtract(&var64,(j+c1+1),0) != (c1-2))
          ERR("gasnett_atomic64_subtract returned wrong value (hi)");
        if (gasnett_atomic64_read(&var64,0) != (c1-2))
          ERR("gasnett_atomic64_subtract wrote wrong value (hi)");
        if (gasnett_atomic64_swap(&var64,(j+c1),0) != (c1-2))
          ERR("gasnett_atomic64_swap returned wrong value (hi)");
        if (gasnett_atomic64_read(&var64,0) != (j+c1))
          ERR("gasnett_atomic64_swap wrote wrong value (hi)");
      }
    }
  }

  TEST_HEADER("Testing client-provided backtrace code...") {  
    TEST_BACKTRACE();
  }

#if GASNETT_THREAD_SAFE
  MSG("Spawning pthreads...");
  { 
    for(int i=0;i<NUM_THREADS;i++) gasnett_atomic_set(thread_flag+i,1,0);
    gasnett_local_mb();
    test_createandjoin_pthreads(NUM_THREADS, &thread_fn, NULL, 0);
  }
#endif

  MSG("Done.");
  return (test_errs > 0 ? 1 : 0);
}

#if GASNETT_THREAD_SAFE

#undef MSG0
#undef ERR
#define MSG0 THREAD_MSG0(id)
#define ERR  THREAD_ERR(id)

gasnett_atomic_t up = gasnett_atomic_init(0);
gasnett_atomic_t down = gasnett_atomic_init(0);
gasnett_atomic_t x1 = gasnett_atomic_init(10000);
gasnett_atomic_t x2 = gasnett_atomic_init(10000);
gasnett_atomic_t x3 = gasnett_atomic_init(10000);
gasnett_atomic_t x4 = gasnett_atomic_init(10000);
gasnett_atomic_t x5 = gasnett_atomic_init(10000);

gasnett_atomic_t _thread_barrier = gasnett_atomic_init(0);

#define THREAD_BARRIER() do {                                               \
   barcnt++;                                                                \
   gasnett_atomic_increment(&_thread_barrier, GASNETT_ATOMIC_REL);          \
   while (gasnett_atomic_read(&_thread_barrier,0) <                         \
                              (gasnett_atomic_val_t)(barcnt*NUM_THREADS)) { \
      gasnett_sched_yield();                                                \
   }                                                                        \
   gasnett_local_rmb(); /* Acquire */                                       \
  } while(0)                                                                \

#undef TEST_HEADER_PREFIX
#define TEST_HEADER_PREFIX() THREAD_BARRIER()

void * thread_fn(void *arg) {
  const int id = (int)(uintptr_t)arg;
  int barcnt = 0;
  char th_test_section = test_section;
  #define test_section th_test_section
 
  /* Avoid overflow to ensure a sane iters2 value. */
  const int iters2 = (iters >= (INT_MAX / 100)) ? (INT_MAX & ~1) : (100 * iters);
  /* Parallel atomic-op pounding test assumes iters2 is even */
  assert_always(iters2 % 2 == 0);

  /* sanity check - ensure unique threadids */
  if (!gasnett_atomic_decrement_and_test(thread_flag+id,0)) {
      ERR("thread %i failed sanity check", id);
  }

  /* sanity check - ensure thread barriers are working */
  TEST_HEADER("parallel atomic-op barrier test...") {  
    for (int i=0;i<iters;i++) {
      /* simple count-up barrier */
      gasnett_atomic_increment(&up,0);
      while (gasnett_atomic_read(&up,0) < (gasnett_atomic_val_t)NUM_THREADS) gasnett_sched_yield(); 

      gasnett_atomic_set(&down, 2*NUM_THREADS, 0);

      /* Why the _REL?. The set(down) above must complete before the inc(up).
       * Otherwise, it might clobber a decrement by another thread in the count-down below. */
      gasnett_atomic_increment(&up, GASNETT_ATOMIC_REL);
      while (gasnett_atomic_read(&up,0) < (gasnett_atomic_val_t)(2*NUM_THREADS)) gasnett_sched_yield(); 

      int tmp = gasnett_atomic_read(&up,0);
      if (tmp != 2*NUM_THREADS)
        ERR("count-up post-barrier read: %i != %i", tmp, 2*NUM_THREADS);

      /* simple count-down barrier */
      gasnett_atomic_decrement(&down,0);
      while (gasnett_atomic_read(&down,0) > (gasnett_atomic_val_t)NUM_THREADS) gasnett_sched_yield(); 

      gasnett_atomic_set(&up, 0, 0);

      /* Why the _REL?. The set(up) above must complete before the dec(down).
       * Otherwise, it might clobber an increment by another thread in the next count-up. */
      gasnett_atomic_decrement(&down, GASNETT_ATOMIC_REL);
      while (gasnett_atomic_read(&down,0) > 0) gasnett_sched_yield(); 

      tmp = gasnett_atomic_read(&down,0);
      if (tmp != 0)
        ERR("count-down post-barrier read: %i != 0", tmp);
    }
  }

  TEST_HEADER("parallel threadkey test...")
  { /* parallel threadkey test */
    test_threadkeys(partest_key1,partest_key2);
  }

  TEST_HEADER("parallel atomic-op pounding test...") {
    gasnett_atomic_set(&x1, 5, 0);
    gasnett_atomic_set(&x2, 5+iters2*NUM_THREADS, 0);
    gasnett_atomic_set(&x3, 5, 0);
    gasnett_atomic_set(&x4, 5+iters2*NUM_THREADS, 0);

    THREAD_BARRIER();

    for (int i=0;i<iters2;i++) {
      gasnett_atomic_increment(&x1,0);
      gasnett_atomic_decrement(&x2,0);
    }
    #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      for (int i=0;i<iters2;i++) {
	int val = (i & 1) << 1; /* Alternate 0 and 2. (iters2=100*iters is always even) */
        gasnett_atomic_add(&x3,val,0);
        gasnett_atomic_subtract(&x4,val,0);
      }
    #endif

    THREAD_BARRIER();

    int val = gasnett_atomic_read(&x1,0);
    if (val != 5+iters2*NUM_THREADS)
      ERR("pounding inc test mismatch: %i != %i",val,5+iters2*NUM_THREADS);

    val = gasnett_atomic_read(&x2,0);
    if (val != 5)
      ERR("pounding dec test mismatch: %i != 5",val);

  #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      val = gasnett_atomic_read(&x3,0);
      if (val != 5+iters2*NUM_THREADS)
        ERR("pounding add test mismatch: %i != %i",val,5+iters2*NUM_THREADS);
  
      val = gasnett_atomic_read(&x4,0);
      if (val != 5)
        ERR("pounding subtract test mismatch: %i != 5",val);
  #endif

  }

  TEST_HEADER("parallel dec-test pounding test...") {

    gasnett_atomic_set(&x3, NUM_THREADS, 0);
    gasnett_atomic_set(&x4, 0, 0);
    gasnett_atomic_set(&x5, 0, 0); /* count of "wins" */

    THREAD_BARRIER();

    for (int i=0;i<iters;i++) {
      if (gasnett_atomic_decrement_and_test(&x3,0)) { /* I won */
        gasnett_atomic_increment(&x5,0); /* tally win */
        if (gasnett_atomic_read(&x3,0) != 0) ERR("pounding dec-test mismatch x3");
        if (gasnett_atomic_read(&x4,0) != 0) ERR("pounding dec-test mismatch x4");
        gasnett_atomic_set(&x4, NUM_THREADS, GASNETT_ATOMIC_REL); /* go */
      } else {
        while (gasnett_atomic_read(&x4,0) == 0) gasnett_sched_yield(); /* I lost - wait */
      }

      if (gasnett_atomic_decrement_and_test(&x4,0)) { /* I won */
        gasnett_atomic_increment(&x5,0); /* tally win */
        if (gasnett_atomic_read(&x3,0) != 0) ERR(" pounding dec-test mismatch x3");
        if (gasnett_atomic_read(&x4,0) != 0) ERR("pounding dec-test mismatch x4");
        gasnett_atomic_set(&x3, NUM_THREADS,  GASNETT_ATOMIC_REL); /* go */
      } else {
        while (gasnett_atomic_read(&x3,0) == 0) gasnett_sched_yield(); /* I lost - wait */
      }
    }

    if (gasnett_atomic_read(&x5, GASNETT_ATOMIC_RMB_PRE) != (gasnett_atomic_val_t)(2*iters))
      ERR("pounding dec-test mismatch");
  }

  TEST_HEADER("parallel word-tearing test...") {

    gasnett_atomic_set(&x3, 0, 0);
    gasnett_atomic_set(&x4, 0, 0);
    gasnett_atomic_set(&x5, 0, 0); 

    THREAD_BARRIER();

    if (NUM_THREADS <= 100) {  /* need 2*NUM_THREADS + 1 < 255 to prevent byte overflow */
      uint32_t x = id + 1;
      uint32_t myval = (x << 24) | (x << 16) | (x << 8) | x;
      for (int i=0;i<iters2;i++) {
        gasnett_atomic_set(&x3, myval, 0);
        gasnett_atomic_set(&x4, myval, 0);
        gasnett_atomic_set(&x5, myval, 0);
        gasnett_atomic_increment(&x4,0);
        gasnett_atomic_decrement(&x5,0);
        uint32_t v = gasnett_atomic_read(&x3,0);
        if (((v >> 24) & 0xFF) != (v & 0xFF) ||
            ((v >> 16) & 0xFF) != (v & 0xFF) ||
            ((v >>  8) & 0xFF) != (v & 0xFF)) 
            ERR("observed word tearing on gasnett_atomic_set");
        v = gasnett_atomic_read(&x4,0); 
        /* bottom byte may have increased by up to NUM_THREADS, but high bytes must be same */
        if (((v >> 24) & 0xFF) != ((v >>  8) & 0xFF) ||
            ((v >> 16) & 0xFF) != ((v >>  8) & 0xFF)) 
            ERR("observed word tearing on gasnett_atomic_set/gasnett_atomic_increment");
        v = gasnett_atomic_read(&x5,0); 
        v += NUM_THREADS;
        /* bottom byte may have decreased by up to NUM_THREADS, but high bytes must be same */
        if (((v >> 24) & 0xFF) != ((v >>  8) & 0xFF) ||
            ((v >> 16) & 0xFF) != ((v >>  8) & 0xFF)) 
            ERR("observed word tearing on gasnett_atomic_set/gasnett_atomic_decrement");
      }
    }

    if (NUM_THREADS <= 100) {  /* need 2*NUM_THREADS + 1 < 255 to prevent byte overflow */
      static gasnett_atomic32_t a1 = gasnett_atomic32_init(0);
      static gasnett_atomic32_t a2 = gasnett_atomic32_init(0);
      uint32_t x = id + 1;
      uint32_t myval = (x << 24) | (x << 16) | (x << 8) | x;
      for (int i=0;i<iters2;i++) {
        gasnett_atomic32_set(&a1, myval, 0);
        gasnett_atomic32_set(&a2, myval, 0);
	uint32_t v = gasnett_atomic32_read(&a2,0);
        gasnett_atomic32_compare_and_swap(&a2,v,v+1,0);
        v = gasnett_atomic32_read(&a1,0);
        if (((v >> 24) & 0xFF) != (v & 0xFF) ||
            ((v >> 16) & 0xFF) != (v & 0xFF) ||
            ((v >>  8) & 0xFF) != (v & 0xFF)) 
            ERR("observed word tearing on gasnett_atomic32_set");
        v = gasnett_atomic32_read(&a2,0); 
        /* bottom byte may have increased by up to NUM_THREADS, but high bytes must be same */
        if (((v >> 24) & 0xFF) != ((v >>  8) & 0xFF) ||
            ((v >> 16) & 0xFF) != ((v >>  8) & 0xFF)) 
            ERR("observed word tearing on gasnett_atomic32_set/gasnett_atomic32_compare_and_swap");
      }
    }

    {  /* No overflow until we're into the tens of thousands of threads */
      static gasnett_atomic64_t a1 = gasnett_atomic64_init(0);
      static gasnett_atomic64_t a2 = gasnett_atomic64_init(0);
      static struct { /* Try to trigger bad alignment if possible */
        char               c;
        gasnett_atomic64_t a;
      } s1 = {0, gasnett_atomic64_init(0)};
      static struct { /* Try to trigger bad alignment if possible */
        char               c;
        double             d;
      } s2 = {0, -1.0};
      uint64_t x = id + 1;
      uint64_t myval = (x << 48) | (x << 32) | (x << 16) | x;

      for (int i=0;i<iters2;i++) {
        gasnett_atomic64_set(&s1.a, myval, 0);
        gasnett_atomic64_set((gasnett_atomic64_t *)(void *)&s2.d, myval, 0); /* (void*) suppresses g++ warning (bug 2158) */
        gasnett_atomic64_set(&a1, myval, 0);
        gasnett_atomic64_set(&a2, myval, 0);
	uint64_t v = gasnett_atomic64_read(&a2,0);
        gasnett_atomic64_compare_and_swap(&a2,v,v+1,0);

        v = gasnett_atomic64_read(&s1.a,0);
        if (((v >> 48) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 32) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 16) & 0xFFFF) != (v & 0xFFFF)) 
            ERR("observed word tearing on gasnett_atomic64_set alignment test");
        v = gasnett_atomic64_read((gasnett_atomic64_t *)(void *)&s2.d,0); /* (void*) suppresses g++ warning (bug 2158) */
        if (((v >> 48) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 32) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 16) & 0xFFFF) != (v & 0xFFFF)) 
            ERR("observed word tearing on gasnett_atomic64_set double alignment test");
        v = gasnett_atomic64_read(&a1,0);
        if (((v >> 48) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 32) & 0xFFFF) != (v & 0xFFFF) ||
            ((v >> 16) & 0xFFFF) != (v & 0xFFFF)) 
            ERR("observed word tearing on gasnett_atomic64_set");
        v = gasnett_atomic64_read(&a2,0); 
        /* bottom byte may have increased by up to NUM_THREADS, but high bytes must be same */
        if (((v >> 48) & 0xFFFF) != ((v >> 16) & 0xFFFF) ||
            ((v >> 32) & 0xFFFF) != ((v >> 16) & 0xFFFF)) 
            ERR("observed word tearing on gasnett_atomic64_set/gasnett_atomic64_compare_and_swap");
      }
    }
  }

  TEST_HEADER("parallel membar test...") {
    { const int partner = (id + 1) % NUM_THREADS;
      /* Allow for wrap-around/overflow */
      #define BIGGER(_x, _y) ((int)(_y - _x) > 0)

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (int i=0;i<iters2;i++) {
        valX[id] = i;
        gasnett_local_wmb();
        valY[id] = i;

        unsigned int ly = valY[partner];
        gasnett_local_rmb();
        unsigned int lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_local_wmb/gasnett_local_rmb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (int i=0;i<iters2;i++) {
        valX[id] = i + iters2;
        gasnett_local_mb();
        valY[id] = i + iters2;

        unsigned int ly = valY[partner];
        gasnett_local_mb();
        unsigned int lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_local_mb/gasnett_local_mb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (int i=0;i<iters2;i++) {
        valX[id] = i;
        gasnett_weak_wmb();
        valY[id] = i;

        unsigned int ly = valY[partner];
        gasnett_weak_rmb();
        unsigned int lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_weak_wmb/gasnett_weak_rmb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (int i=0;i<iters2;i++) {
        valX[id] = i + iters2;
        gasnett_weak_mb();
        valY[id] = i + iters2;

        unsigned int ly = valY[partner];
        gasnett_weak_mb();
        unsigned int lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_weak_mb/gasnett_weak_mb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();
    }
  }

  TEST_HEADER("parallel compare-and-swap test...") {
    #if defined(GASNETT_HAVE_ATOMIC_CAS)
      static gasnett_atomic_t counter2 = gasnett_atomic_init(0);
      static gasnett_atomic_val_t shared_counter = 0;
      gasnett_atomic_val_t woncnt = 0;
      gasnett_atomic_val_t const share = 
         ((unsigned)iters >= (GASNETT_ATOMIC_MAX / NUM_THREADS)) ? (GASNETT_ATOMIC_MAX / NUM_THREADS) : iters;
      gasnett_atomic_val_t const goal = NUM_THREADS * share;
      gasnett_atomic_val_t oldval;

      /* Look for missing or doubled updates by taking an equal share of increments */
      while (woncnt < share &&
             (oldval = gasnett_atomic_read(&counter2,0)) != goal) {
        if (gasnett_atomic_compare_and_swap(&counter2, oldval, (oldval + 1), 0)) {
           woncnt++;
        }
      }
      THREAD_BARRIER();
      oldval = gasnett_atomic_read(&counter2,0);
      if (oldval != goal) 
        ERR("failed compare-and-swap test: counter=%u expecting=%u", (unsigned)oldval, (unsigned)goal);
      if (woncnt != share) 
        ERR("failed compare-and-swap test: woncnt=%u share=%u", (unsigned)woncnt, (unsigned)share);

      /* Now try spinlock construct */
      THREAD_BARRIER();
      for (gasnett_atomic_val_t i=0;i<share;i++) {
	while (!gasnett_atomic_compare_and_swap(&counter2, oldval, ~oldval, 0)) {};
        gasnett_local_rmb(); /* Acquire */
	shared_counter ++;
        gasnett_local_wmb(); /* Release */
        gasnett_atomic_set(&counter2, oldval, 0);
      }
      THREAD_BARRIER();
      if (shared_counter != goal)
        ERR("failed compare-and-swap spinlock (rmb/wmb) test: counter=%i expecting=%i", (unsigned)shared_counter, (unsigned)goal);

      /* Now try spinlock construct using mb() */
      THREAD_BARRIER();
      for (gasnett_atomic_val_t i=0;i<share;i++) {
	while (!gasnett_atomic_compare_and_swap(&counter2, oldval, ~oldval, 0)) {};
        gasnett_local_mb(); /* Acquire */
	shared_counter --;
        gasnett_local_mb(); /* Release */
        gasnett_atomic_set(&counter2, oldval, 0);
      }
      THREAD_BARRIER();
      if (shared_counter != 0)
        ERR("failed compare-and-swap spinlock (mb/mb) test: counter=%i expecting=0", (unsigned)shared_counter);
    #endif

    {
      static gasnett_atomic32_t counter32 = gasnett_atomic32_init(0);
      uint32_t woncnt = 0;
      uint32_t const share = MIN((unsigned)iters, (0xffffffffU / NUM_THREADS));
      uint32_t const goal = NUM_THREADS * share;
      uint32_t oldval;

      /* Look for missing or doubled updates by taking an equal share of increments */
      while (woncnt < share && (oldval = gasnett_atomic32_read(&counter32,0)) < goal) {
        if (gasnett_atomic32_compare_and_swap(&counter32, oldval, (oldval + 1), 0)) {
           woncnt++;
        }
      }
      if (woncnt != share)
        ERR("failed 32-bit compare-and-swap test: woncnt=%u share=%u", (unsigned)woncnt, (unsigned)share);
      THREAD_BARRIER();
      oldval = gasnett_atomic32_read(&counter32,0);
      if (oldval != goal) 
        ERR("failed 32-bit compare-and-swap test: counter=%u expecting=%u", (unsigned)oldval, (unsigned)goal);
    }

    {
      static gasnett_atomic64_t counter64 = gasnett_atomic64_init(0);
      uint64_t const share = MIN((unsigned)iters, (0xffffffffU / NUM_THREADS));
      uint64_t const one = 1;
      uint64_t const incrs[] = { one, one<<32, one + (one<<32) };
      for (size_t i = 0; i < sizeof(incrs)/sizeof(incrs[0]); ++i) {
        uint64_t const incr = incrs[i];
        uint64_t const goal = NUM_THREADS * share * incr;
        uint64_t woncnt = 0;
        uint64_t oldval;

        /* Look for missing or doubled updates by taking an equal share of increments */
        while (woncnt < share && (oldval = gasnett_atomic64_read(&counter64,0)) < goal) {
          if (gasnett_atomic64_compare_and_swap(&counter64, oldval, (oldval + incr), 0)) {
             woncnt++;
          }
        }
        if (woncnt != share)
          ERR("failed 64-bit compare-and-swap test: woncnt=%" PRIu64 " share=%" PRIu64, woncnt, share);
        THREAD_BARRIER();
        if (!id) {
          oldval = gasnett_atomic64_read(&counter64,0);
          if (oldval != goal)
            ERR("failed 64-bit compare-and-swap test: counter=%" PRIu64 " expecting=%" PRIu64, oldval, goal);
          gasnett_atomic64_set(&counter64,0,0);
        }
        THREAD_BARRIER();
      }
    }
  }

  TEST_HEADER("parallel swap test...") {
    const gasnett_atomic_val_t limit = MIN(GASNETT_ATOMIC_MAX, 8192);
    static char *array;
    if (!id) {
      array = (char *)test_calloc(sizeof(char), limit);
    }

    #if GASNETT_HAVE_ATOMIC_CAS
    {
      static gasnett_atomic_t var = gasnett_atomic_init(GASNETT_ATOMIC_MAX);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        /* Write all values in [0,limit) with each thread owning a share of the space.
           The 'array' tracks which values have been seen and ensures no duplicates. */
        for (gasnett_atomic_val_t j = id; j < limit; j += NUM_THREADS) {
          gasnett_atomic_val_t idx = gasnett_atomic_swap(&var, j, 0);
          if_pt (idx != GASNETT_ATOMIC_MAX) {
            if_pf (idx >= limit) {
              ERR("gasnett_atomic_swap read an impossible value 0x%x", (unsigned)idx);
            } else {
              if_pf (array[idx] != 0)
                ERR("gasnett_atomic_swap produced a duplicate value %d", (int)idx);
              array[idx] = 1;
            }
          }
        }

        THREAD_BARRIER();

        if (!id) {
          /* One final swap to simplify the validation */
          gasnett_atomic_val_t idx = gasnett_atomic_swap(&var, GASNETT_ATOMIC_MAX, 0);
          if_pf (idx >= limit) {
            ERR("gasnett_atomic_swap read an impossible value 0x%x", (unsigned)idx);
          } else {
            if_pf (array[idx] != 0)
              ERR("gasnett_atomic_swap produced a duplicate value %d", (int)idx);
            array[idx] = 1;
          }

          /* Now scan the array to ensure no values were missed */
          for (idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic_swap missed an update at %d", (int)idx);
            array[idx] = 0; /* reset for next iteration */
          }
        }

        THREAD_BARRIER();
      }
    }
    #else
      MSG0("  NOTE: gasnett_atomic_swap() is missing");
    #endif

    { // Same for 32-bit fixed-width atomics
      static gasnett_atomic32_t var = gasnett_atomic32_init(GASNETT_ATOMIC_MAX);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        for (uint32_t j = id; j < limit; j += NUM_THREADS) {
          uint32_t idx = gasnett_atomic32_swap(&var, j, 0);
          if_pt (idx != GASNETT_ATOMIC_MAX) {
            if_pf (idx >= limit) {
              ERR("gasnett_atomic32_swap read an impossible value 0x%x", (unsigned)idx);
            } else {
              if (array[idx] != 0)
                ERR("gasnett_atomic32_swap produced a duplicate value %d", (int)idx);
              array[idx] = 1;
            }
          }
        }

        THREAD_BARRIER();

        if (!id) {
          uint32_t idx = gasnett_atomic32_swap(&var, GASNETT_ATOMIC_MAX, 0);
          if (idx >= limit) {
            ERR("gasnett_atomic32_swap read an impossible value 0x%x", (unsigned)idx);
          } else {
            if (array[idx] != 0)
              ERR("gasnett_atomic32_swap produced a duplicate value %u", (int)idx);
            array[idx] = 1;
          }

          for (idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic32_swap missed an update at %d", (int)idx);
            array[idx] = 0;
          }
        }

        THREAD_BARRIER();
      }
    }

    { // Same for 64-bit fixed-width atomics
      // There is an extra wrinkle in this case:
      // We map the updates into both upper and lower halves of a 64-bit word
      #define FWD(n) ((n)&1 ? (n) : (uint64_t)(n)<<32)
      #define BWD(n) (((n)&0xFFFFFFFFu) | ((n)>>32))

      static gasnett_atomic64_t var = gasnett_atomic64_init(GASNETT_ATOMIC_MAX);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        for (uint64_t j = id; j < limit; j += NUM_THREADS) {
          uint64_t read = gasnett_atomic64_swap(&var, FWD(j), 0);
          if_pt (read != GASNETT_ATOMIC_MAX) {
            uint64_t idx = BWD(read);
            if_pf ((idx >= limit) || ((read & 0xFFFFFFFFu) && (read >> 32))) {
              ERR("gasnett_atomic64_swap read an impossible value 0x%" PRIx64, read);
            } else {
              if_pf (array[idx] != 0)
                ERR("gasnett_atomic64_swap produced a duplicate value %d", (int)idx);
              array[idx] = 1;
            }
          }
        }

        THREAD_BARRIER();

        if (!id) {
          uint64_t read = gasnett_atomic64_swap(&var, GASNETT_ATOMIC_MAX, 0);
          uint64_t idx = BWD(read);
          if_pf ((idx >= limit) || ((read & 0xFFFFFFFFu) && (read >> 32))) {
            ERR("gasnett_atomic64_swap read an impossible value 0x%" PRIx64, read);
          } else {
            if_pf (array[idx] != 0)
              ERR("gasnett_atomic64_swap produced a duplicate value %d", (int)idx);
            array[idx] = 1;
          }

          for (idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic64_swap missed an update at %d", (int)idx);
            array[idx] = 0;
          }
        }

        THREAD_BARRIER();
      }
      #undef FWD
      #undef BWD
    }

    if (0 == id) {
      test_free(array);
    }
  }

  TEST_HEADER("parallel add test...") {
    const gasnett_atomic_val_t limit = MIN(GASNETT_ATOMIC_MAX, 8192);
    static char *array;
    if (!id) {
      array = (char *)test_calloc(sizeof(char), limit);
    }

    #if GASNETT_HAVE_ATOMIC_ADD_SUB
    {
      static gasnett_atomic_t var = gasnett_atomic_init(0);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        /* Produce all values in [0,limit) with each thread owning a share of the space.
           The 'array' tracks which values have been seen and ensures no duplicates. */
        for (gasnett_atomic_val_t j = id; j < limit; j += NUM_THREADS) {
          gasnett_atomic_val_t idx = gasnett_atomic_add(&var, 1, 0) - 1;
          if_pf (idx >= limit) {
            ERR("gasnett_atomic_add read an impossible value 0x%x", (unsigned)idx);
          } else {
            if_pf (array[idx] != 0)
              ERR("gasnett_atomic_add produced a duplicate value %d", (int)idx);
            array[idx] = 1;
          }
        }

        THREAD_BARRIER();

        if (!id) {
          /* Now scan the array to ensure no values were missed */
          for (gasnett_atomic_val_t idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic_add missed an update at %d", (int)idx);
            array[idx] = 0; /* reset for next iteration */
          }
          gasnett_atomic_set(&var, 0, 0);
        }

        THREAD_BARRIER();
      }
    }
    #else
      MSG0("  NOTE: gasnett_atomic_add() is missing");
    #endif

    { // Same for 32-bit fixed-width atomics
      static gasnett_atomic32_t var = gasnett_atomic32_init(0);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        for (uint32_t j = id; j < limit; j += NUM_THREADS) {
          uint32_t idx = gasnett_atomic32_add(&var, 1, 0) - 1;
          if_pf (idx >= limit) {
            ERR("gasnett_atomic32_add read an impossible value 0x%x", (unsigned)idx);
          } else {
            if_pf (array[idx] != 0)
              ERR("gasnett_atomic32_add produced a duplicate value %d", (int)idx);
            array[idx] = 1;
          }
        }

        THREAD_BARRIER();

        if (!id) {
          for (uint32_t idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic32_add missed an update at %d", (int)idx);
            array[idx] = 0;
          }
          gasnett_atomic32_set(&var, 0, 0);
        }

        THREAD_BARRIER();
      }
    }

    { // Same for 64-bit fixed-width atomics
      // There is an extra wrinkle in this case:
      // We map the updates into both upper and lower halves of a 64-bit word
      #define FWD(n) ((n)&1 ? (1) : (uint64_t)1<<32)
      #define BWD(n) (((n)&0xFFFFFFFFu) + ((n)>>32))

      static gasnett_atomic64_t var = gasnett_atomic64_init(0);

      THREAD_BARRIER();

      for (int i = 0; i < iters; ++i) {
        for (uint64_t j = id; j < limit; j += NUM_THREADS) {
          uint64_t read = gasnett_atomic64_add(&var, FWD(j), 0);
          uint64_t idx = BWD(read) - 1;
          if_pf (idx >= limit) {
            ERR("gasnett_atomic64_add read an impossible value 0x%" PRIx64, read);
          } else {
            if_pf (array[idx] != 0)
              ERR("gasnett_atomic64_add produced a duplicate value %d", (int)idx);
            array[idx] = 1;
          }
        }

        THREAD_BARRIER();

        if (!id) {
          for (uint64_t idx = 0; idx < limit; ++idx) {
            if (array[idx] != 1)
              ERR("gasnett_atomic64_add missed an update at %d", (int)idx);
            array[idx] = 0;
          }
          gasnett_atomic64_set(&var, 0, 0);
        }

        THREAD_BARRIER();
      }
      #undef FWD
      #undef BWD
    }

    if (0 == id) {
      test_free(array);
    }
  }

  TEST_HEADER("parallel atomic-op fence test...") {
    int const partner = (id + 1) % NUM_THREADS;
    unsigned int lx, ly;

    gasnett_atomic_set(&atomicX[id], 0, 0);
    gasnett_atomic32_set(&atomicX32[id], 0, 0);
    gasnett_atomic64_set(&atomicX64[id], 0, 0);
    valY[id] = 0;
    valY32[id] = 0;
    valY64[id] = 0;

    THREAD_BARRIER();

    /* First a pass through w/ WMB and RMB */
    for (int i=0;i<iters;i++) {
      gasnett_atomic_set(&atomicX[id], 6*i, GASNETT_ATOMIC_WMB_POST);
      valY[id] = 6*i;
      ly = valY[partner];
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_RMB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced set/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);

      gasnett_atomic32_set(&atomicX32[id], 2*i, GASNETT_ATOMIC_WMB_POST);
      valY32[id] = 2*i;
      ly = (unsigned int)valY32[partner];
      lx = (unsigned int)gasnett_atomic32_read(&atomicX32[partner], GASNETT_ATOMIC_RMB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced 32-bit set/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);

      gasnett_atomic64_set(&atomicX64[id], 2*i, GASNETT_ATOMIC_WMB_POST);
      valY64[id] = 2*i;
      ly = (unsigned int)valY64[partner];
      lx = (unsigned int)gasnett_atomic64_read(&atomicX64[partner], GASNETT_ATOMIC_RMB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced 64-bit set/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);

      gasnett_atomic_increment(&atomicX[id], GASNETT_ATOMIC_WMB_POST);
      ++valY[id];
      ly = valY[partner];
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_RMB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced dec/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);

      #if defined(GASNETT_HAVE_ATOMIC_CAS)
      {
	uint32_t oldval;
	do {
	  oldval = gasnett_atomic_read(&atomicX[id], 0);
	} while (!gasnett_atomic_compare_and_swap(&atomicX[id], oldval, oldval + 1, GASNETT_ATOMIC_WMB_POST));
        valY[id]++;
        ly = valY[partner];
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_RMB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced c-a-s/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }
      #endif

      {
	uint32_t oldval;
	do {
	  oldval = gasnett_atomic32_read(&atomicX32[id], 0);
	} while (!gasnett_atomic32_compare_and_swap(&atomicX32[id], oldval, oldval + 1, GASNETT_ATOMIC_WMB_POST));
        valY32[id]++;
        ly = (unsigned int)valY32[partner];
        lx = (unsigned int)gasnett_atomic32_read(&atomicX32[partner], GASNETT_ATOMIC_RMB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced 32-bit c-a-s/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }

      {
	uint64_t oldval;
	do {
	  oldval = gasnett_atomic64_read(&atomicX64[id], 0);
	} while (!gasnett_atomic64_compare_and_swap(&atomicX64[id], oldval, oldval + 1, GASNETT_ATOMIC_WMB_POST));
        valY64[id]++;
        ly = (unsigned int)valY64[partner];
        lx = (unsigned int)gasnett_atomic64_read(&atomicX64[partner], GASNETT_ATOMIC_RMB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced 64-bit c-a-s/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }

      #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      {
        int step = i & 4;
        gasnett_atomic_add(&atomicX[id], step, GASNETT_ATOMIC_WMB_POST);
        valY[id] += step;
        ly = valY[partner];
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_RMB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced add/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }
      #endif
    }

    THREAD_BARRIER();

    for (int i=iters-1;i>=0;i--) {
      #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      {
        int step = i & 4;
        valY[id] -= step;
        gasnett_atomic_subtract(&atomicX[id], step, GASNETT_ATOMIC_REL);
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_ACQ);
        ly = valY[partner];
        if (BIGGER(lx,ly)) ERR("pounding fenced sub/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }
      #endif

      #if defined(GASNETT_HAVE_ATOMIC_CAS)
      {
	uint32_t oldval;
        valY[id]--;
	do {
	  oldval = gasnett_atomic_read(&atomicX[id], 0);
	} while (!gasnett_atomic_compare_and_swap(&atomicX[id], oldval, oldval - 1, GASNETT_ATOMIC_REL));
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_ACQ);
        ly = valY[partner];
        if (BIGGER(lx,ly)) ERR("pounding fenced c-a-s/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
      }
      #endif

      --valY[id];
      gasnett_atomic_decrement(&atomicX[id], GASNETT_ATOMIC_REL);
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_ACQ);
      ly = valY[partner];
      if (BIGGER(lx,ly)) ERR("pounding fenced dec/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);

      valY[id] = 6*i;
      gasnett_atomic_set(&atomicX[id], 6*i, GASNETT_ATOMIC_REL);
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_ACQ);
      ly = valY[partner];
      if (BIGGER(lx,ly)) ERR("pounding fenced set/read mismatch (rmb/wmb): lx=%u ly=%u", lx, ly);
    }

    THREAD_BARRIER();

    gasnett_atomic_set(&atomicX[id], 0, 0);
    gasnett_atomic32_set(&atomicX32[id], 0, 0);
    gasnett_atomic64_set(&atomicX64[id], 0, 0);
    valY[id] = 0;
    valY32[id] = 0;
    valY64[id] = 0;

    THREAD_BARRIER();

    /* Second pass through w/ MB used for both WMB and RMB */
    for (int i=0;i<iters;i++) {
      gasnett_atomic_set(&atomicX[id], 6*i, GASNETT_ATOMIC_MB_POST);
      valY[id] = 6*i;
      ly = valY[partner];
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced set/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);

      gasnett_atomic32_set(&atomicX32[id], 2*i, GASNETT_ATOMIC_MB_POST);
      valY32[id] = 2*i;
      ly = (unsigned int)valY32[partner];
      lx = (unsigned int)gasnett_atomic32_read(&atomicX32[partner], GASNETT_ATOMIC_MB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced 32-bit set/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);

      gasnett_atomic64_set(&atomicX64[id], 2*i, GASNETT_ATOMIC_MB_POST);
      valY64[id] = 2*i;
      ly = (unsigned int)valY64[partner];
      lx = (unsigned int)gasnett_atomic64_read(&atomicX64[partner], GASNETT_ATOMIC_MB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced 64-bit set/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);

      gasnett_atomic_increment(&atomicX[id], GASNETT_ATOMIC_MB_POST);
      ++valY[id];
      ly = valY[partner];
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_PRE);
      if (BIGGER(lx,ly)) ERR("pounding fenced dec/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);

      #if defined(GASNETT_HAVE_ATOMIC_CAS)
      {
	uint32_t oldval;
	do {
	  oldval = gasnett_atomic_read(&atomicX[id], 0);
	} while (!gasnett_atomic_compare_and_swap(&atomicX[id], oldval, oldval + 1, GASNETT_ATOMIC_MB_POST));
        valY[id]++;
        ly = valY[partner];
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced c-a-s/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }
      #endif

      {
	uint32_t oldval;
	do {
	  oldval = gasnett_atomic32_read(&atomicX32[id], 0);
	} while (!gasnett_atomic32_compare_and_swap(&atomicX32[id], oldval, oldval + 1, GASNETT_ATOMIC_MB_POST));
        valY32[id]++;
        ly = (unsigned int)valY32[partner];
        lx = (unsigned int)gasnett_atomic32_read(&atomicX32[partner], GASNETT_ATOMIC_MB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced 32-bit c-a-s/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }

      {
	uint64_t oldval;
	do {
	  oldval = gasnett_atomic64_read(&atomicX64[id], 0);
	} while (!gasnett_atomic64_compare_and_swap(&atomicX64[id], oldval, oldval + 1, GASNETT_ATOMIC_MB_POST));
        valY64[id]++;
        ly = (unsigned int)valY64[partner];
        lx = (unsigned int)gasnett_atomic64_read(&atomicX64[partner], GASNETT_ATOMIC_MB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced 64-bit c-a-s/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }

      #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      {
        int step = i & 4;
        gasnett_atomic_add(&atomicX[id], step, GASNETT_ATOMIC_MB_POST);
        valY[id] += step;
        ly = valY[partner];
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_PRE);
        if (BIGGER(lx,ly)) ERR("pounding fenced add/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }
      #endif
    }

    THREAD_BARRIER();

    for (int i=iters-1;i>=0;i--) {
      #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      {
        int step = i & 4;
        valY[id] -= step;
        gasnett_atomic_subtract(&atomicX[id], step, GASNETT_ATOMIC_MB_PRE);
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_POST);
        ly = valY[partner];
        if (BIGGER(lx,ly)) ERR("pounding fenced sub/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }
      #endif

      #if defined(GASNETT_HAVE_ATOMIC_CAS)
      {
	uint32_t oldval;
        valY[id]--;
	do {
	  oldval = gasnett_atomic_read(&atomicX[id], 0);
	} while (!gasnett_atomic_compare_and_swap(&atomicX[id], oldval, oldval - 1, GASNETT_ATOMIC_MB_PRE));
        lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_POST);
        ly = valY[partner];
        if (BIGGER(lx,ly)) ERR("pounding fenced c-a-s/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
      }
      #endif

      --valY[id];
      gasnett_atomic_decrement(&atomicX[id], GASNETT_ATOMIC_MB_PRE);
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_POST);
      ly = valY[partner];
      if (BIGGER(lx,ly)) ERR("pounding fenced dec/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);

      valY[id] = 6*i;
      gasnett_atomic_set(&atomicX[id], 6*i, GASNETT_ATOMIC_MB_PRE);
      lx = gasnett_atomic_read(&atomicX[partner], GASNETT_ATOMIC_MB_POST);
      ly = valY[partner];
      if (BIGGER(lx,ly)) ERR("pounding fenced set/read mismatch (mb/mb): lx=%u ly=%u", lx, ly);
    }
  }

  THREAD_BARRIER();

  return NULL;
}

#endif
