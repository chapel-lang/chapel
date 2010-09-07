/*   $Source: /var/local/cvs/gasnet/gasnet.h,v $
 *     $Date: 2009/09/18 23:33:23 $
 * $Revision: 1.63 $
 * Description: GASNet Header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_H
#define _GASNET_H
#define _IN_GASNET_H
#define _INCLUDED_GASNET_H
#ifdef _INCLUDED_GASNET_TOOLS_H
  #error Objects that use both GASNet and GASNet tools must   \
         include gasnet.h before gasnet_tools.h 
#endif
#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_IN_GASNET_INTERNAL_H)
  #error Internal GASNet code should not directly include gasnet.h, just gasnet_internal.h
#endif

/* Usage:
   see the GASNet specification and top-level README for details on how to use the GASNet interface
   clients should use the automatically-generated Makefile *.mak fragments to get the correct compile settings
*/

/* autoconf-generated configuration header */
#include <gasnet_config.h>

/* public spec version numbers */
#define GASNET_SPEC_VERSION_MAJOR GASNETI_SPEC_VERSION_MAJOR
#define GASNET_SPEC_VERSION_MINOR GASNETI_SPEC_VERSION_MINOR

/*  legacy name for major spec version number */
#define GASNET_VERSION GASNET_SPEC_VERSION_MAJOR

/* ------------------------------------------------------------------------------------ */
/* check threading configuration */
#if defined(GASNET_SEQ) && !defined(GASNET_PARSYNC) && !defined(GASNET_PAR)
  #undef GASNET_SEQ
  #define GASNET_SEQ 1
  #define GASNETI_THREAD_MODEL SEQ
#elif !defined(GASNET_SEQ) && defined(GASNET_PARSYNC) && !defined(GASNET_PAR)
  #undef GASNET_PARSYNC
  #define GASNET_PARSYNC 1
  #define GASNETI_THREAD_MODEL PARSYNC
#elif !defined(GASNET_SEQ) && !defined(GASNET_PARSYNC) && defined(GASNET_PAR)
  #undef GASNET_PAR
  #define GASNET_PAR 1
  #define GASNETI_THREAD_MODEL PAR
#else
  #error Client code must #define exactly one of (GASNET_PAR, GASNET_PARSYNC, GASNET_SEQ) before #including gasnet.h
#endif

/* GASNETI_CLIENT_THREADS = GASNet client has multiple application threads */
#if GASNET_PAR || GASNET_PARSYNC
  #define GASNETI_CLIENT_THREADS 1
#elif defined(GASNETI_CLIENT_THREADS)
  #error bad defn of GASNETI_CLIENT_THREADS
#endif

#if !((defined(GASNET_DEBUG) && !defined(GASNET_NDEBUG)) || (!defined(GASNET_DEBUG) && defined(GASNET_NDEBUG)))
  #error Conflicting or incorrect definitions of GASNET_DEBUG and GASNET_NDEBUG
#endif

/* codify other configuration settings */
#ifdef GASNET_DEBUG
  #undef GASNET_DEBUG
  #define GASNET_DEBUG 1
  #define GASNETI_DEBUG_CONFIG debug
#else
  #define GASNETI_DEBUG_CONFIG nodebug
#endif

#ifdef GASNET_TRACE
  #undef GASNET_TRACE
  #define GASNET_TRACE 1
  #define GASNETI_TRACE_CONFIG trace
#else
  #define GASNETI_TRACE_CONFIG notrace
#endif

#ifdef GASNET_STATS
  #undef GASNET_STATS
  #define GASNET_STATS 1
  #define GASNETI_STATS_CONFIG stats
#else
  #define GASNETI_STATS_CONFIG nostats
#endif

#if defined(GASNET_SRCLINES) || defined(GASNET_DEBUG)
  #define GASNETI_SRCLINES_FORCE
#endif
#if defined(GASNET_SRCLINES) || defined(GASNET_TRACE)
  #undef GASNET_SRCLINES
  #define GASNET_SRCLINES 1
  #define GASNETI_SRCLINES_CONFIG srclines
#else
  #define GASNETI_SRCLINES_CONFIG nosrclines
#endif

#if defined(GASNET_STATS) || defined(GASNET_TRACE)
  #define GASNETI_STATS_OR_TRACE 1
#elif defined(GASNETI_STATS_OR_TRACE)
  #error bad def of GASNETI_STATS_OR_TRACE
#endif

#if defined(GASNET_PSHM)
  #undef GASNET_PSHM
  #define GASNET_PSHM 1
  #define GASNETI_PSHM_CONFIG pshm
#else
  #define GASNET_PSHM 0
  #define GASNETI_PSHM_CONFIG nopshm
#endif

/* basic utilities used in the headers */
#include <gasnet_basic.h>

/* ------------------------------------------------------------------------------------ */
/* check segment configuration */

#if defined(GASNET_SEGMENT_FAST) && !defined(GASNET_SEGMENT_LARGE) && !defined(GASNET_SEGMENT_EVERYTHING)
  #undef GASNET_SEGMENT_FAST
  #define GASNET_SEGMENT_FAST 1
  #define GASNETI_SEGMENT_CONFIG FAST
#elif !defined(GASNET_SEGMENT_FAST) && defined(GASNET_SEGMENT_LARGE) && !defined(GASNET_SEGMENT_EVERYTHING)
  #undef GASNET_SEGMENT_LARGE
  #define GASNET_SEGMENT_LARGE 1
  #define GASNETI_SEGMENT_CONFIG LARGE
#elif !defined(GASNET_SEGMENT_FAST) && !defined(GASNET_SEGMENT_LARGE) && defined(GASNET_SEGMENT_EVERYTHING)
  #undef GASNET_SEGMENT_EVERYTHING
  #define GASNET_SEGMENT_EVERYTHING 1
  #define GASNETI_SEGMENT_CONFIG EVERYTHING
#else
  #error Segment configuration must be exactly one of (GASNET_SEGMENT_FAST, GASNET_SEGMENT_LARGE, GASNET_SEGMENT_EVERYTHING) 
#endif

/* additional safety check, in case a very smart linker removes all of the checks at the end of this file */
#define gasnet_init _CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT( \
                    gasnet_init_GASNET_,                             \
                    GASNETI_THREAD_MODEL),                           \
                    GASNETI_PSHM_CONFIG),                            \
                    GASNETI_SEGMENT_CONFIG),                         \
                    GASNETI_DEBUG_CONFIG),                           \
                    GASNETI_TRACE_CONFIG),                           \
                    GASNETI_STATS_CONFIG),                           \
                    GASNETI_SRCLINES_CONFIG)

/* ------------------------------------------------------------------------------------ */
/* GASNet forward definitions, which may override some of the defaults below */
#include <gasnet_core_fwd.h>
#include <gasnet_extended_fwd.h>

#include <gasnet_vis_fwd.h>
#include <gasnet_coll_fwd.h>

/* GASNETI_CONDUIT_THREADS = GASNet conduit has one or more private threads
                             which may be used to run AM handlers */
#if defined(GASNETI_CONDUIT_THREADS) && (GASNETI_CONDUIT_THREADS != 1)
  #error bad defn of GASNETI_CONDUIT_THREADS
#endif

/* GASNETI_THREADS = Threads exist at conduit and/or client level, 
                     and/or compiling for a tools-only client with thread-safety
*/
#if GASNETI_CLIENT_THREADS || GASNETI_CONDUIT_THREADS
  #define GASNETI_THREADS 1
#elif defined(GASNETI_THREADS)
  #error bad defn of GASNETI_THREADS
#endif

/* basic utilities used in the headers, which may require GASNETI_THREADS */
#include <gasnet_toolhelp.h>

/* GASNet memory barriers */
#include <gasnet_membar.h>

/* GASNet atomic memory operations */
#include <gasnet_atomicops.h>

/* ------------------------------------------------------------------------------------ */
/* constants */

#ifndef GASNET_MAXNODES
  /*  an integer representing the maximum number of nodes supported in a single GASNet job */
  #define GASNET_MAXNODES (0x7FFFFFFFu)
#endif

#if !defined(GASNET_ALIGNED_SEGMENTS) || \
    (defined(GASNET_ALIGNED_SEGMENTS) && GASNET_ALIGNED_SEGMENTS != 0 && GASNET_ALIGNED_SEGMENTS != 1)
  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
  #error GASNet core failed to define GASNET_ALIGNED_SEGMENTS to 0 or 1
#endif

#if GASNET_ALIGNED_SEGMENTS
  #define GASNETI_ALIGN_CONFIG align
#else 
  #define GASNETI_ALIGN_CONFIG noalign
#endif

#ifndef GASNET_BARRIERFLAG_ANONYMOUS
  /* barrier flags */
  #define GASNET_BARRIERFLAG_ANONYMOUS 1
  #define GASNET_BARRIERFLAG_MISMATCH  2
  #define GASNET_BARRIERFLAG_IMAGES 4
#endif

/* Errors: GASNET_OK must be zero */
#define GASNET_OK   0 

#ifndef _GASNET_ERRORS
#define _GASNET_ERRORS
  #define _GASNET_ERR_BASE 10000
  #define GASNET_ERR_NOT_INIT             (_GASNET_ERR_BASE+1)
  #define GASNET_ERR_RESOURCE             (_GASNET_ERR_BASE+2)
  #define GASNET_ERR_BAD_ARG              (_GASNET_ERR_BASE+3)
  #define GASNET_ERR_NOT_READY            (_GASNET_ERR_BASE+4)
  #define GASNET_ERR_BARRIER_MISMATCH     (_GASNET_ERR_BASE+5)
#endif

/* Largest Medium supported by AMPSHM */
#ifndef GASNETI_MAX_MEDIUM_PSHM
  #define GASNETI_MAX_MEDIUM_PSHM 65000
#endif

GASNETI_BEGIN_EXTERNC
extern const char *gasnet_ErrorName(int);
extern const char *gasnet_ErrorDesc(int);
GASNETI_END_EXTERNC

/* ------------------------------------------------------------------------------------ */
/* core types */

#ifndef _GASNET_NODE_T
#define _GASNET_NODE_T
  /*  unsigned integer type representing a unique 0-based node index */
  typedef uint32_t gasnet_node_t;
#endif

#ifndef _GASNET_HANDLER_T
#define _GASNET_HANDLER_T
  /*  an unsigned integer type representing an index into the core API AM handler table */
  typedef uint8_t gasnet_handler_t;
#endif

#ifndef _GASNET_TOKEN_T
#define _GASNET_TOKEN_T
  /*  an opaque type passed to core API handlers which may be used to query message information  */
  typedef void *gasnet_token_t;
#endif

#ifndef _GASNET_HANDLERARG_T
#define _GASNET_HANDLERARG_T
  /*  a 32-bit signed integer type which is used to express the user-provided arguments to all AM handlers. Platforms lacking a native 32-bit type may define this to a 64-bit type, but only the lower 32-bits are transmitted during an AM message send (and sign-extended on the receiver). */
  typedef int32_t gasnet_handlerarg_t;
#endif

#ifndef _GASNET_HANDLERENTRY_T
#define _GASNET_HANDLERENTRY_T
  /*  struct type used to negotiate handler registration in gasnet_init() */
  typedef struct gasneti_handlerentry_s {
    gasnet_handler_t index; /*  == 0 for don't care  */
   #ifdef GASNET_USE_STRICT_PROTOTYPES
    void *fnptr;    
   #else
    void (*fnptr)();    
   #endif
  } gasnet_handlerentry_t;
#endif

#ifndef _GASNET_SEGINFO_T
#define _GASNET_SEGINFO_T
  typedef struct gasneti_seginfo_s {
    void *addr;
    uintptr_t size;
  #if GASNET_PSHM
    void *remote_addr;
    uintptr_t remote_size;
  #endif
    gasnet_node_t nodeinfo;
  } gasnet_seginfo_t;
#endif

#ifndef _GASNET_THREADINFO_T
#define _GASNET_THREADINFO_T
  typedef void *gasnet_threadinfo_t;
#endif

/* ------------------------------------------------------------------------------------ */
/* extended types */

#ifndef _GASNET_HANDLE_T
#define _GASNET_HANDLE_T
  /*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
  typedef void *gasnet_handle_t;
  #define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#endif

#ifndef _GASNET_REGISTER_VALUE_T
#define _GASNET_REGISTER_VALUE_T
  /*  the largest unsigned integer type that can fit entirely in a single CPU register for the current architecture and ABI.  */
  /*  SIZEOF_GASNET_REGISTER_VALUE_T is a preprocess-time literal integer constant (i.e. not "sizeof()")indicating the size of this type in bytes */
  typedef uintptr_t gasnet_register_value_t;
  #define SIZEOF_GASNET_REGISTER_VALUE_T  SIZEOF_VOID_P
#endif

#ifndef _GASNET_MEMVEC_T
#define _GASNET_MEMVEC_T
  typedef struct {
    void *addr;
    size_t len;
  } gasnet_memvec_t;
#endif

/* ------------------------------------------------------------------------------------ */

/* Main core header */
#include <gasnet_core.h>

/* Main extended header */
#include <gasnet_extended.h>

/* ------------------------------------------------------------------------------------ */

#ifndef GASNET_BEGIN_FUNCTION
  #error GASNet extended API failed to define GASNET_BEGIN_FUNCTION
#endif

#ifndef GASNET_HSL_INITIALIZER
  #error GASNet core failed to define GASNET_HSL_INITIALIZER
#endif

#ifndef GASNET_BLOCKUNTIL
  #error GASNet core failed to define GASNET_BLOCKUNTIL
#endif

#ifndef SIZEOF_GASNET_REGISTER_VALUE_T
  #error GASNet failed to define SIZEOF_GASNET_REGISTER_VALUE_T
#endif

/* GASNET_CONFIG_STRING
   a string representing all the relevant GASNet configuration settings 
   that can be compared using string compare to verify version compatibility
   The string is also embedded into the library itself such that it can be 
   scanned for within a binary executable.
*/
#ifndef GASNET_CONFIG_STRING
  /* allow extension by core and extended (all extensions should follow the same 
     basic comma-delimited feature format below, and include an initial comma)
   */
  #ifndef GASNETC_EXTRA_CONFIG_INFO 
    #define GASNETC_EXTRA_CONFIG_INFO
  #endif
  #ifndef GASNETE_EXTRA_CONFIG_INFO
    #define GASNETE_EXTRA_CONFIG_INFO
  #endif
  #ifdef GASNETI_BUG1389_WORKAROUND
    #define GASNETC_BUG1389_CONFIG_INFO ",ConservativeLocalCopy"
  #else
    #define GASNETC_BUG1389_CONFIG_INFO
  #endif
  #define GASNET_CONFIG_STRING                                            \
             "RELEASE=" _STRINGIFY(GASNETI_RELEASE_VERSION) ","           \
             "SPEC=" _STRINGIFY(GASNET_SPEC_VERSION_MAJOR) "."            \
             _STRINGIFY(GASNET_SPEC_VERSION_MINOR) ","                    \
             "CONDUIT=" GASNET_CONDUIT_NAME_STR "("                       \
             GASNET_CORE_NAME_STR"-"GASNET_CORE_VERSION_STR "/"           \
             GASNET_EXTENDED_NAME_STR"-"GASNET_EXTENDED_VERSION_STR "),"  \
             "THREADMODEL=" _STRINGIFY(GASNETI_THREAD_MODEL) ","          \
             "SEGMENT=" _STRINGIFY(GASNETI_SEGMENT_CONFIG) ","            \
             "PTR=" _STRINGIFY(GASNETI_PTR_CONFIG) ","                    \
             _STRINGIFY(GASNETI_ALIGN_CONFIG) ","                         \
             _STRINGIFY(GASNETI_PSHM_CONFIG) ","                          \
             _STRINGIFY(GASNETI_DEBUG_CONFIG) ","                         \
             _STRINGIFY(GASNETI_TRACE_CONFIG) ","                         \
             _STRINGIFY(GASNETI_STATS_CONFIG) ","                         \
             _STRINGIFY(GASNETI_SRCLINES_CONFIG) ","                      \
             _STRINGIFY(GASNETI_TIMER_CONFIG) ","                         \
             _STRINGIFY(GASNETI_MEMBAR_CONFIG) ","                        \
             _STRINGIFY(GASNETI_ATOMIC_CONFIG) ","                        \
             _STRINGIFY(GASNETI_ATOMIC32_CONFIG) ","                      \
             _STRINGIFY(GASNETI_ATOMIC64_CONFIG)                          \
             GASNETC_BUG1389_CONFIG_INFO                                  \
             GASNETC_EXTRA_CONFIG_INFO                                    \
             GASNETE_EXTRA_CONFIG_INFO                                    
#endif

/* ensure that the client links the correct library configuration
 * all objects in a given executable (client and library) must agree on all
 * of the following configuration settings, otherwise MANY things break,
 * often in very subtle and confusing ways (eg GASNet mutexes, threadinfo, etc.)
 */
#define GASNETI_LINKCONFIG_IDIOTCHECK(name) _CONCAT(gasneti_linkconfig_idiotcheck_,name)
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME));

static int *gasneti_linkconfig_idiotcheck(void);
/* use of void* here avoids a tinyc bug */
static void *_gasneti_linkconfig_idiotcheck = (void *)&gasneti_linkconfig_idiotcheck;
GASNETI_USED
static int *gasneti_linkconfig_idiotcheck(void) {
  static int val;
  val +=  GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME))
        ;
  if (_gasneti_linkconfig_idiotcheck != (void*)&gasneti_linkconfig_idiotcheck)
    val += ((int(*)(void))_gasneti_linkconfig_idiotcheck)();
  return &val;
}
extern int gasneti_internal_idiotcheck(gasnet_handlerentry_t *table, int numentries,
                                       uintptr_t segsize, uintptr_t minheapoffset);

#if defined(GASNET_DEBUG) && (defined(__OPTIMIZE__) || defined(NDEBUG))
  #ifndef GASNET_ALLOW_OPTIMIZED_DEBUG
    #error Tried to compile GASNet client code with optimization enabled but also GASNET_DEBUG (which seriously hurts performance). Reconfigure/rebuild GASNet without --enable-debug
  #endif
#endif

/* ------------------------------------------------------------------------------------ */

#undef _IN_GASNET_H
#endif

/* intentionally expanded on every include */
#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_GASNET_INTERNAL_IDIOTCHECK)
  #define _GASNET_INTERNAL_IDIOTCHECK
  #undef gasnet_attach
  #define gasnet_attach  gasneti_internal_idiotcheck
#endif
