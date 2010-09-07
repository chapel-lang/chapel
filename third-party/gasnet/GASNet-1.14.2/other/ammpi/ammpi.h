/*   $Source: /var/local/cvs/gasnet/other/ammpi/ammpi.h,v $
 *     $Date: 2010/05/18 01:28:07 $
 * $Revision: 1.45.2.1 $
 * Description: AMMPI Header
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef __AMMPI_H
#define __AMMPI_H

#include <portable_inttypes.h>

#include <stdarg.h>
#include <stddef.h>

/* miscellaneous macro helpers */
#define _STRINGIFY_HELPER(x) #x
#define _STRINGIFY(x) _STRINGIFY_HELPER(x)

#define AMMPI_LIBRARY_VERSION      2.6
#define AMMPI_LIBRARY_VERSION_STR  _STRINGIFY(AMMPI_LIBRARY_VERSION)

/* naming policy:
  AM-defined things start with AM_
  internal things start with ammpi_ or AMMPI_
  */

/* ------------------------------------------------------------------------------------ */
/* Internal constants */
#define AMMPI_MAX_SHORT    16      /* max number of handler arguments, >=8 */
#define AMMPI_MAX_MEDIUM   65000   /* max. data transmission unit for medium messages, >= 512 */
#define AMMPI_MAX_LONG     65000   /* max. data transmission unit for large messages, >= 8192 */

#define AMMPI_MAX_NUMHANDLERS      256  /* max. handler-table entries >= 256 */
#define AMMPI_INIT_NUMTRANSLATIONS 256
#define AMMPI_MAX_NUMTRANSLATIONS  (0x7FFFFFFFu)  /* max. translation-table entries >= 256 */
#define AMMPI_MAX_SEGLENGTH  ((uintptr_t)-1) /* max. dest_offset */
#define AMMPI_MAX_BUNDLES          255  /* max bundles that can be allocated */
#define AMMPI_MAX_SPMDPROCS        AMMPI_MAX_NUMTRANSLATIONS  /* max SPMD procs we support */

/* ------------------------------------------------------------------------------------ */
/* Simple user-visible types */

/* Endpoint tag */
typedef uint64_t tag_t;

/* Handler index */
typedef uint8_t handler_t;
#define AMMPI_BADHANDLERVAL(h) (0)
/* #define AMMPI_BADHANDLERVAL(h) (h < 0 || h >= AMMPI_MAX_NUMHANDLERS) */

/* Endpoint naming */
typedef uint32_t ammpi_node_t;
typedef struct {
  int mpirank;
  int mpitag;
} en_t;

/* Endpoint types */
struct ammpi_eb;
struct ammpi_ep; 
typedef struct ammpi_eb *eb_t;
typedef struct ammpi_ep *ep_t;

/* ------------------------------------------------------------------------------------ */
/* AMMPI extension types */
#ifdef GASNET_USE_STRICT_PROTOTYPES
typedef void *ammpi_handler_fn_t;
#else
typedef void (*ammpi_handler_fn_t)();  /* prototype for handler function */
#endif

typedef enum {
  ammpi_Short=0, 
  ammpi_Medium=1, 
  ammpi_Long=2,
  ammpi_NumCategories=3
} ammpi_category_t;

typedef void (*AMMPI_preHandlerCallback_t)(ammpi_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
typedef void (*AMMPI_postHandlerCallback_t)(ammpi_category_t cat, int isReq);

/* statistical collection 
 *  changes here need to also be reflected in the initialization vector AMMPI_initial_stats
 */
typedef struct {
  uint64_t RequestsSent[ammpi_NumCategories];
  uint64_t RepliesSent[ammpi_NumCategories];
  uint64_t RequestsReceived[ammpi_NumCategories];
  uint64_t RepliesReceived[ammpi_NumCategories];
  uint64_t ReturnedMessages;
  uint64_t RequestMinLatency;  /* only if AMMPI_COLLECT_LATENCY_STATS */
  uint64_t RequestMaxLatency;  /* only if AMMPI_COLLECT_LATENCY_STATS */
  uint64_t RequestSumLatency;  /* only if AMMPI_COLLECT_LATENCY_STATS */
  uint64_t RequestDataBytesSent[ammpi_NumCategories];  /* total of args + data payload */
  uint64_t ReplyDataBytesSent[ammpi_NumCategories];  /* total of args + data payload */
  uint64_t RequestTotalBytesSent[ammpi_NumCategories];  /* total of args + data payload */
  uint64_t ReplyTotalBytesSent[ammpi_NumCategories];  /* total of args,payload and overhead */
  uint64_t TotalBytesSent; /* total user level packet sizes for all req/rep */
} ammpi_stats_t;

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
} ammpi_eventmask_t;

typedef enum {
    AM_SEQ,             /* Sequential bundle/endpoint access */
    AM_PAR,             /* Concurrent bundle/endpoint access */
    AM_NUM_BUNDLE_MODES
} ammpi_bundle_mode_t;

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
#ifdef __cplusplus
  #define AMMPI_BEGIN_EXTERNC extern "C" {
  #define AMMPI_END_EXTERNC }
#else
  #define AMMPI_BEGIN_EXTERNC 
  #define AMMPI_END_EXTERNC 
#endif

AMMPI_BEGIN_EXTERNC

/* AMMPI-specific user entry points */
extern int AMMPI_VerboseErrors; /* set to non-zero for verbose error reporting */
extern int AMMPI_SilentMode; /* set to non-zero to silence any non-error output */

#ifdef __GNUC__
__attribute__((__format__ (__printf__, 1, 2)))
#endif
extern void AMMPI_FatalErr(const char *msg, ...);

/* define the communicator to be used as the basis for all
 * subsequent calls to AM_AllocateEndpoint(), which must be called 
 * collectively across the selected comm
 * the call is optional and the comm defaults to MPI_COMM_WORLD
 * the provided comm is only used to establish subcommunicators for library
 * communication, and is not otherwise directly utilized by AMMPI
 * endpoints may only map other endpoints created in the same collective call 
 * to AM_AllocateEndpoint()
 */
extern int AMMPI_SetEndpointCommunicator(void *ptr_to_MPI_Comm);

/* set the client callback fns to run before/after handler execution 
   (callback fns may _NOT_ make any AMMPI calls, directly or indirectly)
   set to NULL for none
*/
extern int AMMPI_SetHandlerCallbacks(ep_t ep, AMMPI_preHandlerCallback_t preHandlerCallback, 
                                              AMMPI_postHandlerCallback_t postHandlerCallback);

/* statistical collection */
extern int AMMPI_GetEndpointStatistics(ep_t ep, ammpi_stats_t *stats); /* get ep counters */
extern int AMMPI_ResetEndpointStatistics(ep_t ep); /* reset ep counters */
extern int AMMPI_AggregateStatistics(ammpi_stats_t *runningsum, ammpi_stats_t *newvalues); 
  /* aggregate statistics - augment running sum with the given values (fp is a FILE *) */
extern const char *AMMPI_DumpStatistics(void *fp, ammpi_stats_t *stats, int globalAnalysis); 
  /* output stats to fp (if non-null) in human-readable form.
   * return a pointer to the same output in an internal static buffer (rewritten on each call)
   * pass globalAnalysis non-zero if stats is a global agreggation across all nodes
   */
extern const ammpi_stats_t AMMPI_initial_stats; /* the "empty" values for counters */
/* ------------------------------------------------------------------------------------ */
/* AM-2 Entry Points */

/* strictly speaking, many of these AM entry points should be true-blue functions
   (so, for example, a user could create a function pointer to them)
   but that seems a silly justification, so we went with macros on many of them 
   in the interests of performance */

#ifdef AMMPI_COEXIST_WITH_AM
  /* allow linking with another library that also implements AM - rename entry points 
   * to use this option, it must be defined when building the AMMPI library _and_ the application using it
   * note this still does not allow the same .c file to use both AM implementations
   * (any given source file should #include at most one AM header file)
   */
  #define AM_Init                 AMMPI_Init
  #define AM_Terminate            AMMPI_Terminate
  #define AM_AllocateBundle       AMMPI_AllocateBundle
  #define AM_AllocateEndpoint     AMMPI_AllocateEndpoint
  #define AM_FreeBundle           AMMPI_FreeBundle
  #define AM_FreeEndpoint         AMMPI_FreeEndpoint
  #define AM_MoveEndpoint         AMMPI_MoveEndpoint
  #define AM_GetSeg               AMMPI_GetSeg
  #define AM_SetSeg               AMMPI_SetSeg
  #define AM_MaxSegLength         AMMPI_MaxSegLength
  #define AM_GetTag               AMMPI_GetTag
  #define AM_SetTag               AMMPI_SetTag
  #define AM_UnMap                AMMPI_UnMap
  #define AM_GetNumTranslations   AMMPI_GetNumTranslations
  #define AM_SetNumTranslations   AMMPI_SetNumTranslations
  #define AM_GetTranslationInuse  AMMPI_GetTranslationInuse
  #define AM_GetTranslationTag    AMMPI_GetTranslationTag
  #define AM_GetTranslationName   AMMPI_GetTranslationName
  #define AM_SetExpectedResources AMMPI_SetExpectedResources
  #define _AM_SetHandler          AMMPI_SetHandler
  #define _AM_SetHandlerAny       AMMPI_SetHandlerAny
  #define AM_GetEventMask         AMMPI_GetEventMask
  #define AM_SetEventMask         AMMPI_SetEventMask
  #define AM_WaitSema             AMMPI_WaitSema
  #define AM_GetSourceEndpoint    AMMPI_GetSourceEndpoint
  #define AM_GetDestEndpoint      AMMPI_GetDestEndpoint
  #define AM_GetMsgTag            AMMPI_GetMsgTag
  #define AM_Poll                 AMMPI_Poll
#endif

/* standardized AM-2 extensions */
#ifndef AMMPI
#define AMMPI 1
#endif

#define AMX_VerboseErrors         AMMPI_VerboseErrors
#define AMX_GetEndpointStatistics AMMPI_GetEndpointStatistics
#define AMX_DumpStatistics        AMMPI_DumpStatistics
#define AMX_AggregateStatistics   AMMPI_AggregateStatistics
#define AMX_initial_stats         AMMPI_initial_stats
#define amx_stats_t               ammpi_stats_t
#define amx_handler_fn_t          ammpi_handler_fn_t
#define AMX_FatalErr              AMMPI_FatalErr

#if !defined(AMMPI_DEBUG) && !defined(AMMPI_NDEBUG)
  #if defined(GASNET_DEBUG) || defined(AMX_DEBUG)
    #define AMMPI_DEBUG 1
  #elif defined(GASNET_NDEBUG) || defined(AMX_NDEBUG)
    #define AMMPI_NDEBUG 1
  #endif
#endif
#if defined(AMMPI_DEBUG) && !defined(AMMPI_NDEBUG)
  #undef AMMPI_DEBUG
  #define AMMPI_DEBUG 1
#elif !defined(AMMPI_DEBUG) && defined(AMMPI_NDEBUG)
  #undef AMMPI_NDEBUG
  #define AMMPI_NDEBUG 1
#else
  #error bad defns of AMMPI_DEBUG and AMMPI_NDEBUG
#endif

#undef AMX_DEBUG
#undef AMX_NDEBUG

#ifdef AMMPI_DEBUG
  #define AMX_DEBUG AMMPI_DEBUG
  #define AMMPI_DEBUG_CONFIG _DEBUG
#endif
#ifdef AMMPI_NDEBUG
  #define AMX_NDEBUG AMMPI_NDEBUG
  #define AMMPI_DEBUG_CONFIG _NDEBUG
#endif
#ifdef AMMPI_DEBUG_VERBOSE
  #define AMX_DEBUG_VERBOSE AMMPI_DEBUG_VERBOSE
#endif

#if defined(AMMPI_DEBUG) && (defined(__OPTIMIZE__) || defined(NDEBUG))
  #if !defined(AMMPI_ALLOW_OPTIMIZED_DEBUG) && !defined(GASNET_ALLOW_OPTIMIZED_DEBUG)
    #error Tried to compile AMMPI client code with optimization enabled but also AMMPI_DEBUG (which seriously hurts performance). Disable C and MPI_CC compiler optimization or reconfigure/rebuild without --enable-debug
  #endif
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

#undef AM_Init
#define AM_Init _CONCAT(AM_Init_AMMPI,AMMPI_DEBUG_CONFIG)

/* System parameters */
#define AM_MaxShort()   AMMPI_MAX_SHORT
#define AM_MaxMedium()  AMMPI_MAX_MEDIUM
#define AM_MaxLong()    AMMPI_MAX_LONG

#define AM_MaxNumHandlers()               AMMPI_MAX_NUMHANDLERS
#define AM_MaxNumTranslations(trans)      (*(trans) = AMMPI_MAX_NUMTRANSLATIONS,AM_OK)
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
/* use special hack in case en_t size is conservatively large */
extern int AMMPI_Map(ep_t ea, int index, en_t *name, tag_t tag);
extern int AMMPI_MapAny(ep_t ea, int *index, en_t *name, tag_t tag);
#define AM_Map(ea, index, name, tag)    AMMPI_Map((ea), (index), &(name), (tag))
#define AM_MapAny(ea, index, name, tag) AMMPI_Map((ea), (index), &(name), (tag))
extern int AM_UnMap(ep_t ea, int index);
extern int AM_GetTranslationInuse(ep_t ea, int i);
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag);
extern int AM_GetTranslationName(ep_t ea, int i, en_t *gan);
extern int AM_GetNumTranslations(ep_t ep, int *pntrans);
extern int AM_SetNumTranslations(ep_t ep, int ntrans);
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests);

/* Handler table */
extern int _AM_SetHandler(ep_t ea, handler_t handler, ammpi_handler_fn_t function);
#define AM_SetHandler(ea, handler, function) _AM_SetHandler((ea), (handler), (ammpi_handler_fn_t)(function))
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, ammpi_handler_fn_t function);
#define AM_SetHandlerAny(ea, handler, function) _AM_SetHandlerAny((ea), (handler), (ammpi_handler_fn_t)(function))
#define AM_GetNumHandlers(ep, pnhandlers)  \
  ((ep) ? ((*(pnhandlers) = AMMPI_MAX_NUMHANDLERS), AM_OK) : AM_ERR_BAD_ARG) : AM_ERR_BAD_ARG)
#define AM_SetNumHandlers(ep, nhandlers)  \
  ((ep) ? ((nhandlers) == AMMPI_MAX_NUMHANDLERS ? AM_OK : AM_ERR_RESOURCE)

/* Events */
extern int AM_GetEventMask(eb_t eb, int *mask);
extern int AM_SetEventMask(eb_t eb, int mask);
extern int AM_WaitSema(eb_t eb);


/* Message interrogation */
extern int AM_GetSourceEndpoint(void *token, en_t *gan);
extern int AM_GetDestEndpoint(void *token, ep_t *endp);
extern int AM_GetMsgTag(void *token, tag_t *tagp);
extern int AMMPI_GetSourceId(void *token, int *srcid);

/* Poll */
extern int AM_Poll(eb_t bundle);

/* Requests and Replies
   These six functions do all requests and replies.
   Macros below expand all the variants */

extern int AMMPI_Request(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                         int numargs, ...);
extern int AMMPI_RequestI (ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, ...);
extern int AMMPI_RequestXfer(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, ...);

extern int AMMPI_Reply(void *token, handler_t handler, 
                         int numargs, ...);
extern int AMMPI_ReplyI(void *token, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, ...);
extern int AMMPI_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, ...);

/* alternate forms that take va_list ptr to support GASNet */
extern int AMMPI_RequestVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMMPI_RequestIVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr);
extern int AMMPI_RequestXferVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, va_list argptr);

extern int AMMPI_ReplyVA(void *token, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMMPI_ReplyIVA(void *token, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr);
extern int AMMPI_ReplyXferVA(void *token, handler_t handler, 
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
   AMMPI_Request(ep, destep, hnum, 0)
#define AM_Request1(ep, destep, hnum, a0) \
   AMMPI_Request(ep, destep, hnum, 1, (int32_t)a0)
#define AM_Request2(ep, destep, hnum, a0, a1) \
   AMMPI_Request(ep, destep, hnum, 2, (int32_t)a0, (int32_t)a1)
#define AM_Request3(ep, destep, hnum, a0, a1, a2) \
   AMMPI_Request(ep, destep, hnum, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_Request4(ep, destep, hnum, a0, a1, a2, a3) \
   AMMPI_Request(ep, destep, hnum, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_Request5(ep, destep, hnum, a0, a1, a2, a3, a4) \
   AMMPI_Request(ep, destep, hnum, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_Request6(ep, destep, hnum, a0, a1, a2, a3, a4, a5) \
   AMMPI_Request(ep, destep, hnum, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_Request7(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_Request(ep, destep, hnum, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_Request8(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_Request(ep, destep, hnum, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_Request9(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_Request(ep, destep, hnum, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_Request10(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_Request(ep, destep, hnum, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_Request11(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_Request(ep, destep, hnum, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_Request12(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_Request(ep, destep, hnum, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_Request13(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_Request(ep, destep, hnum, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_Request14(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_Request(ep, destep, hnum, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_Request15(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_Request(ep, destep, hnum, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_Request16(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_Request(ep, destep, hnum, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestI0(ep, destep, hnum, sa, cnt) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 0)
#define AM_RequestI1(ep, destep, hnum, sa, cnt, a0) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 1, (int32_t)a0)
#define AM_RequestI2(ep, destep, hnum, sa, cnt, a0, a1) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestI3(ep, destep, hnum, sa, cnt, a0, a1, a2) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestI4(ep, destep, hnum, sa, cnt, a0, a1, a2, a3) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestI5(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestI6(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestI7(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestI8(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestI9(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestI10(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestI11(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestI12(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestI13(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestI14(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestI15(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestI16(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_RequestI(ep, destep, hnum, sa, cnt, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestXfer0(ep, destep, desto, hnum, sa, cnt) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 0)
#define AM_RequestXfer1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 1, (int32_t)a0)
#define AM_RequestXfer2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestXfer3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestXfer4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestXfer5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestXfer6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestXfer7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestXfer8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestXfer9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestXfer10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestXfer11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestXfer12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestXfer13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestXfer14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestXfer15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestXfer16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_RequestXferAsync0(ep, destep, desto, hnum, sa, cnt) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 0)
#define AM_RequestXferAsync1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 1, (int32_t)a0)
#define AM_RequestXferAsync2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 2, (int32_t)a0, (int32_t)a1)
#define AM_RequestXferAsync3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_RequestXferAsync4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_RequestXferAsync5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_RequestXferAsync6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_RequestXferAsync7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_RequestXferAsync8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_RequestXferAsync9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_RequestXferAsync10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_RequestXferAsync11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_RequestXferAsync12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_RequestXferAsync13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_RequestXferAsync14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_RequestXferAsync15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_RequestXferAsync16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_Reply0(token, hnum) \
   AMMPI_Reply(token, hnum, 0)
#define AM_Reply1(token, hnum, a0) \
   AMMPI_Reply(token, hnum, 1, (int32_t)a0)
#define AM_Reply2(token, hnum, a0, a1) \
   AMMPI_Reply(token, hnum, 2, (int32_t)a0, (int32_t)a1)
#define AM_Reply3(token, hnum, a0, a1, a2) \
   AMMPI_Reply(token, hnum, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_Reply4(token, hnum, a0, a1, a2, a3) \
   AMMPI_Reply(token, hnum, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_Reply5(token, hnum, a0, a1, a2, a3, a4) \
   AMMPI_Reply(token, hnum, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_Reply6(token, hnum, a0, a1, a2, a3, a4, a5) \
   AMMPI_Reply(token, hnum, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_Reply7(token, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_Reply(token, hnum, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_Reply8(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_Reply(token, hnum, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_Reply9(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_Reply(token, hnum, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_Reply10(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_Reply(token, hnum, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_Reply11(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_Reply(token, hnum, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_Reply12(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_Reply(token, hnum, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_Reply13(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_Reply(token, hnum, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_Reply14(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_Reply(token, hnum, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_Reply15(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_Reply(token, hnum, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_Reply16(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_Reply(token, hnum, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_ReplyI0(token, hnum, sa, cnt) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 0)
#define AM_ReplyI1(token, hnum, sa, cnt, a0) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 1, (int32_t)a0)
#define AM_ReplyI2(token, hnum, sa, cnt, a0, a1) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 2, (int32_t)a0, (int32_t)a1)
#define AM_ReplyI3(token, hnum, sa, cnt, a0, a1, a2) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_ReplyI4(token, hnum, sa, cnt, a0, a1, a2, a3) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_ReplyI5(token, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_ReplyI6(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_ReplyI7(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_ReplyI8(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_ReplyI9(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_ReplyI10(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_ReplyI11(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_ReplyI12(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_ReplyI13(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_ReplyI14(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_ReplyI15(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_ReplyI16(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_ReplyI(token, hnum, sa, cnt, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)

#define AM_ReplyXfer0(token, desto, hnum, sa, cnt) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 0)
#define AM_ReplyXfer1(token, desto, hnum, sa, cnt, a0) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 1, (int32_t)a0)
#define AM_ReplyXfer2(token, desto, hnum, sa, cnt, a0, a1) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 2, (int32_t)a0, (int32_t)a1)
#define AM_ReplyXfer3(token, desto, hnum, sa, cnt, a0, a1, a2) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 3, (int32_t)a0, (int32_t)a1, (int32_t)a2)
#define AM_ReplyXfer4(token, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 4, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3)
#define AM_ReplyXfer5(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 5, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4)
#define AM_ReplyXfer6(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 6, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5)
#define AM_ReplyXfer7(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 7, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6)
#define AM_ReplyXfer8(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 8, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7)
#define AM_ReplyXfer9(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 9, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8)
#define AM_ReplyXfer10(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 10, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9)
#define AM_ReplyXfer11(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 11, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10)
#define AM_ReplyXfer12(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 12, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11)
#define AM_ReplyXfer13(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 13, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12)
#define AM_ReplyXfer14(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 14, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13)
#define AM_ReplyXfer15(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 15, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14)
#define AM_ReplyXfer16(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMMPI_ReplyXfer(token, hnum, sa, cnt, desto, 16, (int32_t)a0, (int32_t)a1, (int32_t)a2, (int32_t)a3, (int32_t)a4, (int32_t)a5, (int32_t)a6, (int32_t)a7, (int32_t)a8, (int32_t)a9, (int32_t)a10, (int32_t)a11, (int32_t)a12, (int32_t)a13, (int32_t)a14, (int32_t)a15)


AMMPI_END_EXTERNC

#endif
