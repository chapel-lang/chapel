/*   $Source: /var/local/cvs/gasnet/gasnet_internal.h,v $
 *     $Date: 2010/04/17 02:09:20 $
 * $Revision: 1.118.2.2 $
 * Description: GASNet header for internal definitions used in GASNet implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_INTERNAL_H
#define _GASNET_INTERNAL_H
#define _IN_GASNET_INTERNAL_H
#define _INCLUDED_GASNET_INTERNAL_H
#ifdef _INCLUDED_GASNET_H
  #error Internal GASNet code should not directly include gasnet.h, just gasnet_internal.h
#endif

#include <gasnet.h> /* MUST come first to ensure correct inttypes behavior */
#include <gasnet_tools.h>
#include <gasnet_syncops.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#if defined(HAVE_MALLOC_H) && !PLATFORM_OS_OPENBSD /* OpenBSD warns that malloc.h is obsolete */
#include <malloc.h> /* prevent problems with redefinition of malloc on solaris */
#endif

GASNETI_BEGIN_EXTERNC

#if PLATFORM_COMPILER_SUN_C
  /* disable warnings triggerred by some macro idioms we use */
  #pragma error_messages(off, E_END_OF_LOOP_CODE_NOT_REACHED)
  #pragma error_messages(off, E_STATEMENT_NOT_REACHED)
#endif

extern int gasneti_init_done; /*  true after init */
extern int gasneti_attach_done; /*  true after attach */

extern char gasneti_exename[PATH_MAX];

/* conduit-independent sanity checks */
extern void gasneti_check_config_preinit(void);
extern void gasneti_check_config_postattach(void);

/* decode the command-line arguments */
extern void gasneti_decode_args(int *argc, char ***argv);

/* extract exit coordination timeout from environment vars (with defaults) */
extern double gasneti_get_exittimeout(double dflt_max, double dflt_min, double dflt_factor, double lower_bound);

/* Safe memory allocation/deallocation 
   Beware - in debug mode, gasneti_malloc/gasneti_calloc/gasneti_free are NOT
   compatible with malloc/calloc/free
   (freeing memory allocated from one using the other is likely to crash)
 */
#define gasneti_malloc(sz)           _gasneti_malloc((sz) GASNETI_CURLOCAARG)
#define gasneti_malloc_allowfail(sz) _gasneti_malloc_allowfail((sz) GASNETI_CURLOCAARG)
#define gasneti_calloc(N,S)          _gasneti_calloc(N,S GASNETI_CURLOCAARG)
#define gasneti_realloc(ptr,sz)      _gasneti_realloc((ptr),(sz) GASNETI_CURLOCAARG)
#define gasneti_free(ptr)	     _gasneti_free((ptr) GASNETI_CURLOCAARG)
#define gasneti_strdup(ptr)	     _gasneti_strdup((ptr) GASNETI_CURLOCAARG)
#define gasneti_strndup(ptr,sz)      _gasneti_strndup((ptr),(sz) GASNETI_CURLOCAARG)
/* corresponding gasneti_memcheck fns are in gasnet_help.h */

#if GASNET_DEBUG
  extern void *_gasneti_malloc(size_t nbytes, const char *curloc) GASNETI_MALLOC;
  extern void *_gasneti_malloc_allowfail(size_t nbytes, const char *curloc) GASNETI_MALLOC;
  extern void _gasneti_free(void *ptr, const char *curloc);
  extern void *_gasneti_realloc(void *ptr, size_t sz, const char *curloc);
  extern void *_gasneti_calloc(size_t N, size_t S, const char *curloc) GASNETI_MALLOC;
  extern size_t _gasneti_memcheck(void *ptr, const char *curloc, int checktype);
  extern void gasneti_malloc_dump_liveobjects(FILE *fp);
#else
  GASNETI_INLINE(_gasneti_malloc) GASNETI_MALLOC
  void *_gasneti_malloc(size_t nbytes) {
    void *ret = NULL;
    GASNETI_STAT_EVENT_VAL(I, GASNET_MALLOC, nbytes);
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    ret = malloc(nbytes);
    if_pf (ret == NULL && nbytes > 0) 
      gasneti_fatalerror("gasneti_malloc(%d) failed", (int)nbytes);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
    return ret;
  }
  GASNETI_INLINE(_gasneti_malloc_allowfail) GASNETI_MALLOC
  void *_gasneti_malloc_allowfail(size_t nbytes) {
    void *ret = NULL;
    GASNETI_STAT_EVENT_VAL(I, GASNET_MALLOC, nbytes);
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    ret = malloc(nbytes);
    if_pf (ret == NULL && nbytes > 0) /* allow a NULL return for out-of-memory */
      GASNETI_TRACE_PRINTF(I,("Warning: returning NULL for a failed gasneti_malloc(%i)",(int)nbytes));
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
    return ret;
  }
  GASNETI_INLINE(_gasneti_calloc) GASNETI_MALLOC
  void *_gasneti_calloc(size_t N, size_t S) {
    void *ret = NULL;
    GASNETI_STAT_EVENT_VAL(I, GASNET_MALLOC, (N*S));
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    ret = calloc(N,S);
    if_pf (ret == NULL && N*S > 0) 
      gasneti_fatalerror("gasneti_calloc(%d,%d) failed", (int)N, (int)S);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
    return ret;
  }
  GASNETI_INLINE(_gasneti_realloc)
  void *_gasneti_realloc(void *ptr, size_t nbytes) {
    void *ret = NULL;
    GASNETI_STAT_EVENT_VAL(I, GASNET_MALLOC, nbytes);
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    ret = realloc(ptr, nbytes);
    if_pf (ret == NULL && nbytes > 0) 
      gasneti_fatalerror("gasneti_realloc(%d) failed", (int)nbytes);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
    return ret;
  }
  GASNETI_INLINE(_gasneti_free)
  void _gasneti_free(void *ptr) {
    GASNETI_STAT_EVENT_VAL(I, GASNET_FREE, 0); /* don't track free size in ndebug mode */
    if_pf (ptr == NULL) return;
    if_pt (gasneti_attach_done) gasnet_hold_interrupts();
    free(ptr);
    if_pt (gasneti_attach_done) gasnet_resume_interrupts();
  }
#endif
GASNETI_MALLOCP(_gasneti_malloc)
GASNETI_MALLOCP(_gasneti_malloc_allowfail)
GASNETI_MALLOCP(_gasneti_calloc)

/* ------------------------------------------------------------------------------------ */
/* Enforce some of the GASNet conduit coding practices - see README */

#ifdef malloc
#undef malloc
#endif
#define gasneti_malloc_error  ERROR__GASNet_conduit_code_must_use_gasneti_malloc
#define malloc(n)             gasneti_malloc_error
#ifdef calloc
#undef calloc
#endif
#define gasneti_calloc_error  ERROR__GASNet_conduit_code_must_use_gasneti_calloc
#define calloc(n,s)           gasneti_calloc_error
#ifdef realloc
#undef realloc
#endif
#define gasneti_realloc_error ERROR__GASNet_conduit_code_must_use_gasneti_realloc
#define realloc(p,n)          gasneti_realloc_error
#ifdef strdup
#undef strdup
#endif
#define gasneti_strdup_error  ERROR__GASNet_conduit_code_must_use_gasneti_strdup
#define strdup(p)             gasneti_strdup_error
#ifdef strndup
#undef strndup
#endif
#define gasneti_strndup_error ERROR__GASNet_conduit_code_must_use_gasneti_strndup
#define strndup(p,n)          gasneti_strndup_error
#ifdef free
#undef free
#endif
#define gasneti_free_error    ERROR__GASNet_conduit_code_must_use_gasneti_free
#define free(p)               gasneti_free_error

#include <assert.h>
#undef assert
#define gasneti_assert_error  ERROR__GASNet_conduit_code_should_use_gasneti_assert
#define assert(x)             gasneti_assert_error

#if GASNETI_NO_FORK
  #ifdef fork
    #undef fork
  #endif
  #define gasneti_fork_error  ERROR__GASNet_conduit_code_calling_fork_while_GASNETI_NO_FORK
  #define fork()	      gasneti_fork_error
  #ifdef vfork
    #undef vfork
  #endif
  #define gasneti_vfork_error ERROR__GASNet_conduit_code_calling_vfork_while_GASNETI_NO_FORK
  #define vfork()	      gasneti_vfork_error
  #ifdef popen
    #undef popen
  #endif
  #define gasneti_popen_error ERROR__GASNet_conduit_code_calling_popen_while_GASNETI_NO_FORK
  #define popen(c,t)	      gasneti_popen_error
  #ifdef system
    #undef system
  #endif
  #define gasneti_system_error ERROR__GASNet_conduit_code_calling_system_while_GASNETI_NO_FORK
  #define system(s)            gasneti_system_error
#endif

/* ------------------------------------------------------------------------------------ */
/* Version of strdup() which is compatible w/ gasneti_free(), instead of plain free() */
GASNETI_INLINE(_gasneti_strdup) GASNETI_MALLOC
char *_gasneti_strdup(const char *s GASNETI_CURLOCFARG) {
  char *retval;
  if_pf (s == NULL) {
    /* special case to avoid strlen(NULL) */
    retval = (char *)_gasneti_malloc(1 GASNETI_CURLOCPARG);
    retval[0] = '\0';
  } else {
    size_t sz = strlen(s) + 1;
    retval = (char *)memcpy((char *)_gasneti_malloc(sz GASNETI_CURLOCPARG), s, sz);
  }
  return retval;
}
GASNETI_MALLOCP(_gasneti_strdup)
/* Like gasneti_strdup, but copy is limited to at most n characters.
 * Note allocation is upto n+1 bytes, due to the '\0' termination.
 */
GASNETI_INLINE(_gasneti_strndup) GASNETI_MALLOC
char *_gasneti_strndup(const char *s, size_t n GASNETI_CURLOCFARG) {
  char *retval;
  if_pf (s == NULL) {
    retval = (char *)_gasneti_malloc(1 GASNETI_CURLOCPARG);
    retval[0] = '\0';
  } else {
    size_t len;
    for (len = 0; len < n && s[len]; len++) ;
    retval = _gasneti_malloc(len + 1 GASNETI_CURLOCPARG);
    memcpy(retval, s, len);
    retval[len] = '\0';
  }
  return retval;
}
GASNETI_MALLOCP(_gasneti_strndup)

/* ------------------------------------------------------------------------------------ */

extern void gasneti_freezeForDebugger(void);

/* GASNET_DEBUG_VERBOSE is set by configure to request job startup and general 
   status messages on stderr 
*/
#ifndef GASNET_DEBUG_VERBOSE
  #define GASNET_DEBUG_VERBOSE               0
#endif

/* ------------------------------------------------------------------------------------ */
/* memory segment registration and management */

void gasneti_registerSignalHandlers(gasneti_sighandlerfn_t handler);
void gasneti_defaultSignalHandler(int sig);

#ifdef HAVE_MMAP
  extern gasnet_seginfo_t gasneti_mmap_segment_search(uintptr_t maxsz);
  extern void gasneti_mmap_fixed(void *segbase, uintptr_t segsize);
  extern void *gasneti_mmap(uintptr_t segsize);
  extern void gasneti_munmap(void *segbase, uintptr_t segsize);
  #ifndef GASNETI_MMAP_MAX_SIZE
    /* GASNETI_MMAP_MAX_SIZE controls the maz size segment attempted by the mmap binary search
       can't use a full 2 GB due to sign bit problems 
       on the int argument to mmap() for some 32-bit systems
       This setting can be overridden using configure --with-segment-mmap-max=XGB
     */
    #define GASNETI_MMAP_MAX_SIZE	  ((((uint64_t)1)<<31) - GASNET_PAGESIZE)  /* 2 GB */
  #endif
  uintptr_t _gasneti_max_segsize(uint64_t configure_val);
  /* GASNETI_MMAP_LIMIT is the user-selected limit for the max mmap size, as gleaned from several sources */
  #define GASNETI_MMAP_LIMIT _gasneti_max_segsize(GASNETI_MMAP_MAX_SIZE)
  #ifndef GASNETI_MMAP_GRANULARITY
    /* GASNETI_MMAP_GRANULARITY is the minimum increment used by the mmap binary search */
    #define GASNETI_MMAP_GRANULARITY  (((size_t)2)<<21)  /* 4 MB */
  #endif
#else
  #ifndef GASNETI_MALLOCSEGMENT_MAX_SIZE
  #define GASNETI_MALLOCSEGMENT_MAX_SIZE (100*1048576) /* Max segment sz to use when mmap not avail */
  #endif
  #define GASNETI_MALLOCSEGMENT_LIMIT _gasneti_max_segsize(GASNETI_MALLOCSEGMENT_MAX_SIZE)
#endif

#ifndef GASNETI_USE_HIGHSEGMENT
#define GASNETI_USE_HIGHSEGMENT 1  /* use the high end of mmap segments */
#endif

typedef void (*gasneti_bootstrapExchangefn_t)(void *src, size_t len, void *dest);
typedef void (*gasneti_bootstrapBroadcastfn_t)(void *src, size_t len, void *dest, int rootnode);
typedef void (*gasneti_bootstrapBarrierfn_t)(void);

#if !GASNET_SEGMENT_EVERYTHING
#ifdef HAVE_MMAP
uintptr_t gasneti_mmapLimit(uintptr_t localLimit, uint64_t sharedLimit,
                            gasneti_bootstrapExchangefn_t exchangefn,
                            gasneti_bootstrapBarrierfn_t barrierfn);
#endif /* HAVE_MMAP */
void gasneti_segmentInit(uintptr_t localSegmentLimit,
                         gasneti_bootstrapExchangefn_t exchangefn);
void gasneti_segmentAttach(uintptr_t segsize, uintptr_t minheapoffset,
                           gasnet_seginfo_t *seginfo,
                           gasneti_bootstrapExchangefn_t exchangefn);
#endif /* !GASNET_SEGMENT_EVERYTHING */
void gasneti_setupGlobalEnvironment(gasnet_node_t numnodes, gasnet_node_t mynode,
                                     gasneti_bootstrapExchangefn_t exchangefn,
                                     gasneti_bootstrapBroadcastfn_t broadcastfn);

/* signature for internally-registered functions that need auxseg space -
   space in the gasnet-registered heap which is hidden from the client.
   The callback is registered by adding the function pointer to GASNET*_AUXSEG_FNS()
   Each registered function is called twice by the GASNet framework at startup:
   * first callback is a "how much space do you want" query
    it occurs between init/attach with auxseg_info == NULL, 
    function should return the absolute minimum and desired auxseg space
    currently, all nodes MUST return the same value (may be relaxed in the future)
   * second callback is "ok, here's what you got"
    it happens after attach and before gasnete_init, with auxseg_info
    set to the array (gasnet_nodes() elements) of auxseg components on each node
    indicating the space assigned to this auxseg consumer.
    callee must copy the array of metadata if it wants to keep it 
    (the seg space it references is permanent)
 */
typedef struct {
  uintptr_t minsz;
  uintptr_t optimalsz;
} gasneti_auxseg_request_t;

typedef gasneti_auxseg_request_t (*gasneti_auxsegregfn_t)(gasnet_seginfo_t *auxseg_info);

/* collect required auxseg sizes and subtract them from the max values to report to client */
void gasneti_auxseg_init(void);

/* consume the client's segsize request and return the 
   value to acquire including auxseg requirements */
uintptr_t gasneti_auxseg_preattach(uintptr_t client_request_sz);

/* provide auxseg to GASNet components and init secondary segment arrays 
   requires gasneti_seginfo has been initialized to the correct values
 */
void gasneti_auxseg_attach(void);

#if GASNET_SEGMENT_EVERYTHING
  extern void gasnetc_auxseg_reqh(gasnet_token_t token, void *buf, size_t nbytes, 
                                  gasnet_handlerarg_t msg, gasnet_handlerarg_t offset);
  #define GASNETC_AUXSEG_HANDLERS() \
    gasneti_handler_tableentry_no_bits(gasnetc_auxseg_reqh)
#endif

/* ------------------------------------------------------------------------------------ */
#ifndef GASNETI_DISABLE_EOP_INTERFACE
#define GASNETI_HAVE_EOP_INTERFACE 1
#endif
#if GASNETI_HAVE_EOP_INTERFACE
/* GASNET-Internal OP Interface - provides a mechanism for conduit-independent services (like VIS)
   to expose non-blocking operations that utilize the regular GASNet op sync mechanisms
   Conduits provide two opaque scalar types: gasneti_eop_t and gasneti_iop_t
   and the following manipulator functions
 */
#ifndef _GASNETI_EOP_T
#define _GASNETI_EOP_T
struct _gasneti_eop_S;
typedef const struct _gasneti_eop_S gasneti_eop_t;
#endif

#ifndef _GASNETI_IOP_T
#define _GASNETI_IOP_T
struct _gasneti_iop_S;
typedef const struct _gasneti_iop_S gasneti_iop_t;
#endif

/* create a new explicit-handle NB operation
   represented with abstract type gasneti_eop_t
   and mark it in-flight */
gasneti_eop_t *gasneti_eop_create(GASNETE_THREAD_FARG_ALONE);

/* convert an gasneti_eop_t* created by an earlier call from this
   thread to gasneti_new_eop(), into a gasnet_handle_t suitable
   for this thread to later pass to gasnet_wait_syncnb & friends */
#if GASNETI_EOP_IS_HANDLE
  #define gasneti_eop_to_handle(eop) ((gasnet_handle_t)(eop))
#else
  gasnet_handle_t gasneti_eop_to_handle(gasneti_eop_t *eop);
#endif

/* register noperations in-flight operations on the currently selected 
   implicit-handle NB context represented with abstract type gasneti_iop_t, 
   and return a pointer to that context
   if isput is non-zero, the registered operations are puts, otherwise they are gets */
gasneti_iop_t *gasneti_iop_register(unsigned int noperations, int isget GASNETE_THREAD_FARG);

/* given an gasneti_eop_t* returned by an earlier call from any thread
   to gasneti_new_eop(), mark that explicit-handle NB operation complete
   such that a subsequent sync call on the relevant operation by the initiating
   thread may return success
   Caller is responsible for calling gasneti_sync_writes before calling this fn, if necessary
   AMSAFE: must be safe to call in AM context */
void gasneti_eop_markdone(gasneti_eop_t *eop);

/* given an gasneti_iop_t* returned by an earlier call from any thread
   to gasneti_iop_register(), increment that implicit-handle NB context
   to indicate that noperations have completed.
   if isput is non-zero, the operations are puts, otherwise they are gets
   noperations must not exceed the number of isput-type operations initiated
   on the given gasneti_iop_t by earlier calls to gasneti_iop_register
   Caller is responsible for calling gasneti_sync_writes before calling this fn, if necessary
   AMSAFE: must be safe to call in AM context */
void gasneti_iop_markdone(gasneti_iop_t *iop, unsigned int noperations, int isget);
#endif
/* ------------------------------------------------------------------------------------ */
/* macros for returning errors that allow verbose error tracking */
extern int gasneti_VerboseErrors;
#define GASNETI_RETURN_ERR(type) do {                                        \
  if (gasneti_VerboseErrors) {                                                 \
    fprintf(stderr, "GASNet %s returning an error code: GASNET_ERR_%s (%s)\n" \
      "  at %s:%i\n"                                                         \
      ,GASNETI_CURRENT_FUNCTION                                              \
      , #type, gasnet_ErrorDesc(GASNET_ERR_##type), __FILE__, __LINE__);     \
    fflush(stderr);                                                          \
    }                                                                        \
  gasnett_freezeForDebuggerErr(); /* allow freeze */                         \
  return GASNET_ERR_ ## type;                                                \
  } while (0)
#define GASNETI_RETURN_ERRF(type, fromfn) do {                                     \
  if (gasneti_VerboseErrors) {                                                     \
    fprintf(stderr, "GASNet %s returning an error code: GASNET_ERR_%s (%s)\n"      \
      "  from function %s\n"                                                       \
      "  at %s:%i\n"                                                               \
      ,GASNETI_CURRENT_FUNCTION                                                    \
      , #type, gasnet_ErrorDesc(GASNET_ERR_##type), #fromfn, __FILE__, __LINE__);  \
    fflush(stderr);                                                                \
    }                                                                              \
  gasnett_freezeForDebuggerErr(); /* allow freeze */                               \
  return GASNET_ERR_ ## type;                                                      \
  } while (0)
#define GASNETI_RETURN_ERRR(type, reason) do {                                             \
  if (gasneti_VerboseErrors) {                                                             \
    fprintf(stderr, "GASNet %s returning an error code: GASNET_ERR_%s (%s)\n"              \
      "  at %s:%i\n"                                                                       \
      "  reason: %s\n"                                                                     \
      ,GASNETI_CURRENT_FUNCTION                                                            \
      , #type, gasnet_ErrorDesc(GASNET_ERR_##type), __FILE__, __LINE__, reason);           \
    fflush(stderr);                                                                        \
    }                                                                                      \
  gasnett_freezeForDebuggerErr(); /* allow freeze */                                       \
  return GASNET_ERR_ ## type;                                                              \
  } while (0)
#define GASNETI_RETURN_ERRFR(type, fromfn, reason) do {                                    \
  if (gasneti_VerboseErrors) {                                                             \
    fprintf(stderr, "GASNet %s returning an error code: GASNET_ERR_%s (%s)\n"              \
      "  from function %s\n"                                                               \
      "  at %s:%i\n"                                                                       \
      "  reason: %s\n"                                                                     \
      ,GASNETI_CURRENT_FUNCTION                                                            \
      , #type, gasnet_ErrorDesc(GASNET_ERR_##type), #fromfn, __FILE__, __LINE__, reason);  \
    fflush(stderr);                                                                        \
    }                                                                                      \
  gasnett_freezeForDebuggerErr(); /* allow freeze */                                       \
  return GASNET_ERR_ ## type;                                                              \
  } while (0)

#ifndef GASNETI_ENABLE_ERRCHECKS
#define GASNETI_ENABLE_ERRCHECKS 0
#endif

#if GASNET_DEBUG || GASNETI_ENABLE_ERRCHECKS
  #define GASNETI_CHECK_ERR(errcond, type) \
    if_pf (errcond) GASNETI_RETURN_ERR(type)
  #define GASNETI_CHECK_ERRF(errcond, type, fromfn) \
    if_pf (errcond) GASNETI_RETURN_ERRF(type, fromfn)
  #define GASNETI_CHECK_ERRR(errcond, type, reason) \
    if_pf (errcond) GASNETI_RETURN_ERRR(type, reason)
  #define GASNETI_CHECK_ERRFR(errcond, type, fromfn, reason) \
    if_pf (errcond) GASNETI_RETURN_ERRFR(type, fromfn, reason)
#else
  #define GASNETI_CHECK_ERR(errcond, type)                    ((void)0)
  #define GASNETI_CHECK_ERRF(errcond, type, fromfn)           ((void)0)
  #define GASNETI_CHECK_ERRR(errcond, type, reason)           ((void)0)
  #define GASNETI_CHECK_ERRFR(errcond, type, fromfn, reason)  ((void)0)
#endif

/* return a possible error */
#define GASNETI_RETURN(expr) do {                                            \
  int _val = (expr);                                                         \
  if_pf (_val != GASNET_OK && gasneti_VerboseErrors) {                       \
    fprintf(stderr, "GASNet %s returning an error code: %s (%s)\n"           \
      "  at %s:%i\n"                                                         \
      ,GASNETI_CURRENT_FUNCTION                                              \
      , gasnet_ErrorName(_val), gasnet_ErrorDesc(_val), __FILE__, __LINE__); \
    fflush(stderr);                                                          \
    }                                                                        \
  return _val;                                                               \
  } while (0)

/* make a GASNet call - if it fails, print error message and return error */
#define GASNETI_SAFE_PROPAGATE(fncall) do {                  \
   int retcode = (fncall);                                   \
   if_pf (gasneti_VerboseErrors && retcode != GASNET_OK) {   \
     char msg[1024];                                         \
     sprintf(msg, "\nGASNet encountered an error: %s(%i)\n", \
        gasnet_ErrorName(retcode), retcode);                 \
     GASNETI_RETURN_ERRFR(RESOURCE, fncall, msg);            \
   }                                                         \
 } while (0)

/* ------------------------------------------------------------------------------------ */
/* common error-checking code for AM request/reply entry points */

#define GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs) do {               \
    GASNETI_CHECKATTACH();                                                     \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());             \
    GASNETI_TRACE_AMREQUESTSHORT(dest,handler,numargs);                        \
    GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high"); \
  } while (0)
#define GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs) do { \
    GASNETI_CHECKATTACH();                                                           \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());                   \
    GASNETI_TRACE_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);          \
    GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high");       \
    GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxMedium()),BAD_ARG,"nbytes too large");  \
  } while (0)
#define GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs) do { \
    GASNETI_CHECKATTACH();                                                                   \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());                           \
    GASNETI_TRACE_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);          \
    GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high");               \
    GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxLongRequest()),BAD_ARG,"nbytes too large");     \
    GASNETI_CHECK_ERRR((!gasneti_in_segment(dest, dest_addr, nbytes)),                       \
            BAD_ARG,"destination address out of segment range");                             \
  } while (0)
#define GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs) do { \
    GASNETI_CHECKATTACH();                                                                        \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());                                \
    GASNETI_TRACE_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);          \
    GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high");                    \
    GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxLongRequest()),BAD_ARG,"nbytes too large");          \
    GASNETI_CHECK_ERRR((!gasneti_in_segment(dest, dest_addr, nbytes)),                            \
            BAD_ARG,"destination address out of segment range");                                  \
  } while (0)
#define GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs) do {    \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs()); \
    GASNETI_TRACE_AMREPLYSHORT(token,handler,numargs);             \
  } while (0)
#define GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs) do { \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());                  \
    GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxMedium()),BAD_ARG,"nbytes too large"); \
    GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);          \
  } while (0)
#if GASNET_DEBUG || GASNETI_ENABLE_ERRCHECKS
  #define _GASNETI_COMMON_AMREPLYLONG_CHECKS(token,handler,source_addr,nbytes,dest_addr,numargs) do { \
      gasnet_node_t dest;                                                                             \
      GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));                                    \
      GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high");                      \
      GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxLongReply()),BAD_ARG,"nbytes too large");              \
      GASNETI_CHECK_ERRR((!gasneti_in_segment(dest, dest_addr, nbytes)),                              \
              BAD_ARG,"destination address out of segment range");                                    \
    } while (0)
#else
  #define _GASNETI_COMMON_AMREPLYLONG_CHECKS(token,handler,source_addr,nbytes,dest_addr,numargs) ((void)0)
#endif
#define GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs) do { \
    gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());                          \
    GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs);          \
    _GASNETI_COMMON_AMREPLYLONG_CHECKS(token,handler,source_addr,nbytes,dest_addr,numargs); \
  } while (0)

/* ------------------------------------------------------------------------------------ */
/* utility macros for dispatching AM handlers */

typedef void (*gasneti_HandlerShort) (gasnet_token_t token, ...);
typedef void (*gasneti_HandlerMedium)(gasnet_token_t token, void *buf, size_t nbytes, ...);
typedef void (*gasneti_HandlerLong)  (gasnet_token_t token, void *buf, size_t nbytes, ...);

/* default AM handler for unregistered entries - prints a fatal error */
extern void gasneti_defaultAMHandler(gasnet_token_t token);

/* ------------------------------------------------------------------------------------ */
#define GASNETI_RUN_HANDLER_SHORT(isReq, hid, phandlerfn, token, pArgs, numargs) do { \
  gasneti_assert(phandlerfn);                                                         \
  if (isReq) GASNETI_TRACE_AMSHORT_REQHANDLER(hid, token, numargs, pArgs);            \
  else       GASNETI_TRACE_AMSHORT_REPHANDLER(hid, token, numargs, pArgs);            \
  if (numargs == 0) (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token);       \
  else {                                                                              \
    gasnet_handlerarg_t *_args = (gasnet_handlerarg_t *)(pArgs); /* eval only once */ \
    switch (numargs) {                                                                \
      case 1:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0]); break; \
      case 2:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1]); break;\
      case 3:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2]); break; \
      case 4:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3]); break; \
      case 5:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4]); break; \
      case 6:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5]); break; \
      case 7:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6]); break; \
      case 8:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7]); break; \
      case 9:  (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8]); break; \
      case 10: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9]); break; \
      case 11: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10]); break; \
      case 12: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11]); break; \
      case 13: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12]); break; \
      case 14: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13]); break; \
      case 15: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13], _args[14]); break; \
      case 16: (*(gasneti_HandlerShort)phandlerfn)((gasnet_token_t)token, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13], _args[14], _args[15]); break; \
      default: gasneti_fatalerror("Illegal numargs=%i in GASNETI_RUN_HANDLER_SHORT", (int)numargs);        \
      }                                                                                                    \
    }                                                                                                      \
    GASNETI_TRACE_PRINTF(A,("AM%s_SHORT_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
  } while (0)
/* ------------------------------------------------------------------------------------ */
#define _GASNETI_RUN_HANDLER_MEDLONG(phandlerfn, token, pArgs, numargs, pData, datalen) do { \
  gasneti_assert(phandlerfn);                                                                \
  if (numargs == 0) (*phandlerfn)(token, pData, datalen);                                    \
  else {                                                                                     \
    gasnet_handlerarg_t *_args = (gasnet_handlerarg_t *)(pArgs); /* eval only once */        \
    switch (numargs) {                                                                       \
      case 1:  (*phandlerfn)(token, pData, datalen, _args[0]); break;                        \
      case 2:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1]); break;              \
      case 3:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2]); break;    \
      case 4:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3]); break; \
      case 5:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4]); break; \
      case 6:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5]); break; \
      case 7:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6]); break; \
      case 8:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7]); break; \
      case 9:  (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8]); break; \
      case 10: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9]); break; \
      case 11: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10]); break; \
      case 12: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11]); break; \
      case 13: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12]); break; \
      case 14: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13]); break; \
      case 15: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13], _args[14]); break; \
      case 16: (*phandlerfn)(token, pData, datalen, _args[0], _args[1], _args[2], _args[3], _args[4], _args[5], _args[6], _args[7], _args[8], _args[9], _args[10], _args[11], _args[12], _args[13], _args[14], _args[15]); break; \
      default: gasneti_fatalerror("Illegal numargs=%i in _GASNETI_RUN_HANDLER_MEDLONG", (int)numargs); \
      }                                                                                 \
    }                                                                                   \
  } while (0)

/* be default, we guarantee double-word alignment for data payload of medium xfers 
 */
#ifndef GASNETI_MEDBUF_ALIGNMENT
#define GASNETI_MEDBUF_ALIGNMENT 8
#endif

#define GASNETI_RUN_HANDLER_MEDIUM(isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen) do {      \
    gasneti_assert(((uintptr_t)pData) % GASNETI_MEDBUF_ALIGNMENT == 0 || datalen == 0);                     \
    if (isReq) GASNETI_TRACE_AMMEDIUM_REQHANDLER(hid, token, pData, datalen, numargs, pArgs);               \
    else       GASNETI_TRACE_AMMEDIUM_REPHANDLER(hid, token, pData, datalen, numargs, pArgs);               \
    _GASNETI_RUN_HANDLER_MEDLONG((gasneti_HandlerMedium)phandlerfn, (gasnet_token_t)token,                  \
                                 pArgs, numargs, (void *)pData, (int)datalen);                              \
    GASNETI_TRACE_PRINTF(A,("AM%s_MEDIUM_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
  } while (0)
#define GASNETI_RUN_HANDLER_LONG(isReq, hid, phandlerfn, token, pArgs, numargs, pData, datalen) do {      \
    if (isReq) GASNETI_TRACE_AMLONG_REQHANDLER(hid, token, pData, datalen, numargs, pArgs);               \
    else       GASNETI_TRACE_AMLONG_REPHANDLER(hid, token, pData, datalen, numargs, pArgs);               \
    _GASNETI_RUN_HANDLER_MEDLONG((gasneti_HandlerLong)phandlerfn, (gasnet_token_t)token,                  \
                                 pArgs, numargs, (void *)pData, (int)datalen);                            \
    GASNETI_TRACE_PRINTF(A,("AM%s_LONG_HANDLER: handler execution complete", (isReq?"REQUEST":"REPLY"))); \
  } while (0)
/* ------------------------------------------------------------------------------------ */
/* nodemap data and functions */

extern gasnet_node_t *gasneti_nodemap;
extern gasnet_node_t *gasneti_nodemap_local;
extern gasnet_node_t gasneti_nodemap_local_count;
extern gasnet_node_t gasneti_nodemap_local_rank;
extern gasnet_node_t gasneti_nodemap_global_count;
extern gasnet_node_t gasneti_nodemap_global_rank;

extern void gasneti_nodemapInit(gasneti_bootstrapExchangefn_t exchangefn,
                                const void *ids, size_t sz, size_t stride);
extern void gasneti_nodemapParse(void);
extern void gasneti_nodemapFini(void);

/* ------------------------------------------------------------------------------------ */

#if GASNET_PSHM
#include <gasnet_pshm.h>
#endif

/* ------------------------------------------------------------------------------------ */
GASNETI_END_EXTERNC

#undef _IN_GASNET_INTERNAL_H
#endif
