/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnetex.h $
 * Description: GASNet Header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNETEX_H
#define _GASNETEX_H
#define _IN_GASNETEX_H
#define _INCLUDED_GASNETEX_H
#ifdef _INCLUDED_GASNET_TOOLS_H
  #error Objects that use both GASNet-EX and GASNet tools must   \
         include gasnetex.h before gasnet_tools.h 
#endif
#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_IN_GASNET_INTERNAL_H)
  #error Internal GASNet code should not directly include gasnetex.h, just gasnet_internal.h
#endif

#ifdef __cplusplus
  extern "C" { // cannot use GASNETI_BEGIN_EXTERNC here due to a header dependency cycle
#endif

/* Usage:
   see the GASNet specification and top-level README for details on how to use the GASNet interface
   clients should use the automatically-generated Makefile *.mak fragments to get the correct compile settings
*/

/* autoconf-generated configuration header */
#include <gasnet_config.h>

/* public spec version numbers */
#define GEX_SPEC_VERSION_MAJOR GASNETI_EX_SPEC_VERSION_MAJOR
#define GEX_SPEC_VERSION_MINOR GASNETI_EX_SPEC_VERSION_MINOR

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
  #error Client code must #define exactly one of (GASNET_PAR, GASNET_PARSYNC, GASNET_SEQ) before #including gasnetex.h
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

#ifdef GASNET_DEBUGMALLOC
  #undef GASNET_DEBUGMALLOC
  #define GASNET_DEBUGMALLOC 1
  #define GASNETI_MALLOC_CONFIG debugmalloc
#else
  #define GASNETI_MALLOC_CONFIG nodebugmalloc
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

/* basic utilities used in the headers */
#include <gasnet_basic.h>

GASNETI_BEGIN_NOWARN

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

/* Too early to know if conduit supports PSHM, so safety check below uses this instead. */
#if defined(GASNETI_PSHM_ENABLED)
  #define GASNETI_PSHM_CONFIG_ENABLED pshm
#else
  #define GASNETI_PSHM_CONFIG_ENABLED nopshm
#endif

/* additional safety check, in case a very smart linker removes all of the checks at the end of this file */
#define gasnetc_Client_Init _CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT(_CONCAT( \
                    gex_Client_Init_GASNET_,                         \
                    GASNET_RELEASE_VERSION_MAJOR),                   \
                    GASNET_RELEASE_VERSION_MINOR),                   \
                    GASNET_RELEASE_VERSION_PATCH),                   \
                    GASNETI_THREAD_MODEL),                           \
                    GASNETI_PSHM_CONFIG_ENABLED),                    \
                    GASNETI_SEGMENT_CONFIG),                         \
                    GASNETI_DEBUG_CONFIG),                           \
                    GASNETI_TRACE_CONFIG),                           \
                    GASNETI_STATS_CONFIG),                           \
                    GASNETI_MALLOC_CONFIG),                          \
                    GASNETI_SRCLINES_CONFIG)
#define gex_Client_Init gasnetc_Client_Init

/* ------------------------------------------------------------------------------------ */
// Default (read-only) shared-memory MaxMedium value "recommended" to conduits.
// Value of GASNETC_MAX_MEDIUM_NBRHD determines the actual maximum.
// See template-conduit/gasnet_core.h for more info.
#if !GASNETI_PSHM_ENABLED
  #define GASNETC_MAX_MEDIUM_NBRHD_DFLT 65536
#elif PLATFORM_ARCH_64
  #define GASNETC_MAX_MEDIUM_NBRHD_DFLT 65416
#else
  #define GASNETC_MAX_MEDIUM_NBRHD_DFLT 65436
#endif

/* ------------------------------------------------------------------------------------ */
/* GASNet forward definitions, which may override some of the defaults below */
#include <gasnet_core_fwd.h>
#include <gasnet_extended_fwd.h>

#include <gasnet_vis_fwd.h>
#include <gasnet_coll_fwd.h>
#include <gasnet_ratomic_fwd.h>

/* GASNET_PSHM = GASNet conduit is using PSHM */
#if defined(GASNET_PSHM) && (GASNET_PSHM != 1)
  #error bad defn of GASNET_PSHM
#elif !defined(GASNET_PSHM)
  #define GASNET_PSHM 0
#endif

/* GASNETI_CONDUIT_THREADS = GASNet conduit has one or more private threads
                             which may be used to run AM handlers */
#if defined(GASNETI_CONDUIT_THREADS) && (GASNETI_CONDUIT_THREADS != 1)
  #error bad defn of GASNETI_CONDUIT_THREADS
#endif

/* GASNET_HIDDEN_AM_CONCURRENCY_LEVEL: non-zero iff the conduit may run AM
 * handlers on a thread not owned by the client 
 */
#if GASNETI_CONDUIT_THREADS
  #define GASNET_HIDDEN_AM_CONCURRENCY_LEVEL 1
#else
  #define GASNET_HIDDEN_AM_CONCURRENCY_LEVEL 0
#endif

/* GASNETI_THREADS = Threads exist at conduit and/or client level, 
                     and/or compiling for a tools-only client with thread-safety
*/
#if GASNETI_CLIENT_THREADS || GASNETI_CONDUIT_THREADS
  #define GASNETI_THREADS 1
  #define _GASNETI_PARSEQ _PAR
#elif defined(GASNETI_THREADS)
  #error bad defn of GASNETI_THREADS
#else
  #define _GASNETI_PARSEQ _SEQ
#endif

/* basic utilities used in the headers, which may require GASNETI_THREADS */
#include <gasnet_toolhelp.h>

/* GASNet memory barriers */
#include <gasnet_membar.h>

/* GASNet atomic memory operations */
#include <gasnet_atomicops.h>

/* ------------------------------------------------------------------------------------ */
/* constants */

/* selected constants and simple types */
#include <gasnet_fwd.h>

#ifndef GASNETC_HANDLER_BASE
  #define GASNETC_HANDLER_BASE 1
#endif
#ifndef GASNETE_HANDLER_BASE
  #define GASNETE_HANDLER_BASE 64
#endif
#ifndef GASNETI_CLIENT_HANDLER_BASE
  #define GASNETI_CLIENT_HANDLER_BASE 128
#endif
#ifndef GASNETC_MAX_NUMHANDLERS
  #define GASNETC_MAX_NUMHANDLERS 256
#endif

#define GEX_AM_INDEX_BASE GASNETI_CLIENT_HANDLER_BASE

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

#if GASNET_PSHM
  #define GASNETI_PSHM_CONFIG pshm
#else
  #define GASNETI_PSHM_CONFIG nopshm
#endif

#ifndef GASNET_BARRIERFLAG_ANONYMOUS
  /* barrier flags */
  #define GASNET_BARRIERFLAG_ANONYMOUS 1
  #define GASNET_BARRIERFLAG_MISMATCH  2

  /* UNNAMED includes ANONYMOUS to yield a trivial default implementation: */
  #define GASNETE_BARRIERFLAG_UNNAMED 8
  #define GASNET_BARRIERFLAG_UNNAMED (GASNET_BARRIERFLAG_ANONYMOUS|GASNETE_BARRIERFLAG_UNNAMED)

  /* reserve bits for use by conduit-specific implementations */
  #define GASNETE_BARRIERFLAG_CONDUIT0 0x80000000
  #define GASNETE_BARRIERFLAG_CONDUIT1 0x40000000
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

extern const char *gasnet_ErrorName(int);
extern const char *gasnet_ErrorDesc(int);

/* ------------------------------------------------------------------------------------ */
/* core types */

// TODO-EX: need comments here?
typedef uint8_t gex_AM_Index_t;
typedef int32_t gex_AM_Arg_t;

/*  an opaque type passed to core API handlers which may be used to query message information  */
struct gasneti_token_s;
typedef struct gasneti_token_s *gex_Token_t;

struct gasneti_team_member_s;
typedef struct gasneti_team_member_s *gex_TM_t;
#define GEX_TM_INVALID ((gex_TM_t)(uintptr_t)0)

struct gasneti_client_s;
typedef struct gasneti_client_s *gex_Client_t;

struct gasneti_endpoint_s;
typedef struct gasneti_endpoint_s *gex_EP_t;

struct gasneti_segment_s;
typedef struct gasneti_segment_s *gex_Segment_t;
#define GEX_SEGMENT_INVALID ((gex_Segment_t)(uintptr_t)0)

typedef void (*gex_AM_Fn_t)();

/*  struct type used to perform handler registration */
typedef struct {
    gex_AM_Index_t          gex_index;   // 0 on input == don't care
    gex_AM_Fn_t             gex_fnptr;
    gex_Flags_t             gex_flags;
    unsigned int            gex_nargs;

    // Optional fields (both are "shallow copy")
    const void             *gex_cdata;   // Available to handler
    const char             *gex_name;    // Used in debug messages
} gex_AM_Entry_t;

#if GASNET_DEBUG
  #define GASNETI_OBJECT_HEADER_DEBUG gasneti_magic_t _magic;
#else
  #define GASNETI_OBJECT_HEADER_DEBUG
#endif

#define GASNETI_OBJECT_HEADER          \
    GASNETI_OBJECT_HEADER_DEBUG        \
    const void *       _cdata;         \
    gex_Flags_t        _flags;

// Needed to break client/tm0 cycle
struct gasneti_team_member_internal_s;

#ifndef _GEX_CLIENT_T
  #define GASNETI_CLIENT_COMMON        \
    GASNETI_OBJECT_HEADER              \
    struct gasneti_team_member_internal_s *_tm0; \
    const char *       _name;
  typedef struct { GASNETI_CLIENT_COMMON } *gasneti_Client_t;
  #if GASNET_DEBUG
    extern gasneti_Client_t gasneti_import_client(gex_Client_t _client);
    extern gex_Client_t gasneti_export_client(gasneti_Client_t _real_client);
  #else
    #define gasneti_import_client(x) ((gasneti_Client_t)(x))
    #define gasneti_export_client(x) ((gex_Client_t)(x))
  #endif
  #define gex_Client_SetCData(client,val)      ((void)(gasneti_import_client(client)->_cdata = (val)))
  #define gex_Client_QueryCData(client)        ((void*)gasneti_import_client(client)->_cdata)
  #define gex_Client_QueryFlags(client)        ((gex_Flags_t)gasneti_import_client(client)->_flags)
  #define gex_Client_QueryName(client)         ((const char*)gasneti_import_client(client)->_name)
#endif

#ifndef _GEX_SEGMENT_T
  #define GASNETI_SEGMENT_COMMON       \
    GASNETI_OBJECT_HEADER              \
    gasneti_Client_t   _client;        \
    void *             _addr;          \
    void *             _ub;            \
    uintptr_t          _size;
  typedef struct { GASNETI_SEGMENT_COMMON } *gasneti_Segment_t;
  #if GASNET_DEBUG
    extern gasneti_Segment_t gasneti_import_segment(gex_Segment_t _segment);
    extern gex_Segment_t gasneti_export_segment(gasneti_Segment_t _real_segment);
  #else
    #define gasneti_import_segment(x) ((gasneti_Segment_t)(x))
    #define gasneti_export_segment(x) ((gex_Segment_t)(x))
  #endif
  #define gex_Segment_SetCData(seg,val)         ((void)(gasneti_import_segment(seg)->_cdata = (val)))
  #define gex_Segment_QueryCData(seg)           ((void*)gasneti_import_segment(seg)->_cdata)
  #define gex_Segment_QueryClient(seg)          gasneti_export_client(gasneti_import_segment(seg)->_client)
  #define gex_Segment_QueryFlags(seg)           ((gex_Flags_t)gasneti_import_segment(seg)->_flags)
  #define gex_Segment_QueryAddr(seg)            ((void*)gasneti_import_segment(seg)->_addr)
  #define gex_Segment_QuerySize(seg)            ((uintptr_t)gasneti_import_segment(seg)->_size)
#endif

#ifndef _GEX_EP_T
  #define GASNETI_EP_COMMON            \
    GASNETI_OBJECT_HEADER              \
    gasneti_Client_t   _client;        \
    gasneti_Segment_t  _segment;       \
    gex_AM_Entry_t     _amtbl[GASNETC_MAX_NUMHANDLERS];
  typedef struct { GASNETI_EP_COMMON } *gasneti_EP_t;
  #if GASNET_DEBUG
    extern gasneti_EP_t gasneti_import_ep(gex_EP_t _ep);
    extern gex_EP_t gasneti_export_ep(gasneti_EP_t _real_ep);
  #else
    #define gasneti_import_ep(x) ((gasneti_EP_t)(x))
    #define gasneti_export_ep(x) ((gex_EP_t)(x))
  #endif
  #define gex_EP_SetCData(ep,val)              ((void)(gasneti_import_ep(ep)->_cdata = (val)))
  #define gex_EP_QueryCData(ep)                ((void*)gasneti_import_ep(ep)->_cdata)
  #define gex_EP_QueryClient(ep)               gasneti_export_client(gasneti_import_ep(ep)->_client)
  #define gex_EP_QueryFlags(ep)                ((gex_Flags_t)gasneti_import_ep(ep)->_flags)
  #define gex_EP_QuerySegment(ep)              gasneti_export_segment(gasneti_import_ep(ep)->_segment)
#endif

#ifndef _GEX_TM_T
  #define GASNETI_TM_COMMON            \
    GASNETI_OBJECT_HEADER              \
    gasneti_EP_t       _ep;            \
    gex_Rank_t         _rank;          \
    gex_Rank_t         _size;          \
    void *             _coll_team;
  #ifdef __cplusplus  // ensure this struct is anonymous to prevent C++ linkage issues
    #define gasneti_team_member_internal_s
  #endif
  typedef struct gasneti_team_member_internal_s { GASNETI_TM_COMMON } *gasneti_TM_t;
  #if GASNET_DEBUG
    extern gasneti_TM_t gasneti_import_tm(gex_TM_t _tm);
    extern gex_TM_t gasneti_export_tm(gasneti_TM_t _real_tm);
  #else
    #define gasneti_import_tm(x) ((gasneti_TM_t)(x))
    #define gasneti_export_tm(x) ((gex_TM_t)(x))
  #endif
  #define gex_TM_SetCData(tm,val)              ((void)(gasneti_import_tm(tm)->_cdata = (val)))
  #define gex_TM_QueryCData(tm)                ((void*)gasneti_import_tm(tm)->_cdata)
  #define gex_TM_QueryClient(tm)               gasneti_export_client(gasneti_import_tm(tm)->_ep->_client)
  #define gex_TM_QueryEP(tm)                   gasneti_export_ep(gasneti_import_tm(tm)->_ep)
  #define gex_TM_QueryFlags(tm)                ((gex_Flags_t)gasneti_import_tm(tm)->_flags)
  #define gex_TM_QueryRank(tm)                 ((gex_Rank_t)gasneti_import_tm(tm)->_rank)
  #define gex_TM_QuerySize(tm)                 ((gex_Rank_t)gasneti_import_tm(tm)->_size)
#endif

// TODO-EX: remove these legacy checks
#ifdef _GASNET_NODE_T
#error "out-of-date #define of _GASNET_NODE_T"
#endif
#ifdef _GASNET_HANDLER_T
#error "out-of-date #define of _GASNET_HANDLER_T"
#endif
#ifdef _GASNET_TOKEN_T
#error "out-of-date #define of _GASNET_TOKEN_T"
#endif
#ifdef _GASNET_HANDLERARG_T
#error "out-of-date #define of _GASNET_HANDLERARG_T"
#endif
#ifdef _GASNET_REGISTER_VALUE_T
#error "out-of-date #define of _GASNET_REGISTER_VALUE_T"
#endif
#ifdef _GASNET_HANDLE_T
#error "out-of-date #define of _GASNET_HANDLE_T"
#endif
#ifdef _GASNET_HANDLERENTRY_T
#error "out-of-date #define of _GASNET_HANDLERENTRY_T"
#endif


/*  struct type used to return info from gex_Token_Info() */
typedef struct {
    gex_Rank_t                 gex_srcrank;
    gex_EP_t                   gex_ep;
    const gex_AM_Entry_t      *gex_entry;
    int                        gex_is_req;
    int                        gex_is_long;
} gex_Token_Info_t;

/*  constants to request specific info from gex_Token_Info() */
typedef unsigned int gex_TI_t;
#define GEX_TI_SRCRANK       ((gex_TI_t)1<<0)
#define GEX_TI_ENTRY         ((gex_TI_t)1<<1)
#define GEX_TI_IS_REQ        ((gex_TI_t)1<<2)
#define GEX_TI_IS_LONG       ((gex_TI_t)1<<3)
#define GEX_TI_EP            ((gex_TI_t)1<<4)
#define GEX_TI_ALL          (((gex_TI_t)1<<5) - 1)

// Default implementation
#ifndef gex_Token_Info
  #define gex_Token_Info gasnetc_Token_Info
#endif
extern gex_TI_t gex_Token_Info(
                gex_Token_t         _token,
                gex_Token_Info_t    *_info,
                gex_TI_t            _mask);

#ifndef _GASNET_SEGINFO_T
#define _GASNET_SEGINFO_T
  typedef struct gasneti_seginfo_s {
    void *addr;
    uintptr_t size;
  } gasnet_seginfo_t;
#endif

#ifndef _GASNET_NODEINFO_T
#define _GASNET_NODEINFO_T
  typedef struct gasneti_nodeinfo_s {
    gex_Rank_t host; /* 0-based identifier for procs on same compute node */
    gex_Rank_t supernode; /* 0-based identifier for procs which comprise a shared-memory supernode */
  #if GASNET_PSHM
    /* Value one must add to find locally mapped address, if any. */
    uintptr_t offset;
    uintptr_t auxoffset; // TODO-EX: this needs to move elsewhere
  #endif
  } gasnet_nodeinfo_t;
#endif

#ifndef _GASNET_THREADINFO_T
#define _GASNET_THREADINFO_T
  typedef void *gasnet_threadinfo_t;
#endif

typedef const struct {
  gex_Rank_t gex_jobrank;
  // Avert your eyes - space below is reserved for internal use

  // Nothing to see here (yet)
} gex_RankInfo_t;

extern void gex_System_QueryNbrhdInfo(
            gex_RankInfo_t         **_info_p,
            gex_Rank_t             *_info_count_p,
            gex_Rank_t             *_my_info_index_p);

extern void gex_System_QueryHostInfo(
            gex_RankInfo_t         **_info_p,
            gex_Rank_t             *_info_count_p,
            gex_Rank_t             *_my_info_index_p);

extern void gex_System_QueryMyPosition(
            gex_Rank_t *_nbrhd_set_size_p,
            gex_Rank_t *_nbrhd_set_rank_p,
            gex_Rank_t *_host_set_size_p,
            gex_Rank_t *_host_set_rank_p);

/* ------------------------------------------------------------------------------------ */
/* extended types */

  /*  the largest unsigned integer type that can fit entirely in a single CPU register for the current architecture and ABI.  */
  /*  SIZEOF_GEX_RMA_VALUE_T is a preprocess-time literal integer constant (i.e. not "sizeof()")indicating the size of this type in bytes */
  // Bug 3953: Our implementation guarantees a 64-bit gex_RMA_Value_t, although this is not currently required by the GASNet spec
typedef uint64_t gex_RMA_Value_t;
#define SIZEOF_GEX_RMA_VALUE_T  8

#ifndef _GEX_MEMVEC_T
#define _GEX_MEMVEC_T
  typedef struct {
    void  *gex_addr;  // TODO-EX: gex_Addr_t
    size_t gex_len;
  } gex_Memvec_t;
#endif

#define _GEX_MAKE_DT_ENUM(name) gasneti_dt_idx_##name = (_GEX_DT_##name)
typedef enum {
  _GEX_MAKE_DT_ENUM(I32),
  _GEX_MAKE_DT_ENUM(U32),
  _GEX_MAKE_DT_ENUM(I64),
  _GEX_MAKE_DT_ENUM(U64),
  _GEX_MAKE_DT_ENUM(FLT),
  _GEX_MAKE_DT_ENUM(DBL),
  _GEX_MAKE_DT_ENUM(USER)
} gasneti_dt_idx_t;
#undef _GEX_MAKE_DT_ENUM

#define _GEX_MAKE_OP_ENUM(name) gasneti_op_idx_##name = (_GEX_OP_##name)
typedef enum {
  _GEX_MAKE_OP_ENUM(AND),
  _GEX_MAKE_OP_ENUM(OR),
  _GEX_MAKE_OP_ENUM(XOR),
  _GEX_MAKE_OP_ENUM(ADD),
  _GEX_MAKE_OP_ENUM(SUB),
  _GEX_MAKE_OP_ENUM(MULT),
  _GEX_MAKE_OP_ENUM(MIN),
  _GEX_MAKE_OP_ENUM(MAX),
  _GEX_MAKE_OP_ENUM(INC),
  _GEX_MAKE_OP_ENUM(DEC),
  _GEX_MAKE_OP_ENUM(FAND),
  _GEX_MAKE_OP_ENUM(FOR),
  _GEX_MAKE_OP_ENUM(FXOR),
  _GEX_MAKE_OP_ENUM(FADD),
  _GEX_MAKE_OP_ENUM(FSUB),
  _GEX_MAKE_OP_ENUM(FMULT),
  _GEX_MAKE_OP_ENUM(FMIN),
  _GEX_MAKE_OP_ENUM(FMAX),
  _GEX_MAKE_OP_ENUM(FINC),
  _GEX_MAKE_OP_ENUM(FDEC),
  _GEX_MAKE_OP_ENUM(SET),
  _GEX_MAKE_OP_ENUM(GET),
  _GEX_MAKE_OP_ENUM(SWAP),
  _GEX_MAKE_OP_ENUM(FCAS),
  _GEX_MAKE_OP_ENUM(CAS),
  _GEX_MAKE_OP_ENUM(USER),
  _GEX_MAKE_OP_ENUM(USER_NC)
} gasneti_op_idx_t;
#undef _GEX_MAKE_OP_ENUM

/* ------------------------------------------------------------------------------------ */
// Error checking (or pass-through) for AM payload queries

#if GASNET_DEBUG
  extern size_t gex_AM_MaxRequestMedium(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
  extern size_t gex_AM_MaxReplyMedium(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
  extern size_t gex_AM_MaxRequestLong(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
  extern size_t gex_AM_MaxReplyLong(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
  extern size_t gex_Token_MaxReplyMedium(
           gex_Token_t _token,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
  extern size_t gex_Token_MaxReplyLong(
           gex_Token_t _token,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags,
           unsigned int _nargs);
#else
  #define gex_AM_MaxRequestMedium gasnetc_AM_MaxRequestMedium
  #define gex_AM_MaxReplyMedium   gasnetc_AM_MaxReplyMedium
  #define gex_AM_MaxRequestLong   gasnetc_AM_MaxRequestLong
  #define gex_AM_MaxReplyLong     gasnetc_AM_MaxReplyLong
  #define gex_Token_MaxReplyMedium   gasnetc_Token_MaxReplyMedium
  #define gex_Token_MaxReplyLong     gasnetc_Token_MaxReplyLong
#endif

/* ------------------------------------------------------------------------------------ */
/* Active Message Source Descriptor */

struct gasneti_srcdesc_s;
typedef struct gasneti_srcdesc_s *gex_AM_SrcDesc_t;
#define GEX_AM_SRCDESC_NO_OP ((gex_AM_SrcDesc_t)(uintptr_t)0)

#ifndef _GASNETI_AM_SRCDESC_T
  typedef struct gasneti_AM_SrcDesc {
  #if GASNET_DEBUG
    #define GASNETI_AM_SRCDESC_MAGIC       GASNETI_MAKE_MAGIC('A','S','D','t')
    #define GASNETI_AM_SRCDESC_BAD_MAGIC   GASNETI_MAKE_BAD_MAGIC('A','S','D','t')
    gasneti_magic_t      _magic;
    int                  _isreq;
    int                  _category; // true type: gasneti_category_t
  #endif
    void *               _addr;
    size_t               _size;
    // TODO: Want to omit _thread field when unused, but GASNETI_THREADINFO_OPT not defined until much later
    gasnet_threadinfo_t  _thread;
    void *               _tofree; // passed to gasneti_free() when sd reset
    void *               _gex_buf; // gasnet-owned buffer, if any
    union {
      struct {
        gex_TM_t             _tm;
        gex_Rank_t           _rank;
      }                    _request;
      struct {
        gex_Token_t          _token;
      }                    _reply;
    }                    _dest; // (tm,rank) or token, as passed to Prepare
    void *               _dest_addr; // Long only
    void *               _void_p; // PSHM and conduit-independent pointer
    gex_Event_t *        _lc_opt;
    gex_Flags_t          _flags;
    int                  _nargs;
    int                  _is_nbrhd;
  #if GASNET_PSHM
    struct {
      gex_Rank_t           _pshmrank; // should be gasneti_pshm_rank_t
      gex_Rank_t           _jobrank;
      int                  _loopback;
    }                    _pshm;
  #endif
  #ifdef GASNETI_AM_SRCDESC_EXTRA
    GASNETI_AM_SRCDESC_EXTRA
  #endif
  } *gasneti_AM_SrcDesc_t;
  #if GASNET_DEBUG
    extern gasneti_AM_SrcDesc_t gasneti_import_srcdesc(gex_AM_SrcDesc_t _srcdesc);
    extern gex_AM_SrcDesc_t gasneti_export_srcdesc(gasneti_AM_SrcDesc_t _real_srcdesc);
  #else
    #define gasneti_import_srcdesc(x) ((gasneti_AM_SrcDesc_t)(x))
    #define gasneti_export_srcdesc(x) ((gex_AM_SrcDesc_t)(x))
  #endif
  #define gex_AM_SrcDescAddr(sd)               ((void*)gasneti_import_srcdesc(sd)->_addr)
  #define gex_AM_SrcDescSize(sd)               ((size_t)gasneti_import_srcdesc(sd)->_size)
#endif


/* ------------------------------------------------------------------------------------ */
/* internal flags (others in gasnet_fwd.h) */

#if defined(_IN_GASNET_INTERNAL_H)
  #define GASNETI_FLAG_LC_OPT_IN             (1U << 31)
  #define GASNETI_FLAG_COLL_SUBORDINATE      (1U << 30)
#endif

#define GASNETI_FLAG_INIT_LEGACY           (1U << 31)

/* ------------------------------------------------------------------------------------ */

extern void (*gasnet_client_attach_hook)(void *, uintptr_t);

/* ------------------------------------------------------------------------------------ */

/* Main core header */
#include <gasnet_core.h>

/* Main extended header */
#include <gasnet_extended.h>

#if GASNETI_THREADINFO_OPT
  #define GASNETI_TIOPT_CONFIG tiopt
#else
  #define GASNETI_TIOPT_CONFIG notiopt
#endif

/* ------------------------------------------------------------------------------------ */

#if !defined(GASNET_NULL_ARGV_OK) || \
    (defined(GASNET_NULL_ARGV_OK) && GASNET_NULL_ARGV_OK != 0 && GASNET_NULL_ARGV_OK != 1)
  /*  defined to be 1 if gasnet_init(NULL,NULL) is supported. defined to 0 otherwise */
  #error GASNet core failed to define GASNET_NULL_ARGV_OK to 0 or 1
#endif

#ifndef GASNET_BEGIN_FUNCTION
  #error GASNet extended API failed to define GASNET_BEGIN_FUNCTION
#endif

#ifndef GEX_HSL_INITIALIZER
  #error GASNet core failed to define GEX_HSL_INITIALIZER
#endif

#ifndef GASNET_BLOCKUNTIL
  #error GASNet core failed to define GASNET_BLOCKUNTIL
#endif

#ifndef SIZEOF_GEX_RMA_VALUE_T
  #error GASNet failed to define SIZEOF_GEX_RMA_VALUE_T
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
             "SPEC=" _STRINGIFY(GEX_SPEC_VERSION_MAJOR) "."               \
             _STRINGIFY(GEX_SPEC_VERSION_MINOR) ","                       \
             "CONDUIT=" GASNET_CONDUIT_NAME_STR "("                       \
             GASNET_CORE_NAME_STR "-" GASNET_CORE_VERSION_STR "/"         \
             GASNET_EXTENDED_NAME_STR "-" GASNET_EXTENDED_VERSION_STR "),"\
             "THREADMODEL=" _STRINGIFY(GASNETI_THREAD_MODEL) ","          \
             "SEGMENT=" _STRINGIFY(GASNETI_SEGMENT_CONFIG) ","            \
             "PTR=" _STRINGIFY(GASNETI_PTR_CONFIG) ","                    \
             "CACHE_LINE_BYTES=" _STRINGIFY(GASNETI_CACHE_LINE_BYTES) "," \
             _STRINGIFY(GASNETI_ALIGN_CONFIG) ","                         \
             _STRINGIFY(GASNETI_PSHM_CONFIG) ","                          \
             _STRINGIFY(GASNETI_DEBUG_CONFIG) ","                         \
             _STRINGIFY(GASNETI_TRACE_CONFIG) ","                         \
             _STRINGIFY(GASNETI_STATS_CONFIG) ","                         \
             _STRINGIFY(GASNETI_MALLOC_CONFIG) ","                        \
             _STRINGIFY(GASNETI_SRCLINES_CONFIG) ","                      \
             _STRINGIFY(GASNETI_TIMER_CONFIG) ","                         \
             _STRINGIFY(GASNETI_MEMBAR_CONFIG) ","                        \
             _STRINGIFY(GASNETI_ATOMIC_CONFIG) ","                        \
             _STRINGIFY(GASNETI_ATOMIC32_CONFIG) ","                      \
             _STRINGIFY(GASNETI_ATOMIC64_CONFIG) ","                      \
             _STRINGIFY(GASNETI_TIOPT_CONFIG)                             \
             GASNETC_BUG1389_CONFIG_INFO                                  \
             GASNETC_EXTRA_CONFIG_INFO                                    \
             GASNETE_EXTRA_CONFIG_INFO                                    
#endif

/* ensure that the client links the correct library configuration
 * all objects in a given executable (client and library) must agree on all
 * of the following configuration settings, otherwise MANY things break,
 * often in very subtle and confusing ways (eg GASNet mutexes, threadinfo, etc.)
 * DO NOT REMOVE ANYTHING FROM THIS LIST!!!!
 */
#define GASNETI_LINKCONFIG_IDIOTCHECK(name) _CONCAT(gasneti_linkconfig_idiotcheck_,name)
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MALLOC_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIOPT_CONFIG);
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(HIDDEN_AM_CONCUR_,GASNET_HIDDEN_AM_CONCURRENCY_LEVEL));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CACHE_LINE_BYTES_,GASNETI_CACHE_LINE_BYTES));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME));
extern int GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME));

static int *gasneti_linkconfig_idiotcheck(void);
#if !PLATFORM_COMPILER_TINY /* avoid a tinyc bug */
  #define GASNETI_IDIOTCHECK_RECURSIVE_REFERENCE 1
  static int *(*_gasneti_linkconfig_idiotcheck)(void) = &gasneti_linkconfig_idiotcheck;
#endif
GASNETI_USED
static int *gasneti_linkconfig_idiotcheck(void) {
  static int _val;
  _val +=
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MAJOR_,GASNET_RELEASE_VERSION_MAJOR))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_MINOR_,GASNET_RELEASE_VERSION_MINOR))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(RELEASE_PATCH_,GASNET_RELEASE_VERSION_PATCH))
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_THREAD_MODEL)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SEGMENT_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_DEBUG_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TRACE_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_STATS_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MALLOC_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_SRCLINES_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ALIGN_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PSHM_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_PTR_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIMER_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_MEMBAR_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC32_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_ATOMIC64_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(GASNETI_TIOPT_CONFIG)
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(HIDDEN_AM_CONCUR_,GASNET_HIDDEN_AM_CONCURRENCY_LEVEL))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CACHE_LINE_BYTES_,GASNETI_CACHE_LINE_BYTES))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(CORE_,GASNET_CORE_NAME))
        + GASNETI_LINKCONFIG_IDIOTCHECK(_CONCAT(EXTENDED_,GASNET_EXTENDED_NAME))
        ;
  #if GASNETI_IDIOTCHECK_RECURSIVE_REFERENCE
  if (_gasneti_linkconfig_idiotcheck == &gasneti_linkconfig_idiotcheck)
    _val += *(*_gasneti_linkconfig_idiotcheck)();
  #endif
  return &_val;
}

#if defined(GASNET_DEBUG) && (defined(__OPTIMIZE__) || defined(NDEBUG))
    #error Tried to compile GASNet client code with optimization enabled but also GASNET_DEBUG (which seriously hurts performance). Reconfigure/rebuild GASNet without --enable-debug
#endif

/* ------------------------------------------------------------------------------------ */

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#undef _IN_GASNETEX_H
#endif

/* ------------------------------------------------------------------------------------ */
// Separate protection for definitions visible to GASNet-1 clients and to
// internal code (to provide corresponding thunks), but *not* to EX clients.
// This allows gasnet.h to be included either before or after gasnetex.h.
#if !defined(_GASNETEX_LEGACY_H) && (defined(_IN_GASNET_INTERNAL_H) || defined(_INCLUDED_GASNET_H))
  #define _GASNETEX_LEGACY_H

  // GASNet-1 version of gex_AM_Entry_t
  typedef struct {
    gex_AM_Index_t index; /*  == 0 for don't care  */
    void (*fnptr)();
  } gasnet_handlerentry_t;
#endif

/* ------------------------------------------------------------------------------------ */
/* intentionally expanded on every include */
#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_GASNET_INTERNAL_IDIOTCHECK)
  #define _GASNET_INTERNAL_IDIOTCHECK
  #undef gex_Client_Init
  #define gex_Client_Init gasneti_idiotcheck_gasnet_internal_dot_h_header_prohibited
#endif
