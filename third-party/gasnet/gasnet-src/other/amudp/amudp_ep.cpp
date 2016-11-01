/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_ep.cpp $
 * Description: AMUDP Implementations of endpoint and bundle operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "amudp_internal.h" // must come after any other headers

/* definitions for internal declarations */
int amudp_Initialized = 0;
static void AMUDP_defaultAMHandler(void * token);
amudp_handler_fn_t amudp_unused_handler = (amudp_handler_fn_t)&AMUDP_defaultAMHandler;
amudp_handler_fn_t amudp_defaultreturnedmsg_handler = (amudp_handler_fn_t)&AMUDP_DefaultReturnedMsg_Handler;
#if AMUDP_DEBUG_VERBOSE
  int AMUDP_VerboseErrors = 1;
#else
  int AMUDP_VerboseErrors = 0;
#endif
int AMUDP_PoliteSync = 0;
const char *AMUDP_ProcessLabel = NULL;
uint32_t AMUDP_RequestTimeoutBackoff = AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER;
uint32_t AMUDP_MaxRequestTimeout_us = AMUDP_MAX_REQUESTTIMEOUT_MICROSEC;
uint32_t AMUDP_InitialRequestTimeout_us = AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC;

int AMUDP_SilentMode = 0; 
AMUDP_IDENT(AMUDP_IdentString_Version, "$AMUDPLibraryVersion: " AMUDP_LIBRARY_VERSION_STR " $")

double AMUDP_FaultInjectionRate = 0.0;
double AMUDP_FaultInjectionEnabled = 0;

const amudp_stats_t AMUDP_initial_stats = /* the initial state for stats type */
        { {0,0,0}, {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
          0,0,0,
          (amudp_cputick_t)-1, 0, 0,
          {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, 
          0
        };

/* ------------------------------------------------------------------------------------ */
/* error handling */
AMUDP_FORMAT_PRINTF(AMUDP_Msg,2,0,
static int AMUDP_Msg(const char *prefix, const char *msg, va_list argptr)) {
  static char _expandedmsg[255]; // use static storage when possible for robustness in panic-mode
  static char plabel[80];

  if (AMUDP_ProcessLabel && !*plabel) snprintf(plabel, sizeof(plabel), "(%s)", AMUDP_ProcessLabel);
  char *expandedmsg = _expandedmsg;
  size_t sz = strlen(prefix) + strlen(plabel) + strlen(msg) + 8;
  if (sz > sizeof(_expandedmsg)) expandedmsg = (char *)AMUDP_malloc(sz);
  int chk = snprintf(expandedmsg, sz, "%s%s: %s\n", prefix, plabel, msg);
  AMUDP_assert(chk < (int)sz); // truncation should not occur
  int retval = vfprintf(stderr, expandedmsg, argptr);
  fflush(stderr);
  if (expandedmsg != _expandedmsg) AMUDP_free(expandedmsg);
  
  return retval; 
}

extern int AMUDP_Info(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg(AMUDP_ENV_PREFIX_STR, msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMUDP_Warn(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("*** " AMUDP_ENV_PREFIX_STR " WARNING", msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMUDP_Err(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("*** " AMUDP_ENV_PREFIX_STR " ERROR", msg, argptr);
  va_end(argptr);
  return retval;
}

extern void AMUDP_FatalErr(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); // pass in last argument
    retval = AMUDP_Msg("*** FATAL ERROR", msg, argptr);
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
    eb->endpoints = (ep_t *)AMUDP_realloc(eb->endpoints, sizeof(ep_t)*newsize);
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
 * Endpoint buffer management
 *------------------------------------------------------------------------------------ */
/* internally, buffers have a header:
 *   while allocated: pool points to the bufferpool
 *   while freed: next pointer in free list
 */
#define AMUDP_BUFFERPOOL_MAGIC ((uint64_t)0x1001feedbac31001llu)
extern amudp_buf_t *AMUDP_AcquireBuffer(ep_t ep, size_t sz) {
  AMUDP_assert(ep);
  AMUDP_assert(sz >= AMUDP_MIN_BUFFER);
  AMUDP_assert(sz <= AMUDP_MAX_BUFFER);
  amudp_bufferpool_t *pool;
  if (sz <= AMUDP_MAX_SHORT_BUFFER) {
    pool = &ep->bufferPool[0];
  } else {
    pool = &ep->bufferPool[1];
  }
  size_t poolsz = pool->buffersz;
  AMUDP_assert(sz <= poolsz);
  AMUDP_assert(pool->magic == AMUDP_BUFFERPOOL_MAGIC);

  amudp_bufferheader_t *bh;
  if (pool->free) {
    bh = pool->free;
    pool->free = bh->next;
  } else {
    bh = (amudp_bufferheader_t *)AMUDP_malloc(sizeof(amudp_bufferheader_t) + poolsz);
  }
  bh->pool = pool;

  AMUDP_memcheck(bh);

  #if AMUDP_BUFFER_STATS
    pool->stats.alloc_curr++;
    pool->stats.alloc_total++;
    pool->stats.buffer_bytes += sz;
    pool->stats.alloc_peak = MAX(pool->stats.alloc_curr,pool->stats.alloc_peak);
  #endif

  amudp_buf_t *buf = (amudp_buf_t *)(bh+1);
  AMUDP_assert(!((uintptr_t)buf & 0x7)); // 8-byte alignment
  return buf;
}
/* ------------------------------------------------------------------------------------ */
extern void AMUDP_ReleaseBuffer(ep_t ep, amudp_buf_t *buf) {
  AMUDP_assert(ep);
  AMUDP_assert(buf);
  amudp_bufferheader_t *bh = ((amudp_bufferheader_t *)buf) - 1;
  AMUDP_memcheck(bh);
  amudp_bufferpool_t *pool = bh->pool;
  AMUDP_assert(pool->magic == AMUDP_BUFFERPOOL_MAGIC);
  bh->next = pool->free;
  pool->free = bh;
  #if AMUDP_BUFFER_STATS
    pool->stats.alloc_curr--;
  #endif
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_InitBuffers(ep_t ep) {
  for (int i=0; i < AMUDP_NUMBUFFERPOOLS; i++) {
    ep->bufferPool[i].free = NULL;
    #if AMUDP_DEBUG
      ep->bufferPool[i].magic = AMUDP_BUFFERPOOL_MAGIC;
    #endif
  }
  ep->bufferPool[0].buffersz = AMUDP_MAX_SHORT_BUFFER;
  ep->bufferPool[1].buffersz = AMUDP_MAX_BUFFER;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_FreeAllBuffers(ep_t ep) {
  AMUDP_memcheck_all();

  for (int i=0; i < AMUDP_NUMBUFFERPOOLS; i++) {
    amudp_bufferpool_t *pool = &ep->bufferPool[i];
    for (amudp_bufferheader_t *bh = pool->free; bh; ) {
      amudp_bufferheader_t *next = bh->next;
      AMUDP_free(bh);
      bh = next;
    }
    #if AMUDP_BUFFER_STATS
      AMUDP_Info("Buffer pool %5i: %7.1fb avg\t%6llu alloc\t%4llu peak\t%2llu leaked",
                 (int)pool->buffersz, 
                 pool->stats.buffer_bytes/(double)pool->stats.alloc_total,
                 (unsigned long long)pool->stats.alloc_total,
                 (unsigned long long)pool->stats.alloc_peak,
                 (unsigned long long)pool->stats.alloc_curr
                 );
    #endif
  }
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
#if USE_SOCKET_RECVBUFFER_GROW
extern int AMUDP_growSocketBufferSize(ep_t ep, int targetsize, 
                                       int szparam, const char *paramname) {
  int initialsize; /* original socket recv size */
  int maxedout = 0;
  GETSOCKOPT_LENGTH_T junk = sizeof(int);
  if (SOCK_getsockopt(ep->s, SOL_SOCKET, szparam, (char *)&initialsize, &junk) == SOCKET_ERROR) {
    AMUDP_DEBUG_WARN(("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s",paramname,strerror(errno)));
    initialsize = 65535;
  } 
  if (szparam == SO_RCVBUF)
    ep->socketRecvBufferSize = initialsize; /* ensure ep->socketRecvBufferSize is always initialized */

  targetsize = MAX(initialsize, targetsize); /* never shrink buffer */

  #if 0 && (PLATFORM_OS_LINUX || PLATFORM_OS_UCLINUX) /* it appears this max means nothing */
  // this code requires <linux/unistd.h> and <linux/sysctl.h>
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
      AMUDP_VERBOSE_INFO(("setsockopt(SOL_SOCKET, %s, %i) on UDP socket failed: %s", paramname, targetsize, strerror(errno)));
    } else {
      int temp = targetsize;
      junk = sizeof(int);
      if (SOCK_getsockopt(ep->s, SOL_SOCKET, szparam, (char *)&temp, &junk) == SOCKET_ERROR) {
        AMUDP_DEBUG_WARN(("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s", paramname, strerror(errno)));
      }
      if (temp >= targetsize) {
        if (!AMUDP_SilentMode) AMUDP_Info("UDP %s buffer successfully set to %i bytes", paramname, targetsize);
        if (szparam == SO_RCVBUF) ep->socketRecvBufferSize = temp;
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
    /* allocate socket */
    ep->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ep->s == INVALID_SOCKET) 
      AMUDP_RETURN_ERRFR(RESOURCE, socket, strerror(errno));

    ep->name.sin_family = AF_INET;
    ep->name.sin_port = 0; /* any port */
    ep->name.sin_addr.s_addr = htonl(AMUDP_currentUDPInterface);
    memset(&ep->name.sin_zero, 0, sizeof(ep->name.sin_zero));

    if (bind(ep->s, (struct sockaddr*)&ep->name, sizeof(struct sockaddr)) == SOCKET_ERROR) {
      closesocket(ep->s);
      AMUDP_RETURN_ERRFR(RESOURCE, bind, strerror(errno));
    }
    { /*  danger: this might fail on multi-homed hosts if AMUDP_currentUDPInterface was not set*/
      GETSOCKNAME_LENGTH_T sz = sizeof(en_t);
      if (SOCK_getsockname(ep->s, (struct sockaddr*)&ep->name, &sz) == SOCKET_ERROR) {
        closesocket(ep->s);
        AMUDP_RETURN_ERRFR(RESOURCE, getsockname, strerror(errno));
      }
      /* can't determine interface address */
      if (ep->name.sin_addr.s_addr == INADDR_ANY) {
        closesocket(ep->s);
        AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_AllocateEndpointResource,
                           "AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface address");
      }
      if (ep->name.sin_port == 0) {
        closesocket(ep->s);
        AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_AllocateEndpointResource,
                           "AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface port");
      }
    }

    ep->translationsz = AMUDP_INIT_NUMTRANSLATIONS;
    ep->translation = (amudp_translation_t *)AMUDP_calloc(ep->translationsz, sizeof(amudp_translation_t));

    return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_AllocateEndpointBuffers(ep_t ep) {
  int PD = ep->PD;
  amudp_buf_t *pool;

  AMUDP_assert(ep != NULL);
  AMUDP_assert(ep->depth >= 1);
  AMUDP_assert(ep->P > 0 && ep->P <= AMUDP_MAX_NUMTRANSLATIONS);
  AMUDP_assert(ep->PD == (int)ep->P * ep->depth);
  AMUDP_assert(ep->recvDepth > 0 && ep->recvDepth <= AMUDP_MAX_RECVDEPTH);
  AMUDP_assert(ep->sendDepth > 0 && ep->sendDepth <= (int)MAX(1,ep->P - 1) * ep->depth);

  AMUDP_assert(sizeof(amudp_buf_t) % sizeof(int) == 0); /* assume word-addressable machine */

  // setup initial socket OS buffer size
  { /* theoretical max required by HPAM is 2*PD*AMUDP_MAX_MSG, but that scales poorly */
    int sz = MIN(ep->recvDepth*AMUDP_MAX_MSG, AMUDP_SOCKETBUFFER_MAX);
     
    #if USE_SOCKET_RECVBUFFER_GROW
        ep->socketRecvBufferMaxedOut = AMUDP_growSocketBufferSize(ep, sz, SO_RCVBUF, "SO_RCVBUF");
    #endif
    #if USE_SOCKET_SENDBUFFER_GROW
        AMUDP_growSocketBufferSize(ep, sz, SO_SNDBUF, "SO_SNDBUF");
    #endif
  }

  /* instance hint pointers & compressed translation table */
  ep->perProcInfo = (amudp_perproc_info_t *)AMUDP_calloc(ep->P, sizeof(amudp_perproc_info_t));

  AMUDP_InitBuffers(ep);

  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointResource(ep_t ep) {
  AMUDP_assert(ep != NULL);
  /*  close UDP port */
  #ifdef AMUDP_BLCR_ENABLED
    if (AMUDP_SPMDRestartActive) { /* it is already gone */ } else
  #endif

  if (ep->translation) AMUDP_free(ep->translation);

  if (closesocket(ep->s) == SOCKET_ERROR) return FALSE;
  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointBuffers(ep_t ep) {
  AMUDP_assert(ep != NULL);

  for (amudp_node_t proc=0; proc < ep->P; proc++) { // release tx buffers in use
    for (int t=0; t < 2; t++) {
      amudp_bufdesc_t *desc = (t ? ep->perProcInfo[proc].requestDesc : ep->perProcInfo[proc].replyDesc );
      if (desc) {
        for (int i=0; i < ep->depth; i++) { 
          amudp_buf_t *buf = desc[i].buffer;
          if (buf) AMUDP_ReleaseBuffer(ep, buf);
        }
        AMUDP_free(desc);
      }
    }
  }
  ep->timeoutCheckPosn = NULL;
  ep->outstandingRequests = 0;

  for (amudp_buf_t *buf = ep->rxHead; buf; ) { // release rx buffers in use
    amudp_buf_t *tmp = buf->status.rx.next;
    AMUDP_ReleaseBuffer(ep, buf);
    buf = tmp;
  }
  ep->rxHead = NULL;
  ep->rxTail = NULL;
  ep->rxCnt = 0;

  AMUDP_FreeAllBuffers(ep);

  AMUDP_free(ep->perProcInfo);
  ep->perProcInfo = NULL;

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

    AMUDP_assert(sizeof(amudp_msg_t) % 4 == 0); // may be required for correct argument alignment
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

  ep = (ep_t)AMUDP_calloc(1, sizeof(struct amudp_ep));
  retval = AMUDP_AllocateEndpointResource(ep);
  if (retval != AM_OK) {
    AMUDP_free(ep);
    AMUDP_RETURN(retval);
  }

  /* setup eb<->ep link */
  AMUDP_InsertEndpoint(bundle, ep);
  ep->eb = bundle;

  { /* initialize ep data */
    ep->handler[0] = amudp_defaultreturnedmsg_handler;
    for (int i = 1; i < AMUDP_MAX_NUMHANDLERS; i++) {
      ep->handler[i] = amudp_unused_handler;
    }
    ep->tag = AM_NONE;
    ep->depth = -1;

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
  if (ea->depth != -1) {
    if (!AMUDP_FreeEndpointBuffers(ea)) retval = AM_ERR_RESOURCE;
  }

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
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->translation[index].inuse) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to re-map */

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

  amudp_node_t i;
  for (i = 0; i < ea->translationsz; i++) { /* find a free entry, possibly a middle hole */
    if (!ea->translation[i].inuse) break; /* use this one */
  }
  if (i == ea->translationsz) AMUDP_RETURN_ERR(RESOURCE); /* none available */

  int retval = AM_Map(ea, i, name, tag);

  if (retval == AM_OK) *index = i;
  return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_UnMap(ep_t ea, int index) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to unmap after call to AM_SetExpectedResources */
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMUDP_RETURN_ERR(RESOURCE); /* not mapped */

  ea->translation[index].inuse = FALSE;
  ea->P--;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetNumTranslations(ep_t ea, int *pntrans) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  AMUDP_assert(ea->translationsz <= AMUDP_MAX_NUMTRANSLATIONS);
  *(pntrans) = ea->translationsz;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetNumTranslations(ep_t ea, int ntrans) {
  amudp_node_t newsz = (amudp_node_t)ntrans;
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (ntrans < 0 || newsz > AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(RESOURCE);
  if (newsz < AMUDP_INIT_NUMTRANSLATIONS) /* don't shrink beyond min value */
    newsz = AMUDP_INIT_NUMTRANSLATIONS;
  if (newsz == ea->translationsz) return AM_OK; /* no change */
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to change translationsz after call to AM_SetExpectedResources */

  for (amudp_node_t i = newsz; i < ea->translationsz; i++) {
    if (ea->translation[i].inuse)
      AMUDP_RETURN_ERR(RESOURCE); /* it's an error to truncate away live maps */
  }
  ea->translation = (amudp_translation_t *)AMUDP_realloc(ea->translation, newsz * sizeof(amudp_translation_t));
  /* we may be growing or truncating the table */
  if (newsz > ea->translationsz) 
    memset(&(ea->translation[ea->translationsz]), 0, (newsz - ea->translationsz) * sizeof(amudp_translation_t));
  ea->translationsz = newsz;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationInuse(ep_t ea, int i) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);

  if (ea->translation && ea->translation[i].inuse) return AM_OK; /* in use */
  else if (!ea->translation && (amudp_node_t)i < ea->P) return AM_OK; /* in use, after AM_SetExpectedResources */
  else return AM_ERR_RESOURCE; /* don't complain here - it's a common case */
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag) {
  AMUDP_CHECKINIT();
  if (!ea || !tag) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,i) != AM_OK) AMUDP_RETURN_ERR(RESOURCE); /* not mapped */

  if (ea->translation) (*tag) = ea->translation[i].tag;
  else                 (*tag) = ea->perProcInfo[i].tag;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SetTranslationTag(ep_t ea, int index, tag_t tag) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,index) != AM_OK) AMUDP_RETURN_ERR(RESOURCE); /* can't change tag if not mapped */

  amudp_node_t id;
  if (ea->translation) { 
    ea->translation[index].tag = tag;
    id = ea->translation[index].id;
  } else id = (amudp_node_t)index;

  if (ea->depth != -1) { /* after call to AM_SetExpectedResources we must update compressed table */
    ea->perProcInfo[id].tag = tag;
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationName(ep_t ea, int i, en_t *gan) {
  AMUDP_CHECKINIT();
  if (!ea || !gan) AMUDP_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMUDP_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,i) != AM_OK) AMUDP_RETURN_ERR(RESOURCE); /* not mapped */

  if (ea->translation) (*gan) = ea->translation[i].name;
  else                 (*gan) = ea->perProcInfo[i].remoteName;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_InitParameters(ep_t ep) {
  static int firsttime = 1;

  static int recvDepth;
  static int sendDepth;

  if (firsttime) { // only consult the environment once per process
    // transfer defaults are based on first endpoint

    // default depths enough for full-bandwidth comms with up to 4 neighbors
    recvDepth = 2 * ep->depth * MIN(MAX(1,ep->P-1),4);
    sendDepth = ep->depth * MIN(MAX(1,ep->P-1),4);


    char *faultRate = AMUDP_getenv_prefixed_withdefault("FAULT_RATE","0.0");
    if (faultRate && (AMUDP_FaultInjectionRate = atof(faultRate)) != 0.0) {
      AMUDP_FaultInjectionEnabled = 1;
      AMUDP_Warn("Running with fault injection enabled. Rate = %6.2f %%", 100.0 * AMUDP_FaultInjectionRate);
      srand( (unsigned)time( NULL ) ); /* TODO: we should really be using a private rand num generator */
    }

    #define ENVINT_WITH_DEFAULT(var, name, validate) do { \
        char defval[80];                                  \
        const char *valstr;                               \
        snprintf(defval, sizeof(defval), "%u", (unsigned int)var); \
        valstr = AMUDP_getenv_prefixed_withdefault(name,defval); \
        if (valstr) {                                            \
           char *end = (char *)valstr;                           \
           long val = strtol(valstr, &end, 0);                   \
           if (end == valstr) {                                  \
            AMUDP_Warn(name" may not be empty! Using default."); \
            val = (long)var;                                     \
           } else if ((int64_t)val != (int64_t)(int32_t)val) {   \
            AMUDP_Warn(name" too large! Using default.");        \
            val = (long)var;                                     \
           } else var = (uint32_t)val;                           \
           validate;                                      \
        }                                                 \
      } while (0)

    ENVINT_WITH_DEFAULT(recvDepth, "RECVDEPTH", { 
      if (val <= 0 || val > AMUDP_MAX_RECVDEPTH) 
        AMUDP_FatalErr("RECVDEPTH must be in 1..%d", AMUDP_MAX_RECVDEPTH);
    });

    ENVINT_WITH_DEFAULT(sendDepth, "SENDDEPTH",
                        { if (!val) AMUDP_FatalErr("SENDDEPTH must be non-zero"); });

    ENVINT_WITH_DEFAULT(AMUDP_MaxRequestTimeout_us, "REQUESTTIMEOUT_MAX",
                        { if (val <= 0) AMUDP_MaxRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_InitialRequestTimeout_us, "REQUESTTIMEOUT_INITIAL",
                        { if (val <= 0) AMUDP_InitialRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_RequestTimeoutBackoff, "REQUESTTIMEOUT_BACKOFF",
                        { if (val <= 1) AMUDP_FatalErr("REQUESTTIMEOUT_BACKOFF must be > 1"); });
    if (AMUDP_InitialRequestTimeout_us > AMUDP_MaxRequestTimeout_us) {
       AMUDP_Warn("REQUESTTIMEOUT_INITIAL must not exceed REQUESTTIMEOUT_MAX. Raising MAX...");
       AMUDP_MaxRequestTimeout_us = MAX(AMUDP_InitialRequestTimeout_us, AMUDP_InitialRequestTimeout_us*AMUDP_RequestTimeoutBackoff);
    }
    AMUDP_InitRetryCache();
    firsttime = 0;
  }

  ep->recvDepth = recvDepth;

  ep->sendDepth = sendDepth;
  int maxsendDepth = ep->depth * MAX(1,ep->P-1);
  if (ep->sendDepth < 0 || ep->sendDepth > maxsendDepth) // silently cap, since the max is P-dependent
    ep->sendDepth = maxsendDepth;
  if (ep->sendDepth < ep->depth) {
    AMUDP_Warn("SENDDEPTH may not be less than DEPTH. Raising SENDDEPTH...");
    ep->sendDepth = ep->depth;
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests) {
  AMUDP_CHECKINIT();
  if (!ea) AMUDP_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMUDP_RETURN_ERR(RESOURCE); /* it's an error to call AM_SetExpectedResources again */
  /* n_endpoints ignored : P is set as we Map translations */
  /*if (n_endpoints < 1 || n_endpoints >= AMUDP_MAX_NUMTRANSLATIONS) AMUDP_RETURN_ERR(BAD_ARG);*/
  if (n_outstanding_requests < 1 || n_outstanding_requests > AMUDP_MAX_NETWORKDEPTH) AMUDP_RETURN_ERR(BAD_ARG);

  ea->depth = n_outstanding_requests;
  ea->PD = ea->P * ea->depth;

  AMUDP_InitParameters(ea);

  if (!AMUDP_AllocateEndpointBuffers(ea)) AMUDP_RETURN_ERR(RESOURCE);

  /*  compact a copy of the translation table into our perproc info array */
  { amudp_node_t procid = 0;
    amudp_node_t i;
    for (i=0; i < ea->translationsz; i++) {
      if (ea->translation[i].inuse) {
        ea->perProcInfo[procid].remoteName = ea->translation[i].name;
        ea->perProcInfo[procid].tag = ea->translation[i].tag;
        ea->translation[i].id = procid;
        if (enEqual(ea->perProcInfo[procid].remoteName, ea->name)) ea->idHint = procid;
        procid++;
        if (procid == ea->P) break; /*  should have all of them now */
      }
    }
    #if AMUDP_DEBUG
      for (amudp_node_t j=i+1; j < ea->translationsz; j++) 
        AMUDP_assert(!ea->translation[j].inuse);
    #endif
    if (i+1 == ea->P) { 
      // common case: dense translation table
      // improve scalability by freeing the now-redundant data structure
      AMUDP_free(ea->translation);
      ea->translation = NULL;
    } else {
      AMUDP_DEBUG_WARN(("Translation table is sparse. Memory utilization will be slightly less scalable."));
    }
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

  *gan = ((amudp_buf_t *)token)->status.rx.sourceAddr;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_GetSourceId(void *token, int *srcid) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !srcid),BAD_ARG);

  *srcid = (int)((amudp_buf_t *)token)->status.rx.sourceId;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetDestEndpoint(void *token, ep_t *endp) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !endp),BAD_ARG);

  *endp = ((amudp_buf_t *)token)->status.rx.dest;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetMsgTag(void *token, tag_t *tagp) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token || !tagp),BAD_ARG);
  
  *tagp = ((amudp_buf_t *)token)->msg.tag;
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
    runningsum->RepliesSquashed[category] += newvalues->RepliesSquashed[category];
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
  int64_t repliesSquashed = 0; 
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
    snprintf(msg, sizeof(msg), "(AMUDP_COLLECT_STATS disabled)\n");
    if (fp != NULL) fprintf(fp, "%s", msg);
    return msg;
  #endif

  for (category = 0; category < amudp_NumCategories; category++) {
    requestsSent += stats->RequestsSent[category];
    requestsRetransmitted += stats->RequestsRetransmitted[category];
    requestsReceived += stats->RequestsReceived[category];
    repliesSent += stats->RepliesSent[category];
    repliesRetransmitted += stats->RepliesRetransmitted[category];
    repliesSquashed += stats->RepliesSquashed[category];
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

  { int packetoverhead = (20 /* IP header */ + 8  /* UDP header*/);
    reqUDPIPheaderbytes = (requestsSent + requestsRetransmitted) * packetoverhead;
    repUDPIPheaderbytes = (repliesSent + repliesRetransmitted) * packetoverhead;
    avgreqpacket += packetoverhead;
    avgreppacket += packetoverhead;
    avgpacket += packetoverhead;
  }

  /* batch lines together to improve chance of output together */
  snprintf(msg, sizeof(msg),
    " Requests: %8llu sent, %4llu retransmitted, %8llu received\n"
    " Replies:  %8llu sent, %4llu retransmitted, %8llu received, %4llu squashed\n"
    " Returned messages:   %8llu\n"
    " Misordered receipt:  %8llu/%llu\n"
  #if AMUDP_COLLECT_LATENCY_STATS
    "Latency (request sent to reply received): \n"
    " min: %8i microseconds\n"
    " max: %8i microseconds\n"
    " avg: %8i microseconds\n"
  #endif

    "Message Breakdown:        Requests     Replies   Avg data sz (Req/Rep/Both)\n"
    " Short  (<=%5i bytes)   %8llu    %8llu  %9.*f/%.*f/%.*f bytes\n"
    " Medium (<=%5i bytes)   %8llu    %8llu  %9.*f/%.*f/%.*f bytes\n"
    " Long   (<=%5i bytes)   %8llu    %8llu  %9.*f/%.*f/%.*f bytes\n"

    " Total                                          %9.*f/%.*f/%.*f bytes\n"

    "Data bytes sent:      %llu/%llu/%llu bytes\n"
    "Total bytes sent:     %llu/%llu/%llu bytes (incl. AM overhead)\n"
    "Bandwidth overhead:   %.2f%%/%.2f%%/%.2f%%\n"        
    "Average packet size:  %.*f/%.*f/%.*f bytes (incl. AM & transport-layer overhead)\n"
    , 
    (unsigned long long)requestsSent, (unsigned long long)requestsRetransmitted, (unsigned long long)requestsReceived,
    (unsigned long long)repliesSent, (unsigned long long)repliesRetransmitted, (unsigned long long)repliesReceived, (unsigned long long)repliesSquashed,
    (unsigned long long)stats->ReturnedMessages,
    (unsigned long long)stats->OutOfOrderRequests,
    (unsigned long long)stats->OutOfOrderReplies,
  #if AMUDP_COLLECT_LATENCY_STATS
    (stats->RequestMinLatency == (amudp_cputick_t)-1?(int)-1:(int)ticks2us(stats->RequestMinLatency)),
    (int)ticks2us(stats->RequestMaxLatency),
    (requestsSent>0?(int)(ticks2us(stats->RequestSumLatency) / requestsSent):-1),
  #endif

    /* Message breakdown */
    (int)(AMUDP_MAX_SHORT*sizeof(int)),
      (unsigned long long)stats->RequestsSent[amudp_Short], (unsigned long long)stats->RepliesSent[amudp_Short], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Short]), reqavgpayload[amudp_Short], 
      AMUDP_StatPrecision(repavgpayload[amudp_Short]), repavgpayload[amudp_Short], 
      AMUDP_StatPrecision(avgpayload[amudp_Short]), avgpayload[amudp_Short], 
    (int)(AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_MEDIUM),
      (unsigned long long)stats->RequestsSent[amudp_Medium], (unsigned long long)stats->RepliesSent[amudp_Medium], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Medium]), reqavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(repavgpayload[amudp_Medium]), repavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(avgpayload[amudp_Medium]), avgpayload[amudp_Medium], 
    (int)(AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_LONG),
      (unsigned long long)stats->RequestsSent[amudp_Long], (unsigned long long)stats->RepliesSent[amudp_Long], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Long]), reqavgpayload[amudp_Long], 
      AMUDP_StatPrecision(repavgpayload[amudp_Long]), repavgpayload[amudp_Long], 
      AMUDP_StatPrecision(avgpayload[amudp_Long]), avgpayload[amudp_Long], 

    /* avg data payload */
    AMUDP_StatPrecision(avgreqdata), avgreqdata,
    AMUDP_StatPrecision(avgrepdata), avgrepdata,
    AMUDP_StatPrecision(avgdata), avgdata,

    (unsigned long long)reqdataBytesSent, (unsigned long long)repdataBytesSent, (unsigned long long)dataBytesSent,

    (unsigned long long)reqTotalBytesSent, (unsigned long long)repTotalBytesSent, (unsigned long long)stats->TotalBytesSent,
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
    int64_t extra_rereq = requestsRetransmitted - reqlost - replost;
    int64_t extra_rerep = repliesRetransmitted - replost;
    int64_t extra_reboth = extra_rereq + extra_rerep;
    #define APPEND_PERCENT(num, denom, eol)                                               \
      if (num > 0) sprintf(msg+strlen(msg), "  (%6.3f%%)%s", (100.0*(num))/(denom), eol); \
      else strcat(msg, eol)
    sprintf(msg+strlen(msg), "Requests lost:        %9lli", (long long)reqlost);
    APPEND_PERCENT(reqlost, reqsent, "\n");
    sprintf(msg+strlen(msg), "Replies lost:         %9lli", (long long)replost);
    APPEND_PERCENT(replost, repsent, "\n");
    sprintf(msg+strlen(msg), "Total packets lost:   %9lli", (long long)packetslost);
    APPEND_PERCENT(packetslost, packetssent, "\n");
    sprintf(msg+strlen(msg), "Useless retransmits:  %lli", (long long)extra_rereq);
    APPEND_PERCENT(extra_rereq, requestsRetransmitted, " / ");
    sprintf(msg+strlen(msg), "%lli", (long long)extra_rerep);
    APPEND_PERCENT(extra_rerep, repliesRetransmitted, " / ");
    sprintf(msg+strlen(msg), "%lli", (long long)extra_reboth);
    APPEND_PERCENT(extra_reboth, requestsRetransmitted+repliesRetransmitted, "\n");
  } 

  if (fp != NULL) fprintf(fp, "%s", msg);
  return msg;
 }
}
/* ------------------------------------------------------------------------------------ */

