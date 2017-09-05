/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_internal.h $
 * Description: AMUDP internal header file
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef _AMUDP_INTERNAL_H
#define _AMUDP_INTERNAL_H

#include <portable_inttypes.h>
#undef _PORTABLE_PLATFORM_H
#include <amudp_portable_platform.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_GASNET_TOOLS 
  #ifndef GASNETT_LITE_MODE
  #define GASNETT_LITE_MODE /* use lite mode, to preserve AMUDP's threading neutrality */
  #endif
  #include <gasnet_tools.h> /* must precede internal assert defs */
  #define AMUDP_FORMAT_PRINTF GASNETT_FORMAT_PRINTF
#elif defined (__GNUC__) || defined (__attribute__)
  #define AMUDP_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) \
            __attribute__((__format__ (__printf__, fmtarg, firstvararg))) declarator
#else
  #define AMUDP_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) declarator
#endif
#include <amudp.h>
#include "sockutil.h" /* for SPMD TCP stuff */

AMUDP_BEGIN_EXTERNC

/* AMUDP system configuration parameters */
#ifndef DISABLE_STDSOCKET_REDIRECT
#define DISABLE_STDSOCKET_REDIRECT  0   /* disable redirection of slave stdin/stdout/stderr to master */
#endif
#ifndef USE_SOCKET_RECVBUFFER_GROW
#define USE_SOCKET_RECVBUFFER_GROW  1   /* grow RCVBUF on UDP sockets */
#endif
#ifndef USE_SOCKET_SENDBUFFER_GROW
#define USE_SOCKET_SENDBUFFER_GROW  1   /* grow SNDBUF on UDP sockets */
#endif
#ifndef AMUDP_SOCKETBUFFER_MAX
#define AMUDP_SOCKETBUFFER_MAX  4194304   /* socket *BUF max to never exceed: 4 MB (huge) */
#endif
#ifndef AMUDP_EXTRA_CHECKSUM
#define AMUDP_EXTRA_CHECKSUM 0 /* add extra checksums to each message to detect buggy IP */
#endif

#define AMUDP_PROCID_NEXT -1  /* Use next unallocated procid */
#define AMUDP_PROCID_ALLOC -2 /* Allocate and return next procis, but do not bootstrap */

#ifndef AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC
#define AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC  100000  /* usec until first retransmit */
#endif
#ifndef AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER
#define AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER     2  /* timeout exponential backoff factor */
#endif
#ifndef AMUDP_MAX_REQUESTTIMEOUT_MICROSEC
#define AMUDP_MAX_REQUESTTIMEOUT_MICROSEC    30000000  /* max timeout before considered undeliverable */
#endif
#define AMUDP_TIMEOUT_INFINITE ((uint32_t)-1)
extern uint32_t AMUDP_MaxRequestTimeout_us;
extern uint32_t AMUDP_InitialRequestTimeout_us;
extern uint32_t AMUDP_RequestTimeoutBackoff;
extern void AMUDP_InitRetryCache();

#ifndef AMUDP_TIMEOUTS_CHECKED_EACH_POLL
#define AMUDP_TIMEOUTS_CHECKED_EACH_POLL            1  /* number of timeout values we check upon each poll */
#endif
#ifndef AMUDP_MAX_RECVMSGS_PER_POLL
#define AMUDP_MAX_RECVMSGS_PER_POLL                10  /* max number of waiting messages serviced per poll (0 for unlimited) */
#endif                                                 /* we actually service up to MAX(AMUDP_MAX_RECVMSGS_PER_POLL, network_depth)
                                                          to prevent unnecessary retransmits (where the awaited reply is sitting in recv buffer) */
#if !defined(USE_CLEAR_UNUSED_SPACE) && AMUDP_DEBUG
#define USE_CLEAR_UNUSED_SPACE 1  /* clear any padding sent in AMs, to prevent valgrind warnings */
#endif

#ifndef AMUDP_INITIAL_NUMENDPOINTS
#define AMUDP_INITIAL_NUMENDPOINTS 1    /* initial size of bundle endpoint table */
#endif

#ifndef AMUDP_DEFAULT_NETWORKDEPTH
#define AMUDP_DEFAULT_NETWORKDEPTH 128    /* default depth if none specified */
#endif

/* AMUDP-SPMD system configuration parameters */
#ifndef USE_NUMERIC_MASTER_ADDR
#define USE_NUMERIC_MASTER_ADDR     0   /* pass a numeric IP on slave command line */
#endif
#ifndef USE_COORD_KEEPALIVE
#define USE_COORD_KEEPALIVE         1   /* set SO_KEEPALIVE on TCP coord sockets */
#endif
#ifndef ABORT_JOB_ON_NODE_FAILURE
#define ABORT_JOB_ON_NODE_FAILURE   1   /* kill everyone if any slave drops the TCP coord */
#endif
#ifndef USE_BLOCKING_SPMD_BARRIER
#define USE_BLOCKING_SPMD_BARRIER   1   /* use blocking AM calls in SPMDBarrier() */
#endif

#if !defined(USE_ASYNC_TCP_CONTROL) && \
   ( PLATFORM_OS_LINUX )   // NOT functional on WSL
  #define USE_ASYNC_TCP_CONTROL     1   /* use O_ASYNC and signals to stat TCP coord sockets, saves overhead on AMPoll */
#endif
#ifndef AMUDP_SIGIO
#define AMUDP_SIGIO                39   /* signal used for async IO operations */
#endif                                  /* avoid SIGIO to prevent conflicts with application using library */


#ifndef AMUDP_DEBUG_VERBOSE
  #if GASNET_DEBUG_VERBOSE
    #define AMUDP_DEBUG_VERBOSE       1
  #else
    #define AMUDP_DEBUG_VERBOSE       0
  #endif
#endif

#undef _STRINGIFY
#define _STRINGIFY AMUDP_STRINGIFY

#undef _CONCAT
#define _CONCAT    AMUDP_CONCAT

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif
#ifndef MAX
#define MAX(x,y)  ((x)>(y)?(x):(y))
#endif
#if defined(__GNUC__) /* try to get the function name from GCC */
  #define AMUDP_CURR_FUNCTION __PRETTY_FUNCTION__
#elif __cplusplus >= 201103L || __STDC_VERSION__ >= 199901
  #define AMUDP_CURR_FUNCTION __func__
#else
  #define AMUDP_CURR_FUNCTION ""
#endif

/* alignment macros */
#define AMUDP_POWEROFTWO(P)    (((P)&((P)-1)) == 0)

#define AMUDP_ALIGNDOWN(p,P)    (AMUDP_assert(AMUDP_POWEROFTWO(P)), \
                                   ((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define AMUDP_ALIGNUP(p,P)     (AMUDP_ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)),P))

/* these macros return the value of the expression given, but pass on
   a hint that you expect the value to be true or false.
   Use them to wrap the conditional expression in an if stmt when
   you have strong reason to believe the branch will frequently go
   in one direction and the branch is a bottleneck
 */
#ifndef PREDICT_TRUE
  #if defined(GASNETT_PREDICT_TRUE)
   #define PREDICT_TRUE(exp)  GASNETT_PREDICT_TRUE(exp)
   #define PREDICT_FALSE(exp) GASNETT_PREDICT_FALSE(exp)
  #elif defined(__GNUC__) && __GNUC__ >= 3 && 0
    #define PREDICT_TRUE(exp)  __builtin_expect( (exp), 1 )
    #define PREDICT_FALSE(exp) __builtin_expect( (exp), 0 )
  #else
    #define PREDICT_TRUE(exp)  (exp)
    #define PREDICT_FALSE(exp) (exp)
  #endif
#endif

#ifndef if_pf
  /* if with branch prediction */
  #define if_pf(cond) if (PREDICT_FALSE(cond))
  #define if_pt(cond) if (PREDICT_TRUE(cond))
#endif

/* ------------------------------------------------------------------------------------ */
/* Internal types */

/* message flags:
 * 0-1: category (amudp_category_t)
 * 2:   request vs. reply 
 * 3-7: numargs
 * message instance:
 * 0-5:  sequence number
 * 6-15: instance number
 */
typedef unsigned char amudp_flag_t;

#define AMUDP_SEQNUM_BITS         6
#define AMUDP_SEQNUM_MASK         0x3F
#define AMUDP_SEQNUM_INC(v)       ((uint8_t)( (((uint8_t)(v))+1) & AMUDP_SEQNUM_MASK))
#define AMUDP_MSG_SETFLAGS(pmsg, isreq, cat, numargs, seqnum, instance) do { \
   (pmsg)->flags = (amudp_flag_t) (                                          \
                   (((numargs) & 0x1F) << 3)                                 \
                 | (((isreq) & 0x1) << 2)                                    \
                 |  ((cat) & 0x3)                                            \
                 );                                                          \
   (pmsg)->_instance = (uint16_t) (                                          \
                   ((seqnum) & AMUDP_SEQNUM_MASK)                            \
                 | (((instance) & 0x3FF) << AMUDP_SEQNUM_BITS)               \
                 );                                                          \
  } while (0)
#define AMUDP_MSG_NUMARGS(pmsg)   ( ( ((unsigned char)(pmsg)->flags) >> 3 ) & 0x1F)
#define AMUDP_MSG_ISREQUEST(pmsg) (!!(((unsigned char)(pmsg)->flags) & 0x4))
#define AMUDP_MSG_CATEGORY(pmsg)  ((amudp_category_t)((pmsg)->flags & 0x3))
#define AMUDP_MSG_SEQNUM(pmsg)    (((uint8_t)(pmsg)->_instance) & AMUDP_SEQNUM_MASK)
#define AMUDP_MSG_INSTANCE(pmsg)  ((uint16_t)((pmsg)->_instance >> AMUDP_SEQNUM_BITS))

struct amudp_buf;

/* active message header & meta info fields */
typedef struct {
  #if AMUDP_EXTRA_CHECKSUM
    uint16_t chk1;  // these fields must come first in amudp_msg_t
    uint16_t chk2;
    uint32_t packetlen;
  #endif

  tag_t         tag;

  uint16_t      _instance; /* instance and seqnum */
  amudp_flag_t  flags;
  handler_t     handlerId;
 
  uint16_t      nBytes;
  uint8_t       systemMessageType;
  uint8_t       systemMessageArg;

  uintptr_t     destOffset;

  // uint8_t     data[]; /* args and data: up to (4*AMUDP_MAX_SHORT)+AMUDP_MAX_LONG */
  // this would be a flexible array member if C++ provided that feature.

} amudp_msg_t;

/* non-transmitted amudp buffer bookkeeping info
 */
typedef union {

  struct amudp_rx_status { // Status for receive buffers
    en_t sourceAddr;        // address of remote (16-byte)
    struct amudp_buf *next; // recv queue
    struct amudp_ep *dest;  /* ep_t of endpoint that received this message */
    amudp_node_t sourceId;  /* 0-based endpoint id of remote */
    int8_t handlerRunning;
    int8_t replyIssued;
  } rx;

  struct amudp_tx_status { // Status for transmit buffers
    /* Request tx fields */
    struct amudp_buf *next; // retransmit ring
    struct amudp_buf *prev;
    amudp_cputick_t timestamp; // request expiration, reply last retransmit
    #if AMUDP_COLLECT_LATENCY_STATS
      amudp_cputick_t firstSendTime; /* for statistical purposes only */
    #endif
    amudp_node_t destId;  /* 0-based endpoint id of remote */
    uint8_t retryCount; /* where we are in the retransmit backoff */
  } tx;

  uint64_t _pad[5];

} amudp_bufstatus_t;

/* active message buffer, including message and space for data payload */
typedef struct amudp_buf {

  amudp_bufstatus_t status;

  amudp_msg_t   msg;

} amudp_buf_t;

/* limits for msg (wire packet) and buffer (in-memory rep) */
#define AMUDP_MIN_MSG           (sizeof(amudp_msg_t))
#define AMUDP_MAX_SHORT_MSG     (AMUDP_MIN_MSG+(4*AMUDP_MAX_SHORT))
#define AMUDP_MAX_MSG           (AMUDP_MIN_MSG+(4*AMUDP_MAX_SHORT)+AMUDP_MAX_LONG)
#define AMUDP_MIN_BUFFER        (sizeof(amudp_buf_t))
#define AMUDP_MAX_SHORT_BUFFER  (AMUDP_MIN_BUFFER+(4*AMUDP_MAX_SHORT))
#define AMUDP_MAX_BUFFER        (AMUDP_MIN_BUFFER+(4*AMUDP_MAX_SHORT)+AMUDP_MAX_LONG)
#define MSGSZ_TO_BUFFERSZ(sz)   (offsetof(amudp_buf_t,msg)+(size_t)(sz))

/* message buffer descriptor - the minimal persistent state, kept to a minimum for scalability */
typedef struct {
  amudp_buf_t *buffer;  /* active buffer, NULL for not in use */
  uint8_t seqNum; /* seq number for next message to be sent/recv'd on this desc */
} amudp_bufdesc_t;

/* ------------------------------------------------------------------------------------ */
/* Buffer pools */
#ifndef AMUDP_BUFFER_STATS
#define AMUDP_BUFFER_STATS AMUDP_DEBUG_VERBOSE
#endif

struct amudp_bufferpool;

typedef union amudp_bufferheader {
  struct amudp_bufferpool *pool;
  union amudp_bufferheader *next;
  uint64_t _pad[1]; // maintain 8-byte alignment
} amudp_bufferheader_t;

typedef struct amudp_bufferpool {
  #if AMUDP_DEBUG
    uint64_t magic;
  #endif
  #if AMUDP_BUFFER_STATS
    struct {
      uint64_t alloc_curr;
      uint64_t alloc_peak;
      uint64_t alloc_total;
      uint64_t buffer_bytes;
    } stats;
  #endif
  amudp_bufferheader_t *free;
  size_t buffersz;
} amudp_bufferpool_t;

#define AMUDP_NUMBUFFERPOOLS 2

/* ------------------------------------------------------------------------------------ */
/* Endpoints */

typedef struct {
  en_t name;  /*  remote address */
  tag_t tag;  /*  remote tag */
  amudp_node_t id;/*  id in compressed table */
  char inuse; /*  entry in use */
} amudp_translation_t;

typedef struct {
  amudp_bufdesc_t* requestDesc; // on-demand alloc
  amudp_bufdesc_t* replyDesc;   // on-demand alloc
  tag_t     tag;          /* compacted from the translation table */
  en_t      remoteName;   /* compacted from the translation table */
  uint16_t  instanceHint; /* instance hint pointer for request buffer allocation */
} amudp_perproc_info_t;

/* Endpoint bundle object */
struct amudp_eb {
  struct amudp_ep **endpoints;   /* dynamically-grown array of endpoints in bundle */
  int     n_endpoints;           /* Number of EPs in the bundle */
  int     cursize;               /* size of the array */
  uint8_t event_mask;            /* Event Mask for blocking ops */
};

/* Endpoint object */
struct amudp_ep {
  en_t name;            /* Endpoint name */
  tag_t tag;            /* current tag */
  eb_t eb;              /* Bundle of endpoint */

  void *segAddr;        /* Start address of EP VM segment */
  uintptr_t segLength;  /* Length of EP VM segment    */

  amudp_translation_t *translation; /* translation table */
  amudp_node_t         translationsz;

  amudp_handler_fn_t  handler[AMUDP_MAX_NUMHANDLERS]; /* handler table */

  /* internal structures */

  SOCKET s; /* UDP socket */
  size_t socketRecvBufferSize; /* only used if USE_SOCKET_RECVBUFFER_GROW */
  int socketRecvBufferMaxedOut;

  amudp_node_t P;     /* the number of endpoints we communicate with - also number of translations currently in use */
  int depth; /* network depth, -1 until AM_SetExpectedResources is called */
  int PD; /* cached value of P * depth */
  int recvDepth; /* recv depth */
  int sendDepth; /* send depth: max outstandingRequests (to all peers) */

  int outstandingRequests; /* number of requests awaiting a reply, does NOT include loopback */
  amudp_buf_t *timeoutCheckPosn; /* current position of the timeout-checking circular walk */

  amudp_cputick_t replyEpoch; /* timestamp of the first non-loopback reply sent during the current AMPoll */

  amudp_perproc_info_t *perProcInfo;

  amudp_node_t idHint; /* hint of my loopback id */

  /* pools for dynamic allocation of buffers */
  amudp_bufferpool_t bufferPool[AMUDP_NUMBUFFERPOOLS];

  amudp_buf_t *rxHead; /* inbound messages queue */
  amudp_buf_t *rxTail;
  int rxCnt; // length of the queue

  AMUDP_preHandlerCallback_t preHandlerCallback; /* client hooks for statistical/debugging usage */
  AMUDP_postHandlerCallback_t postHandlerCallback;

  amudp_stats_t stats;  /* statistical collection */

};

/* ------------------------------------------------------------------------------------ */

AMUDP_FORMAT_PRINTF(AMUDP_Err,1,2,
extern int AMUDP_Err(const char *msg, ...));

AMUDP_FORMAT_PRINTF(AMUDP_Warn,1,2,
extern int AMUDP_Warn(const char *msg, ...));

AMUDP_FORMAT_PRINTF(AMUDP_Info,1,2,
extern int AMUDP_Info(const char *msg, ...));

// verbose debug messages (double-parens)
#if AMUDP_DEBUG_VERBOSE
  #define AMUDP_VERBOSE_INFO(args)    AMUDP_Info args
  #define AMUDP_DEBUG_INFO(args)      AMUDP_Info args
  #define AMUDP_DEBUG_WARN(args)      AMUDP_Warn args
  #define AMUDP_DEBUG_WARN_TH(msg)    AMUDP_Warn(msg)
#elif AMUDP_DEBUG
  #define AMUDP_VERBOSE_INFO(args)    ((void)0)
  #define AMUDP_DEBUG_INFO(args)      AMUDP_Info args
  #define AMUDP_DEBUG_WARN(args)      AMUDP_Warn args
  // throttled debug-only warning
  #define AMUDP_DEBUG_WARN_TH(msg) do {                                 \
    static uint64_t _cnt = 0;                                           \
    _cnt++;                                                             \
    if_pf (!(_cnt & (_cnt-1))) {                                        \
      AMUDP_Warn("%s (%" PRIu64 " occurences)",msg,_cnt);               \
    }                                                                   \
  } while (0)
#else
  #define AMUDP_VERBOSE_INFO(args)    ((void)0)
  #define AMUDP_DEBUG_INFO(args)      ((void)0)
  #define AMUDP_DEBUG_WARN(args)      ((void)0)
  #define AMUDP_DEBUG_WARN_TH(msg)    ((void)0)
#endif

#ifdef GASNETT_NORETURN
GASNETT_NORETURN
#endif
AMUDP_FORMAT_PRINTF(AMUDP_FatalErr,1,2,
extern void AMUDP_FatalErr(const char *msg, ...));
#ifdef GASNETT_NORETURNP
GASNETT_NORETURNP(AMUDP_FatalErr)
#endif

/* memory allocation */
static void *_AMUDP_malloc(size_t sz, const char *curloc) {
  void *ret = malloc(sz);
  if_pf(!ret) AMUDP_FatalErr("Failed to malloc(%" PRIuPTR ") at %s", (uintptr_t)sz, curloc);
  return ret;
}
static void *_AMUDP_calloc(size_t N, size_t S, const char *curloc) {
  void *ret = calloc(N,S);
  if_pf(!ret) AMUDP_FatalErr("Failed to calloc(%" PRIuPTR ",%" PRIuPTR ") at %s", (uintptr_t)N, (uintptr_t)S, curloc);
  return ret;
}
static void *_AMUDP_realloc(void *ptr, size_t S, const char *curloc) {
  void *ret = realloc(ptr,S);
  if_pf(!ret) AMUDP_FatalErr("Failed to realloc(%" PRIuPTR ") at %s", (uintptr_t)S, curloc);
  return ret;
}
static void _AMUDP_free(void *ptr, const char *curloc) {
  free(ptr);
}
#define AMUDP_curloc __FILE__ ":" _STRINGIFY(__LINE__)
#if AMUDP_DEBUG
  /* use the gasnet debug malloc functions if a debug libgasnet is linked */
  extern void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc);
  extern void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc);
  extern void *(*gasnett_debug_realloc_fn)(void *ptr, size_t sz, const char *curloc);
  extern void (*gasnett_debug_free_fn)(void *ptr, const char *curloc);
  extern void (*gasnett_debug_memcheck_fn)(void *ptr, const char *curloc);
  extern void (*gasnett_debug_memcheck_one_fn)(const char *curloc);
  extern void (*gasnett_debug_memcheck_all_fn)(const char *curloc);
  #define AMUDP_malloc(sz)                                   \
    ( (PREDICT_FALSE(gasnett_debug_malloc_fn==NULL) ?        \
        gasnett_debug_malloc_fn = &_AMUDP_malloc : 0),       \
      (*gasnett_debug_malloc_fn)(sz, AMUDP_curloc))
  #define AMUDP_calloc(N,S)                                  \
    ( (PREDICT_FALSE(gasnett_debug_calloc_fn==NULL) ?        \
        gasnett_debug_calloc_fn = &_AMUDP_calloc : 0),       \
      (*gasnett_debug_calloc_fn)((N),(S), AMUDP_curloc))
  #define AMUDP_realloc(ptr,S)                               \
    ( (PREDICT_FALSE(gasnett_debug_realloc_fn==NULL) ?       \
        gasnett_debug_realloc_fn = &_AMUDP_realloc : 0),     \
      (*gasnett_debug_realloc_fn)((ptr),(S), AMUDP_curloc))
  #define AMUDP_free(ptr)                                    \
    ( (PREDICT_FALSE(gasnett_debug_free_fn==NULL) ?          \
        gasnett_debug_free_fn = &_AMUDP_free : 0),           \
      (*gasnett_debug_free_fn)(ptr, AMUDP_curloc))
  #define AMUDP_memcheck(ptr) do {                          \
    AMUDP_assert(ptr);                                      \
    if (gasnett_debug_memcheck_fn)                          \
       (*gasnett_debug_memcheck_fn)(ptr, AMUDP_curloc);     \
  } while (0)
  #define AMUDP_memcheck_one() do {                         \
    if (gasnett_debug_memcheck_one_fn)                      \
       (*gasnett_debug_memcheck_one_fn)(AMUDP_curloc);      \
  } while (0)
  #define AMUDP_memcheck_all() do {                         \
    if (gasnett_debug_memcheck_all_fn)                      \
       (*gasnett_debug_memcheck_all_fn)(AMUDP_curloc);      \
  } while (0)

  #undef malloc
  #define malloc(x)   ERROR_use_AMUDP_malloc
  #undef calloc
  #define calloc(n,s) ERROR_use_AMUDP_calloc
  #undef realloc
  #define realloc(n,s) ERROR_use_AMUDP_realloc
  #undef free
  #define free(x)     ERROR_use_AMUDP_free
#else
  #define AMUDP_malloc(sz)     _AMUDP_malloc((sz),AMUDP_curloc)
  #define AMUDP_calloc(N,S)    _AMUDP_calloc((N),(S),AMUDP_curloc)
  #define AMUDP_realloc(ptr,S) _AMUDP_realloc((ptr),(S),AMUDP_curloc)
  #define AMUDP_free(ptr)      _AMUDP_free(ptr,AMUDP_curloc)
  #define AMUDP_memcheck(ptr)   ((void)0)
  #define AMUDP_memcheck_one()  ((void)0)
  #define AMUDP_memcheck_all()  ((void)0)
#endif

/*------------------------------------------------------------------------------------
 * Error reporting
 *------------------------------------------------------------------------------------ */
#if PLATFORM_COMPILER_MICROSOFT
  #pragma warning(disable: 4127)
#endif
static const char *AMUDP_ErrorName(int errval) {
  switch (errval) {
    case AM_ERR_NOT_INIT: return "AM_ERR_NOT_INIT";      
    case AM_ERR_BAD_ARG:  return "AM_ERR_BAD_ARG";       
    case AM_ERR_RESOURCE: return "AM_ERR_RESOURCE";      
    case AM_ERR_NOT_SENT: return "AM_ERR_NOT_SENT";      
    case AM_ERR_IN_USE:   return "AM_ERR_IN_USE";       
    default: return "*unknown*";
  }
}
static const char *AMUDP_ErrorDesc(int errval) {
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
#define AMUDP_RETURN_ERR(type) do {                                        \
    if (AMUDP_VerboseErrors) {                                             \
      fprintf(stderr, "AMUDP %s returning an error code: AM_ERR_%s (%s)\n" \
        "  at %s:%i\n"                                                     \
        , AMUDP_CURR_FUNCTION                                              \
        , #type, AMUDP_ErrorDesc(AM_ERR_##type), __FILE__, __LINE__);      \
      fflush(stderr);                                                      \
    }                                                                      \
    return AM_ERR_ ## type;                                                \
  } while (0)
#define AMUDP_RETURN_ERRF(type, fromfn) do {                                   \
    if (AMUDP_VerboseErrors) {                                                 \
      fprintf(stderr, "AMUDP %s returning an error code: AM_ERR_%s (%s)\n"     \
        "  from function %s\n"                                                 \
        "  at %s:%i\n"                                                         \
        , AMUDP_CURR_FUNCTION                                                  \
        , #fromfn, #type, AMUDP_ErrorDesc(AM_ERR_##type), __FILE__, __LINE__); \
      fflush(stderr);                                                          \
    }                                                                          \
    return AM_ERR_ ## type;                                                    \
  } while (0)
#define AMUDP_RETURN_ERRFR(type, fromfn, reason) do {                                  \
    if (AMUDP_VerboseErrors) {                                                         \
      fprintf(stderr, "AMUDP %s returning an error code: AM_ERR_%s (%s)\n"             \
        "  from function %s\n"                                                         \
        "  at %s:%i\n"                                                                 \
        "  reason: %s\n"                                                               \
        , AMUDP_CURR_FUNCTION                                                          \
        , #type, AMUDP_ErrorDesc(AM_ERR_##type), #fromfn, __FILE__, __LINE__, reason); \
      fflush(stderr);                                                                  \
    }                                                                                  \
    return AM_ERR_ ## type;                                                            \
  } while (0)
/* return a possible error */
#define AMUDP_RETURN(val) do {                                           \
  if_pf (AMUDP_VerboseErrors && val != AM_OK) {                          \
    fprintf(stderr, "AMUDP %s returning an error code: %s (%s)\n"        \
      "  at %s:%i\n"                                                     \
      , AMUDP_CURR_FUNCTION                                              \
      , AMUDP_ErrorName(val), AMUDP_ErrorDesc(val), __FILE__, __LINE__); \
    fflush(stderr);                                                      \
  }                                                                      \
  return val;                                                            \
} while (0)

#ifndef AMUDP_ENABLE_ERRCHECKS
  #if GASNETI_ENABLE_ERRCHECKS
    #define AMUDP_ENABLE_ERRCHECKS 1
  #else
    #define AMUDP_ENABLE_ERRCHECKS 0
  #endif
#endif

#if AMUDP_COLLECT_STATS
  #define AMUDP_STATS(stmt) stmt
#else
  #define AMUDP_STATS(stmt) 
#endif

#if AMUDP_DEBUG || AMUDP_ENABLE_ERRCHECKS
  #define AMUDP_CHECK_ERR(errcond, type) \
    do { if_pf (errcond) AMUDP_RETURN_ERR(type); } while(0)
  #define AMUDP_CHECK_ERRF(errcond, type, fromfn) \
    do { if_pf (errcond) AMUDP_RETURN_ERRF(type, fromfn); } while(0)
  #define AMUDP_CHECK_ERRR(errcond, type, reason) \
    do { if_pf (errcond) AMUDP_RETURN_ERRR(type, reason); } while(0)
  #define AMUDP_CHECK_ERRFR(errcond, type, fromfn, reason) \
    do { if_pf (errcond) AMUDP_RETURN_ERRFR(type, fromfn, reason); } while(0)
  #define AMUDP_CHECK_ERRFRC(errcond, type, fromfn, reason, cleanup) \
    do { if_pf (errcond) { { cleanup; } AMUDP_RETURN_ERRFR(type, fromfn, reason); } } while(0)
#else
  #define AMUDP_CHECK_ERR(errcond, type)                    ((void)0)
  #define AMUDP_CHECK_ERRF(errcond, type, fromfn)           ((void)0)
  #define AMUDP_CHECK_ERRR(errcond, type, reason)           ((void)0)
  #define AMUDP_CHECK_ERRFR(errcond, type, fromfn, reason)  ((void)0)
  #define AMUDP_CHECK_ERRFRC(errcond, type, fromfn, reason, cleanup) ((void)0)
#endif

#include <assert.h>
#undef assert
#define assert(x) ERROR_use_AMUDP_assert
#if AMUDP_NDEBUG
  #define AMUDP_assert(expr) ((void)0)
#else
  #define AMUDP_assert(expr)                                \
    (PREDICT_TRUE(expr) ? (void)0 :                         \
      AMUDP_FatalErr("Assertion failure at %s %s:%i: %s\n", \
        AMUDP_CURR_FUNCTION, __FILE__, __LINE__, #expr))
#endif

#define enEqual(en1,en2)                    \
    ((en1).sin_port == (en2).sin_port       \
  && (en1).sin_addr.s_addr == (en2).sin_addr.s_addr)

//------------------------------------------------------------------------------------
// global data
extern int AMUDP_numBundles;
extern eb_t AMUDP_bundles[AMUDP_MAX_BUNDLES];

extern double AMUDP_FaultInjectionRate;
extern double AMUDP_FaultInjectionEnabled;

extern int amudp_Initialized;
#define AMUDP_CHECKINIT() AMUDP_CHECK_ERR((!amudp_Initialized),NOT_INIT)
/* ------------------------------------------------------------------------------------ */
// Buffer descriptor accessors
static inline amudp_bufdesc_t *AMUDP_get_desc(ep_t ep, amudp_node_t proc, int inst, int isrequest, int allocate) {
  AMUDP_assert(ep);
  AMUDP_assert(ep->perProcInfo);
  AMUDP_assert(ep->depth > 0);
  AMUDP_assert(proc <= ep->P);
  AMUDP_assert(inst <= ep->depth);
  amudp_perproc_info_t * const pinfo = &ep->perProcInfo[proc];
  amudp_bufdesc_t ** const pdesc = (isrequest ? &pinfo->requestDesc : &pinfo->replyDesc);
  if_pf (allocate && !*pdesc) { // first touch allocate
    *pdesc = (amudp_bufdesc_t *)AMUDP_calloc(ep->depth, sizeof(amudp_bufdesc_t));
  }
  AMUDP_assert(*pdesc);
  return *pdesc + inst;
}
#define GET_REQ_DESC(ep, proc, inst)       AMUDP_get_desc(ep, proc, inst, 1, 0)
#define GET_REP_DESC(ep, proc, inst)       AMUDP_get_desc(ep, proc, inst, 0, 0)
#define GET_REQ_DESC_ALLOC(ep, proc, inst) AMUDP_get_desc(ep, proc, inst, 1, 1)
#define GET_REP_DESC_ALLOC(ep, proc, inst) AMUDP_get_desc(ep, proc, inst, 0, 1)
/* ------------------------------------------------------------------------------------ */
/* accessors for packet args, data and length
 * the only complication here is we want data to be double-word aligned, so we may add
 * an extra unused 4-byte argument to make sure the data lands on a double-word boundary
 */
#define HEADER_EVEN_WORDLENGTH ( (sizeof(amudp_msg_t) & 0x7) == 0 ? 1 : 0)
#define ACTUAL_NUM_ARGS(args) ( ((args) & 0x1) ?                    \
                                (args) +  HEADER_EVEN_WORDLENGTH :  \
                                (args) + !HEADER_EVEN_WORDLENGTH    )

#define COMPUTE_MSG_SZ(args, nbytes) \
  ( sizeof(amudp_msg_t) + 4*ACTUAL_NUM_ARGS(args) + (nbytes) )
#define GET_MSG_SZ(pmsg) \
  COMPUTE_MSG_SZ(AMUDP_MSG_NUMARGS(pmsg), (pmsg)->nBytes)
#define GET_MSG_DATA(pmsg) \
  ((uint8_t*)((pmsg)+1) + 4*ACTUAL_NUM_ARGS(AMUDP_MSG_NUMARGS(pmsg)))
#define GET_MSG_ARGS(pmsg) ( (pmsg)->_instance, /* typecheck pmsg */ \
  ((uint32_t *)((pmsg)+1)) )
//------------------------------------------------------------------------------------
// global helper functions
extern int AMUDP_Block(eb_t eb); 
  /* block until receive buffer becomes non-empty
   * does not poll, but does handle SPMD control socket events
   */

// buffer management
extern amudp_buf_t *AMUDP_AcquireBuffer(ep_t ep, size_t sz);
extern void AMUDP_ReleaseBuffer(ep_t ep, amudp_buf_t *buf);

#if USE_SOCKET_RECVBUFFER_GROW
  extern int AMUDP_growSocketBufferSize(ep_t ep, int targetsize, int szparam, const char *paramname);
#endif

// debugging printouts
extern char *AMUDP_enStr(en_t en, char *buf);
extern char *AMUDP_tagStr(tag_t tag, char *buf);

extern amudp_handler_fn_t amudp_unused_handler;
extern void AMUDP_DefaultReturnedMsg_Handler(int status, op_t opcode, void *token);

//------------------------------------------------------------------------------------
/* SPMD control information that has to be shared */
extern SOCKET AMUDP_SPMDControlSocket; /* SPMD TCP control socket */
extern int AMUDP_SPMDHandleControlTraffic(int *controlMessagesServiced);
extern int AMUDP_SPMDSpawnRunning; /* true while spawn is active */
extern int AMUDP_SPMDRedirectStdsockets; /* true if stdin/stdout/stderr should be redirected */
extern int AMUDP_SPMDwakeupOnControlActivity; /* true if waitForEndpointActivity should return on control socket activity */
extern volatile int AMUDP_SPMDIsActiveControlSocket; 
//------------------------------------------------------------------------------------
/* AMUDP_IDENT() takes a unique identifier and a textual string and embeds the textual
   string in the executable file
 */
#define AMUDP_PRAGMA(x) _Pragma ( #x )
#define _AMUDP_IDENT(identName, identText)  \
  extern char volatile identName[];         \
  char volatile identName[] = identText;    \
  extern char *_##identName##_identfn(void) { return (char*)identName; } 
#if PLATFORM_COMPILER_CRAY && !PLATFORM_ARCH_X86_64 /* fouls up concatenation in ident string */
  #define AMUDP_IDENT(identName, identText) \
    AMUDP_PRAGMA(_CRI ident identText);     \
    _AMUDP_IDENT(identName, identText)
#elif PLATFORM_COMPILER_XLC
    /* #pragma comment(user,"text...") 
         or
       _Pragma ( "comment (user,\"text...\")" );
       are both supposed to work according to compiler docs, but both appear to be broken
     */
  #define AMUDP_IDENT(identName, identText)   \
    _AMUDP_IDENT(identName, identText)
#else
  #define AMUDP_IDENT _AMUDP_IDENT
#endif
//------------------------------------------------------------------------------------

// handler prototypes
typedef void (*AMUDP_HandlerShort)(void *token, ...);
typedef void (*AMUDP_HandlerMedium)(void *token, void *buf, int nbytes, ...);
typedef void (*AMUDP_HandlerLong)(void *token, void *buf, int nbytes, ...);
typedef void (*AMUDP_HandlerReturned)(int status, op_t opcode, void *token);


/* system message type field:
 *  low  4 bits are actual type
 *  high 4 bits are bulk transfer slot (all zero for non-bulk messages)
 * slot is only used if bulk xfer is spanned - more than one packet (arg > 0)
 */

typedef enum {
  amudp_system_user=0,      // not a system message
  amudp_system_autoreply,   // automatically generated reply
  amudp_system_returnedmessage, // arg is reason code, req/rep represents the type of message refused

  amudp_system_numtypes
} amudp_system_messagetype_t;


//------------------------------------------------------------------------------------
#ifndef AMUDP_ENV_PREFIX_STR // AMUDP_ENV_PREFIX_STR is the safest define, to avoid conflicting defns
  #ifdef AMUDP_ENV_PREFIX
    #define AMUDP_ENV_PREFIX_STR _STRINGIFY(AMUDP_ENV_PREFIX)
  #else
    #define AMUDP_ENV_PREFIX_STR "AMUDP"
  #endif
#endif

/* return the first environment variable matching one of:
    AMUDP_ENV_PREFIX_STR##_##basekey
    AMUDP##_##basekey
    basekey
   warn if more than one is set with different values
 */
extern char *AMUDP_getenv_prefixed(const char *basekey);
/* same with a default value and console reporting in verbose mode */
extern char *AMUDP_getenv_prefixed_withdefault(const char *basekey, const char *defaultval);

//------------------------------------------------------------------------------------
// socket support

#if USE_ASYNC_TCP_CONTROL
  #if AMUDP_DEBUG
   #define ASYNC_CHECK(enabled) do {                                             \
      int flags = fcntl(AMUDP_SPMDControlSocket, F_GETFL, 0);                    \
      if ((enabled && (flags & (O_ASYNC|O_NONBLOCK)) != (O_ASYNC|O_NONBLOCK)) || \
         (!enabled && (flags & (O_ASYNC|O_NONBLOCK)) != 0)) {                    \
        AMUDP_FatalErr("Failed to modify O_ASYNC|O_NONBLOCK flags in fcntl"      \
                   " - try disabling USE_ASYNC_TCP_CONTROL");                    \
      }                                                                          \
    } while (0)
  #else
   #define ASYNC_CHECK(enabled) ((void)0)
  #endif
  #define ASYNC_TCP_ENABLE() do {                                                       \
      if (fcntl(AMUDP_SPMDControlSocket, F_SETFL, O_ASYNC|O_NONBLOCK)) {                \
        perror("fcntl(F_SETFL, O_ASYNC|O_NONBLOCK)");                                   \
        AMUDP_FatalErr("Failed to fcntl(F_SETFL, O_ASYNC|O_NONBLOCK) on TCP control socket" \
                   " - try disabling USE_ASYNC_TCP_CONTROL");                           \
      } else ASYNC_CHECK(1);                                                            \
      if (inputWaiting(AMUDP_SPMDControlSocket,false)) /* check for arrived messages */ \
        AMUDP_SPMDIsActiveControlSocket = 1;                                            \
    } while(0)

  #define _ASYNC_TCP_DISABLE(ignoreerr)  do {                              \
      if (fcntl(AMUDP_SPMDControlSocket, F_SETFL, 0) && !ignoreerr) {      \
        perror("fcntl(F_SETFL, 0)");                                       \
        AMUDP_FatalErr("Failed to fcntl(F_SETFL, 0) on TCP control socket" \
                   " - try disabling USE_ASYNC_TCP_CONTROL");              \
      } else if (!ignoreerr) ASYNC_CHECK(0);                               \
    } while(0)
  #define ASYNC_TCP_DISABLE()            _ASYNC_TCP_DISABLE(0)
  #define ASYNC_TCP_DISABLE_IGNOREERR()  _ASYNC_TCP_DISABLE(1)
#else
  #define ASYNC_TCP_ENABLE()             ((void)0)
  #define ASYNC_TCP_DISABLE()            ((void)0)
  #define ASYNC_TCP_DISABLE_IGNOREERR()  ((void)0)
#endif
//------------------------------------------------------------------------------------
/* *** TIMING *** */
#if defined(HAVE_GASNET_TOOLS)
  #define getMicrosecondTimeStamp() (gasnett_ticks_to_us(gasnett_ticks_now()))
  #define getCPUTicks()             ((amudp_cputick_t)gasnett_ticks_now())
  #define ticks2us(ticks)           (gasnett_ticks_to_us((gasnett_tick_t)(ticks)))
  #define us2ticks(us) \
    ((amudp_cputick_t)(us) * \
     (amudp_cputick_t)(1000000000.0 / gasnett_ticks_to_us((gasnett_tick_t)1000000000)))
  #define tickspersec               us2ticks(1000000)  
#else
  static int64_t getMicrosecondTimeStamp(void) {
    int64_t retval;
    struct timeval tv;
    retry:
    if (gettimeofday(&tv, NULL))
      AMUDP_FatalErr("gettimeofday failed: %s",strerror(errno));
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
  #define getCPUTicks()             ((amudp_cputick_t)getMicrosecondTimeStamp())
  #define ticks2us(ticks)           (ticks)
  #define us2ticks(us)              ((amudp_cputick_t)(us))
  #define tickspersec               1000000
#endif
//------------------------------------------------------------------------------------
// checkpoint/restart
extern int AMUDP_SPMDRestartActive;

//------------------------------------------------------------------------------------

AMUDP_END_EXTERNC

#endif
