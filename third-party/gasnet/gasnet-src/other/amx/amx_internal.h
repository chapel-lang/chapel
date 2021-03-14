/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amx/amx_internal.h $
 * Description: AMX internal header file
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _AMX_INTERNAL_H
#define _AMX_INTERNAL_H
#ifndef _AMX_INTERNAL_FWD_H
#error header inclusion error
#endif

#ifdef __cplusplus
extern "C" {
#endif

// At this point we should have the public header, which includes (N)EBUG indicators and library identification
#if !(AMX_DEBUG ^ AMX_NDEBUG)
#error incorrext AMX_(N)DEBUG settings
#endif

#if AMUDP
  #define AMX_LIB_STR "AMUDP"
#elif AMMPI
  #define AMX_LIB_STR "AMMPI"
#else
  #error missing AMX library identification
#endif

#include <assert.h>
#undef assert
#define assert(x) ERROR_use_AMX_assert
#define AMX_curloc __FILE__ ":" AMX_STRINGIFY(__LINE__)
#if AMX_NDEBUG
  #define AMX_assert(expr) ((void)0)
#else
  #define AMX_assert(expr)                                  \
    (AMX_PREDICT_TRUE(expr) ? (void)0 :                     \
      AMX_FatalErr("Assertion failure at %s %s: %s\n",      \
        AMX_CURRENT_FUNCTION, AMX_curloc, #expr))
#endif

#ifdef gasnett_unreachable
  #define AMX_unreachable() gasnett_unreachable()
#else
  #define AMX_unreachable() AMX_assert(!"unreachable")
#endif

// Attributes
#if defined(__has_attribute) // introduced around gcc 5.x (2015)
  #define AMX_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
  #define AMX_HAS_ATTRIBUTE(x) 0
#endif
#ifdef HAVE_GASNET_TOOLS 
  #define AMX_FORMAT_PRINTF GASNETT_FORMAT_PRINTF
#elif defined (__GNUC__) || AMX_HAS_ATTRIBUTE(__format__)
  #define AMX_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) \
            __attribute__((__format__ (__printf__, fmtarg, firstvararg))) declarator
#else
  #define AMX_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) declarator
#endif
#ifdef HAVE_GASNET_TOOLS 
  #define AMX_INLINE(fn) GASNETT_INLINE(fn)
#elif AMX_DEBUG
  #define AMX_INLINE(fn) static
#elif __cplusplus 
  #define AMX_INLINE(fn) inline
#elif  __STDC_VERSION__ >= 199901L
  #define AMX_INLINE(fn) static inline
#else
  #define AMX_INLINE(fn) static
#endif
#ifdef HAVE_GASNET_TOOLS 
  #define AMX_NEVER_INLINE(fnname,declarator) GASNETT_NEVER_INLINE(fnname,declarator)
#else
  #define AMX_NEVER_INLINE(fnname,declarator) declarator
#endif

/* ------------------------------------------------------------------------------------ */

AMX_FORMAT_PRINTF(AMX_Err,1,2,
extern int AMX_Err(const char *msg, ...));

AMX_FORMAT_PRINTF(AMX_Warn,1,2,
extern int AMX_Warn(const char *msg, ...));

AMX_FORMAT_PRINTF(AMX_Info,1,2,
extern int AMX_Info(const char *msg, ...));

// verbose debug messages (double-parens)
#if AMX_DEBUG_VERBOSE
  #define AMX_VERBOSE_INFO(args)    AMX_Info args
  #define _AMX_IS_DEBUG_VERBOSE 1
#else
  #define AMX_VERBOSE_INFO(args)    ((void)0)
  #define _AMX_IS_DEBUG_VERBOSE 0
#endif
#if AMX_DEBUG
  #define AMX_DEBUG_INFO(args)      AMX_Info args
  #define AMX_DEBUG_WARN(args)      AMX_Warn args
  // throttled debug-only warning
  #define AMX_DEBUG_WARN_TH(msg) do {                         \
    static uint64_t _cnt = 0;                                 \
    static uint64_t _mask = 0xFF;                             \
    _cnt++;                                                   \
    if_pf (_AMX_IS_DEBUG_VERBOSE || (_cnt & _mask) == 0) {    \
      _mask = (_mask << 1) | 0x1;                             \
      AMX_Warn("%s (%" PRIu64 " occurences)",msg,_cnt);       \
    }                                                         \
  } while (0)
#else
  #define AMX_DEBUG_INFO(args)      ((void)0)
  #define AMX_DEBUG_WARN(args)      ((void)0)
  #define AMX_DEBUG_WARN_TH(msg)    ((void)0)
#endif

#ifdef GASNETT_NORETURN
GASNETT_NORETURN
#endif
AMX_FORMAT_PRINTF(AMX_FatalErr,1,2,
extern void AMX_FatalErr(const char *msg, ...));
#ifdef GASNETT_NORETURNP
GASNETT_NORETURNP(AMX_FatalErr)
#endif

/* memory allocation */
static void *_AMX_malloc(size_t sz, const char *curloc) {
  void *ret = malloc(sz);
  if_pf(!ret) AMX_FatalErr("Failed to malloc(%" PRIuSZ ") at %s", sz, curloc);
  return ret;
}
static void *_AMX_calloc(size_t N, size_t S, const char *curloc) {
  void *ret = calloc(N,S);
  if_pf(!ret) AMX_FatalErr("Failed to calloc(%" PRIuSZ ",%" PRIuSZ ") at %s", N, S, curloc);
  return ret;
}
static void *_AMX_realloc(void *ptr, size_t S, const char *curloc) {
  void *ret = realloc(ptr,S);
  if_pf(!ret) AMX_FatalErr("Failed to realloc(%" PRIuSZ ") at %s", S, curloc);
  return ret;
}
static void _AMX_free(void *ptr, const char *curloc) {
  free(ptr);
}
#ifndef AMX_TENTATIVE_EXTERN
  #if __cplusplus
    #define AMX_TENTATIVE_EXTERN extern
  #elif AMX_HAS_ATTRIBUTE(__common__)
    // This attribute is notably available in or before:
    // gcc 4.8.5, clang 4.0.0, Intel 2017.0, PGI 18.4, Xcode 8.2.1
    #define AMX_TENTATIVE_EXTERN __attribute__((__common__))
  #else
    #define AMX_TENTATIVE_EXTERN 
  #endif
#endif
#if AMX_DEBUG
  // use the gasnet debug malloc functions if a debug libgasnet is linked
  // This uses tentative definitions which are not supported in C++, 
  // so at least one object in libamx must compile this header in C mode.
  AMX_TENTATIVE_EXTERN void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc);
  AMX_TENTATIVE_EXTERN void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc);
  AMX_TENTATIVE_EXTERN void *(*gasnett_debug_realloc_fn)(void *ptr, size_t sz, const char *curloc);
  AMX_TENTATIVE_EXTERN void (*gasnett_debug_free_fn)(void *ptr, const char *curloc);
  AMX_TENTATIVE_EXTERN char *(*gasnett_debug_strdup_fn)(const char *s, const char *curloc);
  AMX_TENTATIVE_EXTERN char *(*gasnett_debug_strndup_fn)(const char *s, size_t sz, const char *curloc);
  AMX_TENTATIVE_EXTERN void (*gasnett_debug_memcheck_fn)(void *ptr, const char *curloc);
  AMX_TENTATIVE_EXTERN void (*gasnett_debug_memcheck_one_fn)(const char *curloc);
  AMX_TENTATIVE_EXTERN void (*gasnett_debug_memcheck_all_fn)(const char *curloc);
  #define AMX_malloc(sz)                                   \
    ( (AMX_PREDICT_FALSE(gasnett_debug_malloc_fn==NULL) ?  \
        gasnett_debug_malloc_fn = &_AMX_malloc : 0),       \
      (*gasnett_debug_malloc_fn)(sz, AMX_curloc))
  #define AMX_calloc(N,S)                                  \
    ( (AMX_PREDICT_FALSE(gasnett_debug_calloc_fn==NULL) ?  \
        gasnett_debug_calloc_fn = &_AMX_calloc : 0),       \
      (*gasnett_debug_calloc_fn)((N),(S), AMX_curloc))
  #define AMX_realloc(ptr,S)                               \
    ( (AMX_PREDICT_FALSE(gasnett_debug_realloc_fn==NULL) ? \
        gasnett_debug_realloc_fn = &_AMX_realloc : 0),     \
      (*gasnett_debug_realloc_fn)((ptr),(S), AMX_curloc))
  #define AMX_free(ptr)                                    \
    ( (AMX_PREDICT_FALSE(gasnett_debug_free_fn==NULL) ?    \
        gasnett_debug_free_fn = &_AMX_free : 0),           \
      (*gasnett_debug_free_fn)(ptr, AMX_curloc))
  #define AMX_strdup(s)                                    \
    ( (AMX_PREDICT_FALSE(gasnett_debug_strdup_fn==NULL) ?  \
        gasnett_debug_strdup_fn = &_AMX_strdup : 0),       \
      (*gasnett_debug_strdup_fn)(s, AMX_curloc))
  #define AMX_strndup(s,sz)                                \
    ( (AMX_PREDICT_FALSE(gasnett_debug_strndup_fn==NULL) ? \
        gasnett_debug_strndup_fn = &_AMX_strndup : 0),     \
      (*gasnett_debug_strndup_fn)(s, sz, AMX_curloc))
  #define AMX_memcheck(ptr) do {                          \
    AMX_assert(ptr);                                      \
    if (gasnett_debug_memcheck_fn)                        \
       (*gasnett_debug_memcheck_fn)(ptr, AMX_curloc);     \
  } while (0)
  #define AMX_memcheck_one() do {                         \
    if (gasnett_debug_memcheck_one_fn)                    \
       (*gasnett_debug_memcheck_one_fn)(AMX_curloc);      \
  } while (0)
  #define AMX_memcheck_all() do {                         \
    if (gasnett_debug_memcheck_all_fn)                    \
       (*gasnett_debug_memcheck_all_fn)(AMX_curloc);      \
  } while (0)

  #undef malloc
  #define malloc(x)   ERROR_use_AMX_malloc
  #undef calloc
  #define calloc(n,s) ERROR_use_AMX_calloc
  #undef realloc
  #define realloc(n,s) ERROR_use_AMX_realloc
  #undef free
  #define free(x)     ERROR_use_AMX_free
  #undef strdup
  #define strdup(x)     ERROR_use_AMX_strdup
  #undef strndup
  #define strndup(x)     ERROR_use_AMX_strndup
#else
  #define AMX_malloc(sz)     _AMX_malloc((sz),AMX_curloc)
  #define AMX_calloc(N,S)    _AMX_calloc((N),(S),AMX_curloc)
  #define AMX_realloc(ptr,S) _AMX_realloc((ptr),(S),AMX_curloc)
  #define AMX_free(ptr)      _AMX_free(ptr,AMX_curloc)
  #define AMX_strdup(s)      _AMX_strdup(s,AMX_curloc)
  #define AMX_strndup(s,sz)  _AMX_strndup(s,sz,AMX_curloc)
  #define AMX_memcheck(ptr)   ((void)0)
  #define AMX_memcheck_one()  ((void)0)
  #define AMX_memcheck_all()  ((void)0)
#endif
// some older OS's (eg Solaris 10) lack strndup, so roll our own
static char *_AMX_strdup(const char *s, const char *curloc) {
  AMX_assert(s);
  size_t len = strlen(s);
  char *ret = (char*)AMX_malloc(len+1);
  if_pf(!ret) AMX_FatalErr("Failed to strdup(%" PRIuSZ ") at %s", strlen(s), curloc);
  memcpy(ret,s,len);
  ret[len] = 0;
  return ret;
}
static char *_AMX_strndup(const char *s, size_t sz, const char *curloc) {
  AMX_assert(s);
  size_t _len = strlen(s);
  size_t len = MIN(_len,sz);
  char *ret = (char *)AMX_malloc(len+1);
  if_pf(!ret) AMX_FatalErr("Failed to strndup(%" PRIuSZ ",%" PRIuSZ ") at %s", strlen(s), sz, curloc);
  memcpy(ret,s,len);
  ret[len] = 0;
  return ret;
}

/*------------------------------------------------------------------------------------
 * Error reporting
 *------------------------------------------------------------------------------------ */
static const char *AMX_ErrorName(int errval) {
  switch (errval) {
    case AM_ERR_NOT_INIT: return "AM_ERR_NOT_INIT";      
    case AM_ERR_BAD_ARG:  return "AM_ERR_BAD_ARG";       
    case AM_ERR_RESOURCE: return "AM_ERR_RESOURCE";      
    case AM_ERR_NOT_SENT: return "AM_ERR_NOT_SENT";      
    case AM_ERR_IN_USE:   return "AM_ERR_IN_USE";       
    default: return "*unknown*";
  }
}
static const char *AMX_ErrorDesc(int errval) {
  switch (errval) {
    case AM_ERR_NOT_INIT: return "Active message layer not initialized"; 
    case AM_ERR_BAD_ARG:  return "Invalid function parameter passed";    
    case AM_ERR_RESOURCE: return "Problem with requested resource";      
    case AM_ERR_NOT_SENT: return "Synchronous message not sent";  
    case AM_ERR_IN_USE:   return "Resource currently in use";     
    default: return "no description available";
  }
}
//------------------------------------------------------------------------------------
/* macros for returning errors that allow verbose error tracking */
#define AMX_RETURN_ERR(type) do {                                                  \
    if (AMX_VerboseErrors) AMX_Warn("%s returning an error code: AM_ERR_%s (%s)\n" \
        "  at %s",                                                                 \
        AMX_CURRENT_FUNCTION, #type, AMX_ErrorDesc(AM_ERR_##type), AMX_curloc);    \
    return AM_ERR_ ## type;                                                        \
  } while (0)
#define AMX_RETURN_ERRF(type, fromfn) do {                                         \
    if (AMX_VerboseErrors) AMX_Warn("%s returning an error code: AM_ERR_%s (%s)\n" \
        "  from function %s\n"                                                     \
        "  at %s",                                                                 \
        AMX_CURRENT_FUNCTION, #type, AMX_ErrorDesc(AM_ERR_##type),                 \
        #fromfn, AMX_curloc);                                                      \
    return AM_ERR_ ## type;                                                        \
  } while (0)
#define AMX_RETURN_ERRFR(type, fromfn, reason) do {                                \
    if (AMX_VerboseErrors) AMX_Warn("%s returning an error code: AM_ERR_%s (%s)\n" \
        "  from function %s\n"                                                     \
        "  at %s\n"                                                                \
        "  reason: %s\n",                                                          \
        AMX_CURRENT_FUNCTION, #type, AMX_ErrorDesc(AM_ERR_##type),                 \
        #fromfn, AMX_curloc, reason);                                              \
    return AM_ERR_ ## type;                                                        \
  } while (0)
/* return a possible error */
#define AMX_RETURN(val) do {                                                       \
  if_pf (AMX_VerboseErrors && val != AM_OK)                                        \
    AMX_Warn("%s returning an error code: %s (%s)\n"                               \
        "  at %s",                                                                 \
        AMX_CURRENT_FUNCTION, AMX_ErrorName(val), AMX_ErrorDesc(val),              \
        AMX_curloc);                                                               \
  return val;                                                                      \
} while (0)

#ifndef AMX_ENABLE_ERRCHECKS
  #if GASNETI_ENABLE_ERRCHECKS
    #define AMX_ENABLE_ERRCHECKS 1
  #else
    #define AMX_ENABLE_ERRCHECKS 0
  #endif
#endif

#if AMX_DEBUG || AMX_ENABLE_ERRCHECKS
  #define AMX_CHECK_ERR(errcond, type) \
    do { if_pf (errcond) AMX_RETURN_ERR(type); } while(0)
  #define AMX_CHECK_ERRF(errcond, type, fromfn) \
    do { if_pf (errcond) AMX_RETURN_ERRF(type, fromfn); } while(0)
  #define AMX_CHECK_ERRR(errcond, type, reason) \
    do { if_pf (errcond) AMX_RETURN_ERRR(type, reason); } while(0)
  #define AMX_CHECK_ERRFR(errcond, type, fromfn, reason) \
    do { if_pf (errcond) AMX_RETURN_ERRFR(type, fromfn, reason); } while(0)
  #define AMX_CHECK_ERRFRC(errcond, type, fromfn, reason, cleanup) \
    do { if_pf (errcond) { { cleanup; } AMX_RETURN_ERRFR(type, fromfn, reason); } } while(0)
#else
  #define AMX_CHECK_ERR(errcond, type)                    ((void)0)
  #define AMX_CHECK_ERRF(errcond, type, fromfn)           ((void)0)
  #define AMX_CHECK_ERRR(errcond, type, reason)           ((void)0)
  #define AMX_CHECK_ERRFR(errcond, type, fromfn, reason)  ((void)0)
  #define AMX_CHECK_ERRFRC(errcond, type, fromfn, reason, cleanup) ((void)0)
#endif

#ifdef AMX_HERE
  #undef AMX_HERE
  #define AMX_HERE() AMX_Info("HERE at: %s",AMX_curloc)
#else
  #define AMX_HERE() ((void)0)
#endif

extern int amx_Initialized;
#define AMX_CHECKINIT() AMX_CHECK_ERR((!amx_Initialized),NOT_INIT)
extern int AMX_Init();
extern int AMX_Terminate();

//------------------------------------------------------------------------------------
// shared variables

extern amx_handler_fn_t amx_unused_handler;

extern const char *AMX_ProcessLabel;

//------------------------------------------------------------------------------------
// handler dispatch

typedef void (*amx_returned_handler_fn_t)(int status, op_t opcode, void *token);

#define _AMX_EMPTY(a,b,c)
#define _AMX_harg(a,b,c) ,int32_t
#define _AMX_harg_pass(Nm1,N,Np1) ,_args[Nm1]

#define _AMX_Short_handlerfn_typedefN(Nm1,N,Np1) \
  typedef void (*AMX_Short_handlerfn_type_##N)(void * AMX_META_DES##N(_AMX_EMPTY,_AMX_harg));
AMX_META_ASC16(_AMX_Short_handlerfn_typedefN,_AMX_Short_handlerfn_typedefN)

#define _AMX_Short_RunCaseN(Nm1,N,Np1) \
  case N: ((AMX_Short_handlerfn_type_##N)_phandlerfn)(_token AMX_META_ASC##N(_AMX_EMPTY,_AMX_harg_pass)); break;

#define AMX_RUN_HANDLER_SHORT(phandlerfn, token, pArgs, numargs) do {         \
  amx_handler_fn_t const _phandlerfn = (amx_handler_fn_t)phandlerfn;          \
  void * const _token = (void *)token;                                        \
  int32_t const * const _args = (int32_t *)(pArgs);                           \
  AMX_assert(_phandlerfn); AMX_assert(_token); AMX_assert(_args || !numargs); \
  switch (numargs) {                                                          \
    AMX_META_DES16(_AMX_Short_RunCaseN,_AMX_Short_RunCaseN)                   \
    default: AMX_unreachable();                                               \
  }                                                                           \
} while (0)

#define _AMX_MedLong_handlerfn_typedefN(Nm1,N,Np1) \
  typedef void (*AMX_MedLong_handlerfn_type_##N)(void *, void *, size_t AMX_META_DES##N(_AMX_EMPTY,_AMX_harg));
AMX_META_ASC16(_AMX_MedLong_handlerfn_typedefN,_AMX_MedLong_handlerfn_typedefN)

#define _AMX_MedLong_RunCaseN(Nm1,N,Np1) \
  case N: ((AMX_MedLong_handlerfn_type_##N)_phandlerfn)(_token, _pData, _datalen AMX_META_ASC##N(_AMX_EMPTY,_AMX_harg_pass)); break;

#define _AMX_RUN_HANDLER_MEDLONG(phandlerfn, token, pArgs, numargs, pData, datalen, extrachecks) do { \
  amx_handler_fn_t const _phandlerfn = (amx_handler_fn_t)phandlerfn;          \
  void * const _token = (void *)token;                                        \
  int32_t const * const _args = (int32_t *)(pArgs);                           \
  void * const _pData = (void *)pData;                                        \
  size_t const _datalen = datalen;                                            \
  AMX_assert(_phandlerfn); AMX_assert(_token); AMX_assert(_args || !numargs); \
  AMX_assert(_pData || !_datalen);                                            \
  extrachecks;                                                                \
  switch (numargs) {                                                          \
    AMX_META_DES16(_AMX_MedLong_RunCaseN,_AMX_MedLong_RunCaseN)               \
    default: AMX_unreachable();                                               \
  }                                                                           \
} while (0)

#define AMX_RUN_HANDLER_MEDIUM(phandlerfn, token, pArgs, numargs, pData, datalen) \
      _AMX_RUN_HANDLER_MEDLONG(phandlerfn, token, pArgs, numargs, pData, datalen, \
                               AMX_assert(((uintptr_t)pData) % 8 == 0)) // ensure medium align

#define AMX_RUN_HANDLER_LONG(phandlerfn, token, pArgs, numargs, pData, datalen)   \
    _AMX_RUN_HANDLER_MEDLONG(phandlerfn, token, pArgs, numargs, pData, datalen, ((void)0))

//------------------------------------------------------------------------------------
#ifndef AMX_ENV_PREFIX_STR // AMX_ENV_PREFIX_STR is the safest define, to avoid conflicting defns
  #ifdef AMX_ENV_PREFIX
    #define AMX_ENV_PREFIX_STR AMX_STRINGIFY(AMX_ENV_PREFIX)
  #elif defined(AMUDP_ENV_PREFIX)
    #define AMX_ENV_PREFIX_STR AMX_STRINGIFY(AMUDP_ENV_PREFIX)
  #elif defined(AMMPI_ENV_PREFIX)
    #define AMX_ENV_PREFIX_STR AMX_STRINGIFY(AMMPI_ENV_PREFIX)
  #else
    #define AMX_ENV_PREFIX_STR AMX_LIB_STR
  #endif
#endif

/* return the first environment variable matching one of:
    AMX_ENV_PREFIX_STR##_##basekey
    AMX_LIB_STR##_##basekey
    basekey
   warn if more than one is set with different values
 */
extern char *AMX_getenv_prefixed(const char *basekey);
/* same with a default value and console reporting in verbose mode */
extern char *AMX_getenv_prefixed_withdefault(const char *basekey, const char *defaultval);

extern void AMX_freezeForDebugger(void);
//------------------------------------------------------------------------------------
/* *** TIMING *** */
#ifndef _AMX_TICK_T
#define _AMX_TICK_T
typedef uint64_t amx_tick_t;
#endif
#if defined(HAVE_GASNET_TOOLS)
  #define AMX_getMicrosecondTimeStamp() (gasnett_ticks_to_us(gasnett_ticks_now()))
  #define AMX_getCPUTicks()             ((amx_tick_t)gasnett_ticks_now())
  #define AMX_ticks2us(ticks)           (gasnett_ticks_to_us((gasnett_tick_t)(ticks)))
  #define AMX_us2ticks(us) \
    ((amx_tick_t)(us) * \
     (amx_tick_t)(1000000000.0 / gasnett_ticks_to_us((gasnett_tick_t)1000000000)))
  #define AMX_tickspersec               AMX_us2ticks(1000000)  
#else
  static int64_t AMX_getMicrosecondTimeStamp(void) {
    int64_t retval;
    struct timeval tv;
    retry:
    if (gettimeofday(&tv, NULL))
      AMX_FatalErr("gettimeofday failed: %s",strerror(errno));
    retval = ((int64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
    #if PLATFORM_OS_UNICOS
      /* fix an empirically observed bug in UNICOS gettimeofday(),
         which occasionally returns ridiculously incorrect values
       */
      if_pf(retval < (((int64_t)3) << 48)) goto retry;
    #endif
    return retval;
  }
  /* Ticks == us for gettimeofday */
  #define AMX_getCPUTicks()             ((amx_tick_t)AMX_getMicrosecondTimeStamp())
  #define AMX_ticks2us(ticks)           (ticks)
  #define AMX_us2ticks(us)              ((amx_tick_t)(us))
  #define AMX_tickspersec               1000000
#endif
//------------------------------------------------------------------------------------
#if PLATFORM_OS_SUPERUX || PLATFORM_OS_NETBSD || \
    PLATFORM_OS_BLRTS || PLATFORM_OS_OPENBSD
  /* these implement sched_yield() in libpthread only, which we may not want */
  #define AMX_yield() sleep(0)
#elif defined(HAVE_GASNET_TOOLS)
#else

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
