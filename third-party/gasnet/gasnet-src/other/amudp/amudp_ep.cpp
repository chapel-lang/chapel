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
amudp_handler_fn_t amudp_defaultreturnedmsg_handler = (amudp_handler_fn_t)&AMUDP_DefaultReturnedMsg_Handler;
int AMUDP_PoliteSync = 0;
uint32_t AMUDP_RequestTimeoutBackoff = AMUDP_REQUESTTIMEOUT_BACKOFF_MULTIPLIER;
uint32_t AMUDP_MaxRequestTimeout_us = AMUDP_MAX_REQUESTTIMEOUT_MICROSEC;
uint32_t AMUDP_InitialRequestTimeout_us = AMUDP_INITIAL_REQUESTTIMEOUT_MICROSEC;

AMX_IDENT(AMUDP_IdentString_Version, "$AMUDPLibraryVersion: " AMUDP_LIBRARY_VERSION_STR " $");

double AMUDP_FaultInjectionRate = 0.0;
double AMUDP_FaultInjectionEnabled = 0;

const amudp_stats_t AMUDP_initial_stats = /* the initial state for stats type */
        { {0,0,0}, {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
          0,0,0,
          (amx_tick_t)-1, 0, 0,
          {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, 
          0
        };

/* ------------------------------------------------------------------------------------ */
extern int AMUDP_enEqual(en_t en1, en_t en2) {
  return enEqual(en1, en2);
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
  AMX_assert(eb && ep);
  AMX_assert(eb->endpoints != NULL);
  if (eb->n_endpoints == eb->cursize) { /* need to grow array */
    int newsize = eb->cursize * 2;
    eb->endpoints = (ep_t *)AMX_realloc(eb->endpoints, sizeof(ep_t)*newsize);
    eb->cursize = newsize;
  }
  eb->endpoints[eb->n_endpoints] = ep;
  eb->n_endpoints++;
  ep->eb = eb;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_RemoveEndpoint(eb_t eb, ep_t ep) {
  AMX_assert(eb && ep);
  AMX_assert(eb->endpoints != NULL);
  AMX_assert(AMUDP_ContainsEndpoint(eb, ep));
  { int i;
    for (i = 0; i < eb->n_endpoints; i++) {
      if (eb->endpoints[i] == ep) {
        eb->endpoints[i] = eb->endpoints[eb->n_endpoints-1];
        eb->n_endpoints--;
        ep->eb = NULL;
        return;
      }
    }
    AMX_FatalErr("failure in AMUDP_RemoveEndpoint");
  }
}
/*------------------------------------------------------------------------------------
 * Endpoint buffer management
 *------------------------------------------------------------------------------------ */
/* internally, buffers have a header:
 *   while allocated: pool points to the bufferpool
 *   while freed: next pointer in free list
 */
#define AMUDP_BUFFERPOOL_MAGIC ((uint64_t)0x1001feedbac31001ULL)
extern amudp_buf_t *AMUDP_AcquireBuffer(ep_t ep, size_t sz) {
  AMX_assert(ep);
  AMX_assert(sz >= AMUDP_MIN_BUFFER);
  AMX_assert(sz <= AMUDP_MAX_BUFFER);
  amudp_bufferpool_t *pool;
  if (sz <= AMUDP_MAX_SHORT_BUFFER) {
    pool = &ep->bufferPool[0];
  } else {
    pool = &ep->bufferPool[1];
  }
  size_t poolsz = pool->buffersz;
  AMX_assert(sz <= poolsz);
  AMX_assert(pool->magic == AMUDP_BUFFERPOOL_MAGIC);

  amudp_bufferheader_t *bh;
  if (pool->free) {
    bh = pool->free;
    pool->free = bh->next;
  } else {
    bh = (amudp_bufferheader_t *)AMX_malloc(sizeof(amudp_bufferheader_t) + poolsz);
  }
  bh->pool = pool;

  AMX_memcheck(bh);

  #if AMUDP_BUFFER_STATS
    pool->stats.alloc_curr++;
    pool->stats.alloc_total++;
    pool->stats.buffer_bytes += sz;
    pool->stats.alloc_peak = MAX(pool->stats.alloc_curr,pool->stats.alloc_peak);
  #endif

  amudp_buf_t *buf = (amudp_buf_t *)(bh+1);
  AMX_assert(!((uintptr_t)buf & 0x7)); // 8-byte alignment
  return buf;
}
/* ------------------------------------------------------------------------------------ */
extern void AMUDP_ReleaseBuffer(ep_t ep, amudp_buf_t *buf) {
  AMX_assert(ep);
  AMX_assert(buf);
  amudp_bufferheader_t *bh = ((amudp_bufferheader_t *)buf) - 1;
  AMX_memcheck(bh);
  amudp_bufferpool_t *pool = bh->pool;
  AMX_assert(pool->magic == AMUDP_BUFFERPOOL_MAGIC);
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
    #if AMX_DEBUG
      ep->bufferPool[i].magic = AMUDP_BUFFERPOOL_MAGIC;
    #endif
  }
  ep->bufferPool[0].buffersz = AMUDP_MAX_SHORT_BUFFER;
  ep->bufferPool[1].buffersz = AMUDP_MAX_BUFFER;
}
/* ------------------------------------------------------------------------------------ */
static void AMUDP_FreeAllBuffers(ep_t ep) {
  AMX_memcheck_all();

  for (int i=0; i < AMUDP_NUMBUFFERPOOLS; i++) {
    amudp_bufferpool_t *pool = &ep->bufferPool[i];
    for (amudp_bufferheader_t *bh = pool->free; bh; ) {
      amudp_bufferheader_t *next = bh->next;
      AMX_free(bh);
      bh = next;
    }
    #if AMUDP_BUFFER_STATS
      AMX_Info("Buffer pool %5i: %7.1fb avg\t%6" PRIu64 " alloc\t%4" PRIu64 " peak\t%2" PRIu64 " leaked",
                 (int)pool->buffersz, 
                 pool->stats.buffer_bytes/(double)pool->stats.alloc_total,
                 pool->stats.alloc_total,
                 pool->stats.alloc_peak,
                 pool->stats.alloc_curr
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
    AMX_DEBUG_WARN(("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s",paramname,strerror(errno)));
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
        #if AMX_DEBUG
          perror("sysctl");
          AMX_Err("sysctl() on UDP socket failed");
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
      AMX_VERBOSE_INFO(("setsockopt(SOL_SOCKET, %s, %i) on UDP socket failed: %s", paramname, targetsize, strerror(errno)));
    } else {
      int temp = targetsize;
      junk = sizeof(int);
      if (SOCK_getsockopt(ep->s, SOL_SOCKET, szparam, (char *)&temp, &junk) == SOCKET_ERROR) {
        AMX_DEBUG_WARN(("getsockopt(SOL_SOCKET, %s) on UDP socket failed: %s", paramname, strerror(errno)));
      }
      if (temp >= targetsize) {
        if (!AMX_SilentMode) AMX_Info("UDP %s buffer successfully set to %i bytes", paramname, targetsize);
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
    AMX_assert(ep != NULL);
    /* allocate socket */
    ep->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ep->s == INVALID_SOCKET) 
      AMX_RETURN_ERRFR(RESOURCE, socket, strerror(errno));

    ep->name.sin_family = AF_INET;
    ep->name.sin_port = 0; /* any port */
    ep->name.sin_addr.s_addr = htonl(AMUDP_currentUDPInterface);
    memset(&ep->name.sin_zero, 0, sizeof(ep->name.sin_zero));

    if (bind(ep->s, (struct sockaddr*)&ep->name, sizeof(struct sockaddr)) == SOCKET_ERROR) {
      closesocket(ep->s);
      AMX_RETURN_ERRFR(RESOURCE, bind, strerror(errno));
    }
    { /*  danger: this might fail on multi-homed hosts if AMUDP_currentUDPInterface was not set*/
      GETSOCKNAME_LENGTH_T sz = sizeof(en_t);
      if (SOCK_getsockname(ep->s, (struct sockaddr*)&ep->name, &sz) == SOCKET_ERROR) {
        closesocket(ep->s);
        AMX_RETURN_ERRFR(RESOURCE, getsockname, strerror(errno));
      }
      /* can't determine interface address */
      if (ep->name.sin_addr.s_addr == INADDR_ANY) {
        closesocket(ep->s);
        AMX_RETURN_ERRFR(RESOURCE, AMUDP_AllocateEndpointResource,
                           "AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface address");
      }
      if (ep->name.sin_port == 0) {
        closesocket(ep->s);
        AMX_RETURN_ERRFR(RESOURCE, AMUDP_AllocateEndpointResource,
                           "AMUDP_AllocateEndpointResource failed to determine UDP endpoint interface port");
      }
    }

    ep->translationsz = AMUDP_INIT_NUMTRANSLATIONS;
    ep->translation = (amudp_translation_t *)AMX_calloc(ep->translationsz, sizeof(amudp_translation_t));

    return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_AllocateEndpointBuffers(ep_t ep) {

  AMX_assert(ep != NULL);
  AMX_assert(ep->depth >= 1);
  AMX_assert(ep->P > 0 && ep->P <= AMUDP_MAX_NUMTRANSLATIONS);
  AMX_assert(ep->PD == (int)ep->P * ep->depth);
  AMX_assert(ep->recvDepth > 0 && ep->recvDepth <= AMUDP_MAX_RECVDEPTH);
  AMX_assert(ep->sendDepth > 0 && ep->sendDepth <= (int)MAX(1,ep->P - 1) * ep->depth);

  AMX_assert(sizeof(amudp_buf_t) % sizeof(int) == 0); /* assume word-addressable machine */

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
  ep->perProcInfo = (amudp_perproc_info_t *)AMX_calloc(ep->P, sizeof(amudp_perproc_info_t));

  AMUDP_InitBuffers(ep);

  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointResource(ep_t ep) {
  AMX_assert(ep != NULL);
  /*  close UDP port */

  if (ep->translation) AMX_free(ep->translation);

  if (closesocket(ep->s) == SOCKET_ERROR) return FALSE;
  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_FreeEndpointBuffers(ep_t ep) {
  AMX_assert(ep != NULL);

  for (amudp_node_t proc=0; proc < ep->P; proc++) { // release tx buffers in use
    for (int t=0; t < 2; t++) {
      amudp_bufdesc_t *desc = (t ? ep->perProcInfo[proc].requestDesc : ep->perProcInfo[proc].replyDesc );
      if (desc) {
        for (int i=0; i < ep->depth; i++) { 
          amudp_buf_t *buf = desc[i].buffer;
          if (buf) AMUDP_ReleaseBuffer(ep, buf);
        }
        AMX_free(desc);
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

  AMX_free(ep->perProcInfo);
  ep->perProcInfo = NULL;

  return TRUE;
}
/*------------------------------------------------------------------------------------
 * System initialization/termination
 *------------------------------------------------------------------------------------ */
extern int AM_Init() {
  if (AMX_Init()) { /* first call */
    AMX_assert(sizeof(amudp_msg_t) % 4 == 0); // may be required for correct argument alignment
    #if PLATFORM_OS_LINUX /* && !PLATFORM_OS_WSL */
      FILE *fp = fopen("/proc/sys/kernel/osrelease", "r");
      if (fp) {
        char line[255];
        char *rc = fgets(line, sizeof(line), fp);
        fclose(fp);
        if (rc && strstr(line, "Microsoft")) { 
          AMX_Warn("AMUDP was built for native Linux, you are running on an emulated kernel which has known differences that might cause problems. "
          "We do not support this model of operation, but we DO support building from source on this platform. Please rebuild from source here.");
        }
      }
    #endif
  }
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_Terminate() {
  int retval = AM_OK;

  if (amx_Initialized == 1) { /* last termination call */
    for (int i = 0; i < AMUDP_numBundles; i++) {
      if (AM_FreeBundle(AMUDP_bundles[i]) != AM_OK) 
        retval = AM_ERR_RESOURCE;
    }
    AMUDP_numBundles = 0;
  }
  AMX_Terminate();

  AMX_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * endpoint/bundle management
 *------------------------------------------------------------------------------------ */
extern int AM_AllocateBundle(int type, eb_t *endb) {
  eb_t eb;
  AMX_CHECKINIT();
  if (type < 0 || type >= AM_NUM_BUNDLE_MODES) AMX_RETURN_ERR(BAD_ARG);
  if (type != AM_SEQ) AMX_RETURN_ERR(RESOURCE);
  if (AMUDP_numBundles == AMUDP_MAX_BUNDLES-1) AMX_RETURN_ERR(RESOURCE);
  if (!endb) AMX_RETURN_ERR(BAD_ARG);

  eb = (eb_t)AMX_malloc(sizeof(struct amudp_eb));
  eb->endpoints = (ep_t *)AMX_malloc(AMUDP_INITIAL_NUMENDPOINTS*sizeof(ep_t));
  eb->cursize = AMUDP_INITIAL_NUMENDPOINTS;
  eb->n_endpoints = 0;
  eb->event_mask = AM_NOEVENTS;

  AMUDP_bundles[AMUDP_numBundles++] = eb; /* keep track of all bundles */
  *endb = eb;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_FreeBundle(eb_t bundle) {
  if (!bundle) AMX_RETURN_ERR(BAD_ARG);
  { int i;

    /* free all constituent endpoints */
    for (i = 0; i < bundle->n_endpoints; i++) {
      int retval = AM_FreeEndpoint(bundle->endpoints[i]);
      if (retval != AM_OK) AMX_RETURN(retval);
    }
    AMX_assert(bundle->n_endpoints == 0);

    /* remove from bundle list */
    for (i = 0; i < AMUDP_numBundles; i++) {
      if (AMUDP_bundles[i] == bundle) { 
        AMUDP_bundles[i] = AMUDP_bundles[AMUDP_numBundles-1]; 
        break;
      }
    }
    AMX_assert(i < AMUDP_numBundles);
    AMUDP_numBundles--;

    AMX_free(bundle->endpoints);
    AMX_free(bundle);
  }
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_AllocateEndpoint(eb_t bundle, ep_t *endp, en_t *endpoint_name) {
  ep_t ep;
  int retval;

  AMX_CHECKINIT();
  if (!bundle || !endp || !endpoint_name) AMX_RETURN_ERR(BAD_ARG);

  ep = (ep_t)AMX_calloc(1, sizeof(struct amudp_ep));
  retval = AMUDP_AllocateEndpointResource(ep);
  if (retval != AM_OK) {
    AMX_free(ep);
    AMX_RETURN(retval);
  }

  /* setup eb<->ep link */
  AMUDP_InsertEndpoint(bundle, ep);
  ep->eb = bundle;

  { /* initialize ep data */
    ep->handler[0] = amudp_defaultreturnedmsg_handler;
    for (int i = 1; i < AMUDP_MAX_NUMHANDLERS; i++) {
      ep->handler[i] = amx_unused_handler;
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
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (!AMUDP_ContainsEndpoint(ea->eb, ea)) AMX_RETURN_ERR(RESOURCE);

  if (!AMUDP_FreeEndpointResource(ea)) retval = AM_ERR_RESOURCE;
  if (ea->depth != -1) {
    if (!AMUDP_FreeEndpointBuffers(ea)) retval = AM_ERR_RESOURCE;
  }

  AMUDP_RemoveEndpoint(ea->eb, ea);
  AMX_free(ea);
  AMX_RETURN(retval);
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MoveEndpoint(ep_t ea, eb_t from_bundle, eb_t to_bundle) {
  AMX_CHECKINIT();
  if (!ea || !from_bundle || !to_bundle) AMX_RETURN_ERR(BAD_ARG);
  if (!AMUDP_ContainsEndpoint(from_bundle, ea)) AMX_RETURN_ERR(RESOURCE);

  AMUDP_RemoveEndpoint(from_bundle, ea);
  AMUDP_InsertEndpoint(to_bundle, ea);
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Tag management
 *------------------------------------------------------------------------------------ */
extern int AM_SetTag(ep_t ea, tag_t tag) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);

  /*  TODO: return mismatched messages to sender */
  ea->tag = tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTag(ep_t ea, tag_t *tag) {
  AMX_CHECKINIT();
  if (!ea || !tag) AMX_RETURN_ERR(BAD_ARG);

  *tag = ea->tag;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * VM Segment management
 *------------------------------------------------------------------------------------ */
extern int AM_GetSeg(ep_t ea, void **addr, uintptr_t *nbytes) {
  AMX_CHECKINIT();
  if (!ea || !addr || !nbytes) AMX_RETURN_ERR(BAD_ARG);
  *addr = ea->segAddr;
  *nbytes = ea->segLength;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetSeg(ep_t ea, void *addr, uintptr_t nbytes) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (nbytes > AMUDP_MAX_SEGLENGTH) AMX_RETURN_ERR(BAD_ARG);

  ea->segAddr = addr;
  ea->segLength = nbytes;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MaxSegLength(uintptr_t* nbytes) {
  AMX_CHECKINIT();
  if (!nbytes) AMX_RETURN_ERR(BAD_ARG);
  *nbytes = AMUDP_MAX_SEGLENGTH;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Translation management
 *------------------------------------------------------------------------------------ */
extern int AM_Map(ep_t ea, int index, en_t name, tag_t tag) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMX_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);
  if (ea->translation[index].inuse) AMX_RETURN_ERR(RESOURCE); /* it's an error to re-map */

  ea->translation[index].inuse = TRUE;
  ea->translation[index].name = name;
  ea->translation[index].tag = tag;
  ea->P++;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MapAny(ep_t ea, int *index, en_t name, tag_t tag) {
  AMX_CHECKINIT();
  if (!ea || !index) AMX_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMX_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */

  amudp_node_t i;
  for (i = 0; i < ea->translationsz; i++) { /* find a free entry, possibly a middle hole */
    if (!ea->translation[i].inuse) break; /* use this one */
  }
  if (i == ea->translationsz) AMX_RETURN_ERR(RESOURCE); /* none available */

  int retval = AM_Map(ea, i, name, tag);

  if (retval == AM_OK) *index = i;
  return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_UnMap(ep_t ea, int index) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMX_RETURN_ERR(RESOURCE); /* it's an error to unmap after call to AM_SetExpectedResources */
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMX_RETURN_ERR(RESOURCE); /* not mapped */

  ea->translation[index].inuse = FALSE;
  ea->P--;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetNumTranslations(ep_t ea, int *pntrans) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  AMX_assert(ea->translationsz <= AMUDP_MAX_NUMTRANSLATIONS);
  *(pntrans) = ea->translationsz;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetNumTranslations(ep_t ea, int ntrans) {
  amudp_node_t newsz = (amudp_node_t)ntrans;
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (ntrans < 0 || newsz > AMUDP_MAX_NUMTRANSLATIONS) AMX_RETURN_ERR(RESOURCE);
  if (newsz < AMUDP_INIT_NUMTRANSLATIONS) /* don't shrink beyond min value */
    newsz = AMUDP_INIT_NUMTRANSLATIONS;
  if (newsz == ea->translationsz) return AM_OK; /* no change */
  if (ea->depth != -1) AMX_RETURN_ERR(RESOURCE); /* it's an error to change translationsz after call to AM_SetExpectedResources */

  for (amudp_node_t i = newsz; i < ea->translationsz; i++) {
    if (ea->translation[i].inuse)
      AMX_RETURN_ERR(RESOURCE); /* it's an error to truncate away live maps */
  }
  ea->translation = (amudp_translation_t *)AMX_realloc(ea->translation, newsz * sizeof(amudp_translation_t));
  /* we may be growing or truncating the table */
  if (newsz > ea->translationsz) 
    memset(&(ea->translation[ea->translationsz]), 0, (newsz - ea->translationsz) * sizeof(amudp_translation_t));
  ea->translationsz = newsz;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationInuse(ep_t ea, int i) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);

  if (ea->translation && ea->translation[i].inuse) return AM_OK; /* in use */
  else if (!ea->translation && (amudp_node_t)i < ea->P) return AM_OK; /* in use, after AM_SetExpectedResources */
  else return AM_ERR_RESOURCE; /* don't complain here - it's a common case */
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag) {
  AMX_CHECKINIT();
  if (!ea || !tag) AMX_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,i) != AM_OK) AMX_RETURN_ERR(RESOURCE); /* not mapped */

  if (ea->translation) (*tag) = ea->translation[i].tag;
  else                 (*tag) = ea->perProcInfo[i].tag;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SetTranslationTag(ep_t ea, int index, tag_t tag) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (index < 0 || (amudp_node_t)index >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,index) != AM_OK) AMX_RETURN_ERR(RESOURCE); /* can't change tag if not mapped */

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
  AMX_CHECKINIT();
  if (!ea || !gan) AMX_RETURN_ERR(BAD_ARG);
  if (i < 0 || (amudp_node_t)i >= ea->translationsz) AMX_RETURN_ERR(BAD_ARG);
  if (AM_GetTranslationInuse(ea,i) != AM_OK) AMX_RETURN_ERR(RESOURCE); /* not mapped */

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
      AMX_Warn("Running with fault injection enabled. Rate = %6.2f %%", 100.0 * AMUDP_FaultInjectionRate);
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
            AMX_Warn(name" may not be empty! Using default.");   \
            val = (long)var;                                     \
           } else if ((int64_t)val != (int64_t)(int32_t)val) {   \
            AMX_Warn(name" too large! Using default.");          \
            val = (long)var;                                     \
           } else var = (uint32_t)val;                           \
           validate;                                      \
        }                                                 \
      } while (0)

    ENVINT_WITH_DEFAULT(recvDepth, "RECVDEPTH", { 
      if (val <= 0 || val > AMUDP_MAX_RECVDEPTH) 
        AMX_FatalErr("RECVDEPTH must be in 1..%d", AMUDP_MAX_RECVDEPTH);
    });

    ENVINT_WITH_DEFAULT(sendDepth, "SENDDEPTH",
                        { if (!val) AMX_FatalErr("SENDDEPTH must be non-zero"); });

    ENVINT_WITH_DEFAULT(AMUDP_MaxRequestTimeout_us, "REQUESTTIMEOUT_MAX",
                        { if (val <= 0) AMUDP_MaxRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_InitialRequestTimeout_us, "REQUESTTIMEOUT_INITIAL",
                        { if (val <= 0) AMUDP_InitialRequestTimeout_us = AMUDP_TIMEOUT_INFINITE; });
    ENVINT_WITH_DEFAULT(AMUDP_RequestTimeoutBackoff, "REQUESTTIMEOUT_BACKOFF",
                        { if (val <= 1) AMX_FatalErr("REQUESTTIMEOUT_BACKOFF must be > 1"); });
    if (AMUDP_InitialRequestTimeout_us > AMUDP_MaxRequestTimeout_us) {
       AMX_Warn("REQUESTTIMEOUT_INITIAL must not exceed REQUESTTIMEOUT_MAX. Raising MAX...");
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
    AMX_Warn("SENDDEPTH may not be less than DEPTH. Raising SENDDEPTH...");
    ep->sendDepth = ep->depth;
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests) {
  AMX_CHECKINIT();
  if (!ea) AMX_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMX_RETURN_ERR(RESOURCE); /* it's an error to call AM_SetExpectedResources again */
  /* n_endpoints ignored : P is set as we Map translations */
  /*if (n_endpoints < 1 || n_endpoints >= AMUDP_MAX_NUMTRANSLATIONS) AMX_RETURN_ERR(BAD_ARG);*/
  if (n_outstanding_requests < 1 || n_outstanding_requests > AMUDP_MAX_NETWORKDEPTH) AMX_RETURN_ERR(BAD_ARG);

  ea->depth = n_outstanding_requests;
  ea->PD = ea->P * ea->depth;

  AMUDP_InitParameters(ea);

  if (!AMUDP_AllocateEndpointBuffers(ea)) AMX_RETURN_ERR(RESOURCE);

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
    #if AMX_DEBUG
      for (amudp_node_t j=i+1; j < ea->translationsz; j++) 
        AMX_assert(!ea->translation[j].inuse);
    #endif
    if (i+1 == ea->P) { 
      // common case: dense translation table
      // improve scalability by freeing the now-redundant data structure
      AMX_free(ea->translation);
      ea->translation = NULL;
    } else {
      AMX_DEBUG_WARN(("Translation table is sparse. Memory utilization will be slightly less scalable."));
    }
  }

  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Handler management
 *------------------------------------------------------------------------------------ */
extern int _AM_SetHandler(ep_t ea, handler_t handler, amudp_handler_fn_t function) {
  AMX_CHECKINIT();
  if (!ea || !function) AMX_RETURN_ERR(BAD_ARG);
  if (AMUDP_BADHANDLERVAL(handler)) AMX_RETURN_ERR(BAD_ARG);

  ea->handler[handler] = function;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, amudp_handler_fn_t function) {
  int i;
  AMX_CHECKINIT();
  if (!ea || !function || !handler) AMX_RETURN_ERR(BAD_ARG);

  for (i = 1 ; i < AMUDP_MAX_NUMHANDLERS; i++) {
    if (ea->handler[i] == amx_unused_handler) { /* find unused entry */
      ea->handler[i] = function;
      *handler = (handler_t)i;
      return AM_OK;
    }
  }
  AMX_RETURN_ERR(RESOURCE); /* all in use */
}
/*------------------------------------------------------------------------------------
 * Event management
 *------------------------------------------------------------------------------------ */
extern int AM_GetEventMask(eb_t eb, int *mask) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!eb),BAD_ARG);

  *mask = eb->event_mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetEventMask(eb_t eb, int mask) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!eb),BAD_ARG);
  AMX_CHECK_ERR((mask < 0 || ((amudp_eventmask_t)mask) >= AM_NUMEVENTMASKS),BAD_ARG);

  eb->event_mask = (uint8_t)mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_WaitSema(eb_t eb) {
  int retval;
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!eb),BAD_ARG);
  
  if (eb->event_mask == AM_NOEVENTS) 
    AMX_FatalErr("it's an error to block when the mask is not set - will never return");

  /* block here until a message arrives */
  retval = AMUDP_Block(eb);
  if (retval != AM_OK) eb->event_mask = AM_NOEVENTS;

  /* it's not clear from the spec whether we should poll here, 
     but it's probably safer to do so than not */
  if (retval == AM_OK) 
    retval = AM_Poll(eb);

  AMX_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * Message interrogation
 *------------------------------------------------------------------------------------ */
extern int AM_GetSourceEndpoint(void *token, en_t *gan) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token || !gan),BAD_ARG);

  *gan = ((amudp_buf_t *)token)->status.rx.sourceAddr;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_GetSourceId(void *token, int *srcid) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token || !srcid),BAD_ARG);

  *srcid = (int)((amudp_buf_t *)token)->status.rx.sourceId;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetDestEndpoint(void *token, ep_t *endp) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token || !endp),BAD_ARG);

  *endp = ((amudp_buf_t *)token)->status.rx.dest;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetMsgTag(void *token, tag_t *tagp) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token || !tagp),BAD_ARG);
  
  *tagp = ((amudp_buf_t *)token)->msg.tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_SetHandlerCallbacks(ep_t ep, AMUDP_preHandlerCallback_t preHandlerCallback, 
                                              AMUDP_postHandlerCallback_t postHandlerCallback) {
  AMX_CHECKINIT();
  if (!ep) AMX_RETURN_ERR(BAD_ARG);
  ep->preHandlerCallback = preHandlerCallback;
  ep->postHandlerCallback = postHandlerCallback;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Statistics API
 *------------------------------------------------------------------------------------ */
extern int AMUDP_GetEndpointStatistics(ep_t ep, amudp_stats_t *stats) { /* called by user to get statistics */
  AMX_CHECKINIT();
  if (!ep || !stats) AMX_RETURN_ERR(BAD_ARG);
  memcpy(stats, &ep->stats, sizeof(amudp_stats_t));
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_ResetEndpointStatistics(ep_t ep) {
  AMX_CHECKINIT();
  if (!ep) AMX_RETURN_ERR(BAD_ARG);
  ep->stats = AMUDP_initial_stats;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_AggregateStatistics(amudp_stats_t *runningsum, amudp_stats_t *newvalues) {
  int category;
  AMX_CHECKINIT();
  if (!runningsum || !newvalues) AMX_RETURN_ERR(BAD_ARG);
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
  uint64_t requestsSent = 0; 
  uint64_t requestsRetransmitted = 0; 
  uint64_t requestsReceived = 0; 
  uint64_t repliesSent = 0; 
  uint64_t repliesRetransmitted = 0; 
  uint64_t repliesSquashed = 0; 
  uint64_t repliesReceived = 0; 
  uint64_t reqdataBytesSent = 0; 
  uint64_t repdataBytesSent = 0; 
  uint64_t reqTotalBytesSent = 0; 
  uint64_t repTotalBytesSent = 0; 
  double reqavgpayload[amudp_NumCategories];
  double repavgpayload[amudp_NumCategories];
  double avgpayload[amudp_NumCategories];
  uint64_t reqUDPIPheaderbytes, repUDPIPheaderbytes; 
  int category;

  AMX_assert(amx_Initialized);
  AMX_assert(stats != NULL);
  AMX_getCPUTicks(); /* ensure this has been called at least once, even if stats are empty */

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
  uint64_t dataBytesSent = reqdataBytesSent + repdataBytesSent;
  uint64_t packetssent = (requestsSent + requestsRetransmitted + 
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
    " Requests: %8" PRIu64 " sent, %4" PRIu64 " retransmitted, %8" PRIu64 " received\n"
    " Replies:  %8" PRIu64 " sent, %4" PRIu64 " retransmitted, %8" PRIu64 " received, %4" PRIu64 " squashed\n"
    " Returned messages:   %8" PRIu64 "\n"
    " Misordered receipt:  %8" PRIu64 "/%" PRIu64 "\n"
  #if AMUDP_COLLECT_LATENCY_STATS
    "Latency (request sent to reply received): \n"
    " min: %8i microseconds\n"
    " max: %8i microseconds\n"
    " avg: %8i microseconds\n"
  #endif

    "Message Breakdown:        Requests     Replies   Avg data sz (Req/Rep/Both)\n"
    " Short  (<=%5" PRIuSZ " bytes)   %8" PRIu64 "    %8" PRIu64 "  %9.*f/%.*f/%.*f bytes\n"
    " Medium (<=%5" PRIuSZ " bytes)   %8" PRIu64 "    %8" PRIu64 "  %9.*f/%.*f/%.*f bytes\n"
    " Long   (<=%5" PRIuSZ " bytes)   %8" PRIu64 "    %8" PRIu64 "  %9.*f/%.*f/%.*f bytes\n"

    " Total                                          %9.*f/%.*f/%.*f bytes\n"

    "Data bytes sent:      %" PRIu64 "/%" PRIu64 "/%" PRIu64 " bytes\n"
    "Total bytes sent:     %" PRIu64 "/%" PRIu64 "/%" PRIu64 " bytes (incl. AM overhead)\n"
    "Bandwidth overhead:   %.2f%%/%.2f%%/%.2f%%\n"        
    "Average packet size:  %.*f/%.*f/%.*f bytes (incl. AM & transport-layer overhead)\n"
    , 
    requestsSent, requestsRetransmitted, requestsReceived,
    repliesSent, repliesRetransmitted, repliesReceived, repliesSquashed,
    stats->ReturnedMessages,
    stats->OutOfOrderRequests,
    stats->OutOfOrderReplies,
  #if AMUDP_COLLECT_LATENCY_STATS
    (stats->RequestMinLatency == (amx_tick_t)-1?(int)-1:(int)AMX_ticks2us(stats->RequestMinLatency)),
    (int)AMX_ticks2us(stats->RequestMaxLatency),
    (requestsSent>0?(int)(AMX_ticks2us(stats->RequestSumLatency) / requestsSent):-1),
  #endif

    /* Message breakdown */
    AMUDP_MAX_SHORT*sizeof(int),
      stats->RequestsSent[amudp_Short], stats->RepliesSent[amudp_Short], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Short]), reqavgpayload[amudp_Short], 
      AMUDP_StatPrecision(repavgpayload[amudp_Short]), repavgpayload[amudp_Short], 
      AMUDP_StatPrecision(avgpayload[amudp_Short]), avgpayload[amudp_Short], 
    AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_MEDIUM,
      stats->RequestsSent[amudp_Medium], stats->RepliesSent[amudp_Medium], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Medium]), reqavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(repavgpayload[amudp_Medium]), repavgpayload[amudp_Medium], 
      AMUDP_StatPrecision(avgpayload[amudp_Medium]), avgpayload[amudp_Medium], 
    AMUDP_MAX_SHORT*sizeof(int) + AMUDP_MAX_LONG,
      stats->RequestsSent[amudp_Long], stats->RepliesSent[amudp_Long], 
      AMUDP_StatPrecision(reqavgpayload[amudp_Long]), reqavgpayload[amudp_Long], 
      AMUDP_StatPrecision(repavgpayload[amudp_Long]), repavgpayload[amudp_Long], 
      AMUDP_StatPrecision(avgpayload[amudp_Long]), avgpayload[amudp_Long], 

    /* avg data payload */
    AMUDP_StatPrecision(avgreqdata), avgreqdata,
    AMUDP_StatPrecision(avgrepdata), avgrepdata,
    AMUDP_StatPrecision(avgdata), avgdata,

    reqdataBytesSent, repdataBytesSent, dataBytesSent,

    reqTotalBytesSent, repTotalBytesSent, stats->TotalBytesSent,
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
    int64_t reqsent = (int64_t)(requestsSent + requestsRetransmitted);
    int64_t reqlost = reqsent - (int64_t)requestsReceived;
    int64_t repsent = (int64_t)(repliesSent + repliesRetransmitted);
    int64_t replost = repsent - (int64_t)repliesReceived;
    int64_t packetslost = reqlost + replost;
    int64_t extra_rereq = (int64_t)requestsRetransmitted - reqlost - replost;
    int64_t extra_rerep = (int64_t)repliesRetransmitted - replost;
    int64_t extra_reboth = extra_rereq + extra_rerep;
    #define APPEND_PERCENT(num, denom, eol)                                               \
      if (num > 0) sprintf(msg+strlen(msg), "  (%6.3f%%)%s", (100.0*(num))/(denom), eol); \
      else strcat(msg, eol)
    sprintf(msg+strlen(msg), "Requests lost:        %9" PRIi64, reqlost);
    APPEND_PERCENT(reqlost, reqsent, "\n");
    sprintf(msg+strlen(msg), "Replies lost:         %9" PRIi64, replost);
    APPEND_PERCENT(replost, repsent, "\n");
    sprintf(msg+strlen(msg), "Total packets lost:   %9" PRIi64, packetslost);
    APPEND_PERCENT(packetslost, packetssent, "\n");
    sprintf(msg+strlen(msg), "Useless retransmits:  %" PRIi64, extra_rereq);
    APPEND_PERCENT(extra_rereq, requestsRetransmitted, " / ");
    sprintf(msg+strlen(msg), "%" PRIi64, extra_rerep);
    APPEND_PERCENT(extra_rerep, repliesRetransmitted, " / ");
    sprintf(msg+strlen(msg), "%" PRIi64, extra_reboth);
    APPEND_PERCENT(extra_reboth, requestsRetransmitted+repliesRetransmitted, "\n");
  } 

  if (fp != NULL) fprintf(fp, "%s", msg);
  return msg;
 }
}
/* ------------------------------------------------------------------------------------ */
