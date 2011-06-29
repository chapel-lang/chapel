/*   $Source: /var/local/cvs/gasnet/tests/testlockcontend.c,v $
 *     $Date: 2009/04/20 06:40:08 $
 * $Revision: 1.7 $
 * Description: GASNet lock performance test
 *   Measures the overhead associated with contended locks
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <test.h>

#ifndef GASNET_PAR
#error This test can only be built for GASNet PAR configuration
#endif

int mynode = 0;
int iters=0;
void *myseg = NULL;
int accuracy = 0;
int maxthreads = 2;
int threads = 1;

void header(const char *desc) {
      printf("\n---- %s ----\n"
             "   Threads    Total time    Avg. time\n"
             "   -------    ----------    ---------\n", desc);
}
void report(int threads, int64_t totaltime, int iters) {
      char format[80];
      sprintf(format, "%c:  %%6i     %%%i.%if s  %%%i.%if us\n", 
              TEST_SECTION_NAME(), (4+accuracy), accuracy, (4+accuracy), accuracy);
      printf(format, threads, totaltime/1.0E9, (totaltime/1000.0)/iters);
      fflush(stdout);
}

/* placed in a function to avoid excessive inlining */
gasnett_tick_t ticktime(void) { return gasnett_ticks_now(); }
uint64_t tickcvt(gasnett_tick_t ticks) { return gasnett_ticks_to_ns(ticks); }

void* thread_fn1(void*);
void* thread_fn2(void*);
void* thread_fn3(void*);
void* thread_fn4(void*);

/* ------------------------------------------------------------------------------------ */
/* This tester measures the performance of contended HSLs and pthread mutexes.
 */
int main(int argc, char **argv) {
  

  GASNET_Safe(gasnet_init(&argc, &argv));
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testlockcontend",1,"(maxthreads) (iters) (accuracy) (test sections)");

  if (argc > 1) maxthreads = atoi(argv[1]);
  if (maxthreads > TEST_MAXTHREADS || maxthreads < 1) {
    printf("Threads must be between 1 and %i\n", TEST_MAXTHREADS);
    gasnet_exit(-1);
  }

  if (argc > 2) iters = atoi(argv[2]);
  if (!iters) iters = 1000000;

  if (argc > 3) accuracy = atoi(argv[3]);
  if (!accuracy) accuracy = 3;

  if (argc > 4) TEST_SECTION_PARSE(argv[4]);

  if (argc > 5) test_usage();

  mynode = gasnet_mynode();
  myseg = TEST_MYSEG();

  if (mynode == 0) {
    printf("Running locks performance test with 1..%i threads and %i iterations...\n",maxthreads,iters);
    fflush(stdout);
    MSG0("Spawning pthreads...");
    if (TEST_SECTION_BEGIN_ENABLED()) {
      header("lock/unlock contended pthread mutex (others in thread barrier)");
      test_createandjoin_pthreads(threads = maxthreads, &thread_fn1, NULL, 0);
    }
    if (TEST_SECTION_BEGIN_ENABLED()) {
      header("lock/unlock contended HSL (others in thread barrier)");
      test_createandjoin_pthreads(threads = maxthreads, &thread_fn2, NULL, 0);
    }
    if (TEST_SECTION_BEGIN_ENABLED()) {
      header("lock/unlock contended pthread mutex (no other threads)");
      for (threads=1; threads<=maxthreads; ++threads) {
	test_createandjoin_pthreads(threads, &thread_fn3, NULL, 0);
      }
    }
    if (TEST_SECTION_BEGIN_ENABLED()) {
      header("lock/unlock contended HSL (no other threads)");
      for (threads=1; threads<=maxthreads; ++threads) {
	test_createandjoin_pthreads(threads, &thread_fn4, NULL, 0);
      }
    }
  }

  BARRIER();
  MSG("done.");

  gasnet_exit(0);
  return 0;
}

/* ------------------------------------------------------------------------------------ */
#undef MSG0
#undef ERR
#define MSG0 THREAD_MSG0(id)
#define ERR  THREAD_ERR(id)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char _pad[GASNETT_CACHE_LINE_BYTES] = { 0 }; /* bug 2231 workaround */
gasnet_hsl_t hsl = GASNET_HSL_INITIALIZER;

/* ------------------------------------------------------------------------------------ */
#define TIME_OPERATION_SOME(id, op)                             \
do {                                                            \
  PTHREAD_LOCALBARRIER(threads);                                \
  { int i, _thr, _iters = iters, _warmupiters = MAX(1,iters/10);\
    gasnett_tick_t start,end;  /* use ticks interface */        \
    for (i=0; i < _warmupiters; i++) { op; } /* warm-up */      \
    for (_thr = 1; _thr <= threads; ++_thr) {                   \
      PTHREAD_LOCALBARRIER(threads);                            \
      start = ticktime();                                       \
      if (id < _thr) for (i=0; i < _iters; i++) { op; }         \
      PTHREAD_LOCALBARRIER(threads);                            \
      end = ticktime();                                         \
      if (!id)                                                  \
        report(_thr, tickcvt(end - start), iters);              \
    }                                                           \
  }                                                             \
} while (0)

void * thread_fn1(void *arg) { GASNET_BEGIN_FUNCTION();
  int id = (int)(uintptr_t)arg;
  TIME_OPERATION_SOME(id, { pthread_mutex_lock(&mutex); pthread_mutex_unlock(&mutex); });
  return NULL;
}

void * thread_fn2(void *arg) { GASNET_BEGIN_FUNCTION();
  int id = (int)(uintptr_t)arg;
  TIME_OPERATION_SOME(id, { gasnet_hsl_lock(&hsl); gasnet_hsl_unlock(&hsl); });
  return NULL;
}

/* ------------------------------------------------------------------------------------ */
#define TIME_OPERATION_ALL(id, op)                              \
do {                                                            \
  PTHREAD_LOCALBARRIER(threads);                                \
  { int i, _iters = iters, _warmupiters = MAX(1,iters/10);      \
    gasnett_tick_t start,end;  /* use ticks interface */        \
    for (i=0; i < _warmupiters; i++) { op; } /* warm-up */      \
    PTHREAD_LOCALBARRIER(threads);                              \
    start = ticktime();                                         \
    for (i=0; i < _iters; i++) { op; }                          \
    PTHREAD_LOCALBARRIER(threads);                              \
    end = ticktime();                                           \
    if (!id)                                                    \
      report(threads, tickcvt(end - start), iters);             \
  }                                                             \
} while (0)

void * thread_fn3(void *arg) { GASNET_BEGIN_FUNCTION();
  int id = (int)(uintptr_t)arg;
  TIME_OPERATION_ALL(id, { pthread_mutex_lock(&mutex); pthread_mutex_unlock(&mutex); });
  return NULL;
}

void * thread_fn4(void *arg) { GASNET_BEGIN_FUNCTION();
  int id = (int)(uintptr_t)arg;
  TIME_OPERATION_ALL(id, { gasnet_hsl_lock(&hsl); gasnet_hsl_unlock(&hsl); });
  return NULL;
}
/* ------------------------------------------------------------------------------------ */

