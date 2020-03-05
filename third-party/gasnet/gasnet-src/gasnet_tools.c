/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_tools.c $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if defined(GASNETT_THREAD_SAFE) || defined(GASNETT_THREAD_SINGLE)
  /* nothing */
#elif defined(GASNET_PARSYNC) || defined(GASNET_PAR)
  #define GASNETT_THREAD_SAFE 1
#elif defined(GASNET_SEQ)
  #define GASNETT_THREAD_SINGLE 1
#else
  #error Missing threading definition
#endif

// this file is built for exactly one of libgasnet/tools or libgasnet/conduit
#if (GASNETI_BUILDING_TOOLS && GASNETI_BUILDING_CONDUIT) || (!GASNETI_BUILDING_TOOLS && !GASNETI_BUILDING_CONDUIT)
#error Invalid GASNETI_BUILDING_ macro
#endif

#undef GASNET_SEQ
#undef GASNET_PAR
#undef GASNET_PARSYNC

#include <gasnet_tools.h>

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> /* gasneti_system_redirected_coprocess */
#include <sys/select.h>
#include <fcntl.h>
#include <time.h> /* gasneti_gettimeofday_us, gasneti_nsleep */
#include <sys/time.h> /* gasneti_gettimeofday_us, gasneti_nsleep */
#include <signal.h>

#if HAVE_EXECINFO_H
  #include <execinfo.h>
#endif
#ifdef HAVE_UCONTEXT_H
  #include <ucontext.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#if PLATFORM_OS_CYGWIN
#include <cygwin/version.h>
#endif

#if HAVE_PR_SET_PTRACER
  #include <sys/prctl.h>
  #ifndef PR_SET_PTRACER
    #define PR_SET_PTRACER 0x59616d61 /* 'Yama' */
  #endif
  #ifndef PR_SET_PTRACER_ANY
    #define PR_SET_PTRACER_ANY ((unsigned long)(-1))
  #endif
#endif

#if PLATFORM_OS_DARWIN
#include <sys/utsname.h> // uname()
#endif

#if PLATFORM_COMPILER_SUN_C
  /* disable warnings triggerred by some macro idioms we use */
  #pragma error_messages(off, E_END_OF_LOOP_CODE_NOT_REACHED)
  #pragma error_messages(off, E_STATEMENT_NOT_REACHED)
#endif

/* ------------------------------------------------------------------------------------ */
/* generic atomics support */
#if defined(GASNETI_BUILD_GENERIC_ATOMIC32) || defined(GASNETI_BUILD_GENERIC_ATOMIC64)
  #ifdef GASNETI_ATOMIC_LOCK_TBL_DEFNS
    #define _gasneti_atomic_lock_initializer	GASNETT_MUTEX_INITIALIZER
    #define _gasneti_atomic_lock_init(x)	gasnett_mutex_init(x)
    #define _gasneti_atomic_lock_lock(x)        gasnett_mutex_lock(x)
    #define _gasneti_atomic_lock_unlock(x)      gasnett_mutex_unlock(x)
    #define _gasneti_atomic_lock_malloc		malloc
    GASNETI_ATOMIC_LOCK_TBL_DEFNS(gasneti_pthread_atomic_, gasnett_mutex_)
    #undef _gasneti_atomic_lock_initializer
    #undef _gasneti_atomic_lock_init
    #undef _gasneti_atomic_lock_lock
    #undef _gasneti_atomic_lock_unlock
    #undef _gasneti_atomic_lock_malloc
   #endif
  #ifdef GASNETI_GENATOMIC32_DEFN
    GASNETI_GENATOMIC32_DEFN
  #endif
  #ifdef GASNETI_GENATOMIC64_DEFN
    GASNETI_GENATOMIC64_DEFN
  #endif
#endif

#if GASNETI_MUTEX_CAUTIOUS_INIT
#if GASNETI_ATOMIC32_NOT_SIGNALSAFE
  #error GASNETI_MUTEX_CAUTIOUS_INIT requires !GASNETI_ATOMIC32_NOT_SIGNALSAFE
#endif
/* initstep values: 0 = initial value for a new mutex
                    1 = initialization of mutex in progress by some thread
                    2 = mutex is fully initialized and ready for use
 */
extern void gasneti_mutex_cautious_init(/*gasneti_mutex_t*/void *_pl) {
  gasneti_mutex_t *pl = _pl;
  gasneti_atomic32_t *initstep = (gasneti_atomic32_t *)&(pl->initstep);
  while (1) {
    /* check if initialization is complete */
    if_pt (gasneti_atomic32_read(initstep,GASNETI_ATOMIC_RMB_POST) == 2) return;
    /* mutex needs initialization, try to acquire that job */
    if (gasneti_atomic32_compare_and_swap(initstep, 0, 1,
                                           GASNETI_ATOMIC_ACQ_IF_TRUE)) break;
    /* some other thread beat us to it */
    gasneti_sched_yield();
  }

  /* perform an uncontended lock/unlock cycle on the new mutex, 
     to ensure pthread library data structures are correctly created */
  gasneti_assert_zeroret(pthread_mutex_lock(&(pl->lock)));
  gasneti_assert_zeroret(pthread_mutex_unlock(&(pl->lock)));

  /* init complete, release */
  gasneti_atomic32_set(initstep, 2, GASNETI_ATOMIC_WMB_PRE);
  return;
}
#endif

#if GASNETI_THREADS && GASNETI_BUG3430_WORKAROUND
  gasneti_mutex_t gasneti_bug3430_lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_cond_t gasneti_bug3430_cond = GASNETI_COND_INITIALIZER;
  volatile int gasneti_bug3430_creating = 0;
#endif

/* ------------------------------------------------------------------------------------ */
/* rwlock support */

#if GASNET_DEBUG || GASNETI_BUILDING_TOOLS
  /* Use a thread-specific list of locks held, to avoid the need for extra synchronization.
   * If a thread exits with locks held we currently leak this list, although if it ever matters
   * this could be fixed using a destructor function in pthread_key_create.
   */
  GASNETI_THREADKEY_DEFINE(_gasneti_rwlock_list);

  typedef struct _S_gasnet_rwlocklist {
     gasneti_rwlock_t const *l;
     struct _S_gasnet_rwlocklist *next;
     _gasneti_rwlock_state state;
  } _gasneti_rwlocklist_t;

  extern _gasneti_rwlock_state _gasneti_rwlock_query(gasneti_rwlock_t const *l) {
    _gasneti_rwlocklist_t const *list = gasneti_threadkey_get(_gasneti_rwlock_list);
    gasneti_assert(l);
    while (list) {
      if (list->l == l) return list->state;
      list = list->next;
    }
    return _GASNETI_RWLOCK_UNLOCKED;
  }

  extern void _gasneti_rwlock_insert(gasneti_rwlock_t const *l, _gasneti_rwlock_state state) {
    _gasneti_rwlocklist_t *list = gasneti_threadkey_get(_gasneti_rwlock_list);
    _gasneti_rwlocklist_t *elem = malloc(sizeof(_gasneti_rwlocklist_t));
    gasneti_assert(l);
    gasneti_assert(state);
    elem->l = l;
    elem->state = state;
    elem->next = list;
    gasneti_threadkey_set(_gasneti_rwlock_list, elem);
  }

  extern void _gasneti_rwlock_remove(gasneti_rwlock_t const *l) {
    _gasneti_rwlocklist_t *list = gasneti_threadkey_get(_gasneti_rwlock_list);
    _gasneti_rwlocklist_t **p = &list;
    gasneti_assert(l);
    while (*p) {
      if ((*p)->l == l) {
        _gasneti_rwlocklist_t *elem = *p;
        *p = elem->next;
        free(elem);
        break;
      }
      p = &(*p)->next;
    }
    gasneti_threadkey_set(_gasneti_rwlock_list, list);
  }
#endif

/* ------------------------------------------------------------------------------------ */
/* call-based atomic support for C compilers with limited inline assembly */

#ifdef GASNETI_ATOMIC32_SPECIALS
  GASNETI_ATOMIC32_SPECIALS
#endif
#ifdef GASNETI_ATOMIC64_SPECIALS
  GASNETI_ATOMIC64_SPECIALS
#endif

/* ------------------------------------------------------------------------------------ */
/* call-based membar/atomic support for compilers which lack inline assembly of configured CC */
#if defined(GASNETI_USING_SLOW_ATOMICOPS) || \
    defined(GASNETI_USING_SLOW_ATOMIC32) || \
    defined(GASNETI_USING_SLOW_ATOMIC64) || \
    defined(GASNETI_USING_SLOW_MEMBARS)
#error gasnet_tools.c must be compiled with support for inline assembly
#endif

#ifdef GASNETI_TICKS_NOW_BODY
  GASNETI_SPECIAL_ASM_DEFN(gasneti_slow_ticks_now, GASNETI_TICKS_NOW_BODY)
#else
  extern gasneti_tick_t gasneti_slow_ticks_now(void) {
    return gasneti_ticks_now();
  }
#endif
#ifdef GASNETI_COMPILER_FENCE_BODY
  GASNETI_SPECIAL_ASM_DEFN(gasneti_slow_compiler_fence, GASNETI_COMPILER_FENCE_BODY)
#else
  extern void gasneti_slow_compiler_fence(void) {
    gasneti_compiler_fence();
  }
#endif
#ifdef GASNETI_LOCAL_WMB_BODY
  GASNETI_SPECIAL_ASM_DEFN(gasneti_slow_local_wmb, GASNETI_LOCAL_WMB_BODY)
#else
  extern void gasneti_slow_local_wmb(void) {
    gasneti_local_wmb();
  }
#endif
#ifdef GASNETI_LOCAL_RMB_BODY
  GASNETI_SPECIAL_ASM_DEFN(gasneti_slow_local_rmb, GASNETI_LOCAL_RMB_BODY)
#else
  extern void gasneti_slow_local_rmb(void) {
    gasneti_local_rmb();
  }
#endif
#ifdef GASNETI_LOCAL_MB_BODY
  GASNETI_SPECIAL_ASM_DEFN(gasneti_slow_local_mb, GASNETI_LOCAL_MB_BODY)
#else
  extern void gasneti_slow_local_mb(void) {
    gasneti_local_mb();
  }
#endif

/* Warn (once) if slow atomics are reached */
static int gasneti_slow_atomic_warning_issued = 0;
GASNETI_NEVER_INLINE(gasneti_slow_atomic_warn,
static void gasneti_slow_atomic_warn(void)) {
  gasneti_slow_atomic_warning_issued = 1;
  fprintf(stderr,
          "WARNING: using slow atomics due to use of a compiler not probed by GASNet at configure time\n");
  fflush(stderr);
}
#define GASNETI_SLOW_ATOMIC_WARNING() do { \
    if_pf (! gasneti_slow_atomic_warning_issued) gasneti_slow_atomic_warn(); \
  } while (0)

#ifdef GASNETI_USE_GENERIC_ATOMICOPS
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern gasneti_atomic_val_t gasneti_slow_atomic_read(gasneti_atomic_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic_read(p,flags);
  }
  extern void gasneti_slow_atomic_set(gasneti_atomic_t *p, gasneti_atomic_val_t v, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic_set(p, v, flags);
  }
  extern void gasneti_slow_atomic_increment(gasneti_atomic_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic_increment(p, flags);
  }
  extern void gasneti_slow_atomic_decrement(gasneti_atomic_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic_decrement(p, flags);
  }
  extern int gasneti_slow_atomic_decrement_and_test(gasneti_atomic_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic_decrement_and_test(p, flags);
  }
  #if defined(GASNETI_HAVE_ATOMIC_CAS)
    extern int gasneti_slow_atomic_compare_and_swap(gasneti_atomic_t *p, gasneti_atomic_val_t oldval, gasneti_atomic_val_t newval, const int flags) {
      GASNETI_SLOW_ATOMIC_WARNING();
      return gasneti_atomic_compare_and_swap(p,oldval,newval,flags);
    }
    extern gasneti_atomic_val_t gasneti_slow_atomic_swap(gasneti_atomic_t *p, gasneti_atomic_val_t val, const int flags) {
      GASNETI_SLOW_ATOMIC_WARNING();
      return gasneti_atomic_swap(p,val,flags);
    }
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    extern gasneti_atomic_val_t gasneti_slow_atomic_add(gasneti_atomic_t *p, gasneti_atomic_val_t op, const int flags) {
      GASNETI_SLOW_ATOMIC_WARNING();
      return gasneti_atomic_add(p,op,flags);
    }
    extern gasneti_atomic_val_t gasneti_slow_atomic_subtract(gasneti_atomic_t *p, gasneti_atomic_val_t op, const int flags) {
      GASNETI_SLOW_ATOMIC_WARNING();
      return gasneti_atomic_subtract(p,op,flags);
    }
  #endif
#endif
#ifdef GASNETI_USE_GENERIC_ATOMIC32
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern uint32_t gasneti_slow_atomic32_read(gasneti_atomic32_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_read(p,flags);
  }
  extern void gasneti_slow_atomic32_set(gasneti_atomic32_t *p, uint32_t v, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic32_set(p, v, flags);
  }
  extern void gasneti_slow_atomic32_increment(gasneti_atomic32_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic32_increment(p, flags);
  }
  extern void gasneti_slow_atomic32_decrement(gasneti_atomic32_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic32_decrement(p, flags);
  }
  extern int gasneti_slow_atomic32_decrement_and_test(gasneti_atomic32_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_decrement_and_test(p, flags);
  }
  extern int gasneti_slow_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_compare_and_swap(p,oldval,newval,flags);
  }
  extern uint32_t gasneti_slow_atomic32_swap(gasneti_atomic32_t *p, uint32_t val, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_swap(p,val,flags);
  }
  extern uint32_t gasneti_slow_atomic32_add(gasneti_atomic32_t *p, uint32_t op, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_add(p,op,flags);
  }
  extern uint32_t gasneti_slow_atomic32_subtract(gasneti_atomic32_t *p, uint32_t op, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic32_subtract(p,op,flags);
  }
#endif
#ifdef GASNETI_USE_GENERIC_ATOMIC64
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern uint64_t gasneti_slow_atomic64_read(gasneti_atomic64_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_read(p,flags);
  }
  extern void gasneti_slow_atomic64_set(gasneti_atomic64_t *p, uint64_t v, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic64_set(p, v, flags);
  }
  extern void gasneti_slow_atomic64_increment(gasneti_atomic64_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic64_increment(p, flags);
  }
  extern void gasneti_slow_atomic64_decrement(gasneti_atomic64_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    gasneti_atomic64_decrement(p, flags);
  }
  extern int gasneti_slow_atomic64_decrement_and_test(gasneti_atomic64_t *p, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_decrement_and_test(p, flags);
  }
  extern int gasneti_slow_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_compare_and_swap(p,oldval,newval,flags);
  }
  extern uint64_t gasneti_slow_atomic64_swap(gasneti_atomic64_t *p, uint64_t val, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_swap(p,val,flags);
  }
  extern uint64_t gasneti_slow_atomic64_add(gasneti_atomic64_t *p, uint64_t op, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_add(p,op,flags);
  }
  extern uint64_t gasneti_slow_atomic64_subtract(gasneti_atomic64_t *p, uint64_t op, const int flags) {
    GASNETI_SLOW_ATOMIC_WARNING();
    return gasneti_atomic64_subtract(p,op,flags);
  }
#endif

/* ------------------------------------------------------------------------------------ */
/* ident strings and idiot checks */
#define GASNETT_THREAD_MODEL_STR _STRINGIFY(GASNETT_THREAD_MODEL)
GASNETI_IDENT(gasnett_IdentString_ThreadModel, "$GASNetToolsThreadModel: " GASNETT_THREAD_MODEL_STR " $");

GASNETI_IDENT(gasnett_IdentString_Config, "$GASNetToolsConfig: " GASNETT_CONFIG_STRING " $");

GASNETI_IDENT(gasnett_IdentString_BuildTimestamp, 
             "$GASNetBuildTimestamp: " __DATE__ " " __TIME__ " $");

GASNETI_IDENT(gasnett_IdentString_BuildID, 
             "$GASNetBuildId: " GASNETI_BUILD_ID " $");
GASNETI_IDENT(gasnett_IdentString_ConfigureArgs, 
             "$GASNetConfigureArgs: " GASNETI_CONFIGURE_ARGS " $");
GASNETI_IDENT(gasnett_IdentString_SystemTuple, 
             "$GASNetSystemTuple: " GASNETI_SYSTEM_TUPLE " $");
GASNETI_IDENT(gasnett_IdentString_SystemName, 
             "$GASNetSystemName: " GASNETI_SYSTEM_NAME " $");
GASNETI_IDENT(gasnett_IdentString_CompilerID, 
             "$GASNetCompilerID: " PLATFORM_COMPILER_IDSTR " $");

GASNETI_IDENT(gasnett_IdentString_GitHash, 
             "$GASNetGitHash: gex-2019.9.2-46-gaf0aaf5 $");

int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR)) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR)) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH)) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(LITE) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_THREAD_MODEL) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_DEBUG_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_PTR_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_TIMER_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_MEMBAR_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG) = 1;
int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG) = 1;

static gasneti_atomic_t gasneti_backtrace_enabled = gasneti_atomic_init(1);

extern uint64_t gasnett_release_version(void) { // motivated by xSDK Community Policy M8
  return GASNET_RELEASE_VERSION_MAJOR * (uint64_t)1000000 +
         GASNET_RELEASE_VERSION_MINOR * (uint64_t)10000 +
         GASNET_RELEASE_VERSION_PATCH;
}

extern const char *gasnett_release_version_str(void) { // motivated by xSDK Community Policy M8
  return _STRINGIFY(GASNET_RELEASE_VERSION_MAJOR) "." 
         _STRINGIFY(GASNET_RELEASE_VERSION_MINOR) "." 
         _STRINGIFY(GASNET_RELEASE_VERSION_PATCH);
}

extern const char *gasnett_performance_warning_str(void) {
  static const char *result =
    #if defined(GASNET_DEBUG) || defined(GASNETI_STATS_OR_TRACE) || defined(GASNET_DEBUGMALLOC)
      "        "  /* Leading white space: */
      #ifdef GASNET_DEBUG
        "debugging  "
      #endif
      #ifdef GASNET_TRACE
        "tracing  "
      #endif
      #ifdef GASNET_STATS
        "statistical collection  "
      #endif
      #ifdef GASNET_DEBUGMALLOC
        "debugging malloc  "
      #endif
      "\n" /* Trailing white space: */
    #endif
    #if defined(GASNETI_FORCE_GENERIC_ATOMICOPS)
      "        FORCED mutex-based atomicops\n"
    #elif defined(GASNETI_FORCE_OS_ATOMICOPS)
      "        FORCED os-provided atomicops\n"
    #endif
    #if defined(GASNETI_FORCE_TRUE_WEAKATOMICS) && GASNETT_THREAD_SINGLE
      "        FORCED atomics in sequential code\n"
    #endif
    #if defined(GASNETI_FORCE_GENERIC_SEMAPHORES) && GASNETT_THREAD_SAFE
      "        FORCED mutex-based semaphores\n"
    #endif
    #if defined(GASNETI_FORCE_YIELD_MEMBARS)
      "        FORCED sched_yield() in memory barriers\n"
    #elif defined(GASNETI_FORCE_SLOW_MEMBARS)
      "        FORCED non-inlined memory barriers\n"
    #endif
    #if defined(GASNETI_FORCE_GETTIMEOFDAY)
      "        FORCED timers using gettimeofday()\n"
    #elif defined(GASNETI_FORCE_POSIX_REALTIME)
      "        FORCED timers using clock_gettime()\n"
    #endif
    #if defined(GASNETI_BUG1389_WORKAROUND)
      "        FORCED conservative byte-wise local access\n"
    #endif
      ""; 
  return result;
}

/* ------------------------------------------------------------------------------------ */
/* hostname query */
/* get MAXHOSTNAMELEN */ 
#if PLATFORM_OS_SOLARIS 
#include <netdb.h>
#else
#include <sys/param.h>
#endif 
#ifndef MAXHOSTNAMELEN
  #ifdef HOST_NAME_MAX
    #define MAXHOSTNAMELEN HOST_NAME_MAX
  #else
    #define MAXHOSTNAMELEN 1024 /* give up */
  #endif
#endif
const char *gasneti_gethostname(void) {
  static gasneti_mutex_t hnmutex = GASNETI_MUTEX_INITIALIZER;
  static int firsttime = 1;
  static char hostname[MAXHOSTNAMELEN];
  gasneti_mutex_lock(&hnmutex);
    if (firsttime) {
      if (gethostname(hostname, MAXHOSTNAMELEN))
        gasnett_fatalerror("gasneti_gethostname() failed to get hostname: aborting");
      hostname[MAXHOSTNAMELEN - 1] = '\0';
      firsttime = 0;
    }
  gasneti_mutex_unlock(&hnmutex);
  return hostname;
}
/* ------------------------------------------------------------------------------------ */
/* sleep/delay support */

/* Sleep for at least ns_delay nanoseconds
 * If interrupted by signal, may terminate early returning non-zero with errno = EINTR
 */
extern int gasneti_nsleep(uint64_t ns_delay) {
  if_pf (!ns_delay) return 0;
  #if HAVE_NANOSLEEP
    struct timespec ts;
    ts.tv_sec  = ns_delay / (uint64_t)1E9;
    ts.tv_nsec = ns_delay % (uint64_t)1E9;
    return nanosleep(&ts, NULL);
  #elif HAVE_CLOCK_NANOSLEEP
    struct timespec ts;
    ts.tv_sec  = ns_delay / (uint64_t)1E9;
    ts.tv_nsec = ns_delay % (uint64_t)1E9;
    return clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);
  #elif HAVE_NSLEEP
    struct timespec ts;
    ts.tv_sec  = ns_delay / (uint64_t)1E9;
    ts.tv_nsec = ns_delay % (uint64_t)1E9;
    return nsleep(&ts, NULL);
  #elif HAVE_USLEEP && 0
    // Disabled because:
    // 1) some systems prohibit us_delay > 1 million
    // 2) some systems have undesired interactions with alarm()
    return usleep(ns_delay / 1000);
  #else
    struct timeval tv;
    uint64_t us_delay = ns_delay / 1000;
    tv.tv_sec  = us_delay / (uint64_t)1E6;
    tv.tv_usec = us_delay % (uint64_t)1E6;
    return select(0, NULL, NULL, NULL, &tv);
  #endif
}

/* ------------------------------------------------------------------------------------ */
/* timer support */

#ifdef GASNETI_TIMER_DEFN
  GASNETI_TIMER_DEFN
#endif

GASNETI_INLINE(_gasneti_gettimeofday_us)
uint64_t _gasneti_gettimeofday_us(void) {
  uint64_t retval;
  struct timeval tv;
  gasneti_assert_zeroret(gettimeofday(&tv, NULL));
  retval = ((uint64_t)tv.tv_sec) * 1000000 + (uint64_t)tv.tv_usec;
  return retval;
}
extern uint64_t gasneti_gettimeofday_us(void) { return _gasneti_gettimeofday_us(); }

GASNETI_INLINE(_gasneti_wallclock_ns)
uint64_t _gasneti_wallclock_ns(void) {
  #if HAVE_CLOCK_GETTIME
    struct timespec tm;
    #if defined(_POSIX_MONOTONIC_CLOCK)
      static clockid_t clockid = CLOCK_MONOTONIC;
      if_pf (clock_gettime(clockid,&tm)) {
        clockid = CLOCK_REALTIME; // next call will succeed
        gasneti_assert_zeroret(clock_gettime(CLOCK_REALTIME,&tm));
      }
    #else
      gasneti_assert_zeroret(clock_gettime(CLOCK_REALTIME,&tm));
    #endif
    return tm.tv_sec*((uint64_t)1E9)+tm.tv_nsec;
  #else
    struct timeval tv;
    gasneti_assert_zeroret(gettimeofday(&tv, NULL));
    return ((uint64_t)tv.tv_sec)*1000000000 + ((uint64_t)tv.tv_usec)*1000;
  #endif
}
extern uint64_t gasneti_wallclock_ns(void) { return _gasneti_wallclock_ns(); }

// Conditionally available:
#if GASNETI_USING_GETTIMEOFDAY // Used *only* for gtod-based timers:
  extern uint64_t gasneti_ticks_gtod_us(void) { return _gasneti_gettimeofday_us(); }
#endif
#if GASNETI_USING_POSIX_REALTIME // Used *only* for POSIX-RT timers:
  extern uint64_t gasneti_ticks_posix_ns(void) { return _gasneti_wallclock_ns(); }
#endif

extern double gasneti_tick_metric(int idx) {
  static double *_gasneti_tick_metric = NULL;
  gasneti_assert_int(idx ,<=, 1);
  if_pf (_gasneti_tick_metric == NULL) {
    int i, ticks, iters = 1000, minticks = 10;
    double *_tmp_metric;
    gasneti_tick_t min = GASNETI_TICK_MAX;
    gasneti_tick_t start = gasneti_ticks_now();
    gasneti_tick_t last = start;
    for (i=0,ticks=0; i < iters || ticks < minticks; i++) {
      gasneti_tick_t x = gasneti_ticks_now();
      gasneti_tick_t curr = (x - last);
      if_pt (curr > 0) { 
        ticks++;
        if_pf (curr < min) min = curr;
      }
      last = x;
    }
    _tmp_metric = (double *)malloc(2*sizeof(double));
    gasneti_assert(_tmp_metric != NULL);
    /* granularity */
    _tmp_metric[0] = ((double)gasneti_ticks_to_ns(min))/1000.0;
    /* overhead */
    _tmp_metric[1] = ((double)(gasneti_ticks_to_ns(last - start)))/(i*1000.0);
    gasneti_sync_writes();
    _gasneti_tick_metric = _tmp_metric;
  } else gasneti_sync_reads();
  return _gasneti_tick_metric[idx];
}
/* ------------------------------------------------------------------------------------ */
#ifndef GASNETI_MAYBE_TRACEFILE
  #if GASNET_TRACE && GASNETI_BUILDING_CONDUIT
    extern FILE *gasneti_tracefile;
    #define GASNETI_MAYBE_TRACEFILE gasneti_tracefile
  #else
    #define GASNETI_MAYBE_TRACEFILE ((FILE *)NULL)
  #endif
#endif
extern const char *gasneti_procid_str;
const char *gasneti_procid_str = NULL;

extern void gasneti_console_messageVA(const char *prefix, const char *msg, va_list argptr) {
  #ifndef GASNETI_CONSOLEMSG_PREFIX_LEN
  #define GASNETI_CONSOLEMSG_PREFIX_LEN 128
  #endif
  #ifndef GASNETI_CONSOLEMSG_IDSTR_LEN
  #define GASNETI_CONSOLEMSG_IDSTR_LEN MIN(MAXHOSTNAMELEN,128)
  #endif
  char expandedmsg[GASNETI_CONSOLEMSG_PREFIX_LEN+GASNETI_CONSOLEMSG_IDSTR_LEN+20];
  if (gasneti_procid_str) {
    snprintf(expandedmsg, sizeof(expandedmsg)-4, "*** %s (%s): ", prefix, gasneti_procid_str);
  } else {
    // we are either in tools-only mode or early in conduit startup before procid's are established
    // try to provide some useful information to identify the failing process.
    int pid = (int)getpid();
    // Do NOT use gasneti_gethostname here, too many dependencies and chance of recursion
    char hostname[MAXHOSTNAMELEN];
    if (!gethostname(hostname, MAXHOSTNAMELEN) && 
        hostname[0] && strlen(hostname) < GASNETI_CONSOLEMSG_IDSTR_LEN) {
      snprintf(expandedmsg, sizeof(expandedmsg)-4, "*** %s (%s:%i): ", prefix, hostname, pid);
    } else { // no idstr easily accessible
      snprintf(expandedmsg, sizeof(expandedmsg)-4, "*** %s (:%i): ", prefix, pid);
    }
  }
  const size_t maxmsg = sizeof(expandedmsg)-4 - strlen(expandedmsg);
  const size_t msglen = strlen(msg);

  int isshort = 0;
  int isveryshort = 0;
  #ifndef GASNETI_CONSOLEMSG_VERYSHORT_LEN
  #define GASNETI_CONSOLEMSG_VERYSHORT_LEN 256
  #endif
  char veryshort_msg[GASNETI_CONSOLEMSG_VERYSHORT_LEN];
  if (msglen <= maxmsg) { // short enough to send to fprintf(stderr) in a single operation
    strncat(expandedmsg, msg, maxmsg);
    if (expandedmsg[strlen(expandedmsg)-1] != '\n') strcat(expandedmsg, "\n");
    isshort = 1;

    va_list args;
    va_copy(args, argptr);
      int result = vsnprintf(veryshort_msg, sizeof(veryshort_msg), expandedmsg, args);
      if (result < sizeof(veryshort_msg)) isveryshort = 1; // short enough to send as a formatted buffer
    va_end(args);
  }

  FILE * streams[] = { stderr, GASNETI_MAYBE_TRACEFILE };
  for (int s = 0; s < sizeof(streams)/sizeof(streams[0]); s++) {
    FILE *stream = streams[s];
    if (stream) {
      if (isveryshort) {
        fputs(veryshort_msg, stream);
      } else {
        va_list args;
        va_copy(args, argptr);
          if (isshort) {
            vfprintf(stream, expandedmsg, args);
          } else { /* long format msg */
            fputs(expandedmsg, stream);
            vfprintf(stream, msg, args);
            if (msg[msglen-1] != '\n') fprintf(stream, "\n");
          }
        va_end(args);
      }
      fflush(stream);
    }
  }
}

extern void gasneti_console_message(const char *prefix, const char *msg, ...) {
  va_list argptr;
  va_start(argptr, msg); /*  pass in last argument */
    gasneti_console_messageVA(prefix, msg, argptr);
  va_end(argptr);
}

/* Because some glibc headers annotate nearly all system calls
 * with "__attribute__ ((__warn_unused_result__))", we need to
 * do "something" with return values to avoid gcc warnings.
 * Since here in the backtrace code we are dealing with handling
 * of a (presumed fatal) error case, we can't really deal with
 * most errors in any intelligent way.
 * This is a stupid hack to deal with this.
 */
static int gasneti_rc_unused;

extern void gasneti_error_abort(void) {

  gasnett_freezeForDebuggerErr(); /* allow freeze */

  /* try to get a pre-signal backtrace, which may be more precise */
  if (!gasneti_print_backtrace_ifenabled(STDERR_FILENO)) 
    gasneti_atomic_set(&gasneti_backtrace_enabled,0,GASNETI_ATOMIC_REL);

  // Try to flush I/O (especially the tracefile) before crashing
  signal(SIGALRM, _exit); alarm(5); 
  gasneti_flush_streams();

  #if PLATFORM_OS_CYGWIN && CYGWIN_VERSION_DLL_MAJOR < 3000 && GASNETT_THREAD_SAFE
    // Bug 3856 - Cygwin signal-handling discrepancies with multiple threads
    // Following should be equivalent to abort(), but Cygwin 2.x abort is non-compliant
    // and this generates more reliable behavior:
    if (gasneti_raise(SIGABRT) == 0) (void)0; // success
    else
  #endif /* intentional fall-thru */
    abort();

  const char err[] = "ERROR: abort() returned!\n";
  gasneti_rc_unused = write(2 /*stderr*/, err, sizeof(err));
  (void)fsync(2);

  // ensure this function never returns, even if abort does
  _exit(1);
}

extern void gasneti_fatalerror(const char *msg, ...) {
  va_list argptr;
  va_start(argptr, msg); /*  pass in last argument */
    gasneti_console_messageVA("FATAL ERROR", msg, argptr);
  va_end(argptr);
  gasneti_error_abort();
}

extern void _gasneti_assert_fail(const char *funcname, const char *filename, int linenum,
                                 const char *fmt, ...) {
  #ifndef GASNETI_ASSERT_FMT_LEN
  #define GASNETI_ASSERT_FMT_LEN 256
  #endif
  #ifndef GASNETI_ASSERT_NAME_LEN
  #define GASNETI_ASSERT_NAME_LEN  80
  #endif
  // use the last NAME_LEN characters of funcname and filename
  if (!funcname) funcname = "";
  size_t funclen = strlen(funcname);
  if (funclen > GASNETI_ASSERT_NAME_LEN) funcname += (funclen - GASNETI_ASSERT_NAME_LEN);
  if (!filename || !*filename) filename = "*unknown file*";
  size_t filelen = strlen(filename);
  if (filelen > GASNETI_ASSERT_NAME_LEN) filename += (filelen - GASNETI_ASSERT_NAME_LEN);

  // prepend formatted location info to the assertion format string
  char expandedfmt[GASNETI_ASSERT_FMT_LEN];
  if (*funcname)
    snprintf(expandedfmt, sizeof(expandedfmt), 
             "Assertion failure in %s%s at %s:%i: %s",
             funcname, (funcname[strlen(funcname)-1] != ')'?"()":""),
             filename, linenum, fmt);
  else
    snprintf(expandedfmt, sizeof(expandedfmt), 
             "Assertion failure at %s:%i: %s",
             filename, linenum, fmt);

  // generate the fatal error and crash
  va_list argptr;
  va_start(argptr, fmt); /*  pass in last argument */
    gasneti_console_messageVA("FATAL ERROR", expandedfmt, argptr);
  va_end(argptr);
  gasneti_error_abort();
}

/* ------------------------------------------------------------------------------------ */
extern void gasneti_killmyprocess(int exitcode) {
  /* wrapper for _exit() that does the "right thing" to immediately kill this process */
  #if GASNETI_THREADS && defined(HAVE_PTHREAD_KILL_OTHER_THREADS_NP)
    /* on LinuxThreads we need to explicitly kill other threads before calling _exit() */
    pthread_kill_other_threads_np();
  #endif
  _exit(exitcode); /* use _exit to bypass atexit handlers */
  gasneti_fatalerror("gasneti_killmyprocess failed to kill the process!");
}
extern void gasneti_filesystem_sync(void) {
  static int enabled = -1;
  if (enabled == -1) enabled = gasneti_getenv_yesno_withdefault("GASNET_FS_SYNC",0);
  if (enabled) {
    sync();
  }
}
extern void gasneti_flush_streams(void) {
  /* XXX: When should we consider failures to be errors?  When should we warn?
   *      For a long time failures here were fatal, but that choice precludes
   *      clients or systems which close the standard fds.
   */
  fflush(stdout); 
  fflush(stderr);
  fsync(STDOUT_FILENO); /* ignore errors for output is a console */
  fsync(STDERR_FILENO); /* ignore errors for output is a console */
  fflush(NULL); /* passing NULL to fflush SHOULD cause it to flush all open FILE streams */
  gasneti_filesystem_sync();
  gasneti_sched_yield();
}
extern void gasneti_close_streams(void) {
  /* XXX: When should we consider failures to be errors?  When should we warn?
   *      For a long time failures here were fatal, but that choice precludes
   *      clients or systems which close the standard fds.
   */
  gasneti_reghandler(SIGPIPE, SIG_IGN); /* In case we still try to generate output */
  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
  gasneti_sched_yield();
}
/* ------------------------------------------------------------------------------------ */
static void (*_gasneti_exitfn)(int);
#if HAVE_ON_EXIT
  static void gasneti_on_exit(int exitcode, void *arg) {
    if (_gasneti_exitfn) _gasneti_exitfn(exitcode);
  }
#else
  static void gasneti_atexit(void) {
    if (_gasneti_exitfn) _gasneti_exitfn(0);
  }
#endif
extern void gasneti_registerExitHandler(void (*_exitfn)(int)) {
  _gasneti_exitfn = _exitfn;
  static int firstcall = 1;
  if (!firstcall) return;
  firstcall = 0;
  if (gasneti_getenv_yesno_withdefault("GASNET_CATCH_EXIT", 1)) {
    #if HAVE_ON_EXIT
      on_exit(gasneti_on_exit, NULL);
    #else
      atexit(gasneti_atexit);
    #endif
  }
}
/* ------------------------------------------------------------------------------------ */
extern gasneti_sighandlerfn_t gasneti_reghandler(int sigtocatch, gasneti_sighandlerfn_t fp) {
  gasneti_sighandlerfn_t fpret = (gasneti_sighandlerfn_t)signal(sigtocatch, fp); 
  if (fpret == (gasneti_sighandlerfn_t)SIG_ERR) {
    gasneti_fatalerror("Got a SIG_ERR while registering handler for signal %i : %s", 
                       sigtocatch,strerror(errno));
    return NULL;
  }
  #ifdef SIG_HOLD
    else if (fpret == (gasneti_sighandlerfn_t)SIG_HOLD) {
      gasneti_fatalerror("Got a SIG_HOLD while registering handler for signal %i : %s", 
                         sigtocatch,strerror(errno));
      return NULL;
    }
  #endif
  return fpret;
}
/* ------------------------------------------------------------------------------------ */
typedef struct { 
  int signum;
  const char *name;
  const char *desc;
  enum {
    GASNETI_SV_PROGRAM_ERROR, /* program errors that will continus to occur if ignored */
    GASNETI_SV_TERM_INT,      /* interrupt signal from the terminal (stop or kill) */
    GASNETI_SV_SYS_INT,       /*  interrupt signal from the system (kill, hangup, etc.) */
    GASNETI_SV_FATAL,         /*  interrupts that cannot be caught or ignored */
    GASNETI_SV_OTHER          /*  everything else  */
  } variety;
  int enable_gasnet_handler;
  gasneti_sighandlerfn_t oldhandler;
} gasnett_siginfo_t;
static gasnett_siginfo_t gasneti_sigtable[] = {
  #ifdef SIGABRT
    {SIGABRT, "SIGABRT", "Process abort signal.", GASNETI_SV_PROGRAM_ERROR, 1}, /*  (abort()) */
  #endif
  #ifdef SIGFPE
    {SIGFPE,  "SIGFPE", "Erroneous arithmetic operation.", GASNETI_SV_PROGRAM_ERROR, 1}, /*  (FP error) */
  #endif
  #ifdef SIGILL
    {SIGILL,  "SIGILL", "Illegal instruction.", GASNETI_SV_PROGRAM_ERROR, 1}, /*  (bad instruction) */
  #endif
  #ifdef SIGINT
    {SIGINT,  "SIGINT", "Terminal interrupt signal.", GASNETI_SV_TERM_INT, 1}, /*  (control-c) */
  #endif
  #ifdef SIGSEGV
    {SIGSEGV, "SIGSEGV", "Invalid memory reference.", GASNETI_SV_PROGRAM_ERROR, 1}, /*  (seg fault) */
  #endif
  #ifdef SIGTERM
    {SIGTERM, "SIGTERM", "Termination signal.", GASNETI_SV_SYS_INT, 1}, /*  (kill command) */
  #endif
  #ifdef SIGALRM
    {SIGALRM, "SIGALRM", "Alarm clock.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGHUP
    {SIGHUP,  "SIGHUP", "Hangup.", GASNETI_SV_SYS_INT, 1},
  #endif
  #ifdef SIGKILL
    {SIGKILL, "SIGKILL", "Kill (cannot be caught or ignored).", GASNETI_SV_FATAL, 0}, /*  (kill -9 command) */
  #endif
  #ifdef SIGPIPE
    {SIGPIPE, "SIGPIPE", "Write on a pipe with no one to read it.", GASNETI_SV_OTHER, 1}, /*  (send() after close) */
  #endif
  #ifdef SIGQUIT
    {SIGQUIT, "SIGQUIT", "Terminal quit signal.", GASNETI_SV_TERM_INT, 1}, /*  (control-\) */
  #endif
  #ifdef SIGUSR1
    {SIGUSR1, "SIGUSR1", "User-defined signal 1.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGUSR2
    {SIGUSR2, "SIGUSR2", "User-defined signal 2.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGCHLD
    {SIGCHLD, "SIGCHLD", "Child process terminated or stopped.", GASNETI_SV_OTHER, 0}, /*  (sent to parent proc) */
  #endif
  #ifdef SIGCONT
    {SIGCONT, "SIGCONT", "Continue executing, if stopped.", GASNETI_SV_OTHER, 0}, /*  (also sent by kill command) */
  #endif
  #ifdef SIGSTOP
    {SIGSTOP, "SIGSTOP", "Stop executing (cannot be caught or ignored).", GASNETI_SV_FATAL, 0},
  #endif
  #ifdef SIGTSTP
    {SIGTSTP, "SIGTSTP", "Terminal stop signal.", GASNETI_SV_TERM_INT, 0}, /*  (control-z) */
  #endif
  #ifdef SIGTTIN
    {SIGTTIN, "SIGTTIN", "Background process attempting read.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGTTOU
    {SIGTTOU, "SIGTTOU", "Background process attempting write.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGBUS
    {SIGBUS,  "SIGBUS", "Bus error.", GASNETI_SV_PROGRAM_ERROR, 1}, /*  (alignment error) */
  #endif
  #ifdef SIGPOLL
    {SIGPOLL, "SIGPOLL", "Pollable event.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGXFSZ
    {SIGXFSZ, "SIGXFSZ", " File size limit exceeded.", GASNETI_SV_PROGRAM_ERROR, 0},
  #endif
  #ifdef SIGPROF
    {SIGPROF, "SIGPROF", "Profiling timer expired.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGSYS
    {SIGSYS,  "SIGSYS", "Bad system call.", GASNETI_SV_PROGRAM_ERROR, 0},
  #endif
  #ifdef SIGTRAP
    {SIGTRAP, "SIGTRAP", "Trace/breakpoint trap.", GASNETI_SV_PROGRAM_ERROR, 0},
  #endif
  #ifdef SIGURG
    {SIGURG,  "SIGURG", "High bandwidth data is available at a socket.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGVTALRM
    {SIGVTALRM,"SIGVTALRM", "Virtual timer expired.", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGXCPU
    {SIGXCPU, "SIGXCPU", "CPU time limit exceeded.", GASNETI_SV_PROGRAM_ERROR, 0},
  #endif
  #ifdef SIGEMT
    {SIGEMT,     "SIGEMT", "Emulation Trap", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGPWR
    {SIGPWR,     "SIGPWR", "Power Fail or Restart", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGWINCH
    {SIGWINCH,   "SIGWINCH", "Window Size Change", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGWAITING
    {SIGWAITING, "SIGWAITING", "Concurrency signal reserved  by threads library", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGLWP
    {SIGLWP,     "SIGLWP", "Inter-LWP  signal  reserved  by threads library", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGFREEZE
    {SIGFREEZE,  "SIGFREEZE", "Check point Freeze", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGTHAW
    {SIGTHAW,    "SIGTHAW", "Check point Thaw", GASNETI_SV_OTHER, 0},
  #endif
  #ifdef SIGCANCEL
    {SIGCANCEL,  "SIGCANCEL", "Cancellation signal reserved by threads library", GASNETI_SV_OTHER, 0},
  #endif
    {0, NULL, NULL, GASNETI_SV_OTHER, 0}
};
/* ------------------------------------------------------------------------------------ */
gasnett_siginfo_t *gasnett_siginfo_fromval(int sigval) {
  size_t i;
  for (i=0; i<sizeof(gasneti_sigtable)/sizeof(gasnett_siginfo_t)-1; i++) {
    if (sigval == gasneti_sigtable[i].signum) return &gasneti_sigtable[i];
  }
  return NULL;
}
const char *gasnett_signame_fromval(int sigval) {
    gasnett_siginfo_t  *s;
    
    if ((s = gasnett_siginfo_fromval(sigval)))
        return(s->name);
    return(NULL);
}
gasnett_siginfo_t *gasnett_siginfo_fromstr(const char *str) {
  while (*str && isspace(*str)) str++;
  if (isdigit(*str)) return gasnett_siginfo_fromval(atoi(str));
  else {
    size_t i;
    char tmp[255];
    char *p = &tmp[0];
    if (!(strlen(str) >= 3 && 
      toupper(str[0]) == 'S' && toupper(str[1]) == 'I' && toupper(str[2]) == 'G')) {
      strcpy(p,"SIG"); p += 3;
    }
    while (*str && !isspace(*str)) {
      *(p++) = toupper(*(str++));
    }
    *p = '\0';
    for (i=0; i<sizeof(gasneti_sigtable)/sizeof(gasnett_siginfo_t)-1; i++) {
      if (!strcmp(tmp, gasneti_sigtable[i].name)) return &gasneti_sigtable[i];
    }
    return NULL;
  }
}
/* ------------------------------------------------------------------------------------ */
extern int gasneti_raise(int sig) {
  #if PLATFORM_OS_CYGWIN && CYGWIN_VERSION_DLL_MAJOR < 3000 && \
      GASNETT_THREAD_SAFE && HAVE_PTHREAD_KILL 
    // Bug 3856 - Cygwin signal-handling discrepancies with multiple threads
    // Following should be equivalent to raise(), but Cygwin 2.x raise is non-compliant
    // and this generates more reliable behavior.
    // This workaround was previously used for a bug in OpenBSD-5.2 kernel, fixed in OpenBSD-current in Nov 2012
    if (pthread_kill(pthread_self(), sig) == 0) return 0; // success
  #endif /* intentional fall-thru */

  return raise(sig);
}
/* ------------------------------------------------------------------------------------ */
/* Functions to (un)block a single signal:
 *      int gasneti_unblocksig(int sig);
 *      int gasneti_blocksig(int sig);
 * On error (including systems w/o support) both return -1.
 * Otherwise they return the prior state: positive for blocked, zero for not blocked.
 */
#if HAVE_SIGPROCMASK || (HAVE_PTHREAD_SIGMASK && GASNETI_THREADS)
  static int _gasneti_sigmask(int sig, int op) {
    sigset_t sig_set, old_set;
    sigemptyset(&sig_set);
    sigaddset(&sig_set, sig);
  #if HAVE_PTHREAD_SIGMASK && GASNETI_THREADS
    if (! pthread_sigmask(op, &sig_set, &old_set)) {
      return sigismember(&old_set, sig);
    }
  #endif
  #if HAVE_SIGPROCMASK
    if (! sigprocmask(op, &sig_set, &old_set)) {
      return sigismember(&old_set, sig);
    }
  #endif
    return -1;
  }
  extern int gasneti_blocksig(int sig)   { return _gasneti_sigmask(sig, SIG_BLOCK);   }
  extern int gasneti_unblocksig(int sig) { return _gasneti_sigmask(sig, SIG_UNBLOCK); }
#else
  /* TODO: implement for systems w/o POSIX signals */
  extern int gasneti_blocksig(int sig)   { return -1; }
  extern int gasneti_unblocksig(int sig) { return -1; }
#endif
/* ------------------------------------------------------------------------------------ */
#ifndef GASNETI_UNFREEZE_SIGNAL
/* signal to use for unfreezing, could also use SIGUSR1/2 or several others */
#define GASNETI_UNFREEZE_SIGNAL SIGCONT
#define GASNETI_UNFREEZE_SIGNAL_STR "SIGCONT"
#endif
static volatile int * volatile _gasneti_freeze_flag = NULL;
static void gasneti_unfreezeHandler(int sig) {
  gasneti_assert(_gasneti_freeze_flag);
  *_gasneti_freeze_flag = 0;
}
/*  all this to make sure we get a full stack frame for debugger */
static void _freezeForDebugger(int depth) {
  if (!depth) _freezeForDebugger(1);
  else {
    volatile int i=0;
    gasneti_sighandlerfn_t old = gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, gasneti_unfreezeHandler);
    const int was_blocked = (gasneti_unblocksig(GASNETI_UNFREEZE_SIGNAL) > 0);
    while (*_gasneti_freeze_flag) {
      i++;
      sleep(1);
    }
    if (was_blocked) gasneti_blocksig(GASNETI_UNFREEZE_SIGNAL);
  }
}
extern void gasneti_freezeForDebuggerNow(volatile int *flag, const char *flagsymname) {
  gasneti_console_message("Process frozen for debugger","host=%s  pid=%i\n"
                 "    To unfreeze, attach a debugger and set '%s' to 0, or send a "
                 GASNETI_UNFREEZE_SIGNAL_STR "\n", 
                 gasnett_gethostname(), (int)getpid(), flagsymname); 
  _gasneti_freeze_flag = flag;
  *_gasneti_freeze_flag = 1;
  gasneti_local_wmb();
  _freezeForDebugger(0);
}

volatile int gasnet_frozen = 0;
static int gasneti_freezeonerr_isinit = 0;
static int gasneti_freezeonerr_userenabled = 0;
static int gasneti_freezesignal = 0;
static int gasneti_backtracesignal = 0;

static void gasneti_ondemandHandler(int sig) {
  gasnett_siginfo_t const *siginfo = gasnett_siginfo_fromval(sig);
  char sigstr[80];
  if (siginfo) snprintf(sigstr, sizeof(sigstr), "%s(%i)", siginfo->name, sig);
  else  snprintf(sigstr, sizeof(sigstr), "(%i)", sig);
  if (sig == gasneti_freezesignal) {
    gasneti_console_message("Caught GASNET_FREEZE_SIGNAL","signal %s\n", sigstr);
    gasneti_freezeForDebuggerNow(&gasnet_frozen,"gasnet_frozen");
  } else if (sig == gasneti_backtracesignal) {
    gasneti_console_message("Caught GASNET_BACKTRACE_SIGNAL","signal %s\n", sigstr);
    gasneti_print_backtrace(STDERR_FILENO);
  } else gasneti_fatalerror("unrecognized signal in gasneti_ondemandHandler: %i", sig);
}

extern void gasneti_ondemand_init(void) {
  static int firsttime = 1;
  if (firsttime) {
    const char *str = gasneti_getenv_withdefault("GASNET_FREEZE_SIGNAL",NULL);
    if (str) {
      gasnett_siginfo_t const *info = gasnett_siginfo_fromstr(str);
      if (!info) gasneti_console_message("WARNING","ignoring unrecognized GASNET_FREEZE_SIGNAL: %s\n", str);
      else gasneti_freezesignal = info->signum;
    }
    str = gasneti_getenv_withdefault("GASNET_BACKTRACE_SIGNAL",NULL);
    if (str) {
      gasnett_siginfo_t const *info = gasnett_siginfo_fromstr(str);
      if (!info) gasneti_console_message("WARNING","ignoring unrecognized GASNET_BACKTRACE_SIGNAL: %s\n", str);
      else gasneti_backtracesignal = info->signum;
    }
    gasneti_local_wmb();
    firsttime = 0;
  } else gasneti_local_rmb();

  if (gasneti_backtracesignal) 
    gasneti_reghandler(gasneti_backtracesignal, gasneti_ondemandHandler);
  if (gasneti_freezesignal) 
    gasneti_reghandler(gasneti_freezesignal, gasneti_ondemandHandler);
}

static void gasneti_freezeForDebugger_init(void) {
  if (gasneti_freezeonerr_isinit) { gasneti_local_rmb(); return; }
  gasneti_freezeonerr_userenabled = gasneti_getenv_yesno_withdefault("GASNET_FREEZE_ON_ERROR",0);
  gasneti_local_wmb();
  gasneti_freezeonerr_isinit = 1;

  gasneti_ondemand_init();
}
extern void gasneti_freezeForDebuggerErr(void) {
  gasneti_freezeForDebugger_init();
  if (gasneti_freezeonerr_userenabled)
    gasneti_freezeForDebuggerNow(&gasnet_frozen,"gasnet_frozen"); /* allow user freeze */
}
/* ------------------------------------------------------------------------------------ */
/* Dynamic backtrace support */

/* Prefix a string with the CWD, unless already fully qualified.
 * path_out must have size at least PATH_MAX.
 * Also used by gasnet_trace.c
 */
extern void gasneti_qualify_path(char *path_out, const char *path_in) {
  if (path_in[0] == '/' || path_in[0] == '\\') {
    path_out[0] = '\0';
  } else {
    if (getcwd(path_out, PATH_MAX)) {
      strcat(path_out,"/");
    } else {
      strcpy(path_out,"/GETCWD_FAILED/");
    }
  }
  strcat(path_out, path_in);
}

/* All configure-detected backtrace mechanisms available */
#if HAVE_BACKTRACE
  #define GASNETI_BT_EXECINFO	&gasneti_bt_execinfo
#endif
#if HAVE_PRINTSTACK
  #define GASNETI_BT_PRINTSTACK	&printstack
#endif
#if defined(GSTACK_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_GSTACK	&gasneti_bt_gstack
#endif
#if defined(PSTACK_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_PSTACK	&gasneti_bt_pstack
#endif
#if defined(GDB_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_GDB	&gasneti_bt_gdb
#endif
#if defined(DBX_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_DBX	&gasneti_bt_dbx
#endif
#if defined(IDB_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_IDB	&gasneti_bt_idb
#endif
#if defined(LLDB_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_LLDB	&gasneti_bt_lldb
#endif
#if defined(PGDBG_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_PGDBG	&gasneti_bt_pgdbg
#endif

#if !GASNETI_NO_FORK
/* Execute system w/ stdout redirected to 'fd' and std{in,err} to /dev/null */
static int gasneti_system_redirected(const char *cmd, int stdout_fd) {
  int rc;
  int saved_stdin, saved_stdout, saved_stderr;
  off_t beginpos, endpos;


  /* XXX: what if the following two writes fail? */
  gasneti_rc_unused = write(stdout_fd, cmd, strlen(cmd));
  gasneti_rc_unused = write(stdout_fd, "\n", 1);

  beginpos = lseek(stdout_fd, 0, SEEK_CUR); /* fetch current position */

  /* Redirect output to 'fd' and std{in,err} to /dev/null */
  saved_stdin = dup(STDIN_FILENO);
  saved_stdout = dup(STDOUT_FILENO);
  saved_stderr = dup(STDERR_FILENO);
  dup2(stdout_fd, STDOUT_FILENO);
  rc = open("/dev/null", O_WRONLY); dup2(rc, STDERR_FILENO); close(rc);
  rc = open("/dev/null", O_RDONLY); dup2(rc, STDIN_FILENO); close(rc);

  /* Run the command */
  rc = system(cmd); // will return -1 on failure to spawn child process

  endpos = lseek(stdout_fd, 0, SEEK_CUR); /* fetch current position */
  if (!rc && beginpos > 0 && endpos > 0 && (beginpos == endpos)) {
    rc = -2; /* command failed to generate output - consider it a failure */
  }

  /* Restore I/O */
  dup2(saved_stdout, STDOUT_FILENO); close(saved_stdout);
  dup2(saved_stderr, STDERR_FILENO); close(saved_stderr);
  dup2(saved_stdin, STDIN_FILENO); close(saved_stdin);

  return rc;
}

static volatile int gasneti_bt_complete_flag = 0;
static void gasneti_bt_complete_handler(int sig) {
  gasneti_bt_complete_flag = 1;
}
/* Fork a co-process to execute gasneti_system_redirected with the supplied arguments, 
   and wait for completion in a manner which avoids placing the wait system call on the 
   top of the target process's stack (because it confuses some stackwalkers) */
static int gasneti_system_redirected_coprocess(const char *cmd, int stdout_fd) {
  FILE *file;
  int tmpfd;
  int rc = 0;
  pid_t parentpid = getpid();

  /* Create a tmpfile to communicate with the child */
  file = tmpfile();
  if (!file) return -3;
  tmpfd = fileno(file);

  { /* setup the parent to sleep */
    gasneti_sighandlerfn_t old_sigh = gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, gasneti_bt_complete_handler);
    const int was_blocked = (gasneti_unblocksig(GASNETI_UNFREEZE_SIGNAL) > 0);

    volatile int i=0;
    if (!fork()) { /* the child - debugger co-process launcher */
#if PLATFORM_OS_OPENBSD || PLATFORM_OS_DARWIN
      // OpenBSD refuses to ptrace attach a connected ancestor because it
      // would create a cycle in the process tree which the kernel is unable
      // to tolerate.  This behavior was introduced in OpenBSD 4.5 Errata 011
      // and has not changed though at least OpenBSD 6.1.
      // We avoid this cycle using an extra fork()+_exit() to disconnect the
      // process requesting the attach from the target.
      // It appeares macOS 10.15 (Darwin 19) also needs this separation,
      // though it is unknown if the reason is the same as for OpenBSD.
    #if PLATFORM_OS_DARWIN
      struct utsname uname_data;
      int os_ver = uname(&uname_data) ? 0 : atoi(uname_data.release);
      if (!os_ver || os_ver >= 19) // assume extra fork() needed if fail to query os release
    #endif
      {
        pid_t childpid = getpid();
        if (fork()) _exit(0);
        do {} while (getppid() == childpid);
      }
#endif
      int retval = gasneti_system_redirected(cmd, tmpfd);
      if (retval) { /* system call failed - nuke the output */
        gasneti_rc_unused = ftruncate(tmpfd, 0);
      } 
#if 0 /* gasneti_filesystem_sync() is currenlty a no-op by default */
      gasneti_filesystem_sync(); /* flush output */
#else
      fsync(tmpfd); /* flush output */
#endif
      kill(parentpid, GASNETI_UNFREEZE_SIGNAL); /* signal the parent of completion */
      gasneti_killmyprocess(0); /* die */
    } else { /* the parent - our debugger target */
      struct stat tmpstat;
      while (!gasneti_bt_complete_flag) {
        i++;
        gasneti_sched_yield(); /* sched_yield seems to be friendlier than sleep() for stack-walkers */
      }
      /* awakened */
      gasneti_bt_complete_flag = 0;
      if (was_blocked) gasneti_blocksig(GASNETI_UNFREEZE_SIGNAL);
      gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, old_sigh);
      if (fstat(tmpfd, &tmpstat)) rc = -4; /* never happens? */
      else if (tmpstat.st_size == 0) rc = -5; /* child process spawn failed */
      else if (lseek(tmpfd, 0, SEEK_SET)) rc = -6;
      else {
        static char tmpbuf[255];
        ssize_t bytes = tmpstat.st_size;
        while ((bytes = read(tmpfd, &tmpbuf, sizeof(tmpbuf))) > 0 ||
               (bytes == -1 && errno == EINTR)) {
          if (bytes > 0) {
            int retval;
            tryagain:
              retval = write(stdout_fd, tmpbuf, bytes);
              if (retval == -1) {
                if (errno == EINTR) goto tryagain;
                else { rc = -7; break; } /* write error */
              }
          }
        }
        if (bytes == -1) rc = -8; /* read error occurred */
      }
    }
  }
  fclose(file); /* close and delete temp file */
  return rc;
}
#endif

#if (PATH_MAX < 1024)
#define GASNETI_BT_PATHSZ 1024 /* OpenBSD warns if this is smaller than 1024 */
#else
#define GASNETI_BT_PATHSZ PATH_MAX
#endif

static char gasneti_exename_bt[GASNETI_BT_PATHSZ];

static const char *gasneti_tmpdir_bt = "/tmp";
static int gasneti_bt_mkstemp(char *filename, int limit) {
  const char template[] = "/gasnet_XXXXXX";
  char *p;
  int len;

  len = strlen(gasneti_tmpdir_bt);
  len = MIN(len, limit-1);
  memcpy(filename,gasneti_tmpdir_bt,len);
  p = filename + len;

  len = MIN(limit - len - 1, sizeof(template));
  memcpy(p,template,len);

  p[len] = '\0';

  gasneti_assert_int(strlen(filename) ,<, limit);
  return mkstemp(filename);
}

#ifdef GASNETI_BT_DBX
  static int gasneti_bt_dbx(int fd) {
    /* dbx's thread support is poor and not easily scriptable */
    const char fmt[] = "echo 'attach %d; where; quit' | %s '%s'";  
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    const char *dbx = (access(DBX_PATH, X_OK) ? "dbx" : DBX_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, (int)getpid(), dbx, gasneti_exename_bt);
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_IDB
  static int gasneti_bt_idb(int fd) {
    #if GASNETI_THREADS
      const char fmt[] = "echo 'set $stoponattach; attach %d; where thread all; quit' | %s -dbx -quiet '%s'"; 
    #else
      const char fmt[] = "echo 'set $stoponattach; attach %d; where; quit' | %s -dbx -quiet '%s'"; 
    #endif
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    const char *idb = (access(IDB_PATH, X_OK) ? "idb" : IDB_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, (int)getpid(), idb, gasneti_exename_bt);
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_PGDBG
  static int gasneti_bt_pgdbg(int fd) {
    #if GASNETI_THREADS
      const char fmt[] = "%s -text -c 'attach %i %s ; threads ; [all] where ; detach ; quit'";
    #else
      const char fmt[] = "%s -text -c 'attach %i %s ; where ; detach ; quit'";
    #endif
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    const char *pgdbg = (access(PGDBG_PATH, X_OK) ? "pgdbg" : PGDBG_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, pgdbg, (int)getpid(), gasneti_exename_bt);
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_LLDB
  static int gasneti_bt_lldb(int fd) {
    const char fmt[] = "%s -p %d -o 'bt all' -o quit";
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    const char *lldb = (access(LLDB_PATH, X_OK) ? "lldb" : LLDB_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, lldb, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_GSTACK
  static int gasneti_bt_gstack(int fd) {
    static char cmd[12 + GASNETI_BT_PATHSZ];
    const char *gstack = (access(GSTACK_PATH, X_OK) ? "gstack" : GSTACK_PATH);
    int rc = snprintf(cmd, sizeof(cmd), "%s %i", gstack, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_PSTACK
  static int gasneti_bt_pstack(int fd) {
    static char cmd[12 + GASNETI_BT_PATHSZ];
    const char *pstack = (access(PSTACK_PATH, X_OK) ? "pstack" : PSTACK_PATH);
    int rc = snprintf(cmd, sizeof(cmd), "%s %i", pstack, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) return -10;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_GDB
  static int gasneti_bt_gdb(int fd) {
    /* Change "backtrace" to "backtrace full" to also see local vars from each frame */
    #if GASNETI_THREADS
      const char commands[] = "\ninfo threads\nthread apply all backtrace 50\ndetach\nquit\n";
    #elif PLATFORM_OS_CYGWIN 
      /* bug1848: cygwin is always multi-threaded, user thread is usually (always?) #1 */
      const char commands[] = "\nthread 1\nbacktrace 50\ndetach\nquit\n";
    #else
      const char commands[] = "\nbacktrace 50\ndetach\nquit\n";
    #endif
    const char shell_rm[]  = "shell /bin/rm -f ";
    const char fmt[] = "%s -nx -batch -x %s '%s' %d";
    static char cmd[sizeof(fmt) + 3*GASNETI_BT_PATHSZ];
    char filename[GASNETI_BT_PATHSZ];
    const char *gdb = (access(GDB_PATH, X_OK) ? "gdb" : GDB_PATH);
    int rc;

    /* Build gdb commands file, since it won't take commands on stdin */
    {
      int tmpfd, len;

      tmpfd = gasneti_bt_mkstemp(filename,sizeof(filename));
      if (tmpfd < 0) return -11;

      len = sizeof(shell_rm) - 1;
      if (len != write(tmpfd, shell_rm, len)) { rc = -12; goto out; }

      len = strlen(filename);
      if (len != write(tmpfd, filename, len)) { rc = -13; goto out; }

      len = sizeof(commands) - 1;
      if (len != write(tmpfd, commands, len)) { rc = -14; goto out; }

      if (0 != close(tmpfd)) { rc = -15; goto out; }
    }

    rc = snprintf(cmd, sizeof(cmd), fmt, gdb, filename, gasneti_exename_bt, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) {
      rc = -10;
      goto out;
    }

#if PLATFORM_OS_OPENBSD
    // OpenBSD is unable to ptrace attach a connected ancestor.
    // For more info see comment in gasneti_system_redirected_coprocess().
    rc = gasneti_system_redirected_coprocess(cmd, fd);
#else
    rc = gasneti_system_redirected(cmd, fd);
#endif

out:
    (void)unlink(filename); /* just in case */

    return rc;
  }
#endif

#ifdef GASNETI_BT_EXECINFO
  static int gasneti_bt_execinfo(int fd) {
    #define MAXBT 1024
    static void *btaddrs[MAXBT];
    int entries;
    char **fnnames = NULL;
    int i;
    entries = backtrace(btaddrs, MAXBT);
    #if HAVE_BACKTRACE_SYMBOLS
      fnnames = backtrace_symbols(btaddrs, entries);
    #endif
    #if defined(ADDR2LINE_PATH) && !GASNETI_NO_FORK
      // volatile below to avoid an optimizer bug observed on icc 17.0.2
      const char * volatile addr2line_path = (access(ADDR2LINE_PATH, X_OK) ? "addr2line" : ADDR2LINE_PATH);
      const char fmt[] = "%s -f -e '%s' %p";
      static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ + 10];
      #define XLBUF 64 /* even as short as 2 bytes is still safe */
      static char xlstr[XLBUF];
      { strcpy(cmd,addr2line_path);
        strcat(cmd," --version"); // use --version to check if addr2line looks functional
        FILE *fp = popen(cmd,"r");
        while (fp && fgets(xlstr, sizeof(xlstr), fp)) ; // slurp
        if (!fp || pclose(fp)) {
          const char *msg = "*** Warning: "ADDR2LINE_PATH" is unavailable to translate symbols\n";
          gasneti_rc_unused = write(fd, msg, strlen(msg));
          addr2line_path = NULL;
        }
      }
    #endif
    for (i=0; i < entries; i++) {
      /* XXX: what if the write()s fail? */
      static char linebuf[16];
      snprintf(linebuf, sizeof(linebuf), "%i: ", i);
      gasneti_rc_unused = write(fd, linebuf, strlen(linebuf));

      if (fnnames) { // note this usually only gets hex addresses, unless linked w/-rdynamic
        gasneti_rc_unused = write(fd, fnnames[i], strlen(fnnames[i]));
        gasneti_rc_unused = write(fd, " ", 1);
      }

      #if defined(ADDR2LINE_PATH) && !GASNETI_NO_FORK
        if (addr2line_path)
        /* use addr2line when available to retrieve symbolic info */
        {
          FILE *xlate;
          int rc;
          xlstr[0] = '\0';
          rc = snprintf(cmd, sizeof(cmd), fmt, addr2line_path, gasneti_exename_bt, btaddrs[i]);
          if ((rc < 0) || (rc >= sizeof(cmd))) {
            return -10;
          }
          xlate = popen(cmd, "r");
          if (xlate) {
            while (fgets(xlstr, sizeof(xlstr), xlate)) {
              size_t len = strlen(xlstr);
              if (xlstr[len-1] == '\n') xlstr[len-1] = ' ';
              gasneti_rc_unused = write(fd, xlstr, len);
            }
            pclose(xlate);
          }
        }
        #undef XLBUF
      #endif

      gasneti_rc_unused = write(fd, "\n", 1);
    }
    /* if (fnnames) free(fnnames); */
    return 0;
  }
#endif

/* table of known/detected backtrace mechanisms */
static gasnett_backtrace_type_t gasneti_backtrace_mechanisms[] = {
  /*
   * Debuggers capable of backtracing all threads:
   */
  #if defined(GASNETI_BT_LLDB) && PLATFORM_OS_DARWIN // bug3626: vendor-signed debugger has priority
  { "LLDB", GASNETI_BT_LLDB, 1 },
  #endif
  #ifdef GASNETI_BT_GDB
  { "GDB", GASNETI_BT_GDB, 1 },
  #endif
  #ifdef GASNETI_BT_IDB
  { "IDB", GASNETI_BT_IDB, 1 },
  #endif
  #ifdef GASNETI_BT_PGDBG
  { "PGDBG", GASNETI_BT_PGDBG, 1 },
  #endif
  #if defined(GASNETI_BT_LLDB) && !PLATFORM_OS_DARWIN
  { "LLDB", GASNETI_BT_LLDB, 1 },
  #endif
  // On Linux, [gp]stack are shell scripts that invoke gdb. 
  // Place them below gdb to eliminate the script from the process group,
  // since it could interfere with orphan control signalling.
  // On Solaris, pstack is a real utility that generates higher-quality
  // backtraces than dbx.
  #ifdef GASNETI_BT_GSTACK
  { "GSTACK", GASNETI_BT_GSTACK, 1 },
  #endif
  #ifdef GASNETI_BT_PSTACK
  { "PSTACK", GASNETI_BT_PSTACK, 1 },
  #endif
  /*
   * Debuggers NOT capable of backtracing all threads:
   */
  #ifdef GASNETI_BT_DBX
  { "DBX", GASNETI_BT_DBX, 0 },
  #endif
  /*
   * Library calls capable of backtracing only the calling thread:
   */
  #ifdef GASNETI_BT_EXECINFO
  { "EXECINFO", GASNETI_BT_EXECINFO, 1 },
  #endif
  #ifdef GASNETI_BT_PRINTSTACK
  { "PRINTSTACK", GASNETI_BT_PRINTSTACK, 1 },
  #endif
  /*
   * Space for registration of optional user mechanism
   */
  { NULL, NULL, 0 }
};
static int gasneti_backtrace_mechanism_count = /* excludes the NULL */
   (sizeof(gasneti_backtrace_mechanisms)/sizeof(gasneti_backtrace_mechanisms[0])) - 1;

static int gasneti_backtrace_isinit = 0;
static int gasneti_backtrace_userenabled = 0;
#if GASNETI_BUILDING_CONDUIT
static int gasneti_backtrace_userdisabled = 0;
extern const char *gasneti_backtraceid(void); // allow conduit to provide [NODE] line prefix
#endif
static const char *gasneti_backtrace_list = 0;
static int gasneti_backtrace_prctl = -2;
GASNETI_TENTATIVE_LIBRARY
gasnett_backtrace_type_t gasnett_backtrace_user; /* allow client provided backtrace function */
extern void gasneti_backtrace_init(const char *exename) {
  static int user_is_init = 0;

#if HAVE_PR_SET_PTRACER
  // May be necessary to allow ptrace_attach():
  // errors here are ignored, but saved to possibly assist in later diagnosis
  gasneti_backtrace_prctl = prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY);
#endif

  gasneti_qualify_path(gasneti_exename_bt, exename);

  gasneti_backtrace_userenabled = gasneti_getenv_yesno_withdefault("GASNET_BACKTRACE",0);
#if GASNETI_BUILDING_CONDUIT
  if (gasneti_backtrace_userenabled && !gasneti_check_node_list("GASNET_BACKTRACE_NODES")) {
    gasneti_backtrace_userdisabled = 1;
  }
#endif

  gasneti_tmpdir_bt = gasneti_tmpdir();
  if (!gasneti_tmpdir_bt) {
    fprintf(stderr,"WARNING: Failed to init backtrace support because none of $GASNET_TMPDIR, $TMPDIR or /tmp is usable\n");
    fflush(stderr);
    return;
  }

  if (!user_is_init && gasnett_backtrace_user.name && gasnett_backtrace_user.fnp) {
    memcpy(&gasneti_backtrace_mechanisms[gasneti_backtrace_mechanism_count++], &gasnett_backtrace_user, sizeof(gasnett_backtrace_user));
    user_is_init = 1;
  }

  { static char btlist_def[255];
    int th;
    int i;
    btlist_def[0] = '\0';
    #if GASNETI_THREADS
      for (th = 1; th >= 0; th--) 
    #endif
      {
        for (i = 0; i < gasneti_backtrace_mechanism_count; ++i) {
          // silence a buggy array-bounds warning from gcc-5:
          gasneti_assume(i < sizeof(gasneti_backtrace_mechanisms)/sizeof(gasneti_backtrace_mechanisms[0]));
          #if GASNETI_THREADS
          if (th == gasneti_backtrace_mechanisms[i].threadsupport) 
          #endif
            {
              if (strlen(btlist_def)) strcat(btlist_def, ",");
              strcat(btlist_def,gasneti_backtrace_mechanisms[i].name);
            }
        }
      }
  
    gasneti_backtrace_list = gasneti_getenv_withdefault("GASNET_BACKTRACE_TYPE",btlist_def);
  }

  gasneti_backtrace_isinit = 1;
  gasneti_freezeForDebugger_init();
}

/* "best effort" to produce a backtrace
 * Returns 0 on apparent success, non-zero otherwise.
 * NOTE: If fd corresponds to a FILE*, caller should fflush() it first.
 */
extern int gasneti_print_backtrace(int fd) {
  int retval = 1;

  if (!gasneti_backtrace_isinit) {
    fprintf(stderr,"WARNING: Ignoring call to gasneti_print_backtrace before gasneti_backtrace_init\n");
    fflush(stderr);
    return -1;
  }

  /* a hopefully signal-safe lock to ensure mutual exclusion and prevent recursion */
  if (!gasneti_atomic_decrement_and_test(&gasneti_backtrace_enabled,GASNETI_ATOMIC_ACQ)) 
    return -1;

  { /* Save signal handlers to avoid recursion */
    gasneti_sighandlerfn_t old_ABRT = gasneti_reghandler(SIGABRT, SIG_DFL);
    gasneti_sighandlerfn_t old_ILL  = gasneti_reghandler(SIGILL,  SIG_DFL);
    gasneti_sighandlerfn_t old_SEGV = gasneti_reghandler(SIGSEGV, SIG_DFL);
    gasneti_sighandlerfn_t old_BUS  = gasneti_reghandler(SIGBUS,  SIG_DFL);
    gasneti_sighandlerfn_t old_FPE  = gasneti_reghandler(SIGFPE,  SIG_DFL);
    FILE *file;

    /* Create a tmpfile to hold the backtrace */
#if 0 /* tmpfile() doesn't honor TMPDIR (see bug 2671) */
    file = tmpfile();
#else
    {
      char filename[GASNETI_BT_PATHSZ];
      int tmpfd = gasneti_bt_mkstemp(filename,sizeof(filename));
      (void)unlink(filename);
      file = fdopen(tmpfd, "r+");
    }
#endif

    if (file) {
      int tmpfd = fileno(file);
      const char *plist = gasneti_backtrace_list;

      static char linebuf[1024];
      char *linep = linebuf;
      int linelen = sizeof(linebuf);
      const char *btid;
      *linep = '\0';
    #if GASNETI_BUILDING_CONDUIT
      if ((btid = gasneti_backtraceid())) {
        strncpy(linebuf, btid, 80);
        linebuf[80] = '\0';
        linelen -= strlen(linebuf);
        linep += strlen(linebuf);
      }
    #endif

      while (*plist) { /* Loop over selections until success or end */
        int i;
        static char btsel[255]; /* parse selection */
        char *psel = btsel;
        while (*plist && !strchr(" ,|;",*plist)) {
	    /* ICK: Our toupper() replacement for Tru64 is a macro that 
	     * evaluates its argument multiple times.
	     * So, need ++ outside toupper();
	     */
	    *psel++ = toupper(*plist); plist++;
	}
        *psel = '\0';
        if (*plist) plist++;

        for (i = 0; i < gasneti_backtrace_mechanism_count; ++i) {
          // silence a buggy array-bounds warning from gcc-5:
          gasneti_assume(i < sizeof(gasneti_backtrace_mechanisms)/sizeof(gasneti_backtrace_mechanisms[0]));
          if (!strcmp(gasneti_backtrace_mechanisms[i].name,btsel)) {
            snprintf(linep, linelen, "Invoking %s for backtrace...\n", btsel);
            gasneti_rc_unused = write(fd, linebuf, strlen(linebuf));
            retval = (*gasneti_backtrace_mechanisms[i].fnp)(tmpfd);
            break;
          }
        }
        if (i == gasneti_backtrace_mechanism_count) {
          gasneti_console_message("WARNING","GASNET_BACKTRACE_TYPE=%s unrecognized or unsupported - ignoring..\n", btsel);
        } else if (retval == 0) {
	  /* Send to requested destination (and tracefile if any) */
	  GASNETT_TRACE_PRINTF_FORCE("========== BEGIN BACKTRACE ==========");
	  rewind(file);
	  while (fgets(linep, linelen, file)) {
            /* XXX: what if this write() fails? */
            gasneti_rc_unused = write(fd, linebuf, strlen(linebuf)); /* w/ node prefix */
            GASNETT_TRACE_PRINTF_FORCE("%s",linep);/* w/o node prefix */
	  }
	  GASNETT_TRACE_PRINTF_FORCE("========== END BACKTRACE ==========");
          gasneti_flush_streams();
          break;
        } else { /* backtrace attempt failed - retry with next mechanism */
          snprintf(linep, linelen, "%s backtrace failed! (0x%08x:%d)\n", btsel, retval, retval);
          gasneti_rc_unused = write(fd, linebuf, strlen(linebuf));
	  rewind(file);
          gasneti_rc_unused = ftruncate(tmpfd, 0); // in case failed backtrace wrote any output

          // detect and report system configuration issues that may be responsible for backtrace failure
          #if (PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL) && !defined(YAMA_PTRACE_SCOPE)
            #define YAMA_PTRACE_SCOPE "/proc/sys/kernel/yama/ptrace_scope"
          #endif
          #ifdef YAMA_PTRACE_SCOPE
          { int ptracefd = 0;
            if (!access(YAMA_PTRACE_SCOPE,R_OK) && (ptracefd = open(YAMA_PTRACE_SCOPE,O_RDONLY))) {
              char scope = 0; // docs: https://www.kernel.org/doc/Documentation/security/Yama.txt
              if (read(ptracefd, &scope, 1) == 1 
                  && scope != '0' // 0 = no restrictions
                  && !(scope == '1' && !gasneti_backtrace_prctl)) { // 1 = restricted, only works if prctl succeeded
                snprintf(linep, linelen, "WARNING: %s=%c may be preventing debugger attach\n", YAMA_PTRACE_SCOPE, scope);
                gasneti_rc_unused = write(fd, linebuf, strlen(linebuf));
              }
              gasneti_rc_unused = close(ptracefd);
            }
          }
          #endif
          #if PLATFORM_OS_OPENBSD && \
              defined(CTL_KERN) && defined(KERN_GLOBAL_PTRACE)
          { int mib[] = { CTL_KERN, KERN_GLOBAL_PTRACE };
            int ptrace = 0;
            size_t len = sizeof(ptrace);
            if (!sysctl(mib, sizeof(mib)/sizeof(int), &ptrace, &len, NULL, 0) && ptrace == 0) {
                snprintf(linep, linelen, "WARNING: sysctl kern.global_ptrace=%i may be preventing debugger attach\n", ptrace);
                gasneti_rc_unused = write(fd, linebuf, strlen(linebuf));
            }
          }
          #endif
        }
      }

      fclose(file);
    }

    gasneti_reghandler(SIGABRT, old_ABRT);
    gasneti_reghandler(SIGILL,  old_ILL);
    gasneti_reghandler(SIGSEGV, old_SEGV);
    gasneti_reghandler(SIGBUS,  old_BUS);
    gasneti_reghandler(SIGFPE,  old_FPE);
  }
  gasneti_atomic_set(&gasneti_backtrace_enabled,1,GASNETI_ATOMIC_REL);

  return retval;
}


void gasneti_registerSignalHandlers(gasneti_sighandlerfn_t handler) {
  int i;

  char *nocatch =  gasneti_getenv("GASNET_NO_CATCH_SIGNAL");
  int star = ((nocatch != NULL) && (strcmp(nocatch, "*") == 0)) ? 1: 0;

  if ((nocatch != NULL) && !star){
      const char *sep = " ,";
      char *w;
      
      for (w = (char *)strtok(nocatch, sep); w; w = (char *)strtok(NULL, sep)) {
          gasnett_siginfo_t *s;
          
          if ((s = gasnett_siginfo_fromstr(w)) != NULL) {
              GASNETT_TRACE_PRINTF("gasnett leaving signal %s unregistered", s->name);
              s->enable_gasnet_handler = 0;
          } else {
              gasneti_console_message("WARNING","unknown signal %s in GASNET_NO_CATCH_SIGNAL\n", w);  
          }
      }
  }

  if (!star) {
      for (i = 0; i < sizeof(gasneti_sigtable)/sizeof(gasnett_siginfo_t); i++) {
          gasnett_siginfo_t *s = gasneti_sigtable + i; 
          if (s->enable_gasnet_handler)
              s->oldhandler = gasneti_reghandler(s->signum, handler);
      }
  } else {
      GASNETT_TRACE_PRINTF("gasnett not registering any default signal handlers\n");
  }

  gasneti_ondemand_init(); /* allow user override of signal handlers */
}

/* ------------------------------------------------------------------------------------ */
static int _gasneti_print_backtrace_ifenabled(int fd) {
  static int noticeshown = 0;
  if (!gasneti_backtrace_isinit) {
    fprintf(stderr,"WARNING: Ignoring call to gasneti_print_backtrace_ifenabled before gasneti_backtrace_init\n");
    fflush(stderr);
    return -1;
  }
  #if !GASNET_DEBUG
    #define GASNETI_NDEBUG_ADVISORY() do { \
      if (!noticeshown) {                  \
        fprintf(stderr, "NOTICE: We recommend linking the debug version of GASNet to assist you in resolving this application issue.\n"); \
        fflush(stderr);                    \
        noticeshown = 1;                   \
      }                                    \
    } while (0)
  #else
    #define GASNETI_NDEBUG_ADVISORY() ((void)0)
  #endif
#if GASNETI_BUILDING_CONDUIT
  if (gasneti_backtrace_userdisabled) {
    return 1; /* User turned off backtrace, so don't whine */
  } else
#endif
  if (gasneti_backtrace_userenabled) {
    GASNETI_NDEBUG_ADVISORY();
    return gasneti_print_backtrace(fd);
  } else if (gasneti_backtrace_mechanism_count && !noticeshown) {
    fprintf(stderr, "NOTICE: Before reporting bugs, run with GASNET_BACKTRACE=1 in the environment to generate a backtrace. \n");
    fflush(stderr);
    GASNETI_NDEBUG_ADVISORY();
    noticeshown = 1;
    return 1;
  } else {
    GASNETI_NDEBUG_ADVISORY();
    return 1; /* We don't support any backtrace methods, so avoid false advertising. */
  }
}
int (*gasneti_print_backtrace_ifenabled)(int fd) = &_gasneti_print_backtrace_ifenabled;
/* ------------------------------------------------------------------------------------ */
extern uint64_t gasneti_checksum(const void *p, int numbytes) {
 uint8_t const *buf = p;
 uint64_t result = 0;
 int i;
 for (i=0;i<numbytes;i++) {
   result = ((result << 8) | ((result >> 56) & 0xFF) ) ^ *buf;
   buf++;
 }
 return result;
}
/* ------------------------------------------------------------------------------------ */
extern int gasneti_isLittleEndian(void) {
  union {
    int i;                  /* machine word */
    unsigned char b[sizeof(int)];    /* b[0] overlaid with first byte of i */
  } x;
  x.i = 0xFF;    /* set lsb, zero all others */
  return x.b[0] == 0xFF;
}
/* ------------------------------------------------------------------------------------ */
/* build a code-location string */
extern char *gasneti_build_loc_str(const char *funcname, const char *filename, int linenum) {
  int sz;
  char *loc;
  int fnlen;
  if (!funcname) funcname = "";
  if (!filename) filename = "*unknown file*";
  fnlen = strlen(funcname);
  sz = fnlen + strlen(filename) + 20;
  loc = malloc(sz);
  if (*funcname)
    snprintf(loc,sz,"%s%s at %s:%i",
           funcname,
           (fnlen && funcname[fnlen-1] != ')'?"()":""),
           filename, linenum);
  else
    snprintf(loc,sz,"%s:%i", filename, linenum);
  return loc;
}
/* ------------------------------------------------------------------------------------ */
/* number/size formatting */
extern int64_t gasneti_parse_int(const char *str, uint64_t mem_size_multiplier) {
  uint64_t val = 0;
  int base = 10;
  int neg = 0;
  const char *p = str;
  #define GASNETI_NUMBUF_SZ 80
  int isfrac = 0;
  char numbuf[GASNETI_NUMBUF_SZ+1];
  int i = 0;

  if (!str) return 0; /* null returns 0 */

  if (*p == '+') p++; /* check for sign */
  else if (*p == '-') { neg=1; p++; }
  while (*p && isspace(*p)) p++; /* eat spaces */
  if (!*p) return 0; /* empty string returns 0 */
  if (*p == '0' && toupper(*(p+1)) == 'X') { base = 16; p += 2; } /* check for hex */

  while (*p && i < GASNETI_NUMBUF_SZ &&
         ( (isdigit(*p) && *p < ('0'+base)) ||
           (isalpha(*p) && toupper(*p) < ('A'+base-10)) || *p == '.') ) {
    if (isdigit(*p)) { val = (val * base) + (*p - '0'); }
    else if (isalpha(*p)) { val = (val * base) + (10 + toupper(*p) - 'A'); }
    else if (*p == '.') { 
      isfrac = 1; /* user value is a fraction */
      if (base != 10) gasneti_fatalerror("Format error in numerical string: %s", str);
    }
    numbuf[i++] = *p;
    p++;
  }
  numbuf[i] = '\0';
  while (*p && isspace(*p)) p++; /* eat spaces */
  if (mem_size_multiplier) { /* its a mem size, check for provided unit overridder */
    if      (*p == 'T' || *p == 't') mem_size_multiplier = ((uint64_t)1)<<40;
    else if (*p == 'G' || *p == 'g') mem_size_multiplier = ((uint64_t)1)<<30;
    else if (*p == 'M' || *p == 'm') mem_size_multiplier = ((uint64_t)1)<<20;
    else if (*p == 'K' || *p == 'k') mem_size_multiplier = ((uint64_t)1)<<10;
    else if (*p == 'B' || *p == 'b') mem_size_multiplier = 1;
    /* else - default to the context-sensitive mem_size_multiplier of the caller */
  } else {
    mem_size_multiplier = 1;
  }
  if (isfrac) {
    double dval = atof(numbuf);
    val = (uint64_t)(int64_t)(dval*(double)mem_size_multiplier);
  } else {
    val = val * mem_size_multiplier;
  }

  if (neg) return -((int64_t)val);
  return (int64_t)val;
  #undef GASNETI_NUMBUF_SZ
}

extern char *gasneti_format_number(int64_t val, char *buf, size_t bufsz, int is_mem_size) {
  const char *unit = "";
  const char *neg = "";
  int64_t divisor = 1;
  if (val < 0) { val = -val; neg = "-"; }
  if (val >= ((int64_t)1) << 50) divisor = -1; /* use hex for huge vals */
  else if (is_mem_size) {
    /* Try to strike a compromise between digits and round off */
    #define GASNETI_USE_DIV(div, unit_str)                           \
      if ((val >= 10*(div)) || ((val >= (div)) && !(val % (div)))) { \
        divisor = (div); unit = (unit_str); break;                   \
      }
    do {
      GASNETI_USE_DIV(((int64_t)1) << 40, " TB");
      GASNETI_USE_DIV(((int64_t)1) << 30, " GB");
      GASNETI_USE_DIV(((int64_t)1) << 20, " MB");
      GASNETI_USE_DIV(((int64_t)1) << 10, " KB");
      GASNETI_USE_DIV(((int64_t)1), " B");
    } while (0);
    #undef GASNETI_USE_DIV
  } 

  if (divisor > 0) {
    snprintf(buf, bufsz, "%s%"PRId64"%s", neg, (val/divisor), unit);
  } else if (divisor == -1) {
    if (*neg) val = -val;
    snprintf(buf, bufsz, "0x%"PRIx64, val);
  } else gasneti_fatalerror("internal error in gasneti_format_number");
  return buf;
}

// Parses a floating-point number or fraction, with result by reference.
// Errors return non-zero (and result possibly modified)
// NULL is not an error, and yields 0.
extern int gasneti_parse_dbl(const char *str, double *result_p) {
  double result = 0.;
  if (str != NULL) {
    char *endptr;
    result = strtod(str, &endptr);
    if (endptr != str) {
      while (*endptr && isspace(*endptr)) endptr++; /* Skip whitespace */
      if (*endptr == '/') {
        char *endptr2;
        double denom = strtod(1+endptr, &endptr2);
        if ((denom != 0) && (endptr2 != (1+endptr))) {
          for (endptr = endptr2; *endptr && isspace(*endptr); endptr++) {/* Skip whitespace */}
          result /= denom;
        } else {
          /* endptr is left pointing at '/', triggering rejection below */
        }
      }
    }
    if ((endptr == str) || (*endptr != '\0')) { /* match was empty or has trailing non-whitespace */
      return 1;
    }
  }
  *result_p = result;
  return 0;
}
/* ------------------------------------------------------------------------------------ */
/* environment support */
#if HAVE_SETENV && !HAVE_SETENV_DECL
  extern int setenv(const char *, const char *, int);
#endif
#if HAVE_UNSETENV && !HAVE_UNSETENV_DECL
  extern int unsetenv(const char *);
#endif
/* set an environment variable, for the local process ONLY */
extern void gasneti_setenv(const char *key, const char *value) {
  /* both are POSIX - prefer setenv because it manages memory for us */
  #if HAVE_SETENV
    int retval = setenv((char *)key, (char *)value, 1);
    if (retval) gasneti_fatalerror("Failed to setenv(\"%s\",\"%s\",1) in gasneti_setenv => %s(%i)",
                                     key, value, strerror(errno), errno);
  #elif HAVE_PUTENV 
    char *tmp = malloc(strlen(key) + strlen(value) + 2);
    int retval;
    strcpy(tmp, key);
    strcat(tmp, "=");
    strcat(tmp, value);
    retval = putenv(tmp);
    if (retval) gasneti_fatalerror("Failed to putenv(\"%s\") in gasneti_setenv => %s(%i)",
                                     tmp, strerror(errno), errno);
  #else
    gasneti_fatalerror("Got a call to gasneti_setenv, but don't know how to do that on your system");
  #endif
}

/* unset an environment variable, for the local process ONLY */
extern void gasneti_unsetenv(const char *key) {
  /* prefer unsetenv because it's documented to remove env vars */
  #if HAVE_UNSETENV
   #if 0
    /* bug 1135: POSIX requires unsetenv to return int, but several OS's (at least Linux and BSD)
                 are non-compliant and return void. It's not worth our trouble to detect
                 this (since the possible errors are few) so ignore the return value */
    int retval = unsetenv(key);
    if (!retval) gasneti_fatalerror("Failed to unsetenv(\"%s\") in gasneti_unsetenv => %s(%i)",
                                     key, strerror(errno), errno);
   #else
    /* check for a few error cases ourselves */
    if (!key || strlen(key)==0 || strchr(key,'=')) 
       gasneti_fatalerror("Bad key (\"%s\") passed to gasneti_unsetenv",key);
    unsetenv((char *)key);
   #endif
  #elif HAVE_PUTENV
    /* this relies on undocumented putenv behavior, and may or may not work */
    char *tmp = malloc(strlen(key) + 2);
    int retval;
    strcpy(tmp, key);
    strcat(tmp, "=");
    retval = putenv(tmp);
    if (retval) gasneti_fatalerror("Failed to putenv(\"%s\") in gasneti_unsetenv => %s(%i)",
                                     key, strerror(errno), errno);
  #else
    gasneti_fatalerror("Got a call to gasneti_unsetenv, but don't know how to do that on your system");
  #endif
}
/* ------------------------------------------------------------------------------------ */
#if GASNETI_BUILDING_CONDUIT
extern const char * gasneti_decode_envval(const char *);
extern int (*gasneti_verboseenv_fn)(void);
#endif
gasneti_getenv_fn_t *gasneti_getenv_hook = NULL;
char *gasneti_globalEnv = NULL;

// Internal function which will not trace or call envdecode hook (if any)
static char *gasneti_getenv_early(const char *keyname) {
  char *retval = NULL;

  if (keyname && gasneti_getenv_hook) {
    /* highest priority given to spawner- or conduit-specific getenv */
    retval = (*gasneti_getenv_hook)(keyname);
  }

  if (keyname && !retval && gasneti_globalEnv) { 
    /* global environment takes precedence 
     * (callers who want the local environment can call getenv directly)
     */
    char *p = gasneti_globalEnv;
    int keylen = strlen(keyname);
    while (*p) {
      if (!strncmp(keyname, p, keylen) && p[keylen] == '=') {
        retval = p + keylen + 1;
        break;
      }
      p += strlen(p) + 1;
    }
  }

  if (keyname && !retval) /* try local environment */
    retval = getenv(keyname);
  
  return retval;
}

extern char *gasneti_getenv(const char *keyname) {
  char *retval = gasneti_getenv_early(keyname);

  #if GASNETI_BUILDING_CONDUIT
  if (retval && /* check if environment value needs decoding */
      strcmp(keyname, "GASNET_DISABLE_ENVDECODE") &&
      strcmp(keyname, "GASNET_VERBOSEENV")) { /* prevent inf recursion */ 
    retval = (char *)gasneti_decode_envval(retval);
  }
  #endif

  GASNETT_TRACE_PRINTF("gasnet_getenv(%s) => '%s'",
                          (keyname?keyname:"NULL"),(retval?retval:"NULL"));

  return retval;
}

/* indicate whether GASNET_VERBOSEENV reporting is enabled on this node 
   1 = yes, 0 = no, -1 = not yet / don't know
*/
extern int gasneti_verboseenv(void) {
#if GASNETI_BUILDING_CONDUIT
  if (gasneti_verboseenv_fn) return (*gasneti_verboseenv_fn)();
  else 
#endif
    return !!gasneti_getenv("GASNET_VERBOSEENV");
}

typedef struct gasneti_verboseenv_S {
  struct gasneti_verboseenv_S *next;
  const char *key;
  const char *displaystr;
} gasneti_verboseenv_t;

/* display an integral/string/double environment setting iff gasneti_verboseenv() */
extern void gasneti_envstr_display(const char *key, const char *val, int is_dflt) {
  const char *dflt = (is_dflt?"   (default)":"");
  const char *displayval = val;
  int verbose = gasneti_verboseenv();
  if (!val) displayval = "*not set*";
  else if (strlen(val) == 0) displayval = "*empty*";
  GASNETT_TRACE_PRINTF("ENV parameter: %s = %s%s", key, displayval, dflt);
  if (verbose) {
    static gasneti_mutex_t envmutex = GASNETI_MUTEX_INITIALIZER;
    static gasneti_verboseenv_t *displaylist = NULL;
    static gasneti_verboseenv_t *displaylist_tail = NULL;
    static int notyet = 1;
    gasneti_verboseenv_t *p;
    char tmpstr[255];
    char *displaystr = tmpstr;
    int width = MAX(10,55 - strlen(key) - strlen(displayval));
    int len = snprintf(tmpstr, sizeof(tmpstr), "ENV parameter: %s = %s%*s\n", key, displayval, width, dflt);
    if (len >= sizeof(tmpstr)) { /* Too long for the static buffer */
      displaystr = malloc(len + 1);
      snprintf(displaystr, len+1, "ENV parameter: %s = %s%*s\n", key, displayval, width, dflt);
    }
    gasneti_mutex_lock(&envmutex);
      for (p = displaylist; p; p = p->next) { /* check for previous report */
        if (!strcmp(key,p->key)) break;
      }
      if (!p) { /* new entry */
        p = malloc(sizeof(gasneti_verboseenv_t));
        p->key = strdup(key);
        if (verbose > 0 && !notyet) { /* display now */
          p->displaystr = NULL;
          fputs(displaystr, stderr);
          fflush(stderr);
        } else { /* cache for later */
          p->displaystr = strdup(displaystr);
        }
        if (!displaylist) displaylist = p;
        if (displaylist_tail) displaylist_tail->next = p;
        displaylist_tail = p;
        p->next = NULL;
      }
      if (notyet && verbose > 0) { /* dump cached values */ 
        for (p = displaylist; p; p = p->next) {
          fputs(displaystr, stderr);
          fflush(stderr);
          free((void *)p->displaystr);
          p->displaystr = NULL;
        }
        notyet = 0;
      }
    gasneti_mutex_unlock(&envmutex);
    if (displaystr != tmpstr) free(displaystr);
  }
}
extern void gasneti_envdbl_display(const char *key, double val, int is_dflt) {
  char valstr[80];
  char displayval[80];
  if (!gasneti_verboseenv() && !GASNETT_TRACE_ENABLED) return;

  snprintf(valstr, sizeof(valstr), "%g", val);
  const char * const rawval = gasneti_getenv(key);
  gasneti_assert(is_dflt || rawval);

  if (is_dflt || !strcmp(rawval,valstr)) { /* Use the numerical value */
    strcpy(displayval, valstr);
  } else { /* Use both the environment string and numerical value when they differ textually */
    snprintf(displayval, sizeof(displayval), "%s (%s)", rawval, valstr);
  }
  gasneti_envstr_display(key, displayval, is_dflt);
}
extern void gasneti_envint_display(const char *key, int64_t val, int is_dflt, int is_mem_size) {
  char valstr[80];
  char displayval[80];
  if (!gasneti_verboseenv() && !GASNETT_TRACE_ENABLED) return;

  gasneti_format_number(val, valstr, 80, is_mem_size);
  const char * const rawval = gasneti_getenv(key);
  gasneti_assert(is_dflt || rawval);

  if (is_dflt) { /* Use the numerical value */
    strcpy(displayval, valstr);
  } else if (!strcmp(rawval,valstr)) {
    strcpy(displayval, valstr);
  } else { /* Use the environment string and numerical value */
    snprintf(displayval, sizeof(displayval), "%s (%s)", rawval, valstr);
  }
  gasneti_envstr_display(key, displayval, is_dflt);
}

static char *_gasneti_getenv_withdefault(const char *keyname, const char *defaultval, int valmode, int64_t *val) {
  const char * retval = NULL;
  int is_dflt = 0;
  retval = gasneti_getenv(keyname);
  if (retval == NULL) { retval = defaultval; is_dflt = 1; }

  if (valmode == 0) {
    /* just a string value */
    gasneti_envstr_display(keyname, retval, is_dflt);
  } else if (valmode == 1) { /* yes/no value */
    char s[10];
    int i;
    strncpy(s, retval, sizeof(s)-1); s[sizeof(s)-1] = '\0';
    for (i = 0; i < sizeof(s); i++) s[i] = toupper(s[i]);
    if (!strcmp(s, "N") || !strcmp(s, "NO") || !strcmp(s, "0")) retval = "NO";
    else if (!strcmp(s, "Y") || !strcmp(s, "YES") || !strcmp(s, "1")) retval = "YES";
    else gasneti_fatalerror("If used, environment variable '%s' must be set to 'Y|YES|y|yes|1' or 'N|n|NO|no|0'", keyname);
    gasneti_envstr_display(keyname, retval, is_dflt);
  } else if (valmode == 2 || valmode == 3) { /* int value, regular or memsize */
    int is_mem_size = (valmode == 3);
    gasneti_assert(val);
    *val = gasneti_parse_int(retval, *val);
    gasneti_envint_display(keyname, *val, is_dflt, is_mem_size);
  } else gasneti_fatalerror("internal error in _gasneti_getenv_withdefault");

  return (char *)retval;
}
extern char *gasneti_getenv_withdefault(const char *keyname, const char *defaultval) {
  return _gasneti_getenv_withdefault(keyname, defaultval, 0, NULL);
}
extern int gasneti_getenv_yesno_withdefault(const char *keyname, int defaultval) {
  return !strcmp(_gasneti_getenv_withdefault(keyname, (defaultval?"YES":"NO"), 1, NULL), "YES");
}
extern int64_t gasneti_getenv_int_withdefault(const char *keyname, int64_t defaultval, uint64_t mem_size_multiplier) {
  int64_t val = mem_size_multiplier;
  char defstr[80];
  gasneti_format_number(defaultval, defstr, 80, mem_size_multiplier);
  _gasneti_getenv_withdefault(keyname, defstr, (mem_size_multiplier?3:2), &val);
  return val;
}
extern double gasneti_getenv_dbl_withdefault(const char *keyname, double defaultval) {
  double retval = defaultval;
  int is_dflt = 1;
  const char * envval = gasneti_getenv(keyname);

  if (envval != NULL) {
    is_dflt = 0;
    if (gasneti_parse_dbl(envval, &retval)) {
      gasneti_fatalerror("If used, environment variable '%s' must be a valid floating point value or fraction", keyname);
    }
  }

  gasneti_envdbl_display(keyname, retval, is_dflt);
  return retval;
}

// Parse an environment variable as a memory size as follows:
//  + If parses as double between 0. and 1., multiply by "fraction_of".
//  + If parses as integer (w/ optional suffix) take as an absolute size.
// if pph is non-zero, accept [pPhH] suffixes and for [hH] divide by this value.
// The value is then aligned up to PAGESIZE.
// overhead_per_p is added to the resulting value, unless there was an [hH] suffix.
// Final value is silently rounded down to maximum (if it's nonzero)
// and dies if the final result is below "minimum" (which should include overhead, if any).
// The result is always page aligned.
// 32-BIT: The return value is always less than 4GB
extern uint64_t gasneti_getenv_memsize_withdefault(const char *key, const char *dflt, 
                                                   uint64_t minimum, uint64_t maximum,
                                                   uint64_t fraction_of, uint64_t pph,
                                                   uint64_t overhead_per_p) {
  const char *input = gasneti_getenv(key);
  int using_default = (NULL == input);
  if (using_default) input = dflt;
  const char *str = input;

  // Parse and remove (/?[phPH]) suffix
  char tmp[255];
  int got_h = 0;
  if (pph) {
    size_t len = strlen(str);
    gasneti_assert_uint(sizeof(tmp) ,>, len);
    strncpy(tmp,str,sizeof(tmp));
    char *p = &tmp[len-1];
    while (p >= tmp && isspace(*p)) *(p--) = 0; // strip end whitespace
    switch (*p) {
      case 'h': case 'H': 
        got_h = 1; GASNETI_FALLTHROUGH
      case 'p': case 'P':
        *(p--) = 0; // strip
        while (p >= tmp && isspace(*p)) *(p--) = 0; // strip end whitespace
        if (p >= tmp && *p == '/') *(p--) = 0; // strip
    }
    str = tmp;
  }

  double dbl;
  int64_t val;
  int is_fraction = 0;
  if (0 == gasneti_parse_dbl(str, &dbl)) {
    if ((dbl > 0.) && (dbl < 1.)) {
      is_fraction = 1;
      val = dbl * fraction_of;
    } else {
      val = dbl;
    }
  } else {
    // Note: default suffix is irrelevant since un-suffixed case was parsed as a double
    val = gasneti_parse_int(str, 1);
  }

  // check sign before ALIGNDOWN
  if (val < 0) {
    gasneti_fatalerror("%s='%s' is negative.", key, input);
  }

  if (got_h) {
    gasneti_assert(pph > 0);
    val = val / pph;
  }

  if (val == 0 && minimum > 0 && 
     (got_h || minimum > GASNETI_PAGE_ALIGNUP(overhead_per_p))) {
    gasneti_fatalerror("%s='%s' is zero or unrecognized.", key, input);
  }

  // from here on we operate on page granularity
  // max is enforced throughout because the GASNETI_PAGE_ALIGN* macros operate on pointer-width
  // 32-bit builds currently limit value to 4GB - PAGESIZE
  uint64_t maxrep = GASNETI_PAGE_ALIGNDOWN((uintptr_t)-1);
  if (!maximum) maximum = maxrep;
  else {
    maximum = MIN(maximum,maxrep);
    maximum = GASNETI_PAGE_ALIGNDOWN(maximum);
  }
  val = MIN(val, maximum);
  val = GASNETI_PAGE_ALIGNUP(val);

  // display parsed/aligned input value
  GASNETT_TRACE_PRINTF("%s='%s' yields %"PRId64, key, input, val);
  gasneti_envint_display(key, val, using_default, 1);

  // add overhead
  if (overhead_per_p && !got_h) {
    overhead_per_p = GASNETI_PAGE_ALIGNUP(overhead_per_p);
    val += overhead_per_p;
    val = MIN(val, maximum);
  }

  gasneti_assert_uint(val ,==, GASNETI_PAGE_ALIGNDOWN(val));
  gasneti_assert_uint(val ,<=, maxrep);
  gasneti_assert_uint(val ,<=, maximum);
  GASNETT_TRACE_PRINTF("%s='%s' final value: %"PRId64, key, input, val);

  if (val < minimum) {
    const char *parsed_as = is_fraction ? "a fraction" : "an amount";
    char min_display[16];
    char val_display[16];
    gasneti_format_number(minimum, min_display, sizeof(min_display), 1);
    gasneti_format_number(val,     val_display, sizeof(val_display), 1);
    char pph_display[255] = {0};
    if (got_h && pph > 1) {
      snprintf(pph_display, sizeof(pph_display), " (split across %d processes on host %s)",
               (int)pph, gasneti_gethostname());
    }
    char overhead_display[80] = {0};
    if (overhead_per_p) {
      strncpy(overhead_display, ", including overhead of ", sizeof(overhead_display));
      size_t len = strlen(overhead_display);
      gasneti_format_number(overhead_per_p, overhead_display+len, sizeof(overhead_display)-len, 1);
    }
    gasneti_fatalerror(
            "Parsing '%s' as %s of memory%s yields %s of %"PRId64" (%s)%s, "
            "which is less than the minimum supported value of %s%s.",
            input, parsed_as, pph_display,
            key, val, val_display, 
            (got_h?"":overhead_display),
            min_display,
            (got_h?overhead_display:"")
            );
  }

  return (uint64_t) val;
}

static int _gasneti_tmpdir_valid(const char *dir) {
  struct stat s;
  /* non-empty */
  if (!dir || !strlen(dir)) return 0;
  /* an absolute path */
  if (dir[0] != '/') return 0;
  /* an existing directory (stat follows symlinks) */
  if (stat(dir, &s) || !S_ISDIR(s.st_mode)) return 0;
  /* allow us to search and write */
  if (access(dir, (X_OK | W_OK))) return 0;
  return 1;
}
extern const char *gasneti_tmpdir(void) {
  static const char slash_tmp[] = "/tmp";
  static const char *result = NULL;
  const char *tmpdir;

  if_pt (result) return result;

  if (_gasneti_tmpdir_valid(tmpdir = gasneti_getenv_withdefault("GASNET_TMPDIR", NULL))) {
    result = tmpdir;
  } else if (_gasneti_tmpdir_valid(tmpdir = gasneti_getenv_withdefault("TMPDIR", NULL))) {
    result = tmpdir;
  } else if (_gasneti_tmpdir_valid(slash_tmp)) {
    result = slash_tmp;
  }

  return result;
}

/* ------------------------------------------------------------------------------------ */
/* Resource limit control */

int gasnett_maximize_rlimits(void) {
   int success = 1;
   struct res_s { int res; const char *desc; } res[] = {
    #ifdef RLIMIT_CPU
      { RLIMIT_CPU, "RLIMIT_CPU" },
    #endif
    #ifdef RLIMIT_DATA
      { RLIMIT_DATA, "RLIMIT_DATA" },
    #endif
    #ifdef RLIMIT_RSS
      { RLIMIT_RSS, "RLIMIT_RSS" },
    #endif
    #ifdef RLIMIT_STACK
      { RLIMIT_STACK, "RLIMIT_STACK" },
    #endif
    #ifdef RLIMIT_AS
      { RLIMIT_AS, "RLIMIT_AS" },
    #endif
  };
  size_t idx; 
  for (idx = 0; idx < sizeof(res)/sizeof(struct res_s); idx++) {
    success &= gasnett_maximize_rlimit(res[idx].res, res[idx].desc);
  }
  return success;
}

int gasnett_maximize_rlimit(int res, const char *lim_desc) {
  int success = 0;

  char ctrl_var[32] = "GASNET_MAXIMIZE_";
  gasneti_assert_uint(strlen(ctrl_var) + strlen(lim_desc) ,<, sizeof(ctrl_var));
  if (!gasneti_getenv_yesno_withdefault(strncat(ctrl_var, lim_desc, sizeof(ctrl_var)-1), 1))
    return 1;

  /* function pointers used in this macro are a workaround an annoying glibc header bug,
   * which erroneously declares get/setrlimit to take 
   * the enum type __rlimit_resource_t, instead of int as required by POSIX */
  #define SET_RLIMITS(structname, getrlimit, setrlimit) do {                                    \
    static int (*get_fp)(int,structname *) = (int (*)(int,structname *))&getrlimit;             \
    static int (*set_fp)(int,const structname *) = (int (*)(int,const structname *))&setrlimit; \
    structname oldval,newval;                                                                   \
    if (get_fp(res, &oldval)) {                                                                 \
      GASNETT_TRACE_PRINTF("gasnett_maximize_rlimit: "#getrlimit"(%s) failed: %s",              \
                              lim_desc, strerror(errno));                                       \
    } else {                                                                                    \
      char newvalstr[128];                                                                      \
      newval = oldval;                                                                          \
      if (newval.rlim_cur == RLIM_INFINITY ||                                                   \
        newval.rlim_max == RLIM_INFINITY) {                                                     \
        newval.rlim_cur = RLIM_INFINITY;                                                        \
        strncpy(newvalstr, "RLIM_INFINITY", sizeof(newvalstr));                                 \
      } else {                                                                                  \
        gasneti_assert_uint(newval.rlim_cur ,<=, newval.rlim_max);                              \
        newval.rlim_cur = newval.rlim_max;                                                      \
        snprintf(newvalstr, sizeof(newvalstr), "%"PRIu64, (uint64_t)newval.rlim_cur);           \
      }                                                                                         \
      if (newval.rlim_cur != oldval.rlim_cur) {                                                 \
        if (set_fp(res, &newval)) {                                                             \
          GASNETT_TRACE_PRINTF("gasnett_maximize_rlimit: "#setrlimit"(%s, %s) failed: %s",      \
                               lim_desc, newvalstr, strerror(errno));                           \
        } else {                                                                                \
          GASNETT_TRACE_PRINTF("gasnett_maximize_rlimit: "#setrlimit"(%s, %s)"                  \
                               " raised limit from %"PRIu64,                                    \
                               lim_desc, newvalstr, (uint64_t)oldval.rlim_cur);                 \
          success = 1;                                                                          \
        }                                                                                       \
      }                                                                                         \
    }                                                                                           \
  } while (0)
  #if defined(HAVE_GETRLIMIT) && defined(HAVE_SETRLIMIT)
    SET_RLIMITS(struct rlimit, getrlimit, setrlimit);
  #endif
  /* do 64-bit second, to favor the potentially higher limits */
  #if defined(HAVE_GETRLIMIT64) && defined(HAVE_SETRLIMIT64)
    SET_RLIMITS(struct rlimit64, getrlimit64, setrlimit64);
  #endif
  return success;
}

/* ------------------------------------------------------------------------------------ */
/* Physical CPU query */
#if PLATFORM_OS_DARWIN || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
#include <sys/param.h>
#include <sys/sysctl.h>
#endif
/* return the physical count of CPU's on this node, 
   or zero if that cannot be determined */
extern int gasneti_cpu_count(void) {
  static int hwprocs = -1;
  if (hwprocs >= 0) return hwprocs;

  #if PLATFORM_OS_DARWIN || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
      {
        int mib[2];
        size_t len;

        mib[0] = CTL_HW;
        mib[1] = HW_NCPU;
        len = sizeof(hwprocs);
        gasneti_assert_zeroret(sysctl(mib, 2, &hwprocs, &len, NULL, 0));
        if (hwprocs < 1) hwprocs = 0;
      }
  #else
      hwprocs = sysconf(_SC_NPROCESSORS_ONLN);
      if (hwprocs < 1) hwprocs = 0; /* catch failures on Solaris/Cygwin */
  #endif

  gasneti_assert_always(hwprocs >= 0);
  return hwprocs;
}

/* ------------------------------------------------------------------------------------ */
/* Physical memory query */
#ifdef _SC_PHYS_PAGES
  /* if the sysconf exists, try to use it */
  static uint64_t _gasneti_getPhysMemSysconf(void) {
    long pages = sysconf(_SC_PHYS_PAGES);
    if (pages < 0) pages = 0;
    return (((uint64_t)pages)*GASNET_PAGESIZE);
  }
#else
  #define _gasneti_getPhysMemSysconf() 0
#endif
#if PLATFORM_OS_DARWIN || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
  #include <sys/types.h>
  #include <sys/sysctl.h>
#endif
extern uint64_t gasneti_getPhysMemSz(int failureIsFatal) {
  uint64_t retval = _gasneti_getPhysMemSysconf();
  if (retval) return retval;
  #if PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX || PLATFORM_OS_WSL
    #define _BUFSZ        120
    { FILE *fp;
      char line[_BUFSZ+1];

      if ((fp = fopen("/proc/meminfo", "r")) == NULL)
        gasneti_fatalerror("Failed to open /proc/meminfo in gasneti_getPhysMemSz()");

      while (fgets(line, _BUFSZ, fp)) {
        uint64_t memval = 0;
        /* MemTotal: on 2.4 and 2.6 kernels - preferred because less chance of scanf overflow */
        if (sscanf(line, "MemTotal: %"SCNu64" kB", &memval) > 0 && memval > 0) {
          retval = memval * 1024;
        }
        /* Mem: only on 2.4 kernels */
        else if (sscanf(line, "Mem: %"SCNu64, &memval) > 0 && memval > 0 && !retval) {
          retval = memval;
        }
      }
      fclose(fp);
    }
    #undef _BUFSZ
  #elif PLATFORM_OS_DARWIN || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD
    { /* see "man 3 sysctl" */    
      int mib[2];
      size_t len = 0;
      mib[0] = CTL_HW;
      mib[1] = HW_PHYSMEM;
      sysctl(mib, 2, NULL, &len, NULL, 0);
      switch (len) { /* accomodate both 32 and 64-bit systems */
        case 4: { 
          uint32_t retval32 = 0;
          if (sysctl(mib, 2, &retval32, &len, NULL, 0)) 
            gasneti_fatalerror("sysctl(CTL_HW.HW_PHYSMEM) failed: %s(%i)",strerror(errno),errno);
          if (retval32) retval = (uint64_t)retval32;
          break;
        }
        case 8:
          if (sysctl(mib, 2, &retval, &len, NULL, 0)) 
            gasneti_fatalerror("sysctl(CTL_HW.HW_PHYSMEM) failed: %s(%i)",strerror(errno),errno);
          break;
        default:
          gasneti_fatalerror("sysctl(CTL_HW.HW_PHYSMEM) failed to get required size, got len=%i: %s(%i)",
            (int)len, strerror(errno), errno);
      }
    }
  #else  /* unknown OS */
    { }
  #endif

  if (!retval && failureIsFatal) 
    gasneti_fatalerror("Failed to determine physical memory size in gasneti_getPhysMemSz()");
  return retval;
}
/* ------------------------------------------------------------------------------------ */
/* CPU affinity control */
#if HAVE_PLPA
  #include "plpa.h"
#elif PLATFORM_OS_SOLARIS
  #include <sys/types.h>
  #include <sys/processor.h>
  #include <sys/procset.h>
#endif
static int gasneti_set_affinity_cpus(void) {
    int cpus = gasneti_cpu_count();
    if_pf (cpus == 0) {
      static int once = 1;
      if (once) {
	once = 0;
        fprintf(stderr, "WARNING: gasnett_set_affinity called, but cannot determine cpu count.\n");
        fflush(stderr);
      }
    }
    return cpus;
}
#if PLATFORM_OS_LINUX || PLATFORM_OS_WSL
// return non-zero iff this Linux system is actually Microsoft Windows Subsystem for Linux
extern int gasneti_platform_isWSL(void) {
    // Ideally we would use uname(2) here, but direct experimentation on the 4/16/17 version
    // of WSL reveals that uname does not return any identifying marks to distinguish Microsoft's
    // emulated Ubuntu kernel. 
    // Microsoft devs suggest grepping /proc/version or /proc/sys/kernel/osrelease for "Microsoft"
    int fd = open("/proc/sys/kernel/osrelease", O_RDONLY);
    if (fd < 0) return 0;

    static char line[255];
    line[0] = 0;
    ssize_t rc = read(fd, line, sizeof(line));
    close(fd);
    if (rc > 0 && strstr(line, "Microsoft")) return 1;
    else return 0;
}
#endif
//
// NOTE: when adding implementations here, update the #define of
// GASNETT_SET_AFFINITY_SUPPORT in gasnet_toolhelp.h to match.
//
int gasneti_set_affinity_default(int rank) {
  #if HAVE_PLPA
  {
    static int fails = 0;
    gasneti_plpa_cpu_set_t mask;
    int cpus = gasneti_set_affinity_cpus();

    if (cpus == 1) {
      /* NO-OP (success) on single-processor platform */
      return 0;
    }

    if (fails == 1) {
      /* NO-OP (failure) as determined by an earlier call */
      return 1;
    }

    // Dynamically handle binaries built on native Ubuntu and ported to Microsoft's WSL kernel
    // emulator, which currently fail inside plpa_sched_setaffinity with EINVAL.
  #if PLATFORM_OS_LINUX || PLATFORM_OS_WSL
    if (gasneti_platform_isWSL()) {
        /* NO-OP on WSL */
        fails = 1;
        return 1;
    }
  #endif
    
    /* Try a GET first to check for support */
    if_pf (ENOSYS == gasneti_plpa_sched_getaffinity(0, sizeof(mask), &mask)) {
      /* becomes a NO-OP on next call*/
      fails = 1;
      return 1;
    }
    
    {
      int local_rank = rank % cpus;
      PLPA_CPU_ZERO(&mask);
      PLPA_CPU_SET(local_rank, &mask);
      return gasneti_plpa_sched_setaffinity(0, sizeof(mask), &mask);
    }
  }
  #elif PLATFORM_OS_SOLARIS
  {
    static processorid_t *avail_cpus = NULL;
    static int num_cpus = 0;

    if_pf (!num_cpus) {
      static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;

      gasneti_mutex_lock(&lock);
      if (!num_cpus) {
        processorid_t i;
        int tmp_cpus;
        int j;

        /* Find max possible CPU ID */
        processorid_t cpuid_max = sysconf(_SC_CPUID_MAX);

        /* Find the number of online CPUS. */
        tmp_cpus = gasneti_set_affinity_cpus();

        /* Allocate avail. CPU table. */
        avail_cpus = (processorid_t *) malloc(tmp_cpus * sizeof(processorid_t));

        /* Init avail. CPU table */
        for (i = j = 0; i <= cpuid_max; i++) {
            if (p_online(i, P_STATUS) != -1)
              avail_cpus[j++] = i;
        }

        /* Ensure avail_cpus written/populated before num_cpus is written */
        gasneti_sync_writes();
        num_cpus = tmp_cpus;
      }
      gasneti_mutex_unlock(&lock);
    } else {
      gasneti_sync_reads();
    }

    /* From the processor_bind man page:
     * P_LWPID: the binding affects the LWP of the
     *           current process with LWP ID id.
     * P_MYID: the specified LWP, process, task, or  
     *          process is the current one.
     */
    {
      int local_rank = rank % num_cpus;
      return processor_bind(P_LWPID, P_MYID, avail_cpus[local_rank], NULL);
    }
  }
  #elif defined(GASNETT_SET_AFFINITY_SUPPORT)
    #error "GASNETT_SET_AFFINITY_SUPPORT defined, but no implementation is reachable."
  #else
    /* No implementation -> NO-OP */
    return 1;
  #endif
}
int gasneti_set_affinity(int rank) {
  GASNETT_TRACE_PRINTF("gasnett_set_affinity(%d)", rank);
  return gasneti_set_affinity_default(rank);
}
/* ------------------------------------------------------------------------------------ */
/* Count zero bytes in a region w/ or w/o a memcpy() */
/* These implementations use full-word reads and writes where possible */

#if PLATFORM_ARCH_64
  #define gasneti_count0s_word_shift 3 /* multiply or divide by 8 */
#else
  #define gasneti_count0s_word_shift 2 /* multiply or divide by 4 */
#endif

/* gasneti_count0s_xform_limit determines how many iterations of xform1 may
 * be performed before applying xform2 to reduce the partial results.  For
 * "Algorithm D" in xform2, the limit is the potential overflow of the 8-bit
 * fields after 255 iterations.
 * If one were to switch to Algorithm B or C, then the limit is the possible
 * overflow in the summation of xform2, and one should use the word_shift
 * dependent limit value.
 */
#if 1
  #define gasneti_count0s_xform_limit 255
#else
  #define gasneti_count0s_xform_limit ((256 >> gasneti_count0s_word_shift) - 1)
#endif

/* Given a word, set the least-significant bit of each non-zero byte, zeroing all other bits */
GASNETI_INLINE(gasneti_count0s_xform1) GASNETI_CONST
uintptr_t gasneti_count0s_xform1(uintptr_t x) {
#if 0 /* Original shift-based method */
  x |= (x >> 4);
  x |= (x >> 2);
  x |= (x >> 1);
  #if PLATFORM_ARCH_64
    return (x & 0x0101010101010101UL);
  #else
    return (x & 0x01010101UL);
  #endif
#else
  /* Variants of the following algorithm can be found many places on the internet.
   * One such is http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
   */
  #if PLATFORM_ARCH_64
    const uint64_t mask = 0x7f7f7f7f7f7f7f7fUL;
    uint64_t tmp;
  #else
    const uint32_t mask = 0x7f7f7f7fUL;
    uint32_t tmp;
  #endif
  tmp = x & mask;
  tmp += mask;
  tmp |= x;
  tmp &= ~mask;
  return tmp >> 7;
#endif
}

/* Given a sum of words generated by no more than gasneti_count0s_xform_limit iterations
 * of xform1, sum the least-significant bits of the bytes into a single value. */
GASNETI_INLINE(gasneti_count0s_xform2) GASNETI_CONST
size_t gasneti_count0s_xform2(uintptr_t x) {
#if 0
  /* Algorithm A:
   * simplest expression, but usually a poor performer
   */
  return x % 255;
#elif 0
  /* Algorithm B:
   * Form that is equvalent to Algorithm C, and is said to be faster
   * IFF one has a fast integer multiply instruction.
   * Works because the product is x + (x<<8) + (x<<16) + (x<<24) + ...
   * and thus our sum is in the UPPER 8 bits.
   * See, for instance, http://en.wikipedia.org/wiki/Hamming_weight
   * This form will overflow if the sum of xform1-results
   * includes more than 255 counts.
   */
 #if PLATFORM_ARCH_64
  return (x * 0x0101010101010101UL) >> 56;
 #else
  return (x * 0x01010101UL) >> 24;
 #endif
#elif 0
  /* Algorithm C:
   * A "tree" reduction of 4 or 8 "fields" of 8-bits each.
   * This form will overflow if the sum of xform1-results
   * includes more than 255 counts.
   */
 #if PLATFORM_ARCH_64
  x += (x >> 32);
 #endif
  x += (x >> 16);
  x += (x >> 8);
  return (x & 0xff);
#else
  /* Algorithm D:
   * A "tree" reduction of 4 or 8 "fields" of 8-bits each.
   * This form won't overflow regardless of final sum and requires
   * only 2 more operations (both ANDs) than Algorithm C while
   * raising gasneti_count0s_xform_limit (almost) by a factor
   * of either 4 or 8.
   */
 #if PLATFORM_ARCH_64
  const uintptr_t mask = 0x00ff00ff00ff00ffUL;
 #else
  const uintptr_t mask = 0x00ff00ffUL;
 #endif
  x = (x & mask) + ((x >> 8) & mask);
  x += (x >> 16);
 #if PLATFORM_ARCH_64
  x += (x >> 32);
 #endif
  return (x & 0x7ff);
#endif
}

/* Count non-zero bytes in a word-aligned region */
GASNETI_INLINE(gasneti_count0s_nzs_aligned_region) GASNETI_PURE
size_t gasneti_count0s_nzs_aligned_region(const uintptr_t *p, size_t words) {
  size_t non_zeros = 0;
  int i;

  while (words > gasneti_count0s_xform_limit) {
    uintptr_t partial = 0;
    for (i = 0; i < gasneti_count0s_xform_limit; ++i) {
      partial += gasneti_count0s_xform1(*(p++));
    }
    non_zeros += gasneti_count0s_xform2(partial);
    words -= gasneti_count0s_xform_limit;
  }
  {
    uintptr_t partial = 0;
    for (i = 0; i < words; ++i) {
      partial += gasneti_count0s_xform1(*(p++));
    }
    non_zeros += gasneti_count0s_xform2(partial);
  }

  return non_zeros;
}

/* Copy and count non-zero bytes w/o any alignment requirement */
GASNETI_INLINE(gasneti_count0s_copy_bytes)
int gasneti_count0s_copy_bytes(void * GASNETI_RESTRICT dst, const void * GASNETI_RESTRICT src, size_t bytes) {
  int non_zeros = 0;
  uint8_t *d = dst;
  const uint8_t *s = src;
  gasneti_assert_uint(bytes ,<, SIZEOF_VOID_P);

  switch (bytes) {
  #if PLATFORM_ARCH_64
    case 7: non_zeros  = !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
    case 6: non_zeros += !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
    case 5: non_zeros += !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
    case 4: non_zeros += !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
    case 3: non_zeros += !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
  #else
    case 3: non_zeros  = !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
  #endif
    case 2: non_zeros += !!(*(d++) = *(s++)); GASNETI_FALLTHROUGH
    case 1: non_zeros += !!(*(d++) = *(s++));
  }
  return non_zeros;
}

/* Copy and count non-zero bytes w/ both dst and src word-aligned */
GASNETI_INLINE(gasneti_count0s_copy_dstsrc_aligned)
size_t gasneti_count0s_copy_dstsrc_aligned(void * GASNETI_RESTRICT dst, const void * GASNETI_RESTRICT src, size_t words) {
  size_t non_zeros = 0;
  uintptr_t *d = dst;
  const uintptr_t *s = src;
  int i;

  gasneti_assert(!((uintptr_t)dst & (SIZEOF_VOID_P - 1)));
  gasneti_assert(!((uintptr_t)src & (SIZEOF_VOID_P - 1)));

  while (words > gasneti_count0s_xform_limit) {
    uintptr_t partial = 0;
    for (i = 0; i < gasneti_count0s_xform_limit; ++i) {
      partial += gasneti_count0s_xform1((*(d++) = *(s++)));
    }
    non_zeros += gasneti_count0s_xform2(partial);
    words -= gasneti_count0s_xform_limit;
  }
  {
    uintptr_t partial = 0;
    for (i = 0; i < words; ++i) {
      partial += gasneti_count0s_xform1((*(d++) = *(s++)));
    }
    non_zeros += gasneti_count0s_xform2(partial);
  }

  return non_zeros;
}

/* Copy and count non-zero bytes w/ dst word-aligned, but not src */
GASNETI_INLINE(gasneti_count0s_copy_dst_aligned)
size_t gasneti_count0s_copy_dst_aligned(void * GASNETI_RESTRICT dst, const void * GASNETI_RESTRICT src, size_t words) {
  #if !WORDS_BIGENDIAN
    #define GASNETI_MEMCPY0_MERGE(w0,s0,w1,s1) (((w0)>>(s0)) | ((w1)<<(s1)))
  #else
    #define GASNETI_MEMCPY0_MERGE(w0,s0,w1,s1) (((w0)<<(s0)) | ((w1)>>(s1)))
  #endif
  const uintptr_t *s = (uintptr_t *)GASNETI_ALIGNDOWN(src, SIZEOF_VOID_P);
  const size_t s0 = ((uintptr_t)src & (SIZEOF_VOID_P - 1)) << 3;
  const size_t s1 = (SIZEOF_VOID_P * 8) - s0;
  uintptr_t *d = dst;
  uintptr_t w0;
  size_t non_zeros = 0;
  int i;
  
  gasneti_assert(!((uintptr_t)dst & (SIZEOF_VOID_P - 1)));
  gasneti_assert(((uintptr_t)src & (SIZEOF_VOID_P - 1)));

  /* XXX: Options for reducing the bottle neck in GASNETI_MEMCPY0_MERGE()
   *
   * 1) Consider an outer 3- or 7-way switch on alignment.  Doing so would allow for
   *    fixed-count shifts which might be cheaper than variable count.
   *
   * 2) Consider splitting _MERGE(), to perform the shift of w0 as soon as it is available.
   *    Doing so would benefit x86, x86_64 and PA-RISC which must reload a fixed shift-count
   *    register to perform a variable-count shift.  However, keeping the form with both of
   *    the shifts and the OR together benefits machines w/ powerful extract/deposit/merge
   *    instructions.  Use of macros could hide the differences.
   *
   * 3) Consider arch-specific asm().  For x86 and x86_64 the SHLD or SHRD instructions
   *    would be appropriate.  Others have similar (or more powerful) merging operations
   *    that have no C equivalents.
   * 3.5) Search for compiler-specific C constructs that may generate the merge instructions.
   */

  w0 = *(s++);
  while (words > gasneti_count0s_xform_limit) {
    uintptr_t partial = 0;
    for (i = 0; i < gasneti_count0s_xform_limit; ++i) {
      const uintptr_t w1 = *(s++);
      partial += gasneti_count0s_xform1((*(d++) = GASNETI_MEMCPY0_MERGE(w0,s0,w1,s1)));
      w0 = w1;
    }
    non_zeros += gasneti_count0s_xform2(partial);
    words -= gasneti_count0s_xform_limit;
  }
  {
    uintptr_t partial = 0;
    for (i = 0; i < words; ++i) {
      const uintptr_t w1 = *(s++);
      partial += gasneti_count0s_xform1((*(d++) = GASNETI_MEMCPY0_MERGE(w0,s0,w1,s1)));
      w0 = w1;
    }
    non_zeros += gasneti_count0s_xform2(partial);
  }

  return non_zeros;
}

extern size_t
gasneti_count0s_copy(void * GASNETI_RESTRICT dst, const void * GASNETI_RESTRICT src, size_t bytes) {
#if 0 /* Naive byte-oriented loop */
  size_t zeros = 0;
  uint8_t *d = dst;
  const uint8_t *s = src;
  while (bytes--) zeros += !(*(d++) = *(s++));
  return zeros;
#elif PLATFORM_COMPILER_PATHSCALE /* avoid bug 3428 using pre-memcpy/post-count0s */
  size_t zeros = gasneti_count0s(memcpy(dst, src, bytes), bytes);
#else /* Carefully optimized (but still portable) word-oriented loop */
  size_t tmp, remain, zeros;
  const uint8_t *s;
  uint8_t *d;
  
  /* Short cut on less than full word, simplifying the logic below */
  if (bytes < SIZEOF_VOID_P) {
    return (bytes - gasneti_count0s_copy_bytes(dst, src, bytes));
  }

  s = (uint8_t *)src;
  d = (uint8_t *)dst;
  remain = zeros = bytes;

  /* Copy by bytes until dst is aligned */
  tmp = ((uintptr_t)dst & (SIZEOF_VOID_P - 1));
  if (tmp) {
    tmp = SIZEOF_VOID_P - tmp;
    zeros -= gasneti_count0s_copy_bytes(d, s, tmp); 
    d += tmp;
    s += tmp;
    remain -= tmp;
  }

  /* Copy full words of dst */
  tmp = remain >> gasneti_count0s_word_shift;
  if ((uintptr_t)s & (SIZEOF_VOID_P - 1)) {
    zeros -= gasneti_count0s_copy_dst_aligned(d, s, tmp);
  } else {
    zeros -= gasneti_count0s_copy_dstsrc_aligned(d, s, tmp);
  }
  d += tmp << gasneti_count0s_word_shift;
  s += tmp << gasneti_count0s_word_shift;
 
  /* Copy any remainder by bytes until done */
  tmp = remain & (SIZEOF_VOID_P - 1);
  zeros -= gasneti_count0s_copy_bytes(d, s, tmp); 
#endif

  return zeros;
}
  
size_t
gasneti_count0s(const void * src, size_t bytes) {
#if 0 /* Naive byte-oriented loop */
  const uint8_t *s = src;
  size_t zeros = 0;
  while (bytes--) { zeros += !*(s++); }
#else /* Carefully optimized (but still portable) word-oriented loop */
  const uintptr_t *s;
  size_t zeros, tmp;

  /* Short cut on less than full word, simplifying the logic below */
  if (bytes < SIZEOF_VOID_P) {
    const uint8_t *s8 = src;
    zeros = 0;
    while (bytes--) { zeros += !*(s8++); }
    return zeros;
  }

  s = (uintptr_t *)GASNETI_ALIGNUP(src, SIZEOF_VOID_P);
  zeros = bytes;

  /* Count partial leading word (if any) */
  tmp = (uintptr_t)s - (uintptr_t)src;
  if_pf (tmp) {
    const uint8_t *s8 = src;
    bytes -= tmp;
    do { zeros -= !!*(s8++); } while (--tmp);
  }

  /* Count full words of src */
  tmp = bytes >> gasneti_count0s_word_shift;
  zeros -= gasneti_count0s_nzs_aligned_region(s, tmp);
  s += tmp;
 
  /* Count partial trailing word (if any) */
  tmp = bytes & (SIZEOF_VOID_P - 1);
  if_pf (tmp) {
    const uint8_t *s8 = (const uint8_t *)s;
    do { zeros -= !!*(s8++); } while (--tmp);
  }
#endif

  return zeros;
}
/* ------------------------------------------------------------------------------------ */
/* "out-of-line" helper(s) for calibration of timers */

// gasneti_clock_t: (struct timespec) or (struct timeval)
// gasneti_clock_init(x):     required initialization (not thread-safe)
// gasneti_clock_gettime(&x): record current wallclock as opaque gasneti_clock_t (struct)
// gasneti_clock_to_ns(x):    convert gasneti_clock_t to nanoseconds since arbitrary base
// gasneti_clock_getns(x):    return current wallcock as nanoseconds since arbitrary base
#if HAVE_CLOCK_GETTIME
  static clockid_t gasneti_clockid = CLOCK_REALTIME;
  typedef struct timespec gasneti_clock_t;
  #define gasneti_clock_to_ns(x) ((x).tv_sec*((uint64_t)1E9)+(x).tv_nsec)
#else
  typedef struct timeval gasneti_clock_t;
  #define gasneti_clock_to_ns(x) ((x).tv_sec*((uint64_t)1E9)+1000*(x).tv_usec)
#endif
static int gasneti_clock_is_init = 0;
static void gasneti_clock_init(void) {
  if (gasneti_clock_is_init) return;
  gasneti_clock_is_init = 1;
  #if HAVE_CLOCK_GETTIME
    #ifdef _POSIX_MONOTONIC_CLOCK
    struct timespec tm;
    if (!clock_gettime(CLOCK_MONOTONIC, &tm)) {
      // Monotonic but subject to rate adjustment by NTP
      gasneti_clockid = CLOCK_MONOTONIC;
      #if GASNET_DEBUG_VERBOSE
      fprintf(stderr, "TICKS: using clock_gettime(CLOCK_MONOTONIC)\n");
      #endif
    } else
    #endif
    {
      // May be adjusted by both ntp and by clock_settime()
      gasneti_assert(gasneti_clockid == CLOCK_REALTIME);
      #if GASNET_DEBUG_VERBOSE
      fprintf(stderr, "TICKS: using clock_gettime(CLOCK_REALTIME)\n");
      #endif
    }
  #elif GASNET_DEBUG_VERBOSE
    fprintf(stderr, "TICKS: using gettimeofday()\n");
  #endif
}
GASNETI_INLINE(gasneti_clock_gettime)
void gasneti_clock_gettime(gasneti_clock_t *x) {
  gasneti_assert(gasneti_clock_is_init);
  #if HAVE_CLOCK_GETTIME
    gasneti_assert_zeroret(clock_gettime(gasneti_clockid, x));
  #else
    gasneti_assert_zeroret(gettimeofday(x, NULL));
  #endif
}
GASNETI_INLINE(gasneti_clock_getns)
uint64_t gasneti_clock_getns(void) {
  gasneti_clock_t tmp;
  gasneti_clock_gettime(&tmp);
  return gasneti_clock_to_ns(tmp);
}

// Estimate gasneti_ticks_now() rate in GHz by comparision to
// the OS-provided wallclock an interval of the specified length.
static double gasneti_approx_tick_ghz(uint64_t ns_interval) {
  gasneti_assert(ns_interval > 0);
  uint64_t ref, ref_base;
  uint64_t tsc, tsc_base;
  uint64_t goal;
  ref_base = gasneti_clock_getns();
  tsc_base = gasneti_ticks_now();
  ref = ref_base;
  goal = ref_base + ns_interval;
  do {
    gasneti_nsleep(goal - ref);
    ref = gasneti_clock_getns();
    tsc = gasneti_ticks_now();
  } while (ref < goal);
  return (tsc-tsc_base) / (double)(ref-ref_base);
}

// Calibrate GHz rate of gasneti_ticks_now() against the OS-provided wallclock.
//
// This algorithm is based on use of upper- and lower-bounds which are collected
// using sampling methodologies with one-sided errors, and combined via MIN()
// and MAX() to yield an estimate that is nearly oblivous to noise and outliers.
//
// Consider the following code
//   1.   A0 = sample_timer_A();
//   2.   B0 = sample_timer_B();
//   3.   delay();
//   4.   B1 = sample_timer_B();
//   5.   A1 = sample_timer_A();
// If one assumes the delay is sufficiently long relative to the resolutions and
// overheads of timers A and B, then the ratio (A1-A0)/(B1-B0) will always
// over-estimate the true ratio of their rates, since the B samples are taken
// closer togther than the A samples.  The introduction of delays between lines
// 1 and 2, or between lines 4 and 5 can only increase the ratio, since the
// diffence in B samples is unchanged.  Meanwhile, any delays beween lines 2 and
// 4 will increase both the A and B intervals equally (up to their respective
// resolutions).  A repetition of the AB-BA pattern is the basis for measuring
// an upper bound on the true ratio of the rates of timers A and B.  By taking
// the MIN() over many pairs a fairly tight upper-bound is obtained.  The
// simultaneous collection of a BA-BA pattern provides a fairly tight
// lower-bound without the need for an additional delay, and the mid-point
// between the the two bounds is returned as the calibrated rate.
//
// IN: ref_res should be a estimated resolution in nanoseconds of the
// gasneti_clock_getns() timer if available, or 1E9 otherwise.
//
// OUT: If non-NULL, err_p is a location in which to store the relative
// error of the calibration.
//
// RETURN: the rate gasneti_ticks_now() in units of GHz.
static double gasneti_calibrate_tick_ghz(uint64_t ref_res, double *err_p) {
  #ifndef GASNETI_TICKS_WC_ITERS
  #define GASNETI_TICKS_WC_ITERS 10
  #endif
  #ifndef GASNETI_TICKS_WC_MIN_INTERVAL
  #define GASNETI_TICKS_WC_MIN_INTERVAL 100E6 // 100ms
  #endif
  #ifndef GASNETI_TICKS_WC_MIN_REF_TICKS
  #define GASNETI_TICKS_WC_MIN_REF_TICKS 1000
  #endif
  #ifndef GASNETI_TICKS_WC_MAX_RETRY
  #define GASNETI_TICKS_WC_MAX_RETRY 1
  #endif

  // Collected start and end times:
  gasneti_clock_t wc0[GASNETI_TICKS_WC_ITERS], wc1[GASNETI_TICKS_WC_ITERS]; // wallclock samples
  uint64_t lo0[GASNETI_TICKS_WC_ITERS], lo1[GASNETI_TICKS_WC_ITERS]; // "too low" ticks samples
  uint64_t hi0[GASNETI_TICKS_WC_ITERS], hi1[GASNETI_TICKS_WC_ITERS]; // "too high" ticks samples
  int trycnt = 0;

retry_calibration:;
  // Collect start-time samples and compute {ticks,ref}_res.
  uint64_t ticks_res = 1E9;
  const int count = GASNETI_TICKS_WC_ITERS;
  for (int i = 0; i < count; ++i) {
    gasneti_compiler_fence(); hi0[i] = gasneti_ticks_now();
    gasneti_compiler_fence(); gasneti_clock_gettime(&wc0[i]);
    gasneti_compiler_fence(); lo0[i] = gasneti_ticks_now();
    gasneti_compiler_fence();

    // Ensure next samples will be distinct, and refine *_res estimates.
    // Use of ">=" is (quite limited) protection against ticks samples going backwards
    uint64_t tmp1, tmp2;
    tmp1 = lo0[i];
    while (tmp1 >= (tmp2 = gasneti_ticks_now())) { gasneti_compiler_fence(); }
    tmp2 -= tmp1;
    ticks_res = MIN(ticks_res, tmp2);
    tmp1 = gasneti_clock_getns();
    while (tmp1 >= (tmp2 = gasneti_clock_getns())) { gasneti_compiler_fence(); }
    tmp2 -= tmp1;
    ref_res = MIN(ref_res, tmp2);
  }

  #if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "TICKS: ticks and wallclock resolutions are %d and %d ns (or better)\n",
          (int)ticks_res, (int)ref_res);
  #endif

  // Delay, with a default interval of MAX(100ms, 1000 ref ticks)
  const uint64_t interval_ns = MAX(GASNETI_TICKS_WC_MIN_INTERVAL,
                                   GASNETI_TICKS_WC_MIN_REF_TICKS * ref_res);
  uint64_t now = gasneti_clock_to_ns(wc0[count - 1]);
  uint64_t end = now + interval_ns;
  do {
    gasneti_nsleep(end - now);
    now = gasneti_clock_getns();
  } while (now < end);

  // Collect end-time samples
  static volatile double acc = 0.;
  for (int i = 0; i < count; ++i) {
    gasneti_compiler_fence(); lo1[i] = gasneti_ticks_now();
    gasneti_compiler_fence(); gasneti_clock_gettime(&wc1[i]);
    gasneti_compiler_fence(); hi1[i] = gasneti_ticks_now();
    gasneti_compiler_fence();

    // Busy-work to supply some inter-sample delay
    for (int j = 0; j < i; ++j) { acc += (double)hi1[j]; }
  }

  // Sanity check: detect the largest monotonicity violations in the sample set
  uint64_t max_err_tick = 0;
  uint64_t max_err_wcns = 0;
  uint64_t samp_tick = hi0[0];
  uint64_t samp_wcns = gasneti_clock_to_ns(wc0[0]);
  #define CHECK_NEXT_TICK(this_tick) do {              \
    if (this_tick + ticks_res < samp_tick) {           \
      uint64_t back_step = samp_tick - this_tick;      \
      max_err_tick = MAX(max_err_tick, back_step);     \
    }                                                  \
    samp_tick = this_tick;                             \
  } while (0)
  #define CHECK_NEXT_WC(this_wc) do {                  \
    uint64_t this_wcns = gasneti_clock_to_ns(this_wc); \
    if (this_wcns + ref_res < samp_wcns) {             \
      uint64_t back_step = samp_wcns - this_wcns;      \
      max_err_wcns = MAX(max_err_wcns, back_step);     \
    }                                                  \
    samp_wcns = this_wcns;                             \
  } while (0)
  for (int i = 0; i < count; ++i) {
    CHECK_NEXT_TICK(hi0[i]); // do not reorder these calls
    CHECK_NEXT_TICK(lo0[i]);
    CHECK_NEXT_WC(wc0[i]);
  }
  for (int i = 0; i < count; ++i) {
    CHECK_NEXT_TICK(lo1[i]); // do not reorder these calls
    CHECK_NEXT_TICK(hi1[i]);
    CHECK_NEXT_WC(wc1[i]);
  }
  #undef CHECK_NEXT_TICK
  #undef CHECK_NEXT_WCE

  // Compute the best lower- and upper-bounds from the collected samples
  // Worst case each difference is too high or low by its respective granulatity
  double lo = 0;
  double hi = 1E30;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < count; ++j) {
      // bug 3866: following expressions are carefully written to encourage isolated "backward steps" in the
      // tick samples to end up as double values that will be discarded by the MIN/MAX operation,
      // instead of overflowing into values that corrupt the entire calculation.
      // Backward steps in the ref timer are not tolerated here, but will trigger a repeat of sample collection.
      const double wc_delta  = (double)(int64_t)(gasneti_clock_to_ns(wc1[i]) - gasneti_clock_to_ns(wc0[j]));
      const double lo_delta  = (double)(int64_t)(lo1[i] - lo0[j]); // bias overflow to negative
      const double hi_delta  = (double)(uint64_t)(hi1[i] - hi0[j]); // bias overflow to large positive
      double new_lo = (lo_delta - ticks_res) / (wc_delta + ref_res);
      double new_hi = (hi_delta + ticks_res) / (wc_delta - ref_res);
      lo = MAX(lo, new_lo);
      hi = MIN(hi, new_hi);
    }
  }

  // Sanity check: detect violations of the relationship between computed frequency bounds
  // This should never happen with properly-behaved clocks (ie those that
  // reliably return samples with a fixed linear relationship to true time,
  // within resolution error), but might concievably occur if calibration coincides
  // with a process migration across cores with sufficiently de-synchronized time bases.
  if (lo > hi || 
      max_err_tick > 0 || max_err_wcns > 0) {  // also report monotonicity violations
    gasneti_console_message("WARNING","GASNet timer calibration on %s detected non-linear timer behavior: "
                    "max_err_tick=%"PRIu64" max_err_wcns=%"PRIu64" ticks_res=%"PRIu64" ref_res=%"PRIu64" lo=%"PRIu64" hi=%"PRIu64". See docs for GASNET_TSC_RATE."
                    "%s\n",
                    gasneti_gethostname(),
                    max_err_tick, max_err_wcns, 
                    ticks_res, ref_res,
                    (uint64_t)(1e9 * lo), (uint64_t)(1e9 * hi), 
                    (trycnt < GASNETI_TICKS_WC_MAX_RETRY?" Retrying...":""));

    char sample_msg[GASNETI_TICKS_WC_ITERS*400];
    char *p = sample_msg;
    for (int n0 = 0; n0 < count; ++n0) {
      if (p < &sample_msg[sizeof(sample_msg)]) {
        int n1 = count-1-n0;
        uint64_t wc0_n0 = gasneti_clock_to_ns(wc0[n0]);
        uint64_t wc1_n1 = gasneti_clock_to_ns(wc1[n1]);
        const double wc_delta  = (double)(int64_t)(wc1_n1 - wc0_n0);
        const double lo_delta  = (double)(int64_t)(lo1[n1] - lo0[n0]);
        const double hi_delta  = (double)(uint64_t)(hi1[n1] - hi0[n0]);
        double new_lo = (lo_delta - ticks_res) / (wc_delta + ref_res);
        double new_hi = (hi_delta + ticks_res) / (wc_delta - ref_res);
        p += snprintf(p, sizeof(sample_msg) - (p - sample_msg),
             " wc1[%i]=%-10"PRIu64" wc0[%i]=%-10"PRIu64" delta=%-10.0f"
             " lo1[%i]=%-10"PRIu64" lo0[%i]=%-10"PRIu64" delta=%-10.0f"
             " hi1[%i]=%-10"PRIu64" hi0[%i]=%-10"PRIu64" delta=%-10.0f"
             " lo=%8.6f hi=%8.6f\n",
             n1, wc1_n1,  n0, wc0_n0,  wc_delta,
             n1, lo1[n1], n0, lo0[n0], lo_delta,
             n1, hi1[n1], n0, hi0[n0], hi_delta,
             new_lo, new_hi);
      }
    }
    gasneti_console_message("TICKS: Debugging information:","\n%s",sample_msg);

    if (++trycnt <= GASNETI_TICKS_WC_MAX_RETRY) goto retry_calibration;

    if (lo > hi) { // retry did not help
      // swap hi and lo to prevent sign errors below and continue
      double tmp = lo; lo = hi; hi = tmp;
    }
  }

  // Find mid-point between the two bounds, and its associated relative error
  gasneti_assert_dbl(lo ,<=, hi);
  double mid = (hi + lo) / 2.;
  double half_width = mid - lo;
  double err = half_width / hi;
  if (err_p) *err_p = err;

  #if GASNET_DEBUG_VERBOSE
  double sum = 0;
  for (int i = 0; i < count; ++i) {
    const double delta  = gasneti_clock_to_ns(wc1[i]) - gasneti_clock_to_ns(wc0[i]);
    sum += (hi1[i] - lo0[i]) / delta;
    sum += (lo1[i] - hi0[i]) / delta;
  }
  double mean = sum / (2 * count);
  fprintf(stderr, "TICKS: range: %"PRIu64" +/- %"PRIu64"  mean: %"PRIu64"  offset: %"PRId64"\n",
          (uint64_t)(1e9 * mid),  (uint64_t)(1e9 * half_width),
          (uint64_t)(1e9 * mean), (int64_t)(1e9 * (mean-mid)));
  fprintf(stderr, "TICKS: calibrated to err of %g in %d iters\n", err, GASNETI_TICKS_WC_ITERS);
  #endif

  return mid;
}

#if GASNETI_CALIBRATE_TSC /* x86, x86-64, MIC and ia64 */
extern double gasneti_calibrate_tsc_from_kernel(void) {
  double Tick = 0.0; /* Inverse GHz */

  #if GASNETI_HAVE_SYSCTL_MACHDEP_TSC_FREQ /* FreeBSD and NetBSD */
    int64_t cpuspeed = 0;
    size_t len = sizeof(cpuspeed);
    if (sysctlbyname("machdep.tsc_freq", &cpuspeed, &len, NULL, 0) == -1)
      gasneti_fatalerror("*** ERROR: Failure in sysctlbyname('machdep.tsc_freq')=%s",strerror(errno));
    // ensure it looks reasonable
    gasneti_assert_dbl(cpuspeed ,>, 1E6); 
    gasneti_assert_dbl(cpuspeed ,<, 1E11); 
    Tick = 1.0E9 / cpuspeed;
  #elif PLATFORM_OS_OPENBSD
    int MHz = 0;
    size_t len = sizeof(MHz);
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_CPUSPEED;
    if (sysctl(mib, 2, &MHz, &len, NULL, 0))
      gasneti_fatalerror("*** ERROR: Failure in sysctl(CTL_HW.HW_CPUSPEED)=%s",strerror(errno));
    // ensure it looks reasonable
    gasneti_assert_int(MHz ,>, 1);
    gasneti_assert_int(MHz ,<, 100000); 
    Tick = 1000. / MHz;
  #elif PLATFORM_ARCH_IA64  /* && ( PLATFORM_OS_LINUX || PLATFORM_OS_CNL ) */
    FILE *fp = fopen("/proc/cpuinfo","r");
    char input[255];
    if (!fp) gasneti_fatalerror("*** ERROR: Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));
    while (!feof(fp) && fgets(input, sizeof(input), fp)) {
      if (strstr(input,"itc MHz")) {
        char *p = strchr(input,':');
        double MHz = 0.0;
        if (p) MHz = atof(p+1);
        // ensure it looks reasonable
        gasneti_assert_dbl(MHz ,>, 1);
        gasneti_assert_dbl(MHz ,<, 100000); 
        Tick = 1000. / MHz;
        break;
      }
    }
    fclose(fp);
  #else /* (X86 || X86_64 || MIC) && (Linux || CNL || WSL) */
  FILE *fp = NULL;
  char input[512]; /* 256 is too small for "flags" line in /proc/cpuino */
  double MHz = 0.0;

  fp = fopen("/proc/cpuinfo","r");
  if (!fp) gasneti_fatalerror("*** ERROR: Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));

  /* First pass gets speed from /proc/cpuinfo */
  while (!feof(fp) && fgets(input, sizeof(input), fp)) {
    if (strstr(input,"cpu MHz")) {
      char *p = strchr(input,':');
      if (p) MHz = atof(p+1);
      // ensure it looks reasonable
      gasneti_assert_dbl(MHz ,>, 1);
      gasneti_assert_dbl(MHz ,<, 100000); 
      Tick = 1000. / MHz;
      break;
    }
  }

  /* Now try to deal with variable frequency CPUs.
     The TSC may still run at constant rate (or else is useless to us).
     If so, this code will determine the proper rate.
   */
  {
    FILE *fp2 = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq","r");
    if (fp2 && fgets(input, sizeof(input), fp2)) {
      int good_tsc = 0;
      int bogocount = 0;
      double bogosum = 0.0;

      /* cpuinfo_max_freq contains a "round" value in KHz */
      MHz = atof(input) / 1000.0;
      fclose(fp2);
      // ensure it looks reasonable
      gasneti_assert_dbl(MHz ,>, 1);
      gasneti_assert_dbl(MHz ,<, 100000); 

      /* Now use mean of measured bogomips values to correct the "round" MHz */
      rewind(fp);
      while (!feof(fp) && fgets(input, sizeof(input), fp)) {
        if (strstr(input,"bogomips")) {
          char *p = strchr(input,':');
          double tmp;
          if (p && (sscanf(p+1, "%lg", &tmp) == 1)) {
            bogocount++;
            bogosum += tmp;
          }
        } else if (strstr(input," constant_tsc") || strstr(input," tsc_reliable")) {
          good_tsc = 1;
        }
      }
      if (bogocount && good_tsc) {
        double tmp = 12.0 * bogosum / bogocount; /* 12 allows 1/2, 1/3 or 1/4 steps */
        int ratio = 0.5 + (tmp / MHz); /* truncates toward zero */
        MHz = tmp / ratio;
        Tick = 1000. / MHz;
      } /* Else we are stuck trusting the "cpu MHz" from /proc/cpuinfo */
    }
  }

  fclose(fp);
  #endif

  return Tick;
}

extern double gasneti_calibrate_tsc(void) {
  static int firstTime = 1;
  static double Tick = 0.0; /* Inverse GHz */

  if_pt (! firstTime) {
    gasneti_sync_reads();
    return Tick;
  }

  // Serialize threads attempting initialization
  static gasneti_mutex_t tscmutex = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&tscmutex);
  // NOTICE:
  //   To avoid potential mutual-recursion with the tracing code, one must not
  //   make any calls that may produce tracing output until 'firstTime' has been
  //   set to zero.  In particular one must use gasneti_getenv_early() for any
  //   reads of environment variables.  However, GASNETI_TSC_TRACE_OUTPUT() can
  //   be defined to perform tracing calls, such as to gasneti_env*_display(),
  //   and will run when it is safe.
  if_pf (firstTime) {
  #if !(PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64 || PLATFORM_ARCH_MIC) || \
      !(PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL)
    Tick = gasneti_calibrate_tsc_from_kernel();
  #else /* (X86 || X86_64 || MIC) && (Linux || CNL || WSL) */
    #ifndef GASNETI_DEFAULT_TSC_RATE
    // TODO: need logic to default to "cpuinfo" when we can determine CPU model is trustworthy
    #define GASNETI_DEFAULT_TSC_RATE "wallclock"
    #endif
    const char *tsc_rate = gasneti_getenv_early("GASNET_TSC_RATE");
    int tsc_rate_dflt = (NULL == tsc_rate);
    if (tsc_rate_dflt) tsc_rate = GASNETI_DEFAULT_TSC_RATE;
    enum {
      tsc_source_cpuinfo,    // "cpuinfo"    - parse /proc/cpuinfo for the TSC rate
      tsc_source_wallclock,  // "wallclock"  - calibrate TSC against OS-provided wallclock
      tsc_source_user,       // rate in Hz   - user provided rate (subject to verification)
    } tsc_source;
    if (0 == strcmp(tsc_rate, "cpuinfo")) {
      tsc_source = tsc_source_cpuinfo;
    } else if (0 == strcmp(tsc_rate, "wallclock")) {
      tsc_source = tsc_source_wallclock;
    } else {
      tsc_source = tsc_source_user;
      int64_t Hz = gasneti_parse_int(tsc_rate, 0);
      if (Hz < 1E6 || Hz > 1E11) { // 1MHz to 100GHz (same range accepted elsewhere)
        gasneti_fatalerror("GASNET_TSC_RATE must be a rate in Hz (no M or G suffix) or the name of a known source ('cpuinfo' or 'wallclock'), but was set to '%s'.", tsc_rate);
      }
      Tick = 1e9 / Hz;
    }

    // Allowable relative error for the calibrated TSC rate.
    // GASNETI_DEFAULT_TSC_RATE_TOLERANCE      - warn if we can't meet this
    // GASNETI_DEFAULT_TSC_RATE_HARD_TOLERANCE - fatalerror if we can't meet this
    // Zero values disable the associated checks.
    // NOTE: "wallclock" calibration applies this slightly differently than others.
    #ifndef GASNETI_DEFAULT_TSC_RATE_TOLERANCE
    #define GASNETI_DEFAULT_TSC_RATE_TOLERANCE 0.0005 // 0.05% - matches testtools default
    #endif
    double soft_tolerance = GASNETI_DEFAULT_TSC_RATE_TOLERANCE;
    const char *soft_tol_str = gasneti_getenv_early("GASNET_TSC_RATE_TOLERANCE");
    int soft_tol_dflt = (NULL == soft_tol_str);
    if (!soft_tol_dflt && gasneti_parse_dbl(soft_tol_str, &soft_tolerance)) {
      gasneti_fatalerror("If set, environment variable GASNET_TSC_RATE_TOLERANCE must be a valid floating point value or fraction");
    }
    const int check_soft = soft_tolerance > 0.0;
    if ((soft_tolerance < 0.0) || (soft_tolerance > 1.0)) {
      gasneti_fatalerror(
          "GASNET_TSC_RATE_TOLERANCE must be in the range 0.0 - 1.0, inclusive, but '%g' was given",
          soft_tolerance);
    }
    #ifndef GASNETI_DEFAULT_TSC_RATE_HARD_TOLERANCE
    #define GASNETI_DEFAULT_TSC_RATE_HARD_TOLERANCE 0.02 // 2%
    #endif
    double hard_tolerance = GASNETI_DEFAULT_TSC_RATE_HARD_TOLERANCE;
    const char *hard_tol_str = gasneti_getenv_early("GASNET_TSC_RATE_HARD_TOLERANCE");
    int hard_tol_dflt = (NULL == hard_tol_str);
    if (!hard_tol_dflt && gasneti_parse_dbl(hard_tol_str, &hard_tolerance)) {
      gasneti_fatalerror("If set, environment variable GASNET_TSC_RATE_HARD_TOLERANCE must be a valid floating point value or fraction");
    }
    const int check_hard = hard_tolerance > 0.0;
    if ((hard_tolerance < 0.0) || (hard_tolerance > 1.0)) {
      gasneti_fatalerror(
          "GASNET_TSC_RATE_HARD_TOLERANCE must be in the range 0.0 - 1.0, inclusive, but '%g' was given",
          hard_tolerance);
    }

    // We test using the smaller of the "active" tolerances
    double tolerance;
    if (check_hard) {
      if (check_soft) {
        tolerance = MIN(hard_tolerance, soft_tolerance);
      } else {
        tolerance = hard_tolerance;
      }
    } else {
      tolerance = soft_tolerance; // Even if zero
    }

    #define GASNETI_TSC_TRACE_OUTPUT()  do { \
      gasneti_envstr_display("GASNET_TSC_RATE", tsc_rate, tsc_rate_dflt); \
      gasneti_envdbl_display("GASNET_TSC_RATE_TOLERANCE", soft_tolerance, soft_tol_dflt); \
      gasneti_envdbl_display("GASNET_TSC_RATE_HARD_TOLERANCE", hard_tolerance, hard_tol_dflt); \
    } while (0)

    // Determine/initialize the best available wallclock timer
    gasneti_clock_init();

    #if GASNET_DEBUG_VERBOSE
    uint64_t begin_tsc_calibration = gasneti_clock_getns();
    #endif

    // Approximate the resolution of the reference clock in ns (if needed)
    uint64_t ref_res = (uint64_t)1E9;
    if ((tsc_source == tsc_source_wallclock) || (tolerance > 0.0)) {
      const uint64_t max_res = 5000;     // We actually only care that it is below 5us
      const uint64_t max_sum = 20000000; // And we don't want to spend more than 20ms here
      uint64_t sum = 0;
      int i;
      for (i=0; (i < 1000) && (ref_res > max_res) && (i < 10 || sum < max_sum); i++) {
        uint64_t start, next;
        start = gasneti_clock_getns();
        while (start == (next = gasneti_clock_getns()));
        uint64_t delta = (next-start);
        ref_res = MIN(ref_res, delta);
        sum += delta;
      }
      #if GASNET_DEBUG_VERBOSE
      fprintf(stderr, "TICKS: reference resolution is %d ns or better (in %d iters, %lu ns)\n",
                      (int)ref_res, i, (unsigned long)sum);
      #endif
      if_pf (ref_res > max_res) {
        gasneti_fatalerror("Reference timer resolution of %lu ns on %s is not acceptable for calibration of the TSC.\n"
                           "Please reconfigure with --enable-force-gettimeofday or --enable-force-posix-realtime.\n",
                           (unsigned long)ref_res, gasneti_gethostname());
      }
    }

    if (tsc_source == tsc_source_cpuinfo) {
      Tick = gasneti_calibrate_tsc_from_kernel();
    } else if (tsc_source == tsc_source_wallclock) {
      double err, rate = gasneti_calibrate_tick_ghz(ref_res , &err);
      Tick = 1. / rate; // Inverse GHz

      // Check that we converged within given tolerance
      if (check_hard && (err > hard_tolerance)) {
        gasneti_fatalerror(
            "TSC calibration did not converge with reasonable certainty on %s (%g > %g).\n"
            "Please see GASNet's README-tools for a description of GASNET_TSC_RATE_HARD_TOLERANCE or "
            "reconfigure with either --enable-force-gettimeofday or --enable-force-posix-realtime.",
            gasneti_gethostname(), err, hard_tolerance);
      }
      if (check_soft && (err > soft_tolerance)) {
        gasneti_console_message("WARNING",
            "TSC calibration did not converge with reasonable certainty on %s (%g > %g).  "
            "Please see GASNet's README-tools for a description of GASNET_TSC_RATE_TOLERANCE or "
            "reconfigure with either --enable-force-gettimeofday or --enable-force-posix-realtime.\n",
            gasneti_gethostname(), err, soft_tolerance);
      }
    } else {
      gasneti_assert(tsc_source == tsc_source_user);
    }

    // Don't blindly trust either the user or cpuinfo
    // Validate Tick by comparison to reference wallclock over 10ms interval
    // Zero or negative GASNET_TSC_RATE_TOLERANCE disables this check
    if ((tsc_source != tsc_source_wallclock) && (tolerance > 0.0)) {
      const int max_tries = 3;
      double best = 1e4;
      int i;
      for (i = 0; i < max_tries; ++i) { // Retry in case we get unlucky
        double ratio = Tick * gasneti_approx_tick_ghz(10000000);
        if (ratio < best) {
          best = ratio;
          if ((best > (1. - tolerance)) && (best < (1. + tolerance))) break; // Pass
        }
      }
      if (i == max_tries) {
        if (check_hard && ((best < (1. - hard_tolerance)) || (best > (1. + hard_tolerance)))) {
            gasneti_fatalerror(
                "Reference timer and calibrated TSC differ too much on %s (ratio %g).\n"
                "Please see GASNet's README-tools for a description of GASNET_TSC_RATE_HARD_TOLERANCE or "
                "reconfigure with either --enable-force-gettimeofday or --enable-force-posix-realtime.",
                gasneti_gethostname(), best);
        }
        if (check_soft && ((best < (1. - soft_tolerance)) || (best > (1. + soft_tolerance)))) {
            gasneti_console_message("WARNING",
                "Reference timer and calibrated TSC differ too much on %s (ratio %g).  "
                "Please see GASNet's README-tools for a description of GASNET_TSC_RATE_TOLERANCE or "
                "reconfigure with either --enable-force-gettimeofday or --enable-force-posix-realtime.\n",
                gasneti_gethostname(), best);
        }
      }
      #if GASNET_DEBUG_VERBOSE
      fprintf(stderr, "TICKS: relative to wallclock = %g\n", best);
      #endif
    }

    #if GASNET_DEBUG_VERBOSE
    fprintf(stderr, "TICKS: rate calibrated to %g MHz in %g sec\n",
            1e3/Tick, 1e-9*(gasneti_clock_getns()-begin_tsc_calibration));
    #endif
  #endif

    gasneti_sync_writes();
    firstTime = 0;

  #ifdef GASNETI_TSC_TRACE_OUTPUT
    GASNETI_TSC_TRACE_OUTPUT();
  #endif
  }
  gasneti_mutex_unlock(&tscmutex);

  return Tick;
}
#endif
/* ------------------------------------------------------------------------------------ */

