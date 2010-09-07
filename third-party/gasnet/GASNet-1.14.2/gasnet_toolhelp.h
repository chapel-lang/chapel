/*   $Source: /var/local/cvs/gasnet/gasnet_toolhelp.h,v $
 *     $Date: 2010/04/16 23:58:26 $
 * $Revision: 1.52.2.2 $
 * Description: misc declarations needed by both gasnet_tools and libgasnet
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNET_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnet.h or gasnet_tools.h
#endif

#ifndef _GASNET_TOOLHELP_H
#define _GASNET_TOOLHELP_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#if GASNETI_THREADS
  #if PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX
   struct timespec; /* avoid an annoying warning on Linux */
  #endif
  #include <pthread.h>
#endif

#include <limits.h>
#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

#ifndef STDIN_FILENO
  #define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
  #define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
  #define STDERR_FILENO 2
#endif

GASNETI_BEGIN_EXTERNC

#if PLATFORM_OS_MTA
   #include <machine/runtime.h>
   #define _gasneti_sched_yield() mta_yield()
#elif defined(HAVE_SCHED_YIELD) && !PLATFORM_OS_BLRTS && !PLATFORM_OS_CATAMOUNT
   #include <sched.h>
   #define _gasneti_sched_yield() sched_yield()
#else
   #define _gasneti_sched_yield() (sleep(0),0)
#endif
#define gasneti_sched_yield() gasneti_assert_zeroret(_gasneti_sched_yield())

extern void gasneti_filesystem_sync(void);

#if PLATFORM_COMPILER_GNU_CXX /* bug 1681 */
  #define GASNETI_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__GNUC__) || defined(__FUNCTION__)
  #define GASNETI_CURRENT_FUNCTION __FUNCTION__
#elif defined(HAVE_FUNC) && !defined(__cplusplus)
  /* __func__ should also work for ISO C99 compilers */
  #define GASNETI_CURRENT_FUNCTION __func__
#else
  #define GASNETI_CURRENT_FUNCTION ""
#endif
extern char *gasneti_build_loc_str(const char *funcname, const char *filename, int linenum);
#define gasneti_current_loc gasneti_build_loc_str(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__)

/* gasneti_assert_always():
 * an assertion that never compiles away - for sanity checks in non-critical paths 
 */
#define gasneti_assert_always(expr) \
    (PREDICT_TRUE(expr) ? (void)0 : gasneti_fatalerror("Assertion failure at %s: %s", gasneti_current_loc, #expr))

/* gasneti_assert():
 * an assertion that compiles away in non-debug mode - for sanity checks in critical paths 
 */
#if GASNET_NDEBUG
  #define gasneti_assert(expr) ((void)0)
#else
  #define gasneti_assert(expr) gasneti_assert_always(expr)
#endif

/* gasneti_assert_zeroret(), gasneti_assert_nzeroret():
 * evaluate an expression (always), and in debug mode additionally 
 * assert that it returns zero or non-zero
 * useful for making system calls and checking the result
 */
#if GASNET_DEBUG
  #define gasneti_assert_zeroret(op) do {                   \
    int _retval = (op);                                     \
    if_pf(_retval) {                                        \
      /* can't use strdup() or gasneti_strdup() here. */    \
      char _tmp[128];                                       \
      strncpy(_tmp, strerror(_retval), sizeof(_tmp));       \
      _tmp[sizeof(_tmp)-1] = '\0';                          \
      gasneti_fatalerror(#op": %s(%i), errno=%s(%i) at %s", \
        _tmp, _retval, strerror(errno), errno,              \
        gasneti_current_loc);                               \
    }                                                       \
  } while (0)
  #define gasneti_assert_nzeroret(op) do {                  \
    int _retval = (op);                                     \
    if_pf(!_retval) {                                       \
      /* can't use strdup() or gasneti_strdup() here. */    \
      char _tmp[128];                                       \
      strncpy(_tmp, strerror(_retval), sizeof(_tmp));       \
      _tmp[sizeof(_tmp)-1] = '\0';                          \
      gasneti_fatalerror(#op": %s(%i), errno=%s(%i) at %s", \
        _tmp, _retval, strerror(errno), errno,              \
        gasneti_current_loc);                               \
    }                                                       \
  } while (0)
#else
  #define gasneti_assert_zeroret(op)  op
  #define gasneti_assert_nzeroret(op) op
#endif

#if GASNET_DEBUG
  #define GASNETI_UNUSED_UNLESS_DEBUG
#else
  #define GASNETI_UNUSED_UNLESS_DEBUG GASNETI_UNUSED
#endif
#if GASNETI_THREADS
  #define GASNETI_UNUSED_UNLESS_THREADS
#else
  #define GASNETI_UNUSED_UNLESS_THREADS GASNETI_UNUSED
#endif

/* return physical memory of machine
   on failure, failureIsFatal nonzero => fatal error, failureIsFatal zero => return 0 */
extern uint64_t gasneti_getPhysMemSz(int failureIsFatal); 

GASNETI_FORMAT_PRINTF(gasneti_fatalerror,1,2,
extern void gasneti_fatalerror(const char *msg, ...) GASNETI_NORETURN);
GASNETI_NORETURNP(gasneti_fatalerror)

extern void gasneti_killmyprocess(int exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasneti_killmyprocess)

extern void gasneti_freezeForDebuggerErr(void); /* freeze iff user enabled error freezing */
extern void gasneti_freezeForDebuggerNow(volatile int *flag, const char *flagsymname);
extern volatile int gasnet_frozen; /* export to simplify debugger restart */ 
extern void gasneti_qualify_path(char *path_out, const char *path_in);
extern void gasneti_backtrace_init(const char *exename);
extern int (*gasneti_print_backtrace_ifenabled)(int fd);
extern int gasneti_print_backtrace(int fd);
extern void gasneti_ondemand_init(void);

extern void gasneti_flush_streams(void); /* flush all open streams */
extern void gasneti_close_streams(void); /* close standard streams (for shutdown) */

extern int gasneti_cpu_count(void);

extern void gasneti_set_affinity(int rank);

const char *gasneti_gethostname(void); /* returns the current host name - dies with an error on failure */

extern int gasneti_isLittleEndian(void);

typedef void (*gasneti_sighandlerfn_t)(int);
gasneti_sighandlerfn_t gasneti_reghandler(int sigtocatch, gasneti_sighandlerfn_t fp);

/* return a fast but simple/insecure 64-bit checksum of arbitrary data */
extern uint64_t gasneti_checksum(const void *p, int numbytes);

/* ------------------------------------------------------------------------------------ */
/* Count zero bytes in a region w/ or w/o a memcpy(), or in a "register" */

extern size_t gasneti_count0s_copy(void * GASNETI_RESTRICT dst,
                                   const void * GASNETI_RESTRICT src,
                                   size_t len);
extern size_t gasneti_count0s(const void * src, size_t len);


GASNETI_INLINE(gasneti_count0s_uint32_t) GASNETI_CONST
int gasneti_count0s_uint32_t(uint32_t x) {
#if 0
  x |= (x >> 4); x |= (x >> 2); x |= (x >> 1);
  x &= 0x01010101UL;
  x += (x >> 16); x += (x >> 8);
  return sizeof(x) - (x & 0xf);
#else
  const uint32_t mask = 0x7f7f7f7fUL;
  uint32_t tmp;
  tmp = x & mask;
  tmp += mask;
  tmp |= x;
  tmp &= ~mask;
  tmp >>= 7;
  tmp += (tmp >> 16);
  tmp += (tmp >> 8);
  return sizeof(x) - (tmp & 0xf);
#endif
}

#if PLATFORM_ARCH_32
  GASNETI_INLINE(gasneti_count0s_uint64_t) GASNETI_CONST
  int gasneti_count0s_uint64_t(uint64_t x) {
    return gasneti_count0s_uint32_t(GASNETI_LOWORD(x)) + 
           gasneti_count0s_uint32_t(GASNETI_HIWORD(x));
  }
  #define gasneti_count0s_uintptr_t(x) gasneti_count0s_uint32_t(x)
#elif PLATFORM_ARCH_64
  GASNETI_INLINE(gasneti_count0s_uint64_t) GASNETI_CONST
  int gasneti_count0s_uint64_t(uint64_t x) {
  #if 0
    x |= (x >> 4); x |= (x >> 2); x |= (x >> 1);
    x &= 0x0101010101010101UL;
    x += (x >> 32); x += (x >> 16); x += (x >> 8);
    return sizeof(x) - (x & 0xf);
  #else
    const uint64_t mask = 0x7f7f7f7f7f7f7f7fULL;
    uint64_t tmp;
    tmp = x & mask;
    tmp += mask;
    tmp |= x;
    tmp &= ~mask;
    tmp >>= 7;
    tmp += (tmp >> 32);
    tmp += (tmp >> 16);
    tmp += (tmp >> 8);
    return sizeof(x) - (tmp & 0xf);
  #endif
  }
  #define gasneti_count0s_uintptr_t(x) gasneti_count0s_uint64_t(x)
#else
  #error "Unknown word size"
#endif

/* ------------------------------------------------------------------------------------ */
/* Error checking system mutexes -
     wrapper around pthread mutexes that provides extra support for 
     error checking when GASNET_DEBUG is defined
   See README-tools for important usage information.
  
   -DGASNETI_USE_TRUE_MUTEXES=1 will force gasneti_mutex_t to always
    use true locking (even under GASNET_SEQ or GASNET_PARSYNC config)
*/
#if GASNET_PAR || GASNETI_CONDUIT_THREADS || GASNETT_THREAD_SAFE
  /* need to use true locking if we have concurrent calls from multiple client threads 
     or if conduit has private threads that can run handlers */
  #define GASNETI_USE_TRUE_MUTEXES 1 
#elif defined(GASNETT_USE_TRUE_MUTEXES)
  #undef  GASNETI_USE_TRUE_MUTEXES
  #define GASNETI_USE_TRUE_MUTEXES GASNETT_USE_TRUE_MUTEXES
#elif !defined(GASNETI_USE_TRUE_MUTEXES)
  #define GASNETI_USE_TRUE_MUTEXES 0
#endif

#if PLATFORM_OS_CYGWIN || defined(GASNETI_FORCE_MUTEX_INITCLEAR)
  /* we're sometimes unable to call pthread_mutex_destroy when freeing a mutex
     some pthread implementations will fail to re-init a mutex
     (eg after a free and realloc of the associated mem) unless
     the contents are first cleared to zero
   */
  #define GASNETI_MUTEX_INITCLEAR(pm) memset(pm,0,sizeof(*(pm)))
#else
  #define GASNETI_MUTEX_INITCLEAR(pm) ((void)0)
#endif

#if !defined(GASNETI_BUG2231_WORKAROUND) && \
    (PLATFORM_OS_DARWIN && PLATFORM_ARCH_64 && \
     PLATFORM_COMPILER_PGI && PLATFORM_COMPILER_VERSION_LT(7,1,4))
  #define GASNETI_BUG2231_WORKAROUND       1
#endif
#if GASNETI_BUG2231_WORKAROUND
  #define GASNETI_BUG2231_WORKAROUND_PAD   char _bug2231_pad[64];
#else
  #define GASNETI_BUG2231_WORKAROUND_PAD  
#endif

#if GASNETI_USE_TRUE_MUTEXES && PLATFORM_OS_CYGWIN
  /* bug1847: Cygwin mutexes initialized using PTHREAD_MUTEX_INITIALIZER are unsafe upon first acquire */
  #define GASNETI_MUTEX_CAUTIOUS_INIT 1
#endif

#if GASNETI_MUTEX_CAUTIOUS_INIT
  /* implemented using fixed-width atomics to avoid a header dependency cycle */
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_FIELD       volatile int32_t initstep;
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER , 0
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl)   gasneti_mutex_cautious_init(pl) 
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(pl)  ( (pl)->initstep = 0, \
                                                   gasneti_mutex_cautious_init(pl) )
  extern void gasneti_mutex_cautious_init(/*gasneti_mutex_t*/void *pl);
#else
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_FIELD
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl)      ((void)0)
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(pl)       ((void)0)
#endif

#if GASNET_DEBUG || GASNETI_BUG2231_WORKAROUND || GASNETI_MUTEX_CAUTIOUS_INIT
  #define GASNETI_MUTEX_NOOWNER         ((GASNETI_THREADID_T)(uintptr_t)-1)
  #ifndef GASNETI_THREADIDQUERY
    /* allow conduit override of thread-id query */
    #if GASNETI_USE_TRUE_MUTEXES
      #define GASNETI_THREADID_T        pthread_t
      #define GASNETI_THREADIDQUERY()   pthread_self()
    #else
      #define GASNETI_THREADID_T        uintptr_t
      #define GASNETI_THREADIDQUERY()   ((uintptr_t)0)
    #endif
  #endif
  #if GASNETI_USE_TRUE_MUTEXES
    #include <pthread.h>
    typedef struct {
      volatile GASNETI_THREADID_T owner;
      pthread_mutex_t lock;
      _GASNETI_MUTEX_CAUTIOUS_INIT_FIELD
      GASNETI_BUG2231_WORKAROUND_PAD
    } gasneti_mutex_t;
    #if defined(PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP)
      /* These are faster, though less "featureful" than the default
       * mutexes on linuxthreads implementations which offer them.
       */
      #define _GASNETI_PTHREAD_MUTEX_INITIALIZER PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP
    #else
      #define _GASNETI_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
    #endif
    #define GASNETI_MUTEX_INITIALIZER { GASNETI_MUTEX_NOOWNER,                   \
                                        _GASNETI_PTHREAD_MUTEX_INITIALIZER       \
                                        _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER \
                                      }
    #define gasneti_mutex_lock(pl) do {                                        \
              _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl);                          \
              gasneti_assert(GASNETI_THREADIDQUERY() != GASNETI_MUTEX_NOOWNER);\
              gasneti_assert((pl)->owner != GASNETI_THREADIDQUERY());          \
              gasneti_assert_zeroret(pthread_mutex_lock(&((pl)->lock)));       \
              gasneti_assert((pl)->owner == GASNETI_MUTEX_NOOWNER);            \
              (pl)->owner = GASNETI_THREADIDQUERY();                           \
            } while (0)
    GASNETI_INLINE(gasneti_mutex_trylock) GASNETI_WARN_UNUSED_RESULT
    int gasneti_mutex_trylock(gasneti_mutex_t *pl) {
              int retval;
              _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl);
              gasneti_assert(GASNETI_THREADIDQUERY() != GASNETI_MUTEX_NOOWNER);
              gasneti_assert((pl)->owner != GASNETI_THREADIDQUERY());
              retval = pthread_mutex_trylock(&((pl)->lock));
              if (retval == EBUSY) return EBUSY;
              if (retval) gasneti_fatalerror("pthread_mutex_trylock()=%s",strerror(retval));
              gasneti_assert((pl)->owner == GASNETI_MUTEX_NOOWNER);
              (pl)->owner = GASNETI_THREADIDQUERY();
              return 0;
    }
    #define gasneti_mutex_unlock(pl) do {                                  \
              gasneti_assert( GASNETI_THREADIDQUERY() !=  GASNETI_MUTEX_NOOWNER); \
              gasneti_assert((pl)->owner == GASNETI_THREADIDQUERY());      \
              (pl)->owner = GASNETI_MUTEX_NOOWNER;                         \
              gasneti_assert_zeroret(pthread_mutex_unlock(&((pl)->lock))); \
            } while (0)
    #define gasneti_mutex_init(pl) do {                                       \
              GASNETI_MUTEX_INITCLEAR(&((pl)->lock));                         \
              gasneti_assert_zeroret(pthread_mutex_init(&((pl)->lock),NULL)); \
              (pl)->owner = GASNETI_MUTEX_NOOWNER;                            \
              _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(pl);                          \
            } while (0)
    #define gasneti_mutex_destroy_ignoreerr(pl) \
              pthread_mutex_destroy(&((pl)->lock))
    #define gasneti_mutex_destroy(pl) \
              gasneti_assert_zeroret(gasneti_mutex_destroy_ignoreerr(pl))
  #else /* GASNET_DEBUG non-pthread (error-check-only) mutexes */
    typedef struct {
      volatile GASNETI_THREADID_T owner;
    } gasneti_mutex_t;
    #define GASNETI_MUTEX_INITIALIZER   { GASNETI_MUTEX_NOOWNER }
    #define gasneti_mutex_lock(pl) do {                             \
              gasneti_assert((pl)->owner == GASNETI_MUTEX_NOOWNER); \
              (pl)->owner = GASNETI_THREADIDQUERY();                \
            } while (0)
    GASNETI_INLINE(gasneti_mutex_trylock) GASNETI_WARN_UNUSED_RESULT
    int gasneti_mutex_trylock(gasneti_mutex_t *pl) {
              gasneti_assert((pl)->owner == GASNETI_MUTEX_NOOWNER);
              (pl)->owner = GASNETI_THREADIDQUERY();
              return 0;
    }
    #define gasneti_mutex_unlock(pl) do {                             \
              gasneti_assert((pl)->owner == GASNETI_THREADIDQUERY()); \
              (pl)->owner = GASNETI_MUTEX_NOOWNER;                    \
            } while (0)
    #define gasneti_mutex_init(pl) do {                       \
              (pl)->owner = GASNETI_MUTEX_NOOWNER;            \
            } while (0)
    #define gasneti_mutex_destroy_ignoreerr(pl) 0
    #define gasneti_mutex_destroy(pl) ((void)0)
  #endif
  #define gasneti_mutex_assertlocked(pl)    gasneti_assert((pl)->owner == GASNETI_THREADIDQUERY())
  #define gasneti_mutex_assertunlocked(pl)  gasneti_assert((pl)->owner != GASNETI_THREADIDQUERY())
#else /* non-debug mutexes */
  #if GASNETI_USE_TRUE_MUTEXES
    #include <pthread.h>
    typedef pthread_mutex_t           gasneti_mutex_t;
    #if defined(PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP)
      /* These are faster, though less "featureful" than the default
       * mutexes on linuxthreads implementations which offer them.
       */
      #define GASNETI_MUTEX_INITIALIZER PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP
    #else
      #define GASNETI_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
    #endif
    #define gasneti_mutex_lock(pl)      pthread_mutex_lock(pl)
    #define gasneti_mutex_trylock(pl)   pthread_mutex_trylock(pl)
    #define gasneti_mutex_unlock(pl)    pthread_mutex_unlock(pl)
    #define gasneti_mutex_init(pl)      (GASNETI_MUTEX_INITCLEAR(pl),  \
                                         pthread_mutex_init((pl),NULL))
    #define gasneti_mutex_destroy_ignoreerr(pl)   pthread_mutex_destroy(pl)
    #define gasneti_mutex_destroy(pl)   gasneti_mutex_destroy_ignoreerr(pl)
  #else
    typedef char           gasneti_mutex_t;
    #define GASNETI_MUTEX_INITIALIZER '\0'
    #define gasneti_mutex_lock(pl)    ((void)0)
    #define gasneti_mutex_trylock(pl) 0
    #define gasneti_mutex_unlock(pl)  ((void)0)
    #define gasneti_mutex_init(pl)    ((void)0)
    #define gasneti_mutex_destroy_ignoreerr(pl) 0
    #define gasneti_mutex_destroy(pl) ((void)0)
  #endif
  #define gasneti_mutex_assertlocked(pl)    ((void)0)
  #define gasneti_mutex_assertunlocked(pl)  ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
/* gasneti_cond_t Condition variables - 
   Provides pthread_cond-like functionality, with error checking
   See README-tools for important usage information.
*/

#if GASNETI_USE_TRUE_MUTEXES
  typedef struct {
    pthread_cond_t cond;
    GASNETI_BUG2231_WORKAROUND_PAD
  } gasneti_cond_t;

  #define GASNETI_COND_INITIALIZER    { PTHREAD_COND_INITIALIZER }
  #define gasneti_cond_init(pc) do {                       \
      GASNETI_MUTEX_INITCLEAR(&((pc)->cond));                         \
      gasneti_assert_zeroret(pthread_cond_init(&((pc)->cond), NULL)); \
  } while (0)
  #define gasneti_cond_destroy(pc)    gasneti_assert_zeroret(pthread_cond_destroy(&((pc)->cond)))

  #if PLATFORM_ARCH_CRAYX1 /* bug 993 - workaround for buggy pthread library */
    static gasneti_cond_t const gasneti_cond_staticinitialized = GASNETI_COND_INITIALIZER;
    #define GASNETI_COND_INIT_CHECK(pc) \
      (!memcmp(&(gasneti_cond_staticinitialized.cond),&((pc)->cond),sizeof(pthread_cond_t)) ? \
        (void)pthread_cond_init(&((pc)->cond), NULL) : (void)0 )
  #else
    #define GASNETI_COND_INIT_CHECK(pc) ((void)0)
  #endif

  #define gasneti_cond_signal(pc) do {                 \
      GASNETI_COND_INIT_CHECK(pc);                     \
      gasneti_assert_zeroret(pthread_cond_signal(&((pc)->cond))); \
    } while (0)
  #define gasneti_cond_broadcast(pc) do {                 \
      GASNETI_COND_INIT_CHECK(pc);                        \
      gasneti_assert_zeroret(pthread_cond_broadcast(&((pc)->cond))); \
    } while (0)

  #if GASNET_DEBUG || GASNETI_BUG2231_WORKAROUND || GASNETI_MUTEX_CAUTIOUS_INIT
    #define gasneti_cond_wait(pc,pl)  do {                          \
      gasneti_assert((pl)->owner == GASNETI_THREADIDQUERY());       \
      (pl)->owner = GASNETI_MUTEX_NOOWNER;                          \
      GASNETI_COND_INIT_CHECK(pc);                                  \
      _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl);                       \
      gasneti_assert_zeroret(pthread_cond_wait(&((pc)->cond), &((pl)->lock))); \
      gasneti_assert((pl)->owner == GASNETI_MUTEX_NOOWNER);         \
      (pl)->owner = GASNETI_THREADIDQUERY();                        \
    } while (0)
  #else
    #define gasneti_cond_wait(pc,pl)  do {               \
      GASNETI_COND_INIT_CHECK(pc);                       \
      gasneti_assert_zeroret(pthread_cond_wait(&((pc)->cond), pl)); \
    } while (0)
  #endif
#else
  typedef char           gasneti_cond_t;
  #define GASNETI_COND_INITIALIZER  '\0'
  #define gasneti_cond_init(pc)       ((void)0)
  #define gasneti_cond_destroy(pc)    ((void)0)
  #define gasneti_cond_signal(pc)     ((void)0)
  #define gasneti_cond_broadcast(pc)  ((void)0)
  #define gasneti_cond_wait(pc,pl) \
      gasneti_fatalerror("There's only one thread: waiting on condition variable => deadlock")
#endif

/* ------------------------------------------------------------------------------------ */
/* Wrappers for thread-local data storage
   See README-tools for usage information.
*/
#define _GASNETI_THREADKEY_MAGIC 0xFF00ABCDEF573921ULL

#if GASNETI_THREADS
  #if GASNETI_HAVE_TLS_SUPPORT /* use __thread, if available */
    #define _GASNETI_THREADKEY_USES_TLS 1
  #else
    #define _GASNETI_THREADKEY_USES_PTHREAD_GETSPECIFIC 1
  #endif
#else /* no threads */
    #define _GASNETI_THREADKEY_USES_NOOP 1
#endif

#if _GASNETI_THREADKEY_USES_PTHREAD_GETSPECIFIC
  typedef struct { 
    #if GASNET_DEBUG
      uint64_t magic;
      #define _GASNETI_THREADKEY_MAGIC_INIT _GASNETI_THREADKEY_MAGIC,
    #else
      #define _GASNETI_THREADKEY_MAGIC_INIT
    #endif
      gasneti_mutex_t initmutex;
      volatile int isinit;
      pthread_key_t value;
  } _gasneti_threadkey_t;
  #define _GASNETI_THREADKEY_INITIALIZER \
    { _GASNETI_THREADKEY_MAGIC_INIT      \
      GASNETI_MUTEX_INITIALIZER,         \
      0 /* value field left NULL */ }
#else
  typedef void *_gasneti_threadkey_t;
  #define _GASNETI_THREADKEY_INITIALIZER NULL
#endif

#if GASNETI_THREADS
  GASNETI_NEVER_INLINE(_gasneti_threadkey_init, /* avoid inserting overhead for an uncommon path */
  static void _gasneti_threadkey_init(pthread_key_t *_value, gasneti_mutex_t *_initmutex, volatile int *_isinit)) {
    gasneti_mutex_lock(_initmutex);
      if (*_isinit == 0) {
        gasneti_assert_zeroret(pthread_key_create(_value,NULL));
        { /* need a wmb, but have to avoid a header dependency cycle */
          gasneti_mutex_t dummymutex = GASNETI_MUTEX_INITIALIZER;
          gasneti_mutex_lock(&dummymutex);gasneti_mutex_unlock(&dummymutex); 
        }
        *_isinit = 1;
      } 
    gasneti_mutex_unlock(_initmutex);
  }
#endif

#if _GASNETI_THREADKEY_USES_PTHREAD_GETSPECIFIC
  #define GASNETI_THREADKEY_DECLARE(key) \
    extern _gasneti_threadkey_t key
  #define GASNETI_THREADKEY_DEFINE(key) \
    _gasneti_threadkey_t key = _GASNETI_THREADKEY_INITIALIZER
#elif _GASNETI_THREADKEY_USES_TLS
  #if GASNETI_CONFIGURE_MISMATCH
    /* mismatched compilers can access TLS threadkeys defined in objects
       built by supported compiler via extern function call */
    #define GASNETI_THREADKEY_DECLARE(key)         \
      extern void *_gasneti_threadkey_get_##key(void); \
      extern void _gasneti_threadkey_set_##key(void *_val)
    /* bug 1947 - following only expanded when a configure-mismatched compiler is 
       DEFINING a threadkey - use pthread_getspecific in that case for safety
     */
    #define GASNETI_THREADKEY_DEFINE(key)                                                      \
      static pthread_key_t _gasneti_threadkey_##key##_value;                                   \
      static gasneti_mutex_t _gasneti_threadkey_##key##_initmutex = GASNETI_MUTEX_INITIALIZER; \
      static volatile int _gasneti_threadkey_##key##_isinit = 0;                               \
      extern void *_gasneti_threadkey_get_##key(void) {                                        \
        if (!_gasneti_threadkey_##key##_isinit)                                                \
           _gasneti_threadkey_init(&_gasneti_threadkey_##key##_value,                          \
                                   &_gasneti_threadkey_##key##_initmutex,                      \
                                   &_gasneti_threadkey_##key##_isinit);                        \
        return pthread_getspecific(_gasneti_threadkey_##key##_value);                          \
      }                                                                                        \
      extern void _gasneti_threadkey_set_##key(void *_val) {                                   \
        if (!_gasneti_threadkey_##key##_isinit)                                                \
           _gasneti_threadkey_init(&_gasneti_threadkey_##key##_value,                          \
                                   &_gasneti_threadkey_##key##_initmutex,                      \
                                   &_gasneti_threadkey_##key##_isinit);                        \
        gasneti_assert_zeroret(pthread_setspecific(_gasneti_threadkey_##key##_value, _val));   \
      }                                                                                        \
      GASNETI_THREADKEY_DECLARE(key)
  #else /* no mismatch */
    #define GASNETI_THREADKEY_DECLARE(key) \
      extern __thread _gasneti_threadkey_t _gasneti_threadkey_val_##key
    #define GASNETI_THREADKEY_DEFINE(key)                    \
      GASNETI_THREADKEY_DECLARE(key);                        \
      extern void *_gasneti_threadkey_get_##key(void) {      \
        return gasneti_threadkey_get(key);                   \
      }                                                      \
      extern void _gasneti_threadkey_set_##key(void *_val) { \
        gasneti_threadkey_set(key, _val);                    \
      }                                                      \
      __thread _gasneti_threadkey_t _gasneti_threadkey_val_##key = _GASNETI_THREADKEY_INITIALIZER
  #endif
#else /* _GASNETI_THREADKEY_USES_NOOP */
  #define GASNETI_THREADKEY_DECLARE(key) \
    extern _gasneti_threadkey_t _gasneti_threadkey_val_##key
  #define GASNETI_THREADKEY_DEFINE(key) \
    _gasneti_threadkey_t _gasneti_threadkey_val_##key = _GASNETI_THREADKEY_INITIALIZER
#endif

#if _GASNETI_THREADKEY_USES_PTHREAD_GETSPECIFIC
  /* struct prevents accidental direct access, magic provides extra safety checks */
 #if GASNET_DEBUG
  #define _gasneti_threadkey_check(key, requireinit)         \
   ( gasneti_assert((key).magic == _GASNETI_THREADKEY_MAGIC), \
     (requireinit ? gasneti_assert((key).isinit) : ((void)0)))
 #else
  /* Special case needed to suppress -Wunused-value warnings.
   * You would think the DEBUG version would be fine, but it's not
   * regardnless of how many (void) casts one inserts (gcc bug?).
   */
  #define _gasneti_threadkey_check(key, requireinit)         ((void)0)
 #endif
  #define gasneti_threadkey_get_noinit(key) \
    ( _gasneti_threadkey_check((key), 1),   \
      pthread_getspecific((key).value) )
  #define gasneti_threadkey_set_noinit(key, newvalue) do {                \
    _gasneti_threadkey_check((key), 1);                                   \
    gasneti_assert_zeroret(pthread_setspecific((key).value, (newvalue))); \
  } while (0)
  #define gasneti_threadkey_init(key) (_gasneti_threadkey_check((key), 0),         \
                                       _gasneti_threadkey_init(&((key).value),     \
                                                               &((key).initmutex), \
                                                               &((key).isinit)),   \
                                       _gasneti_threadkey_check((key), 1))
  #define gasneti_threadkey_get(key)       \
    ( _gasneti_threadkey_check(key, 0),    \
      ( PREDICT_FALSE((key).isinit == 0) ? \
        gasneti_threadkey_init(key) :      \
        ((void)0) ),                       \
      gasneti_threadkey_get_noinit(key) )

  #define gasneti_threadkey_set(key,newvalue) do { \
      _gasneti_threadkey_check(key, 0);            \
      if_pf((key).isinit == 0)                     \
        gasneti_threadkey_init(key);               \
      gasneti_threadkey_set_noinit(key, newvalue); \
    } while (0)
#else /* _GASNETI_THREADKEY_USES_TLS, _GASNETI_THREADKEY_USES_NOOP */
  /* name shift to _gasneti_threadkey_val_##key prevents accidental direct access */
  #define gasneti_threadkey_init(key) ((void)0)
  #if _GASNETI_THREADKEY_USES_TLS && GASNETI_CONFIGURE_MISMATCH
    /* defined as __thread data storage, but current compiler doesn't support TLS 
       use an extern function call as conservative fall-back position
     */
    #define gasneti_threadkey_get_noinit(key) \
          (_gasneti_threadkey_get_##key())
    #define gasneti_threadkey_set_noinit(key, newvalue) \
          (_gasneti_threadkey_set_##key(newvalue))
  #else
    #define gasneti_threadkey_get_noinit(key) \
          (_gasneti_threadkey_val_##key)
    #define gasneti_threadkey_set_noinit(key, newvalue) \
         ((_gasneti_threadkey_val_##key) = (newvalue))
  #endif
  #define gasneti_threadkey_get gasneti_threadkey_get_noinit
  #define gasneti_threadkey_set gasneti_threadkey_set_noinit
#endif

/* ------------------------------------------------------------------------------------ */
/* environment support 
   see README-tools for usage information 
 */

extern char *gasneti_format_number(int64_t val, char *buf, size_t bufsz, int is_mem_size);
extern int64_t gasneti_parse_int(const char *str, uint64_t mem_size_multiplier);
extern void gasneti_setenv(const char *key, const char *value);
extern void gasneti_unsetenv(const char *key);

extern char *gasneti_getenv(const char *keyname);
extern char *gasneti_getenv_withdefault(const char *keyname, const char *defaultval);
extern int gasneti_getenv_yesno_withdefault(const char *keyname, int defaultval);
extern int64_t gasneti_getenv_int_withdefault(const char *keyname, int64_t defaultval, uint64_t mem_size_multiplier);
extern double gasneti_getenv_dbl_withdefault(const char *keyname, double defaultval);
extern int gasneti_verboseenv(void);
extern void gasneti_envint_display(const char *key, int64_t val, int is_dflt, int is_mem_size);
extern void gasneti_envstr_display(const char *key, const char *val, int is_dflt);
extern void gasneti_envdbl_display(const char *key, double val, int is_dflt);

/* Conduit-specific supplement to gasneti_getenv
 * If set to non-NULL this has precedence over gasneti_globalEnv.
 */
typedef char *(gasneti_getenv_fn_t)(const char *keyname);
extern gasneti_getenv_fn_t *gasneti_conduit_getenv;


/* ------------------------------------------------------------------------------------ */
/* Attempt to maximize allowable cpu and memory resource limits for this
 * process, silently ignoring any errors
 * return non-zero on success */
int gasnett_maximize_rlimits(void);
/* maximize a particular rlimit, and return non-zero on success.
   For portability, this should be called within an ifdef to ensure 
   the specified RLIMIT_ constant exists
 */
int gasnett_maximize_rlimit(int res, const char *lim_desc);

/* ------------------------------------------------------------------------------------ */

  /* Older AIX's stdio.h won't provide prototypes for snprintf() and vsnprintf()
   * by default since they are in C99 but not C89.
   */
#if !HAVE_SNPRINTF_DECL
  GASNETI_FORMAT_PRINTF(snprintf,3,4,
  extern int snprintf(char * s, size_t n, const char * format, ...));
#endif
#if !HAVE_VSNPRINTF_DECL
  #include <stdarg.h>
  GASNETI_FORMAT_PRINTF(vsnprintf,3,0,
  extern int vsnprintf(char * s, size_t n, const char * format, va_list ap));
#endif

/* ------------------------------------------------------------------------------------ */

/* By default GASNet(tools) enforces a spec-compliant ctype interface,
   even when the OS version is buggy / warning-prone.
   Clients who want the buggy OS version can -DGASNETT_USE_CTYPE_WRAPPERS=0
 */
#ifndef GASNETT_USE_CTYPE_WRAPPERS
   #if GASNETI_NEED_CTYPE_WRAPPERS
      #define GASNETT_USE_CTYPE_WRAPPERS 1
   #else
      #define GASNETT_USE_CTYPE_WRAPPERS 0
   #endif
#endif

#include <ctype.h>
#if GASNETT_USE_CTYPE_WRAPPERS 
  GASNETI_ALWAYS_INLINE(gasnett_toupper) GASNETI_CONST
  int gasnett_toupper(int _c) { return toupper(_c); }
  #undef toupper
  #define toupper gasnett_toupper

  GASNETI_ALWAYS_INLINE(gasnett_tolower) GASNETI_CONST
  int gasnett_tolower(int _c) { return tolower(_c); }
  #undef tolower
  #define tolower gasnett_tolower

  GASNETI_ALWAYS_INLINE(gasnett_isalnum) GASNETI_CONST
  int gasnett_isalnum(int _c) { return isalnum(_c); }
  #undef isalnum
  #define isalnum gasnett_isalnum

  GASNETI_ALWAYS_INLINE(gasnett_isalpha) GASNETI_CONST
  int gasnett_isalpha(int _c) { return isalpha(_c); }
  #undef isalpha
  #define isalpha gasnett_isalpha

  GASNETI_ALWAYS_INLINE(gasnett_iscntrl) GASNETI_CONST
  int gasnett_iscntrl(int _c) { return iscntrl(_c); }
  #undef iscntrl
  #define iscntrl gasnett_iscntrl

  GASNETI_ALWAYS_INLINE(gasnett_isdigit) GASNETI_CONST
  int gasnett_isdigit(int _c) { return isdigit(_c); }
  #undef isdigit
  #define isdigit gasnett_isdigit

  GASNETI_ALWAYS_INLINE(gasnett_isgraph) GASNETI_CONST
  int gasnett_isgraph(int _c) { return isgraph(_c); }
  #undef isgraph
  #define isgraph gasnett_isgraph

  GASNETI_ALWAYS_INLINE(gasnett_islower) GASNETI_CONST
  int gasnett_islower(int _c) { return islower(_c); }
  #undef islower
  #define islower gasnett_islower

  GASNETI_ALWAYS_INLINE(gasnett_isprint) GASNETI_CONST
  int gasnett_isprint(int _c) { return isprint(_c); }
  #undef isprint
  #define isprint gasnett_isprint

  GASNETI_ALWAYS_INLINE(gasnett_ispunct) GASNETI_CONST
  int gasnett_ispunct(int _c) { return ispunct(_c); }
  #undef ispunct
  #define ispunct gasnett_ispunct

  GASNETI_ALWAYS_INLINE(gasnett_isspace) GASNETI_CONST
  int gasnett_isspace(int _c) { return isspace(_c); }
  #undef isspace
  #define isspace gasnett_isspace

  GASNETI_ALWAYS_INLINE(gasnett_isupper) GASNETI_CONST
  int gasnett_isupper(int _c) { return isupper(_c); }
  #undef isupper
  #define isupper gasnett_isupper

  GASNETI_ALWAYS_INLINE(gasnett_isxdigit) GASNETI_CONST
  int gasnett_isxdigit(int _c) { return isxdigit(_c); }
  #undef isxdigit
  #define isxdigit gasnett_isxdigit

 #if HAVE_ISBLANK /* Added in ISO C99 */
  #if !(HAVE_ISBLANK_DECL || defined(isblank))
   extern int isblank(int);
  #endif
  GASNETI_ALWAYS_INLINE(gasnett_isblank) GASNETI_CONST
  int gasnett_isblank(int _c) { return isblank(_c); }
  #undef isblank
  #define isblank gasnett_isblank
 #endif

 #if HAVE_ISASCII /* X/OPEN */
  #if !(HAVE_ISASCII_DECL || defined(isascii))
   extern int isascii(int);
  #endif
  GASNETI_ALWAYS_INLINE(gasnett_isascii) GASNETI_CONST
  int gasnett_isascii(int _c) { return isascii(_c); }
  #undef isascii
  #define isascii gasnett_isascii
 #endif

 #if HAVE_TOASCII /* X/OPEN */
  #if !(HAVE_TOASCII_DECL || defined(toascii))
   extern int toascii(int);
  #endif
  GASNETI_ALWAYS_INLINE(gasnett_toascii) GASNETI_CONST
  int gasnett_toascii(int _c) { return toascii(_c); }
  #undef toascii
  #define toascii gasnett_toascii
 #endif
#endif

/* If a platform lacks isblank() we supply it, assuming the C/POSIX locale.
 */
#if !HAVE_ISBLANK
  GASNETI_ALWAYS_INLINE(gasnett_isblank) GASNETI_CONST
  int gasnett_isblank(int _c) { return (_c == ' ') || (_c == '\t'); }
  #undef isblank /* Paranoia */
  #define isblank gasnett_isblank
#endif

/* If a platform lacks isascii() we supply it.
 */
#if !HAVE_ISASCII
  GASNETI_ALWAYS_INLINE(gasnett_isascii) GASNETI_CONST
  int gasnett_isascii(int _c) { return !(_c & ~0x7f); }
  #undef isascii /* Paranoia */
  #define isascii gasnett_isascii
#endif

/* If a platform lacks toascii() we supply it.
 */
#if !HAVE_TOASCII
  GASNETI_ALWAYS_INLINE(gasnett_toascii) GASNETI_CONST
  int gasnett_toascii(int _c) { return (_c & 0x7f); }
  #undef toascii /* Paranoia */
  #define toascii gasnett_toascii
#endif

/* ------------------------------------------------------------------------------------ */

GASNETI_END_EXTERNC

#endif
