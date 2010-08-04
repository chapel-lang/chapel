/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp_internal.h,v $
 *     $Date: 2010/05/04 01:45:18 $
 * $Revision: 1.35.4.2 $
 * Description: AMUDP internal header file
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#ifndef _AMUDP_INTERNAL_H
#define _AMUDP_INTERNAL_H

#include <portable_inttypes.h>
#include <portable_platform.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if !PLATFORM_OS_MSWINDOWS
  #include <unistd.h>
  #include <errno.h>
#endif
#include <sockutil.h> /* for SPMD TCP stuff */
#include <amudp.h>
#ifdef HAVE_GASNET_TOOLS 
  #define GASNETT_LITE_MODE /* use lite mode, to preserve AMUDP's threading neutrality */
  #include <gasnet_tools.h> /* must precede internal assert defs */
  #define AMUDP_FORMAT_PRINTF GASNETT_FORMAT_PRINTF
#elif defined (__GNUC__) || defined (__attribute__)
  #define AMUDP_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) \
            __attribute__((__format__ (__printf__, fmtarg, firstvararg))) declarator
#else
  #define AMUDP_FORMAT_PRINTF(fnname,fmtarg,firstvararg,declarator) declarator
#endif

/* AMUDP system configuration parameters */
#ifndef DISABLE_STDSOCKET_REDIRECT
#if PLATFORM_OS_SUPERUX || PLATFORM_OS_HPUX
  /* broken on SuperUX due to a bad FIONREAD implementation, which causes numBytesWaiting to fail
     also seems to possibly be some issue with a redirected stdout always triggering select, even when
     no output is waiting
     This also seems the only reliable way to get good output on HPUX, especially in 64-bit mode
   */
  #define DISABLE_STDSOCKET_REDIRECT  1 
#else
  #define DISABLE_STDSOCKET_REDIRECT  0   /* disable redirection of slave stdin/stdout/stderr to master */
#endif
#endif
#ifndef USE_SOCKET_RECVBUFFER_GROW
#define USE_SOCKET_RECVBUFFER_GROW  1   /* grow RCVBUF on UDP sockets */
#endif
#ifndef USE_SOCKET_SENDBUFFER_GROW
#define USE_SOCKET_SENDBUFFER_GROW  1   /* grow SNDBUF on UDP sockets */
#endif
#define AMUDP_RECVBUFFER_MAX  4194304   /* never exceed 4 MB (huge) */
#ifdef UETH
  #define USE_TRUE_BULK_XFERS       0   /* bulk xfers use long packets rather than segmentation */
#else
  #define USE_TRUE_BULK_XFERS       1   /* bulk xfers use long packets rather than segmentation */
#endif
#define AMUDP_SIGIO                39  
                                        /* signal used for async IO operations - 
                                         * avoid SIGIO to prevent conflicts with application using library
                                         * also, ueth uses 38
                                         */
#ifdef UETH
  #define AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC   10000  /* usec until first retransmit */
  #define UETH_RECVPOOLFUDGEFACTOR                    1  /* scale up the recv buffer */
#else
  #define AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC   10000  /* usec until first retransmit */
#endif
#define AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER     2  /* timeout exponential backoff factor */
#define AMUDP_MAX_REQUESTTIMEOUT_MICROSEC    30000000  /* max timeout before considered undeliverable */
#define AMUDP_DEFAULT_EXPECTED_BANDWIDTH         1220  /* expected Kbytes/sec bandwidth: 1220 = 10Mbit LAN */
#define AMUDP_TIMEOUT_INFINITE ((uint32_t)-1)
extern uint32_t AMUDP_MaxRequestTimeout_us;
extern uint32_t AMUDP_InitialRequestTimeout_us;
extern uint32_t AMUDP_RequestTimeoutBackoff;
extern uint32_t AMUDP_ExpectedBandwidth; /* expected half-duplex bandwidth in KBytes/sec */


#define AMUDP_TIMEOUTS_CHECKED_EACH_POLL            1  /* number of timeout values we check upon each poll */
#define AMUDP_MAX_RECVMSGS_PER_POLL                10  /* max number of waiting messages serviced per poll (0 for unlimited) 
                                                          we actually service up to MAX(AMUDP_MAX_RECVMSGS_PER_POLL, network_depth)
                                                          to prevent unnecessary retransmits (where the awaited reply is sitting in recv buffer) */
#if AMUDP_DEBUG
#define USE_CLEAR_UNUSED_SPACE 1  /* clear any padding sent in AMs, to prevent valgrind warnings */
#endif

#define AMUDP_INITIAL_NUMENDPOINTS 1    /* initial size of bundle endpoint table */

#define AMUDP_DEFAULT_NETWORKDEPTH 4    /* default depth if none specified */

/* AMUDP-SPMD system configuration parameters */
#define USE_NUMERIC_MASTER_ADDR     0   /* pass a numeric IP on slave command line */
#define USE_COORD_KEEPALIVE         1   /* set SO_KEEPALIVE on TCP coord sockets */
#define ABORT_JOB_ON_NODE_FAILURE   1   /* kill everyone if any slave drops the TCP coord */
#define USE_BLOCKING_SPMD_BARRIER   1   /* use blocking AM calls in SPMDBarrier() */
#if defined( LINUX )
  #define USE_ASYNC_TCP_CONTROL     1   /* use O_ASYNC and signals to stat TCP coord sockets */
#else
  #define USE_ASYNC_TCP_CONTROL     0
#endif

#ifndef AMUDP_DEBUG_VERBOSE
  #if GASNET_DEBUG_VERBOSE
    #define AMUDP_DEBUG_VERBOSE       1
  #else
    #define AMUDP_DEBUG_VERBOSE       0
  #endif
#endif

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
#if defined(__GNUC__) || defined(__FUNCTION__) /* try to get the function name from GCC */
  #define __CURR_FUNCTION __FUNCTION__
#else
  #define __CURR_FUNCTION ((const char *) 0) /* could use __func__ for C99 compilers.. */
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
  #if defined(__GNUC__) && __GNUC__ >= 3 && 0
    #define PREDICT_TRUE(exp)  __builtin_expect( (exp), 1 )
    #define PREDICT_FALSE(exp) __builtin_expect( (exp), 0 )
  #else
    #define PREDICT_TRUE(exp)  (exp)
    #define PREDICT_FALSE(exp) (exp)
  #endif

  /* if with branch prediction */
  #define if_pf(cond) if (PREDICT_FALSE(cond))
  #define if_pt(cond) if (PREDICT_TRUE(cond))
#endif

SOCK_BEGIN_EXTERNC

AMUDP_FORMAT_PRINTF(AMUDP_Err,1,2,
extern int AMUDP_Err(const char *msg, ...));

AMUDP_FORMAT_PRINTF(AMUDP_Warn,1,2,
extern int AMUDP_Warn(const char *msg, ...));

#ifdef GASNETT_NORETURN
GASNETT_NORETURN
#endif
AMUDP_FORMAT_PRINTF(AMUDP_FatalErr,1,2,
extern void AMUDP_FatalErr(const char *msg, ...));
#ifdef GASNETT_NORETURNP
GASNETT_NORETURNP(AMUDP_FatalErr)
#endif

/* memory allocation */
#if AMUDP_DEBUG
  /* use the gasnet debug malloc functions if a debug libgasnet is linked */
  extern void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc);
  extern void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc);
  extern void (*gasnett_debug_free_fn)(void *ptr, const char *curloc);
  static void *_AMUDP_malloc(size_t sz, const char *curloc) {
    void *ret = malloc(sz);
    if_pf(!ret) AMUDP_FatalErr("Failed to malloc(%lu) at %s", (unsigned long)sz, curloc);
    return ret;
  }
  static void *_AMUDP_calloc(size_t N, size_t S, const char *curloc) {
    void *ret = calloc(N,S);
    if_pf(!ret) AMUDP_FatalErr("Failed to calloc(%lu,%lu) at %s", (unsigned long)N, (unsigned long)S, curloc);
    return ret;
  }
  static void _AMUDP_free(void *ptr, const char *curloc) {
    free(ptr);
  }
  #define AMUDP_curloc __FILE__ ":" _STRINGIFY(__LINE__)
  #define AMUDP_malloc(sz)                                   \
    ( (PREDICT_FALSE(gasnett_debug_malloc_fn==NULL) ?        \
        gasnett_debug_malloc_fn = &_AMUDP_malloc : 0), \
      (*gasnett_debug_malloc_fn)(sz, AMUDP_curloc))
  #define AMUDP_calloc(N,S)                                  \
    ( (PREDICT_FALSE(gasnett_debug_calloc_fn==NULL) ?        \
        gasnett_debug_calloc_fn = &_AMUDP_calloc : 0), \
      (*gasnett_debug_calloc_fn)((N),(S), AMUDP_curloc))
  #define AMUDP_free(ptr)                                \
    ( (PREDICT_FALSE(gasnett_debug_free_fn==NULL) ?      \
        gasnett_debug_free_fn = &_AMUDP_free : 0), \
      (*gasnett_debug_free_fn)(ptr, AMUDP_curloc))

  #undef malloc
  #define malloc(x)   ERROR_use_AMUDP_malloc
  #undef calloc
  #define calloc(n,s) ERROR_use_AMUDP_calloc
  #undef free
  #define free(x)     ERROR_use_AMUDP_free
#else
  #define AMUDP_malloc(sz)  malloc(sz)
  #define AMUDP_calloc(N,S) calloc((N),(S))
  #define AMUDP_free(ptr)   free(ptr)
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
static const char *AMUDP_curr_function(const char *arg) {
  /* hides a constant expression from compilers that whine about them */
  return arg ? arg : "";
}
#define AMUDP_RETURN_ERR(type) do {                                        \
    if (AMUDP_VerboseErrors) {                                             \
      fprintf(stderr, "AMUDP %s returning an error code: AM_ERR_%s (%s)\n" \
        "  at %s:%i\n"                                                     \
        , AMUDP_curr_function(__CURR_FUNCTION)                             \
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
        , AMUDP_curr_function(__CURR_FUNCTION)                                 \
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
        , AMUDP_curr_function(__CURR_FUNCTION)                                         \
        , #type, AMUDP_ErrorDesc(AM_ERR_##type), #fromfn, __FILE__, __LINE__, reason); \
      fflush(stderr);                                                                  \
    }                                                                                  \
    return AM_ERR_ ## type;                                                            \
  } while (0)
/* return a possible error */
#define AMUDP_RETURN(val) do {                                           \
  if (AMUDP_VerboseErrors && val != AM_OK) {                             \
    fprintf(stderr, "AMUDP %s returning an error code: %s (%s)\n"        \
      "  at %s:%i\n"                                                     \
      , AMUDP_curr_function(__CURR_FUNCTION)                             \
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
    if_pf (errcond) AMUDP_RETURN_ERR(type)
  #define AMUDP_CHECK_ERRF(errcond, type, fromfn) \
    if_pf (errcond) AMUDP_RETURN_ERRF(type, fromfn)
  #define AMUDP_CHECK_ERRR(errcond, type, reason) \
    if_pf (errcond) AMUDP_RETURN_ERRR(type, reason)
  #define AMUDP_CHECK_ERRFR(errcond, type, fromfn, reason) \
    if_pf (errcond) AMUDP_RETURN_ERRFR(type, fromfn, reason)
#else
  #define AMUDP_CHECK_ERR(errcond, type)                    ((void)0)
  #define AMUDP_CHECK_ERRF(errcond, type, fromfn)           ((void)0)
  #define AMUDP_CHECK_ERRR(errcond, type, reason)           ((void)0)
  #define AMUDP_CHECK_ERRFR(errcond, type, fromfn, reason)  ((void)0)
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
        AMUDP_curr_function(__CURR_FUNCTION), __FILE__, __LINE__, #expr))
#endif

extern const char *sockErrDesc();

#ifdef UETH
  #define enEqual(en1,en2) (!memcmp(&en1, &en2, sizeof(en_t)))
#else
  #define enEqual(en1,en2)                  \
    ((en1).sin_port == (en2).sin_port       \
  && (en1).sin_addr.s_addr == (en2).sin_addr.s_addr)
#endif

//------------------------------------------------------------------------------------
// global data
extern int AMUDP_numBundles;
extern eb_t AMUDP_bundles[AMUDP_MAX_BUNDLES];

extern double AMUDP_FaultInjectionRate;
extern double AMUDP_FaultInjectionEnabled;

#ifdef UETH
  extern ep_t AMUDP_UETH_endpoint; /* the one-and-only UETH endpoint */
#endif
extern int amudp_Initialized;
#define AMUDP_CHECKINIT() AMUDP_CHECK_ERR((!amudp_Initialized),NOT_INIT)
/* ------------------------------------------------------------------------------------ */
/* these handle indexing into our 2-D array of desriptors and buffers */
#define GET_REQ_DESC(ep, remoteProc, inst) \
  (&((ep)->requestDesc[inst * (ep)->P + remoteProc]))
#define GET_REQ_BUF(ep, remoteProc, inst) \
  (&((ep)->requestBuf[inst * (ep)->P + remoteProc]))
#define GET_REP_DESC(ep, remoteProc, inst) \
  (&((ep)->replyDesc[inst * (ep)->P + remoteProc]))
#define GET_REP_BUF(ep, remoteProc, inst) \
  (&((ep)->replyBuf[inst * (ep)->P + remoteProc]))
/* these recover the processor and instance indexes from an entry offset into the 2-D arrays */
#define GET_REMOTEPROC_FROM_POS(ep, curpos) \
  (curpos % (ep)->P)
#define GET_INST_FROM_POS(ep, curpos) \
  (curpos / (ep)->P)
/* ------------------------------------------------------------------------------------ */
/* accessors for packet args, data and length
 * the only complication here is we want data to be double-word aligned, so we may add
 * an extra unused 4-byte argument to make sure the data lands on a double-word boundary
 */
#if 0
  #define HEADER_EVEN_WORDLENGTH  (((int)(uintptr_t)((&((amudp_buf_t *)NULL)->_Data)-1))%8==0?1:0)
#else
  #define HEADER_EVEN_WORDLENGTH \
    ( ( (((uint8_t *)&(((amudp_buf_t *)NULL)->_Data)) - ((uint8_t *)NULL)) & 0x7) == 0 ? 1 : 0)
#endif
#define ACTUAL_NUM_ARGS(pMsg) (AMUDP_MSG_NUMARGS(pMsg)%2==0?       \
                            AMUDP_MSG_NUMARGS(pMsg)+!HEADER_EVEN_WORDLENGTH:  \
                            AMUDP_MSG_NUMARGS(pMsg)+HEADER_EVEN_WORDLENGTH)

#define GET_PACKET_LENGTH(pbuf)                                       \
  (((char *)&pbuf->_Data[4*ACTUAL_NUM_ARGS(&pbuf->Msg) + pbuf->Msg.nBytes]) - ((char *)pbuf))
#define GET_PACKET_DATA(pbuf)                                         \
  (&pbuf->_Data[4*ACTUAL_NUM_ARGS(&pbuf->Msg)])
#define GET_PACKET_ARGS(pbuf)                                         \
  ((uint32_t *)pbuf->_Data)
//------------------------------------------------------------------------------------
// global helper functions
extern int AMUDP_Block(eb_t eb); 
  /* block until receive buffer becomes non-empty
   * does not poll, but does handle SPMD control socket events
   */
// bulk buffers
extern amudp_buf_t *AMUDP_AcquireBulkBuffer(ep_t ep); // get a bulk buffer
extern void AMUDP_ReleaseBulkBuffer(ep_t ep, amudp_buf_t *buf); // release a bulk buffer

#if !defined(UETH) && USE_SOCKET_RECVBUFFER_GROW
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
#ifdef UETH
  extern void AMUDP_SPMDAddressChangeCallback(ueth_addr_t *address);
#endif
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
  amudp_system_bulkxferfragment, // arg is total number of other packets in transfer (totalpackets - 1)
  amudp_system_returnedmessage, // arg is reason code, req/rep represents the type of message refused

  amudp_system_numtypes
} amudp_system_messagetype_t;


//------------------------------------------------------------------------------------
#ifndef AMUDP_ENV_PREFIX
  #define AMUDP_ENV_PREFIX AMUDP
#endif
#define AMUDP_ENV_PREFIX_STR _STRINGIFY(AMUDP_ENV_PREFIX)

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
extern int myselect(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
            struct timeval *timeout);
#define select myselect

extern int myrecvfrom(SOCKET s, char * buf, int len, int flags,                  
  struct sockaddr *from, int *fromlen);
 

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
      if (inputWaiting(AMUDP_SPMDControlSocket)) /* check for arrived messages */       \
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
#elif defined(UETH)
  /* Ticks == CPU cycles for UETH */
  #define getMicrosecondTimeStamp() ueth_getustime()
  #define getCPUTicks()             ((amudp_cputick_t)ueth_getcputime())
  #define ticks2us(ticks)           (ueth_ticks_to_us(ticks))
  #define us2ticks(us)              ((amudp_cputick_t)(ueth_us_to_ticks(us)))
  #define tickspersec               ueth_ticks_per_second
#else
  #if PLATFORM_OS_MSWINDOWS
    static int64_t getMicrosecondTimeStamp(void) {
      static int status = -1;
      static double multiplier;
      if (status == -1) { /*  first time run */
        LARGE_INTEGER freq;
        if (!QueryPerformanceFrequency(&freq)) status = 0; /*  don't have high-perf counter */
        else {
          multiplier = 1000000 / (double)freq.QuadPart;
          status = 1;
        }
      }
      if (status) { /*  we have a high-performance counter */
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return (int64_t)(multiplier * count.QuadPart);
      } else { /*  no high-performance counter */
        /*  this is a millisecond-granularity timer that wraps every 50 days */
        return (GetTickCount() * 1000);
      }
    }
  /* #elif PLATFORM_ARCH_X86
   * TODO: it would be nice to take advantage of the Pentium's "rdtsc" instruction,
   * which reads a fast counter incremented on each cycle. Unfortunately, that
   * requires a way to convert cycles to microseconds, and there doesn't appear to 
   * be a way to directly query the cycle speed
   */

  #else /* unknown processor - use generic UNIX call */
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
  #endif
  /* Ticks == us for gettimeofday */
  #define getCPUTicks()             ((amudp_cputick_t)getMicrosecondTimeStamp())
  #define ticks2us(ticks)           (ticks)
  #define us2ticks(us)              ((amudp_cputick_t)(us))
  #define tickspersec               1000000
#endif
//------------------------------------------------------------------------------------

SOCK_END_EXTERNC

#endif
