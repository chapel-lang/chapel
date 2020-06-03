/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_toolhelp.h $
 * Description: misc declarations needed by both gasnet_tools and libgasnet
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if !defined(_IN_GASNETEX_H) && !defined(_IN_GASNET_TOOLS_H)
  #error This file is not meant to be included directly- clients should include gasnetex.h or gasnet_tools.h
#endif

#ifndef _GASNET_TOOLHELP_H
#define _GASNET_TOOLHELP_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
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

#if defined(HAVE_SCHED_YIELD)
   #include <sched.h>
   #define _gasneti_sched_yield() sched_yield()
#else
   #define _gasneti_sched_yield() (sleep(0),0)
#endif
#define gasneti_sched_yield() gasneti_assert_zeroret(_gasneti_sched_yield())

extern void gasneti_filesystem_sync(void);

#if PLATFORM_COMPILER_GNU_CXX /* bug 1681 */
  #define GASNETI_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif (defined(HAVE_FUNC) && GASNETI_COMPILER_IS_CC) || __STDC_VERSION__ >= 199901 || __cplusplus >= 201103L
  /* __func__ should also work for ISO C99 or C++11 compilers */
  #define GASNETI_CURRENT_FUNCTION __func__
#elif PLATFORM_COMPILER_GNU /* fallback on gcc, last resort because it generates warnings w/-pedantic */
  #define GASNETI_CURRENT_FUNCTION __FUNCTION__
#else
  #define GASNETI_CURRENT_FUNCTION ""
#endif

GASNETI_COLD
extern char *gasneti_build_loc_str(const char *_funcname, const char *_filename, int _linenum);
// NOTE: this returns a malloced buffer!
#define gasneti_current_loc gasneti_build_loc_str(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__)

GASNETI_COLD
GASNETI_FORMAT_PRINTF(_gasneti_assert_fail,4,5,
extern void _gasneti_assert_fail(const char *_funcname, const char *_filename, int _linenum,
                                 const char *_fmt, ...) GASNETI_NORETURN);
GASNETI_NORETURNP(_gasneti_assert_fail)

/* gasneti_assert_always(expr):
 *   an assertion that never compiles away - for sanity checks in non-critical paths 
 * gasneti_assert_reason_always(expr, reason): 
 *   same, but with a string reason to explain the failure (defaults to preprocessed expression)
 */
#define gasneti_assert_reason_always(expr, reason) \
    (GASNETT_PREDICT_TRUE(expr) ? (void)0 : \
     _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,"%s",reason))

#define gasneti_assert_always(expr) gasneti_assert_reason_always(expr,#expr)

/* gasneti_assert(expr):
 *   an assertion that compiles away in non-debug mode - for sanity checks in critical paths 
 * gasneti_assert_reason(expr, reason):
 *   same, but with a string reason to explain the failure (defaults to preprocessed expression)
 */
#if GASNET_NDEBUG
  #define gasneti_assert(expr)               ((void)0)
  #define gasneti_assert_reason(expr,reason) ((void)0)
#else
  #define gasneti_assert(expr)               gasneti_assert_always(expr)
  #define gasneti_assert_reason(expr,reason) gasneti_assert_reason_always(expr,reason)
#endif

// gasneti_assert(_always)_{(u)int,ptr,dbl}(op1, operator, op2);
// A statement that is mostly equivalent to gasneti_assert(_always)(op1 operator op2);
// but assumes op1/op2 have (unsigned)integer/pointer/floating type and outputs operand values on failure.
// E.g.: gasneti_assert_int(idx, >=, 4);
// C++ NOTE: gasneti_assert_ptr() operates on raw pointer values, and does not perform 
//   C++'s normal implicit pointer conversions for comparisons on pointers to polymorphic object types.
#define gasneti_assert_always_int(op1, operator, op2) do {          \
    int64_t const _gaa_op1 = (op1);                                 \
    int64_t const _gaa_op2 = (op2);                                 \
    if (!GASNETT_PREDICT_TRUE(_gaa_op1 operator _gaa_op2)) {        \
      int const _gaa_bigval = ((int32_t)_gaa_op1 != _gaa_op1) ||    \
                              ((int32_t)_gaa_op2 != _gaa_op2);      \
      int const _gaa_negval = (_gaa_op1 < 0) || (_gaa_op2 < 0);     \
      int const _gaa_decwid = (_gaa_bigval ? 20 : 11);              \
      int const _gaa_hexwid = (_gaa_bigval || _gaa_negval ? 16 : 8);\
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
         "%s %s %s\n"                                               \
         "   op1 : %*" PRId64 " (0x%0*" PRIx64 ") == %s\n"          \
         "   op2 : %*" PRId64 " (0x%0*" PRIx64 ") == %s\n"          \
       , #op1, #operator, #op2                                      \
       , _gaa_decwid, _gaa_op1, _gaa_hexwid, _gaa_op1, #op1         \
       , _gaa_decwid, _gaa_op2, _gaa_hexwid, _gaa_op2, #op2         \
      );                                                            \
    }                                                               \
} while (0)
#define gasneti_assert_always_uint(op1, operator, op2) do {         \
    uint64_t const _gaa_op1 = (op1);                                \
    uint64_t const _gaa_op2 = (op2);                                \
    if (!GASNETT_PREDICT_TRUE(_gaa_op1 operator _gaa_op2)) {        \
      int const _gaa_bigval = ((uint32_t)_gaa_op1 != _gaa_op1) ||   \
                              ((uint32_t)_gaa_op2 != _gaa_op2);     \
      int const _gaa_decwid = (_gaa_bigval ? 20 : 11);              \
      int const _gaa_hexwid = (_gaa_bigval ? 16 : 8);               \
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
         "%s %s %s\n"                                               \
         "   op1 : %*" PRIu64 " (0x%0*" PRIx64 ") == %s\n"          \
         "   op2 : %*" PRIu64 " (0x%0*" PRIx64 ") == %s\n"          \
       , #op1, #operator, #op2                                      \
       , _gaa_decwid, _gaa_op1, _gaa_hexwid, _gaa_op1, #op1         \
       , _gaa_decwid, _gaa_op2, _gaa_hexwid, _gaa_op2, #op2         \
      );                                                            \
    }                                                               \
} while (0)
#ifdef __cplusplus
  #define _gasneti_voidp_cvt(p) (reinterpret_cast<const void *>(p))
#else
  #define _gasneti_voidp_cvt(p) (p) // rely on default conversion
#endif
#define gasneti_assert_always_ptr(op1, operator, op2) do {          \
    const void * const _gaa_op1 = _gasneti_voidp_cvt(op1);          \
    const void * const _gaa_op2 = _gasneti_voidp_cvt(op2);          \
    if (!GASNETT_PREDICT_TRUE(_gaa_op1 operator _gaa_op2)) {        \
      int const _gaa_hexwid = 2*sizeof(void *);                     \
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
         "%s %s %s\n"                                               \
         "   op1 : 0x%0*" PRIxPTR " == %s\n"                        \
         "   op2 : 0x%0*" PRIxPTR " == %s\n"                        \
       , #op1, #operator, #op2                                      \
       , _gaa_hexwid, (uintptr_t)_gaa_op1, #op1                     \
       , _gaa_hexwid, (uintptr_t)_gaa_op2, #op2                     \
      );                                                            \
    }                                                               \
} while (0)
#define gasneti_assert_always_dbl(op1, operator, op2) do {          \
    double const _gaa_op1 = (op1);                                  \
    double const _gaa_op2 = (op2);                                  \
    if (!GASNETT_PREDICT_TRUE(_gaa_op1 operator _gaa_op2)) {        \
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
         "%s %s %s\n"                                               \
         "   op1 : %#13.6g (0x%016" PRIx64 ") == %s\n"              \
         "   op2 : %#13.6g (0x%016" PRIx64 ") == %s\n"              \
       , #op1, #operator, #op2                                      \
       , _gaa_op1, *(uint64_t*)&_gaa_op1, #op1                      \
       , _gaa_op2, *(uint64_t*)&_gaa_op2, #op2                      \
      );                                                            \
    }                                                               \
} while (0)

#if GASNET_NDEBUG
  #define gasneti_assert_int(op1, operator, op2)  do{}while(0)
  #define gasneti_assert_uint(op1, operator, op2) do{}while(0)
  #define gasneti_assert_ptr(op1, operator, op2)  do{}while(0)
  #define gasneti_assert_dbl(op1, operator, op2)  do{}while(0)
#else
  #define gasneti_assert_int  gasneti_assert_always_int
  #define gasneti_assert_uint gasneti_assert_always_uint
  #define gasneti_assert_ptr  gasneti_assert_always_ptr
  #define gasneti_assert_dbl  gasneti_assert_always_dbl
#endif

// gasneti_static_assert(cond)
// statically assert `cond`, which must be a compile-time integer constant expression
// Invocation is an expression must appear in expression or statement context.
// The assertion is active for both DEBUG/NDEBUG and upon success compiles away to nothing.
// Upon failure, causes a compile-time error at the invocation line.
#if __cplusplus >= 201103L // C++11 gives us mostly what we want
  // wrapper makes it usable in expression context
  #define gasneti_static_assert(cond) \
      ((void)([](){ static_assert(cond, "gasneti_static_assert(" #cond ")"); }))
#elif __cplusplus // C++98 lacks static assert and forbids type declarations in casts
  // use a negative array size 
  #define gasneti_static_assert(cond) \
    ((void)(void (*)(int _gasneti_static_assert[(cond)?1:-1]))0)
#else // C
  // use a bit field width, which enforces both integer constant expression and rejects negatives
  #define gasneti_static_assert(cond) \
    ((void)(struct{int _gasneti_static_assert:((cond)?8:-1);}*)0)
#endif

/* gasneti_unreachable(): annotation to mark the current code location as unreachable, to assist optimization 
 * deliberately compiles away in NDEBUG to hopefully avoid inserting dead instructions
 */
#if GASNETT_USE_BUILTIN_UNREACHABLE
  #define gasneti_builtin_unreachable() __builtin_unreachable()
#else
  #define gasneti_builtin_unreachable() ((void)0)
#endif
#define gasneti_unreachable() (gasneti_builtin_unreachable(),gasneti_assert(!"gasneti_unreachable"))

// gasneti_unreachable_error((fmt, args...)) 
// a version of gasneti_unreachable that in DEBUG mode (only) executes gasneti_fatalerror(fmt, ...args)
// otherwise expands to gasneti_unreachable() (and args are not evaluated)
#if GASNET_DEBUG
  #define gasneti_unreachable_error(args) gasneti_fatalerror args
#else
  #define gasneti_unreachable_error(args) gasneti_unreachable()
#endif

/* gasneti_assume(cond): assert a simple condition is always true, as a directive to help compiler analysis
 * Becomes an assertion in DEBUG mode and an analysis directive (when available) in NDEBUG mode.
 * This notably differs from gasneti_assert() in that the expression must remain valid in NDEBUG mode
 * (because it is not preprocessed away), and furthermore may or may not be evaluated at runtime.
 * To ensure portability and performance, cond should NOT contain any function calls or side-effects.
 */
#if GASNET_DEBUG
  #define gasneti_assume(cond) gasneti_assert_always(cond)
#elif GASNETT_USE_BUILTIN_ASSUME
  #define gasneti_assume(cond) ((void)__builtin_assume(cond))
#elif GASNETT_USE_ASSUME
  #define gasneti_assume(cond) ((void)__assume(cond))
#else
  #define gasneti_assume(cond) (GASNETT_PREDICT_TRUE(cond) ? (void)0 : gasneti_unreachable())
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
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
        "%s yielded %s(%i), errno=%s(%i)",                  \
        #op, _tmp, _retval, strerror(errno), errno);        \
    }                                                       \
  } while (0)
  #define gasneti_assert_nzeroret(op) do {                  \
    int _retval = (op);                                     \
    if_pf(!_retval) {                                       \
      /* can't use strdup() or gasneti_strdup() here. */    \
      char _tmp[128];                                       \
      strncpy(_tmp, strerror(_retval), sizeof(_tmp));       \
      _tmp[sizeof(_tmp)-1] = '\0';                          \
      _gasneti_assert_fail(GASNETI_CURRENT_FUNCTION,__FILE__,__LINE__,\
        "%s yielded %s(%i), errno=%s(%i)",                  \
        #op, _tmp, _retval, strerror(errno), errno);        \
    }                                                       \
  } while (0)
#else
  #define gasneti_assert_zeroret(op)  do { op; } while(0)
  #define gasneti_assert_nzeroret(op) do { op; } while(0)
#endif

/* return physical memory of machine
   on failure, failureIsFatal nonzero => fatal error, failureIsFatal zero => return 0 */
extern uint64_t gasneti_getPhysMemSz(int _failureIsFatal); 

extern const char *gasneti_procid_str; // process identifier for error messages

GASNETI_COLD
GASNETI_FORMAT_PRINTF(gasneti_console_message,2,3, // output a formatted message with a prefix type
extern void gasneti_console_message(const char *_prefix, const char *_msg, ...));
GASNETI_COLD
GASNETI_FORMAT_PRINTF(gasneti_console_messageVA,2,0,
extern void gasneti_console_messageVA(const char *_prefix, const char *_msg, va_list _argptr));

GASNETI_COLD
GASNETI_FORMAT_PRINTF(gasneti_fatalerror,1,2,
extern void gasneti_fatalerror(const char *_msg, ...) GASNETI_NORETURN);
GASNETI_NORETURNP(gasneti_fatalerror)

GASNETI_COLD
extern void gasneti_error_abort(void) GASNETI_NORETURN; // perform pre-abort actions then abort
GASNETI_NORETURNP(gasneti_error_abort)

GASNETI_COLD
extern void gasneti_killmyprocess(int _exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasneti_killmyprocess)

extern void gasneti_freezeForDebuggerErr(void); /* freeze iff user enabled error freezing */
GASNETI_COLD
extern void gasneti_freezeForDebuggerNow(volatile int *_flag, const char *_flagsymname);
extern volatile int gasnet_frozen; /* export to simplify debugger restart */ 
extern void gasneti_qualify_path(char *_path_out, const char *_path_in);
extern void gasneti_backtrace_init(const char *_exename);
extern int (*gasneti_print_backtrace_ifenabled)(int _fd);
extern int gasneti_print_backtrace(int _fd);
extern void gasneti_ondemand_init(void);

extern int gasneti_check_node_list(const char *_listvar);

GASNETI_COLD
extern void gasneti_flush_streams(void); /* flush all open streams */
GASNETI_COLD
extern void gasneti_close_streams(void); /* close standard streams (for shutdown) */

extern int gasneti_cpu_count(void);

extern int gasneti_set_affinity(int _rank);
#if HAVE_PLPA || PLATFORM_OS_SOLARIS
  #define GASNETT_SET_AFFINITY_SUPPORT 1
#endif

const char *gasneti_gethostname(void); /* returns the current host name - dies with an error on failure */

extern int gasneti_isLittleEndian(void);

typedef void (*gasneti_sighandlerfn_t)(int);
gasneti_sighandlerfn_t gasneti_reghandler(int _sigtocatch, gasneti_sighandlerfn_t _fp);
void gasneti_registerSignalHandlers(gasneti_sighandlerfn_t _handler);
const char *gasnett_signame_fromval(int _sigval);

// register a function to be called at libc exit time
// passing NULL disables the exit handler
extern void gasneti_registerExitHandler(void (*_exitfn)(int));

GASNETI_COLD
extern int gasneti_raise(int sig); // portability wrapper around POSIX raise(3)

extern int gasneti_blocksig(int _sig);
extern int gasneti_unblocksig(int _sig);

/* return a fast but simple/insecure 64-bit checksum of arbitrary data */
extern uint64_t gasneti_checksum(const void *_p, int _numbytes);

extern int gasneti_nsleep(uint64_t _ns_delay);

/* ------------------------------------------------------------------------------------ */
/* Count zero bytes in a region w/ or w/o a memcpy(), or in a "register" */

extern size_t gasneti_count0s_copy(void * GASNETI_RESTRICT _dst,
                                   const void * GASNETI_RESTRICT _src,
                                   size_t _len);
extern size_t gasneti_count0s(const void * _src, size_t _len);


GASNETI_INLINE(gasneti_count0s_uint32_t) GASNETI_CONST
int gasneti_count0s_uint32_t(uint32_t _x) {
#if 0
  _x |= (_x >> 4); _x |= (_x >> 2); _x |= (_x >> 1);
  _x &= 0x01010101UL;
  _x += (_x >> 16); _x += (_x >> 8);
  return sizeof(_x) - (_x & 0xf);
#else
  const uint32_t _mask = 0x7f7f7f7fUL;
  uint32_t _tmp;
  _tmp = _x & _mask;
  _tmp += _mask;
  _tmp |= _x;
  _tmp &= ~_mask;
  _tmp >>= 7;
  _tmp += (_tmp >> 16);
  _tmp += (_tmp >> 8);
  return sizeof(_x) - (_tmp & 0xf);
#endif
}

#if PLATFORM_ARCH_32
  GASNETI_INLINE(gasneti_count0s_uint64_t) GASNETI_CONST
  int gasneti_count0s_uint64_t(uint64_t _x) {
    return gasneti_count0s_uint32_t(GASNETI_LOWORD(_x)) + 
           gasneti_count0s_uint32_t(GASNETI_HIWORD(_x));
  }
  #define gasneti_count0s_uintptr_t(x) gasneti_count0s_uint32_t(x)
#elif PLATFORM_ARCH_64
  GASNETI_INLINE(gasneti_count0s_uint64_t) GASNETI_CONST
  int gasneti_count0s_uint64_t(uint64_t _x) {
  #if 0
    _x |= (_x >> 4); _x |= (_x >> 2); _x |= (_x >> 1);
    _x &= 0x0101010101010101ULL;
    _x += (_x >> 32); _x += (_x >> 16); _x += (_x >> 8);
    return sizeof(_x) - (_x & 0xf);
  #else
    const uint64_t _mask = 0x7f7f7f7f7f7f7f7fULL;
    uint64_t _tmp;
    _tmp = _x & _mask;
    _tmp += _mask;
    _tmp |= _x;
    _tmp &= ~_mask;
    _tmp >>= 7;
    _tmp += (_tmp >> 32);
    _tmp += (_tmp >> 16);
    _tmp += (_tmp >> 8);
    return sizeof(_x) - (_tmp & 0xf);
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
  #include <cygwin/version.h>
  /* bug1847: Until Cygwin 1.7-3, mutexes initialized using PTHREAD_MUTEX_INITIALIZER were unsafe upon first acquire */
  #if CYGWIN_VERSION_DLL_MAJOR < 1007 || (CYGWIN_VERSION_DLL_MAJOR == 1007 && CYGWIN_VERSION_DLL_MINOR < 3)
    #define GASNETI_MUTEX_CAUTIOUS_INIT 1
  #endif
#endif

#if GASNETI_MUTEX_CAUTIOUS_INIT
  /* implemented using fixed-width atomics to avoid a header dependency cycle */
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_FIELD       volatile int32_t _initstep;
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER , 0
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl)   gasneti_mutex_cautious_init(pl) 
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(pl)  ( (pl)->_initstep = 0, \
                                                   gasneti_mutex_cautious_init(pl) )
  extern void gasneti_mutex_cautious_init(/*gasneti_mutex_t*/void *pl);
#else
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_FIELD
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(pl)      ((void)0)
  #define _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(pl)       ((void)0)
#endif

#if GASNET_DEBUG || GASNETI_BUG2231_WORKAROUND || GASNETI_MUTEX_CAUTIOUS_INIT
  /* Here we deliberately avoid assuming pthread_t is an arithmetic type, 
   * because the POSIX standard allows it to be a struct.
   * We assume only that the bit pattern below (deliberately chosen to be an
   * unlikely value for an index or aligned pointer) will not correspond to the
   * pthread_t identifier of any valid thread.
   * If this assumption is ever wrong, then the most expedient solution is to
   * redefine this constant appropriately, otherwise disable the debug checking entirely.
   */
  #ifndef GASNETI_OWNERID_NONE
  #define _GASNETI_OWNERID_WORD (0x50055005u)
  #define GASNETI_OWNERID_NONE  GASNETI_MAKEWORD(_GASNETI_OWNERID_WORD,_GASNETI_OWNERID_WORD)
  #endif
  #define _GASNETI_MUTEXOWNER_INIT           { GASNETI_OWNERID_NONE }
  // boolean ownership queries, only available in debug mode:
  // _gasneti_mutex_heldbysomeone(pl) and _gasneti_mutex_heldbyme(pl)
  #define _gasneti_mutex_heldbysomeone(pl) ((pl)->_owner._id64 != GASNETI_OWNERID_NONE)
  #if GASNETI_USE_TRUE_MUTEXES
    typedef union {
      volatile uint64_t  _id64;
      volatile pthread_t _id;
    } _gasneti_mutexowner_t;
    #define _gasneti_mutex_heldbyme(pl)      (_gasneti_mutex_heldbysomeone(pl) && \
                                             pthread_equal(pthread_self(), (pl)->_owner._id))
  #else
    typedef struct {
      volatile uint64_t  _id64;
    } _gasneti_mutexowner_t;
    #define _gasneti_ownerid_me_word (0x11111111u)
    #define _gasneti_ownerid64_me    GASNETI_MAKEWORD(_gasneti_ownerid_me_word,_gasneti_ownerid_me_word)
    #define _gasneti_mutex_heldbyme(pl)      ((pl)->_owner._id64 == _gasneti_ownerid64_me)
  #endif
  #if GASNETI_USE_TRUE_MUTEXES
    #include <pthread.h>
    typedef struct {
      _gasneti_mutexowner_t _owner;
      pthread_mutex_t _lock;
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
    #define GASNETI_MUTEX_INITIALIZER { _GASNETI_MUTEXOWNER_INIT,                \
                                        _GASNETI_PTHREAD_MUTEX_INITIALIZER       \
                                        _GASNETI_MUTEX_CAUTIOUS_INIT_INITIALIZER \
                                      }
    #define gasneti_mutex_lock(pl) do {                                                      \
              gasneti_mutex_t * const _pl = (pl);                                            \
              _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(_pl);                                       \
              gasneti_assert_reason(!_gasneti_mutex_heldbyme(_pl),                           \
                                    "Invalid recursive mutex acquire in gasneti_mutex_lock");\
              gasneti_assert_zeroret(pthread_mutex_lock(&(_pl->_lock)));   /* LOCK */        \
              gasneti_assert(!_gasneti_mutex_heldbysomeone(_pl)); /* lock sanity */          \
              _pl->_owner._id = pthread_self();                  /* record ownership */      \
              gasneti_assert(_gasneti_mutex_heldbysomeone(_pl)); /* sanity check NONE val */ \
            } while (0)
    GASNETI_INLINE(gasneti_mutex_trylock) GASNETI_WARN_UNUSED_RESULT
    int gasneti_mutex_trylock(gasneti_mutex_t *_pl) {
              int _retval;
              _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(_pl);
              gasneti_assert_reason(!_gasneti_mutex_heldbyme(_pl),
                                    "Invalid recursive mutex acquire in gasneti_mutex_trylock");
              _retval = pthread_mutex_trylock(&(_pl->_lock)); // LOCK
              if (_retval == EBUSY) return EBUSY;
              if (_retval) gasneti_fatalerror("pthread_mutex_trylock()=%s",strerror(_retval));
              gasneti_assert(!_gasneti_mutex_heldbysomeone(_pl)); // lock sanity
              _pl->_owner._id = pthread_self();                   // record ownership
              gasneti_assert(_gasneti_mutex_heldbysomeone(_pl));  // sanity check NONE val
              return 0;
    }
    #define gasneti_mutex_unlock(pl) do {                                       \
              gasneti_mutex_t * const _pl = (pl);                               \
              gasneti_assert_reason(_gasneti_mutex_heldbyme(_pl),               \
                 "Invalid release of an unheld mutex in gasneti_mutex_unlock"); \
              _pl->_owner._id64 = GASNETI_OWNERID_NONE; /* release ownership */ \
              gasneti_assert_zeroret(pthread_mutex_unlock(&(_pl->_lock)));      \
            } while (0)
    #define gasneti_mutex_init(pl) do {                                         \
              gasneti_mutex_t * const _pl = (pl);                               \
              GASNETI_MUTEX_INITCLEAR(&(_pl->_lock));                           \
              gasneti_assert_zeroret(pthread_mutex_init(&(_pl->_lock),NULL));   \
              _pl->_owner._id64 = GASNETI_OWNERID_NONE; /* clear ownership */   \
              _GASNETI_MUTEX_CAUTIOUS_INIT_INIT(_pl);                           \
            } while (0)
    #if PLATFORM_OS_NETBSD
      /* bug 1476: destroying a locked mutex has undefined effects by POSIX, and some
       * systems whine about it. So instead use the following sequence which
       * accomplishes the same effect, but leaks the mutex if it's held by another
       * thread (unlocking another thread's held lock also has undefined effects).
       */
      GASNETI_INLINE(gasneti_mutex_destroy_ignoreerr)
      int gasneti_mutex_destroy_ignoreerr(gasneti_mutex_t *_gmdi_pl) {
        if (_gasneti_mutex_heldbyme(_gmdi_pl) || !gasneti_mutex_trylock(_gmdi_pl)) {
          /* held by us */
          gasneti_mutex_unlock(_gmdi_pl);
          return pthread_mutex_destroy(&(_gmdi_pl->_lock));
        } else {
          /* held by someone else */
          memset(_gmdi_pl,0,sizeof(*_gmdi_pl)); /* clobber */
          return 0;
        }
      }
    #else
      #define gasneti_mutex_destroy_ignoreerr(pl) \
              pthread_mutex_destroy(&((pl)->_lock))
    #endif
    #define gasneti_mutex_destroy(pl) \
              gasneti_assert_zeroret(gasneti_mutex_destroy_ignoreerr(pl))
  #else /* GASNET_DEBUG non-pthread (error-check-only) mutexes */
    typedef struct {
      _gasneti_mutexowner_t _owner;
    } gasneti_mutex_t;
    #define GASNETI_MUTEX_INITIALIZER   { _GASNETI_MUTEXOWNER_INIT }
    #define gasneti_mutex_lock(pl) do {                                     \
              gasneti_mutex_t * const _pl = (pl);                           \
              gasneti_assert_reason(!_gasneti_mutex_heldbyme(_pl),          \
                  "Invalid recursive mutex acquire in gasneti_mutex_lock"); \
              gasneti_assert_reason(!_gasneti_mutex_heldbysomeone(_pl),     \
                  "gasneti_mutex_lock detected mutex corruption");          \
              _pl->_owner._id64 = _gasneti_ownerid64_me;                    \
            } while (0)
    GASNETI_INLINE(gasneti_mutex_trylock) GASNETI_WARN_UNUSED_RESULT
    int gasneti_mutex_trylock(gasneti_mutex_t *_pl) {
              gasneti_static_assert(_gasneti_ownerid64_me != GASNETI_OWNERID_NONE);
              gasneti_assert_reason(!_gasneti_mutex_heldbyme(_pl),
                  "Invalid recursive mutex acquire in gasneti_mutex_trylock");
              gasneti_assert_reason(!_gasneti_mutex_heldbysomeone(_pl),
                  "gasneti_mutex_trylock detected mutex corruption");
              _pl->_owner._id64 = _gasneti_ownerid64_me;
              return 0;
    }
    #define gasneti_mutex_unlock(pl) do {                                       \
              gasneti_mutex_t * const _pl = (pl);                               \
              gasneti_assert_reason(_gasneti_mutex_heldbyme(_pl),               \
                 "Invalid release of an unheld mutex in gasneti_mutex_unlock"); \
              _pl->_owner._id64 = GASNETI_OWNERID_NONE;                         \
            } while (0)
    #define gasneti_mutex_init(pl) do {                             \
              gasneti_mutex_t * const _pl = (pl);                   \
              _pl->_owner._id64 = GASNETI_OWNERID_NONE;             \
            } while (0)
    #define gasneti_mutex_destroy_ignoreerr(pl) 0
    #define gasneti_mutex_destroy(pl) ((void)0)
  #endif
  #define gasneti_mutex_assertlocked(pl) \
          gasneti_assert_reason( _gasneti_mutex_heldbyme(pl), "gasneti_mutex_assertlocked(" #pl ")")
  #define gasneti_mutex_assertunlocked(pl) \
          gasneti_assert_reason(!_gasneti_mutex_heldbyme(pl), "gasneti_mutex_assertunlocked(" #pl ")")
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
    #if PLATFORM_OS_NETBSD
      /* bug 1476: destroying a locked mutex has undefined effects by POSIX, and some
       * systems whine about it. So instead use the following sequence which
       * accomplishes the same effect, but leaks the mutex if it's held by another
       * thread (unlocking another thread's held lock also has undefined effects),
       * or if held by the caller and gasneti_mutex_trylock() is non-recursive.
       */
      GASNETI_INLINE(gasneti_mutex_destroy_ignoreerr)
      int gasneti_mutex_destroy_ignoreerr(gasneti_mutex_t *_pl) {
        if (!gasneti_mutex_trylock(_pl)) {
          /* held by us */
          gasneti_mutex_unlock(_pl);
          return pthread_mutex_destroy(_pl);
        } else {
          /* held by someone, possibly us */
          memset(_pl,0,sizeof(*_pl)); /* clobber */
          return 0;
        }
      }
    #else
      #define gasneti_mutex_destroy_ignoreerr(pl)   pthread_mutex_destroy(pl)
    #endif
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
    pthread_cond_t _cond;
    GASNETI_BUG2231_WORKAROUND_PAD
  } gasneti_cond_t;

  #define GASNETI_COND_INITIALIZER    { PTHREAD_COND_INITIALIZER }
  #define gasneti_cond_init(pc) do {                                  \
      gasneti_cond_t * const _pc = (pc);                              \
      GASNETI_MUTEX_INITCLEAR(&(_pc->_cond));                         \
      gasneti_assert_zeroret(pthread_cond_init(&(_pc->_cond), NULL)); \
  } while (0)
  #define gasneti_cond_destroy(pc)    gasneti_assert_zeroret(pthread_cond_destroy(&((pc)->_cond)))

  #define gasneti_cond_signal(pc) do {                                \
      gasneti_cond_t * const _pc = (pc);                              \
      gasneti_assert_zeroret(pthread_cond_signal(&(_pc->_cond)));     \
    } while (0)
  #define gasneti_cond_broadcast(pc) do {                             \
      gasneti_cond_t * const _pc = (pc);                              \
      gasneti_assert_zeroret(pthread_cond_broadcast(&(_pc->_cond)));  \
    } while (0)

  #if GASNET_DEBUG || GASNETI_BUG2231_WORKAROUND || GASNETI_MUTEX_CAUTIOUS_INIT
    #define gasneti_cond_wait(pc,pl)  do {                                     \
      gasneti_cond_t * const _pc = (pc);                                       \
      gasneti_mutex_t * const _pl = (pl);                                      \
      gasneti_mutex_assertlocked(_pl);                                         \
      _gasneti_mutexowner_t _ownersave = _pl->_owner;                          \
      _pl->_owner._id64 = GASNETI_OWNERID_NONE;                                \
      _GASNETI_MUTEX_CAUTIOUS_INIT_CHECK(_pl);                                 \
      gasneti_assert_zeroret(pthread_cond_wait(&(_pc->_cond), &(_pl->_lock))); \
      gasneti_assert(!_gasneti_mutex_heldbysomeone(_pl));                      \
      _pl->_owner = _ownersave;                                                \
    } while (0)
  #else
    #define gasneti_cond_wait(pc,pl)  do {               \
      gasneti_assert_zeroret(pthread_cond_wait(&((pc)->_cond), (pl))); \
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
/* Reader-writer lock support */

typedef enum {
  _GASNETI_RWLOCK_UNLOCKED=0,
  _GASNETI_RWLOCK_RDLOCKED,
  _GASNETI_RWLOCK_WRLOCKED
} _gasneti_rwlock_state; /* must always be defined for gasnet_tools-par */

#if !GASNETI_USE_TRUE_MUTEXES ||  /* mutexes compile away to nothing */ \
    !GASNETI_HAVE_PTHREAD_RWLOCK || /* OS rwlocks missing, use standard mutex (no read concurrency) */ \
    GASNETI_MUTEX_CAUTIOUS_INIT     /* assume pthread_rwlocks are also broken */
  #define gasneti_rwlock_t              gasneti_mutex_t
  #define gasneti_rwlock_init           gasneti_mutex_init
  #define gasneti_rwlock_destroy        gasneti_mutex_destroy
  #define gasneti_rwlock_rdlock         gasneti_mutex_lock
  #define gasneti_rwlock_wrlock         gasneti_mutex_lock
  #define gasneti_rwlock_tryrdlock      gasneti_mutex_trylock
  #define gasneti_rwlock_trywrlock      gasneti_mutex_trylock
  #define gasneti_rwlock_unlock         gasneti_mutex_unlock    
  #define GASNETI_RWLOCK_INITIALIZER    GASNETI_MUTEX_INITIALIZER
  #define gasneti_rwlock_assertlocked   gasneti_mutex_assertlocked
  #define gasneti_rwlock_assertrdlocked gasneti_mutex_assertlocked
  #define gasneti_rwlock_assertwrlocked gasneti_mutex_assertlocked
  #define gasneti_rwlock_assertunlocked gasneti_mutex_assertunlocked   

#elif GASNET_DEBUG /* debug checking rwlocks */
  #define gasneti_rwlock_t            pthread_rwlock_t
  #define GASNETI_RWLOCK_INITIALIZER  PTHREAD_RWLOCK_INITIALIZER

  extern _gasneti_rwlock_state _gasneti_rwlock_query(gasneti_rwlock_t const *_l);
  extern void _gasneti_rwlock_insert(gasneti_rwlock_t const *_l, _gasneti_rwlock_state _state);
  extern void _gasneti_rwlock_remove(gasneti_rwlock_t const *_l);

  #define gasneti_rwlock_assertlocked(pl)   \
          gasneti_assert(_gasneti_rwlock_query(pl))
  #define gasneti_rwlock_assertrdlocked(pl) \
          gasneti_assert_uint(_gasneti_rwlock_query(pl) ,==, _GASNETI_RWLOCK_RDLOCKED)
  #define gasneti_rwlock_assertwrlocked(pl) \
          gasneti_assert_uint(_gasneti_rwlock_query(pl) ,==, _GASNETI_RWLOCK_WRLOCKED)
  #define gasneti_rwlock_assertunlocked(pl) \
          gasneti_assert_uint(_gasneti_rwlock_query(pl) ,==, _GASNETI_RWLOCK_UNLOCKED) 

  #define gasneti_rwlock_init(pl) \
          gasneti_assert_zeroret(pthread_rwlock_init(pl,NULL))
  #define gasneti_rwlock_destroy(pl) do {                                      \
    gasneti_rwlock_t * const _pl = (pl);                                       \
    gasneti_rwlock_assertunlocked(_pl);                                        \
    gasneti_assert_zeroret(pthread_rwlock_destroy(_pl));                       \
  } while (0)

  #define gasneti_rwlock_rdlock(pl) do {                                       \
    gasneti_rwlock_t * const _pl = (pl);                                       \
    int _ret;                                                                  \
    gasneti_rwlock_assertunlocked(_pl);                                        \
    while ((_ret = pthread_rwlock_rdlock(_pl)) == EAGAIN)                      \
      gasneti_sched_yield(); /* too many readers */                            \
    if (_ret) gasneti_fatalerror("pthread_rwlock_rdlock()=%s",strerror(_ret)); \
    _gasneti_rwlock_insert(_pl, _GASNETI_RWLOCK_RDLOCKED);                     \
  } while (0)

  #define gasneti_rwlock_wrlock(pl) do {                                       \
    gasneti_rwlock_t * const _pl = (pl);                                       \
    gasneti_rwlock_assertunlocked(_pl);                                        \
    gasneti_assert_zeroret(pthread_rwlock_wrlock(_pl));                        \
    _gasneti_rwlock_insert(_pl, _GASNETI_RWLOCK_WRLOCKED);                     \
  } while (0)

  #define gasneti_rwlock_unlock(pl) do {                                       \
    gasneti_rwlock_t * const _pl = (pl);                                       \
    gasneti_rwlock_assertlocked(_pl);                                          \
    gasneti_assert_zeroret(pthread_rwlock_unlock(_pl));                        \
    _gasneti_rwlock_remove(_pl);                                               \
  } while (0)

  GASNETI_INLINE(_gasneti_rwlock_trylock)
  int _gasneti_rwlock_trylock(gasneti_rwlock_t *_pl, int _writer) {
    int _ret;
    gasneti_rwlock_assertunlocked(_pl);
    if (_writer) _ret = pthread_rwlock_trywrlock(_pl);
    else         _ret = pthread_rwlock_tryrdlock(_pl);
    if (_ret == EBUSY) return EBUSY;
    if (_ret) gasneti_fatalerror("pthread_rwlock_trylock()=%s",strerror(_ret));
    _gasneti_rwlock_insert(_pl, 
      (_writer ? _GASNETI_RWLOCK_WRLOCKED : _GASNETI_RWLOCK_RDLOCKED));
    return 0;
  }
  #define gasneti_rwlock_tryrdlock(pl)  _gasneti_rwlock_trylock(pl,0)
  #define gasneti_rwlock_trywrlock(pl)  _gasneti_rwlock_trylock(pl,1)

#else /* using real, OS-provided rwlocks */
  #define gasneti_rwlock_t            pthread_rwlock_t
  #define gasneti_rwlock_init(pl)     pthread_rwlock_init(pl,NULL)
  #define gasneti_rwlock_destroy      pthread_rwlock_destroy
  #define gasneti_rwlock_rdlock(pl)   /* mask too-many-readers failure */ \
    while (GASNETT_PREDICT_FALSE(pthread_rwlock_rdlock(pl) == EAGAIN)) gasneti_sched_yield()
  #define gasneti_rwlock_wrlock       pthread_rwlock_wrlock
  #define gasneti_rwlock_tryrdlock    pthread_rwlock_tryrdlock
  #define gasneti_rwlock_trywrlock    pthread_rwlock_trywrlock
  #define gasneti_rwlock_unlock       pthread_rwlock_unlock    
  #define GASNETI_RWLOCK_INITIALIZER  PTHREAD_RWLOCK_INITIALIZER
  #define gasneti_rwlock_assertlocked(pl)   ((void)0)
  #define gasneti_rwlock_assertrdlocked(pl) ((void)0)
  #define gasneti_rwlock_assertwrlocked(pl) ((void)0)
  #define gasneti_rwlock_assertunlocked(pl) ((void)0)
#endif
/* ------------------------------------------------------------------------------------ */
/* Wrappers for thread-local data storage
   See README-tools for usage information.
*/
#define _GASNETI_THREADKEY_MAGIC ((uint64_t)0xFF00ABCDEF573921ULL)

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
      uint64_t _magic;
      #define _GASNETI_THREADKEY_MAGIC_INIT _GASNETI_THREADKEY_MAGIC,
    #else
      #define _GASNETI_THREADKEY_MAGIC_INIT
    #endif
      gasneti_mutex_t _initmutex;
      volatile int _isinit;
      pthread_key_t _value;
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
          gasneti_mutex_t _dummymutex = GASNETI_MUTEX_INITIALIZER;
          gasneti_mutex_lock(&_dummymutex);gasneti_mutex_unlock(&_dummymutex); 
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
  #if !GASNETI_COMPILER_IS_CC
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
  #define _gasneti_threadkey_check(key, requireinit)           \
   ( gasneti_assert((key)._magic == _GASNETI_THREADKEY_MAGIC), \
     (requireinit ? gasneti_assert((key)._isinit) : ((void)0)))
 #else
  /* Special case needed to suppress -Wunused-value warnings.
   * You would think the DEBUG version would be fine, but it's not
   * regardnless of how many (void) casts one inserts (gcc bug?).
   */
  #define _gasneti_threadkey_check(key, requireinit)         ((void)0)
 #endif
  #define gasneti_threadkey_get_noinit(key) \
    ( _gasneti_threadkey_check((key), 1),   \
      pthread_getspecific((key)._value) )
  #define gasneti_threadkey_set_noinit(key, newvalue) do {                \
    _gasneti_threadkey_check((key), 1);                                   \
    gasneti_assert_zeroret(pthread_setspecific((key)._value, (newvalue))); \
  } while (0)
  #define gasneti_threadkey_init(key) (_gasneti_threadkey_check((key), 0),          \
                                       _gasneti_threadkey_init(&((key)._value),     \
                                                               &((key)._initmutex), \
                                                               &((key)._isinit)),   \
                                       _gasneti_threadkey_check((key), 1))
  #define gasneti_threadkey_get(key)       \
    ( _gasneti_threadkey_check(key, 0),    \
      ( GASNETT_PREDICT_FALSE((key)._isinit == 0) ? \
        gasneti_threadkey_init(key) :      \
        ((void)0) ),                       \
      gasneti_threadkey_get_noinit(key) )

  #define gasneti_threadkey_set(key,newvalue) do { \
      _gasneti_threadkey_check(key, 0);            \
      if_pf((key)._isinit == 0)                    \
        gasneti_threadkey_init(key);               \
      gasneti_threadkey_set_noinit(key, newvalue); \
    } while (0)
#else /* _GASNETI_THREADKEY_USES_TLS, _GASNETI_THREADKEY_USES_NOOP */
  /* name shift to _gasneti_threadkey_val_##key prevents accidental direct access */
  #define gasneti_threadkey_init(key) ((void)0)
  #if _GASNETI_THREADKEY_USES_TLS && !GASNETI_COMPILER_IS_CC
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
#if !defined(GASNETI_BUG3430_WORKAROUND) && PLATFORM_OS_CYGWIN
  #define GASNETI_BUG3430_WORKAROUND       1
#endif
#if GASNETI_THREADS && GASNETI_BUG3430_WORKAROUND
  // a workaround for the Cygwin pthread_create vs. sched_yield performance bug
  extern gasneti_mutex_t gasneti_bug3430_lock;
  extern gasneti_cond_t gasneti_bug3430_cond;
  extern volatile int gasneti_bug3430_creating;

  static int gasneti_bug3430_sched_yield(void) {
    if (gasneti_bug3430_creating) { // deliberately unsynchronized "optimistic" read
      gasneti_mutex_lock(&gasneti_bug3430_lock);
        while (gasneti_bug3430_creating) { // sleep thru pthread_create
          gasneti_cond_wait(&gasneti_bug3430_cond, &gasneti_bug3430_lock);
        }
      gasneti_mutex_unlock(&gasneti_bug3430_lock);
    }
    return _gasneti_sched_yield();
  }

  static int gasneti_bug3430_pthread_create(pthread_t * GASNETI_RESTRICT _thread,
           const pthread_attr_t * GASNETI_RESTRICT _attr,
           void *(*_start_routine)(void*), void * GASNETI_RESTRICT _arg) {
    gasneti_mutex_lock(&gasneti_bug3430_lock);
      gasneti_bug3430_creating++;
      int _ret = pthread_create(_thread, _attr, _start_routine, _arg);
      gasneti_bug3430_creating--;
      gasneti_cond_broadcast(&gasneti_bug3430_cond);
    gasneti_mutex_unlock(&gasneti_bug3430_lock);
    return _ret;
  }

  // install hooks via macro
  #undef  sched_yield
  #define sched_yield    gasneti_bug3430_sched_yield
  #undef  _gasneti_sched_yield
  #define _gasneti_sched_yield() gasneti_bug3430_sched_yield()
  #undef  pthread_create
  #define pthread_create gasneti_bug3430_pthread_create
#endif

/* ------------------------------------------------------------------------------------ */
/* environment support 
   see README-tools for usage information 
 */

extern char *gasneti_format_number(int64_t _val, char *_buf, size_t _bufsz, int _is_mem_size);
extern int64_t gasneti_parse_int(const char *_str, uint64_t _mem_size_multiplier);
extern int gasneti_parse_dbl(const char *_str, double *_result_ptr);
extern void gasneti_setenv(const char *_key, const char *_value);
extern void gasneti_unsetenv(const char *_key);

extern char *gasneti_getenv(const char *_keyname);
extern char *gasneti_getenv_withdefault(const char *_keyname, const char *_defaultval);
extern int gasneti_getenv_yesno_withdefault(const char *_keyname, int _defaultval);
extern int64_t gasneti_getenv_int_withdefault(const char *_keyname, int64_t _defaultval, uint64_t _mem_size_multiplier);
extern double gasneti_getenv_dbl_withdefault(const char *_keyname, double _defaultval);
extern uint64_t gasneti_getenv_memsize_withdefault(const char *_key, const char *_dflt,
                                                   uint64_t _minimum, uint64_t _maximum,
                                                   uint64_t _fraction_of, uint64_t _pph,
                                                   uint64_t _overhead_per_p);
extern int gasneti_verboseenv(void);
extern void gasneti_envint_display(const char *_key, int64_t _val, int _is_dflt, int _is_mem_size);
extern void gasneti_envstr_display(const char *_key, const char *_val, int _is_dflt);
extern void gasneti_envdbl_display(const char *_key, double _val, int _is_dflt);

extern const char *gasneti_tmpdir(void);

/* Custom (spawner- or conduit-specific) supplement to gasneti_getenv
 * If set to non-NULL this has precedence over gasneti_globalEnv.
 */
typedef char *(gasneti_getenv_fn_t)(const char *_keyname);
extern gasneti_getenv_fn_t *gasneti_getenv_hook;


/* ------------------------------------------------------------------------------------ */
/* Attempt to maximize allowable cpu and memory resource limits for this
 * process, silently ignoring any errors
 * return non-zero on success */
int gasnett_maximize_rlimits(void);
/* maximize a particular rlimit, and return non-zero on success.
   For portability, this should be called within an ifdef to ensure 
   the specified RLIMIT_ constant exists
 */
int gasnett_maximize_rlimit(int _res, const char *_lim_desc);

/* ------------------------------------------------------------------------------------ */

  /* Older AIX's stdio.h won't provide prototypes for snprintf() and vsnprintf()
   * by default since they are in C99 but not C89.
   */
#if !HAVE_SNPRINTF_DECL
  GASNETI_FORMAT_PRINTF(snprintf,3,4,
  extern int snprintf(char *, size_t, const char *, ...));
#endif
#if !HAVE_VSNPRINTF_DECL
  #include <stdarg.h>
  GASNETI_FORMAT_PRINTF(vsnprintf,3,0,
  extern int vsnprintf(char *, size_t, const char *, va_list));
#endif

/* ------------------------------------------------------------------------------------ */

/* By default GASNet(tools) enforces a spec-compliant ctype interface,
   even when the OS version is buggy / warning-prone.
   Clients who want the buggy OS version can -DGASNETT_USE_CTYPE_WRAPPERS=0
 */
#ifndef GASNETT_USE_CTYPE_WRAPPERS
   #if GASNETI_NEED_CTYPE_WRAPPERS && !__cplusplus // bug 3834
      #define GASNETT_USE_CTYPE_WRAPPERS 1
   #else
      #define GASNETT_USE_CTYPE_WRAPPERS 0
   #endif
#endif

#include <ctype.h>
#if GASNETT_USE_CTYPE_WRAPPERS 
  GASNETI_INLINE(gasnett_toupper) GASNETI_CONST
  int gasnett_toupper(int _c) { return toupper(_c); }
  #undef toupper
  #define toupper gasnett_toupper

  GASNETI_INLINE(gasnett_tolower) GASNETI_CONST
  int gasnett_tolower(int _c) { return tolower(_c); }
  #undef tolower
  #define tolower gasnett_tolower

  GASNETI_INLINE(gasnett_isalnum) GASNETI_CONST
  int gasnett_isalnum(int _c) { return isalnum(_c); }
  #undef isalnum
  #define isalnum gasnett_isalnum

  GASNETI_INLINE(gasnett_isalpha) GASNETI_CONST
  int gasnett_isalpha(int _c) { return isalpha(_c); }
  #undef isalpha
  #define isalpha gasnett_isalpha

  GASNETI_INLINE(gasnett_iscntrl) GASNETI_CONST
  int gasnett_iscntrl(int _c) { return iscntrl(_c); }
  #undef iscntrl
  #define iscntrl gasnett_iscntrl

  GASNETI_INLINE(gasnett_isdigit) GASNETI_CONST
  int gasnett_isdigit(int _c) { return isdigit(_c); }
  #undef isdigit
  #define isdigit gasnett_isdigit

  GASNETI_INLINE(gasnett_isgraph) GASNETI_CONST
  int gasnett_isgraph(int _c) { return isgraph(_c); }
  #undef isgraph
  #define isgraph gasnett_isgraph

  GASNETI_INLINE(gasnett_islower) GASNETI_CONST
  int gasnett_islower(int _c) { return islower(_c); }
  #undef islower
  #define islower gasnett_islower

  GASNETI_INLINE(gasnett_isprint) GASNETI_CONST
  int gasnett_isprint(int _c) { return isprint(_c); }
  #undef isprint
  #define isprint gasnett_isprint

  GASNETI_INLINE(gasnett_ispunct) GASNETI_CONST
  int gasnett_ispunct(int _c) { return ispunct(_c); }
  #undef ispunct
  #define ispunct gasnett_ispunct

  GASNETI_INLINE(gasnett_isspace) GASNETI_CONST
  int gasnett_isspace(int _c) { return isspace(_c); }
  #undef isspace
  #define isspace gasnett_isspace

  GASNETI_INLINE(gasnett_isupper) GASNETI_CONST
  int gasnett_isupper(int _c) { return isupper(_c); }
  #undef isupper
  #define isupper gasnett_isupper

  GASNETI_INLINE(gasnett_isxdigit) GASNETI_CONST
  int gasnett_isxdigit(int _c) { return isxdigit(_c); }
  #undef isxdigit
  #define isxdigit gasnett_isxdigit

 #if HAVE_ISBLANK /* Added in ISO C99 */
  #if !(HAVE_ISBLANK_DECL || defined(isblank))
   extern int isblank(int);
  #endif
  GASNETI_INLINE(gasnett_isblank) GASNETI_CONST
  int gasnett_isblank(int _c) { return isblank(_c); }
  #undef isblank
  #define isblank gasnett_isblank
 #endif

 #if HAVE_ISASCII /* X/OPEN */
  #if !(HAVE_ISASCII_DECL || defined(isascii))
   extern int isascii(int);
  #endif
  GASNETI_INLINE(gasnett_isascii) GASNETI_CONST
  int gasnett_isascii(int _c) { return isascii(_c); }
  #undef isascii
  #define isascii gasnett_isascii
 #endif

 #if HAVE_TOASCII /* X/OPEN */
  #if !(HAVE_TOASCII_DECL || defined(toascii))
   extern int toascii(int);
  #endif
  GASNETI_INLINE(gasnett_toascii) GASNETI_CONST
  int gasnett_toascii(int _c) { return toascii(_c); }
  #undef toascii
  #define toascii gasnett_toascii
 #endif
#endif

/* If a platform lacks isblank() we supply it, assuming the C/POSIX locale.
 */
#if !HAVE_ISBLANK
  GASNETI_INLINE(gasnett_isblank) GASNETI_CONST
  int gasnett_isblank(int _c) { return (_c == ' ') || (_c == '\t'); }
  #undef isblank /* Paranoia */
  #define isblank gasnett_isblank
#endif

/* If a platform lacks isascii() we supply it.
 */
#if !HAVE_ISASCII
  GASNETI_INLINE(gasnett_isascii) GASNETI_CONST
  int gasnett_isascii(int _c) { return !(_c & ~0x7f); }
  #undef isascii /* Paranoia */
  #define isascii gasnett_isascii
#endif

/* If a platform lacks toascii() we supply it.
 */
#if !HAVE_TOASCII
  GASNETI_INLINE(gasnett_toascii) GASNETI_CONST
  int gasnett_toascii(int _c) { return (_c & 0x7f); }
  #undef toascii /* Paranoia */
  #define toascii gasnett_toascii
#endif

/* ------------------------------------------------------------------------------------ */

#endif
