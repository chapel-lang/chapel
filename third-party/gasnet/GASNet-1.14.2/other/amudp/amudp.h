/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp.h,v $
 *     $Date: 2010/05/18 01:28:12 $
 * $Revision: 1.41.2.1 $
 * Description: AMUDP Header
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef __AMUDP_H
#define __AMUDP_H

#include <portable_inttypes.h>
#include <portable_platform.h>

#ifdef UETH
  #include <ueth.h>
#else
  #include <socket.h>
#endif

#include <stdarg.h>
#include <stddef.h>

/* miscellaneous macro helpers */
#define _STRINGIFY_HELPER(x) #x
#define _STRINGIFY(x) _STRINGIFY_HELPER(x)

#define AMUDP_LIBRARY_VERSION      3.8
#define AMUDP_LIBRARY_VERSION_STR  _STRINGIFY(AMUDP_LIBRARY_VERSION)

/* naming policy:
  AM-defined things start with AM_
  internal things start with amudp_ or AMUDP_
  */

/* ------------------------------------------------------------------------------------ */
/* Internal constants */
#define AMUDP_MAX_SHORT    16    /* max number of handler arguments, >=8 */
#define AMUDP_MAX_MEDIUM   512   /* max. data transmission unit for medium messages, >= 512 */
#ifdef UETH
  #define AMUDP_MAX_LONG     (AMUDP_MAX_MEDIUM*256)  /* max. data size for xfer and get operations >= 8192 */
#elif PLATFORM_OS_IRIX
  #define AMUDP_MAX_LONG     61000  /* max. UDP datagram on IRIX is apparently 61412 */
#elif PLATFORM_OS_TRU64 || PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || \
      PLATFORM_OS_DARWIN || PLATFORM_OS_AIX
  #define AMUDP_MAX_LONG     9000   /* max UDP datagram on OSF/FREEBSD/DARWIN is apparently 9196 */
#else
  #define AMUDP_MAX_LONG     65000  /* default max. UDP datagram */
#endif

#define AMUDP_MAX_NUMHANDLERS      256  /* max. handler-table entries >= 256 */
#define AMUDP_MAX_NUMTRANSLATIONS  16384 /* max. translation-table entries >= 256 */
#define AMUDP_MAX_SEGLENGTH  ((uintptr_t)-1) /* max. dest_offset */

#define AMUDP_MAX_BUNDLES          255  /* max bundles that can be allocated */
#define AMUDP_MAX_NETWORKDEPTH     1024 /* max depth we ever allow user to ask for (constrained by instance bits) */
#define AMUDP_MAX_SPMDPROCS        AMUDP_MAX_NUMTRANSLATIONS  /* max SPMD procs we support */

#ifndef AMUDP_COLLECT_STATS
#define AMUDP_COLLECT_STATS   1
#endif
#ifndef AMUDP_COLLECT_LATENCY_STATS
#define AMUDP_COLLECT_LATENCY_STATS   1
#endif
/* ------------------------------------------------------------------------------------ */
/* Simple user-visible types */

/* Endpoint tag */
typedef uint64_t tag_t;

/* Handler index */
typedef uint8_t handler_t;
#define AMUDP_BADHANDLERVAL(h) (0)
/* #define AMUDP_BADHANDLERVAL(h) (h < 0 || h >= AMUDP_MAX_NUMHANDLERS) */

/* Endpoint name */
#ifdef UETH
  typedef ueth_addr_t en_t;
#else
  typedef struct sockaddr_in en_t;
#endif

struct amudp_ep; /* forward decls */
struct amudp_buf;

/* CPU ticks */
typedef uint64_t amudp_cputick_t; 

/* ------------------------------------------------------------------------------------ */
/* Internal types */

/* message flags:
 * 0-1: category
 * 2:   request vs. reply 
 * 3-7: numargs
 * message instance:
 * 0:    sequence number
 * 1-10: instance number
 * 11-15: reserved
 */
typedef unsigned char amudp_flag_t;
typedef enum {
  amudp_Short=0, 
  amudp_Medium=1, 
  amudp_Long=2,
  amudp_NumCategories=3
} amudp_category_t;

#define AMUDP_MSG_SETFLAGS(pmsg, isreq, cat, numargs, seqnum, instance) do { \
   (pmsg)->flags = (amudp_flag_t) (                                          \
                   (((numargs) & 0x1F) << 3)                                 \
                 | (((isreq) & 0x1) << 2)                                    \
                 |  ((cat) & 0x3)                                            \
                 );                                                          \
   (pmsg)->_instance = (uint16_t) (                                          \
                   ((seqnum) & 0x1)                                          \
                 | (((instance) & 0x3FF) << 1)                               \
                 );                                                          \
  } while (0)
#define AMUDP_MSG_NUMARGS(pmsg)   ( ( ((unsigned char)(pmsg)->flags) >> 3 ) & 0x1F)
#define AMUDP_MSG_ISREQUEST(pmsg) (!!(((unsigned char)(pmsg)->flags) & 0x4))
#define AMUDP_MSG_CATEGORY(pmsg)  ((amudp_category_t)((pmsg)->flags & 0x3))
#define AMUDP_MSG_SEQNUM(pmsg)    (((unsigned char)(pmsg)->_instance) & 0x1)
#define AMUDP_MSG_INSTANCE(pmsg)  ((uint16_t)((pmsg)->_instance >> 1))

/* active message header & meta info fields */
typedef struct {
  tag_t         tag;

  uint16_t      _instance; /* instance and seqnum */
  amudp_flag_t  flags;
  handler_t     handlerId;

  uint16_t      nBytes;
  uint8_t       systemMessageType;
  uint8_t       systemMessageArg;

  uintptr_t	destOffset;
} amudp_msg_t;

/* non-transmitted amudp buffer bookkeeping info -
 * this data must be kept to a bare minimum because it constrains packet size 
 */
typedef struct {
  int8_t handlerRunning;
  int8_t replyIssued;
  uint16_t sourceId;      /* 0-based endpoint id of remote */
  en_t sourceAddr;        /* address of remote */
  struct amudp_ep *dest;  /* ep_t of endpoint that received this message */
  struct amudp_buf *bulkBuffer; /* if non-NULL, points to a bulk buffer 
                              holding the transmitted data fields for this buffer */
} amudp_bufstatus_t;

/* active message buffer, including message and space for data payload */
typedef struct amudp_buf {
  #ifdef UETH
    char ueth_header[UETH_HEADERLENGTH];
  #endif

  amudp_msg_t	Msg;
  uint8_t     _Data[(4*AMUDP_MAX_SHORT)+AMUDP_MAX_MEDIUM]; /* holds args and data */

  /* received requests & replies only 
    (NOT valid inside bulk buffers, and only the bulkBuffer field is valid in send buffers) */
  amudp_bufstatus_t status;

  #ifdef UETH
   /* this bufhandle allows us to query UETH on the status of whether this send buffer has
      left the NIC yet, or is still waiting in the memory FIFO
      this could go in descriptor, but need 4-bytes of pad here anyhow for correct alignment */
   int32_t bufhandle; 
  #endif
} amudp_buf_t;

#define AMUDP_MIN_NETWORK_MSG     ((int)(uintptr_t)&((amudp_buf_t *)NULL)->_Data[0])
#define AMUDP_MAX_NETWORK_MSG     (AMUDP_MIN_NETWORK_MSG+(4*AMUDP_MAX_SHORT)+AMUDP_MAX_MEDIUM)
#define AMUDP_MAXBULK_NETWORK_MSG (AMUDP_MIN_NETWORK_MSG+(4*AMUDP_MAX_SHORT)+AMUDP_MAX_LONG)

/* message buffer descriptor */
typedef struct {
  amudp_cputick_t timestamp; 
  #if AMUDP_COLLECT_LATENCY_STATS
    amudp_cputick_t firstSendTime; /* for statistical purposes only */
  #endif
  uint8_t retryCount; /* where we are in the retransmit backoff */
  uint8_t transmitCount; /* how many times we've actually transmitted */
  uint8_t inuse;
  uint8_t seqNum; /* seq number for next message to be sent/recv'd on this desc */
} amudp_bufdesc_t;

/* ------------------------------------------------------------------------------------ */
/* Complex user-visible types */

/* statistical collection 
 *  changes here need to also be reflected in the initialization vector AMUDP_initial_stats
 */
typedef struct {
  uint64_t RequestsSent[amudp_NumCategories]; /* counts fragments for amudp_Long && !USE_TRUE_BULK_XFERS */
  uint64_t RepliesSent[amudp_NumCategories];
  uint64_t RequestsRetransmitted[amudp_NumCategories];
  uint64_t RepliesRetransmitted[amudp_NumCategories];
  uint64_t RequestsReceived[amudp_NumCategories];   /*  includes retransmits */
  uint64_t RepliesReceived[amudp_NumCategories];    /*  includes retransmits */
  uint64_t ReturnedMessages;
  amudp_cputick_t RequestMinLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  amudp_cputick_t RequestMaxLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  amudp_cputick_t RequestSumLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  uint64_t RequestDataBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t ReplyDataBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t RequestTotalBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t ReplyTotalBytesSent[amudp_NumCategories];  /* total of args,payload and overhead */
  uint64_t TotalBytesSent; /* total user level packet sizes for all req/rep */
} amudp_stats_t;

#ifdef GASNET_USE_STRICT_PROTOTYPES
typedef void *amudp_handler_fn_t;
#else
typedef void (*amudp_handler_fn_t)();  /* prototype for handler function */
#endif
typedef struct {
  char inuse; /*  entry in use */
  en_t name;  /*  remote address */
  tag_t tag;  /*  remote tag */
  uint16_t id; /*  id in compressed table */
} amudp_translation_t;

typedef struct {
  uintptr_t minDestOffset;   /* smallest destOffset seen in this xfer */
  uint32_t runningLength;    /* number of bytes copied thus far */
  uint8_t  packetsRemaining; /* number of packets left to be recieved (0 = notinuse)*/
  uint8_t  numargs;          /* cache number of args */
  uint32_t args[AMUDP_MAX_SHORT]; /* cache the args (sent in a single fragment) */
} bulkslot_t;

typedef struct {
  uint16_t  instanceHint; /* instance hint pointer for request buffer allocation */
  en_t      remoteName;   /* gives us a compacted version of the translation table */
  tag_t     tag;
  bulkslot_t inboundBulkSlot[16]; /* slots for maintaining inbound bulk transfer status */
} amudp_perproc_info_t;

typedef void (*AMUDP_preHandlerCallback_t)(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
typedef void (*AMUDP_postHandlerCallback_t)(amudp_category_t cat, int isReq);

/* Endpoint bundle object */
typedef struct amudp_eb {
  struct amudp_ep **endpoints;   /* dynamically-grown array of endpoints in bundle */
  int	  n_endpoints;           /* Number of EPs in the bundle */
  int	  cursize;               /* size of the array */
  uint8_t event_mask;            /* Event Mask for blocking ops */
} *eb_t;

/* Endpoint object */
typedef struct amudp_ep {
  en_t name;            /* Endpoint name */
  tag_t tag;            /* current tag */
  eb_t eb;              /* Bundle of endpoint */

  void *segAddr;        /* Start address of EP VM segment */
  uintptr_t segLength;  /* Length of EP VM segment    */

  amudp_translation_t translation[AMUDP_MAX_NUMTRANSLATIONS]; /* translation table */
  amudp_handler_fn_t  handler[AMUDP_MAX_NUMHANDLERS]; /* handler table */

  /* internal structures */
  #ifndef UETH 
    SOCKET s; /* UDP socket */
    int socketRecvBufferSize; /* only used if USE_SOCKET_RECVBUFFER_GROW */
    int socketRecvBufferMaxedOut;
  #endif
  int P;     /* the number of endpoints we communicate with - also number of translations currently in use */
  int depth; /* network depth, -1 until AM_SetExpectedResources is called */
  int PD; /* cached value of P * depth */

  /* buffer descriptor tables */
  amudp_bufdesc_t* requestDesc;
  amudp_bufdesc_t* replyDesc;

  int outstandingRequests; /* number of requests awaiting a reply, does NOT include loopback */
  int timeoutCheckPosn; /* current position of the timeout-checking circular walk */

  amudp_perproc_info_t *perProcInfo; 

  /* buffers for outbound messages */
  amudp_buf_t* requestBuf;
  amudp_buf_t* replyBuf;
  amudp_buf_t* temporaryBuf; /* a single extra buffer used for temporary operations */

  /* buffers for inbound messages */
  #ifndef UETH
    /* invariant: buffer empty when rxFreeIdx == rxReadyIdx
     *            buffer full when (rxFreeIdx + 1) % rxNumBufs == rxReadyIdx
     * active buffers are rxReadyIdx ... rxFreeIdx-1 (with circular wrap-around)
     */
    amudp_buf_t* rxBuf;  /* circular buffer */
    uint32_t rxNumBufs;  /* size of circular buffer */
    uint32_t rxReadyIdx; /* oldest unserviced, received message */
    uint32_t rxFreeIdx;  /* beginning of free list */
  #endif

  AMUDP_preHandlerCallback_t preHandlerCallback; /* client hooks for statistical/debugging usage */
  AMUDP_postHandlerCallback_t postHandlerCallback;

  amudp_stats_t stats;  /* statistical collection */

  amudp_buf_t **bulkBufferPool; /* list of all bulk buffers */
  int bulkBufferPoolSz;     /* length of list */
  int bulkBufferPoolFreeCnt; /* number of those that are free (which appear first in list) */

} *ep_t;

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
 * Error codes for the AM error handler (status).
 */
#define EBADARGS          1     /* Arguments to request or reply function invalid    */
#define EBADENTRY         2     /* X-lation table index selected unbound table entry */
#define EBADTAG           3     /* Sender's tag did not match the receiver's EP tag  */ 
#define EBADHANDLER       4     /* Invalid index into the recv.'s handler table      */ 
#define EBADSEGOFF        5     /* Offset into the dest-memory VM segment invalid    */
#define EBADLENGTH        6     /* Bulk xfer length goes beyond a segment's end      */
#define EBADENDPOINT      7     /* Destination endpoint does not exist               */
#define ECONGESTION       8     /* Congestion at destination endpoint                */
#define EUNREACHABLE      9     /* Destination endpoint unreachable                  */
#define EREPLYREJECTED    10    /* Destination endpoint refused reply message        */


/*
 * Op codes for the AM error handler (opcode).
 */
typedef int op_t;
#define AM_REQUEST_M      1
#define AM_REQUEST_IM     2
#define AM_REQUEST_XFER_M 3
#define AM_REPLY_M        4
#define AM_REPLY_IM       5
#define AM_REPLY_XFER_M   6

/* ------------------------------------------------------------------------------------ */

SOCK_BEGIN_EXTERNC

/* AMUDP-specific user entry points */
extern int AMUDP_VerboseErrors; /* set to non-zero for verbose error reporting */
extern int AMUDP_PoliteSync; /* set to non-zero for polite blocking while awaiting send resources */
extern int AMUDP_SilentMode; /* set to non-zero to silence any non-error output */

#ifdef __GNUC__
__attribute__((__format__ (__printf__, 1, 2)))
#endif
extern void AMUDP_FatalErr(const char *msg, ...);

/* set the UDP interface (local IP address) to be used for new endpoints -
 * it's necessary to call this on multi-homed hosts, otherwise endpoint creation will fail
 */
extern int AMUDP_SetUDPInterface(uint32_t IPAddress); 

/* statistical collection */
extern int AMUDP_GetEndpointStatistics(ep_t ep, amudp_stats_t *stats); /* get ep counters */
extern int AMUDP_ResetEndpointStatistics(ep_t ep); /* reset ep counters */
extern int AMUDP_AggregateStatistics(amudp_stats_t *runningsum, amudp_stats_t *newvalues); 
  /* aggregate statistics - augment running sum with the given values (fp is a FILE *) */
extern const char *AMUDP_DumpStatistics(void *fp, amudp_stats_t *stats, int globalAnalysis); 
  /* output stats to fp (if non-null) in human-readable form.
   * return a pointer to the same output in an internal static buffer (rewritten on each call)
   * pass globalAnalysis non-zero if stats is a global agreggation across all nodes
   */

extern const amudp_stats_t AMUDP_initial_stats; /* the "empty" values for counters */

/* set the client callback fns to run before/after handler execution 
   (callback fns may _NOT_ make any AMMPI calls, directly or indirectly)
   set to NULL for none
*/
extern int AMUDP_SetHandlerCallbacks(ep_t ep, AMUDP_preHandlerCallback_t preHandlerCallback, 
                                              AMUDP_postHandlerCallback_t postHandlerCallback);

/* ------------------------------------------------------------------------------------ */
/* AM-2 Entry Points */

/* strictly speaking, many of these AM entry points should be true-blue functions
   (so, for example, a user could create a function pointer to them)
   but that seems a silly justification, so we went with macros on many of them 
   in the interests of performance */

#ifdef AMUDP_COEXIST_WITH_AM
  /* allow linking with another library that also implements AM - rename entry points 
   * note this still does not allow the same .c file to use both AM implementations
   * (any given source file should #include at most one AM header file)
   */
  #define AM_Init                 AMUDP_Init
  #define AM_Terminate            AMUDP_Terminate
  #define AM_AllocateBundle       AMUDP_AllocateBundle
  #define AM_AllocateEndpoint     AMUDP_AllocateEndpoint
  #define AM_FreeBundle           AMUDP_FreeBundle
  #define AM_FreeEndpoint         AMUDP_FreeEndpoint
  #define AM_MoveEndpoint         AMUDP_MoveEndpoint
  #define AM_GetSeg               AMUDP_GetSeg
  #define AM_SetSeg               AMUDP_SetSeg
  #define AM_MaxSegLength         AMUDP_MaxSegLength
  #define AM_GetTag               AMUDP_GetTag
  #define AM_SetTag               AMUDP_SetTag
  #define AM_Map                  AMUDP_Map
  #define AM_MapAny               AMUDP_MapAny
  #define AM_UnMap                AMUDP_UnMap
  #define AM_GetTranslationInuse  AMUDP_GetTranslationInuse
  #define AM_GetTranslationTag    AMUDP_GetTranslationTag
  #define AM_GetTranslationName   AMUDP_GetTranslationName
  #define AM_SetExpectedResources AMUDP_SetExpectedResources
  #define _AM_SetHandler          AMUDP_SetHandler
  #define _AM_SetHandlerAny       AMUDP_SetHandlerAny
  #define AM_GetEventMask         AMUDP_GetEventMask
  #define AM_SetEventMask         AMUDP_SetEventMask
  #define AM_WaitSema             AMUDP_WaitSema
  #define AM_GetSourceEndpoint    AMUDP_GetSourceEndpoint
  #define AM_GetDestEndpoint      AMUDP_GetDestEndpoint
  #define AM_GetMsgTag            AMUDP_GetMsgTag
  #define AM_Poll                 AMUDP_Poll
#endif

/* standardized AM-2 extensions */
#ifndef AMUDP
#define AMUDP 1
#endif

#define AMX_VerboseErrors         AMUDP_VerboseErrors
#define AMX_GetEndpointStatistics AMUDP_GetEndpointStatistics
#define AMX_DumpStatistics        AMUDP_DumpStatistics
#define AMX_AggregateStatistics   AMUDP_AggregateStatistics
#define AMX_initial_stats         AMUDP_initial_stats
#define amx_stats_t               amudp_stats_t
#define amx_handler_fn_t          amudp_handler_fn_t
#define AMX_FatalErr            AMUDP_FatalErr

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

/* idiot proofing */
#if defined(AMUDP_DEBUG) && (defined(__OPTIMIZE__) || defined(NDEBUG))
  #if !defined(AMUDP_ALLOW_OPTIMIZED_DEBUG) && !defined(GASNET_ALLOW_OPTIMIZED_DEBUG)
    #error Tried to compile AMUDP client code with optimization enabled but also AMUDP_DEBUG (which seriously hurts performance). Disable C and C++ compiler optimization or reconfigure/rebuild without --enable-debug
  #endif
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

#undef AM_Init
#define AM_Init _CONCAT(AM_Init_AMUDP,AMUDP_DEBUG_CONFIG)

/* System parameters */
#define AM_MaxShort()   AMUDP_MAX_SHORT
#define AM_MaxMedium()  AMUDP_MAX_MEDIUM
#define AM_MaxLong()    AMUDP_MAX_LONG

#define AM_MaxNumHandlers()               AMUDP_MAX_NUMHANDLERS
#define AM_MaxNumTranslations(trans)      (*(trans) = AMUDP_MAX_NUMTRANSLATIONS,AM_OK)
extern int AM_MaxSegLength(uintptr_t* nbytes);

/* System initialization/termination */
extern int AM_Init(void);
extern int AM_Terminate(void);

/* endpoint/bundle management */
extern int AM_AllocateBundle(int type, eb_t *endb);
extern int AM_AllocateEndpoint(eb_t bundle, ep_t *endp, en_t *endpoint_name);
extern int AM_FreeBundle(eb_t bundle);
extern int AM_FreeEndpoint(ep_t ea);
extern int AM_MoveEndpoint(ep_t ea, eb_t from_bundle, eb_t to_bundle);

extern int AM_GetSeg(ep_t ea, void **addr, uintptr_t *nbytes);
extern int AM_SetSeg(ep_t ea, void *addr, uintptr_t nbytes);
extern int AM_GetTag(ep_t ea, tag_t *tag);
extern int AM_SetTag(ep_t ea, tag_t tag);

/* Translation table */
extern int AM_Map(ep_t ea, int index, en_t name, tag_t tag);
extern int AM_MapAny(ep_t ea, int *index, en_t name, tag_t tag);
extern int AM_UnMap(ep_t ea, int index);
extern int AM_GetTranslationInuse(ep_t ea, int i);
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag);
extern int AM_GetTranslationName(ep_t ea, int i, en_t *gan);
#define AM_GetNumTranslations(ep, pntrans)  \
  ((ep) ? ((*(pntrans) = AMUDP_MAX_NUMTRANSLATIONS), AM_OK) : AM_ERR_BAD_ARG)
#define AM_SetNumTranslations(ep, ntrans)  \
  ((ep) ? ((ntrans) == AMUDP_MAX_NUMTRANSLATIONS ? AM_OK : AM_ERR_RESOURCE) : AM_ERR_BAD_ARG)
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests);

/* Handler table */
extern int _AM_SetHandler(ep_t ea, handler_t handler, amudp_handler_fn_t function);
#define AM_SetHandler(ea, handler, function) _AM_SetHandler((ea), (handler), (amudp_handler_fn_t)(function)) 
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, amudp_handler_fn_t function);
#define AM_SetHandlerAny(ea, handler, function) _AM_SetHandlerAny((ea), (handler), (amudp_handler_fn_t)(function))
#define AM_GetNumHandlers(ep, pnhandlers)  \
  ((ep) ? ((*(pnhandlers) = AMUDP_MAX_NUMHANDLERS), AM_OK) : AM_ERR_BAD_ARG) : AM_ERR_BAD_ARG)
#define AM_SetNumHandlers(ep, nhandlers)  \
  ((ep) ? ((nhandlers) == AMUDP_MAX_NUMHANDLERS ? AM_OK : AM_ERR_RESOURCE)

/* Events */
extern int AM_GetEventMask(eb_t eb, int *mask);
extern int AM_SetEventMask(eb_t eb, int mask);
extern int AM_WaitSema(eb_t eb);


/* Message interrogation */
extern int AM_GetSourceEndpoint(void *token, en_t *gan);
extern int AM_GetDestEndpoint(void *token, ep_t *endp);
extern int AM_GetMsgTag(void *token, tag_t *tagp);
extern int AMUDP_GetSourceId(void *token, int *srcid);

/* Poll */
extern int AM_Poll(eb_t bundle);

/* Requests and Replies
   These six functions do all requests and replies.
   Macros below expand all the variants */

extern int AMUDP_Request(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                         int numargs, ...);
extern int AMUDP_RequestI (ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, ...);
extern int AMUDP_RequestXfer(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, ...);

extern int AMUDP_Reply(void *token, handler_t handler, 
                         int numargs, ...);
extern int AMUDP_ReplyI(void *token, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, ...);
extern int AMUDP_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, ...);

/* alternate forms that take va_list ptr to support GASNet */
extern int AMUDP_RequestVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMUDP_RequestIVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr);
extern int AMUDP_RequestXferVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, va_list argptr);

extern int AMUDP_ReplyVA(void *token, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMUDP_ReplyIVA(void *token, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr);
extern int AMUDP_ReplyXferVA(void *token, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr);

/* we cast to int32_t here to simluate function call - AM says these functions take 32-bit int args, 
 * so this cast accomplishes the conversion to integral type for floating-point actuals, and
 * the truncation which might happen to long integer actuals
 * note the C compiler will subsequently apply default argument promotion to these arguments 
 * (because these arguments fall within the ellipses (...) of the called functions)
 * which means they'll subsequently be promoted to int (which may differ from int32_t)
 */

#define AM_Request0(ep, destep, hnum) \
   AMUDP_Request(ep, destep, hnum, 0)
#define AM_Request1(ep, destep, hnum, a0) \
   AMUDP_Request(ep, destep, hnum, 1, (int32_t)a0)
#define AM_Request2(ep, destep, hnum, a0, a1) \
   AMUDP_Request(ep, destep, hnum, 2, (int32_t)a0, (int32_t)a1)
#define AM_Request3(ep, destep, hnum, a0, a1, a2) \
   AMUDP_Request(ep, destep, hnum, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_Request4(ep, destep, hnum, a0, a1, a2, a3) \
   AMUDP_Request(ep, destep, hnum, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_Request5(ep, destep, hnum, a0, a1, a2, a3, a4) \
   AMUDP_Request(ep, destep, hnum, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_Request6(ep, destep, hnum, a0, a1, a2, a3, a4, a5) \
   AMUDP_Request(ep, destep, hnum, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_Request7(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_Request(ep, destep, hnum, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_Request8(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_Request(ep, destep, hnum, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_Request9(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_Request(ep, destep, hnum, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_Request10(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_Request(ep, destep, hnum, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_Request11(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_Request(ep, destep, hnum, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_Request12(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_Request(ep, destep, hnum, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_Request13(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_Request(ep, destep, hnum, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_Request14(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_Request(ep, destep, hnum, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_Request15(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_Request(ep, destep, hnum, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_Request16(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_Request(ep, destep, hnum, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestI0(ep, destep, hnum, sa, cnt) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 0)
#define AM_RequestI1(ep, destep, hnum, sa, cnt, a0) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 1, (int32_t)a0)
#define AM_RequestI2(ep, destep, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestI3(ep, destep, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestI4(ep, destep, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestI5(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestI6(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestI7(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestI8(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestI9(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestI10(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestI11(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestI12(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestI13(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestI14(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestI15(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestI16(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestXfer0(ep, destep, desto, hnum, sa, cnt) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 0)
#define AM_RequestXfer1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 1, (int32_t)a0)
#define AM_RequestXfer2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestXfer3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestXfer4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestXfer5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestXfer6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestXfer7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestXfer8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestXfer9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestXfer10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestXfer11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestXfer12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestXfer13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestXfer14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestXfer15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestXfer16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestXferAsync0(ep, destep, desto, hnum, sa, cnt) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 0)
#define AM_RequestXferAsync1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 1, (int32_t)a0)
#define AM_RequestXferAsync2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestXferAsync3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestXferAsync4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestXferAsync5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestXferAsync6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestXferAsync7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestXferAsync8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestXferAsync9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestXferAsync10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestXferAsync11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestXferAsync12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestXferAsync13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestXferAsync14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestXferAsync15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestXferAsync16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_Reply0(token, hnum) \
   AMUDP_Reply(token, hnum, 0)
#define AM_Reply1(token, hnum, a0) \
   AMUDP_Reply(token, hnum, 1, (int32_t)a0)
#define AM_Reply2(token, hnum, a0, a1) \
   AMUDP_Reply(token, hnum, 2, (int32_t)a0, (int32_t)a1)
#define AM_Reply3(token, hnum, a0, a1, a2) \
   AMUDP_Reply(token, hnum, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_Reply4(token, hnum, a0, a1, a2, a3) \
   AMUDP_Reply(token, hnum, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_Reply5(token, hnum, a0, a1, a2, a3, a4) \
   AMUDP_Reply(token, hnum, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_Reply6(token, hnum, a0, a1, a2, a3, a4, a5) \
   AMUDP_Reply(token, hnum, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_Reply7(token, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_Reply(token, hnum, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_Reply8(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_Reply(token, hnum, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_Reply9(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_Reply(token, hnum, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_Reply10(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_Reply(token, hnum, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_Reply11(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_Reply(token, hnum, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_Reply12(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_Reply(token, hnum, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_Reply13(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_Reply(token, hnum, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_Reply14(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_Reply(token, hnum, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_Reply15(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_Reply(token, hnum, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_Reply16(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_Reply(token, hnum, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_ReplyI0(token, hnum, sa, cnt) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 0)
#define AM_ReplyI1(token, hnum, sa, cnt, a0) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 1, (int32_t)a0)
#define AM_ReplyI2(token, hnum, sa, cnt, a0, a1) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 2, (int32_t)a0, (int32_t)a1)
#define AM_ReplyI3(token, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_ReplyI4(token, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_ReplyI5(token, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_ReplyI6(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_ReplyI7(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_ReplyI8(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_ReplyI9(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_ReplyI10(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_ReplyI11(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_ReplyI12(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_ReplyI13(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_ReplyI14(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_ReplyI15(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_ReplyI16(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_ReplyXfer0(token, desto, hnum, sa, cnt) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 0)
#define AM_ReplyXfer1(token, desto, hnum, sa, cnt, a0) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 1, (int32_t)a0)
#define AM_ReplyXfer2(token, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 2, (int32_t)a0, (int32_t)a1)
#define AM_ReplyXfer3(token, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_ReplyXfer4(token, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_ReplyXfer5(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_ReplyXfer6(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_ReplyXfer7(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_ReplyXfer8(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_ReplyXfer9(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_ReplyXfer10(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_ReplyXfer11(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_ReplyXfer12(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_ReplyXfer13(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_ReplyXfer14(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_ReplyXfer15(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_ReplyXfer16(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)


SOCK_END_EXTERNC

#endif
