/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp.h $
 * Description: AMUDP Namespace-clean Header for constants
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef __AMUDP_CONST_H
#define __AMUDP_CONST_H

#undef _PORTABLE_PLATFORM_H
#include <amx_portable_platform.h>

/* naming policy:
  AM-defined things start with AM_
  internal things start with amudp_ or AMUDP_
  */

#ifndef AMX_STRINGIFY
#define _AMX_STRINGIFY_HELPER(x) #x
#define AMX_STRINGIFY(x) _AMX_STRINGIFY_HELPER(x)
#endif

#ifndef AMX_CONCAT
#define _AMX_CONCAT_HELPER(a,b) a ## b
#define AMX_CONCAT(a,b) _AMX_CONCAT_HELPER(a,b)
#endif

#ifndef AMUDP
#define AMUDP 1
#endif

#define AMUDP_LIBRARY_VERSION_MAJOR   3
#define AMUDP_LIBRARY_VERSION_MINOR   16
#define AMUDP_LIBRARY_VERSION      AMUDP_LIBRARY_VERSION_MAJOR.AMUDP_LIBRARY_VERSION_MINOR
#define AMUDP_LIBRARY_VERSION_STR  AMX_STRINGIFY(AMUDP_LIBRARY_VERSION)

#if !defined(AMUDP_DEBUG) && !defined(AMUDP_NDEBUG)
  #if defined(GASNET_DEBUG) || defined(AMX_DEBUG)
    #define AMUDP_DEBUG 1
  #elif defined(GASNET_NDEBUG) || defined(AMX_NDEBUG)
    #define AMUDP_NDEBUG 1
  #endif
#endif
#if defined(AMUDP_DEBUG) && !defined(AMUDP_NDEBUG)
  #undef AMUDP_DEBUG
  #define AMUDP_DEBUG 1
#elif !defined(AMUDP_DEBUG) && defined(AMUDP_NDEBUG)
  #undef AMUDP_NDEBUG
  #define AMUDP_NDEBUG 1
#else
  #error bad defns of AMUDP_DEBUG and AMUDP_NDEBUG
#endif

#undef AMX_DEBUG
#undef AMX_NDEBUG

#ifdef AMUDP_DEBUG
  #define AMX_DEBUG AMUDP_DEBUG
  #define AMUDP_DEBUG_CONFIG _DEBUG
#endif
#ifdef AMUDP_NDEBUG
  #define AMX_NDEBUG AMUDP_NDEBUG
  #define AMUDP_DEBUG_CONFIG _NDEBUG
#endif
#if AMX_DEBUG_VERBOSE || AMUDP_DEBUG_VERBOSE || GASNET_DEBUG_VERBOSE
  #undef  AMX_DEBUG_VERBOSE
  #define AMX_DEBUG_VERBOSE 1
#endif

/* idiot proofing */
#if defined(AMUDP_DEBUG) && (defined(__OPTIMIZE__) || defined(NDEBUG))
    #error Tried to compile AMUDP client code with optimization enabled but also AMUDP_DEBUG (which seriously hurts performance). Disable C and C++ compiler optimization or reconfigure/rebuild without --enable-debug
#endif


/* ------------------------------------------------------------------------------------ */
/* Internal constants */
#define AMUDP_MAX_SHORT    16    /* max number of handler arguments, >=8 */
#define AMUDP_MAX_MEDIUM   AMUDP_MAX_LONG  /* max. data transmission unit for medium messages, >= 512 */

#if PLATFORM_OS_IRIX
  #define AMUDP_MAX_LONG     61000  /* max. UDP datagram on IRIX is apparently 61412 */
#elif PLATFORM_OS_TRU64 || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || \
      PLATFORM_OS_DARWIN || PLATFORM_OS_AIX
  #define AMUDP_MAX_LONG     9000   /* max UDP datagram on OSF/FREEBSD/DARWIN is apparently 9196 */
#else
  #define AMUDP_MAX_LONG     65000  /* default max. UDP datagram */
#endif

#define AMUDP_MAX_NUMHANDLERS      256  /* max. handler-table entries >= 256 */
#define AMUDP_INIT_NUMTRANSLATIONS 256
#define AMUDP_MAX_NUMTRANSLATIONS  (1U<<20) /* max. translation-table entries. Ensure P*D cannnt overflow int32 */
#define AMUDP_MAX_SEGLENGTH  ((uintptr_t)-1) /* max. dest_offset */

#define AMUDP_MAX_BUNDLES          255  /* max bundles that can be allocated */
#define AMUDP_MAX_NETWORKDEPTH     1024 /* max depth we ever allow user to ask for (constrained by instance bits) */
#define AMUDP_MAX_RECVDEPTH        65534 /* max length of recv queue we ever allow user to ask for (2^16-2) */
#define AMUDP_MAX_SPMDPROCS        AMUDP_MAX_NUMTRANSLATIONS  /* max SPMD procs we support */

#ifndef AMUDP_COLLECT_STATS
#define AMUDP_COLLECT_STATS   1
#endif
#ifndef AMUDP_COLLECT_LATENCY_STATS
#define AMUDP_COLLECT_LATENCY_STATS   1
#endif

/* ------------------------------------------------------------------------------------ */
/* User-visible constants */

#define AM_ALL     1    /* Deliver all messages to endpoint */
#define AM_NONE    0    /* Deliver no messages to endpoint */

typedef enum {
  AM_NOEVENTS,   /* No endpoint state transition generates an event */
  AM_NOTEMPTY,   /* A nonempty receive pool or a receive pool that has 
                    a message delivered to it generates an event */
  /* AM_CANSEND, */ /* TODO: can send without blocking */
  AM_NUMEVENTMASKS
} amudp_eventmask_t;

typedef enum {
  AM_SEQ,             /* Sequential bundle/endpoint access */
  AM_PAR,             /* Concurrent bundle/endpoint access */
  AM_NUM_BUNDLE_MODES
} amudp_bundle_mode_t;

/*
 * Return values to Active Message and Endpoint/Bundle API functions
 */
#define AM_OK           0       /* Function completed successfully */
#define AM_ERR_NOT_INIT 1       /* Active message layer not initialized */
#define AM_ERR_BAD_ARG  2       /* Invalid function parameter passed */
#define AM_ERR_RESOURCE 3       /* Problem with requested resource */
#define AM_ERR_NOT_SENT 4       /* Synchronous message not sent */
#define AM_ERR_IN_USE   5       /* Resource currently in use */

/*
 * Op codes for the AM error handler (opcode).
 */
#define AM_REQUEST_M      1
#define AM_REQUEST_IM     2
#define AM_REQUEST_XFER_M 3
#define AM_REPLY_M        4
#define AM_REPLY_IM       5
#define AM_REPLY_XFER_M   6

/* System parameters */

#define AM_MaxShort()   AMUDP_MAX_SHORT
#define AM_MaxMedium()  AMUDP_MAX_MEDIUM
#define AM_MaxLong()    AMUDP_MAX_LONG

#define AM_MaxNumHandlers()               AMUDP_MAX_NUMHANDLERS
#define AM_GetNumHandlers(ep, pnhandlers)  \
  ((ep) ? ((*(pnhandlers) = AMUDP_MAX_NUMHANDLERS), AM_OK) : AM_ERR_BAD_ARG)
#define AM_SetNumHandlers(ep, nhandlers)  \
  ((ep) ? ((nhandlers) == AMUDP_MAX_NUMHANDLERS ? AM_OK : AM_ERR_RESOURCE) : AM_ERR_BAD_ARG)

#define AM_MaxNumTranslations(trans)      (*(trans) = AMUDP_MAX_NUMTRANSLATIONS,AM_OK)

/* ------------------------------------------------------------------------------------ */

#endif
