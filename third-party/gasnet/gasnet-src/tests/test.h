/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/test.h $
 * Description: helpers for GASNet tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


#ifndef _TEST_H
#define _TEST_H

#ifdef TEST_GASNET_TOOLS_ONLY
  /* do not use gasnetex.h */
  #include <gasnet_tools.h>
#else
  #include <gasnetex.h>
  #include <gasnet_tools.h>
  #define TEST_GASNETEX_H
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

#if defined(HAVE_PTHREAD_H) && \
    (defined(GASNET_PAR) || defined(GASNET_PARSYNC) || GASNETT_THREAD_SAFE)
  #define TEST_PAR 1
  #include <pthread.h>
#endif

GASNETT_BEGIN_EXTERNC

#if !defined(DEBUG) && !defined(NDEBUG)
  #ifdef GASNET_DEBUG
    #define DEBUG 1
  #else
    #define NDEBUG 1
  #endif
#endif

#if PLATFORM_COMPILER_SUN_C
  /* disable a harmless warning */
  #pragma error_messages(off, E_STATEMENT_NOT_REACHED)
#endif

/* bug 1206: several systems (notably Compaq C++ and OSX gcc) have an assert.h header 
   which is broken in one or more subtle ways. Don't trust the system assert.h. */
#include <assert.h>
#undef assert

/* Grumble... On Solaris-x86 /usr/include/sys/regset.h polutes the namespace */
#if PLATFORM_OS_SOLARIS && (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64)
  #if defined(_SYS_REGSET_H) && defined(ERR)
    #undef ERR
  #endif
#endif

// this macro is for annotating variables as "used" so compilers don't warn
#if PLATFORM_COMPILER_OPEN64
  #define test_mark_used(var) ((void)var)
#else
  #define test_mark_used(var) ((void)(&var?0:0))
#endif

#if HAVE_SRAND_DETERMINISTIC
  /* OpenBSD "breaks" rand() by design */
  #define srand(seed) srand_deterministic(seed)
#endif

#define assert_always(expr) \
    ((expr) ? (void)0 : (void)FATALERR("Assertion failure: %s", #expr))

#ifdef NDEBUG
  #define assert(x) ((void)0)
#else
  #define assert(x) assert_always(x)
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

/* ------------------------------------------------------------------------------------ */
// Static assertions : assert a property at compile time, generate an error otherwise
// `cond` must be an integer constant expression - ie no non-integer types or variable references

// static assertion in a block scope
// safe to use multiple times per line (eg in a macro expansion)
#define test_static_assert(cond) do { \
  static const char *_test_static_assert[ (cond) ?1:-1] = { "Static assertion: " #cond }; \
  test_mark_used(_test_static_assert); \
} while (0)

// static assertion at file scope
// safe to use multiple times per line only in C99 mode, not C++ mode
#define test_static_assert_file(cond) \
  static const char *_CONCAT(_test_static_assert_file_,__LINE__)[ (cond) ?1:-1]

/* ------------------------------------------------------------------------------------ */
/* generic message output utility
   test_makeMsg(baseformatargs, msgpred, isfatal, msgeval): 
     baseformatargs - parenthesized printf-style argument defining the generic stem
     msgpred - predicate which must evaluate to true to perform message output (eg 1 for always)
     isfatal - literal 1 to request an abort after the message output, literal 0 otherwise
     msgeval - expression which is evaluated in a critical section, 
               immediately before each message output (or 0 for none)
 */
#if PLATFORM_COMPILER_PATHSCALE
  typedef void (*_testformatter_fn_t)(const char *format, ...);
  #define BUG3343_WORKAROUND(expr) ((_testformatter_fn_t)(expr))
#else
  #define BUG3343_WORKAROUND(expr) expr
#endif
#define test_makeMsg(baseformatargs, msgpred, isfatal, msgeval)     \
  BUG3343_WORKAROUND(                                               \
  ( _test_makeErrMsg baseformatargs ,                               \
    ( (msgpred) ? (void)(msgeval) : (void)(_test_squashmsg = 1) ) , \
    _test_doErrMsg##isfatal ) )

#ifdef _INCLUDED_GASNET_H
  #define TEST_MYPROC gasnet_mynode()
  #define TEST_PROCS  gasnet_nodes()
#else
  #define TEST_MYPROC gex_System_QueryJobRank()
  #define TEST_PROCS  gex_System_QueryJobSize()
#endif

/* define several useful messaging macros */
static int test_errs = 0;
#ifdef TEST_GASNETEX_H
  #define MSG   test_makeMsg(("node %i/%i %s\n", (int)TEST_MYPROC, (int)TEST_PROCS, "%s"), 1, 0, \
                             GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__))
  #define MSG0  test_makeMsg(("%s\n","%s"), (TEST_MYPROC == 0), 0, \
                             GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__))
  #define _TERR(isfatal)                                                                                        \
                test_makeMsg(("ERROR: node %i/%i %s (at %s:%i)\n",                                             \
                              (int)TEST_MYPROC, (int)TEST_PROCS, "%s",__FILE__, __LINE__), 1, isfatal, \
                             (test_errs++, GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__)))
  #define THREAD_MSG0(id)  test_makeMsg(("%s\n","%s"), (TEST_MYPROC == 0 && id == 0), 0, 0)
  #define THREAD_ERR(id)   test_makeMsg(("ERROR: node %i/%i thread %i: %s (at %s:%i)\n", \
                              (int)TEST_MYPROC, (int)TEST_PROCS,id, "%s", __FILE__, __LINE__), 1, 0, test_errs++)
#else
  #define MSG   test_makeMsg(("%s\n","%s"), 1, 0, 0)
  #define MSG0  MSG
  #define _TERR(isfatal) test_makeMsg(("ERROR: %s (at %s:%i)\n","%s",__FILE__, __LINE__), 1, isfatal, test_errs++)
  #define THREAD_MSG0(id)  test_makeMsg(("%s\n","%s"), (id == 0), 0, 0)
  #define THREAD_ERR(id)   test_makeMsg(("ERROR: thread %i: %s (at %s:%i)\n", \
                              id, "%s", __FILE__, __LINE__), 1, 0, test_errs++)
#endif
#define ERR      _TERR(0)
#define FATALERR _TERR(1)
/* The following two help us avoid warnings from a gcc that doesn't like
   seeing a lone non-literal argument to printf() and family. */
#define PUTS(_s)  MSG("%s",(_s))
#define PUTS0(_s) MSG0("%s",(_s))

#define _TEST_MSG_BUFSZ 1024
static char _test_baseformat[_TEST_MSG_BUFSZ];
static volatile int _test_squashmsg = 0;
#if TEST_PAR
  static gasnett_mutex_t _test_msg_lock = GASNETT_MUTEX_INITIALIZER;
  #define _test_LOCKMSG()   gasnett_mutex_lock(&_test_msg_lock)
  #define _test_UNLOCKMSG() gasnett_mutex_unlock(&_test_msg_lock)
#else
  #define _test_LOCKMSG()   ((void)0)
  #define _test_UNLOCKMSG() ((void)0)
#endif

/* define two versions of _test_doErrMsg, 
 * so the fatal one can have a GASNETT_NORETURN attribute */
#define _test_doErrMsg_functor(suff, code)                             \
GASNETT_FORMAT_PRINTF(_test_doErrMsg##suff,1,2,                        \
static void _test_doErrMsg##suff(const char *format, ...)) {           \
  if (_test_squashmsg) _test_squashmsg = 0;                            \
  else {                                                               \
    char output[_TEST_MSG_BUFSZ];                                      \
    va_list argptr;                                                    \
    va_start(argptr, format); /*  pass in last argument */             \
      { int sz = vsnprintf(output, _TEST_MSG_BUFSZ, format, argptr);   \
        if (sz >= (_TEST_MSG_BUFSZ-5) || sz < 0)                       \
           strcpy(output+(_TEST_MSG_BUFSZ-5),"...");                   \
      }                                                                \
    va_end(argptr);                                                    \
    printf(_test_baseformat, output);                                  \
    GASNETT_TRACE_PRINTF(_test_baseformat, output);                    \
    fflush(stdout);                                                    \
  }                                                                    \
  code /* conditionally fatal */                                       \
}

_test_doErrMsg_functor(0, { _test_UNLOCKMSG(); })

GASNETT_NORETURN
_test_doErrMsg_functor(1, { fflush(NULL); sleep(1); abort(); })
GASNETT_NORETURNP(_test_doErrMsg1)
#undef _test_doErrMsg_functor

GASNETT_FORMAT_PRINTF(_test_makeErrMsg,1,2,
static void _test_makeErrMsg(const char *format, ...)) {
  va_list argptr;
  _test_LOCKMSG();
  va_start(argptr, format); /*  pass in last argument */
    { int sz = vsnprintf(_test_baseformat, _TEST_MSG_BUFSZ, format, argptr);
      if (sz >= (_TEST_MSG_BUFSZ-5) || sz < 0) strcpy(_test_baseformat+(_TEST_MSG_BUFSZ-5),"...");
    }
  va_end(argptr);
}
/* ------------------------------------------------------------------------------------ */
/* misc tools */

#ifndef MIN
  #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef MAX
  #define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#define test_checksum gasnett_checksum

#define PAGESZ GASNETT_PAGESIZE
#define alignup(a,b) ((((a)+(b)-1)/(b))*(b))
#define alignup_ptr(a,b) ((void *)(((((uintptr_t)(a))+(b)-1)/(b))*(b)))
#define aligndown(a,b) (((a)/(b))*(b))

// Two 64-bit PRNGs are available
//  + "LCG64" uses 64-bit Linear Congruential Generator
//  + "RANDn" uses multiple calls to rand().
#if defined(_TEST_USE_LCG64)
  // Keep setting
#elif (RAND_MAX < 255) || \
      PLATFORM_COMPILER_OPEN64 || PLATFORM_COMPILER_CLANG
  // SHOULD NOT use "RANDn" in the following cases:
  // + Any platform with RAND_MAX < 255 (though C99 requires 32K+)
  // + Open64 to work-around bug 3738
  // + Clang to work-around bug 3630
  #define _TEST_USE_LCG64 1
#else
  // Current default is LCG64
  #define _TEST_USE_LCG64 1
#endif

#if _TEST_USE_LCG64
  // NOTE: this state and seed operation are per compilation unit.
  // TODO: If/when we write tests spanning multiple source files we may
  // need to move the PRNG state into its own object file.
  static uint64_t _test_rand_val = 5551212;
  #define TEST_SRAND(seed)  ((void)(_test_rand_val = (seed)))
#else
  #define TEST_SRAND(seed)  srand(seed)
#endif

// Some platforms are missing these from stdint.h by default in C++ mode (without __STDC_LIMIT_MACROS)
#ifndef INT64_MAX
  #ifdef __INT64_MAX__
    #define INT64_MAX __INT64_MAX__
  #else
    #define INT64_MAX (0x7fffffffffffffffLL)
  #endif
#endif
#ifndef INT64_MIN
#define INT64_MIN (-INT64_MAX - 1)
#endif

// NOTE: (high - low + 1) must be <= INT64_MAX to avoid undefined behavior
static int64_t _test_rand(int64_t low, int64_t high) {
  assert(low <= high);
  assert(low <= high+1); /* We will overflow otherwise */
  // conservatively avoid the use of the high bit to avoid any chance of overflow
  // this could be made tighter, but this utility is not intended for generating 64-bits of randomness
  assert(low > INT64_MIN/2);
  assert(high < INT64_MAX/2);
  uint64_t const range = high - low + 1;
#if _TEST_USE_LCG64
  // Implement the well-known LCG PRNG with widely used parameters.
  // For an explanation of the LCG algorithm:
  //     https://en.wikipedia.org/wiki/Linear_congruential_generator
  // or any CS text (such as Knuth) covering pseudo-random number generation.
  // For an example of where this particular generator is in use:
  //     https://nuclear.llnl.gov/CNP/rng/rngman/
  // and specifically the description of *this* generator:
  //     https://nuclear.llnl.gov/CNP/rng/rngman/node4.html
  // which states this multiplier is also the default for lcg64 in SPRNG:
  //     http://www.sprng.org/sprng.html
  _test_rand_val *= 2862933555777941757ull;
  _test_rand_val += 3037000493ull;
  uint64_t val = _test_rand_val;
#else
  // The "RANDn" generator accumulates 8 random bits at a time using
  // multiple calls to libc's rand(), making only as many calls as
  // are required to meet the requested [low,high] range.
  assert(RAND_MAX >= 255); // C99 guarantees 32k+
  uint64_t rs = range;
  uint64_t val = 0;
  do { 
    val = (val << 8) ^ rand();
    rs >>= 8;
  } while (rs);
#endif
  int64_t result = (int64_t)(val % range) + low;
  assert(result >= low && result <= high);
  return result;
}
#define TEST_RAND(low,high) _test_rand((low), (high))
#define TEST_RAND_PICK(a,b) (TEST_RAND(0,1)==1?(a):(b))
#define TEST_RAND_ONEIN(p)  (TEST_RAND(1,p) == 1)

#define TEST_HIWORD(arg)     ((uint32_t)(((uint64_t)(arg)) >> 32))
#define TEST_LOWORD(arg)     ((uint32_t)((uint64_t)(arg)))

#define check_zeroret(op) do {                                       \
  int _retval = (op);                                                \
  if_pf(_retval) FATALERR(#op": %s(%i)",strerror(_retval), _retval); \
} while (0)

#define check_nzeroret(op) do {                                       \
  int _retval = (op);                                                 \
  if_pf(!_retval) FATALERR(#op": %s(%i)",strerror(_retval), _retval); \
} while (0)

static char test_section;
static char test_sections[255];

#define TEST_SECTION_BEGIN()        ((void)(!test_section ? test_section = 'A' : test_section++))
#define TEST_SECTION_ENABLED()      ((!test_sections[0] || strchr(test_sections, test_section)))
#define TEST_SECTION_BEGIN_ENABLED() (TEST_SECTION_BEGIN(), TEST_SECTION_ENABLED())
#define TEST_SECTION_NAME() ((char)test_section)
#define TEST_SECTION_PARSE(arg) do {       \
      const char *p = (arg);               \
      char *q = test_sections;             \
      while (*p) *(q++) = toupper((int)*(p++)); \
    } while (0)

/* ------------------------------------------------------------------------------------ */
/* timing - TIME() returns a microsecond time-stamp */

#ifdef FORCE_GETTIMEOFDAY
  #define TIME() gasnett_gettimeofday_us()
#else
  #define TIME() gasnett_ticks_to_us(gasnett_ticks_now()) 
#endif

#define test_ns_to_ticks(_ns) \
 (gasnett_tick_t)((_ns)*((double)(1<<30))/gasnett_ticks_to_ns(1<<30))

/* ------------------------------------------------------------------------------------ */
// Auto-scaling Iteration support
// Macros to support scaling the iterations of a test to meet a target running time
// For macro usage example, see testvisperf.c
// For operational details, build with MANUAL_DEFINES=-DTEST_ASI_DEBUG
#ifndef TEST_ASI_DEBUG
#define TEST_ASI_DEBUG 0
#endif
#ifndef TEST_ASI_BANKS
#define TEST_ASI_BANKS 10
#endif
#ifndef TEST_ASI_MININTERVAL
#define TEST_ASI_MININTERVAL  0.001  // min acceptable interval, in seconds
#endif
static gasnett_tick_t _test_asi_interval = 0; // min interval and enable
static gasnett_tick_t _test_asi_begin = 0;
static size_t _test_asi_iters[TEST_ASI_BANKS];
static size_t _test_asi_sz[TEST_ASI_BANKS];
static size_t _test_asi_retry;
static int _test_asi_bank;

// init the ASI subsystem, with a string specifying the target time interval for each test
#define TEST_ASI_INIT(interval_str) do {                   \
  const char *_str = (interval_str);                       \
  if (!_str || !*_str) _str = "1.0";                       \
  double _time = atof(_str);                               \
  _time = MAX(_time, TEST_ASI_MININTERVAL);                \
  _test_asi_interval = test_ns_to_ticks(_time*1.0e9);      \
  assert_always(labs((long)(_time*1000 -  /* within 10ms */\
    gasnett_ticks_to_ns(_test_asi_interval)/1.0e6)) < 10); \
  if (TEST_ASI_DEBUG)                                      \
    MSG0(" ASI: test interval: %0.3f sec",_time);          \
} while (0)

// inform ASI we are starting a new test so wipe timing state
#define TEST_ASI_NEW_TEST() do {                     \
  memset(_test_asi_sz,0,sizeof(_test_asi_sz));       \
  memset(_test_asi_iters,0,sizeof(_test_asi_iters)); \
} while (0)

// begin a timed region, with a label name and size "cost", computes iters
// test can optionally use up to TEST_ASI_BANKS separate banks of iter/size state
// Call this BEFORE test's own begin timer 
#define TEST_ASI_BEGIN(name,iters,size,bank) do {                 \
  if (_test_asi_interval) {                                       \
   {size_t _new_sz = (size);                                      \
    int _bank = (bank);                                           \
    assert(_bank < TEST_ASI_BANKS);                               \
    assert(_new_sz > 0);                                          \
    _test_asi_retry = 0;                                          \
    if (!_test_asi_sz[_bank]) { /* first use this test */         \
      _test_asi_iters[_bank] = 1; /* slow start and warmup */     \
      _test_asi_sz[_bank] = _new_sz;                              \
    } else if (_new_sz != _test_asi_sz[_bank]) { /* scale */      \
      _test_asi_iters[_bank] *=                                   \
                    ((double) _test_asi_sz[_bank] / _new_sz );    \
      _test_asi_iters[_bank] = MAX(1,_test_asi_iters[_bank]);     \
      _test_asi_sz[_bank] = _new_sz;                              \
    }                                                             \
    _test_asi_bank = _bank;                                       \
   } _TEST_ASI_##name:                                            \
    assert(_test_asi_iters[_test_asi_bank] > 0);                  \
    (iters) = _test_asi_iters[_test_asi_bank];                    \
    _test_asi_begin = gasnett_ticks_now();                        \
  }                                                               \
} while (0)
// end a timed region, may increase iters and branch back to the named label
// call this AFTER test's own end timer 
#define TEST_ASI_END(name,iters) do {                             \
  if (_test_asi_interval) {                                       \
    assert(_test_asi_begin);                                      \
    gasnett_tick_t _int = gasnett_ticks_now() - _test_asi_begin;  \
    _test_asi_begin = 0;                                          \
    if (_int < _test_asi_interval) { /* too fast, retry */        \
      _test_asi_iters[_test_asi_bank] *= 2;                       \
      _test_asi_retry++;                                          \
      goto _TEST_ASI_##name;                                      \
    }                                                             \
    if (TEST_ASI_DEBUG)                                           \
      MSG(" ASI: retries=%d  final iters=%i  final time=%0.3fs",  \
        (int)_test_asi_retry,(int)_test_asi_iters[_test_asi_bank],\
        (double)gasnett_ticks_to_ns(_int)/1.0e9);                 \
    if (_int > 3*_test_asi_interval) { /* too slow, reset next */ \
      _test_asi_sz[_test_asi_bank] = 0;                           \
    }                                                             \
  }                                                               \
} while (0)

/* ------------------------------------------------------------------------------------ */
/* memory management */

static void *_test_malloc(size_t sz, const char *curloc) {
  void *ptr;
  ptr = malloc(sz);
  if (ptr == NULL) FATALERR("Failed to malloc(%" PRIuPTR ") bytes at %s\n",(uintptr_t)sz,curloc);
  return ptr;
}
static void *_test_calloc(size_t sz, const char *curloc) {
  void *retval = _test_malloc(sz, curloc);
  if (retval) memset(retval, 0, sz);
  return retval;
}
#define test_malloc(sz) _test_malloc((sz), __FILE__ ":" _STRINGIFY(__LINE__))
#define test_calloc(N,S) _test_calloc((N*S), __FILE__ ":" _STRINGIFY(__LINE__))

#define test_free(p) free(p)

/* ------------------------------------------------------------------------------------ */
/* progress bar */

#ifndef TEST_PROGRESS_STEPS
#define TEST_PROGRESS_STEPS 10
#endif
/* node 0 caller should call with curriter=0..(numiters-1) */
#define TEST_PROGRESS_BAR(curriter, numiters) do {    \
    static int _breakwidth, _breakpt;                 \
    if_pf(curriter == 0) {                            \
      _breakwidth = MAX(1,((numiters)/TEST_PROGRESS_STEPS));   \
      _breakpt = _breakwidth;                         \
    }                                                 \
    if (((curriter)+1) == _breakpt) {                 \
      MSG0("%i%%",(int)(100.0*((curriter)+1)/(numiters))); \
      _breakpt += _breakwidth;                        \
    }                                                 \
  } while (0)

/* ------------------------------------------------------------------------------------ */
/* Functions for obtaining calibrated delays */

#ifdef TEST_DELAY
/* NOTE: If you #define TEST_DELAY, be certain Makefile.in shows you depend on delay.o */

extern void test_delay(int64_t n, int pollcnt);	 /* in delay.o */

/* smallest number of delay loops to try in calibration */
#ifndef TEST_DELAY_LOOP_MIN
  #define TEST_DELAY_LOOP_MIN        100
#endif
/* max number of calibration iterations to wait for convergance */
#ifndef TEST_DELAY_CALIBRATION_LIMIT
  #define TEST_DELAY_CALIBRATION_LIMIT 100
#endif

/* Compute the number of loops needed to get no less that the specified delay
 * when executing "test_delay(loops, pollcnt)" exactly 'iters' times.
 *
 * Returns the number of loops needed and overwrites the argument with the
 * actual achieved delay for 'iters' calls to "delay(*time_p)".
 * The 'time_p' is given in microseconds.
 */
static int64_t test_calibrate_delay(int iters, int pollcnt, int64_t *time_p) 
{
	int64_t begin, end, time;
	float target = *time_p;
	float ratio = 0.0;
	int i;
        int64_t loops = 0;
        int caliters = 0;

	do {
		if (loops == 0) {
			loops = TEST_DELAY_LOOP_MIN;	/* first pass */
		} else {
			int64_t tmp = loops * ratio;

			if (tmp > loops) {
				loops = tmp;
			} else {
				loops += 1;	/* ensure progress in the face of round-off */
			}
                        assert(loops < 1ll<<62);
		}

		begin = TIME();
		for (i = 0; i < iters; i++) { test_delay(loops, pollcnt); }
		end = TIME();
		time = end - begin;
                assert(time >= 0);
                if (time == 0) ratio = 2.0;/* handle systems with very high granularity clocks */
                else ratio = target / (float)time;
                caliters++;
                if (caliters > TEST_DELAY_CALIBRATION_LIMIT)
                  FATALERR("test_calibrate_delay(%i,%i,%i) failed to converge after %i iterations.\n",
                          iters, pollcnt, (int)*time_p, iters);
              #if 0
                printf("loops=%" PRIi64 "\n",loops); fflush(stdout);
                printf("ratio=%f target=%f time=%" PRIi64 "\n",ratio,target,time); fflush(stdout);
              #endif
	} while (ratio > 1.0);

	*time_p = time;
	return loops;
}
#endif

/* ------------------------------------------------------------------------------------ */
/* config strings */

#ifndef TEST_OMIT_CONFIGSTRINGS
#ifdef TEST_GASNETEX_H
  #define TEST_CONFIG_STRING GASNET_CONFIG_STRING
  #define TEST_TITANIUM_BACKEND "gasnet-" GASNET_CONDUIT_NAME_STR "-uni"
#else
  #ifndef GASNETI_TOOLS_CONDUIT
  #define GASNETI_TOOLS_CONDUIT SMP
  #endif
  #define TEST_TITANIUM_BACKEND "sequential"
  #define TEST_CONFIG_STRING \
    "RELEASE=x,SPEC=x,CONDUIT=" _STRINGIFY(GASNETI_TOOLS_CONDUIT) "(" _STRINGIFY(GASNETI_TOOLS_CONDUIT) "-x/REFERENCE-x),THREADMODEL=PAR,SEGMENT=FAST,PTR=x,align,nodebug,notrace,nostats"
#endif
/* mimic Berkeley UPC build config strings, to allow running GASNet tests using upcrun */
GASNETT_IDENT(GASNetT_IdentString_link_GASNetConfig, 
 "$GASNetConfig: (<link>) " TEST_CONFIG_STRING " $");
#if !TEST_PAR
  /* pthread support is compiled out - ensure upcrun never tries to use it */
  #if GASNET_PSHM || (defined(GASNETI_PSHM_ENABLED) && defined(TEST_GASNET_TOOLS_ONLY))
    #define TEST_SHMEM_CONFIG "pshm"
  #else
    #define TEST_SHMEM_CONFIG "none"
  #endif
#else 
  /* mimic pthreads, to ensure harness -pthreads=T -threads=N will run us 
     such a setup will only run the gasnet test on N/T nodes (as opposed to N as one might like)
     but the alternative is not to run at all. harness -nopthreads does not have this problem.
   */
  #if GASNET_PSHM || (defined(GASNETI_PSHM_ENABLED) && defined(TEST_GASNET_TOOLS_ONLY))
    #define TEST_SHMEM_CONFIG "pthreads/pshm"
  #else
    #define TEST_SHMEM_CONFIG "pthreads"
  #endif
  GASNETT_IDENT(GASNetT_IdentString_PthCnt, "$UPCRDefaultPthreadCount: 1 $");
#endif
GASNETT_IDENT(GASNetT_IdentString_link_UPCRConfig,
   "$UPCRConfig: (<link>) " TEST_CONFIG_STRING ",SHMEM=" TEST_SHMEM_CONFIG ",SHAREDPTRREP=packed,dynamicthreads $");
GASNETT_IDENT(GASNetT_IdentString_link_upcver, 
 "$UPCVersion: (<link>) *** GASNet test *** $");
GASNETT_IDENT(GASNetT_IdentString_link_compileline, 
 "$UPCCompileLine: (<link>) *** GASNet test *** $");
GASNETT_IDENT(GASNetT_IdentString_link_compiletime, 
 "$UPCCompileTime: (<link>) " __DATE__ " " __TIME__ " $");
GASNETT_IDENT(GASNetT_IdentString_HeapSz, 
 "$UPCRDefaultHeapSizes: UPC_SHARED_HEAP_OFFSET=0 UPC_SHARED_HEAP_SIZE=0 $");
#if PLATFORM_ARCH_32
  GASNETT_IDENT(GASNetT_IdentString_PtrSz, "$UPCRSizeof: void_ptr=( $");
#else
  GASNETT_IDENT(GASNetT_IdentString_PtrSz, "$UPCRSizeof: void_ptr=, $");
#endif
GASNETT_IDENT(GASNetT_IdentString_ABI, "$UPCRBinaryInterface: " GASNETT_SYSTEM_TUPLE " $");
/* Ditto for Titanium tcrun */
GASNETT_IDENT(GASNetT_TiBackend_IdentString, 
 "$TitaniumBackend: " TEST_TITANIUM_BACKEND " $");
GASNETT_IDENT(GASNetT_TiCompiler_IdentString, 
 "$TitaniumCompilerFlags: *** GASNet test *** -g $");
#endif

#if TEST_PAR
/* create numthreads pthreads to call start_routine. 
   if threadarg_arr is NULL, then a unique 0-based integer threadid is passed as arg to start_routine
   else threadarg_arr is an array of numthreads opaque datastructures of size threadarg_elemsz bytes each,
     and each thread recieves a pointer to a unique element of this array as the arg to start_routine
   then join the threads and add any non-zero results to test_errs
   TEST_USE_PRIMORDIAL_THREAD can be defined to spawn only numthreads-1 new pthreads and run the last on this thread
 */
#ifndef TEST_USE_PRIMORDIAL_THREAD
  #if PLATFORM_OS_CNL
    /* some do default thread pinning that can mess with our results */
    #define TEST_USE_PRIMORDIAL_THREAD 1
  #else
    #define TEST_USE_PRIMORDIAL_THREAD 0
  #endif
#endif
#ifndef TEST_MAXTHREADS
  /* TEST_MAXTHREADS is a compile-time constant */
  #if GASNETT_MAX_THREADS > 1  /* may be missing for tools-only */
    #define TEST_MAXTHREADS_SYSTEM GASNETT_MAX_THREADS
  #else
    #define TEST_MAXTHREADS_SYSTEM 256
  #endif
  #define TEST_MAXTHREADS (TEST_MAXTHREADS_SYSTEM + TEST_USE_PRIMORDIAL_THREAD - 1)
#endif
/* Runtime enforcement of TEST_MAXTHREADS, GASNET_TEST_THREAD_LIMIT, or platform-specific limits */
static int test_thread_limit(int numthreads) {
    int limit = gasnett_getenv_int_withdefault("GASNET_TEST_THREAD_LIMIT", TEST_MAXTHREADS, 0);
    limit = MIN(limit, TEST_MAXTHREADS); /* Ignore attempt to raise above TEST_MAXTHREADS */
    return MIN(numthreads, limit);
}
#if HAVE_PTHREAD_SETCONCURRENCY && __cplusplus
  // ensure we have a declaration for the configure-detected function
  #undef pthread_setconcurrency
  #ifndef __THROW
  #define __THROW
  #endif
  extern "C" int pthread_setconcurrency(int) __THROW;
#endif
static void test_createandjoin_pthreads(int numthreads, void *(*start_routine)(void *), 
                                      void *threadarg_arr, size_t threadarg_elemsz) {
    int i;
    int jointhreads = 0;
    uint8_t *threadarg_pos = (uint8_t *)threadarg_arr;
    pthread_t *threadid = (pthread_t *)test_malloc(sizeof(pthread_t)*numthreads);
    #if HAVE_PTHREAD_SETCONCURRENCY
      pthread_setconcurrency(numthreads);
    #endif

    for(i=0;i<numthreads;i++) {
      void *threadarg = (void *)(uintptr_t)i;
      if (threadarg_arr) { 
        threadarg = threadarg_pos; 
        threadarg_pos += threadarg_elemsz; 
      }
    #if TEST_USE_PRIMORDIAL_THREAD
      if (i == numthreads-1) {
        test_errs += (intptr_t)start_routine(threadarg); /* execute the last thread on primordial thread */
      } else
    #endif
      { pthread_attr_t attr;   
        pthread_attr_init(&attr);   
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); /* ignore failures */
        check_zeroret(pthread_create(&threadid[i], &attr, start_routine, threadarg));
        check_zeroret(pthread_attr_destroy(&attr));
        jointhreads++;
      }
    }

    for(i=0;i<jointhreads;i++) {
      void *retval = NULL;
      check_zeroret(pthread_join(threadid[i], &retval));
      test_errs += (intptr_t)retval;
    }
    test_free(threadid);
  }
#endif

/* ------------------------------------------------------------------------------------ *
 * ------------------------------------------------------------------------------------ *
 *                        begin gasnetex.h specific stuff                                 *
 * ------------------------------------------------------------------------------------ *
 * ------------------------------------------------------------------------------------ */
#ifdef TEST_GASNETEX_H

/* ------------------------------------------------------------------------------------ */
/* misc GASNet utilities */

#define GASNET_Safe(fncall) do {                                     \
    int _retval;                                                     \
    if ((_retval = fncall) != GASNET_OK) {                           \
      fprintf(stderr, "ERROR calling: %s\n"                          \
                   " at: %s:%i\n"                                    \
                   " error: %s (%s)\n",                              \
              #fncall, __FILE__, __LINE__,                           \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval)); \
      fflush(stderr);                                                \
      gasnet_exit(_retval);                                          \
    }                                                                \
  } while(0)

static gex_Rank_t test_msgsource(gex_Token_t token) {
    gex_Token_Info_t info;
    gex_TI_t rc = gex_Token_Info(token, &info, GEX_TI_SRCRANK);
    assert(rc & GEX_TI_SRCRANK);
    return info.gex_srcrank;
}

/* ------------------------------------------------------------------------------------ */
/* barriers */

static gex_TM_t _test_tm0;
// TODO-EX: need tm0 earlier to get rid of the branch here
#define BARRIER() do {                                        \
 if (_test_tm0 == GEX_TM_INVALID) {                           \
   GASNET_Safe(gasnet_barrier(0,GASNET_BARRIERFLAG_UNNAMED)); \
 } else {                                                     \
   gex_Event_Wait(gex_Coll_BarrierNB(_test_tm0,0));           \
 }                                                            \
} while (0)

#if defined(GASNET_PAR) || defined(GASNET_PARSYNC)
  /* Cheap (but functional!) pthread + gasnet barrier */
  #if PLATFORM_ARCH_CRAYX1 || PLATFORM_OS_CYGWIN
    /* pthread_cond is unreliable on some versions of these OS's - use semaphores */
    #include <semaphore.h>
    static void test_pthread_barrier(unsigned int local_pthread_count, int doGASNetbarrier) {
      static gasnett_mutex_t barrier_mutex = GASNETT_MUTEX_INITIALIZER;
      static volatile int phase = 0;
      static volatile unsigned int barrier_count = 0;
      static sem_t sem[2];
      gasnett_mutex_lock(&barrier_mutex);
      { int myphase = phase;
        static volatile int firsttime = 1;
        if (firsttime) {
          check_zeroret(sem_init(&sem[0], 0, 0));
          check_zeroret(sem_init(&sem[1], 0, 0));
          firsttime = 0;
        }
        barrier_count++;
        if (barrier_count < local_pthread_count) { 
          gasnett_mutex_unlock(&barrier_mutex);
          check_zeroret(sem_wait(&sem[myphase]));
        } else {
          int i;
          if (doGASNetbarrier) BARRIER();
          barrier_count = 0;
          phase = !phase;
          gasnett_mutex_unlock(&barrier_mutex);
          for (i=0; i < (int)(local_pthread_count-1); i++) {
            check_zeroret(sem_post(&sem[myphase]));
          }
        }
      }
    }
  #else
    static void test_pthread_barrier(unsigned int local_pthread_count, int doGASNetbarrier) {
      /* cond variables must be phased on some OS's (HPUX) */
      static struct {
        gasnett_cond_t cond;
        gasnett_mutex_t mutex;
      } barrier[2] = { { GASNETT_COND_INITIALIZER, GASNETT_MUTEX_INITIALIZER }, 
                       { GASNETT_COND_INITIALIZER, GASNETT_MUTEX_INITIALIZER }};
      static volatile unsigned int barrier_count = 0;
      static volatile int phase = 0;
      const int myphase = phase;
      gasnett_mutex_lock(&(barrier[myphase].mutex));
      barrier_count++;
      if (barrier_count < local_pthread_count) {
	/* CAUTION: changing the "do-while" to a "while" triggers a bug in the SunStudio 2006-08
         * compiler for x86_64.  See https://gasnet-bugs.lbl.gov/bugzilla/show_bug.cgi?id=1858
         * which includes a link to Sun's own database entry for this issue.
         */
        do {
          gasnett_cond_wait(&(barrier[myphase].cond), &(barrier[myphase].mutex));
        } while (myphase == phase);
      } else {  
        /* Now do the gasnet barrier */
        if (doGASNetbarrier) BARRIER();
        barrier_count = 0;
        phase = !phase;
        gasnett_cond_broadcast(&(barrier[myphase].cond));
      }       
      gasnett_mutex_unlock(&(barrier[myphase].mutex));
    }
  #endif
  #define PTHREAD_BARRIER(local_pthread_count)      \
    test_pthread_barrier(local_pthread_count, 1)
  #define PTHREAD_LOCALBARRIER(local_pthread_count) \
    test_pthread_barrier(local_pthread_count, 0)
#else
  #define PTHREAD_BARRIER(local_pthread_count) do { \
    PTHREAD_LOCALBARRIER(local_pthread_count);      \
    BARRIER();                                      \
  } while (0)
  #define PTHREAD_LOCALBARRIER(local_pthread_count) do {          \
    if (local_pthread_count != 1)                                 \
      FATALERR("cannot call PTHREAD_BARRIER in GASNET_SEQ mode"); \
  } while (0)
#endif

/* cheap and simple broadcast operation */
#define TEST_BCAST(dst, rootid, src, sz) do {                         \
  assert_always(_test_tm0 != GEX_TM_INVALID);                         \
  gex_Event_Wait(gex_Coll_BroadcastNB(_test_tm0, (rootid), (dst), (src), (sz), 0)); \
} while (0)

/* ------------------------------------------------------------------------------------ */
/* standard messages */
static void TEST_DEBUGPERFORMANCE_WARNING(void) {
  if (TEST_MYPROC == 0) {
    const char *warning = gasnett_performance_warning_str();
    if (*warning) {
      fflush(NULL);
      fprintf(stdout,
        "-----------------------------------------------------------------------\n"
        " WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n"
        "\n"
        " GASNet was configured and built with these optional features enabled:\n"
	"%s"
        " This usually has a SERIOUS impact on performance, so you should NOT\n"
        " trust any performance numbers reported in this run!!!\n"
        " You should configure and build from scratch without the configure\n"
        " flags that enable the optional additional checking/reporting.\n"
        "\n"
        " WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n"
        "-----------------------------------------------------------------------\n",
        warning);
      fflush(NULL);
    }
  }
}

#define TEST_PRINT_CONDUITINFO() do {                              \
  MSG0("%s conduit: v%s GASNET_ALIGNED_SEGMENTS=%i\n"              \
       " gex_AM_MaxArgs():             %i\n"                     \
       " gex_AM_LUBRequestMedium(): %i\n"                     \
       " gex_AM_LUBReplyMedium():   %i\n"                     \
       " gex_AM_LUBRequestLong():   %i\n"                     \
       " gex_AM_LUBReplyLong():     %i"                       \
    ,                                                              \
    _STRINGIFY(GASNET_CORE_NAME), _STRINGIFY(GASNET_CORE_VERSION), \
    GASNET_ALIGNED_SEGMENTS,                                       \
    (int)gex_AM_MaxArgs(),                                       \
    (int)gex_AM_LUBRequestMedium(),                           \
    (int)gex_AM_LUBReplyMedium(),                             \
    (int)gex_AM_LUBRequestLong(),                             \
    (int)gex_AM_LUBReplyLong());                              \
  } while (0)

#if defined(GASNET_SEQ)
  #define TEST_PARSEQ SEQ
#elif defined(GASNET_PAR)
  #define TEST_PARSEQ PAR
#elif defined(GASNET_PARSYNC)
  #define TEST_PARSEQ PARSYNC
#endif

/* ------------------------------------------------------------------------------------ */
/* segment management */
#ifndef TEST_SEGSZ_PADFACTOR
  #define TEST_SEGSZ_PADFACTOR 1
#endif
#define TEST_SEGSZ_REQUEST (TEST_SEGSZ_PADFACTOR*TEST_SEGSZ)

#if defined(GASNET_PAR) || defined(GASNET_PARSYNC)
  #ifndef TEST_MAXTHREADS
    #define TEST_MAXTHREADS      GASNETT_MAX_THREADS
  #endif
  #ifndef TEST_SEGZ_PER_THREAD  // provides a default per-thread segsize when TEST_SEGSZ not defined
    #define TEST_SEGZ_PER_THREAD ((uintptr_t)64*1024)
  #endif
  #ifndef TEST_SEGSZ // TEST_SEGSZ provides a statically-known override value
    #ifdef TEST_SEGSZ_EXPR // TEST_SEGSZ_EXPR provides a value not statically known
      #define TEST_SEGSZ  alignup(TEST_SEGSZ_EXPR,PAGESZ)
    #else
      #define TEST_SEGSZ  alignup(TEST_MAXTHREADS*TEST_SEGZ_PER_THREAD,PAGESZ)
      test_static_assert_file(TEST_SEGSZ >= (TEST_MAXTHREADS*TEST_SEGZ_PER_THREAD));
    #endif
  #endif
#else
  #ifndef TEST_SEGSZ
    #ifdef TEST_SEGSZ_EXPR
      #define TEST_SEGSZ  alignup(TEST_SEGSZ_EXPR,PAGESZ)
    #else
      #define TEST_SEGSZ  alignup(64*1024,PAGESZ)
    #endif
  #endif
#endif
#ifndef TEST_SEGSZ_EXPR
  // validate TEST_SEGSZ properties, when the value is statically-known
  test_static_assert_file(TEST_SEGSZ > 0);
  test_static_assert_file(TEST_SEGSZ % PAGESZ == 0);
  test_static_assert_file(TEST_SEGSZ_REQUEST % PAGESZ == 0);
#endif

#define TEST_MINHEAPOFFSET  alignup(128*4096,PAGESZ)
#if (TEST_MINHEAPOFFSET % PAGESZ) != 0 
  #error Bad TEST_MINHEAPOFFSET
#endif

static size_t test_num_am_handlers = 0;
#ifdef GASNET_SEGMENT_EVERYTHING
  /* following trivially handles the case where static data is aligned
     across the nodes, and also works on X-1 where the static data is
     misaligned across nodes. 
     The only assumption is that AM mediums, barriers and atomics work properly
     We intercept the gasnet_attach or gex_Segment_Attach call and do the segment exchange there
   */
  static gasnet_seginfo_t *_test_seginfo;
  static int _test_seggather_idx;
  static gasnett_atomic_t _test_seggather_done = gasnett_atomic_init(0);
  static void _test_seggather(gex_Token_t token, void *buf, size_t nbytes) {
    assert(nbytes == sizeof(gasnet_seginfo_t));
    assert(_test_seginfo != NULL);
    gex_Rank_t srcid = test_msgsource(token);
    assert(srcid < TEST_PROCS);
    _test_seginfo[srcid] = *(gasnet_seginfo_t *)buf;
    gasnett_atomic_increment(&_test_seggather_done, GASNETT_ATOMIC_REL);
  }
  static int _test_segbcast_idx;
  static gasnett_atomic_t _test_segbcast_count = gasnett_atomic_init(0);
  static void _test_segbcast(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t idx) {
    void *dst = (void*)((uintptr_t)_test_seginfo + idx * gex_AM_LUBRequestMedium());
    memcpy(dst, buf, nbytes);
    gasnett_atomic_increment(&_test_segbcast_count, GASNETT_ATOMIC_REL);
  }
  static int _test_create_test_segment(gex_TM_t tm, uintptr_t segsize) {
    #ifdef TEST_SEGSZ_EXPR
      /* dynamically allocate segment */
      uint8_t *_test_hidden_seg;
    #else
      /* use a block of static data as the segment */
      static uint8_t _test_hidden_seg[TEST_SEGSZ+PAGESZ];
    #endif
    int i;
    gasnet_seginfo_t myseg;

    gex_AM_Entry_t mytab[] = {
      { 0, (gex_AM_Fn_t)_test_seggather, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0, NULL, NULL },
      { 0, (gex_AM_Fn_t)_test_segbcast,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 1, NULL, NULL }
    };
    size_t numentries = sizeof(mytab)/sizeof(gex_AM_Entry_t);
    test_num_am_handlers += numentries;
    GASNET_Safe(gex_EP_RegisterHandlers(gex_TM_QueryEP(tm), mytab, numentries));
    _test_seggather_idx = mytab[0].gex_index;
    _test_segbcast_idx = mytab[1].gex_index;

    gex_Rank_t myrank = gex_TM_QueryRank(tm);
    gex_Rank_t numrank = gex_TM_QuerySize(tm);

    _test_seginfo = (gasnet_seginfo_t *)test_calloc(numrank,sizeof(gasnet_seginfo_t));
    #ifdef TEST_SEGSZ_EXPR
      _test_hidden_seg = (uint8_t *)test_malloc(TEST_SEGSZ+PAGESZ);
    #endif
    myseg = _test_seginfo[myrank];
    myseg.addr = ((void *)(((uint8_t*)_test_hidden_seg) + 
      (((((uintptr_t)_test_hidden_seg)%PAGESZ) == 0)? 0 : 
       (PAGESZ-(((uintptr_t)_test_hidden_seg)%PAGESZ)))));
    myseg.size = TEST_SEGSZ;
    BARRIER();
    gex_AM_RequestMedium0(tm, 0, _test_seggather_idx, &myseg, sizeof(gasnet_seginfo_t), GEX_EVENT_NOW, 0);
    { const size_t total_bytes = numrank*sizeof(gasnet_seginfo_t);
      const size_t msg_bytes = gex_AM_LUBRequestMedium();
      const int msg_count = (total_bytes + msg_bytes - 1) / msg_bytes;
      if (myrank == 0) {
        size_t remain = total_bytes;
        void *payload = _test_seginfo;
        int idx;
        GASNET_BLOCKUNTIL((int)gasnett_atomic_read(&_test_seggather_done, 0) == (int)numrank);
        for (idx = 0; idx < msg_count; ++idx) {
          const size_t nbytes = MIN(remain, msg_bytes);
          for (i=0; i < (int)numrank; i++) {
            gex_AM_RequestMedium1(tm, i, _test_segbcast_idx, payload, nbytes, GEX_EVENT_NOW, 0, idx);
          }
          remain -= nbytes;
          payload = (void*)((uintptr_t)payload + nbytes);
        }
      }
      GASNET_BLOCKUNTIL((int)gasnett_atomic_read(&_test_segbcast_count, 0) == msg_count);
    }
    BARRIER();
    for (i=0; i < (int)numrank; i++) {
      assert_always(_test_seginfo[i].size >= TEST_SEGSZ);
      assert_always((((uintptr_t)_test_seginfo[i].addr) % PAGESZ) == 0);
    }
    return GASNET_OK;
  }
 #ifdef _INCLUDED_GASNET_H
  static int _test_attach(gasnet_handlerentry_t *table, int numentries, uintptr_t segsize, uintptr_t minheapoffset)
  {
    /* do regular attach, then setup seg_everything segment */
    GASNET_Safe(gasnet_attach(table, numentries, segsize, minheapoffset));
    gasnet_QueryGexObjects(NULL,NULL,&_test_tm0,NULL);
    return _test_create_test_segment(_test_tm0, segsize);
  }
  #undef gasnet_attach
  #define gasnet_attach _test_attach
 #endif
  static int _test_Segment_Attach(
                gex_Segment_t     *segment_p,
                gex_TM_t          tm,
                uintptr_t         length)
  {
    _test_tm0 = tm;
    return _test_create_test_segment(tm, length);
  }
  #undef gex_Segment_Attach
  #define gex_Segment_Attach _test_Segment_Attach
  #define TEST_SEG(node) (assert(_test_seginfo), _test_seginfo[node].addr)
#else
 /* Segment FAST or LARGE
  * Wrap gasnet_attach() or gex_Segment_Attach() to validate
  * the allocated segment size, alignment, etc.
  */
 #ifdef _INCLUDED_GASNET_H
  static int _test_attach(gasnet_handlerentry_t *table, int numentries, uintptr_t segsize, uintptr_t minheapoffset)
  {
       GASNET_Safe(gasnet_attach(table, numentries, segsize, minheapoffset));
       gex_Rank_t i;
       gasnet_seginfo_t *s = (gasnet_seginfo_t *)test_malloc(TEST_PROCS*sizeof(gasnet_seginfo_t));
       GASNET_Safe(gasnet_getSegmentInfo(s, TEST_PROCS));
       for (i=0; i < TEST_PROCS; i++) {
         assert_always(s[i].size >= TEST_SEGSZ);
         assert_always(((uintptr_t)s[i].size) % PAGESZ == 0);
         #if GASNET_ALIGNED_SEGMENTS == 1
           assert_always(s[i].addr == s[0].addr);
         #endif
       }
       test_free(s);
       gasnet_QueryGexObjects(NULL,NULL,&_test_tm0,NULL);
       return GASNET_OK;
  }
  #undef gasnet_attach
  #define gasnet_attach _test_attach
 #endif
  static int _test_Segment_Attach(
                gex_Segment_t     *segment_p,
                gex_TM_t          tm,
                uintptr_t         length)
  {
      check_zeroret(gex_Segment_Attach(segment_p, tm, length));
      BARRIER();
      for (gex_Rank_t i=0; i < TEST_PROCS; i++) {
        void *_addr;  uintptr_t _size;
        check_zeroret(gex_Segment_QueryBound(tm, i, &_addr, NULL, &_size));
        assert_always(_size >= TEST_SEGSZ);
        assert_always(((uintptr_t)_size) % PAGESZ == 0);
      }
      _test_tm0 = tm;
      return GASNET_OK;
  }
  #undef gex_Segment_Attach
  #define gex_Segment_Attach _test_Segment_Attach

  static void* _test_seg(gex_Rank_t rank) {
    void *addr;
    check_zeroret(gex_Segment_QueryBound(_test_tm0, rank, &addr, NULL, NULL));
    return addr;
  }
  #define TEST_SEG(rank)        _test_seg(rank)
#endif

#define TEST_MYSEG()          (TEST_SEG(TEST_MYPROC))

// helper for SEGMENT_EVERYTHING
static void *TEST_SEG_TM(gex_TM_t tm, gex_Rank_t rank) {
#if GASNET_SEGMENT_EVERYTHING
  return TEST_SEG(gex_TM_TranslateRankToJobrank(tm, rank));
#else
  void *result;
  check_zeroret(gex_Segment_QueryBound(tm, rank, &result, NULL, NULL));
  return result;
#endif
}

/* ------------------------------------------------------------------------------------ */
/* segment alignment */
#if defined(GASNET_SEGMENT_EVERYTHING) || !GASNET_ALIGNED_SEGMENTS || !defined(_INCLUDED_GASNET_H)
  static int TEST_ALIGNED_SEGMENTS(void) {
    static volatile int is_aligned = -1;
    if_pf (is_aligned < 0) {
      int result = 1; /* Assume aligned until we find otherwise */
      void *addr0 = TEST_SEG(0);
      gex_Rank_t i;
      for (i = 1; i < TEST_PROCS; i++) {
        if (TEST_SEG(i) != addr0) {
          result = 0;
          break;
        }
      }
      is_aligned = result;
    }
    return is_aligned;
  }
#else
  #define TEST_ALIGNED_SEGMENTS() 1
#endif

/* ------------------------------------------------------------------------------------ */
/* local process and thread count management */
static gex_Rank_t _test_firstnode;
static int _test_localprocs(void) { /* First call is not thread safe */
  static gex_Rank_t count = 0;
  if (!count) {
    gex_System_QueryHostInfo(NULL, &count, NULL);
  }
  assert(count > 0);
  return count;
}
#define TEST_LOCALPROCS() (_test_localprocs())

static int _test_in_polite_mode = 0;
static void test_yield_if_polite(void) {
  if (_test_in_polite_mode) gasnett_sched_yield();
}

static void _test_set_waitmode(int threads, int reserve_cores) {
  const int local_procs = TEST_LOCALPROCS();
  if (gasnett_getenv_yesno_withdefault("GASNET_TEST_POLITE_SYNC",0)) return; // already set
#if TEST_PAR
  if (threads > 1) {
    int threads_serialized = 0;
  #if PLATFORM_OS_OPENBSD
    /* check for userland "uthreads" running on a single core */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    threads_serialized = (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM));
  #endif
    if (threads_serialized) {
      if (_test_firstnode == TEST_MYPROC)
        MSG("WARNING: per-proc thread count (%i) exceeds platform's concurrency "
            "- enabling  \"polite\", low-performance synchronization algorithms",
            threads);
      gasnet_set_waitmode(GASNET_WAIT_BLOCK);
      _test_in_polite_mode = 1;
      return;
    }
  }
#endif
  threads *= local_procs;
  if (threads + reserve_cores > gasnett_cpu_count()) {
    if (_test_firstnode == TEST_MYPROC) {
      char reserve[80];
      if (reserve_cores) sprintf(reserve, " + %i reserved cores", reserve_cores);
      else reserve[0] = 0;
      MSG("WARNING: per-node thread count (%i%s) exceeds actual cpu count (%i) "
          "- enabling  \"polite\", low-performance synchronization algorithms",
          threads, reserve, gasnett_cpu_count());
    }
    gasnet_set_waitmode(GASNET_WAIT_BLOCK);
    _test_in_polite_mode = 1;
  }
}
// Compute whether `th` (single-valued) threads per process for the current host
// would lead to overcommit, and set waitmode appropriately.
// The second variant reserves additional cores to leave idle, eg for system daemons
#define TEST_SET_WAITMODE(th) _test_set_waitmode(th,0)
#define TEST_SET_WAITMODE_RESERVE(th,reserve) _test_set_waitmode(th,reserve)

#endif /* TEST_GASNETEX_H */
/* ------------------------------------------------------------------------------------ */
/* test initialization boilerplate */
#if PLATFORM_ARCH_ALPHA || PLATFORM_ARCH_CRAYT3E
  #define TEST_SIG_INIT() gasnett_reghandler(SIGFPE, SIG_IGN)
#else
  #define TEST_SIG_INIT()
#endif

static void TEST_GENERICS_WARNING(void) {
  #ifdef TEST_GASNETEX_H
    if (TEST_MYPROC == 0)
  #endif
  {
    #ifdef GASNETT_USING_GETTIMEOFDAY
      fflush(NULL);
      fprintf(stdout, 
        "WARNING: using gettimeofday() for timing measurement - all short-term time measurements\n"             
        "WARNING: will be very rough and include significant timer overheads\n");
      fflush(NULL);
    #endif
    #ifdef GASNETT_USING_GENERIC_ATOMICOPS
      fflush(NULL);
      fprintf(stdout, 
        "WARNING: using generic mutex-based GASNet atomics, which are likely to have high overhead\n"             
        "WARNING: consider implementing true GASNet atomics, if supported by your platform/compiler\n");
      fflush(NULL);
    #endif
  }
}

static const char *_test_usagestr = NULL;
static const char *_test_testname = NULL;
static const char *_test_argvzero = NULL;
static void _test_usage(int early) {
  #ifdef TEST_GASNETEX_H
    if (TEST_MYPROC == 0) {
      fprintf(stderr, "%s %s\n", _test_testname, GASNET_CONFIG_STRING);
      fprintf(stderr, "Usage: %s %s%s", _test_argvzero, _test_usagestr,
        (_test_usagestr[strlen(_test_usagestr)-1] == '\n' ? "" : "\n"));
      fflush(NULL);
      sleep(1);
      gasnet_exit(1);
    } else { /* wait to die */
      if (early) {
        // we are waiting for node 0 to kill the job, but cannot safely AMpoll before attach
        // wait for a bounded time, and then force an exit to prevent zombies on polling-only conduits
        gasnett_tick_t starttime = gasnett_ticks_now();
        sleep(1);
        while (gasnett_ticks_to_us(gasnett_ticks_now()-starttime)<5000000) gasnett_sched_yield();
        gasnet_exit(-1);
      }
      else BARRIER();
    }
  #else
      fprintf(stderr, "%s %s\n", _test_testname, GASNETT_CONFIG_STRING);
      fprintf(stderr, "Usage: %s %s%s", _test_argvzero, _test_usagestr,
        (_test_usagestr[strlen(_test_usagestr)-1] == '\n' ? "" : "\n"));
      fflush(NULL);
      sleep(1);
      exit(1);
  #endif
}
#define test_usage() _test_usage(0)
#define test_usage_early() _test_usage(1)
static void _test_init(const char *testname, int reports_performance, int early,
                       int argc, const char * const *argv, const char *usagestr) {
  /* convenient place to put inits we want in all tests */
  int i;
  _test_usagestr = strdup(usagestr);
  _test_testname = strdup(testname);
  _test_argvzero = strdup(argv[0]);
  for (i = 0; i < argc; i++) { /* check for standard help option */
    if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"--h") || 
        !strcmp(argv[i],"-help") || !strcmp(argv[i],"--help")) _test_usage(early);
  }

  TEST_SIG_INIT();
  #ifdef TEST_GASNETEX_H
    if (!early) BARRIER();
    if (reports_performance) {
      TEST_DEBUGPERFORMANCE_WARNING();
      TEST_GENERICS_WARNING();
      if (TEST_MYPROC == 0)
        fprintf(stdout, "Timer granularity: <= %.3f us, overhead: ~ %.3f us\n",
                       gasnett_tick_granularityus(), gasnett_tick_overheadus());
      fflush(NULL);
    }
    if (gasnett_getenv_yesno_withdefault("GASNET_TEST_POLITE_SYNC",0)) {
      MSG0("WARNING: GASNET_TEST_POLITE_SYNC is set - enabling  \"polite\", low-performance synchronization algorithms");
      gasnet_set_waitmode(GASNET_WAIT_BLOCK);
      _test_in_polite_mode = 1;
    }
    MSG0("=====> %s nprocs=%d config=%s compiler=%s/%s sys=%s",
        testname, (int)TEST_PROCS, GASNET_CONFIG_STRING,
        _STRINGIFY(PLATFORM_COMPILER_FAMILYNAME), PLATFORM_COMPILER_VERSION_STR,
        GASNETT_SYSTEM_TUPLE);
    fflush(NULL);
    /* must use malloc here, pre-attach if "early" */
    gex_Rank_t nbrhd_rank;
    gex_System_QueryMyPosition(NULL,&nbrhd_rank,NULL,NULL);
    if (!early) {
      BARRIER();
    } else gasnett_nsleep(250000);
    MSG("hostname is: %s (supernode=%i pid=%i)", gasnett_gethostname(), (int)nbrhd_rank, (int)getpid());
    fflush(NULL);
    if (!early) BARRIER();
  #else
    MSG0("=====> %s config=%s compiler=%s/%s sys=%s",
          testname, GASNETT_CONFIG_STRING,
          _STRINGIFY(PLATFORM_COMPILER_FAMILYNAME), PLATFORM_COMPILER_VERSION_STR,
          GASNETT_SYSTEM_TUPLE);
    MSG("hostname is: %s (pid=%i)", gasnett_gethostname(), (int)getpid());
    fflush(NULL);
  #endif
  if (gasnett_verboseenv()) MSG("%s running...", testname);
}
#define test_init(testname, reports_performance, usagestr) \
       _test_init(testname, reports_performance, 0, argc, (const char * const *)argv, usagestr)
#define test_init_early(testname, reports_performance, usagestr) \
       _test_init(testname, reports_performance, 1, argc, (const char * const *)argv, usagestr)

#define TEST_BACKTRACE_DECLS()                              \
  static int test_my_backtrace = 0;                         \
  static volatile int test_my_backtrace_ran = 0;            \
  GASNETT_EXTERNC int test_my_backtrace_fn(int fd) {        \
    if (test_my_backtrace_ran != -1) {                      \
      /* Indicate FAILURE if we were not testing */         \
      /* So the next available mechanism will run. */       \
      return 1;                                             \
    }                                                       \
    test_my_backtrace_ran = 1;                              \
    return 0;                                               \
  }                                                         \
  gasnett_backtrace_type_t gasnett_backtrace_user = {       \
    "USER", &test_my_backtrace_fn, 1                        \
  }
#define TEST_BACKTRACE_INIT(_exename)                       \
  /* Only test our backtrace handler if the user is not trying to backtrace */ \
  /* Bug 3644: cannot reliably override GASNET_BACKTRACE_TYPE if already set */ \
  if (!gasnett_getenv("GASNET_BACKTRACE") &&                \
      !gasnett_getenv("GASNET_BACKTRACE_TYPE")) {           \
    test_my_backtrace = 1;                                  \
    gasnett_setenv("GASNET_BACKTRACE_TYPE","USER");         \
  }                                                         \
  gasnett_backtrace_init(_exename)
#define TEST_BACKTRACE() do {                               \
    if (test_my_backtrace) {                                \
      test_my_backtrace_ran = -1;                           \
      gasnett_print_backtrace(STDOUT_FILENO);               \
      if (test_my_backtrace_ran != 1) {                     \
        ERR("failed to run user-supplied backtrace code\n");\
      }                                                     \
    }                                                       \
  } while(0)
  

#define TEST_TRACING_MACROS() do {                                                 \
  const char *file; test_mark_used(file);                                          \
  unsigned int line; test_mark_used(line);                                         \
  GASNETT_TRACE_GETSOURCELINE(&file, &line);                                       \
  GASNETT_TRACE_SETSOURCELINE(file, line);                                         \
  GASNETT_TRACE_FREEZESOURCELINE();                                                \
  GASNETT_TRACE_UNFREEZESOURCELINE();                                              \
  if (GASNETT_TRACE_ENABLED)                                                       \
    GASNETT_TRACE_PRINTF("TEST_TRACING_MACROS: GASNETT_TRACE_PRINTF()");           \
  GASNETT_TRACE_PRINTF_FORCE("TEST_TRACING_MACROS: GASNETT_TRACE_PRINTF_FORCE()"); \
  GASNETT_TRACE_SETMASK(GASNETT_TRACE_GETMASK());                                  \
  GASNETT_STATS_SETMASK(GASNETT_STATS_GETMASK());                                  \
  GASNETT_TRACE_SET_TRACELOCAL(GASNETT_TRACE_GET_TRACELOCAL());                    \
} while (0)

GASNETT_END_EXTERNC

#endif
