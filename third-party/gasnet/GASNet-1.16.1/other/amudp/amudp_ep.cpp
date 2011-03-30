/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp_ep.cpp,v $
 *     $Date: 2008/10/02 07:56:51 $
 * $Revision: 1.29 $
 * Description: AMUDP Implementations of endpoint and bundle operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <amudp_internal.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

/* definitions for internal declarations */
int amudp_Initialized = 0;
static void AMUDP_defaultAMHandler(void * token);
amudp_handler_fn_t amudp_unused_handler = (amudp_handler_fn_t)&AMUDP_defaultAMHandler;
amudp_handler_fn_t amudp_defaultreturnedmsg_handler = (amudp_handler_fn_t)&AMUDP_DefaultReturnedMsg_Handler;
int AMUDP_VerboseErrors = 0;
int AMUDP_PoliteSync = 0;
uint32_t AMUDP_ExpectedBandwidth = AMUDP_DEFAULT_EXPECTED_BANDWIDTH;
uint32_t AMUDP_RequestTimeoutBackoff = AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER;
uint32_t AMUDP_MaxRequestTimeout_us = AMUDP_MAX_REQUESTTIMEOUT_MICROSEC;
uint32_t AMUDP_InitialRequestTimeout_us = AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC;

int AMUDP_SilentMode = 0; 
AMUDP_IDENT(AMUDP_IdentString_Version, "$AMUDPLibraryVersion: " AMUDP_LIBRARY_VERSION_STR " $");
#ifdef UETH
  ep_t AMUDP_UETH_endpoint = NULL; /* the one-and-only UETH endpoint */
#endif

double AMUDP_FaultInjectionRate = 0.0;
double AMUDP_FaultInjectionEnabled = 0;

const amudp_stats_t AMUDP_initial_stats = /* the initial state for stats type */
        { {0,0,0}, {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, {0,0,0}, 
          0,
          (amudp_cputick_t)-1, 0, 0,
          {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, 
          0
        };

/* ------------------------------------------------------------------------------------ */
/* error handling */
AMUDP_FORMAT_PRINTF(AMUDP_Msg,2,0,
static int AMUDP_Msg(const char *prefix, const char *msg, va_list argptr)) {
  char *expandedmsg = (char *)AMUDP_malloc(strlen(msg)+strlen(prefix)+50);
  int retval;

  sprintf(expandedmsg, "*** %s: %s\n", prefix, msg);
  retval = vfprintf(stderr, expandedmsg, argptr);
  fflush(stderr);
  AMUDP_free(expandedmsg);

  return retval; 
}

extern int AMUDP_Warn(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("AMUDP WARNING", msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMUDP_Err(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("AMUDP ERROR", msg, argptr);
  va_end(argptr);
  return retval;
}

extern void AMUDP_FatalErr(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("FATAL ERROR", msg, argptr);
  va_end(argptr);
  abort();
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_defaultAMHandler(void *token) {
  int srcnode = -1;
  AMUDP_GetSourceId(token, &srcnode);
  AMUDP_FatalErr("AMUDP received an AM message from node %i for a handler index "
                     "with no associated AM handler function registered", 
                     srcnode);
}
/*------------------------------------------------------------------------------------
 * Endpoint list handling for bundles
 *------------------------------------------------------------------------------------ */
int AMUDP_numBundles = 0;
eb_t AMUDP_bundles[AMUDP_MAX_BUNDLES] = {0};
/* ------------------------------------------------------------------------------------ */
static int AMUDP_ContainsEndpoint(eb_t eb, ep_t ep) {
  int i;
  for (i = 0; i < eb->n_endpoints; i++) {
    if (eb->endpoints[i] == ep) return TRUE;
  }
  return FALSE;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_InsertEndpoint(eb_t eb, ep_t ep) {
  AMUDP_assert(eb && ep);
  AMUDP_assert(eb->endpoints != NULL);
  if (eb->n_endpoints == eb->cursize) { /* need to grow array */
    int newsize = eb->cursize * 2;
    ep_t *newendpoints = (ep_t *)AMUDP_malloc(sizeof(ep_t)*newsize);
    memcpy(newendpoints, eb->endpoints, sizeof(ep_t)*eb->n_endpoints);
    eb->endpoints = newendpoints;
    eb->cursize = newsize;
  }
  eb->endpoints[eb->n_endpoints] = ep;
  eb->n_endpoints++;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_RemoveEndpoint(eb_t eb, ep_t ep) {
  AMUDP_assert(eb && ep);
  AMUDP_assert(eb->endpoints != NULL);
  AMUDP_assert(AMUDP_ContainsEndpoint(eb, ep));
  { int i;
    for (i = 0; i < eb->n_endpoints; i++) {
      if (eb->endpoints[i] == ep) {
        eb->endpoints[i] = eb->endpoints[eb->n_endpoints-1];
        eb->n_endpoints--;
        return;
      }
    }
    AMUDP_FatalErr("failure in AMUDP_RemoveEndpoint");
  }
}
/*------------------------------------------------------------------------------------
 * Endpoint bulk buffer management
 *------------------------------------------------------------------------------------ */
extern amudp_buf_t *AMUDP_AcquireBulkBuffer(ep_t ep) { // get a bulk buffer
  AMUDP_assert(ep != NULL);
  AMUDP_assert(ep->bulkBufferPoolFreeCnt <= ep->bulkBufferPoolSz);
  if (ep->bulkBufferPoolFreeCnt == 0) {
    // grow the pool
    int oldsz = ep->bulkBufferPoolSz;
    amudp_buf_t **temp = (amudp_buf_t **)AMUDP_malloc((oldsz+1)*sizeof(amudp_buf_t *));
    temp[0] = (amudp_buf_t *)AMUDP_malloc(AMUDP_MAXBULK_NETWORK_MSG);
    temp[0]->status.bulkBuffer = NULL;
    if (ep->bulkBufferPool) {
      memcpy(temp+1, ep->bulkBufferPool, sizeof(amudp_buf_t *)*oldsz);
      AMUDP_free(ep->bulkBufferPool);
    }
    ep->bulkBufferPool = temp;
    ep->bulkBufferPoolSz = oldsz + 1;
    ep->bulkBufferPoolFreeCnt = 1;
  }
  return ep->bulkBufferPool[--ep->bulkBufferPoolFreeCnt];
}
/* ------------------------------------------------------------------------------------ */
extern void AMUDP_ReleaseBulkBuffer(ep_t ep, amudp_buf_t *buf) { // release a bulk buffer
  int i; // this is non-optimal, but the list should never get too long, so it won't matter
  AMUDP_assert(ep != NULL);
  AMUDP_assert(ep->bulkBufferPoolFreeCnt <= ep->bulkBufferPoolSz);
  for (i = ep->bulkBufferPoolFreeCnt; i < ep->bulkBufferPoolSz; i++) {
    if (ep->bulkBufferPool[i] == buf) {
      ep->bulkBufferPool[i] = ep->bulkBufferPool[ep->bulkBufferPoolFreeCnt];
      ep->bulkBufferPool[ep->bulkBufferPoolFreeCnt] = buf;
      ep->bulkBufferPoolFreeCnt++;
      return;
    }
  }
  AMUDP_FatalErr("Internal error in AMUDP_ReleaseBulkBuffer()");
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_FreeAllBulkBuffers(ep_t ep) {
  int i;
  AMUDP_assert(ep != NULL);
  AMUDP_assert(ep->bulkBufferPoolFreeCnt <= ep->bulkBufferPoolSz);
  for (i=0; i < ep->bulkBufferPoolSz; i++) AMUDP_free(ep->bulkBufferPool[i]);
  if (ep->bulkBufferPool) AMUDP_free(ep->bulkBufferPool);
  ep->bulkBufferPool = NULL;
  ep->bulkBufferPoolFreeCnt = 0;
  ep->bulkBufferPoolSz = 0;
}
/*------------------------------------------------------------------------------------
 * Endpoint resource management
 *------------------------------------------------------------------------------------ */
static uint32_t AMUDP_currentUDPInterface = INADDR_ANY;
extern int AMUDP_SetUDPInterface(uint32_t IPAddress) {
  AMUDP_currentUDPInterface = IPAddress;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
#if !defined(UETH) && USE_SOCKET_RECVBUFFER_GROW
  #if 0
  #if PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX
    #include <linux/unistd.h>
    #include <linux/sysctl.h>
  #endif
#endif
extern int AMUDP_growSocketBufferSize(ep_t ep, int targetsize, 
                                       int szparam, const char *paramname) {
  int initialsize; /* original socket recv size */
  int maxedout = 0;
  GETSOCKOPT_LENGTH_T junk = sizeof(int);
  if (SOCK_getsockopt(ep->s, SOL_SOCKET, szparam, (char *)&initialsize, &junk) == SOCKET_ERROR) {
    #if AMUDP_DEBUG
      AMUDP_Warn("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s",paramname,strerror(errno));
    #endif
    initialsize = 65535;
  } 
  ep->socketRecvBufferSize = initialsize; /* ensure ep->socketRecvBufferSize is always initialized */

  targetsize = MAX(initialsize, targetsize); /* never shrink buffer */

  #if 0 /* it appears this max means nothing */
  { int maxsize;
    #if PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX
    { /*  try to determine the max we can use (reading /proc/sys/net/core/rmem_max may be more reliable) */
      int rmem_max[1] = { NET_CORE_RMEM_MAX };
      struct __sysctl_args args={&rmem_max,sizeof(rmem_max),&maxsize,sizeof(int),0,0};
      if (_sysctl(&args)) {
        #if AMUDP_DEBUG
          perror("sysctl");
          AMUDP_Err("sysctl() on UDP socket failed");
        #endif
      }
      size = MIN(size, maxsize);
    }
    #endif
  }
  #endif
  /* now set it to the largest value it will take */
  while (targetsize > initialsize) {
    int sz = targetsize; /* prevent OS from tampering */
    if (setsockopt(ep->s, SOL_SOCKET, szparam, (char *)&sz, sizeof(int)) == SOCKET_ERROR) {
      #if AMUDP_DEBUG
        AMUDP_Warn("setsockopt(SOL_SOCKET, %s, %i) on UDP socket failed: %s", paramname, targetsize, strerror(errno));
      #endif
    } else {
      int temp = targetsize;
      junk = sizeof(int);
      if (SOCK_getsockopt(ep->s, SOL_SOCKET, szparam, (char *)&temp, &junk) == SOCKET_ERROR) {
        #if AMUDP_DEBUG
          AMUDP_Warn("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s", paramname, strerror(errno));
        #endif
      }
      if (temp >= targetsize) {
        if (!AMUDP_SilentMode) {
          fprintf(stderr, "UDP %s buffer successfully set to %i bytes\n", paramname, targetsize); fflush(stderr);
        }
        ep->socketRecvBufferSize = temp;
        break; /* success */
      }
    }
    targetsize = (int)(0.9 * targetsize);
    maxedout = 1;
  }
  return maxedout;
}
#endif
/* ------------------------------------------------------------------------------------ */
static int AMUDP_AllocateEndpointResource(ep_t ep) {
  AMUDP_assert(ep != NULL);
  #ifdef UETH
    if (AMUDP_UETH_endpoint) return FALSE; /* only one endpoint per process */
    if (ueth_init() != UETH_OK) return FALSE;
    if (ueth_getaddress(&ep->name) != UETH_OK) {
      ueth_terminate();
      return FALSE;
    }
    /*  TODO this doesn't handle apps that dont use SPMD extensions */
    if (ueth_setaddresshook(&AMUDP_SPMDAddressChangeCallback) != UETH_OK) {
      ueth_terminate();
      return FALSE;
    }
    AMUDP_UETH_endpoint = ep;
  #else
    /* allocate socket */
    ep->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ep->s == INVALID_SOCKET) 
      AMUDP_RETURN_ERRFR(RESOURCE, socket, sockErrDesc());

    #ifdef WINSOCK
    { /* check transport message size - UNIX doesn't seem to have a way for doing this */
      unsigned int maxmsg;
      GETSOCKOPT_LENGTH_T sz = sizeof(unsigned int);
      if (SOCK_getsockopt(ep->s, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&maxmsg, &sz) == SOCKET_ERROR)
        AMUDP_RETURN_ERRFR(RESOURCE, getsockopt, sockErrDesc());
      if (maxmsg < AMUDP_MAX_NETWORK_MSG) 
        AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_AllocateEndpointResource, "max datagram size of UDP provider is too small");
    }
    #endif

    ep->name.sin_family = AF_INET;
    ep->name.sin_port = 0; /* any port */
    ep->name.sin_addr.s_addr = htonl(AMUDP_currentUDPInterface);
    memset(&ep->name.sin_zero, 0, sizeof(ep->name.sin_zero));

    if (bind(ep->s, (struct sockaddr*)&ep->name, sizeof(struct sockaddr)) == SOCKET_ERROR) {
      closesocket(ep->s);
      return FALSE;
    }
    { /*  danger: this might fail on multi-homed hosts if AMUDP_currentUDPInterface was not set*/
      GETSOCKNAME_LENGTH_T sz = sizeof(en_t);
      if (SOCK_getsockname(ep->s, (struct sockaddr*)&ep->name, &sz) == SOCKET_ERROR) {
        closesocket(ep->s);
        return FALSE;
      }
      /* can't determine interface address */
      if (ep->name.sin_addr.s_addr == INADDR_ANY) {
        AMUDP_Err("AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface address");
        return FALSE;
      }
      if (ep->name.sin_port == 0) {
        AMUDP_Err("AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface port");
        return FALSE; 
      }
    }
  #endif
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_AllocateEndpointBuffers(ep_t ep) {
  int PD = ep->PD;
  amudp_buf_t *pool;

  AMUDP_assert(ep != NULL);
  AMUDP_assert(ep->depth >= 1);
  AMUDP_assert(ep->P > 0 && ep->P <= AMUDP_MAX_NUMTRANSLATIONS);
  AMUDP_assert(ep->PD == ep->P * ep->depth);

  #ifdef UETH
    AMUDP_assert(sizeof(amudp_buf_t) <= UETH_MAXPACKETSIZE);
    AMUDP_assert(sizeof(amudp_buf_t) % UETH_ALIGNMENT == 0);
    /* one extra for temporary buffer */
    if (ueth_allocatepool((void**)&pool, sizeof(amudp_buf_t), 
                          2*PD + 1, UETH_RECVPOOLFUDGEFACTOR * (2*PD + 1)) 
                          != UETH_OK) return FALSE;
    ep->requestBuf = pool;
    ep->replyBuf = &pool[PD];
    ep->temporaryBuf = &pool[2*PD];
    /*if (ueth_setrecvpool(&pool[2*PD], 2*PD, sizeof(amudp_buf_t));*/
  #else
    AMUDP_assert(sizeof(amudp_buf_t) % sizeof(int) == 0); /* assume word-addressable machine */
    if (2*PD+1 > 65535) return FALSE; /* would overflow rxNumBufs */
    /* one extra rx buffer for ease of implementing circular rx buf
     * one for temporary buffer
     * allocate using calloc to prevent valgrind warnings for sending phantom padding argument
     */
    pool = (amudp_buf_t *)AMUDP_calloc((4 * PD + 2), sizeof(amudp_buf_t));
    if (!pool) return FALSE;
    ep->requestBuf = pool;
    ep->replyBuf = &pool[PD];
    ep->rxBuf = &pool[2*PD];
    ep->rxNumBufs = (uint16_t)(2*PD + 1);
    ep->rxFreeIdx = 0;
    ep->rxReadyIdx = 0;
    ep->temporaryBuf = &pool[4*PD+1];
  
    { int HPAMsize = 2*PD*AMUDP_MAX_NETWORK_MSG; /* theoretical max required by plain-vanilla HPAM */
      int padsize = 2*AMUDP_MAXBULK_NETWORK_MSG; /* some pad for non-HPAM true bulk xfers & retransmissions */
    
      #if USE_SOCKET_RECVBUFFER_GROW
          ep->socketRecvBufferMaxedOut = AMUDP_growSocketBufferSize(ep, HPAMsize+padsize, SO_RCVBUF, "SO_RCVBUF");
      #endif
      #if USE_SOCKET_SENDBUFFER_GROW
          AMUDP_growSocketBufferSize(ep, HPAMsize+padsize, SO_SNDBUF, "SO_SNDBUF");
      #endif
    }

  #endif
  ep->requestDesc = (amudp_bufdesc_t*)AMUDP_malloc(2 * PD * sizeof(amudp_bufdesc_t));
  ep->replyDesc = &ep->requestDesc[PD];
  /* init descriptor tables */
  memset(ep->requestDesc, 0, PD * sizeof(amudp_bufdesc_t));
  memset(ep->replyDesc,   0, PD * sizeof(amudp_bufdesc_t));
  ep->outstandingRequests = 0;
  ep->timeoutCheckPosn = 0;

  /* instance hint pointers & compressed translation table */
  ep->perProcInfo = (amudp_perproc_info_t *)AMUDP_malloc(ep->P * sizeof(amudp_perproc_info_t));
  memset(ep->perProcInfo, 0, ep->P * sizeof(amudp_perproc_info_t));

  { int i; /* need to init the reply bulk buffer ptrs */
    /* must do this regardless of USE_TRUE_BULK_XFERS, because we check it later even
     * if USE_TRUE_BULK_XFERS is off
     * */
    for (i = 0; i < PD; i++) ep->replyBuf[i].status.bulkBuffer = NULL;
  }

  ep->bulkBufferPool = NULL;
  ep->bulkBufferPoolSz = 0;
  ep->bulkBufferPoolFreeCnt = 0;
  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointResource(ep_t ep) {
  AMUDP_assert(ep != NULL);
  #ifdef UETH
    if (!AMUDP_UETH_endpoint) return FALSE;
    if (ueth_terminate() != UETH_OK) return FALSE;
    AMUDP_UETH_endpoint = NULL;
  #else
    /*  close UDP port */
    if (closesocket(ep->s) == SOCKET_ERROR) return FALSE;
  #endif
  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointBuffers(ep_t ep) {
  AMUDP_assert(ep != NULL);
  #ifdef UETH
    /* no explicit free required - handled by ueth_terminate */
  #else
    AMUDP_free(ep->requestBuf);
    ep->rxBuf = NULL;
  #endif
  ep->requestBuf = NULL;
  ep->replyBuf = NULL;

  AMUDP_free(ep->requestDesc);
  ep->requestDesc = NULL;
  ep->replyDesc = NULL;

  AMUDP_free(ep->perProcInfo);
  ep->perProcInfo = NULL;

  AMUDP_FreeAllBulkBuffers(ep);

  return TRUE;
}
/*------------------------------------------------------------------------------------
 * System initialization/termination
 *------------------------------------------------------------------------------------ */
extern int AM_Init() {
  if (amudp_Initialized == 0) { /* first call */
    /* check system attributes */
    AMUDP_assert(sizeof(int8_t) == 1);
    AMUDP_assert(sizeof(uint8_t) == 1);
    #ifndef INTTYPES_16BIT_MISSING
      AMUDP_assert(sizeof(int16_t) == 2);
      AMUDP_assert(sizeof(uint16_t) == 2);
    #endif
    AMUDP_assert(sizeof(int32_t) == 4);
    AMUDP_assert(sizeof(uint32_t) == 4);
    AMUDP_assert(sizeof(int64_t) == 8);
    AMUDP_assert(sizeof(uint64_t) == 8);

    AMUDP_assert(sizeof(uintptr_t) >= sizeof(void *));

    #ifdef WINSOCK
    { WSADATA wsa;
      #if 1
        WORD wVersionRequested = MAKEWORD( 1, 1 );
      #else
        WORD wVersionRequested = MAKEWORD( 2, 2 );
      #endif

      if (WSAStartup(wVersionRequested, &wsa)) AMUDP_RETURN_ERR(RESOURCE);
    }
    #endif

    { char *faultRate = AMUDP_getenv_prefixed("FAULT_RATE");
      if (faultRate && (AMUDP_FaultInjectionRate = atof(faultRate)) != 0.0) {
        AMUDP_FaultInjectionEnabled = 1;
        fprintf(stderr, "*** Warning: AMUDP running with fault injection enabled. Rate = %6.2f %%\n",
          100.0 * AMUDP_FaultInjectionRate);
        fflush(stderr);
        srand( (unsigned)time( NULL ) ); /* TODO: we should really be using a private rand num generator */
      }
    }

    #ifdef UETH
    { int retval = ueth_kill_link_on_signal(SIGUSR2);
      if (retval != UETH_OK)
        AMUDP_RETURN_ERRFR(RESOURCE, AM_Init, "ueth_kill_link_on_signal() failed");
    }
    #endif
  }
  amudp_Initialized++;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_Terminate() {
  int i;
  int retval = AM_OK;
  AMUDP_CHECKINIT();

  if (amudp_Initialized == 1) { /* last termination call */
    for (i = 0; i < AMUDP_numBundles; i++) {
      if (AM_FreeBundle(AMUDP_bundles[i]) != AM_OK) 
        retval = AM_ERR_RESOURCE;
    }
    AMUDP_numBundles = 0;
    #ifdef WINSOCK
      if (WSACleanup()) retval = AM_ERR_RESOURCE;
    #endif
  }

  amudp_Initialized--;
  AMUDP_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * endpoint/bundle management
 *------------------------------------------------------------------------------------ */
extern int AM_AllocateBundle(int type, eb_t *endb) {
  eb_t eb;
  AMUDP_CHECKINIT();
  if (type < 0 || type >= AM_NUM_BUNDLE_MODES) AMUDP_RETURN_ERR(BAD_ARG);
  if (type != AM_SEQ) AMUDP_RETURN_ERR(RESOURCE);
  if (AMUDP_numBundles == AMUDP_MAX_BUNDLES-1) AMUDP_RETURN_ERR(RESOURCE);
  if (!endb) AMUDP_RETURN_ERR(BAD_ARG);

  eb = (eb_t)AMUDP_malloc(sizeof(struct amudp_eb));
  eb->endpoints = (ep_t *)AMUDP_malloc(AMUDP_INITIAL_NUMENDPOINTS*sizeof(ep_t));
  eb->cursize = AMUDP_INITIAL_NUMENDPOINTS;
  eb->n_endpoints = 0;
  eb->event_mask = AM_NOEVENTS;

  AMUDP_bundles[AMUDP_numBundles++] = eb; /* keep track of all bundles */
  *endb = eb;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_FreeBundle(eb_t bundle) {
  if (!bundle) AMUDP_RETURN_ERR(BAD_ARG);
  { int i;

    /* free all constituent endpoints */
    for (i = 0; i < bundle->n_endpoints; i++) {
      int retval = AM_FreeEndpoint(bundle->endpoints[i]);
      if (retval != AM_OK) AMUDP_RETURN(retval);
    }
    AMUDP_assert(bundle->n_endpoints == 0);

    /* remove from bundle list */
    for (i = 0; i < AMUDP_numBundles; i++) {
      if (AMUDP_bundles[i] == bundle) { 
        AMUDP_bundles[i] = AMUDP_bundles[AMUDP_numBundles-1]; 
        break;
      }
    }
    AMUDP_assert(i < AMUDP_numBundles);
    AMUDP_numBundles--;

    AMUDP_free(bundle->endpoints);
    AMUDP_free(bundle);
  }
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_AllocateEndpoint(eb_t bundle, ep_t *endp, en_t *endpoint_name) {
  ep_t ep;
  int retval;

  AMUDP_CHECKINIT();
  if (!bundle || !endp || !endpoint_name) AMUDP_RETURN_ERR(BAD_ARG);

  ep = (ep_t)AMUDP_malloc(sizeof(struct amudp_ep));
  retval = AMUDP_AllocateEndpointResource(ep);
  if (retval != AM_OK) {
    AMUDP_free(ep);
    AMUDP_RETURN(retval);
  }

  /* setup eb<->ep link */
  AMUDP_InsertEndpoint(bundle, ep);
  ep->eb = bundle;

  /* initialize ep data */
  { int i;
    for (i = 0; i < AMUDP_MAX_NUMTRANSLATIONS; i++) {
      ep->translation[i].inuse = FALSE;
    }
    ep->handler[0] = amudp_defaultreturnedmsg_handler;
    for (i = 1; i < AMUDP_MAX_NUMHANDLERS; i++) {
      ep->handler[i] = amudp_unused_handler;
    }
    ep->tag = AM_NONE;
    ep->segAddr = NULL;
    ep->segLength = 0;
    ep->P = 0; 
    ep->depth = -1;
    ep->PD = 0;
    ep->preHandlerCallback = NULL; 
    ep->postHandlerCallback = NULL;

    ep->stats = AMUDP_initial_stats;
  }

  *endp = ep;
  *endpoint_name = ep->name;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_FreeEndpoint(ep_t ea) {
  int retval = AM_OK;
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (!AMUDP_ContainsEndpoint(ea->eb, ea)) AMUDP_RETURN_ERR(RESOURCE);

  if (!AMUDP_FreeEndpointResource(ea)) retval = AM_ERR_RESOURCE;
  if (!AMUDP_FreeEndpointBuffers(ea)) retval = AM_ERR_RESOURCE;

  AMUDP_RemoveEndpoint(ea->eb, ea);
  AMUDP_free(ea);
  AMUDP_RETURN(retval);
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MoveEndpoint(ep_t ea, eb_t from_bundle, eb_t to_bundle) {
  AMUDP_CHECKINIT();
  if (!ea || !from_bundle || !to_bundle) AMUDP_RETURN_ERR(BAD_ARG);
  if (!AMUDP_ContainsEndpoint(from_bundle, ea)) AMUDP_RETURN_ERR(RESOURCE);

  AMUDP_RemoveEndpoint(from_bundle, ea);
  AMUDP_InsertEndpoint(to_bundle, ea);
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Tag management
 *------------------------------------------------------------------------------------ */
extern int AM_SetTag(ep_t ea, tag_t tag) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);

  /*  TODO: return mismatched messages to sender */
  ea->tag = tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTag(ep_t ea, tag_t *tag) {
  AMUDP_CHECKINIT();
  if (!ea || !tag) AMUDP_RETURN_ERR(BAD_ARG);

  *tag = ea->tag;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * VM Segment management
 *------------------------------------------------------------------------------------ */
extern int AM_GetSeg(ep_t ea, void **addr, uintptr_t *nbytes) {
  AMUDP_CHECKINIT();
  if (!ea || !addr || !nbytes) AMUDP_RETURN_ERR(BAD_ARG);
  *addr = ea->segAddr;
  *nbytes = ea->segLength;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetSeg(ep_t ea, void *addr, uintptr_t nbytes) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (nbytes > AMUDP_MAX_SEGLENGTH) AMUDP_RETURN_ERR(BAD_ARG);

  ea->segAddr = addr;
  ea->segLength = nbytes;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MaxSegLength(uintptr_t* nbytes) {
  AMUDP_CHECKINIT();
  if (!nbytes) AMUDP_RETURN_ERR(BAD_ARG);
  *nbytes = AMUDP_MAX_SEGLENGTH;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Translation management
 *------------------------------------------------------------------------------------ */
extern int AM_Map(ep_t ea, int index, en_t name, tag_t tag) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->translation[index].inuse) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to re-map */
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */

  ea->translation[index].inuse = TRUE;
  ea->translation[index].name = name;
  ea->translation[index].tag = tag;
  ea->P++;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MapAny(ep_t ea, int *index, en_t name, tag_t tag) {
  AMUDP_CHECKINIT();
  if (!ea || !index) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */

  { int i;
    for (i = 0; i < AMUDP_MAX_NUMTRANSLATIONS; i++) {
      if (!ea->translation[i].inuse) { /* use this one */
        ea->translation[i].inuse = TRUE;
        ea->translation[i].name = name;
        ea->translation[i].tag = tag;
        ea->P++;  /* track num of translations */
        *index = i;
        return AM_OK;
      }
    }
    AMUDP_RETURN_ERR(RESOURCE); /* none available */
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AM_UnMap(ep_t ea, int index) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMUDP_RETURN_ERR(RESOURCE); /* not mapped */
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to unmap after call to AM_SetExpectedResources */

  ea->translation[index].inuse = FALSE;
  ea->P--;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationInuse(ep_t ea, int i) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);

  if (ea->translation[i].inuse) return AM_OK; /* in use */
  else return AM_ERR_RESOURCE; /* don't complain here - it's a common case */
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag) {
  AMUDP_CHECKINIT();
  if (!ea || !tag) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);
  if (!ea->translation[i].inuse) AMUDP_RETURN_ERR(RESOURCE);

  (*tag) = ea->translation[i].tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SetTranslationTag(ep_t ea, int index, tag_t tag) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMUDP_RETURN_ERR(RESOURCE); /* can't change tag if not mapped */

  ea->translation[index].tag = tag;

  if (ea->depth != -1) { /* after call to AM_SetExpectedResources we must update compressed table */
    ea->perProcInfo[ea->translation[index].id].tag = tag;
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationName(ep_t ea, int i, en_t *gan) {
  AMUDP_CHECKINIT();
  if (!ea || !gan) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);
  if (!ea->translation[i].inuse) AMUDP_RETURN_ERR(RESOURCE);

  (*gan) = ea->translation[i].name; 
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests) {
  static int firsttime = 1;
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to call AM_SetExpectedResources again */
  /* n_endpoints ignored : P is set as we Map translations */
  /*if (n_endpoints < 1 || n_endpoints >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);*/
  if (n_outstanding_requests < 1 || n_outstanding_requests > AMUDP_MAX_NETWORKDEPTH) AMUDP_RETURN_ERR(BAD_ARG);

  ea->depth = n_outstanding_requests;
  ea->PD = ea->P * ea->depth;

  if (!AMUDP_AllocateEndpointBuffers(ea)) AMUDP_RETURN_ERR(RESOURCE);

  /*  compact a copy of the translation table into our perproc info array */
  { int procid = 0;
    int i;
    for (i=0; i < AMUDP_MAX_NUMTRANSLATIONS; i++) {
      if (ea->translation[i].inuse) {
        ea->perProcInfo[procid].remoteName = ea->translation[i].name;
        ea->perProcInfo[procid].tag = ea->translation[i].tag;
        ea->translation[i].id = (uint16_t)procid;
        procid++;
        if (procid == ea->P) break; /*  should have all of them now */
      }
    }
  }

  #ifdef UETH
  { /* need to init the request/reply destinations */
    for (int inst = 0; inst < ea->depth; inst++) {
      for (int procid = 0; procid < ea->P; procid++) {
        amudp_buf_t *reqbuf = GET_REQ_BUF(ea, procid, inst);
        amudp_buf_t *repbuf = GET_REP_BUF(ea, procid, inst);
        if (ueth_set_packet_destination(reqbuf, &ea->perProcInfo[procid].remoteName) != UETH_OK)
          AMUDP_RETURN_ERRFR(RESOURCE, AM_SetExpectedResources, "ueth_set_packet_destination failed");
        if (ueth_set_packet_destination(repbuf, &ea->perProcInfo[procid].remoteName) != UETH_OK)
          AMUDP_RETURN_ERRFR(RESOURCE, AM_SetExpectedResources, "ueth_set_packet_destination failed");
      }
    }
  }
  #endif

  if (firsttime) { /* set transfer parameters */
    #define ENVINT_WITH_DEFAULT(var, name, validate) do { \
        long val;                                         \
        char defval[80];                                  \
        const char *valstr;                               \
        sprintf(defval, "%u", (unsigned int)var);         \
        valstr = AMUDP_getenv_prefixed_withdefault(name,defval); \
        if (valstr) {                                     \
           val = atol(valstr);                            \
           if ((int64_t)val != (int64_t)(int32_t)val)     \
            AMUDP_FatalErr(name" setting too large!");    \
           var = val;                                     \
           validate;                                      \
        }                                                 \
      } while (0)

    ENVINT_WITH_DEFAULT(AMUDP_MaxRequestTimeout_us, "REQUESTTIMEOUT_MAX",
                        { if (val <= 0) AMUDP_MaxRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_InitialRequestTimeout_us, "REQUESTTIMEOUT_INITIAL",
                        { if (val <= 0) AMUDP_InitialRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_RequestTimeoutBackoff, "REQUESTTIMEOUT_BACKOFF",
                        { if (val <= 1) AMUDP_FatalErr("REQUESTTIMEOUT_BACKOFF must be > 1"); });
    ENVINT_WITH_DEFAULT(AMUDP_ExpectedBandwidth, "EXPECTED_BANDWIDTH",
                        { if (val < 1) AMUDP_FatalErr("EXPECTED_BANDWIDTH must be >= 1"); });
    if (AMUDP_InitialRequestTimeout_us > AMUDP_MaxRequestTimeout_us) 
       AMUDP_FatalErr("INITIAL_REQUESTTIMEOUT must not exceed MAX_REQUESTTIMEOUT");
    #if 0
      printf("AMUDP_MaxRequestTimeout_us=%08x\n",AMUDP_MaxRequestTimeout_us);
      printf("AMUDP_InitialRequestTimeout_us=%08x\n",AMUDP_InitialRequestTimeout_us);
      printf("AMUDP_RequestTimeoutBackoff=%08x\n",AMUDP_RequestTimeoutBackoff);
      printf("AMUDP_ExpectedBandwidth=%08x\n",AMUDP_ExpectedBandwidth);
    #endif
    firsttime = 0;
  }
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Handler management
 *------------------------------------------------------------------------------------ */
extern int _AM_SetHandler(ep_t ea, handler_t handler, amudp_handler_fn_t function) {
  AMUDP_CHECKINIT();
  if (!ea || !function) AMUDP_RETURN_ERR(BAD_ARG);
  if (AMUDP_BADHANDLERVAL(handler)) AMUDP_RETURN_ERR(BAD_ARG);

  ea->handler[handler] = function;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, amudp_handler_fn_t function) {
  int i;
  AMUDP_CHECKINIT();
  if (!ea || !function || !handler) AMUDP_RETURN_ERR(BAD_ARG);

  for (i = 1 ; i < AMUDP_MAX_NUMHANDLERS; i++) {
    if (ea->handler[i] == amudp_unused_handler) { /* find unused entry */
      ea->handler[i] = function;
      *handler = (handler_t)i;
      return AM_OK;
    }
  }
  AMUDP_RETURN_ERR(RESOURCE); /* all in use */
}
/*------------------------------------------------------------------------------------
 * Event management
 *------------------------------------------------------------------------------------ */
extern int AM_GetEventMask(eb_t eb, int *mask) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!eb),BAD_ARG);

  *mask = eb->event_mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetEventMask(eb_t eb, int mask) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!eb),BAD_ARG);
  AMUDP_CHECK_ERR((mask < 0 || ((amudp_eventmask_t)mask) >= AM_NUMEVENTMASKS),BAD_ARG);

  eb->event_mask = (uint8_t)mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_WaitSema(eb_t eb) {
  int retval;
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!eb),BAD_ARG);
  
  if (eb->event_mask == AM_NOEVENTS) 
    AMUDP_FatalErr("it's an error to block when the mask is not set - will never return");

  /* block here until a message arrives */
  retval = AMUDP_Block(eb);
  if (retval != AM_OK) eb->event_mask = AM_NOEVENTS;

  /* it's not clear from the spec whether we should poll here, 
     but it's probably safer to do so than not */
  if (retval == AM_OK) 
    retval = AM_Poll(eb);

  AMUDP_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * Message interrogation
 *------------------------------------------------------------------------------------ */
extern int AM_GetSourceEndpoint(void *token, en_t *gan) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !gan),BAD_ARG);

  *gan = ((amudp_buf_t *)token)->status.sourceAddr;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_GetSourceId(void *token, int *srcid) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !srcid),BAD_ARG);

  *srcid = ((amudp_buf_t *)token)->status.sourceId;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetDestEndpoint(void *token, ep_t *endp) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !endp),BAD_ARG);

  *endp = ((amudp_buf_t *)token)->status.dest;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetMsgTag(void *token, tag_t *tagp) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !tagp),BAD_ARG);
  
  if (((amudp_buf_t *)token)->status.bulkBuffer)
    *tagp = ((amudp_buf_t *)token)->status.bulkBuffer->Msg.tag;
  else *tagp = ((amudp_buf_t *)token)->Msg.tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SetHandlerCallbacks(ep_t ep, AMUDP_preHandlerCallback_t preHandlerCallback, 
                                              AMUDP_postHandlerCallback_t postHandlerCallback) {
  AMUDP_CHECKINIT();
  if (!ep) AMUDP_RETURN_ERR(BAD_ARG);
  ep->preHandlerCallback = preHandlerCallback;
  ep->postHandlerCallback = postHandlerCallback;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Statistics API
 *------------------------------------------------------------------------------------ */
extern int AMUDP_GetEndpointStatistics(ep_t ep, amudp_stats_t *stats) { /* called by user to get statistics */
  AMUDP_CHECKINIT();
  if (!ep || !stats) AMUDP_RETURN_ERR(BAD_ARG);
  memcpy(stats, &ep->stats, sizeof(amudp_stats_t));
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_ResetEndpointStatistics(ep_t ep) {
  AMUDP_CHECKINIT();
  if (!ep) AMUDP_RETURN_ERR(BAD_ARG);
  ep->stats = AMUDP_initial_stats;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_AggregateStatistics(amudp_stats_t *runningsum, amudp_stats_t *newvalues) {
  int category;
  AMUDP_CHECKINIT();
  if (!runningsum || !newvalues) AMUDP_RETURN_ERR(BAD_ARG);
  for (category = 0; category < amudp_NumCategories; category++) {
    runningsum->RequestsSent[category] += newvalues->RequestsSent[category];
    runningsum->RequestsRetransmitted[category] += newvalues->RequestsRetransmitted[category];
    runningsum->RequestsReceived[category] += newvalues->RequestsReceived[category];
    runningsum->RepliesSent[category] += newvalues->RepliesSent[category];
    runningsum->RepliesRetransmitted[category] += newvalues->RepliesRetransmitted[category];
    runningsum->RepliesReceived[category] += newvalues->RepliesReceived[category];

    runningsum->RequestDataBytesSent[category] += newvalues->RequestDataBytesSent[category];
    runningsum->ReplyDataBytesSent[category] += newvalues->ReplyDataBytesSent[category];
    runningsum->RequestTotalBytesSent[category] += newvalues->RequestTotalBytesSent[category];
    runningsum->ReplyTotalBytesSent[category] += newvalues->ReplyTotalBytesSent[category];
  }
  runningsum->ReturnedMessages += newvalues->ReturnedMessages;
  #if AMUDP_COLLECT_LATENCY_STATS
    runningsum->RequestSumLatency += newvalues->RequestSumLatency;
    if (newvalues->RequestMinLatency < runningsum->RequestMinLatency)
      runningsum->RequestMinLatency = newvalues->RequestMinLatency;
    if (newvalues->RequestMaxLatency > runningsum->RequestMaxLatency)
      runningsum->RequestMaxLatency = newvalues->RequestMaxLatency;
  #endif

  runningsum->TotalBytesSent += newvalues->TotalBytesSent;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_StatPrecision(double val) {
    int prec = 3; 
    while (val >= 10.0 && prec > 0) { val /= 10; prec -= 1; }
    return prec;
}
/* ------------------------------------------------------------------------------------ */
extern const char *AMUDP_DumpStatistics(void *_fp, amudp_stats_t *stats, int globalAnalysis) {
  FILE *fp = (FILE *)_fp;
  static char msg[4096];
  int64_t requestsSent = 0; 
  int64_t requestsRetransmitted = 0; 
  int64_t requestsReceived = 0; 
  int64_t repliesSent = 0; 
  int64_t repliesRetransmitted = 0; 
  int64_t repliesReceived = 0; 
  int64_t reqdataBytesSent = 0; 
  int64_t repdataBytesSent = 0; 
  int64_t reqTotalBytesSent = 0; 
  int64_t repTotalBytesSent = 0; 
  double reqavgpayload[amudp_NumCategories];
  double repavgpayload[amudp_NumCategories];
  double avgpayload[amudp_NumCategories];
  int64_t reqUDPIPheaderbytes, repUDPIPheaderbytes; 
  int category;

  AMUDP_assert(amudp_Initialized);
  AMUDP_assert(stats != NULL);
  getCPUTicks(); /* ensure this has been called at least once, even if stats are empty */

  #if !AMUDP_COLLECT_STATS
    sprintf(msg, "(AMUDP_COLLECT_STATS disabled)\n");
    if (fp != NULL) fprintf(fp, "%s", msg);
    return msg;
  #endif

  for (category = 0; category < amudp_NumCategories; category++) {
    requestsSent += stats->RequestsSent[category];
    requestsRetransmitted += stats->RequestsRetransmitted[category];
    requestsReceived += stats->RequestsReceived[category];
    repliesSent += stats->RepliesSent[category];
    repliesRetransmitted += stats->RepliesRetransmitted[category];
    repliesReceived += stats->RepliesReceived[category];
    reqdataBytesSent += stats->RequestDataBytesSent[category];
    repdataBytesSent += stats->ReplyDataBytesSent[category];
    reqTotalBytesSent += stats->RequestTotalBytesSent[category];
    repTotalBytesSent += stats->ReplyTotalBytesSent[category];
    if (stats->RequestsSent[category] == 0) 
      reqavgpayload[category] = 0.0;
    else 
      reqavgpayload[category] = 
        stats->RequestDataBytesSent[category] / (double)(stats->RequestsSent[category]);

    if (stats->RepliesSent[category] == 0) 
      repavgpayload[category] = 0.0;
    else 
      repavgpayload[category] = 
        stats->ReplyDataBytesSent[category] / (double)(stats->RepliesSent[category]);

    if (stats->RequestsSent[category] + stats->RepliesSent[category] == 0) 
      avgpayload[category] = 0.0;
    else 
      avgpayload[category] = 
        (stats->RequestDataBytesSent[category] + stats->ReplyDataBytesSent[category]) / 
          (double)(stats->RequestsSent[category] + stats->RepliesSent[category]);
  }
 {
  int64_t dataBytesSent = reqdataBytesSent + repdataBytesSent;
  int64_t packetssent = (requestsSent + requestsRetransmitted + 
                         repliesSent  + repliesRetransmitted);

  double avgreqdata = (requestsSent > 0 ?  reqdataBytesSent / (double)requestsSent : 0.0);
  double avgrepdata = (repliesSent  > 0 ?  repdataBytesSent / (double)repliesSent : 0.0);
  double avgdata = (packetssent  > 0 ?  dataBytesSent    / (double)packetssent : 0.0);

  double avgreqpacket = (requestsSent > 0 ?
      ((double)(reqTotalBytesSent)) / ((double)requestsSent + requestsRetransmitted)
      : 0.0);
  double avgreppacket = (repliesSent > 0 ?
      ((double)(repTotalBytesSent)) / ((double)repliesSent + repliesRetransmitted)
      : 0.0);
  double avgpacket =(packetssent > 0 ?
      ((double)(stats->TotalBytesSent)) / ((double)packetssent)
      : 0.0);

  #ifdef UETH 
    reqUDPIPheaderbytes = 0; /* n/a- all headers already included */
    repUDPIPheaderbytes = 0; /* n/a- all headers already included */
  #else
    { int packetoverhead = (20 /* IP header */ + 8  /* UDP header*/);
      reqUDPIPheaderbytes = (requestsSent + requestsRetransmitted) * packetoverhead;
      repUDPIPheaderbytes = (repliesSent + repliesRetransmitted) * packetoverhead;
      avgreqpacket += packetoverhead;
      avgreppacket += packetoverhead;
      avgpacket += packetoverhead;
    }
  #endif

  /* batch lines together to improve chance of output together */
  sprintf(msg, 
    " Requests: %8lu sent, %4lu retransmitted, %8lu received\n"
    " Replies:  %8lu sent, %4lu retransmitted, %8lu received\n"
    " Returned messages:  %8lu\n"
  #if AMUDP_COLLECT_LATENCY_STATS
    "Latency (request sent to reply received): \n"
    " min: %8i microseconds\n"
    " max: %8i microseconds\n"
    " avg: %8i microseconds\n"
  #endif

    "Message Breakdown:        Requests     Replies   Avg data sz (Req/Rep/Both)\n"
    " Small  (<=%5i bytes)   %8lu    %8lu  %9.*f/%.*f/%.*f bytes\n"
    " Medium (<=%5i bytes)   %8lu    %8lu  %9.*f/%.*f/%.*f bytes\n"
    " Large  (<=%5i bytes)   %8lu    %8lu  %9.*f/%.*f/%.*f bytes\n"

  #if !USE_TRUE_BULK_XFERS
    " ^^^^^ (Statistics for Large refer to internal fragments)\n"
  #endif
    " Total                                          %9.*f/%.*f/%.*f bytes\n"

    "Data bytes sent:      %lu/%lu/%lu bytes\n"
    "Total bytes sent:     %lu/%lu/%lu bytes (incl. AM overhead)\n"
    "Bandwidth overhead:   %.2f%%/%.2f%%/%.2f%%\n"        
    "Average packet size:  %.*f/%.*f/%.*f bytes (incl. AM & transport-layer overhead)\n"
    , 
    (unsigned long)requestsSent, (unsigned long)requestsRetransmitted, (unsigned long)requestsReceived,
    (unsigned long)repliesSent, (unsigned long)repliesRetransmitted, (unsigned long)repliesReceived,
    (unsigned long)stats->ReturnedMessages,
  #if AMUDP_COLLECT_LATENCY_STATS
    (stats->RequestMinLatency == (amudp_cputick_t)-1?(int)-1:(int)ticks2us(stats->RequestMinLatency)),
    (int)ticks2us(stats->RequestMaxLatency),
    (requestsSent>0?(int)(ticks2us(stats->RequestSumLatency) / requestsSent):-1),
  #endif

    /* Message breakdown */
    (int)(AMUDP_MAX_SHORT*sizeof(int)),
      (unsigned long)stats->RequestsSent[amudp_Short], (unsigned long)stats->RepliesSent[amudp_Short], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Short]), reqavgpayload[amudp_Short], 
      AMUDP_StatPrecision(repavgpayload[amudp_Short]), repavgpayload[amudp_Short], 
      AMUDP_StatPrecision(avgpayload[amudp_Short]), avgpayload[amudp_Short], 
    (int)(AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_MEDIUM),
      (unsigned long)stats->RequestsSent[amudp_Medium], (unsigned long)stats->RepliesSent[amudp_Medium], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Medium]), reqavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(repavgpayload[amudp_Medium]), repavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(avgpayload[amudp_Medium]), avgpayload[amudp_Medium], 
  #if USE_TRUE_BULK_XFERS
    (int)(AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_LONG),
  #else
    (int)(AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_MEDIUM),
  #endif
      (unsigned long)stats->RequestsSent[amudp_Long], (unsigned long)stats->RepliesSent[amudp_Long], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Long]), reqavgpayload[amudp_Long], 
      AMUDP_StatPrecision(repavgpayload[amudp_Long]), repavgpayload[amudp_Long], 
      AMUDP_StatPrecision(avgpayload[amudp_Long]), avgpayload[amudp_Long], 

    /* avg data payload */
    AMUDP_StatPrecision(avgreqdata), avgreqdata,
    AMUDP_StatPrecision(avgrepdata), avgrepdata,
    AMUDP_StatPrecision(avgdata), avgdata,

    (unsigned long)reqdataBytesSent, (unsigned long)repdataBytesSent, (unsigned long)dataBytesSent,

    (unsigned long)reqTotalBytesSent, (unsigned long)repTotalBytesSent, (unsigned long)stats->TotalBytesSent,
    /* bandwidth overhead */
    (reqTotalBytesSent > 0 ?
      100.0*((double)(reqTotalBytesSent + reqUDPIPheaderbytes - reqdataBytesSent)) / 
      ((double)reqTotalBytesSent + reqUDPIPheaderbytes) 
      : 0.0),
    (repTotalBytesSent > 0 ?
      100.0*((double)(repTotalBytesSent + repUDPIPheaderbytes - repdataBytesSent)) / 
      ((double)repTotalBytesSent + repUDPIPheaderbytes) 
      : 0.0),
    (stats->TotalBytesSent > 0 ?
      100.0*((double)(stats->TotalBytesSent + reqUDPIPheaderbytes + repUDPIPheaderbytes - dataBytesSent)) / 
      ((double)stats->TotalBytesSent + reqUDPIPheaderbytes + repUDPIPheaderbytes) 
      : 0.0),
    /* avg packet size */
    AMUDP_StatPrecision(avgreqpacket), avgreqpacket,
    AMUDP_StatPrecision(avgreppacket), avgreppacket,
    AMUDP_StatPrecision(avgpacket), avgpacket
  );

  if (globalAnalysis) {
    int64_t reqsent = requestsSent + requestsRetransmitted;
    int64_t reqlost = reqsent - requestsReceived;
    int64_t repsent = repliesSent + repliesRetransmitted;
    int64_t replost = repsent - repliesReceived;
    int64_t packetslost = reqlost + replost;
    int64_t extra_rexmits = requestsRetransmitted - reqlost;
    #define APPEND_PERCENT(num, denom)                                           \
      if (num > 0) sprintf(msg+strlen(msg), "  (%6.3f%%)\n", (100.0*num)/denom); \
      else strcat(msg, "\n")
    sprintf(msg+strlen(msg), "Requests lost:        %9i", (int)reqlost);
    APPEND_PERCENT(reqlost, reqsent);
    sprintf(msg+strlen(msg), "Replies lost:         %9i", (int)replost);
    APPEND_PERCENT(replost, repsent);
    sprintf(msg+strlen(msg), "Total packets lost:   %9i", (int)packetslost);
    APPEND_PERCENT(packetslost, packetssent);
    sprintf(msg+strlen(msg), "Useless retransmits:  %9i", (int)extra_rexmits);
    APPEND_PERCENT(extra_rexmits, requestsRetransmitted);
  } 

  if (fp != NULL) fprintf(fp, "%s", msg);
  return msg;
 }
}
/* ------------------------------------------------------------------------------------ */

