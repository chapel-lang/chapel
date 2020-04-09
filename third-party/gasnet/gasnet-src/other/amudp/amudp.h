/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp.h $
 * Description: AMUDP Header
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef __AMUDP_H
#define __AMUDP_H

#include <stdarg.h>
#include <stddef.h>
#include <netinet/in.h> /* for sockaddr_in */

#include <portable_inttypes.h>

#include <amudp_const.h>

/* naming policy:
  AM-defined things start with AM_
  internal things start with amudp_ or AMUDP_
  */

#ifdef __cplusplus
  #define AMUDP_BEGIN_EXTERNC extern "C" {
  #define AMUDP_END_EXTERNC }
  #define AMUDP_EXTERNC extern "C"
#else
  #define AMUDP_BEGIN_EXTERNC 
  #define AMUDP_END_EXTERNC 
  #define AMUDP_EXTERNC extern
#endif

AMUDP_BEGIN_EXTERNC

/* ------------------------------------------------------------------------------------ */
/* User-visible types */

/* Endpoint tag */
typedef uint64_t tag_t;

/* Handler index */
typedef uint8_t handler_t;
#define AMUDP_BADHANDLERVAL(h) (0)
/* #define AMUDP_BADHANDLERVAL(h) (h < 0 || h >= AMUDP_MAX_NUMHANDLERS) */

/* Endpoint name */
typedef uint32_t amudp_node_t;
typedef struct sockaddr_in en_t;

/* CPU ticks */
#define _AMX_TICK_T
typedef uint64_t amx_tick_t; 

typedef enum {
  amudp_Short=0,
  amudp_Medium=1,
  amudp_Long=2,
  amudp_NumCategories=3
} amudp_category_t;

/* statistical collection 
 *  changes here need to also be reflected in the initialization vector AMUDP_initial_stats
 */
typedef struct {
  uint64_t RequestsSent[amudp_NumCategories]; 
  uint64_t RepliesSent[amudp_NumCategories];
  uint64_t RequestsRetransmitted[amudp_NumCategories];
  uint64_t RepliesRetransmitted[amudp_NumCategories];
  uint64_t RepliesSquashed[amudp_NumCategories];
  uint64_t RequestsReceived[amudp_NumCategories];   /*  includes retransmits */
  uint64_t RepliesReceived[amudp_NumCategories];    /*  includes retransmits */
  uint64_t ReturnedMessages;
  uint64_t OutOfOrderRequests;
  uint64_t OutOfOrderReplies;
  amx_tick_t RequestMinLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  amx_tick_t RequestMaxLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  amx_tick_t RequestSumLatency;  /* in CPU ticks, only if AMUDP_COLLECT_LATENCY_STATS */
  uint64_t RequestDataBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t ReplyDataBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t RequestTotalBytesSent[amudp_NumCategories];  /* total of args + data payload */
  uint64_t ReplyTotalBytesSent[amudp_NumCategories];  /* total of args,payload and overhead */
  uint64_t TotalBytesSent; /* total user level packet sizes for all req/rep */
} amudp_stats_t;

typedef void (*amudp_handler_fn_t)();  /* prototype for handler function */

/* Endpoint bundle object */
struct amudp_eb;
typedef struct amudp_eb *eb_t;

/* Endpoint object */
struct amudp_ep;
typedef struct amudp_ep *ep_t;

/*
 * Op codes for the AM error handler (opcode).
 */
typedef int op_t;

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


/* ------------------------------------------------------------------------------------ */
/* AMUDP-specific user entry points */

// programmatic tuning knobs
extern int AMUDP_PoliteSync; /* set to non-zero for polite blocking while awaiting send resources */
extern int AMX_VerboseErrors; /* set to non-zero for verbose error reporting */
extern int AMX_SilentMode; /* set to non-zero to silence any non-error output */
extern const char *AMX_ProcessLabel; /* human-readable label for this process */

#ifdef __GNUC__
__attribute__((__format__ (__printf__, 1, 2)))
#endif
extern void AMX_FatalErr(const char *msg, ...);

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
typedef void (*AMUDP_preHandlerCallback_t)(amudp_category_t cat, int isReq, int handlerId, void *token, 
                                         void *buf, size_t nbytes, int numargs, uint32_t *args);
typedef void (*AMUDP_postHandlerCallback_t)(amudp_category_t cat, int isReq);
extern int AMUDP_SetHandlerCallbacks(ep_t ep, AMUDP_preHandlerCallback_t preHandlerCallback, 
                                              AMUDP_postHandlerCallback_t postHandlerCallback);

extern int AMUDP_SPMDHandleControlTraffic(int *controlMessagesServiced);

/* ------------------------------------------------------------------------------------ */
/* AM-2 Entry Points */

/* strictly speaking, many of these AM entry points should be true-blue functions
   (so, for example, a user could create a function pointer to them)
   but that seems a silly justification, so we went with macros on many of them 
   in the interests of performance */

#ifdef AMUDP_COEXIST_WITH_AM
  /* allow linking with another library that also implements AM - rename entry points 
   * note this still does not allow the same .c file to use both AM implementations
   * (any given  file should #include at most one AM header file)
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
  #define AM_GetNumTranslations   AMUDP_GetNumTranslations
  #define AM_SetNumTranslations   AMUDP_SetNumTranslations
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
#define AMX_SetTranslationTag     AMUDP_SetTranslationTag
#define AMX_GetEndpointStatistics AMUDP_GetEndpointStatistics
#define AMX_DumpStatistics        AMUDP_DumpStatistics
#define AMX_AggregateStatistics   AMUDP_AggregateStatistics
#define AMX_initial_stats         AMUDP_initial_stats
#define amx_stats_t               amudp_stats_t
#define amx_handler_fn_t          amudp_handler_fn_t
#define AMX_GetSourceId           AMUDP_GetSourceId
#define AMX_enEqual               AMUDP_enEqual

#undef AM_Init
#define AM_Init AMX_CONCAT(AM_Init_AMUDP,AMUDP_DEBUG_CONFIG)

/* System parameters */
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
extern int AM_GetNumTranslations(ep_t ep, int *pntrans);
extern int AM_SetNumTranslations(ep_t ep, int ntrans);
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests);
extern int AMUDP_SetTranslationTag(ep_t ea, int index, tag_t tag); /* extension: legal after AM_SetExpectedResources */

/* Handler table */
extern int _AM_SetHandler(ep_t ea, handler_t handler, amudp_handler_fn_t function);
#define AM_SetHandler(ea, handler, function) _AM_SetHandler((ea), (handler), (amudp_handler_fn_t)(function)) 
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, amudp_handler_fn_t function);
#define AM_SetHandlerAny(ea, handler, function) _AM_SetHandlerAny((ea), (handler), (amudp_handler_fn_t)(function))

/* Events */
extern int AM_GetEventMask(eb_t eb, int *mask);
extern int AM_SetEventMask(eb_t eb, int mask);
extern int AM_WaitSema(eb_t eb);


/* Message interrogation */
extern int AM_GetSourceEndpoint(void *token, en_t *gan);
extern int AM_GetDestEndpoint(void *token, ep_t *endp);
extern int AM_GetMsgTag(void *token, tag_t *tagp);
extern int AMUDP_GetSourceId(void *token, int *srcid); /* srcid retrieves a compressed id */
extern int AMUDP_enEqual(en_t en1, en_t en2);


/* Poll */
extern int AM_Poll(eb_t bundle);

/* Requests and Replies
   These six functions do all requests and replies.
   Macros below expand all the variants */

extern int AMUDP_Request(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                         int numargs, ...);
extern int AMUDP_RequestI (ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...);
extern int AMUDP_RequestXfer(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, ...);

extern int AMUDP_Reply(void *token, handler_t handler, 
                         int numargs, ...);
extern int AMUDP_ReplyI(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...);
extern int AMUDP_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, ...);

/* alternate forms that take va_list ptr to support GASNet */
extern int AMUDP_RequestVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMUDP_RequestIVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr);
extern int AMUDP_RequestXferVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async,
                          int numargs, va_list argptr);

extern int AMUDP_ReplyVA(void *token, handler_t handler, 
                         int numargs, va_list argptr);
extern int AMUDP_ReplyIVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr);
extern int AMUDP_ReplyXferVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr);

/* we cast to int32_t here to simulate function call - AM says these functions take 32-bit int args, 
 * so this cast accomplishes the conversion to integral type for floating-point actuals, and
 * the truncation which might happen to long integer actuals
 * note the C compiler will subsequently apply default argument promotion to these arguments 
 * (because these arguments fall within the ellipses (...) of the called functions)
 * which means they'll subsequently be promoted to int (which may differ from int32_t)
 */

#define AM_Request0(ep, destep, hnum) \
   AMUDP_Request(ep, destep, hnum, 0)
#define AM_Request1(ep, destep, hnum, a0) \
   AMUDP_Request(ep, destep, hnum, 1, (int32_t)(a0))
#define AM_Request2(ep, destep, hnum, a0, a1) \
   AMUDP_Request(ep, destep, hnum, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_Request3(ep, destep, hnum, a0, a1, a2) \
   AMUDP_Request(ep, destep, hnum, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_Request4(ep, destep, hnum, a0, a1, a2, a3) \
   AMUDP_Request(ep, destep, hnum, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_Request5(ep, destep, hnum, a0, a1, a2, a3, a4) \
   AMUDP_Request(ep, destep, hnum, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_Request6(ep, destep, hnum, a0, a1, a2, a3, a4, a5) \
   AMUDP_Request(ep, destep, hnum, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_Request7(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_Request(ep, destep, hnum, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_Request8(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_Request(ep, destep, hnum, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_Request9(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_Request(ep, destep, hnum, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_Request10(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_Request(ep, destep, hnum, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_Request11(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_Request(ep, destep, hnum, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_Request12(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_Request(ep, destep, hnum, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_Request13(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_Request(ep, destep, hnum, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_Request14(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_Request(ep, destep, hnum, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_Request15(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_Request(ep, destep, hnum, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_Request16(ep, destep, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_Request(ep, destep, hnum, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_RequestI0(ep, destep, hnum, sa, cnt) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 0)
#define AM_RequestI1(ep, destep, hnum, sa, cnt, a0) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 1, (int32_t)(a0))
#define AM_RequestI2(ep, destep, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_RequestI3(ep, destep, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_RequestI4(ep, destep, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_RequestI5(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_RequestI6(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_RequestI7(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_RequestI8(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_RequestI9(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_RequestI10(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_RequestI11(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_RequestI12(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_RequestI13(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_RequestI14(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_RequestI15(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_RequestI16(ep, destep, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestI(ep, destep, hnum, sa, cnt, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_RequestXfer0(ep, destep, desto, hnum, sa, cnt) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 0)
#define AM_RequestXfer1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 1, (int32_t)(a0))
#define AM_RequestXfer2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_RequestXfer3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_RequestXfer4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_RequestXfer5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_RequestXfer6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_RequestXfer7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_RequestXfer8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_RequestXfer9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_RequestXfer10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_RequestXfer11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_RequestXfer12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_RequestXfer13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_RequestXfer14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_RequestXfer15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_RequestXfer16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 0, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_RequestXferAsync0(ep, destep, desto, hnum, sa, cnt) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 0)
#define AM_RequestXferAsync1(ep, destep, desto, hnum, sa, cnt, a0) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 1, (int32_t)(a0))
#define AM_RequestXferAsync2(ep, destep, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_RequestXferAsync3(ep, destep, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_RequestXferAsync4(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_RequestXferAsync5(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_RequestXferAsync6(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_RequestXferAsync7(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_RequestXferAsync8(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_RequestXferAsync9(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_RequestXferAsync10(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_RequestXferAsync11(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_RequestXferAsync12(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_RequestXferAsync13(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_RequestXferAsync14(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_RequestXferAsync15(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_RequestXferAsync16(ep, destep, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_RequestXfer(ep, destep, hnum, sa, cnt, desto, 1, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_Reply0(token, hnum) \
   AMUDP_Reply(token, hnum, 0)
#define AM_Reply1(token, hnum, a0) \
   AMUDP_Reply(token, hnum, 1, (int32_t)(a0))
#define AM_Reply2(token, hnum, a0, a1) \
   AMUDP_Reply(token, hnum, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_Reply3(token, hnum, a0, a1, a2) \
   AMUDP_Reply(token, hnum, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_Reply4(token, hnum, a0, a1, a2, a3) \
   AMUDP_Reply(token, hnum, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_Reply5(token, hnum, a0, a1, a2, a3, a4) \
   AMUDP_Reply(token, hnum, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_Reply6(token, hnum, a0, a1, a2, a3, a4, a5) \
   AMUDP_Reply(token, hnum, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_Reply7(token, hnum, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_Reply(token, hnum, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_Reply8(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_Reply(token, hnum, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_Reply9(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_Reply(token, hnum, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_Reply10(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_Reply(token, hnum, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_Reply11(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_Reply(token, hnum, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_Reply12(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_Reply(token, hnum, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_Reply13(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_Reply(token, hnum, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_Reply14(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_Reply(token, hnum, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_Reply15(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_Reply(token, hnum, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_Reply16(token, hnum, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_Reply(token, hnum, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_ReplyI0(token, hnum, sa, cnt) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 0)
#define AM_ReplyI1(token, hnum, sa, cnt, a0) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 1, (int32_t)(a0))
#define AM_ReplyI2(token, hnum, sa, cnt, a0, a1) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_ReplyI3(token, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_ReplyI4(token, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_ReplyI5(token, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_ReplyI6(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_ReplyI7(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_ReplyI8(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_ReplyI9(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_ReplyI10(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_ReplyI11(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_ReplyI12(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_ReplyI13(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_ReplyI14(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_ReplyI15(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_ReplyI16(token, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_ReplyI(token, hnum, sa, cnt, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))

#define AM_ReplyXfer0(token, desto, hnum, sa, cnt) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 0)
#define AM_ReplyXfer1(token, desto, hnum, sa, cnt, a0) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 1, (int32_t)(a0))
#define AM_ReplyXfer2(token, desto, hnum, sa, cnt, a0, a1) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 2, (int32_t)(a0), (int32_t)(a1))
#define AM_ReplyXfer3(token, desto, hnum, sa, cnt, a0, a1, a2) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 3, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2))
#define AM_ReplyXfer4(token, desto, hnum, sa, cnt, a0, a1, a2, a3) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 4, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3))
#define AM_ReplyXfer5(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 5, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4))
#define AM_ReplyXfer6(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 6, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5))
#define AM_ReplyXfer7(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 7, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6))
#define AM_ReplyXfer8(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 8, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7))
#define AM_ReplyXfer9(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 9, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8))
#define AM_ReplyXfer10(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 10, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9))
#define AM_ReplyXfer11(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 11, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10))
#define AM_ReplyXfer12(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 12, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11))
#define AM_ReplyXfer13(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 13, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12))
#define AM_ReplyXfer14(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 14, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13))
#define AM_ReplyXfer15(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 15, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14))
#define AM_ReplyXfer16(token, desto, hnum, sa, cnt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15) \
   AMUDP_ReplyXfer(token, hnum, sa, cnt, desto, 16, (int32_t)(a0), (int32_t)(a1), (int32_t)(a2), (int32_t)(a3), (int32_t)(a4), (int32_t)(a5), (int32_t)(a6), (int32_t)(a7), (int32_t)(a8), (int32_t)(a9), (int32_t)(a10), (int32_t)(a11), (int32_t)(a12), (int32_t)(a13), (int32_t)(a14), (int32_t)(a15))


AMUDP_END_EXTERNC

#endif
