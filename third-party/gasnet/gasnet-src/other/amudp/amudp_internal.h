/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_internal.h $
 * Description: AMUDP internal header file
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef _AMUDP_INTERNAL_H
#define _AMUDP_INTERNAL_H

#include <amx_internal_fwd.h>

#include <amudp.h>

#if __cplusplus
#include "sockutil.h"
#else
#include "socket.h"
#endif

#include <amx_internal.h>

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
#if !defined(USE_CLEAR_UNUSED_SPACE) && AMX_DEBUG
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
    amx_tick_t timestamp; // request expiration, reply last retransmit
    #if AMUDP_COLLECT_LATENCY_STATS
      amx_tick_t firstSendTime; /* for statistical purposes only */
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
#define AMUDP_BUFFER_STATS AMX_DEBUG_VERBOSE
#endif

struct amudp_bufferpool;

typedef union amudp_bufferheader {
  struct amudp_bufferpool *pool;
  union amudp_bufferheader *next;
  uint64_t _pad[1]; // maintain 8-byte alignment
} amudp_bufferheader_t;

typedef struct amudp_bufferpool {
  #if AMX_DEBUG
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

  amx_tick_t replyEpoch; /* timestamp of the first non-loopback reply sent during the current AMPoll */

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

#if AMUDP_COLLECT_STATS
  #define AMUDP_STATS(stmt) stmt
#else
  #define AMUDP_STATS(stmt) 
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

/* ------------------------------------------------------------------------------------ */
// Buffer descriptor accessors
static inline amudp_bufdesc_t *AMUDP_get_desc(ep_t ep, amudp_node_t proc, int inst, int isrequest, int allocate) {
  AMX_assert(ep);
  AMX_assert(ep->perProcInfo);
  AMX_assert(ep->depth > 0);
  AMX_assert(proc <= ep->P);
  AMX_assert(inst <= ep->depth);
  amudp_perproc_info_t * const pinfo = &ep->perProcInfo[proc];
  amudp_bufdesc_t ** const pdesc = (isrequest ? &pinfo->requestDesc : &pinfo->replyDesc);
  if_pf (allocate && !*pdesc) { // first touch allocate
    *pdesc = (amudp_bufdesc_t *)AMX_calloc(ep->depth, sizeof(amudp_bufdesc_t));
  }
  AMX_assert(*pdesc);
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
#define GET_MSG_ARGS(pmsg) ( (void)((pmsg)->_instance), /* typecheck pmsg */ \
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

extern void AMUDP_DefaultReturnedMsg_Handler(int status, op_t opcode, void *token);

//------------------------------------------------------------------------------------
/* SPMD control information that has to be shared */
extern SOCKET AMUDP_SPMDControlSocket; /* SPMD TCP control socket */
extern int AMUDP_SPMDSpawnRunning; /* true while spawn is active */
extern int AMUDP_SPMDRedirectStdsockets; /* true if stdin/stdout/stderr should be redirected */
extern int AMUDP_SPMDwakeupOnControlActivity; /* true if waitForEndpointActivity should return on control socket activity */
extern volatile int AMUDP_SPMDIsActiveControlSocket; 
//------------------------------------------------------------------------------------

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
/* return the first environment variable matching one of:
    AMX_ENV_PREFIX_STR##_##basekey
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
  #if AMX_DEBUG
   #define ASYNC_CHECK(enabled) do {                                             \
      int flags = fcntl(AMUDP_SPMDControlSocket, F_GETFL, 0);                    \
      if ((enabled && (flags & (O_ASYNC|O_NONBLOCK)) != (O_ASYNC|O_NONBLOCK)) || \
         (!enabled && (flags & (O_ASYNC|O_NONBLOCK)) != 0)) {                    \
        AMX_FatalErr("Failed to modify O_ASYNC|O_NONBLOCK flags in fcntl"        \
                   " - try disabling USE_ASYNC_TCP_CONTROL");                    \
      }                                                                          \
    } while (0)
  #else
   #define ASYNC_CHECK(enabled) ((void)0)
  #endif
  #define ASYNC_TCP_ENABLE() do {                                                       \
      if (fcntl(AMUDP_SPMDControlSocket, F_SETFL, O_ASYNC|O_NONBLOCK)) {                \
        perror("fcntl(F_SETFL, O_ASYNC|O_NONBLOCK)");                                   \
        AMX_FatalErr("Failed to fcntl(F_SETFL, O_ASYNC|O_NONBLOCK) on TCP control socket" \
                   " - try disabling USE_ASYNC_TCP_CONTROL");                           \
      } else ASYNC_CHECK(1);                                                            \
      if (inputWaiting(AMUDP_SPMDControlSocket,false)) /* check for arrived messages */ \
        AMUDP_SPMDIsActiveControlSocket = 1;                                            \
    } while(0)

  #define _ASYNC_TCP_DISABLE(ignoreerr)  do {                              \
      if (fcntl(AMUDP_SPMDControlSocket, F_SETFL, 0) && !ignoreerr) {      \
        perror("fcntl(F_SETFL, 0)");                                       \
        AMX_FatalErr("Failed to fcntl(F_SETFL, 0) on TCP control socket"   \
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
// checkpoint/restart
extern int AMUDP_SPMDRestartActive;

//------------------------------------------------------------------------------------

AMUDP_END_EXTERNC

#endif
