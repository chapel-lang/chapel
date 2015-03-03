/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_tools.c $
 * Description: GASNet implementation of internal helpers
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if defined(GASNET_PARSYNC) || defined(GASNET_PAR) 
  #define GASNETT_THREAD_SAFE 1
#elif defined(GASNET_SEQ)
  #define GASNETI_THREAD_SINGLE 1
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
#include <fcntl.h>
#include <time.h> /* gasneti_gettimeofday_us */
#include <sys/time.h> /* gasneti_gettimeofday_us */
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
    #define _gasneti_atomic_lock_malloc		malloc
    GASNETI_ATOMIC_LOCK_TBL_DEFNS(gasneti_pthread_atomic_, gasnett_mutex_)
    #undef _gasneti_atomic_lock_initializer
    #undef _gasneti_atomic_lock_init
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
    if (gasneti_atomic32_read(initstep,GASNETI_ATOMIC_RMB_POST) == 2) return;
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

/* ------------------------------------------------------------------------------------ */
/* call-based atomic support for C compilers with limited inline assembly */

#ifdef GASNETI_ATOMIC_SPECIALS
  GASNETI_ATOMIC_SPECIALS
#endif

/* ------------------------------------------------------------------------------------ */
/* call-based membar/atomic support for C++ compilers which lack inline assembly */
#if defined(GASNETI_USING_SLOW_ATOMICS) || \
    defined(GASNETI_USING_SLOW_MEMBARS) || \
    defined(GASNETI_USING_SLOW_TIMERS)
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

#ifdef GASNETI_USE_GENERIC_ATOMICOPS
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern gasneti_atomic_val_t gasneti_slow_atomic_read(gasneti_atomic_t *p, const int flags) {
    return gasneti_atomic_read(p,flags);
  }
  extern void gasneti_slow_atomic_set(gasneti_atomic_t *p, gasneti_atomic_val_t v, const int flags) {
    gasneti_atomic_set(p, v, flags);
  }
  extern void gasneti_slow_atomic_increment(gasneti_atomic_t *p, const int flags) {
    gasneti_atomic_increment(p, flags);
  }
  extern void gasneti_slow_atomic_decrement(gasneti_atomic_t *p, const int flags) {
    gasneti_atomic_decrement(p, flags);
  }
  extern int gasneti_slow_atomic_decrement_and_test(gasneti_atomic_t *p, const int flags) {
    return gasneti_atomic_decrement_and_test(p, flags);
  }
  #if defined(GASNETI_HAVE_ATOMIC_CAS)
    extern int gasneti_slow_atomic_compare_and_swap(gasneti_atomic_t *p, gasneti_atomic_val_t oldval, gasneti_atomic_val_t newval, const int flags) {
      return gasneti_atomic_compare_and_swap(p,oldval,newval,flags);
    }
    extern gasneti_atomic_val_t gasneti_slow_atomic_swap(gasneti_atomic_t *p, gasneti_atomic_val_t val, const int flags) {
      return gasneti_atomic_swap(p,val,flags);
    }
  #endif
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    extern gasneti_atomic_val_t gasneti_slow_atomic_add(gasneti_atomic_t *p, gasneti_atomic_val_t op, const int flags) {
      return gasneti_atomic_add(p,op,flags);
    }
    extern gasneti_atomic_val_t gasneti_slow_atomic_subtract(gasneti_atomic_t *p, gasneti_atomic_val_t op, const int flags) {
      return gasneti_atomic_subtract(p,op,flags);
    }
  #endif
#endif
#ifdef GASNETI_USE_GENERIC_ATOMIC32
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern uint32_t gasneti_slow_atomic32_read(gasneti_atomic32_t *p, const int flags) {
    return gasneti_atomic32_read(p,flags);
  }
  extern void gasneti_slow_atomic32_set(gasneti_atomic32_t *p, uint32_t v, const int flags) {
    gasneti_atomic32_set(p, v, flags);
  }
  extern void gasneti_slow_atomic32_increment(gasneti_atomic32_t *p, const int flags) {
    gasneti_atomic32_increment(p, flags);
  }
  extern void gasneti_slow_atomic32_decrement(gasneti_atomic32_t *p, const int flags) {
    gasneti_atomic32_decrement(p, flags);
  }
  extern int gasneti_slow_atomic32_decrement_and_test(gasneti_atomic32_t *p, const int flags) {
    return gasneti_atomic32_decrement_and_test(p, flags);
  }
  extern int gasneti_slow_atomic32_compare_and_swap(gasneti_atomic32_t *p, uint32_t oldval, uint32_t newval, const int flags) {
    return gasneti_atomic32_compare_and_swap(p,oldval,newval,flags);
  }
  extern uint32_t gasneti_slow_atomic32_swap(gasneti_atomic32_t *p, uint32_t val, const int flags) {
    return gasneti_atomic32_swap(p,val,flags);
  }
  extern uint32_t gasneti_slow_atomic32_add(gasneti_atomic32_t *p, uint32_t op, const int flags) {
    return gasneti_atomic32_add(p,op,flags);
  }
  extern uint32_t gasneti_slow_atomic32_subtract(gasneti_atomic32_t *p, uint32_t op, const int flags) {
    return gasneti_atomic32_subtract(p,op,flags);
  }
#endif
#ifdef GASNETI_USE_GENERIC_ATOMIC64
  /* We don't need or want slow versions of generics (they use no ASM) */
#else
  extern uint64_t gasneti_slow_atomic64_read(gasneti_atomic64_t *p, const int flags) {
    return gasneti_atomic64_read(p,flags);
  }
  extern void gasneti_slow_atomic64_set(gasneti_atomic64_t *p, uint64_t v, const int flags) {
    gasneti_atomic64_set(p, v, flags);
  }
  extern void gasneti_slow_atomic64_increment(gasneti_atomic64_t *p, const int flags) {
    gasneti_atomic64_increment(p, flags);
  }
  extern void gasneti_slow_atomic64_decrement(gasneti_atomic64_t *p, const int flags) {
    gasneti_atomic64_decrement(p, flags);
  }
  extern int gasneti_slow_atomic64_decrement_and_test(gasneti_atomic64_t *p, const int flags) {
    return gasneti_atomic64_decrement_and_test(p, flags);
  }
  extern int gasneti_slow_atomic64_compare_and_swap(gasneti_atomic64_t *p, uint64_t oldval, uint64_t newval, const int flags) {
    return gasneti_atomic64_compare_and_swap(p,oldval,newval,flags);
  }
  extern uint64_t gasneti_slow_atomic64_swap(gasneti_atomic64_t *p, uint64_t val, const int flags) {
    return gasneti_atomic64_swap(p,val,flags);
  }
  extern uint64_t gasneti_slow_atomic64_add(gasneti_atomic64_t *p, uint64_t op, const int flags) {
    return gasneti_atomic64_add(p,op,flags);
  }
  extern uint64_t gasneti_slow_atomic64_subtract(gasneti_atomic64_t *p, uint64_t op, const int flags) {
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
    #if defined(GASNETI_FORCE_TRUE_WEAKATOMICS) && GASNETI_THREAD_SINGLE
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
/* timer support */

extern uint64_t gasneti_gettimeofday_us(void) {
  uint64_t retval;
  struct timeval tv;
  gasneti_assert_zeroret(gettimeofday(&tv, NULL));
  retval = ((uint64_t)tv.tv_sec) * 1000000 + (uint64_t)tv.tv_usec;
  return retval;
}

extern double gasneti_tick_metric(int idx) {
  static double *_gasneti_tick_metric = NULL;
  gasneti_assert(idx <= 1);
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
volatile int gasnet_frozen = 0;
extern void gasneti_fatalerror(const char *msg, ...) {
  va_list argptr;
  char expandedmsg[255];

  strcpy(expandedmsg, "*** FATAL ERROR: ");
  strcat(expandedmsg, msg);
  strcat(expandedmsg, "\n");
  va_start(argptr, msg); /*  pass in last argument */
    vfprintf(stderr, expandedmsg, argptr);
    fflush(stderr);
  va_end(argptr);

  gasnett_freezeForDebuggerErr(); /* allow freeze */

  /* try to get a pre-signal backtrace, which may be more precise */
  if (!gasneti_print_backtrace_ifenabled(STDERR_FILENO)) 
    gasneti_atomic_set(&gasneti_backtrace_enabled,0,GASNETI_ATOMIC_REL);

  abort();
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
  if ( gasneti_getenv_yesno_withdefault("GASNET_FS_SYNC",0) ) {
#if PLATFORM_OS_MTA
    mta_sync();
#else
    sync();
#endif
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
    while (*_gasneti_freeze_flag) {
      i++;
      sleep(1);
    }
    gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, old);
  }
}
extern void gasneti_freezeForDebuggerNow(volatile int *flag, const char *flagsymname) {
  fprintf(stderr,"Process frozen for debugger: host=%s  pid=%i\n"
                 "To unfreeze, attach a debugger and set '%s' to 0, or send a "
                 GASNETI_UNFREEZE_SIGNAL_STR "\n", 
                 gasnett_gethostname(), (int)getpid(), flagsymname); 
  fflush(stderr);
  _gasneti_freeze_flag = flag;
  *_gasneti_freeze_flag = 1;
  gasneti_local_wmb();
  _freezeForDebugger(0);
}

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
    fprintf(stderr,"Caught GASNET_FREEZE_SIGNAL: signal %s\n", sigstr);
    gasneti_freezeForDebuggerNow(&gasnet_frozen,"gasnet_frozen");
  } else if (sig == gasneti_backtracesignal) {
    fprintf(stderr,"Caught GASNET_BACKTRACE_SIGNAL: signal %s\n", sigstr);
    gasneti_print_backtrace(STDERR_FILENO);
  } else gasneti_fatalerror("unrecognized signal in gasneti_ondemandHandler: %i", sig);
}

extern void gasneti_ondemand_init(void) {
  static int firsttime = 1;
  if (firsttime) {
    const char *str = gasneti_getenv_withdefault("GASNET_FREEZE_SIGNAL",NULL);
    if (str) {
      gasnett_siginfo_t const *info = gasnett_siginfo_fromstr(str);
      if (!info) fprintf(stderr, "WARNING: ignoring unrecognized GASNET_FREEZE_SIGNAL: %s\n", str);
      else gasneti_freezesignal = info->signum;
    }
    str = gasneti_getenv_withdefault("GASNET_BACKTRACE_SIGNAL",NULL);
    if (str) {
      gasnett_siginfo_t const *info = gasnett_siginfo_fromstr(str);
      if (!info) fprintf(stderr, "WARNING: ignoring unrecognized GASNET_BACKTRACE_SIGNAL: %s\n", str);
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
#if defined(GDB_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_GDB	&gasneti_bt_gdb
#endif
#if defined(LADEBUG_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_LADEBUG	&gasneti_bt_ladebug
#endif
#if defined(DBX_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_DBX	&gasneti_bt_dbx
#endif
#if defined(IDB_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_IDB	&gasneti_bt_idb
#endif
#if defined(PGDBG_PATH) && !GASNETI_NO_FORK
  #define GASNETI_BT_PGDBG	&gasneti_bt_pgdbg
#endif

/* Because some glibc headers annotate nearly all system calls
 * with "__attribute__ ((__warn_unused_result__))", we need to
 * do "something" with return values to avoid gcc warnings.
 * Since here in the backtrace code we are dealing with handling
 * of a (presumed fatal) error case, we can't really deal with
 * most errors in any intelligent way.
 * This is a stupid hack to deal with this.
 */
static int gasneti_bt_rc_unused;

#if !GASNETI_NO_FORK
/* Execute system w/ stdout redirected to 'fd' and std{in,err} to /dev/null */
static int gasneti_system_redirected(const char *cmd, int stdout_fd) {
  int rc;
  int saved_stdin, saved_stdout, saved_stderr;
  off_t beginpos, endpos;


  /* XXX: what if the following two writes fail? */
  gasneti_bt_rc_unused = write(stdout_fd, cmd, strlen(cmd));
  gasneti_bt_rc_unused = write(stdout_fd, "\n", 1);

  beginpos = lseek(stdout_fd, 0, SEEK_CUR); /* fetch current position */

  /* Redirect output to 'fd' and std{in,err} to /dev/null */
  saved_stdin = dup(STDIN_FILENO);
  saved_stdout = dup(STDOUT_FILENO);
  saved_stderr = dup(STDERR_FILENO);
  dup2(stdout_fd, STDOUT_FILENO);
  rc = open("/dev/null", O_WRONLY); dup2(rc, STDERR_FILENO); close(rc);
  rc = open("/dev/null", O_RDONLY); dup2(rc, STDIN_FILENO); close(rc);

  /* Run the command */
  rc = system(cmd);

  endpos = lseek(stdout_fd, 0, SEEK_CUR); /* fetch current position */
  if (beginpos > 0 && endpos > 0 && (beginpos == endpos)) {
    rc = -1; /* command failed to generate output - consider it a failure */
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
  if (!file) return -1;
  tmpfd = fileno(file);

  { /* setup the parent to sleep */
    gasneti_sighandlerfn_t old_sigh = gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, gasneti_bt_complete_handler);
    volatile int i=0;
    if (!fork()) { /* the child - debugger co-process launcher */
      int retval = gasneti_system_redirected(cmd, tmpfd);
      if (retval) { /* system call failed - nuke the output */
        gasneti_bt_rc_unused = ftruncate(tmpfd, 0);
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
      gasneti_reghandler(GASNETI_UNFREEZE_SIGNAL, old_sigh);
      if (fstat(tmpfd, &tmpstat)) rc = -1; /* never happens? */
      else if (tmpstat.st_size == 0) rc = -1; /* child process spawn failed */
      else if (lseek(tmpfd, 0, SEEK_SET)) rc = -1;
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
                else { rc = -1; break; } /* write error */
              }
          }
        }
        if (bytes == -1) rc = -1; /* read error occurred */
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

  gasneti_assert(strlen(filename) < limit);
  return mkstemp(filename);
}

#ifdef GASNETI_BT_LADEBUG
  static int gasneti_bt_ladebug(int fd) {
    #if GASNETI_THREADS
      const char fmt[] = "echo 'set $stoponattach; attach %d; show thread *; where thread *; quit' | %s '%s'"; 
    #else
      const char fmt[] = "echo 'set $stoponattach; attach %d; where; quit' | %s '%s'"; 
    #endif
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    /* Try to be smart if not in same place as at configure time */
    const char *ladebug = (access(LADEBUG_PATH, X_OK) ? "ladebug" : LADEBUG_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, (int)getpid(), ladebug, gasneti_exename_bt);
    if ((rc < 0) || (rc >= sizeof(cmd))) return -1;
    return gasneti_system_redirected(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_DBX
  static int gasneti_bt_dbx(int fd) {
    /* dbx's thread support is poor and not easily scriptable */
    const char fmt[] = "echo 'attach %d; where; quit' | %s '%s'";  
    static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ];
    const char *dbx = (access(DBX_PATH, X_OK) ? "dbx" : DBX_PATH);
    int rc = snprintf(cmd, sizeof(cmd), fmt, (int)getpid(), dbx, gasneti_exename_bt);
    if ((rc < 0) || (rc >= sizeof(cmd))) return -1;
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
    if ((rc < 0) || (rc >= sizeof(cmd))) return -1;
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
    if ((rc < 0) || (rc >= sizeof(cmd))) return -1;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_GSTACK
  static int gasneti_bt_gstack(int fd) {
    static char cmd[12 + GASNETI_BT_PATHSZ];
    const char *gstack = (access(GSTACK_PATH, X_OK) ? "gstack" : GSTACK_PATH);
    int rc = snprintf(cmd, sizeof(cmd), "%s %i", gstack, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) return -1;
    return gasneti_system_redirected_coprocess(cmd, fd);
  }
#endif

#ifdef GASNETI_BT_GDB
  static int gasneti_bt_gdb(int fd) {
    /* Change "backtrace" to "backtrace full" to also see local vars from each frame */
    #if GASNETI_THREADS
      const char commands[] = "\ninfo threads\nthread apply all backtrace 50\ndetach\nquit\n";
    #else
      const char commands[] = "\nbacktrace 50\ndetach\nquit\n";
    #endif
    const char shell_rm[]  = "shell rm ";
    const char fmt[] = "%s -nx -batch -x %s '%s' %d";
    static char cmd[sizeof(fmt) + 3*GASNETI_BT_PATHSZ];
    char filename[GASNETI_BT_PATHSZ];
    const char *gdb = (access(GDB_PATH, X_OK) ? "gdb" : GDB_PATH);
    int rc;

    /* Build gdb commands file, since it won't take commands on stdin */
    {
      int tmpfd, len;

      tmpfd = gasneti_bt_mkstemp(filename,sizeof(filename));
      if (tmpfd < 0) return -1;

      rc = -1;

      len = sizeof(shell_rm) - 1;
      if (len != write(tmpfd, shell_rm, len)) goto out;

      len = strlen(filename);
      if (len != write(tmpfd, filename, len)) goto out;

      len = sizeof(commands) - 1;
      if (len != write(tmpfd, commands, len)) goto out;

      if (0 != close(tmpfd)) goto out;
    }

    rc = snprintf(cmd, sizeof(cmd), fmt, gdb, filename, gasneti_exename_bt, (int)getpid());
    if ((rc < 0) || (rc >= sizeof(cmd))) {
      rc = -1;
      goto out;
    }

    rc = gasneti_system_redirected(cmd, fd);

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
    int have_addr2line = 0;
    entries = backtrace(btaddrs, MAXBT);
    #if HAVE_BACKTRACE_SYMBOLS
      fnnames = backtrace_symbols(btaddrs, entries);
    #endif
    #if defined(ADDR2LINE_PATH) && !GASNETI_NO_FORK
      { FILE *fp = fopen(ADDR2LINE_PATH,"r"); /* make sure the executable is actually there */
        if (fp) { have_addr2line = 1; fclose(fp); }
        else {
          const char *msg = "*** Warning: "ADDR2LINE_PATH" is unavailable to translate symbols\n";
          gasneti_bt_rc_unused = write(fd, msg, strlen(msg));
        }
      }
    #endif
    for (i=0; i < entries; i++) {
      /* XXX: what if the write()s fail? */
      static char linebuf[16];
      snprintf(linebuf, sizeof(linebuf), "%i: ", i);
      gasneti_bt_rc_unused = write(fd, linebuf, strlen(linebuf));

      if (fnnames) {
        gasneti_bt_rc_unused = write(fd, fnnames[i], strlen(fnnames[i]));
        gasneti_bt_rc_unused = write(fd, " ", 1);
      }

      #if defined(ADDR2LINE_PATH) && !GASNETI_NO_FORK
        if (have_addr2line)
        /* use addr2line when available to retrieve symbolic info */
        #define XLBUF 64 /* even as short as 2 bytes is still safe */
        { const char fmt[] = "%s -f -e '%s' %p";
          static char cmd[sizeof(fmt) + 2*GASNETI_BT_PATHSZ + 10];
          static char xlstr[XLBUF];
          FILE *xlate;
          int rc;
          xlstr[0] = '\0';
          rc = snprintf(cmd, sizeof(cmd), fmt, ADDR2LINE_PATH, gasneti_exename_bt, btaddrs[i]);
          if ((rc < 0) || (rc >= sizeof(cmd))) {
            return -1;
          }
          xlate = popen(cmd, "r");
          if (xlate) {
            while (fgets(xlstr, sizeof(xlstr), xlate)) {
              size_t len = strlen(xlstr);
              if (xlstr[len-1] == '\n') xlstr[len-1] = ' ';
              gasneti_bt_rc_unused = write(fd, xlstr, len);
            }
            pclose(xlate);
          }
        }
        #undef XLBUF
      #endif

      gasneti_bt_rc_unused = write(fd, "\n", 1);
    }
    /* if (fnnames) free(fnnames); */
    return 0;
  }
#endif

/* table of known/detected backtrace mechanisms */
static gasnett_backtrace_type_t gasneti_backtrace_mechanisms[] = {
  #ifdef GASNETI_BT_LADEBUG
  { "LADEBUG", GASNETI_BT_LADEBUG, 1 },
  #endif
  #ifdef GASNETI_BT_GSTACK
  { "GSTACK", GASNETI_BT_GSTACK, 1 },
  #endif
  #ifdef GASNETI_BT_GDB
  { "GDB", GASNETI_BT_GDB, 1 },
  #endif
  #ifdef GASNETI_BT_DBX
  { "DBX", GASNETI_BT_DBX, 0 },
  #endif
  #ifdef GASNETI_BT_EXECINFO
  { "EXECINFO", GASNETI_BT_EXECINFO, 1 },
  #endif
  #ifdef GASNETI_BT_PRINTSTACK
  { "PRINTSTACK", GASNETI_BT_PRINTSTACK, 1 },
  #endif
  #ifdef GASNETI_BT_IDB
  { "IDB", GASNETI_BT_IDB, 1 },
  #endif
  #ifdef GASNETI_BT_PGDBG
  { "PGDBG", GASNETI_BT_PGDBG, 1 },
  #endif
  { NULL, NULL, 0 } /* Space for registration of optional user mechanism */
};
static int gasneti_backtrace_mechanism_count = /* excludes the NULL */
   (sizeof(gasneti_backtrace_mechanisms)/sizeof(gasneti_backtrace_mechanisms[0])) - 1;

static int gasneti_backtrace_isinit = 0;
static int gasneti_backtrace_userenabled = 0;
#ifndef GASNETT_BUILDING_TOOLS
static int gasneti_backtrace_userdisabled = 0;
#endif
static const char *gasneti_backtrace_list = 0;
GASNETT_TENTATIVE_EXTERN
const char *(*gasneti_backtraceid_fn)(void); /* allow client override of backtrace line prefix */
gasnett_backtrace_type_t gasnett_backtrace_user; /* allow client provided backtrace function */
extern void gasneti_backtrace_init(const char *exename) {
  static int user_is_init = 0;

  gasneti_qualify_path(gasneti_exename_bt, exename);

  gasneti_backtrace_userenabled = gasneti_getenv_yesno_withdefault("GASNET_BACKTRACE",0);
#ifndef GASNETT_BUILDING_TOOLS
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
    GASNETI_UNUSED_UNLESS_THREADS int th;
    int i;
    btlist_def[0] = '\0';
    #if GASNETI_THREADS
      for (th = 1; th >= 0; th--) 
    #endif
      {
        for (i = 0; i < gasneti_backtrace_mechanism_count; ++i) {
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
          if (!strcmp(gasneti_backtrace_mechanisms[i].name,btsel)) {
            retval = (*gasneti_backtrace_mechanisms[i].fnp)(tmpfd);
            break;
          }
        }
        if (i == gasneti_backtrace_mechanism_count) {
          fprintf(stderr, "WARNING: GASNET_BACKTRACE_TYPE=%s unrecognized or unsupported - ignoring..\n", btsel);
          fflush(stderr);
        } else if (retval == 0) {
	  static char linebuf[1024];
	  char *p = linebuf;
	  int len = sizeof(linebuf);
          if (gasneti_backtraceid_fn) {
            strcpy(p, (*gasneti_backtraceid_fn)());
            len -= strlen(p);
            p += strlen(p);
          } else *p = '\0';

	  /* Send to requested destination (and tracefile if any) */
	  GASNETT_TRACE_PRINTF_FORCE("========== BEGIN BACKTRACE ==========");
	  rewind(file);
	  while (fgets(p, len, file)) {
            /* XXX: what if this write() fails? */
            gasneti_bt_rc_unused = write(fd, linebuf, strlen(linebuf)); /* w/ node prefix */
            GASNETT_TRACE_PRINTF_FORCE("%s",p);/* w/o node prefix */
	  }
	  GASNETT_TRACE_PRINTF_FORCE("========== END BACKTRACE ==========");
          gasneti_flush_streams();
          break;
        } else { /* backtrace attempt failed - retry with next mechanism */
	  rewind(file);
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
              fprintf(stderr, "WARNING: unknown signal %s in GASNET_NO_CATCH_SIGNAL\n", w);  
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
#ifndef GASNETT_BUILDING_TOOLS
  if (gasneti_backtrace_userdisabled) {
    return 1; /* User turned off backtrace, so don't whine */
  } else
#endif
  if (gasneti_backtrace_userenabled) {
    return gasneti_print_backtrace(fd);
  } else if (gasneti_backtrace_mechanism_count && !noticeshown) {
    fprintf(stderr, "NOTICE: Before reporting bugs, run with GASNET_BACKTRACE=1 in the environment to generate a backtrace. \n");
    fflush(stderr);
    noticeshown = 1;
    return 1;
  } else {
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
    snprintf(buf, bufsz, "%s%llu%s", neg, (unsigned long long)(val/divisor), unit);
  } else if (divisor == -1) {
    if (*neg) val = -val;
    snprintf(buf, bufsz, "0x%llx", (unsigned long long)val);
  } else gasneti_fatalerror("internal error in gasneti_format_number");
  return buf;
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
GASNETT_TENTATIVE_EXTERN
const char * (*gasnett_decode_envval_fn)(const char *);
GASNETT_TENTATIVE_EXTERN
int (*gasneti_verboseenv_fn)(void);
gasneti_getenv_fn_t *gasneti_conduit_getenv = NULL;
char *gasneti_globalEnv = NULL;

extern char *gasneti_getenv(const char *keyname) {
  char *retval = NULL;

  if (keyname && gasneti_conduit_getenv) {
    /* highest priority given to conduit-specific getenv */
    retval = (*gasneti_conduit_getenv)(keyname);
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
  
  if (retval && gasnett_decode_envval_fn && /* check if environment value needs decoding */
      strcmp(keyname, "GASNET_DISABLE_ENVDECODE") &&
      strcmp(keyname, "GASNET_VERBOSEENV")) { /* prevent inf recursion */ 
    retval = (char *)((*gasnett_decode_envval_fn)(retval));
  }

  GASNETT_TRACE_PRINTF("gasnet_getenv(%s) => '%s'",
                          (keyname?keyname:"NULL"),(retval?retval:"NULL"));

  return retval;
}

/* indicate whether GASNET_VERBOSEENV reporting is enabled on this node 
   1 = yes, 0 = no, -1 = not yet / don't know
*/
extern int gasneti_verboseenv(void) {
  if (gasneti_verboseenv_fn) return (*gasneti_verboseenv_fn)();
  else return !!gasneti_getenv("GASNET_VERBOSEENV");
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
    GASNETI_UNUSED_UNLESS_THREADS
    static gasneti_mutex_t envmutex = GASNETI_MUTEX_INITIALIZER;
    static gasneti_verboseenv_t *displaylist = NULL;
    static gasneti_verboseenv_t *displaylist_tail = NULL;
    static int notyet = 1;
    gasneti_verboseenv_t *p;
    char tmpstr[255];
    char *displaystr = tmpstr;
    int width = MAX(10,55 - strlen(key) - strlen(displayval));
    int len = snprintf(tmpstr, sizeof(tmpstr), "ENV parameter: %s = %s%*s", key, displayval, width, dflt);
    if (len >= sizeof(tmpstr)) { /* Too long for the static buffer */
      displaystr = malloc(len + 1);
      snprintf(displaystr, len+1, "ENV parameter: %s = %s%*s", key, displayval, width, dflt);
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
          fprintf(stderr, "%s\n", displaystr);
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
          fprintf(stderr, "%s\n", p->displaystr);
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
  const char *rawval;
  if (!gasneti_verboseenv() && !GASNETT_TRACE_ENABLED) return;

  snprintf(valstr, sizeof(valstr), "%g", val);
  rawval = gasneti_getenv(key);

  if (is_dflt || !strcmp(rawval,valstr)) { /* Use the numerical value */
    strcpy(displayval, valstr);
  } else { /* Use both the environment string and numerical value when they differ textually */
    snprintf(displayval, sizeof(displayval), "%s (%s)", gasneti_getenv(key), valstr);
  }
  gasneti_envstr_display(key, displayval, is_dflt);
}
extern void gasneti_envint_display(const char *key, int64_t val, int is_dflt, int is_mem_size) {
  char valstr[80];
  char displayval[80];
  const char *rawval;
  if (!gasneti_verboseenv() && !GASNETT_TRACE_ENABLED) return;

  gasneti_format_number(val, valstr, 80, is_mem_size);
  rawval = gasneti_getenv(key);

  if (is_dflt) { /* Use the numerical value */
    strcpy(displayval, valstr);
  } else if (!strcmp(rawval,valstr)) {
    strcpy(displayval, valstr);
  } else { /* Use the environment string and numerical value */
    snprintf(displayval, sizeof(displayval), "%s (%s)", gasneti_getenv(key), valstr);
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
    strncpy(s, retval, 10); s[9] = '\0';
    for (i = 0; i < 10; i++) s[i] = toupper(s[i]);
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
    char *endptr;
    retval = strtod(envval, &endptr);
    is_dflt = 0;
    if (endptr != envval) {
      while (*endptr && isspace(*endptr)) endptr++; /* Skip whitespace */
      if (*endptr == '/') {
        char *endptr2;
        double denom = strtod(1+endptr, &endptr2);
        if ((denom != 0) && (endptr2 != (1+endptr))) {
          for (endptr = endptr2; *endptr && isspace(*endptr); endptr++) {/* Skip whitespace */}
          retval /= denom;
        } else {
          /* endptr is left pointing at '/', triggering rejection below */
        }
      }
    }
    if ((endptr == envval) || (*endptr != '\0')) { /* match was empty or has trailing non-whitespace */
      gasneti_fatalerror("If used, environment variable '%s' must be a valid floating point value or fraction", keyname);
    }
  }

  gasneti_envdbl_display(keyname, retval, is_dflt);
  return retval;
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
  #ifdef __USE_GNU
    /* workaround an annoying glibc header bug, which erroneously declares get/setrlimit to take 
       the enum type __rlimit_resource_t, instead of int as required by POSIX */
   #if PLATFORM_COMPILER_GNU && PLATFORM_COMPILER_VERSION_GE(4,0,0)
    /* Gcc-4.x gives a sequence-point warning on the (,) form despite the comma operator.
     * So, we use a GCC-specific statement-expressions construct instead. */
    #define RLIM_CALL(fnname,structname) ({void *_fp=(void *)&fnname; (int (*)(int,structname *))_fp;})
   #else
    void *_fp;
    #define RLIM_CALL(fnname,structname) ( (_fp=(void *)&fnname), *(int (*)(int,structname *))_fp)
   #endif
  #else
    #define RLIM_CALL(fnname,structname) fnname
  #endif

  #define SET_RLIMITS(structname, getrlimit, setrlimit) do {                                    \
    structname oldval,newval;                                                                   \
    if (RLIM_CALL(getrlimit,structname)(res, &oldval)) {                                        \
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
        gasneti_assert(newval.rlim_cur <= newval.rlim_max);                                     \
        newval.rlim_cur = newval.rlim_max;                                                      \
        snprintf(newvalstr, sizeof(newvalstr), "%llu", (unsigned long long)newval.rlim_cur);    \
      }                                                                                         \
      if (newval.rlim_cur != oldval.rlim_cur) {                                                 \
        if (RLIM_CALL(setrlimit,structname)(res, &newval)) {                                    \
          GASNETT_TRACE_PRINTF("gasnett_maximize_rlimit:                                        \
            "#setrlimit"(%s, %s) failed: %s", lim_desc, newvalstr, strerror(errno));            \
        } else {                                                                                \
          GASNETT_TRACE_PRINTF("gasnett_maximize_rlimit:                                        \
          "#setrlimit"(%s, %s) raised limit from %llu", lim_desc, newvalstr,                    \
          (unsigned long long)oldval.rlim_cur);                                                 \
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
  #elif defined(GASNETI_HAVE_BGQ_INLINES) && 0 /* correct, but sysconf() gives same result */
      { 
        const uint64_t sprg7 = mfspr(SPRN_SPRG7RO);
        const uint8_t ppn = (sprg7 >> 8) & 0xff; /* Byte 6 is processes per node: 1,2,4,8,16,32 or 64 */
        hwprocs = 64 / ppn; /* XXX: this counts all SMT threads as cpus */
      }
  #elif PLATFORM_OS_MTA
      hwprocs = 0; /* appears to be no way to query CPU count */
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
  #if PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX
    #define _BUFSZ        120
    { FILE *fp;
      char line[_BUFSZ+1];

      if ((fp = fopen("/proc/meminfo", "r")) == NULL)
        gasneti_fatalerror("Failed to open /proc/meminfo in gasneti_getPhysMemSz()");

      while (fgets(line, _BUFSZ, fp)) {
        unsigned long memul = 0;
        unsigned long long memull = 0;
        /* MemTotal: on 2.4 and 2.6 kernels - preferred because less chance of scanf overflow */
        if (sscanf(line, "MemTotal: %lu kB", &memul) > 0 && memul > 0) {
          retval = ((uint64_t)memul) * 1024;
        }
        /* Mem: only on 2.4 kernels */
        else if (sscanf(line, "Mem: %llu", &memull) > 0 && memull > 0 && !retval) {
          retval = (uint64_t)memull;
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
void gasneti_set_affinity_default(int rank) {
  #if HAVE_PLPA
  {
    gasneti_plpa_cpu_set_t mask;
    int cpus = gasneti_set_affinity_cpus();

    if (cpus == 1) {
      /* NO-OP on single-processor platform */
      return;
    }
    
    /* Try a GET first to check for support */
    if_pf (ENOSYS == gasneti_plpa_sched_getaffinity(0, sizeof(mask), &mask)) {
      /* becomes a NO-OP */
      return;
    }
    
    {
      int local_rank = rank % cpus;
      PLPA_CPU_ZERO(&mask);
      PLPA_CPU_SET(local_rank, &mask);
      gasneti_assert_zeroret(gasneti_plpa_sched_setaffinity(0, sizeof(mask), &mask));
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
      gasneti_assert_zeroret(processor_bind(P_LWPID, P_MYID, avail_cpus[local_rank], NULL));
    }
  }
  #else
    /* No implementation -> NO-OP */
    return;
  #endif
}
#ifndef GASNETC_SET_AFFINITY
  #define GASNETC_SET_AFFINITY(rank) gasneti_set_affinity_default(rank)
#else
  /* Will use conduit-specific GASNETC_SET_AFFINITY() */
#endif
void gasneti_set_affinity(int rank) {
  GASNETT_TRACE_PRINTF("gasnett_set_affinity(%d)", rank);
  GASNETC_SET_AFFINITY(rank);
}
/* ------------------------------------------------------------------------------------ */
/* hostname query */
/* get MAXHOSTNAMELEN */ 
#if PLATFORM_OS_SOLARIS 
#include <netdb.h>
#elif defined(GASNETI_HAVE_BGQ_INLINES)
 #ifdef GASNETI_DEFINE__INLINE__
   #define __INLINE__ GASNETI_DEFINE__INLINE__
 #endif
 #include <hwi/include/common/uci.h>
 #include <firmware/include/personality.h>
 #ifdef GASNETI_DEFINE__INLINE__
   #undef __INLINE__
 #endif
 #undef MAXHOSTNAMELEN
 #define MAXHOSTNAMELEN 19
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
  GASNETI_UNUSED_UNLESS_THREADS
  static gasneti_mutex_t hnmutex = GASNETI_MUTEX_INITIALIZER;
  static int firsttime = 1;
  static char hostname[MAXHOSTNAMELEN];
  gasneti_mutex_lock(&hnmutex);
    if (firsttime) {
    #if GASNETI_HAVE_BGQ_INLINES
      uint64_t cc_uci;
      unsigned int proc;
      { /* Need entire Personality struct to extract the UCI  */
        Personality_t pers;
        int rc = CNK_SPI_SYSCALL_2(GET_PERSONALITY, (uintptr_t)&pers, (uint64_t)sizeof(pers));
        if (rc)
          gasnett_fatalerror("gasneti_gethostname() failed to get hostname: aborting");
        cc_uci = pers.Kernel_Config.UCI;
      }
      gasneti_assert(BG_UCI_GET_COMPONENT(cc_uci) == BG_UCI_Component_ComputeCardOnNodeBoard);
      { /* Extract process rank from SPRG7 */
        const uint64_t sprg7 = mfspr(SPRN_SPRG7RO);
        uint8_t ppn = (sprg7 >> 8) & 0xff; /* Byte 6 is processes per node: 1,2,4,8,16,32 or 64 */
        uint8_t cpu = (sprg7 & 0x3f); /* Byte 7 is logical processor id: 0 ... 63 */
        /* Shift the "process-local" bits out of the processor id */
        while (ppn & 0x3f) { ppn <<= 1; cpu >>= 1; }
        proc = cpu;
      }
      /* Rrc-Mm-Nnn-Jjj-Ppp.  All but "-Ppp" is standard BG/Q component naming. */
      snprintf(hostname, MAXHOSTNAMELEN, "R%1x%1x-M%1u-N%02u-J%02u-P%02u",
                         (unsigned int)BG_UCI_GET_ROW(cc_uci),
                         (unsigned int)BG_UCI_GET_COLUMN(cc_uci),
                         (unsigned int)BG_UCI_GET_MIDPLANE(cc_uci),
                         (unsigned int)BG_UCI_GET_NODE_BOARD(cc_uci),
                         (unsigned int)BG_UCI_GET_COMPUTE_CARD(cc_uci),
                         (unsigned int)proc
              );
    #else
      if (gethostname(hostname, MAXHOSTNAMELEN))
        gasnett_fatalerror("gasneti_gethostname() failed to get hostname: aborting");
    #endif
      hostname[MAXHOSTNAMELEN - 1] = '\0';
      firsttime = 0;
    }
  gasneti_mutex_unlock(&hnmutex);
  return hostname;
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
GASNETI_ALWAYS_INLINE(gasneti_count0s_xform1) GASNETI_CONST
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
GASNETI_ALWAYS_INLINE(gasneti_count0s_xform2) GASNETI_CONST
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
GASNETI_ALWAYS_INLINE(gasneti_count0s_nzs_aligned_region) GASNETI_PURE
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
GASNETI_ALWAYS_INLINE(gasneti_count0s_copy_bytes)
int gasneti_count0s_copy_bytes(void * GASNETI_RESTRICT dst, const void * GASNETI_RESTRICT src, size_t bytes) {
  int non_zeros = 0;
  uint8_t *d = dst;
  const uint8_t *s = src;
  gasneti_assert(bytes < SIZEOF_VOID_P);

  switch (bytes) {
  #if PLATFORM_ARCH_64
    case 7: non_zeros  = !!(*(d++) = *(s++));
    case 6: non_zeros += !!(*(d++) = *(s++));
    case 5: non_zeros += !!(*(d++) = *(s++));
    case 4: non_zeros += !!(*(d++) = *(s++));
    case 3: non_zeros += !!(*(d++) = *(s++));
  #else
    case 3: non_zeros  = !!(*(d++) = *(s++));
  #endif
    case 2: non_zeros += !!(*(d++) = *(s++));
    case 1: non_zeros += !!(*(d++) = *(s++));
  }
  return non_zeros;
}

/* Copy and count non-zero bytes w/ both dst and src word-aligned */
GASNETI_ALWAYS_INLINE(gasneti_count0s_copy_dstsrc_aligned)
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
GASNETI_ALWAYS_INLINE(gasneti_count0s_copy_dst_aligned)
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

#if (PLATFORM_ARCH_X86 || PLATFORM_ARCH_X86_64) && \
    (PLATFORM_OS_LINUX || PLATFORM_OS_CNL)
extern double gasneti_calibrate_tsc(void) {
  static int firstTime = 1;
  static double Tick = 0.0; /* Inverse GHz */
  FILE *fp = NULL;
  char input[512]; /* 256 is too small for "flags" line in /proc/cpuino */
  double MHz = 0.0;

 if_pf (firstTime) {
  fp = fopen("/proc/cpuinfo","r");
  if (!fp) gasneti_fatalerror("*** ERROR: Failure in fopen('/proc/cpuinfo','r')=%s",strerror(errno));

  /* First pass gets speed from /proc/cpuinfo */
  while (!feof(fp) && fgets(input, sizeof(input), fp)) {
    if (strstr(input,"cpu MHz")) {
      char *p = strchr(input,':');
      if (p) MHz = atof(p+1);
      gasneti_assert(MHz > 1 && MHz < 100000); /* ensure it looks reasonable */
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
      gasneti_assert(MHz > 1 && MHz < 10000); /* ensure it looks reasonable */

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

  gasneti_sync_writes();
  firstTime = 0;
 } else gasneti_sync_reads();

  return Tick;
}
#endif
/* ------------------------------------------------------------------------------------ */

