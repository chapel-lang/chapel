/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_tools.h $
 * Description: GASNet Tools library 
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#ifndef _GASNET_TOOLS_H
#define _GASNET_TOOLS_H
#define _IN_GASNET_TOOLS_H
#define _INCLUDED_GASNET_TOOLS_H
#if !defined(_INCLUDED_GASNETEX_H) && \
    (defined(GASNET_SEQ) || defined(GASNET_PARSYNC) || defined(GASNET_PAR))
  #error Objects that use both GASNet and GASNet tools must   \
         include gasnetex.h before gasnet_tools.h 
#endif

#ifdef __cplusplus
  extern "C" { // cannot use GASNETI_BEGIN_EXTERNC here due to a header dependency cycle
#endif

/* Recognized (mutually-exclusive) definitions for threading behavior:
   GASNETT_THREAD_SAFE - may be defined by client to enable thread-safety. 
     For gasnet.h clients, this is also enabled by GASNET_PAR(SYNC)
   GASNETT_THREAD_SINGLE - may be defined by client to explcitly disable thread-safety.
     For gasnet.h clients, this is also enabled by GASNET_SEQ
   GASNETT_LITE_MODE - tools-lite mode, for tools-only clients that want threading neutrality
      only provides the timer and membar interfaces
   If none of the above definitions are present, then thread-safety defaults to
     THREAD_SAFE if _REENTRANT or _THREAD_SAFE are defined and/or pthread.h is included, 
     and THREAD_SINGLE otherwise.
*/
#if defined(GASNETT_LITE_MODE) + defined(GASNETT_THREAD_SAFE) + defined(GASNETT_THREAD_SINGLE) > 1
  #error You must define at most one of: GASNETT_THREAD_SAFE, GASNETT_THREAD_SINGLE, GASNETT_LITE_MODE
#endif
#if defined(GASNETT_THREAD_SAFE) && defined(GASNET_SEQ)
  #error Conflicting threading definitions
#endif
#if defined(GASNETT_THREAD_SINGLE) && (defined(GASNET_PAR) || defined(GASNET_PARSYNC))
  #error Conflicting threading definitions
#endif
#ifdef GASNETT_LITE_MODE
  #undef  GASNETT_LITE_MODE
  #define GASNETT_LITE_MODE 1
  #define GASNETT_THREAD_MODEL LITE
  #ifdef _INCLUDED_GASNETEX_H
    #error GASNETT_LITE_MODE not supported for libgasnet clients
  #endif
#elif defined(GASNETT_THREAD_SAFE) ||                             \
      defined(GASNET_PARSYNC) || defined(GASNET_PAR) ||           \
      (!defined(GASNET_SEQ) && !defined(GASNETT_THREAD_SINGLE) && \
       (defined(_REENTRANT) || defined(_THREAD_SAFE) ||           \
        defined(PTHREAD_MUTEX_INITIALIZER)))
  #undef GASNETT_THREAD_SAFE
  #define GASNETT_THREAD_SAFE 1
  #define GASNETT_THREAD_MODEL PAR
  /* headers should test for GASNETI_THREADS, not GASNETT_THREAD_SAFE */
  #ifndef GASNETI_THREADS 
  #define GASNETI_THREADS 1
  #endif
#else
  #undef  GASNETT_THREAD_SINGLE
  #define GASNETT_THREAD_SINGLE 1
  #define GASNETT_THREAD_MODEL SEQ
#endif

#include <gasnet_config.h>

/* public spec version numbers */
#define GASNETT_SPEC_VERSION_MAJOR GASNETI_TOOLS_SPEC_VERSION_MAJOR
#define GASNETT_SPEC_VERSION_MINOR GASNETI_TOOLS_SPEC_VERSION_MINOR
#define GASNETT_RELEASE_VERSION_MAJOR GASNET_RELEASE_VERSION_MAJOR
#define GASNETT_RELEASE_VERSION_MINOR GASNET_RELEASE_VERSION_MINOR
#define GASNETT_RELEASE_VERSION_PATCH GASNET_RELEASE_VERSION_PATCH

#include <gasnet_basic.h>

GASNETI_BEGIN_NOWARN

#include <gasnet_toolhelp.h>

/* allow conduit-specific tool helpers (eg elan timers) */
#ifdef GASNETI_TOOLS_HELPER
#include _STRINGIFY(GASNETI_TOOLS_HELPER)
#endif

/* following preserves include ordering of gasnet_help.h */
#include <gasnet_membar.h>
#if !GASNETT_LITE_MODE
#include <gasnet_atomicops.h>
#endif
#include <gasnet_timer.h>

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------------------ */
/* basic purely syntactic features */
#define GASNETT_IDENT(identName, identText) GASNETI_IDENT(identName, identText)

#if defined(GASNET_PAGESIZE)
  #define GASNETT_PAGESIZE GASNET_PAGESIZE
#elif defined(GASNETI_PAGESIZE)
  #define GASNETT_PAGESIZE GASNETI_PAGESIZE
#endif

#if defined(GASNETI_PAGESHIFT)
  #define GASNETT_PAGESHIFT GASNETI_PAGESHIFT
#endif

#define GASNETT_CACHE_LINE_BYTES GASNETI_CACHE_LINE_BYTES
#define GASNETT_SYSTEM_TUPLE     GASNETI_SYSTEM_TUPLE
#define GASNETT_CONFIGURE_ARGS   GASNETI_CONFIGURE_ARGS

#if !GASNETI_COMPILER_IS_CC
#define GASNETT_CONFIGURE_MISMATCH 1  /* indicate build compiler mismatches configure $CC */
#endif

/* platform identifiers for the compiler detected at *configure*-time */
#define GASNETT_PLATFORM_COMPILER_IDSTR    GASNETI_PLATFORM_COMPILER_IDSTR
#define GASNETT_PLATFORM_COMPILER_FAMILYID GASNETI_PLATFORM_COMPILER_FAMILYID
#define GASNETT_PLATFORM_COMPILER_ID       GASNETI_PLATFORM_COMPILER_ID 
#define GASNETT_PLATFORM_COMPILER_VERSION  GASNETI_PLATFORM_COMPILER_VERSION 

/* various configure-detected C compiler features available in only some compilers */
#define GASNETT_INLINE                  GASNETI_INLINE
#define GASNETT_NEVER_INLINE            GASNETI_NEVER_INLINE
#define GASNETT_RESTRICT                GASNETI_RESTRICT
#define GASNETT_USED                    GASNETI_USED
#define GASNETT_NORETURN                GASNETI_NORETURN
#define GASNETT_NORETURNP               GASNETI_NORETURNP
#define GASNETT_MALLOC                  GASNETI_MALLOC
#define GASNETT_MALLOCP                 GASNETI_MALLOCP
#define GASNETT_PURE                    GASNETI_PURE
#define GASNETT_PUREP                   GASNETI_PUREP
#define GASNETT_CONST                   GASNETI_CONST
#define GASNETT_CONSTP                  GASNETI_CONSTP
#define GASNETT_HOT                     GASNETI_HOT
#define GASNETT_COLD                    GASNETI_COLD
#define GASNETT_DEPRECATED              GASNETI_DEPRECATED
#define GASNETT_WARN_UNUSED_RESULT      GASNETI_WARN_UNUSED_RESULT
#define GASNETT_FORMAT_PRINTF           GASNETI_FORMAT_PRINTF
#define GASNETT_FORMAT_PRINTF_FUNCPTR   GASNETI_FORMAT_PRINTF_FUNCPTR

#define GASNETT_FALLTHROUGH             GASNETI_FALLTHROUGH

#define GASNETT_CURRENT_FUNCTION        GASNETI_CURRENT_FUNCTION

#define GASNETT_BEGIN_EXTERNC           GASNETI_BEGIN_EXTERNC
#define GASNETT_END_EXTERNC             GASNETI_END_EXTERNC
#define GASNETT_EXTERNC                 GASNETI_EXTERNC

#define gasnett_constant_p              gasneti_constant_p

#define gasnett_unreachable             gasneti_unreachable

/* ------------------------------------------------------------------------------------ */
/* portable memory barriers */

#define gasnett_local_wmb()          gasneti_local_wmb()
#define gasnett_local_rmb()          gasneti_local_rmb()
#define gasnett_local_mb()           gasneti_local_mb()
#define gasnett_weak_wmb()           gasneti_sync_writes()
#define gasnett_weak_rmb()           gasneti_sync_reads()
#define gasnett_weak_mb()            gasneti_sync_mem()
#define gasnett_compiler_fence()     gasneti_compiler_fence()
#define GASNETT_MEMBAR_CONFIG        GASNETI_MEMBAR_CONFIG

/* ------------------------------------------------------------------------------------ */
/* microsecond-resolution sleep */

#define gasnett_nsleep(ns_delay)     gasneti_nsleep(ns_delay)

/* ------------------------------------------------------------------------------------ */
/* portable high-performance, low-overhead timers */

#define gasnett_tick_t               gasneti_tick_t
#define GASNETT_TICK_MIN             GASNETI_TICK_MIN
#define GASNETT_TICK_MAX             GASNETI_TICK_MAX
#define gasnett_ticks_to_us(ticks)   gasneti_ticks_to_us(ticks)
#define gasnett_ticks_to_ns(ticks)   gasneti_ticks_to_ns(ticks)
#define gasnett_ticks_now()          gasneti_ticks_now()
#define gasnett_tick_granularityus() gasneti_tick_granularity()
#define gasnett_tick_overheadus()    gasneti_tick_overhead()
#define GASNETT_TIMER_CONFIG         GASNETI_TIMER_CONFIG
#define gasnett_gettimeofday_us()    gasneti_gettimeofday_us()
#ifdef GASNETI_USING_GETTIMEOFDAY
#define GASNETT_USING_GETTIMEOFDAY 1
#endif

#if !GASNETT_LITE_MODE
/* ------------------------------------------------------------------------------------ */
/* portable atomic increment/decrement */

#define GASNETT_ATOMIC_NONE			GASNETI_ATOMIC_NONE
#define GASNETT_ATOMIC_RMB_PRE			GASNETI_ATOMIC_RMB_PRE
#define GASNETT_ATOMIC_RMB_POST			GASNETI_ATOMIC_RMB_POST
#define GASNETT_ATOMIC_RMB_POST_IF_TRUE		GASNETI_ATOMIC_RMB_POST_IF_TRUE
#define GASNETT_ATOMIC_RMB_POST_IF_FALSE	GASNETI_ATOMIC_RMB_POST_IF_FALSE
#define GASNETT_ATOMIC_WMB_PRE			GASNETI_ATOMIC_WMB_PRE
#define GASNETT_ATOMIC_WMB_POST			GASNETI_ATOMIC_WMB_POST
#define GASNETT_ATOMIC_REL			GASNETI_ATOMIC_REL
#define GASNETT_ATOMIC_ACQ			GASNETI_ATOMIC_ACQ
#define GASNETT_ATOMIC_ACQ_IF_TRUE		GASNETI_ATOMIC_ACQ_IF_TRUE
#define GASNETT_ATOMIC_ACQ_IF_FALSE		GASNETI_ATOMIC_ACQ_IF_FALSE
#define GASNETT_ATOMIC_MB_PRE			GASNETI_ATOMIC_MB_PRE
#define GASNETT_ATOMIC_MB_POST			GASNETI_ATOMIC_MB_POST

#define gasnett_atomic_val_t			gasneti_atomic_val_t
#define GASNETT_ATOMIC_MAX			GASNETI_ATOMIC_MAX

#define gasnett_atomic_sval_t			gasneti_atomic_sval_t
#define gasnett_atomic_signed(v)		gasneti_atomic_signed(v)
#define GASNETT_ATOMIC_SIGNED_MIN		GASNETI_ATOMIC_SIGNED_MIN
#define GASNETT_ATOMIC_SIGNED_MAX		GASNETI_ATOMIC_SIGNED_MAX

/* strong atomics always map to true atomic operations, regardless of threading mode */
#if GASNETI_ATOMICOPS_NOT_SIGNALSAFE
  #define GASNETT_STRONGATOMIC_NOT_SIGNALSAFE 1
#endif
#define gasnett_strongatomic_t               gasneti_atomic_t
#define gasnett_strongatomic_read(p,f)       gasneti_atomic_read(p,f)
#define gasnett_strongatomic_init(v)         gasneti_atomic_init(v)
#define gasnett_strongatomic_set(p,v,f)      gasneti_atomic_set(p,v,f)
#define gasnett_strongatomic_increment(p,f)  gasneti_atomic_increment(p,f)
#define gasnett_strongatomic_decrement(p,f)  gasneti_atomic_decrement(p,f)
#define gasnett_strongatomic_decrement_and_test(p,f)  \
                                       gasneti_atomic_decrement_and_test(p,f)
#ifdef GASNETI_HAVE_ATOMIC_CAS
  #define GASNETT_HAVE_STRONGATOMIC_CAS 1
  #define gasnett_strongatomic_compare_and_swap(p,oldval,newval,f)  \
                                       gasneti_atomic_compare_and_swap(p,oldval,newval,f)
  #define gasnett_strongatomic_swap(p,val,f) gasneti_atomic_swap(p,val,f)
#endif

#ifdef GASNETI_HAVE_ATOMIC_ADD_SUB
  #define GASNETT_HAVE_STRONGATOMIC_ADD_SUB 1
  #define gasnett_strongatomic_add(p,op,f)      gasneti_atomic_add(p,op,f)
  #define gasnett_strongatomic_subtract(p,op,f) gasneti_atomic_subtract(p,op,f)
#endif

#if GASNETI_ATOMIC32_NOT_SIGNALSAFE
  #define GASNETT_STRONGATOMIC32_NOT_SIGNALSAFE 1
#endif
#define gasnett_strongatomic32_t              gasneti_atomic32_t
#define gasnett_strongatomic32_read(p,f)      gasneti_atomic32_read(p,f)
#define gasnett_strongatomic32_init(v)        gasneti_atomic32_init(v)
#define gasnett_strongatomic32_set(p,v,f)     gasneti_atomic32_set(p,v,f)
#define gasnett_strongatomic32_compare_and_swap(p,oldval,newval,f)  \
                                        gasneti_atomic32_compare_and_swap(p,oldval,newval,f)
#define gasnett_strongatomic32_swap(p,v,f)     gasneti_atomic32_swap(p,v,f)
#define gasnett_strongatomic32_add(p,v,f)      gasneti_atomic32_add(p,v,f)
#define gasnett_strongatomic32_subtract(p,v,f) gasneti_atomic32_subtract(p,v,f)
#define gasnett_strongatomic32_increment(p,f)  gasneti_atomic32_increment(p,f)
#define gasnett_strongatomic32_decrement(p,f)  gasneti_atomic32_decrement(p,f)
#define gasnett_strongatomic32_decrement_and_test(p,f) \
                                               gasneti_atomic32_decrement_and_test(p,f)

#if GASNETI_ATOMIC64_NOT_SIGNALSAFE
  #define GASNETT_STRONGATOMIC64_NOT_SIGNALSAFE 1
#endif
#define gasnett_strongatomic64_t              gasneti_atomic64_t
#define gasnett_strongatomic64_read(p,f)      gasneti_atomic64_read(p,f)
#define gasnett_strongatomic64_init(v)        gasneti_atomic64_init(v)
#define gasnett_strongatomic64_set(p,v,f)     gasneti_atomic64_set(p,v,f)
#define gasnett_strongatomic64_compare_and_swap(p,oldval,newval,f)  \
                                          gasneti_atomic64_compare_and_swap(p,oldval,newval,f)
#define gasnett_strongatomic64_swap(p,v,f)     gasneti_atomic64_swap(p,v,f)
#define gasnett_strongatomic64_add(p,v,f)      gasneti_atomic64_add(p,v,f)
#define gasnett_strongatomic64_subtract(p,v,f) gasneti_atomic64_subtract(p,v,f)
#define gasnett_strongatomic64_increment(p,f)  gasneti_atomic64_increment(p,f)
#define gasnett_strongatomic64_decrement(p,f)  gasneti_atomic64_decrement(p,f)
#define gasnett_strongatomic64_decrement_and_test(p,f) \
                                               gasneti_atomic64_decrement_and_test(p,f)

/* regular atomics map to either true atomics or sequential stubs, based on thread mode */
#if GASNETI_THREADS
  /* PAR, PARSYNC and thread-safe tools clients */
  #ifdef GASNETI_HAVE_ATOMIC_CAS
    #define GASNETT_HAVE_ATOMIC_CAS 1
  #endif
  #ifdef GASNETI_HAVE_ATOMIC_ADD_SUB
    #define GASNETT_HAVE_ATOMIC_ADD_SUB 1
  #endif
  #define _gasnett_atomic_cons(_id)       gasneti_atomic##_id
#else /* GASNET_SEQ and non-thread-safe tools clients */
  /* safe to use weak atomics here, because the client is single-threaded and 
     should only be modifying atomics from the host CPU (using these calls). 
     TODO: consider exposing "signal-safe" atomics (only avail on some platforms)
  */
  #ifdef GASNETI_HAVE_WEAKATOMIC_CAS
    #define GASNETT_HAVE_ATOMIC_CAS 1
  #endif
  #ifdef GASNETI_HAVE_WEAKATOMIC_ADD_SUB
    #define GASNETT_HAVE_ATOMIC_ADD_SUB 1
  #endif
  #define _gasnett_atomic_cons(_id)       gasneti_weakatomic##_id
#endif

#define gasnett_atomic_t                  _gasnett_atomic_cons(_t)
#define gasnett_atomic_read(p,f)          _gasnett_atomic_cons(_read)(p,f)
#define gasnett_atomic_init(v)            _gasnett_atomic_cons(_init)(v)
#define gasnett_atomic_set(p,v,f)         _gasnett_atomic_cons(_set)(p,v,f)
#define gasnett_atomic_increment(p,f)     _gasnett_atomic_cons(_increment)(p,f)
#define gasnett_atomic_decrement(p,f)     _gasnett_atomic_cons(_decrement)(p,f)
#define gasnett_atomic_decrement_and_test(p,f)  \
                                          _gasnett_atomic_cons(_decrement_and_test)(p,f)
#ifdef GASNETT_HAVE_ATOMIC_CAS
  #define gasnett_atomic_compare_and_swap(p,oldval,newval,f)  \
                                          _gasnett_atomic_cons(_compare_and_swap)(p,oldval,newval,f)
  #define gasnett_atomic_swap(p,val,f)    _gasnett_atomic_cons(_swap)(p,val,f)
#endif
#ifdef GASNETT_HAVE_ATOMIC_ADD_SUB
  #define gasnett_atomic_add(p,op,f)      _gasnett_atomic_cons(_add)(p,op,f)
  #define gasnett_atomic_subtract(p,op,f) _gasnett_atomic_cons(_subtract)(p,op,f)
#endif

#define gasnett_atomic32_t                _gasnett_atomic_cons(32_t)
#define gasnett_atomic32_read(p,f)        _gasnett_atomic_cons(32_read)(p,f)
#define gasnett_atomic32_init(v)          _gasnett_atomic_cons(32_init)(v)
#define gasnett_atomic32_set(p,v,f)       _gasnett_atomic_cons(32_set)(p,v,f)
#define gasnett_atomic32_increment(p,f)   _gasnett_atomic_cons(32_increment)(p,f)
#define gasnett_atomic32_decrement(p,f)   _gasnett_atomic_cons(32_decrement)(p,f)
#define gasnett_atomic32_decrement_and_test(p,f) \
                                          _gasnett_atomic_cons(32_decrement_and_test)(p,f)
#define gasnett_atomic32_compare_and_swap(p,oldval,newval,f)  \
                                          _gasnett_atomic_cons(32_compare_and_swap)(p,oldval,newval,f)
#define gasnett_atomic32_swap(p,v,f)      _gasnett_atomic_cons(32_swap)(p,v,f)
#define gasnett_atomic32_add(p,v,f)       _gasnett_atomic_cons(32_add)(p,v,f)
#define gasnett_atomic32_subtract(p,v,f)  _gasnett_atomic_cons(32_subtract)(p,v,f)

#define gasnett_atomic64_t                _gasnett_atomic_cons(64_t)
#define gasnett_atomic64_read(p,f)        _gasnett_atomic_cons(64_read)(p,f)
#define gasnett_atomic64_init(v)          _gasnett_atomic_cons(64_init)(v)
#define gasnett_atomic64_set(p,v,f)       _gasnett_atomic_cons(64_set)(p,v,f)
#define gasnett_atomic64_increment(p,f)   _gasnett_atomic_cons(64_increment)(p,f)
#define gasnett_atomic64_decrement(p,f)   _gasnett_atomic_cons(64_decrement)(p,f)
#define gasnett_atomic64_decrement_and_test(p,f) \
                                          _gasnett_atomic_cons(64_decrement_and_test)(p,f)
#define gasnett_atomic64_compare_and_swap(p,oldval,newval,f)  \
                                          _gasnett_atomic_cons(64_compare_and_swap)(p,oldval,newval,f)
#define gasnett_atomic64_swap(p,v,f)      _gasnett_atomic_cons(64_swap)(p,v,f)
#define gasnett_atomic64_add(p,v,f)       _gasnett_atomic_cons(64_add)(p,v,f)
#define gasnett_atomic64_subtract(p,v,f)  _gasnett_atomic_cons(64_subtract)(p,v,f)

#ifdef GASNETI_USE_GENERIC_ATOMICOPS
#define GASNETT_USING_GENERIC_ATOMICOPS
#endif
#define GASNETT_ATOMIC_CONFIG         GASNETI_ATOMIC_CONFIG
#define GASNETT_ATOMIC32_CONFIG       GASNETI_ATOMIC32_CONFIG
#define GASNETT_ATOMIC64_CONFIG       GASNETI_ATOMIC64_CONFIG

/* ------------------------------------------------------------------------------------ */
/* environment utilities */

#define gasnett_format_number             gasneti_format_number
#define gasnett_parse_int                 gasneti_parse_int
#define gasnett_setenv                    gasneti_setenv
#define gasnett_unsetenv                  gasneti_unsetenv
#define gasnett_getenv                    gasneti_getenv
#define gasnett_getenv_withdefault        gasneti_getenv_withdefault
#define gasnett_getenv_yesno_withdefault  gasneti_getenv_yesno_withdefault
#define gasnett_getenv_int_withdefault    gasneti_getenv_int_withdefault
#define gasnett_getenv_dbl_withdefault    gasneti_getenv_dbl_withdefault
#define gasnett_verboseenv                gasneti_verboseenv
#define gasnett_envstr_display            gasneti_envstr_display
#define gasnett_envint_display            gasneti_envint_display
#define gasnett_envdbl_display            gasneti_envdbl_display

/* ------------------------------------------------------------------------------------ */
/* misc tools utilities */

/* return a (possibly empty) string of any configuration options that might negtively impact performance */
extern const char *gasnett_performance_warning_str(void);

/* return a string representation of the compiled library's package version */
extern const char *gasnett_release_version_str(void);

/* return an monotonically advancing integral representation of the compiled library's package version */
extern uint64_t gasnett_release_version(void);

#define gasnett_sched_yield     gasneti_sched_yield 
#define gasnett_cpu_count       gasneti_cpu_count
#define gasnett_flush_streams   gasneti_flush_streams
#define gasnett_close_streams   gasneti_close_streams
#define gasnett_getPhysMemSz    gasneti_getPhysMemSz
#define gasnett_fatalerror      gasneti_fatalerror
#define gasnett_killmyprocess   gasneti_killmyprocess
#define gasnett_current_loc     gasneti_current_loc
#define gasnett_sighandlerfn_t  gasneti_sighandlerfn_t
#define gasnett_reghandler      gasneti_reghandler
#define gasnett_checksum        gasneti_checksum
#define gasnett_isLittleEndian  gasneti_isLittleEndian
#define gasnett_set_affinity    gasneti_set_affinity
#define gasnett_gethostname     gasneti_gethostname
#define gasnett_spinloop_hint   gasneti_spinloop_hint
#define gasnett_freezeForDebuggerNow gasneti_freezeForDebuggerNow
#define gasnett_freezeForDebuggerErr gasneti_freezeForDebuggerErr
#define gasnett_backtrace_init gasneti_backtrace_init
#define gasnett_print_backtrace gasneti_print_backtrace
#define gasnett_print_backtrace_ifenabled gasneti_print_backtrace_ifenabled
#define gasnett_count0s_copy    gasneti_count0s_copy
#define gasnett_count0s         gasneti_count0s
#define gasnett_count0s_uintptr_t gasneti_count0s_uintptr_t
#define gasnett_count0s_uint32_t  gasneti_count0s_uint32_t
#define gasnett_count0s_uint64_t  gasneti_count0s_uint64_t

#define gasnett_mutex_t               gasneti_mutex_t
#define gasnett_mutex_init            gasneti_mutex_init
#define gasnett_mutex_destroy         gasneti_mutex_destroy
#define gasnett_mutex_destroy_ignoreerr gasneti_mutex_destroy_ignoreerr
#define GASNETT_MUTEX_INITIALIZER     GASNETI_MUTEX_INITIALIZER
#define gasnett_mutex_lock            gasneti_mutex_lock
#define gasnett_mutex_trylock         gasneti_mutex_trylock
#define gasnett_mutex_unlock          gasneti_mutex_unlock
#define gasnett_mutex_assertlocked    gasneti_mutex_assertlocked
#define gasnett_mutex_assertunlocked  gasneti_mutex_assertunlocked

#define gasnett_rwlock_t              gasneti_rwlock_t
#define gasnett_rwlock_init           gasneti_rwlock_init
#define gasnett_rwlock_destroy        gasneti_rwlock_destroy
#define gasnett_rwlock_rdlock         gasneti_rwlock_rdlock
#define gasnett_rwlock_wrlock         gasneti_rwlock_wrlock
#define gasnett_rwlock_tryrdlock      gasneti_rwlock_tryrdlock
#define gasnett_rwlock_trywrlock      gasneti_rwlock_trywrlock
#define gasnett_rwlock_unlock         gasneti_rwlock_unlock
#define GASNETT_RWLOCK_INITIALIZER    GASNETI_RWLOCK_INITIALIZER
#define gasnett_rwlock_assertlocked   gasneti_rwlock_assertlocked 
#define gasnett_rwlock_assertrdlocked gasneti_rwlock_assertrdlocked
#define gasnett_rwlock_assertwrlocked gasneti_rwlock_assertwrlocked
#define gasnett_rwlock_assertunlocked gasneti_rwlock_assertunlocked

#define gasnett_cond_t               gasneti_cond_t
#define gasnett_cond_init            gasneti_cond_init
#define gasnett_cond_destroy         gasneti_cond_destroy
#define GASNETT_COND_INITIALIZER     GASNETI_COND_INITIALIZER
#define gasnett_cond_wait            gasneti_cond_wait
#define gasnett_cond_signal          gasneti_cond_signal
#define gasnett_cond_broadcast       gasneti_cond_broadcast

#define GASNETT_THREADKEY_DECLARE                 GASNETI_THREADKEY_DECLARE
#define GASNETT_THREADKEY_DEFINE                  GASNETI_THREADKEY_DEFINE
#define gasnett_threadkey_get(key)                gasneti_threadkey_get(key)
#define gasnett_threadkey_set(key,newval)         gasneti_threadkey_set(key,newval)
#define gasnett_threadkey_init(key)               gasneti_threadkey_init(key)
#define gasnett_threadkey_get_noinit(key)         gasneti_threadkey_get_noinit(key)
#define gasnett_threadkey_set_noinit(key,newval)  gasneti_threadkey_set_noinit(key,newval)

/* backtrace extensibility support */
typedef struct {
  const char *name;        /* upper-case display name of backtrace function */
  int (* fnp)(int);   /* pointer to backtrace function */
  int threadsupport; /* does backtrace function handle threads correctly? 
                              -ie backtrace the calling thread and optionally others as well */
} gasnett_backtrace_type_t;
GASNETI_TENTATIVE_LIBRARY
gasnett_backtrace_type_t gasnett_backtrace_user;

/* ------------------------------------------------------------------------------------ */
/* GASNet tracing/stats support (automatically stubbed out when libgasnet absent) */

#if defined(_INCLUDED_GASNETEX_H) && defined(GASNET_SRCLINES)
  #define GASNETT_TRACE_SETSOURCELINE      GASNETI_TRACE_SETSOURCELINE
  #define GASNETT_TRACE_GETSOURCELINE      GASNETI_TRACE_GETSOURCELINE
  #define GASNETT_TRACE_FREEZESOURCELINE   GASNETI_TRACE_FREEZESOURCELINE
  #define GASNETT_TRACE_UNFREEZESOURCELINE GASNETI_TRACE_UNFREEZESOURCELINE
#else
  #define GASNETT_TRACE_SETSOURCELINE(file,line)    ((void)0)
  #define GASNETT_TRACE_GETSOURCELINE(pfile,pline)  ((void)0)
  #define GASNETT_TRACE_FREEZESOURCELINE()          ((void)0)
  #define GASNETT_TRACE_UNFREEZESOURCELINE()        ((void)0)
#endif

#if PLATFORM_COMPILER_PGI
  #include <stdarg.h>
#endif
GASNETI_FORMAT_PRINTF(_gasnett_trace_printf_noop,1,2,
static void _gasnett_trace_printf_noop(const char *_format, ...)) {
  #if PLATFORM_COMPILER_PGI
    va_list _ap; va_start(_ap,_format); va_end(_ap); /* avoid a silly warning */
  #endif
  return; 
}
#if defined(GASNET_TRACE) && !GASNETI_BUILDING_TOOLS && !defined(__cplusplus)
  GASNETT_FORMAT_PRINTF_FUNCPTR(_gasnett_trace_printf,1,2,
  GASNETI_TENTATIVE_CLIENT void (*_gasnett_trace_printf)(const char *_format, ...));
  GASNETT_FORMAT_PRINTF_FUNCPTR(_gasnett_trace_printf_force,1,2,
  GASNETI_TENTATIVE_CLIENT void (*_gasnett_trace_printf_force)(const char *_format, ...));
  #if PLATFORM_COMPILER_PGI /* bug 1703 - workaround a PGI bug using Gnu-style variadic macros which PGI supports */
    #define GASNETT_TRACE_PRINTF(args...) \
            (_gasnett_trace_printf ? _gasnett_trace_printf(args) : _gasnett_trace_printf_noop(args))
    #define GASNETT_TRACE_PRINTF_FORCE(args...) \
            (_gasnett_trace_printf_force ? _gasnett_trace_printf_force(args) : _gasnett_trace_printf_noop(args))
  #else
    #define GASNETT_TRACE_PRINTF \
            (*(_gasnett_trace_printf?_gasnett_trace_printf:&_gasnett_trace_printf_noop))
    #define GASNETT_TRACE_PRINTF_FORCE \
            (*(_gasnett_trace_printf_force?_gasnett_trace_printf_force:&_gasnett_trace_printf_noop))
  #endif

  #ifdef _INCLUDED_GASNETEX_H
    #define GASNETT_TRACE_ENABLED       GASNETI_TRACE_ENABLED(H)
    #define GASNETT_TRACE_GETMASK()     GASNETI_TRACE_GETMASK()
    #define GASNETT_TRACE_SETMASK(mask) GASNETI_TRACE_SETMASK(mask)
    #define GASNETT_TRACE_GET_TRACELOCAL()        GASNETI_TRACE_GET_TRACELOCAL()
    #define GASNETT_TRACE_SET_TRACELOCAL(newval)  GASNETI_TRACE_SET_TRACELOCAL(newval)
  #else
    GASNETI_TENTATIVE_CLIENT int (*_gasnett_trace_enabled)(char _tracecat);
    #define GASNETT_TRACE_ENABLED       (_gasnett_trace_enabled?_gasnett_trace_enabled('H'):0)
    #define GASNETT_TRACE_GETMASK()               ""
    #define GASNETT_TRACE_SETMASK(mask)           ((void)0)
    #define GASNETT_TRACE_GET_TRACELOCAL()        (0)
    #define GASNETT_TRACE_SET_TRACELOCAL(newval)  ((void)0)
  #endif
#else
  #define GASNETT_TRACE_ENABLED  0
  #define GASNETT_TRACE_PRINTF        _gasnett_trace_printf_noop
  #define GASNETT_TRACE_PRINTF_FORCE  _gasnett_trace_printf_noop
  #define GASNETT_TRACE_GETMASK()               ""
  #define GASNETT_TRACE_SETMASK(mask)           ((void)0)
  #define GASNETT_TRACE_GET_TRACELOCAL()        (0)
  #define GASNETT_TRACE_SET_TRACELOCAL(newval)  ((void)0)
#endif

#if defined(_INCLUDED_GASNETEX_H) && defined(GASNET_STATS)
  /* GASNETT_STATS_INIT can be called at any time to register a callback function, which 
     will be invoked at stats dumping time (provided H stats are enabled)
     and passed a printf-like function that can be used to write output into the stats
   */
  #define GASNETT_STATS_INIT(callbackfn) \
    (gasnett_stats_callback = (callbackfn), GASNETI_STATS_ENABLED(H))
  #define GASNETT_STATS_GETMASK()     GASNETI_STATS_GETMASK()
  #define GASNETT_STATS_SETMASK(mask) GASNETI_STATS_SETMASK(mask)
#else
  #define GASNETT_STATS_INIT(callbackfn) 0
  #define GASNETT_STATS_GETMASK()     ""
  #define GASNETT_STATS_SETMASK(mask) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
/* misc internal libgasnet-specific features we wish to expose when available */
#if defined(_INCLUDED_GASNETEX_H) 
  /* these tools ONLY available when linking a libgasnet.a */
  #ifdef HAVE_MMAP
    extern void *gasneti_mmap(uintptr_t _segsize);
    #define gasnett_mmap(sz) gasneti_mmap(sz)
  #else
    #define gasnett_mmap(sz) gasnett_fatalerror("gasnett_mmap not available")
  #endif
  extern int gasneti_run_diagnostics(int _iters, int _threadcnt,
                                     const char *_testsections,
                                     gex_TM_t _myteam, void* _myseg,
                                     gex_Rank_t _peer, void* _peerseg);
  extern void gasneti_diagnostic_gethandlers(gex_AM_Entry_t **_htable, int *_htable_cnt);
  #define gasnett_run_diagnostics gasneti_run_diagnostics
  #define gasnett_diagnostic_gethandlers gasneti_diagnostic_gethandlers

  #define GASNETT_FAST_ALIGNED_MEMCPY   GASNETE_FAST_ALIGNED_MEMCPY
  #define GASNETT_FAST_UNALIGNED_MEMCPY GASNETE_FAST_UNALIGNED_MEMCPY
  #define GASNETT_VALUE_ASSIGN          GASNETE_VALUE_ASSIGN
  #define GASNETT_VALUE_RETURN          GASNETE_VALUE_RETURN

  #define GASNETT_MAX_THREADS GASNETI_MAX_THREADS 

  #if GASNET_DEBUGMALLOC
    #define gasnett_debug_malloc(sz)      gasneti_extern_malloc(sz) 
    #define gasnett_debug_realloc(ptr,sz) gasneti_extern_realloc((ptr),(sz))
    #define gasnett_debug_calloc(N,S)     gasneti_extern_calloc((N),(S))
    #define gasnett_debug_free(ptr)       gasneti_extern_free(ptr)  
    #define gasnett_debug_strdup(s)       gasneti_extern_strdup(s)
    #define gasnett_debug_strndup(s,n)    gasneti_extern_strndup(s,n)
    #define gasnett_debug_memcheck(ptr)   gasneti_memcheck(ptr)
    #define gasnett_debug_memcheck_one()  gasneti_memcheck_one()
    #define gasnett_debug_memcheck_all()  gasneti_memcheck_all()
    #define gasnett_heapstats_t           gasneti_heapstats_t
    #define gasnett_getheapstats(pstat)   gasneti_getheapstats(pstat)
  #endif

  /* VIS string formatting */
  #define gasnett_format_memveclist_bufsz gasneti_format_memveclist_bufsz 
  #define gasnett_format_memveclist       gasneti_format_memveclist       
  #define gasnett_format_putvgetv_bufsz   gasneti_format_putvgetv_bufsz   
  #define gasnett_format_putvgetv         gasneti_format_putvgetv         
  #define gasnett_format_addrlist_bufsz   gasneti_format_addrlist_bufsz   
  #define gasnett_format_addrlist         gasneti_format_addrlist         
  #define gasnett_format_putigeti_bufsz   gasneti_format_putigeti_bufsz   
  #define gasnett_format_putigeti         gasneti_format_putigeti         
  #define gasnett_format_strides_bufsz    gasneti_format_strides_bufsz    
  #define gasnett_format_strides          gasneti_format_strides          
  #define gasnett_format_putsgets_bufsz   gasneti_format_putsgets_bufsz   
  #define gasnett_format_putsgets         gasneti_format_putsgets         

  /* Enum/mask string formatting */
  #define gasnett_format_dt               gasneti_format_dt
  #define gasnett_format_op               gasneti_format_op
  #define gasnett_format_ti               gasneti_format_ti

  #if defined(GASNETI_ATOMIC_LOCK_TBL_DECLS)
    GASNETI_ATOMIC_LOCK_TBL_DECLS(gasneti_hsl_atomic_, gex_HSL_)
  #endif
#else
  #define gasnett_mmap(sz)        gasnett_fatalerror("gasnett_mmap not available")

  #if defined(GASNETI_ATOMIC_LOCK_TBL_DECLS)
    GASNETI_ATOMIC_LOCK_TBL_DECLS(gasneti_pthread_atomic_, gasnett_mutex_)
  #endif
#endif

#endif /* !GASNETT_LITE_MODE */
/* ------------------------------------------------------------------------------------ */
/* Build config sanity checking */
#if defined(GASNET_NDEBUG)
  #define GASNETT_DEBUG_CONFIG nodebug
#else
  #define GASNETT_DEBUG_CONFIG debug
#endif
#define GASNETT_PTR_CONFIG            GASNETI_PTR_CONFIG

#define _GASNETT_LITE_CONFIG_STRING              \
       "RELEASE=" _STRINGIFY(GASNETI_RELEASE_VERSION) "," \
       "SPEC=" _STRINGIFY(GASNETT_SPEC_VERSION_MAJOR) "." \
               _STRINGIFY(GASNETT_SPEC_VERSION_MINOR) "," \
       "PTR=" _STRINGIFY(GASNETI_PTR_CONFIG) "," \
       _STRINGIFY(GASNETT_DEBUG_CONFIG) ","      \
       _STRINGIFY(GASNETT_THREAD_MODEL) ","      \
       _STRINGIFY(GASNETT_TIMER_CONFIG) ","      \
       _STRINGIFY(GASNETT_MEMBAR_CONFIG)
#if GASNETT_LITE_MODE
  #define GASNETT_CONFIG_STRING _GASNETT_LITE_CONFIG_STRING
#else
  #define GASNETT_CONFIG_STRING                \
       _GASNETT_LITE_CONFIG_STRING ","         \
       _STRINGIFY(GASNETT_ATOMIC_CONFIG) ","   \
       _STRINGIFY(GASNETT_ATOMIC32_CONFIG) "," \
       _STRINGIFY(GASNETT_ATOMIC64_CONFIG)
#endif

#define GASNETT_LINKCONFIG_IDIOTCHECK(name) _CONCAT(gasnett_linkconfig_idiotcheck_,name)
extern int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR));
extern int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR));
extern int GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH));
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_THREAD_MODEL);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_DEBUG_CONFIG);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_PTR_CONFIG);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_TIMER_CONFIG);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_MEMBAR_CONFIG);
#if !GASNETT_LITE_MODE
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC_CONFIG);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC32_CONFIG);
extern int GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC64_CONFIG);
#endif
static int *gasnett_linkconfig_idiotcheck(void);
#if !PLATFORM_COMPILER_TINY /* avoid a tinyc bug */
  #define GASNETI_IDIOTCHECK_RECURSIVE_REFERENCE 1
  static int *(*_gasnett_linkconfig_idiotcheck)(void) = &gasnett_linkconfig_idiotcheck;
#endif
GASNETT_USED
static int *gasnett_linkconfig_idiotcheck(void) 
{
  static int _val;
  _val +=  
        + GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR))
        + GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR))
        + GASNETT_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH))
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_THREAD_MODEL)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_DEBUG_CONFIG)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_PTR_CONFIG)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_TIMER_CONFIG)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_MEMBAR_CONFIG)
      #if !GASNETT_LITE_MODE
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC_CONFIG)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC32_CONFIG)
        + GASNETT_LINKCONFIG_IDIOTCHECK(GASNETT_ATOMIC64_CONFIG)
      #endif
        ;
  #if GASNETI_IDIOTCHECK_RECURSIVE_REFERENCE
  if (_gasnett_linkconfig_idiotcheck == &gasnett_linkconfig_idiotcheck)
    _val += *(*_gasnett_linkconfig_idiotcheck)();
  #endif
  return &_val;
}

/* ------------------------------------------------------------------------------------ */

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#undef _IN_GASNET_TOOLS_H
#endif
