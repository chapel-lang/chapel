/*   $Source: /var/local/cvs/gasnet/tests/testtools.c,v $
 *     $Date: 2010/05/04 01:26:24 $
 * $Revision: 1.95.4.4 $
 * Description: helpers for GASNet tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#define TEST_GASNET_TOOLS_ONLY
#include "test.h"
#include <limits.h> /* For INT_MAX */

/* specifically omit gasnet.h/test.h to test independence */
#if defined(_GASNET_H) || defined(TEST_GASNET_H)
#error testtools should *not* include gasnet.h
#endif

#ifdef HAVE_PTHREAD_H
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

GASNETT_BEGIN_EXTERNC
void *test_dummy3(void) GASNETT_MALLOC;
void *test_dummy3(void) { return malloc(1); }
GASNETT_INLINE(test_dummy4) GASNETT_MALLOC
void *test_dummy4(void) { return malloc(1); }
GASNETT_END_EXTERNC
double volatile d_junk = 0;

GASNETT_EXTERNC
void test_dummy5(void) { }

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
  #ifdef HAVE_PTHREAD_H
    if (argc > 2) NUM_THREADS = atoi(argv[2]);
    if (NUM_THREADS < 1) NUM_THREADS = DEFAULT_THREADS;
    NUM_THREADS = test_thread_limit(NUM_THREADS);
  #else
    if (argc > 2 && atoi(argv[2]) != 1) { ERR("no pthreads - only one thread available."); test_usage(); }
  #endif
  if (argc > 3) TEST_SECTION_PARSE(argv[3]);
  if (argc > 4) test_usage();

  TEST_GENERICS_WARNING();
  #ifdef HAVE_PTHREAD_H
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
        ERR("gasnett_getPhysMemSz() got a ridiculous result: %llu bytes", (unsigned long long)val);
    }
  }

  { char tmp_str[50];
    int i;
    gasnett_format_number(0, tmp_str, sizeof(tmp_str), 1);
    assert_always(gasnett_parse_int(tmp_str, 1) == 0);
    gasnett_format_number(0, tmp_str, sizeof(tmp_str), 0);
    assert_always(gasnett_parse_int(tmp_str, 0) == 0);
    for (i=0; i < 62; i++) {
      int64_t x = (((int64_t)1) << i);
      int64_t y;
      gasnett_format_number(x, tmp_str, sizeof(tmp_str), 1);
      y = gasnett_parse_int(tmp_str, 1);
      if (x != y) ERR("gasnett_format_number/gasnett_parse_int memsz mismatch: %lld != %lld (%s)",
                      (long long)x, (long long)y, tmp_str);
      gasnett_format_number(x, tmp_str, sizeof(tmp_str), 0);
      y = gasnett_parse_int(tmp_str, 0);
      if (x != y) ERR("gasnett_format_number/gasnett_parse_int mismatch: %lld != %lld (%s)",
                      (long long)x, (long long)y, tmp_str);
    }
  }

  gasnett_sched_yield();
  gasnett_flush_streams();
  gasnett_maximize_rlimits();
  TEST_TRACING_MACROS();

  TEST_HEADER("Testing high-performance timers...")
  { /* high performance timers */
    int i;
    gasnett_tick_t begin, start, end;
    uint64_t beginref, startref, endref;
    int timeiters = MAX(1,iters / 10);
    gasnett_tick_t ticktimemin = GASNETT_TICK_MIN;
    gasnett_tick_t ticktimemax = GASNETT_TICK_MAX;

    if (!(ticktimemin < ticktimemax)) ERR("!(min < max)");
    if (!(gasnett_ticks_now() > ticktimemin)) ERR("!(now > min)");
    if (!(gasnett_ticks_now() < ticktimemax)) ERR("!(now < max)");

    begin = gasnett_ticks_now();  /* outer time point */
    beginref = gasnett_gettimeofday_us();
    for (i=0; i < timeiters; i++) {
      int time, timeref;
      if (i == timeiters - 1) {
        start = begin; /* use outer time point for base of last iteration */
        startref = beginref;
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
            ERR("gasnett_ticks_to_us not monotonic! !(%llu <= %llu)",
                 (unsigned long long)last, (unsigned long long)next);
          if (next <= GASNETT_TICK_MIN) 
            ERR("gasnett_ticks_to_us()=%llu <= GASNETT_TICK_MIN=%llu",
                 (unsigned long long)next, (unsigned long long)GASNETT_TICK_MIN);
          if (next >= GASNETT_TICK_MAX) 
            ERR("gasnett_ticks_to_us()=%llu >= GASNETT_TICK_MAX=%llu",
                 (unsigned long long)next, (unsigned long long)GASNETT_TICK_MAX);
          d_junk *= 1.0001;
          last = next;
        } while (gasnett_ticks_to_us(last-start) < 100000);
      }
      end = gasnett_ticks_now();
      endref = gasnett_gettimeofday_us();

      time = gasnett_ticks_to_us(end) - gasnett_ticks_to_us(start);
      timeref = endref - startref;

      if (abs(timeref - time) > 10000)
        ERR("timer and reference differ by more than 0.01sec:\n"
               "\ttime=%i  timeref=%i\n",time,timeref);

      if (abs( (int)((gasnett_ticks_to_us(end) - gasnett_ticks_to_us(start)) - 
                      gasnett_ticks_to_us(end - start)) ) > 1)
        ERR("ticks_to_us(A) - ticks_to_us(B) != ticks_to_us(A-B)");

      if (abs( (int)(gasnett_ticks_to_ns(end - start)/1000 - 
                     gasnett_ticks_to_us(end - start)) ) > 1)
        ERR("ticks_to_ns(A)/1000 != ticks_to_us(A)");

    }
  }
  { double overhead = gasnett_tick_overheadus();
    double granularity = gasnett_tick_granularityus();
    if (granularity <= 0.0 || overhead <= 0.0 ||
        (granularity+0.1) < 0.5*overhead) 
        /* allow some leeway for noise at granularities approaching cycle speed */
        /*granularity < 0.5*overhead)*/
        ERR("nonsensical timer overhead/granularity measurements:\n"
             "  overhead: %.3fus  granularity: %.3fus\n",overhead, granularity);
  }

  TEST_HEADER("Testing zero-byte counting...")
  { /* gasnett_count0s*() */
    static const char src[24] = { '\0', '1',  '\0', '2', '\0', '3', '\0', '4',
                                  '5',  '6',  '7',  '8', '\0', '9', '\0', 'a',
                                  'b',  '\0', 'c',  'd', 'e',  'f', 'g',  'h' };
    char dst_guarded[24+16];
    char *dst = dst_guarded+8;
    int l, i, j, k, z0, z1, z2;

    for (i=0;i<8;++i) { /* src alignment */
      for (j=0;j<8;++j) { /* dst alignment */
        for (l=0;l<16;++l) { /* length */
          memset(dst_guarded, 0, sizeof(dst_guarded));
	  z0 = gasnett_count0s_copy(dst+j, src+i, l);
	  if (memcmp(dst+j, src+i, l)) ERR("memory mismatch from gasnett_count0s_copy(dst+%i, src+%i, %i)",i,j,l);
	  z1 = gasnett_count0s(dst+j, l);
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
      for (i=0;i<=CNT0SMAX;++i)
	if (gasnett_count0s(src, i) != (unsigned)i)
          ERR("incorrect return from gasnett_count0s(string-of-%i-zeros)", i);
      memset(src, 1, CNT0SMAX);
      for (i=0;i<=CNT0SMAX;++i)
	if (gasnett_count0s(src, i) != 0)
          ERR("incorrect return from gasnett_count0s(string-of-%i-nonzeros)", i);
      test_free(src);
    }

    for (i=0;i<8*(int)sizeof(uintptr_t);++i) {
      uintptr_t val = ((uintptr_t)1) << i;
      if (gasnett_count0s_uintptr_t(val) != (sizeof(uintptr_t) - 1))
        ERR("incorrect return from gasnett_count0s_uintptr_t(1<<%i)", i);
    }
    if (gasnett_count0s_uintptr_t(0) != sizeof(uintptr_t))
      ERR("incorrect return from gasnett_count0s_uintptr_t(0)");

    for (i=0;i<32;++i) {
      uint32_t val = ((uint32_t)1) << i;
      if (gasnett_count0s_uint32_t(val) != 3)
        ERR("incorrect return from gasnett_count0s_uint32_t(1<<%i)", i);
    }
    if (gasnett_count0s_uint32_t(0) != 4)
      ERR("incorrect return from gasnett_count0s_uint32_t(0)");

    for (i=0;i<64;++i) {
      uint64_t val = ((uint64_t)1) << i;
      if (gasnett_count0s_uint64_t(val) != 7)
        ERR("incorrect return from gasnett_count0s_uint64_t(1<<%i)", i);
    }
    if (gasnett_count0s_uint64_t(0) != 8)
      ERR("incorrect return from gasnett_count0s_uint64_t(0)");
  }

  TEST_HEADER("Testing local membars...")
  { /* local membar */
    int i;
    for (i=0;i<iters;i++) {
      gasnett_local_mb();
    }
    for (i=0;i<iters;i++) {
      gasnett_weak_mb();
    }
  }

  TEST_HEADER("Testing local write membars...")
  { /* local membar */
    int i;
    for (i=0;i<iters;i++) {
      gasnett_local_wmb();
    }
    for (i=0;i<iters;i++) {
      gasnett_weak_wmb();
    }
  }

  TEST_HEADER("Testing local read membars...")
  { /* local membar */
    int i;
    for (i=0;i<iters;i++) {
      gasnett_local_rmb();
    }
    for (i=0;i<iters;i++) {
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
    int i = 0;
    gasnett_atomic_t var = gasnett_atomic_init(10);

    if (gasnett_atomic_read(&var,0) != 10)
      ERR("gasnett_atomic_init/gasnett_atomic_read got wrong value");

    gasnett_atomic_set(&var, 2*iters, 0);
    if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters))
      ERR("gasnett_atomic_set/gasnett_atomic_read got wrong value");

    for (i=1;i<=iters;i++) {
      gasnett_atomic_increment(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters + i))
        ERR("gasnett_atomic_increment got wrong value");
    }

    for (i=iters-1;i>=0;i--) {
      gasnett_atomic_decrement(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(2*iters + i))
        ERR("gasnett_atomic_decrement got wrong value");
    }

    for (i=1;i<=iters;i++) {
      gasnett_atomic_set(&var, i, 0);
      gasnett_atomic_increment(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i+1))
        ERR("gasnett_atomic_set/gasnett_atomic_increment got wrong value");
    }

    for (i=1;i<=iters;i++) {
      gasnett_atomic_set(&var, i, 0);
      gasnett_atomic_decrement(&var,0);
      if (gasnett_atomic_read(&var,0) != (gasnett_atomic_val_t)(i-1))
        ERR("gasnett_atomic_set/gasnett_atomic_decrement got wrong value");
    }

    gasnett_atomic_set(&var, iters, 0);
    for (i=iters-1;i>=1;i--) {
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
      for (i=0;i<=iters;i++) {
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
    #endif

    #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      gasnett_atomic_set(&var, 1, 0);
      for (i=1;i<=iters;i++) {
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
      uint32_t tmp32;

      if (!gasnett_atomic32_read(&var32,0) != 0)
        ERR("gasnett_atomic32_init/gasnett_atomic32_read got wrong value");

      gasnett_atomic32_set(&var32, 2*iters, 0);
      if (gasnett_atomic32_read(&var32,0) != (uint32_t)(2*iters))
        ERR("gasnett_atomic32_set/gasnett_atomic32_read got wrong value");

      /* single bit-marching tests */
      for (i=0;i<32;i++) {
        gasnett_atomic32_set(&var32, one32<<i, 0);
	tmp32 = gasnett_atomic32_read(&var32, 0);
	if (tmp32 != (one32<<i))
          ERR("gasnett_atomic32_set/gasnett_atomic32_read got wrong value on bit %i", i);
	if (gasnett_atomic32_compare_and_swap(&var32, 0, tmp32, 0))
          ERR("gasnett_atomic32_compare_and_swap succeeded at bit %i when it should have failed", i);
	if (!gasnett_atomic32_compare_and_swap(&var32, tmp32, 0, 0))
          ERR("gasnett_atomic32_compare_and_swap failed at bit %i when it should have succeeded", i);
      }

      /* double bit-marching tests */
      for (i=0;i<32;i++) {
        int j;
        for (j=0;j<i;j++) {
          tmp32 = (one32<<i) | (one32<<j);
          gasnett_atomic32_set(&var32, tmp32, 0);
          if (gasnett_atomic32_compare_and_swap(&var32, (one32<<i), tmp32, 0) ||
              gasnett_atomic32_compare_and_swap(&var32, (one32<<j), tmp32, 0))
            ERR("gasnett_atomic32_compare_and_swap succeeded at bits %i and %i when it should have failed", i, j);
        }
      }

      gasnett_atomic32_set(&var32, 0, 0);
      for (i=0;i<=iters;i++) {
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
    }

    {
      gasnett_atomic64_t var64 = gasnett_atomic64_init(~(uint64_t)0);
      const uint64_t one64 = 1;
      uint64_t tmp64;

      if (~gasnett_atomic64_read(&var64,0) != 0)
        ERR("gasnett_atomic64_init/gasnett_atomic64_read got wrong value");

      gasnett_atomic64_set(&var64, 2*iters, 0);
      if (gasnett_atomic64_read(&var64,0) != (uint64_t)(2*iters))
        ERR("gasnett_atomic64_set/gasnett_atomic64_read got wrong value");

      /* single bit-marching tests */
      for (i=0;i<64;i++) {
        gasnett_atomic64_set(&var64, one64<<i, 0);
	tmp64 = gasnett_atomic64_read(&var64, 0);
	if (tmp64 != (one64<<i))
          ERR("gasnett_atomic64_set/gasnett_atomic64_read got wrong value on bit %i", i);
	if (gasnett_atomic64_compare_and_swap(&var64, 0, tmp64, 0))
          ERR("gasnett_atomic64_compare_and_swap succeeded at bit %i when it should have failed", i);
	if (!gasnett_atomic64_compare_and_swap(&var64, tmp64, 0, 0))
          ERR("gasnett_atomic64_compare_and_swap failed at bit %i when it should have succeeded", i);
      }

      /* double bit-marching tests */
      for (i=0;i<64;i++) {
        int j;
        for (j=0;j<i;j++) {
          tmp64 = (one64<<i) | (one64<<j);
          gasnett_atomic64_set(&var64, tmp64, 0);
          if (gasnett_atomic64_compare_and_swap(&var64, (one64<<i), tmp64, 0) ||
              gasnett_atomic64_compare_and_swap(&var64, (one64<<j), tmp64, 0))
            ERR("gasnett_atomic64_compare_and_swap succeeded at bits %i and %i when it should have failed", i, j);
        }
      }

      gasnett_atomic64_set(&var64, 0, 0);
      for (i=0;i<=iters;i++) {
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
    }
  }

  TEST_HEADER("Testing client-provided backtrace code...") {  
    TEST_BACKTRACE();
  }

#ifdef HAVE_PTHREAD_H
  MSG("Spawning pthreads...");
  { 
    int i;
    for(i=0;i<NUM_THREADS;i++) gasnett_atomic_set(thread_flag+i,1,0);
    gasnett_local_mb();
    test_createandjoin_pthreads(NUM_THREADS, &thread_fn, NULL, 0);
  }
#endif

  MSG("Done.");
  return (test_errs > 0 ? 1 : 0);
}

#ifdef HAVE_PTHREAD_H

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
  int id = (int)(uintptr_t)arg;
  int i;
  int iters2;
  int barcnt = 0;
  char th_test_section = test_section;
  #define test_section th_test_section
 
  /* Avoid overflow to ensure a sane iters2 value. */
  iters2 = (iters >= (INT_MAX / 100)) ? INT_MAX : (100 * iters);
  /* Parallel atomic-op pounding test assumes iters2 is even */
  iters2 &= ~1;

  /* sanity check - ensure unique threadids */
  if (!gasnett_atomic_decrement_and_test(thread_flag+id,0)) {
      ERR("thread %i failed sanity check", id);
  }

  /* sanity check - ensure thread barriers are working */
  TEST_HEADER("parallel atomic-op barrier test...") {  
    for (i=0;i<iters;i++) {
      int tmp;
      /* simple count-up barrier */
      gasnett_atomic_increment(&up,0);
      while (gasnett_atomic_read(&up,0) < (gasnett_atomic_val_t)NUM_THREADS) gasnett_sched_yield(); 

      gasnett_atomic_set(&down, 2*NUM_THREADS, 0);

      /* Why the _REL?. The set(down) above must complete before the inc(up).
       * Otherwise, it might clobber a decrement by another thread in the count-down below. */
      gasnett_atomic_increment(&up, GASNETT_ATOMIC_REL);
      while (gasnett_atomic_read(&up,0) < (gasnett_atomic_val_t)(2*NUM_THREADS)) gasnett_sched_yield(); 

      tmp = gasnett_atomic_read(&up,0);
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
    int val;
    gasnett_atomic_set(&x1, 5, 0);
    gasnett_atomic_set(&x2, 5+iters2*NUM_THREADS, 0);
    gasnett_atomic_set(&x3, 5, 0);
    gasnett_atomic_set(&x4, 5+iters2*NUM_THREADS, 0);

    THREAD_BARRIER();

    for (i=0;i<iters2;i++) {
      gasnett_atomic_increment(&x1,0);
      gasnett_atomic_decrement(&x2,0);
    }
    #if defined(GASNETT_HAVE_ATOMIC_ADD_SUB)
      for (i=0;i<iters2;i++) {
	val = (i & 1) << 1; /* Alternate 0 and 2. (iters2=100*iters is always even) */
        gasnett_atomic_add(&x3,val,0);
        gasnett_atomic_subtract(&x4,val,0);
      }
    #endif

    THREAD_BARRIER();

    val = gasnett_atomic_read(&x1,0);
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

    for (i=0;i<iters;i++) {
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
      for (i=0;i<iters2;i++) {
        uint32_t v;
        gasnett_atomic_set(&x3, myval, 0);
        gasnett_atomic_set(&x4, myval, 0);
        gasnett_atomic_set(&x5, myval, 0);
        gasnett_atomic_increment(&x4,0);
        gasnett_atomic_decrement(&x5,0);
        v = gasnett_atomic_read(&x3,0);
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
      for (i=0;i<iters2;i++) {
        uint32_t v;
        gasnett_atomic32_set(&a1, myval, 0);
        gasnett_atomic32_set(&a2, myval, 0);
	v = gasnett_atomic32_read(&a2,0);
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

      for (i=0;i<iters2;i++) {
        uint64_t v;
        gasnett_atomic64_set(&s1.a, myval, 0);
        gasnett_atomic64_set((gasnett_atomic64_t *)(void *)&s2.d, myval, 0); /* (void*) suppresses g++ warning (bug 2158) */
        gasnett_atomic64_set(&a1, myval, 0);
        gasnett_atomic64_set(&a2, myval, 0);
	v = gasnett_atomic64_read(&a2,0);
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
    { int partner = (id + 1) % NUM_THREADS;
      unsigned int lx, ly;
      /* Allow for wrap-around/overflow */
      #define BIGGER(_x, _y) ((int)(_y - _x) > 0)

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (i=0;i<iters2;i++) {
        valX[id] = i;
        gasnett_local_wmb();
        valY[id] = i;

        ly = valY[partner];
        gasnett_local_rmb();
        lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_local_wmb/gasnett_local_rmb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (i=0;i<iters2;i++) {
        valX[id] = i + iters2;
        gasnett_local_mb();
        valY[id] = i + iters2;

        ly = valY[partner];
        gasnett_local_mb();
        lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_local_mb/gasnett_local_mb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (i=0;i<iters2;i++) {
        valX[id] = i;
        gasnett_weak_wmb();
        valY[id] = i;

        ly = valY[partner];
        gasnett_weak_rmb();
        lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_weak_wmb/gasnett_weak_rmb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();

      valX[id] = 0;
      valY[id] = 0;

      THREAD_BARRIER();
      for (i=0;i<iters2;i++) {
        valX[id] = i + iters2;
        gasnett_weak_mb();
        valY[id] = i + iters2;

        ly = valY[partner];
        gasnett_weak_mb();
        lx = valX[partner];
        if (BIGGER(lx,ly)) ERR("mismatch in gasnett_weak_mb/gasnett_weak_mb test: lx=%u ly=%u", lx, ly);
      }
      THREAD_BARRIER();
    }
  }

  TEST_HEADER("parallel compare-and-swap test...") {
    #if defined(GASNETT_HAVE_ATOMIC_CAS)
      static gasnett_atomic_t counter2 = gasnett_atomic_init(0);
      static uint32_t shared_counter = 0;
      uint32_t woncnt = 0;
      uint32_t share = ((unsigned)iters >= (0xffffffffU / NUM_THREADS)) ? (0xffffffffU / NUM_THREADS) : iters;
      uint32_t goal = NUM_THREADS * share;
      uint32_t i, oldval;

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
      for (i=0;i<share;i++) {
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
      for (i=0;i<share;i++) {
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
      uint32_t share = ((unsigned)iters >= (0xffffffffU / NUM_THREADS)) ? (0xffffffffU / NUM_THREADS) : iters;
      uint32_t goal = NUM_THREADS * share;
      uint32_t oldval;

      /* Look for missing or doubled updates by taking an equal share of increments */
      while (woncnt < share && (oldval = gasnett_atomic32_read(&counter32,0)) != goal) {
        if (gasnett_atomic32_compare_and_swap(&counter32, oldval, (oldval + 1), 0)) {
           woncnt++;
        }
      }
      THREAD_BARRIER();
      oldval = gasnett_atomic32_read(&counter32,0);
      if (oldval != goal) 
        ERR("failed 32-bit compare-and-swap test: counter=%u expecting=%u", (unsigned)oldval, (unsigned)goal);
      if (woncnt != share) 
        ERR("failed 32-bit compare-and-swap test: woncnt=%u share=%u", (unsigned)woncnt, (unsigned)share);
    }

    {
      static gasnett_atomic64_t counter64 = gasnett_atomic64_init(0);
      uint64_t woncnt = 0;
      uint64_t share = iters;
      uint64_t goal = NUM_THREADS * share; /* Not going to overflow */
      uint64_t oldval;

      /* Look for missing or doubled updates by taking an equal share of increments */
      while (woncnt < share && (oldval = gasnett_atomic64_read(&counter64,0)) != goal) {
        if (gasnett_atomic64_compare_and_swap(&counter64, oldval, (oldval + 1), 0)) {
           woncnt++;
        }
      }
      THREAD_BARRIER();
      oldval = gasnett_atomic64_read(&counter64,0);
      if (oldval != goal) 
        ERR("failed 64-bit compare-and-swap test: counter=%llu expecting=%llu", (unsigned long long)oldval, (unsigned long long)goal);
      if (woncnt != share) 
        ERR("failed 64-bit compare-and-swap test: woncnt=%llu share=%llu", (unsigned long long)woncnt, (unsigned long long)share);
    }
  }

  TEST_HEADER("parallel atomic-op fence test...") {
    int partner = (id + 1) % NUM_THREADS;
    unsigned int lx, ly;

    gasnett_atomic_set(&atomicX[id], 0, 0);
    gasnett_atomic32_set(&atomicX32[id], 0, 0);
    gasnett_atomic64_set(&atomicX64[id], 0, 0);
    valY[id] = 0;
    valY32[id] = 0;
    valY64[id] = 0;

    THREAD_BARRIER();

    /* First a pass through w/ WMB and RMB */
    for (i=0;i<iters;i++) {
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

    for (i=iters-1;i>=0;i--) {
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
    for (i=0;i<iters;i++) {
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

    for (i=iters-1;i>=0;i--) {
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
