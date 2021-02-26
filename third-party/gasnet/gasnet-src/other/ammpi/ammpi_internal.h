/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/ammpi/ammpi_internal.h $
 * Description: AMMPI internal header file
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef _AMMPI_INTERNAL_H
#define _AMMPI_INTERNAL_H

#include <amx_internal_fwd.h>

#include <mpi.h>

#include <ammpi.h>

#include <amx_internal.h>

/* ------------------------------------------------------------------------------------ */
/* AMMPI system configuration parameters */

#ifndef AMMPI_MAX_RECVMSGS_PER_POLL
#define AMMPI_MAX_RECVMSGS_PER_POLL 10  /* max number of waiting messages serviced per poll (-1 for unlimited) */
#endif
#ifndef AMMPI_INITIAL_NUMENDPOINTS
#define AMMPI_INITIAL_NUMENDPOINTS  1   /* initial size of bundle endpoint table */
#endif
#ifndef AMMPI_DEFAULT_NETWORKDEPTH
#define AMMPI_DEFAULT_NETWORKDEPTH  4  /* default depth if none specified */
#endif
#ifndef AMMPI_PREPOST_RECVS
#define AMMPI_PREPOST_RECVS         1   /* pre-post non-blocking MPI recv's */
#endif
#ifndef AMMPI_RECV_REPOST_SLACK
#define AMMPI_RECV_REPOST_SLACK     1   /* number of recv operations to lazily re-post */
#endif
#ifndef AMMPI_SEPARATE_TEST
#define AMMPI_SEPARATE_TEST         1   /* issue separate MPI_Test calls in the common case, instead of a single MPI_TestAny */
#endif
#ifndef AMMPI_SEPARATE_TEST_BOUNCE
#define AMMPI_SEPARATE_TEST_BOUNCE  1   /* same as AMMPI_SEPARATE_TEST, but alternate pools */
#endif
#ifndef AMMPI_NONBLOCKING_SENDS
#define AMMPI_NONBLOCKING_SENDS     1   /* use non-blocking MPI send's */
#endif
#ifndef AMMPI_SEND_EARLYCOMPLETE
#define AMMPI_SEND_EARLYCOMPLETE    2   /* num outstanding send ops that initiates early completion attempt (0==never) */
#endif
#ifndef AMMPI_LINEAR_SEND_COMPLETE
#define AMMPI_LINEAR_SEND_COMPLETE  0   /* use linear algorithm to complete sends */
#endif
#ifndef AMMPI_MPIIRECV_ORDERING_BUGCHECK
  #if PLATFORM_OS_AIX
    /* some MPI implementations intermittently fail to correctly maintain irecv ordering required by MPI spec
       number is how often to check for this bug */
    #define AMMPI_MPIIRECV_ORDERING_BUGCHECK 100
  #else
    #define AMMPI_MPIIRECV_ORDERING_BUGCHECK 0
  #endif
#endif
#if AMMPI_MPIIRECV_ORDERING_BUGCHECK && AMMPI_RECV_REPOST_SLACK
  /* AMMPI_RECV_REPOST_SLACK incompatible with AMMPI_MPIIRECV_ORDERING_BUGCHECK */
  #undef AMMPI_RECV_REPOST_SLACK
  #define AMMPI_RECV_REPOST_SLACK 0
#endif
#ifndef AMMPI_VERIFY_MPI_ORDERING
#define AMMPI_VERIFY_MPI_ORDERING  0 /* debugging aid for MPI implementations (not for general use) */
#endif

#if AMMPI_NONBLOCKING_SENDS
#define AMMPI_SENDBUFFER_SZ         2*AMMPI_MAX_NETWORK_MSG /* size of MPI send buffer (used for rejections) */
#else
#define AMMPI_SENDBUFFER_SZ         1048576 /* size of MPI send buffer */
#endif
#ifdef AMMPI_DISABLE_AMTAGS 
  /* disable the use of the AM-2.0 message tags
     saves 8 bytes of AM header on the wire */
  #define AMMPI_USE_AMTAGS 0
#else
  #define AMMPI_USE_AMTAGS 1
#endif
#ifndef AMMPI_MAX_NETWORKDEPTH
#define AMMPI_MAX_NETWORKDEPTH     (1024*1024) /* max depth we ever allow user to ask for */
#endif
#ifndef AMMPI_BUF_ALIGN
#define AMMPI_BUF_ALIGN 128 /* alignment to use for buffers - optimized for cache lines (min is 8) */
#endif
#ifndef AMMPI_FLOW_CONTROL
#define AMMPI_FLOW_CONTROL 1 /* use token-based flow control to limit unexpected MPI messages */
#endif
#ifndef AMMPI_DEFAULT_SYNCSEND_THRESH  
  /* size threshhold above which to use synchronous non-blocking MPI send's 
   * this can be used in lieu of AMMPI_FLOW_CONTROL as an alternate 
   * (and less effective) flow control mechanism for MPI implementations lacking
   * a spec-compliant backpressure mechanism
   */
  #define AMMPI_DEFAULT_SYNCSEND_THRESH -1 /* -1 == never use sync sends */
#endif
/* AMMPI-SPMD system configuration parameters */
#ifndef AMMPI_BLOCKING_SPMD_BARRIER
#define AMMPI_BLOCKING_SPMD_BARRIER   1   /* use blocking AM calls in SPMDBarrier() */
#endif

#ifndef AMMPI_COLLECT_STATS
#define AMMPI_COLLECT_STATS           1 /* collect AM messaging layer statistics */
#endif

#ifndef AMMPI_COLLECT_LATENCY_STATS
#define AMMPI_COLLECT_LATENCY_STATS   0 /* not yet implemented */
#endif

#ifndef AMMPI_REPLYBUF_POOL_GROWTHFACTOR
#define AMMPI_REPLYBUF_POOL_GROWTHFACTOR 1.5
#endif

/* ------------------------------------------------------------------------------------ */

AMMPI_BEGIN_EXTERNC

#if AMMPI_COLLECT_STATS
  #define AMMPI_STATS(stmt) stmt
#else
  #define AMMPI_STATS(stmt) 
#endif

/* ------------------------------------------------------------------------------------ */

/* Internal types */

/* message flags */
 /* 0-1: category
  * 2:   request vs. reply 
  * 3:   sequence number
  * 4-7: numargs
  */
typedef unsigned char ammpi_flag_t;

#define AMMPI_MSG_SETFLAGS(pmsg, isreq, cat, numargs) \
  ((pmsg)->flags = (ammpi_flag_t) (                   \
                   (((numargs) & 0x1F) << 3)           \
                 | (((isreq) & 0x1) << 2)             \
                 |  ((cat) & 0x3)                     \
                   ))
#define AMMPI_MSG_NUMARGS(pmsg)   ( ( ((unsigned char)(pmsg)->flags) >> 3 ) & 0x1F)
#define AMMPI_MSG_ISREQUEST(pmsg) (!!(((unsigned char)(pmsg)->flags) & 0x4))
#define AMMPI_MSG_CATEGORY(pmsg)  ((ammpi_category_t)((pmsg)->flags & 0x3))

/* active message header & meta info fields */
typedef struct {
  #if AMMPI_VERIFY_MPI_ORDERING  
    uint64_t      seqnum;
  #endif

  #if AMMPI_USE_AMTAGS
    tag_t         tag;
  #endif

  ammpi_flag_t  flags;
  uint8_t       systemMessageType;
  uint8_t       systemMessageArg;
  handler_t     handlerId;

  uint16_t      nBytes;     /* TODO: remove for short */
  uint16_t      _reserved;
  uintptr_t	destOffset; /* TODO: remove for short/med */

} ammpi_msg_t;

/* non-transmitted ammpi buffer bookkeeping info -
 * this data must be kept to a bare minimum because it constrains packet size 
 */
typedef struct {
  int8_t handlerRunning;
  int8_t replyIssued;
  ammpi_node_t sourceId;  /* 0-based endpoint id of remote */
  struct ammpi_ep *dest;  /* ep_t of endpoint that received this message */
  en_t sourceAddr;        /* address of remote */
} ammpi_bufstatus_t;

/* active message buffer, including message and space for data payload */
typedef struct ammpi_buf {

  ammpi_msg_t	Msg;
  uint8_t     _Data[(4*AMMPI_MAX_SHORT)+AMMPI_MAX_LONG]; /* holds args and data */

  /* received requests & replies only */
  ammpi_bufstatus_t status;

  /* padding to enforce sizeof(ammpi_buf_t)%AMMPI_BUF_ALIGN == 0 */
  #define __EXP ((AMMPI_BUF_ALIGN - \
                  (sizeof(ammpi_msg_t) + \
                   (4*AMMPI_MAX_SHORT)+AMMPI_MAX_LONG + \
                   sizeof(ammpi_bufstatus_t)) % AMMPI_BUF_ALIGN) \
                 % AMMPI_BUF_ALIGN)
  #ifdef __GNUC__
    uint8_t _pad[__EXP];
  #else
    /* non-GNU compilers may choke on 0-length array in a struct */
    uint8_t _pad[__EXP == 0 ? AMMPI_BUF_ALIGN : __EXP];
  #endif
  #undef __EXP
} ammpi_buf_t;

#define AMMPI_MIN_NETWORK_MSG ((int)(uintptr_t)&((ammpi_buf_t *)NULL)->_Data[0])
#define AMMPI_MAX_SMALL_NETWORK_MSG ((int)(uintptr_t)&((ammpi_buf_t *)NULL)->_Data[(4*AMMPI_MAX_SHORT)])
#define AMMPI_MAX_NETWORK_MSG ((int)(uintptr_t)&((ammpi_buf_t *)NULL)->_Data[(4*AMMPI_MAX_SHORT)+AMMPI_MAX_LONG])

#define AMMPI_SMALL_SENDBUF_SZ AMX_ALIGNUP(AMMPI_MAX_SMALL_NETWORK_MSG, AMMPI_BUF_ALIGN)

/* ------------------------------------------------------------------------------------ */
/* Complex user-visible types */

typedef struct {
  tag_t tag;  /*  remote tag */
  char inuse; /*  entry in use */
  ammpi_node_t id; /*  id in compressed table */
  en_t name;  /*  remote address */
} ammpi_translation_t;

typedef struct { /* gives us a compacted version of the translation table */
  en_t      remoteName;  
  #if AMMPI_USE_AMTAGS
    tag_t     tag;
  #endif
  #if AMMPI_FLOW_CONTROL
    uint32_t  tokens_out; /* remaining tokens for sends to this host */
    uint32_t  tokens_in;  /* coalesced tokens recieved from this host */
  #endif
  #if AMMPI_VERIFY_MPI_ORDERING /* debugging aid for MPI implementations */
    struct {
      uint64_t  in;  /* last seqnum recvd from this host */
      uint64_t  out; /* last seqnum sent to this host */
    } seqnum[2]; /* reply, request */
  #endif
} ammpi_perproc_info_t;

typedef struct {
  MPI_Request* txHandle; /* send buffer handles */
  ammpi_buf_t** txBuf;   /* send buffer ptrs */
  int numBufs;
  int numActive;
  int bufSize;

  int numBlocks; /* buffer memory management */
  char **memBlocks;

  int *tmpIndexArray; /* temporaries used during MPI interface */
  MPI_Status *tmpStatusArray;
} ammpi_sendbuffer_pool_t;

typedef struct {
  MPI_Comm *mpicomm;

  /* send buffer tables (for AMMPI_NONBLOCKING_SENDS) */
  ammpi_sendbuffer_pool_t sendPool_small;
  ammpi_sendbuffer_pool_t sendPool_large;

  /* recv buffer tables (for AMMPI_PREPOST_RECVS) */
  MPI_Request* rxHandle;
  ammpi_buf_t* rxBuf;     /* recv buffers (aligned) */
  uint32_t rxNumBufs;     /* number of recv buffers in each pool */
  int rxCurr;             /* the oldest recv buffer index */
  #if AMMPI_RECV_REPOST_SLACK
    int rxPostSlack;      /* number of recv reposts lazily delayed */
    int rxPostSlackMax;   /* max number of recv reposts lazily delayed */
  #endif
} ammpi_virtual_network_t;

/* Endpoint bundle object */
struct ammpi_eb {
  struct ammpi_ep **endpoints;   /* dynamically-grown array of endpoints in bundle */
  int	  n_endpoints;           /* Number of EPs in the bundle */
  int	  cursize;               /* size of the array */
  uint8_t event_mask;            /* Event Mask for blocking ops */
};

/* Endpoint object */
struct ammpi_ep {
  en_t name;            /* Endpoint name */
  tag_t tag;            /* current tag */
  eb_t eb;              /* Bundle of endpoint */

  void *segAddr;          /* Start address of EP VM segment */
  uintptr_t segLength;    /* Length of EP VM segment    */

  ammpi_translation_t *translation;  /* translation table */
  ammpi_node_t        translationsz; /* current size of table */
  ammpi_handler_fn_t  handler[AMMPI_MAX_NUMHANDLERS]; /* handler table */

  ammpi_handler_fn_t controlMessageHandler;

  /* internal structures */

  ammpi_node_t totalP; /* the number of endpoints we communicate with - also number of translations currently in use */
  int depth;           /* network depth, -1 until AM_SetExpectedResources is called */

  #if AMMPI_FLOW_CONTROL
    uint32_t tokens_perhost;
    uint32_t tokens_slack;
  #endif

  ammpi_perproc_info_t *perProcInfo; 

  ammpi_stats_t stats;  /* statistical collection */

  AMMPI_preHandlerCallback_t preHandlerCallback; /* client hooks for statistical/debugging usage */
  AMMPI_postHandlerCallback_t postHandlerCallback;

  ammpi_buf_t* rxBuf_alloc; /* recv buffers (mallocated ptr) */
  MPI_Request* rxHandle_both; /* all the recv handles, reply then request */

  ammpi_virtual_network_t Req; /* requests */
  ammpi_virtual_network_t Rep; /* replies */
};

/*------------------------------------------------------------------------------------
 * Error reporting
 *------------------------------------------------------------------------------------ */
static const char *MPI_ErrorName(int errval) {
  const char *code = NULL;
  char systemErrDesc[MPI_MAX_ERROR_STRING+10];
  int len = MPI_MAX_ERROR_STRING;
  static char msg[MPI_MAX_ERROR_STRING+100];
  switch (errval) {
    case MPI_ERR_BUFFER:    code = "MPI_ERR_BUFFER"; break;     
    case MPI_ERR_COUNT:     code = "MPI_ERR_COUNT"; break;     
    case MPI_ERR_TYPE:      code = "MPI_ERR_TYPE"; break;      
    case MPI_ERR_TAG:       code = "MPI_ERR_TAG"; break;      
    case MPI_ERR_COMM:      code = "MPI_ERR_COMM"; break;      
    case MPI_ERR_RANK:      code = "MPI_ERR_RANK"; break;      
    case MPI_ERR_REQUEST:   code = "MPI_ERR_REQUEST"; break;      
    case MPI_ERR_ROOT:      code = "MPI_ERR_ROOT"; break;      
    case MPI_ERR_GROUP:     code = "MPI_ERR_GROUP"; break;      
    case MPI_ERR_OP:        code = "MPI_ERR_OP"; break;      
    case MPI_ERR_TOPOLOGY:  code = "MPI_ERR_TOPOLOGY"; break;      
    case MPI_ERR_DIMS:      code = "MPI_ERR_DIMS"; break;      
    case MPI_ERR_ARG:       code = "MPI_ERR_ARG"; break;      
    case MPI_ERR_UNKNOWN:   code = "MPI_ERR_UNKNOWN"; break;      
    case MPI_ERR_TRUNCATE:  code = "MPI_ERR_TRUNCATE"; break;      
    case MPI_ERR_OTHER:     code = "MPI_ERR_OTHER"; break;      
    case MPI_ERR_INTERN:    code = "MPI_ERR_INTERN"; break;      
    case MPI_ERR_PENDING:   code = "MPI_ERR_PENDING"; break;      
    case MPI_ERR_IN_STATUS: code = "MPI_ERR_IN_STATUS"; break;      
    case MPI_ERR_LASTCODE:  code = "MPI_ERR_LASTCODE";  break;     
    default: code = "*unknown MPI error*";
  }
  if (MPI_Error_string(errval, systemErrDesc, &len) != MPI_SUCCESS || len == 0) 
    strcpy(systemErrDesc, "(no description available)");
  sprintf(msg, "%s(%i): %s", code, errval, systemErrDesc);
  return msg;
}
/* ------------------------------------------------------------------------------------ */

/* make an MPI call - if it fails, print error message and return */
#if AMX_DEBUG || AMX_ENABLE_ERRCHECKS
 #define MPI_SAFE(fncall) do {                                                                    \
   int retcode = (fncall);                                                                        \
   if_pf (retcode != MPI_SUCCESS) {                                                               \
     char msg[1024];                                                                              \
     sprintf(msg, "\nAMMPI encountered an MPI Error: %s(%i)\n", MPI_ErrorName(retcode), retcode); \
     AMX_RETURN_ERRFR(RESOURCE, fncall, msg);                                                     \
   }                                                                                              \
 } while (0)
#else
 #define MPI_SAFE(fncall) (fncall)
#endif

/* make an MPI call - 
 * if it fails, print error message and value of expression is FALSE, 
 * otherwise, the value of this expression will be TRUE 
 */
#if AMX_DEBUG || AMX_ENABLE_ERRCHECKS
 #define MPI_SAFE_NORETURN(fncall) (AMX_VerboseErrors ?                                  \
      AMMPI_checkMPIreturn(fncall, #fncall, AMX_CURRENT_FUNCTION, __FILE__, __LINE__):   \
      (fncall) == MPI_SUCCESS)
#else
 #define MPI_SAFE_NORETURN(fncall) ((fncall),TRUE)
#endif
static int AMMPI_checkMPIreturn(int retcode, const char *fncallstr, 
                                const char *context, const char *file, int line) {
   if_pf (retcode != MPI_SUCCESS) {  
     fprintf(stderr, "\nAMMPI %s encountered an MPI Error: %s(%i)\n"
                     "  at %s:%i\n", 
       context, MPI_ErrorName(retcode), retcode, file, line); 
     fflush(stderr);
     return FALSE;
   }
   else return TRUE;
}

/* ------------------------------------------------------------------------------------ */
/*  global data */
extern int AMMPI_numBundles;
extern eb_t AMMPI_bundles[AMMPI_MAX_BUNDLES];

/* ------------------------------------------------------------------------------------ */
/*  global helper functions */
extern int AMMPI_Block(eb_t eb); 
  /* block until some endpoint receive buffer becomes non-empty with a user message
   * may poll, and does handle SPMD control events
   */
extern int AMMPI_ServiceIncomingMessages(ep_t ep, int blockForActivity, int repliesOnly);

/*  debugging printouts */
extern char *AMMPI_enStr(en_t en, char *buf);
extern char *AMMPI_tagStr(tag_t tag, char *buf);

extern void AMMPI_DefaultReturnedMsg_Handler(int status, op_t opcode, void *token);

/* ------------------------------------------------------------------------------------ */
/* interface for allowing control messages to be sent between mutually mapped endpoints 
 * up to AMMPI_MAX_SHORT integer arguments are passed verbatim to the registered handler,
 * which should NOT call any AMMPI functions (including poll, reply, etc)
 * AMMPI_SendControlMessage is safe to call from a handler context
 */
extern int AMMPI_SendControlMessage(ep_t from, en_t to, int numargs, ...);
  /* beware - cast all optional args of AMMPI_SendControlMessage to int32_t */
extern int AMMPI_RegisterControlMessageHandler(ep_t ea, ammpi_handler_fn_t function);

#if AMMPI_NONBLOCKING_SENDS
extern int AMMPI_AllocateSendBuffers(ep_t ep);
extern int AMMPI_ReleaseSendBuffers(ep_t ep);
extern int AMMPI_AcquireSendBuffer(ep_t ep, int numBytes, int isrequest, 
                            ammpi_buf_t** pbuf, MPI_Request** pHandle);
extern int AMMPI_ReapSendCompletions(ammpi_sendbuffer_pool_t* pool);
extern int AMMPI_GrowReplyPool(ammpi_sendbuffer_pool_t* pool);
#endif
#if AMMPI_PREPOST_RECVS
extern int AMMPI_PostRecvBuffer(ammpi_buf_t *rxBuf, MPI_Request *prxHandle, MPI_Comm *pmpicomm);
#endif
/* ------------------------------------------------------------------------------------ */

/* system message type field */
typedef enum {
  ammpi_system_user=0,      /*  not a system message */
  ammpi_system_autoreply,   /*  automatically generated reply */
  ammpi_system_returnedmessage, /*  arg is reason code, req/rep represents the type of message refused */
  ammpi_system_controlmessage, /*  used to pass system control information - arg is reserved */

  ammpi_system_numtypes
} ammpi_system_messagetype_t;


/* ------------------------------------------------------------------------------------ */

AMMPI_END_EXTERNC

#endif
