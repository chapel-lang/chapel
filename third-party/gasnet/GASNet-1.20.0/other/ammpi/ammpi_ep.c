/*   $Source: /var/local/cvs/gasnet/other/ammpi/ammpi_ep.c,v $
 *     $Date: 2009/03/30 02:40:43 $
 * $Revision: 1.49 $
 * Description: AMMPI Implementations of endpoint and bundle operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <ammpi_internal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* definitions for internal declarations */
int ammpi_Initialized = 0;
static void AMMPI_defaultAMHandler(void *token);
ammpi_handler_fn_t ammpi_unused_handler = (ammpi_handler_fn_t)&AMMPI_defaultAMHandler;
ammpi_handler_fn_t ammpi_defaultreturnedmsg_handler = (ammpi_handler_fn_t)&AMMPI_DefaultReturnedMsg_Handler;
int AMMPI_VerboseErrors = 0;
int AMMPI_SilentMode = 0; 
int AMMPI_syncsend_thresh = 0;
AMMPI_IDENT(AMMPI_IdentString_Version, "$AMMPILibraryVersion: " AMMPI_LIBRARY_VERSION_STR " $");

#ifdef MPI_VERSION
  #define AMMPI_MPI_VERSION_STR_1 "|MPI_VERSION="_STRINGIFY(MPI_VERSION)
#else
  #define AMMPI_MPI_VERSION_STR_1 
#endif
#ifdef MPI_SUBVERSION
  #define AMMPI_MPI_VERSION_STR_2 "|MPI_SUBVERSION="_STRINGIFY(MPI_SUBVERSION)
#else
  #define AMMPI_MPI_VERSION_STR_2 
#endif
#ifdef MPICH_VERSION
  #define AMMPI_MPI_VERSION_STR_3 "|MPICH_VERSION="MPICH_VERSION
#else
  #define AMMPI_MPI_VERSION_STR_3 
#endif
#ifdef ROMIO_VERSION
  #define AMMPI_MPI_VERSION_STR_4 "|ROMIO_VERSION="_STRINGIFY(ROMIO_VERSION)
#else
  #define AMMPI_MPI_VERSION_STR_4 
#endif
AMMPI_IDENT(AMMPI_IdentString_MPIVersion, "$MPIVersion: " \
            AMMPI_MPI_VERSION_STR_1 \
            AMMPI_MPI_VERSION_STR_2 \
            AMMPI_MPI_VERSION_STR_3 \
            AMMPI_MPI_VERSION_STR_4 \
            "| $");
AMMPI_IDENT(AMMPI_IdentString_MPICCVersion, "$MPICompiler: " PLATFORM_COMPILER_IDSTR " $");

const ammpi_stats_t AMMPI_initial_stats = /* the initial state for stats type */
        { {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0},
          0,
          (uint64_t)-1, 0, 0,
          {0,0,0}, {0,0,0}, 
          {0,0,0}, {0,0,0}, 
          0
        };
/* ------------------------------------------------------------------------------------ */
/* error handling */
AMMPI_FORMAT_PRINTF(AMMPI_Msg,2,0,
static int AMMPI_Msg(const char *prefix, const char *msg, va_list argptr)) {
  char *expandedmsg = (char *)AMMPI_malloc(strlen(msg)+strlen(prefix)+50);
  int retval;

  sprintf(expandedmsg, "*** %s: %s\n", prefix, msg);
  retval = vfprintf(stderr, expandedmsg, argptr);
  fflush(stderr);
  AMMPI_free(expandedmsg);

  return retval; 
}

extern int AMMPI_Warn(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); /* pass in last argument */
    retval = AMMPI_Msg("AMMPI WARNING", msg, argptr);
  va_end(argptr);
  return retval;
}

extern int AMMPI_Err(const char *msg, ...) {
  va_list argptr;
  int retval;
  va_start(argptr, msg); /* pass in last argument */
    retval = AMMPI_Msg("AMMPI ERROR", msg, argptr);
  va_end(argptr);
  return retval;
}

extern void AMMPI_FatalErr(const char *msg, ...) {
  va_list argptr;
  va_start(argptr, msg); /* pass in last argument */
    AMMPI_Msg("FATAL ERROR", msg, argptr);
  va_end(argptr);
  abort();
}
/* ------------------------------------------------------------------------------------ */
static void AMMPI_defaultAMHandler(void *token) {
  int srcnode = -1;
  AMMPI_GetSourceId(token, &srcnode);
  AMMPI_FatalErr("AMMPI received an AM message from node %i for a handler index "
                     "with no associated AM handler function registered", 
                     srcnode);
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_enEqual(en_t en1, en_t en2) {
  return (en1.mpirank == en2.mpirank && en1.mpitag == en2.mpitag);
}
/*------------------------------------------------------------------------------------
 * Endpoint list handling for bundles
 *------------------------------------------------------------------------------------ */
int AMMPI_numBundles = 0;
eb_t AMMPI_bundles[AMMPI_MAX_BUNDLES] = {0};
/* ------------------------------------------------------------------------------------ */
static int AMMPI_ContainsEndpoint(eb_t eb, ep_t ep) {
  int i;
  for (i = 0; i < eb->n_endpoints; i++) {
    if (eb->endpoints[i] == ep) return TRUE;
  }
  return FALSE;
}
/* ------------------------------------------------------------------------------------ */
static void AMMPI_InsertEndpoint(eb_t eb, ep_t ep) {
  AMMPI_assert(eb && ep);
  AMMPI_assert(eb->endpoints);
  if (eb->n_endpoints == eb->cursize) { /* need to grow array */
    int newsize = eb->cursize * 2;
    ep_t *newendpoints = (ep_t *)AMMPI_malloc(sizeof(ep_t)*newsize);
    memcpy(newendpoints, eb->endpoints, sizeof(ep_t)*eb->n_endpoints);
    AMMPI_free(eb->endpoints);
    eb->endpoints = newendpoints;
    eb->cursize = newsize;
  }
  eb->endpoints[eb->n_endpoints] = ep;
  eb->n_endpoints++;
}
/* ------------------------------------------------------------------------------------ */
static void AMMPI_RemoveEndpoint(eb_t eb, ep_t ep) {
  AMMPI_assert(eb && ep);
  AMMPI_assert(eb->endpoints);
  AMMPI_assert(AMMPI_ContainsEndpoint(eb, ep));
  { int i;
    for (i = 0; i < eb->n_endpoints; i++) {
      if (eb->endpoints[i] == ep) {
        eb->endpoints[i] = eb->endpoints[eb->n_endpoints-1];
        eb->n_endpoints--;
        return;
      }
    }
    AMMPI_FatalErr("AMMPI_RemoveEndpoint failed");
  }
}
/*------------------------------------------------------------------------------------
 * Endpoint resource management
 *------------------------------------------------------------------------------------ */
static MPI_Comm currentComm = MPI_COMM_WORLD;
extern int AMMPI_SetEndpointCommunicator(void *ptr_to_MPI_Comm) {
  AMMPI_assert(ptr_to_MPI_Comm);
  currentComm = *(MPI_Comm*)ptr_to_MPI_Comm;
  return AM_OK;
}
/*------------------------------------------------------------------------------------*/
static int AMMPI_AllocateEndpointResource(ep_t ep) {
  int procnum;
  int mpitag;
  int pid = getpid();
  static int callcnt = 0;
  AMMPI_assert(ep);

  ep->translation = AMMPI_calloc(AMMPI_INIT_NUMTRANSLATIONS, sizeof(ammpi_translation_t));
  if (ep->translation == NULL) 
    AMMPI_RETURN_ERRFR(RESOURCE, AMMPI_AllocateEndpointResource, "out of memory");
  ep->translationsz = AMMPI_INIT_NUMTRANSLATIONS;

  /* base MPI tag on pid to prevent receiving cross-talk messages sent to dead processes */
  mpitag = (pid + (callcnt<<16)) % (MPI_TAG_UB+1);
  if (mpitag == MPI_ANY_TAG) mpitag = (mpitag + 1) % (MPI_TAG_UB+1);
  callcnt++;

  { /* setup MPI communicators for isolation */
    MPI_Group world_group;
    MPI_SAFE(MPI_Comm_group(currentComm, &world_group));
    ep->Req.mpicomm = AMMPI_malloc(sizeof(MPI_Comm));
    ep->Rep.mpicomm = AMMPI_malloc(sizeof(MPI_Comm));
    if (!ep->Req.mpicomm || !ep->Rep.mpicomm) 
      AMMPI_RETURN_ERRFR(RESOURCE, AMMPI_AllocateEndpointResource, "out of memory");
    MPI_SAFE(MPI_Comm_create(currentComm, world_group, ep->Req.mpicomm));
    MPI_SAFE(MPI_Comm_create(currentComm, world_group, ep->Rep.mpicomm));
    MPI_SAFE(MPI_Group_free(&world_group));
  }

  MPI_SAFE(MPI_Comm_rank(currentComm, &procnum));
  ep->name.mpirank = procnum;
  ep->name.mpitag = mpitag;
  MPI_SAFE(MPI_Errhandler_set(*ep->Req.mpicomm, MPI_ERRORS_RETURN));
  MPI_SAFE(MPI_Errhandler_set(*ep->Rep.mpicomm, MPI_ERRORS_RETURN));

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_AllocateEndpointBuffers(ep_t ep) {
  int numBufs;
  int retval = TRUE;
  AMMPI_assert(ep);
  AMMPI_assert(ep->depth >= 1);
  AMMPI_assert(ep->translationsz >= AMMPI_INIT_NUMTRANSLATIONS);
  AMMPI_assert(ep->translationsz <= AMMPI_MAX_NUMTRANSLATIONS);
  AMMPI_assert(ep->totalP <= ep->translationsz);
  AMMPI_assert(sizeof(ammpi_buf_t) % sizeof(int) == 0); /* assume word-addressable machine */

  numBufs = 2*ep->depth; /* 2x to match small/large split in send pool */

  /* compressed translation table */
  ep->perProcInfo = (ammpi_perproc_info_t *)AMMPI_malloc(ep->totalP * sizeof(ammpi_perproc_info_t));
  if (ep->perProcInfo == NULL) return FALSE;
  memset(ep->perProcInfo, 0, ep->totalP * sizeof(ammpi_perproc_info_t));

  #if AMMPI_PREPOST_RECVS 
    /* setup recv buffers */
    ep->rxBuf_alloc = (ammpi_buf_t *)AMMPI_malloc((2*numBufs * sizeof(ammpi_buf_t))+AMMPI_BUF_ALIGN);
    ep->rxHandle_both = (MPI_Request *)AMMPI_malloc(2*numBufs * sizeof(MPI_Request));
    if (!ep->rxBuf_alloc || !ep->rxHandle_both) return FALSE;
    ep->Rep.rxBuf = (ammpi_buf_t *)AMMPI_ALIGNUP(ep->rxBuf_alloc,AMMPI_BUF_ALIGN);
    ep->Req.rxBuf = ep->Rep.rxBuf + numBufs;
    ep->Rep.rxHandle = ep->rxHandle_both;
    ep->Req.rxHandle = ep->rxHandle_both + numBufs;
    AMMPI_assert(((uintptr_t)ep->Rep.rxBuf) % AMMPI_BUF_ALIGN == 0);
    AMMPI_assert(((uintptr_t)ep->Req.rxBuf) % AMMPI_BUF_ALIGN == 0);
    AMMPI_assert(sizeof(ammpi_buf_t) % AMMPI_BUF_ALIGN == 0);
    ep->Rep.rxNumBufs = numBufs;
    ep->Req.rxNumBufs = numBufs;

    { int i;
      for(i=0;i<numBufs;i++) {
        ep->Req.rxHandle[i] = MPI_REQUEST_NULL;
        ep->Rep.rxHandle[i] = MPI_REQUEST_NULL;
      }
      for(i=0;i<numBufs;i++) {
        retval &= !AMMPI_PostRecvBuffer(&ep->Req.rxBuf[i],&ep->Req.rxHandle[i],ep->Req.mpicomm);
        retval &= !AMMPI_PostRecvBuffer(&ep->Rep.rxBuf[i],&ep->Rep.rxHandle[i],ep->Rep.mpicomm);
      }
      ep->Req.rxCurr = 0; /* oldest recv */
      ep->Rep.rxCurr = 0;
      #if AMMPI_RECV_REPOST_SLACK
        ep->Req.rxPostSlack = 0;
        ep->Rep.rxPostSlack = 0;
        ep->Req.rxPostSlackMax = MIN(numBufs-1,AMMPI_RECV_REPOST_SLACK);
        ep->Rep.rxPostSlackMax = MIN(numBufs-1,AMMPI_RECV_REPOST_SLACK);
      #endif
    }
  #endif

  #if AMMPI_NONBLOCKING_SENDS
    if (!AMMPI_AllocateSendBuffers(ep)) retval = FALSE;
  #endif

  return retval;
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_FreeEndpointResource(ep_t ep) {
  AMMPI_assert(ep);
  AMMPI_assert(ep->translation);
  AMMPI_free(ep->translation);
  ep->translation = NULL;
  MPI_SAFE(MPI_Comm_free(ep->Req.mpicomm));
  MPI_SAFE(MPI_Comm_free(ep->Rep.mpicomm));
  AMMPI_free(ep->Req.mpicomm);
  AMMPI_free(ep->Rep.mpicomm);
  ep->Req.mpicomm = NULL;
  ep->Rep.mpicomm = NULL;
  return TRUE;
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_FreeEndpointBuffers(ep_t ep) {
  int retval = TRUE;
  AMMPI_assert(ep);

  AMMPI_free(ep->perProcInfo);
  ep->perProcInfo = NULL;

  #if AMMPI_PREPOST_RECVS 
    { int i,j;
      for (j=0; j < 2; j++) {
        ammpi_virtual_network_t *net = (j ? &ep->Req : &ep->Rep);
        for(i=0; i < net->rxNumBufs; i++) {
          MPI_Request *rxh = &net->rxHandle[i];
          if (*rxh != MPI_REQUEST_NULL) {
            MPI_Status mpistatus;
            retval &= MPI_SAFE_NORETURN(MPI_Cancel(rxh));
            #if PLATFORM_ARCH_CRAYT3E
              /* Cray MPI implementation sometimes hangs forever if you cancel-wait */
              retval &= MPI_SAFE_NORETURN(MPI_Request_free(rxh));
            #elif PLATFORM_OS_CATAMOUNT && MPI_VERSION == 1
              /* Sandia MPI implementation hangs on cancel-wait */
              retval &= MPI_SAFE_NORETURN(MPI_Request_free(rxh));
            #elif PLATFORM_OS_AIX
              /* AIX 5.2 32-bit MPI implementation is unreliable for cancel-wait
                 (frequent crashes observed for Titanium shutdown on 
                  MPI-over-LAPI 3.5.0.15, for 2 or more nodes) */
              retval &= MPI_SAFE_NORETURN(MPI_Request_free(rxh));
            #else
              retval &= MPI_SAFE_NORETURN(MPI_Wait(rxh, &mpistatus));
            #endif
            *rxh = MPI_REQUEST_NULL;
          }
        }
        net->rxBuf = NULL;
        net->rxHandle = NULL;
        net->rxNumBufs = 0;
      }
    }  
    AMMPI_free(ep->rxHandle_both);
    ep->rxHandle_both = NULL;
    AMMPI_free(ep->rxBuf_alloc);
    ep->rxBuf_alloc = NULL;
  #endif

  #if AMMPI_NONBLOCKING_SENDS
    retval &= AMMPI_ReleaseSendBuffers(ep);
  #endif

  return retval;
}
/*------------------------------------------------------------------------------------
 * non-blocking recv buffer management
 *------------------------------------------------------------------------------------ */
#if AMMPI_PREPOST_RECVS
extern int AMMPI_PostRecvBuffer(ammpi_buf_t *rxBuf, MPI_Request *prxHandle, MPI_Comm *pmpicomm) { 
  AMMPI_assert(*prxHandle == MPI_REQUEST_NULL);
  AMMPI_assert(((uintptr_t)rxBuf) % AMMPI_BUF_ALIGN == 0);
  MPI_SAFE(MPI_Irecv(rxBuf, AMMPI_MAX_NETWORK_MSG, MPI_BYTE, 
                     MPI_ANY_SOURCE, MPI_ANY_TAG, *pmpicomm, 
                     prxHandle));
  AMMPI_assert(*prxHandle != MPI_REQUEST_NULL);
  return AM_OK;
}
#endif
/*------------------------------------------------------------------------------------
 * non-blocking send buffer management
 *------------------------------------------------------------------------------------ */
#if AMMPI_NONBLOCKING_SENDS
/* ------------------------------------------------------------------------------------ */
static int AMMPI_initSendBufferPool(ammpi_sendbuffer_pool_t* pool, int count, int bufsize) {
  char* tmp = NULL;
  int i;
  AMMPI_assert(pool && count > 0 && bufsize > 0);
  AMMPI_assert(bufsize % sizeof(int) == 0);
  bufsize = AMMPI_ALIGNUP(bufsize, AMMPI_BUF_ALIGN);
  pool->txHandle = (MPI_Request *)AMMPI_malloc(count*sizeof(MPI_Request));
  pool->txBuf = (ammpi_buf_t**)AMMPI_malloc(count*sizeof(ammpi_buf_t*)); 
  tmp = (char*)AMMPI_malloc(count*bufsize+AMMPI_BUF_ALIGN);
  pool->memBlocks = (char **)AMMPI_malloc(sizeof(char *));
  pool->tmpIndexArray = (int *)AMMPI_malloc(count * sizeof(int));
  pool->tmpStatusArray = (MPI_Status *)AMMPI_malloc(count * sizeof(MPI_Status));
  if (!tmp || !pool->txHandle || !pool->txBuf || 
      !pool->memBlocks || !pool->tmpIndexArray || !pool->tmpStatusArray) 
    return FALSE; /* out of mem */
  pool->numBlocks = 1;
  pool->memBlocks[0] = tmp;
  tmp = (char *)AMMPI_ALIGNUP(tmp, AMMPI_BUF_ALIGN);
  for (i=0; i < count; i++) {
    AMMPI_assert(((uintptr_t)tmp) % AMMPI_BUF_ALIGN == 0);
    pool->txBuf[i] = (ammpi_buf_t*)tmp;
    tmp += bufsize;
    pool->txHandle[i] = MPI_REQUEST_NULL;
  }
  pool->numBufs = count;
  pool->numActive = 0;
  pool->bufSize = bufsize;

  return TRUE;
}
/* allocate non-blocking send buffers for this endpoint, 
 * return TRUE/FALSE to indicate success
 */
extern int AMMPI_AllocateSendBuffers(ep_t ep) {
  int retval = TRUE;
  AMMPI_assert(ep);
  AMMPI_assert(ep->depth >= 1);
  AMMPI_assert(ep->translationsz <= AMMPI_MAX_NUMTRANSLATIONS);
  AMMPI_assert(ep->totalP <= ep->translationsz);
  
  retval &= AMMPI_initSendBufferPool(&(ep->Req.sendPool_small), ep->depth, AMMPI_MAX_SMALL_NETWORK_MSG);
  retval &= AMMPI_initSendBufferPool(&(ep->Rep.sendPool_small), ep->depth, AMMPI_MAX_SMALL_NETWORK_MSG);
  retval &= AMMPI_initSendBufferPool(&(ep->Req.sendPool_large), ep->depth, AMMPI_MAX_NETWORK_MSG);
  retval &= AMMPI_initSendBufferPool(&(ep->Rep.sendPool_large), ep->depth, AMMPI_MAX_NETWORK_MSG);

  return retval;
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_freeSendBufferPool(ammpi_sendbuffer_pool_t* pool) {
  int retval = TRUE;
  AMMPI_assert(pool);

  /* terminate any outstanding communications */
  { int i;
    for(i=0; i < pool->numActive; i++) {
      if (pool->txHandle[i] != MPI_REQUEST_NULL) {
        MPI_Status mpistatus;
        #if 0
          /* the MPI spec states that MPI_Cancel is a local operation that must
           * complete immediately without blocking, but apparently too many 
           * implementations screw this up
           */
          retval &= MPI_SAFE_NORETURN(MPI_Cancel(&pool->txHandle[i]));
          #if PLATFORM_ARCH_CRAYT3E
            /* Cray MPI implementation sometimes hangs forever if you cancel-wait */
            retval &= MPI_SAFE_NORETURN(MPI_Request_free(&pool->txHandle[i]));
          #else
            retval &= MPI_SAFE_NORETURN(MPI_Wait(&pool->txHandle[i], &mpistatus));
          #endif
        #else
          #if 0
            /* better to simply wait and hope the remote node hasn't crashed 
             * (in which case we might get stuck here) 
             */
            retval &= MPI_SAFE_NORETURN(MPI_Wait(&pool->txHandle[i], &mpistatus));
          #else
            { /* use a timeout to decide remote side is dead */
              int j;
              int flag;
              #define RETRIES 2
              for (j = 0; j < RETRIES; j++) {
                retval &= MPI_SAFE_NORETURN(MPI_Test(&pool->txHandle[i], &flag, &mpistatus));
                if (flag) break;
                else ammpi_usleep(1000);
              }
              if (j == RETRIES) {
                #if AMMPI_DEBUG_VERBOSE
                  fprintf(stderr,"WARNING: Giving up on a timed-out send during shutdown\n");
                #endif
                /* attempt to cancel */
                retval &= MPI_SAFE_NORETURN(MPI_Cancel(&pool->txHandle[i]));
                retval &= MPI_SAFE_NORETURN(MPI_Request_free(&pool->txHandle[i]));
              }
            }
          #endif
        #endif
        pool->txHandle[i] = MPI_REQUEST_NULL;
      }
    }
  }  
  
  /* free the mem */
  AMMPI_free(pool->txHandle);
  pool->txHandle = NULL;
  AMMPI_free(pool->txBuf);
  pool->txBuf = NULL;
  AMMPI_free(pool->tmpIndexArray);
  pool->tmpIndexArray = NULL;
  AMMPI_free(pool->tmpStatusArray);
  pool->tmpStatusArray = NULL;
  { int i;
    for (i=0; i < pool->numBlocks; i++) 
      AMMPI_free(pool->memBlocks[i]);
    AMMPI_free(pool->memBlocks);
    pool->memBlocks = NULL;
  }

  return retval;
}
/* cancel any outstanding non-blocking sends and release the associated buffers 
 * for this endpoint, return TRUE/FALSE to indicate success
 */
extern int AMMPI_ReleaseSendBuffers(ep_t ep) {
  int retval = TRUE;
  AMMPI_assert(ep);

  retval &= AMMPI_freeSendBufferPool(&(ep->Req.sendPool_small));
  retval &= AMMPI_freeSendBufferPool(&(ep->Rep.sendPool_small));
  retval &= AMMPI_freeSendBufferPool(&(ep->Req.sendPool_large));
  retval &= AMMPI_freeSendBufferPool(&(ep->Rep.sendPool_large));

  return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_ReapSendCompletions(ammpi_sendbuffer_pool_t* pool) {
  int numcompleted = 0;
  int i;
  MPI_SAFE(MPI_Testsome(pool->numActive, pool->txHandle, &numcompleted,
                        pool->tmpIndexArray, pool->tmpStatusArray));

  AMMPI_assert(numcompleted >= 0 && numcompleted <= pool->numActive);

#if AMMPI_LINEAR_SEND_COMPLETE
  /* this algorithm is linear in the number of handles (as is MPI_Testsome), 
     but relies on a spec-compliant MPI_Testsome. Appears to be slightly
     slower for most platforms with a small number of completions or deep queue.
  */
  { int numActive = pool->numActive;
    MPI_Request * const txHandle = pool->txHandle;
    for (i = numActive-1; i >= 0; i--) {
      if (txHandle[i] == MPI_REQUEST_NULL) {
        numActive--;
        if (i != numActive) { /* swap a still-active buffer into this place */
          ammpi_buf_t** const txBuf = pool->txBuf;
          ammpi_buf_t* const tmp = txBuf[i];
          AMMPI_assert(txHandle[numActive] != MPI_REQUEST_NULL);
          txHandle[i] = txHandle[numActive];
          txBuf[i] = txBuf[numActive];
          txHandle[numActive] = MPI_REQUEST_NULL;
          txBuf[numActive] = tmp;
        }
      }
    }
    pool->numActive = numActive;
  }
#else /* this algorithm is quadratic in the number of completed operations, 
         but tolerates buggy MPI_Testsome */
  /* sort the completions in ascending order (simple insertion sort) */
  for (i=1; i < numcompleted; i++) {
    int x = pool->tmpIndexArray[i];
    int j;
    for (j = i; j > 0 && pool->tmpIndexArray[j-1] > x; j--) 
      pool->tmpIndexArray[j] = pool->tmpIndexArray[j-1];
    pool->tmpIndexArray[j] = x;
  }

  /* collect completed buffers - maintain invariant that active buffers are all at front */
  for (i=numcompleted-1; i >= 0; i--) {
    int doneidx = pool->tmpIndexArray[i];
    int activeidx = pool->numActive-1;
    AMMPI_assert(doneidx >= 0 && doneidx < pool->numActive);
    #if PLATFORM_OS_AIX
      /* Some versions of IBM MPI (PE v? to v4.2.2.3) fail to set MPI_REQUEST_NULL as required by MPI_Testsome,
         and also apparently fail to reclaim the resources associated with the request */
      if (pool->txHandle[doneidx] != MPI_REQUEST_NULL) {
        MPI_Status s;
        MPI_SAFE(MPI_Wait(&(pool->txHandle[doneidx]),&s));
      }
    #endif
    AMMPI_assert(pool->txHandle[doneidx] == MPI_REQUEST_NULL); 
    if (doneidx != activeidx) {
      /* swap a still-active buffer into this place */
      ammpi_buf_t* tmp = pool->txBuf[doneidx];
      AMMPI_assert(pool->txHandle[activeidx] != MPI_REQUEST_NULL);
      pool->txHandle[doneidx] = pool->txHandle[activeidx];
      pool->txBuf[doneidx] = pool->txBuf[activeidx];
      pool->txHandle[activeidx] = MPI_REQUEST_NULL;
      pool->txBuf[activeidx] = tmp;
    }
    pool->numActive--;
  }
#endif
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
/* acquire a buffer of at least the given size numBytes associated with ep, 
 * to be used in a subsequent non-blocking MPI send operation
 * return a pointer to the buffer and the location that should be used to store the MPI
 * handle when the operation is initiated
 * if isrequest, may poll for an unbounded amount of time until some buffers become free
 * for replies, will not poll and will return in a bounded amount of time
 * the non-blocking send should be initiated before this method is called again
 * return AM_OK to indicate success
 */
extern int AMMPI_AcquireSendBuffer(ep_t ep, int numBytes, int isrequest, 
                            ammpi_buf_t** pbuf, MPI_Request** pHandle) {
  ammpi_sendbuffer_pool_t* pool;
  AMMPI_assert(ep);
  AMMPI_assert(pbuf);
  AMMPI_assert(pHandle);
  AMMPI_assert(numBytes >= AMMPI_MIN_NETWORK_MSG && numBytes <= AMMPI_MAX_NETWORK_MSG);

  /* select the appropriate pool */
  if (numBytes <= AMMPI_SMALL_SENDBUF_SZ) 
    pool = (isrequest ? &ep->Req.sendPool_small : &ep->Rep.sendPool_small);
  else 
    pool = (isrequest ? &ep->Req.sendPool_large : &ep->Rep.sendPool_large);

  /* find a free buffer to fulfill request */
  tryagain:
  if (pool->numActive < pool->numBufs) { /* buffer available */
    const int idx = pool->numActive;
    AMMPI_assert(pool->txBuf[idx] && pool->txHandle[idx] == MPI_REQUEST_NULL);
    AMMPI_assert(((uintptr_t)pool->txBuf[idx]) % AMMPI_BUF_ALIGN == 0);
    *pbuf = pool->txBuf[idx];
    *pHandle = &pool->txHandle[idx];
    pool->numActive++;
    return AM_OK;
  }

  while (1) {
    if (pool->numActive > 0) { /* reap any pending pool completions */
      int retval = AMMPI_ReapSendCompletions(pool);
      if_pf (retval != AM_OK) AMMPI_RETURN(retval);
      if (pool->numActive < pool->numBufs) goto tryagain; /* should now succeed */
    }

    /* nothing immediately available */
    if (isrequest) { /* poll until something available */
      int retval;
      AMMPI_BACKPRESSURE_WARNING("Out of request send buffers");
      retval = AMMPI_ServiceIncomingMessages(ep, FALSE, FALSE); /* NOTE this may actually cause reentrancy to this fn on reply pool */
      if_pf (retval != AM_OK) AMMPI_RETURN(retval);
    } else { 
      #if 1 /* poll the reply network only */
        int retval;
        AMMPI_BACKPRESSURE_WARNING("Out of reply send buffers");
        retval = AMMPI_ServiceIncomingMessages(ep, FALSE, TRUE); 
        if_pf (retval != AM_OK) AMMPI_RETURN(retval);
      #else /* UNSAFE - do not use: can lead to unbounded buffer growth */
        int retval = AMMPI_GrowReplyPool(pool);
        if_pf (retval != AM_OK) AMMPI_RETURN(retval);
      #endif
    }
  }
  AMMPI_FatalErr("AMMPI_AcquireSendBuffer failed");
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
/* old code to grow the reply pool instead of polling -- can lead to unbounded buffer growth */
extern int AMMPI_GrowReplyPool(ammpi_sendbuffer_pool_t* pool) {
    int newnumBufs = pool->numBufs + (int)(pool->numBufs * (AMMPI_REPLYBUF_POOL_GROWTHFACTOR-1));
    MPI_Request *newtxHandle = (MPI_Request *)AMMPI_malloc(newnumBufs*sizeof(MPI_Request));
    ammpi_buf_t**newtxBuf = (ammpi_buf_t**)AMMPI_malloc(newnumBufs*sizeof(ammpi_buf_t*));
    char **newmemBlocks = (char **)AMMPI_malloc(sizeof(char *)*(pool->numBlocks+1));
    char* newBlock = (char*)AMMPI_malloc((newnumBufs-pool->numBufs)*pool->bufSize+AMMPI_BUF_ALIGN);
    int * newtmpIndexArray = (int *)AMMPI_malloc(newnumBufs * sizeof(int));
    MPI_Status *newtmpStatusArray = (MPI_Status *)AMMPI_malloc(newnumBufs * sizeof(MPI_Status));
    int i;
    AMMPI_assert(newnumBufs > pool->numBufs);

    if (!newtxHandle || !newtxBuf || !newmemBlocks || !newBlock || 
        !newtmpIndexArray || !newtmpStatusArray) AMMPI_RETURN_ERR(RESOURCE); /* out of mem */

    #if AMMPI_DEBUG
      fprintf(stderr, "*** AMMPI WARNING: Out of reply send buffers. growing pool...\n"); fflush(stderr);
    #endif

    /* copy old values & preserve ordering */
    memcpy(newtxHandle, pool->txHandle, pool->numBufs*sizeof(MPI_Request));
    memcpy(newtxBuf, pool->txBuf, pool->numBufs*sizeof(ammpi_buf_t*));
    memcpy(newmemBlocks, pool->memBlocks, pool->numBlocks*sizeof(char*));
    newmemBlocks[pool->numBlocks] = newBlock;
    /* tmps needs not be preserved */

    newBlock = (char *)AMMPI_ALIGNUP(newBlock, AMMPI_BUF_ALIGN);
    AMMPI_assert(pool->bufSize % AMMPI_BUF_ALIGN == 0);
    for (i=pool->numBufs; i < newnumBufs; i++) {
      AMMPI_assert(((uintptr_t)newBlock) % AMMPI_BUF_ALIGN == 0);
      newtxBuf[i] = (ammpi_buf_t*)newBlock;
      newBlock += pool->bufSize;
      newtxHandle[i] = MPI_REQUEST_NULL;
    }

    AMMPI_free(pool->txHandle);
    pool->txHandle = newtxHandle;
    AMMPI_free(pool->txBuf);
    pool->txBuf = newtxBuf;
    AMMPI_free(pool->memBlocks);
    pool->memBlocks = newmemBlocks;
    AMMPI_free(pool->tmpIndexArray);
    pool->tmpIndexArray = newtmpIndexArray;
    AMMPI_free(pool->tmpStatusArray);
    pool->tmpStatusArray = newtmpStatusArray;

    pool->numBlocks++;
    pool->numBufs = newnumBufs;

    return AM_OK;
}
#endif
/*------------------------------------------------------------------------------------
 * System initialization/termination
 *------------------------------------------------------------------------------------ */
extern int AM_Init(void) {
  { int initialized = 0;
    MPI_SAFE(MPI_Initialized(&initialized));
    if (!initialized) AMMPI_RETURN_ERRFR(RESOURCE, AM_Init, "MPI not initialized");
  }

  if (ammpi_Initialized == 0) { /* first call */
    /* check system attributes */
    AMMPI_assert(sizeof(int8_t) == 1);
    AMMPI_assert(sizeof(uint8_t) == 1);
    #ifndef INTTYPES_16BIT_MISSING
      AMMPI_assert(sizeof(int16_t) == 2);
      AMMPI_assert(sizeof(uint16_t) == 2);
    #endif
    AMMPI_assert(sizeof(int32_t) == 4);
    AMMPI_assert(sizeof(uint32_t) == 4);
    AMMPI_assert(sizeof(int64_t) == 8);
    AMMPI_assert(sizeof(uint64_t) == 8);

    AMMPI_assert(sizeof(uintptr_t) >= sizeof(void *));

    #if 0
      #define DUMPSZ(T) printf("sizeof(" #T ")=%i\n", sizeof(T))
      DUMPSZ(ammpi_msg_t); DUMPSZ(ammpi_buf_t); DUMPSZ(en_t); DUMPSZ(ammpi_bufstatus_t);
    #endif 
    AMMPI_assert(sizeof(ammpi_msg_t) % 4 == 0);
    AMMPI_assert(sizeof(ammpi_buf_t) % 8 == 0); /* needed for payload alignment */

    { char *buffer;
      /* we attach a buffer regardless of AMMPI_NONBLOCKING_SENDS, 
         to ensure we can always use BSend to perform AM refusal */
      buffer = (char *)AMMPI_malloc(AMMPI_SENDBUFFER_SZ);
      MPI_SAFE(MPI_Buffer_attach(buffer, AMMPI_SENDBUFFER_SZ));
    }
  }
  { const char *syncsend_str = getenv("AMMPI_SYNCSEND_THRESH");
    int thresh = AMMPI_DEFAULT_SYNCSEND_THRESH;
    if (syncsend_str) thresh = atoi(syncsend_str);
    if (thresh < 0) thresh = ((unsigned int)-1)>>1; /* negative == infinite */
    AMMPI_syncsend_thresh = thresh;
  }

  ammpi_Initialized++;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_Terminate(void) {
  int i;
  int retval = AM_OK;
  AMMPI_CHECKINIT();

  if (ammpi_Initialized == 1) { /* last termination call */
    for (i = 0; i < AMMPI_numBundles; i++) {
      if (AM_FreeBundle(AMMPI_bundles[i]) != AM_OK) 
        retval = AM_ERR_RESOURCE;
    }
    AMMPI_numBundles = 0;

    { char *buffer= NULL;
      int sz = 0;
      if (!MPI_SAFE_NORETURN(MPI_Buffer_detach(&buffer, &sz))) retval = AM_ERR_RESOURCE;
      AMMPI_free(buffer);
    }
  }

  ammpi_Initialized--;
  AMMPI_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * endpoint/bundle management
 *------------------------------------------------------------------------------------ */
extern int AM_AllocateBundle(int type, eb_t *endb) {
  eb_t eb;
  AMMPI_CHECKINIT();
  if (type < 0 || type >= AM_NUM_BUNDLE_MODES) AMMPI_RETURN_ERR(BAD_ARG);
  if (type != AM_SEQ) AMMPI_RETURN_ERR(RESOURCE);
  if (AMMPI_numBundles == AMMPI_MAX_BUNDLES-1) AMMPI_RETURN_ERR(RESOURCE);
  if (!endb) AMMPI_RETURN_ERR(BAD_ARG);

  eb = (eb_t)AMMPI_malloc(sizeof(struct ammpi_eb));
  eb->endpoints = (ep_t *)AMMPI_malloc(AMMPI_INITIAL_NUMENDPOINTS*sizeof(ep_t));
  eb->cursize = AMMPI_INITIAL_NUMENDPOINTS;
  eb->n_endpoints = 0;
  eb->event_mask = AM_NOEVENTS;

  AMMPI_bundles[AMMPI_numBundles++] = eb; /* keep track of all bundles */
  *endb = eb;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_FreeBundle(eb_t bundle) {
  if (!bundle) AMMPI_RETURN_ERR(BAD_ARG);
  { int i;

    /* free all constituent endpoints */
    for (i = 0; i < bundle->n_endpoints; i++) {
      int retval = AM_FreeEndpoint(bundle->endpoints[i]);
      if (retval != AM_OK) AMMPI_RETURN(retval);
    }
    AMMPI_assert(bundle->n_endpoints == 0);

    /* remove from bundle list */
    for (i = 0; i < AMMPI_numBundles; i++) {
      if (AMMPI_bundles[i] == bundle) { 
        AMMPI_bundles[i] = AMMPI_bundles[AMMPI_numBundles-1]; 
        break;
      }
    }
    AMMPI_assert(i < AMMPI_numBundles);
    AMMPI_numBundles--;

    AMMPI_free(bundle->endpoints);
    AMMPI_free(bundle);
  }
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_AllocateEndpoint(eb_t bundle, ep_t *endp, en_t *endpoint_name) {
  ep_t ep;
  int retval;

  AMMPI_CHECKINIT();
  if (!bundle || !endp || !endpoint_name) AMMPI_RETURN_ERR(BAD_ARG);

  ep = (ep_t)AMMPI_malloc(sizeof(struct ammpi_ep));
  if (!ep) AMMPI_RETURN_ERRFR(RESOURCE, AM_AllocateEndpoint, "out of memory");

  retval = AMMPI_AllocateEndpointResource(ep);
  if (retval != AM_OK) {
    AMMPI_free(ep);
    AMMPI_RETURN(retval);
  }

  /* setup eb<->ep link */
  AMMPI_InsertEndpoint(bundle, ep);
  ep->eb = bundle;

  /* initialize ep data */
  { int i;
    ep->handler[0] = ammpi_defaultreturnedmsg_handler;
    for (i = 1; i < AMMPI_MAX_NUMHANDLERS; i++) {
      ep->handler[i] = ammpi_unused_handler;
      }
    ep->controlMessageHandler = ammpi_unused_handler;
    ep->tag = AM_NONE;
    ep->segAddr = NULL;
    ep->segLength = 0;
    ep->totalP = 0; 
    ep->depth = -1;

    ep->stats = AMMPI_initial_stats;
    ep->preHandlerCallback = NULL;
    ep->postHandlerCallback = NULL;
  }

  *endp = ep;
  *endpoint_name = ep->name;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_FreeEndpoint(ep_t ea) {
  int retval = AM_OK;
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (!AMMPI_ContainsEndpoint(ea->eb, ea)) AMMPI_RETURN_ERR(RESOURCE);

  if (!AMMPI_FreeEndpointResource(ea)) retval = AM_ERR_RESOURCE;
  if (!AMMPI_FreeEndpointBuffers(ea)) retval = AM_ERR_RESOURCE;

  AMMPI_RemoveEndpoint(ea->eb, ea);
  AMMPI_free(ea);
  AMMPI_RETURN(retval);
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MoveEndpoint(ep_t ea, eb_t from_bundle, eb_t to_bundle) {
  AMMPI_CHECKINIT();
  if (!ea || !from_bundle || !to_bundle) AMMPI_RETURN_ERR(BAD_ARG);
  if (!AMMPI_ContainsEndpoint(from_bundle, ea)) AMMPI_RETURN_ERR(RESOURCE);

  AMMPI_RemoveEndpoint(from_bundle, ea);
  AMMPI_InsertEndpoint(to_bundle, ea);
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Tag management
 *------------------------------------------------------------------------------------ */
extern int AM_SetTag(ep_t ea, tag_t tag) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);

  /*  TODO: return mismatched messages to sender */
  ea->tag = tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTag(ep_t ea, tag_t *tag) {
  AMMPI_CHECKINIT();
  if (!ea || !tag) AMMPI_RETURN_ERR(BAD_ARG);

  *tag = ea->tag;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * VM Segment management
 *------------------------------------------------------------------------------------ */
extern int AM_GetSeg(ep_t ea, void **addr, uintptr_t *nbytes) {
  AMMPI_CHECKINIT();
  if (!ea || !addr || !nbytes) AMMPI_RETURN_ERR(BAD_ARG);
  *addr = ea->segAddr;
  *nbytes = ea->segLength;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetSeg(ep_t ea, void *addr, uintptr_t nbytes) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (nbytes > AMMPI_MAX_SEGLENGTH) AMMPI_RETURN_ERR(BAD_ARG);

  ea->segAddr = addr;
  ea->segLength = nbytes;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_MaxSegLength(uintptr_t* nbytes) {
  AMMPI_CHECKINIT();
  if (!nbytes) AMMPI_RETURN_ERR(BAD_ARG);
  *nbytes = AMMPI_MAX_SEGLENGTH;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Translation management
 *------------------------------------------------------------------------------------ */
extern int AMMPI_Map(ep_t ea, int index, en_t *name, tag_t tag) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);
  if (ea->translation[index].inuse) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to re-map */
  if (ea->depth != -1) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */

  { int commsz; /* check communicator */
    MPI_SAFE(MPI_Comm_size(*ea->Req.mpicomm, &commsz));
    if (name->mpirank < 0 || name->mpirank >= commsz)
      AMMPI_RETURN_ERRFR(RESOURCE, AM_Map, "Bad endpoint name - may be due to a MPI communicator mismatch");
  }

  ea->translation[index].inuse = TRUE;
  ea->translation[index].name = *name;
  ea->translation[index].tag = tag;
  ea->totalP++;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_MapAny(ep_t ea, int *index, en_t *name, tag_t tag) {
  AMMPI_CHECKINIT();
  if (!ea || !index) AMMPI_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to map after call to AM_SetExpectedResources */

  { int commsz; /* check communicator */
    MPI_SAFE(MPI_Comm_size(*ea->Req.mpicomm, &commsz));
    if (name->mpirank < 0 || name->mpirank >= commsz)
      AMMPI_RETURN_ERRFR(RESOURCE, AM_Map, "Bad endpoint name - may be due to a MPI communicator mismatch");
  }

  { ammpi_node_t i;
    for (i = 0; i < ea->translationsz; i++) {
      if (!ea->translation[i].inuse) { /* use this one */
        ea->translation[i].inuse = TRUE;
        ea->translation[i].name = *name;
        ea->translation[i].tag = tag;
        ea->totalP++;  /* track num of translations */
        *index = i;
        return AM_OK;
      }
    }
    AMMPI_RETURN_ERR(RESOURCE); /* none available */
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AM_UnMap(ep_t ea, int index) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMMPI_RETURN_ERR(RESOURCE); /* not mapped */
  if (ea->depth != -1) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to unmap after call to AM_SetExpectedResources */

  ea->translation[index].inuse = FALSE;
  ea->totalP--;  /* track num of translations */
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetNumTranslations(ep_t ea, int *pntrans) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  AMMPI_assert(ea->translationsz <= AMMPI_MAX_NUMTRANSLATIONS);
  *(pntrans) = ea->translationsz;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetNumTranslations(ep_t ea, int ntrans) {
  ammpi_translation_t *temp;
  ammpi_node_t i;
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (ntrans < 0 || ntrans > AMMPI_MAX_NUMTRANSLATIONS) AMMPI_RETURN_ERR(RESOURCE);
  if (ntrans < AMMPI_INIT_NUMTRANSLATIONS) /* don't shrink beyond min value */
    ntrans = AMMPI_INIT_NUMTRANSLATIONS;
  if (ntrans == ea->translationsz) return AM_OK; /* no change */
  if (ea->depth != -1) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to change translationsz after call to AM_SetExpectedResources */

  for (i = ntrans; i < ea->translationsz; i++) {
    if (ea->translation[i].inuse) 
      AMMPI_RETURN_ERR(RESOURCE); /* it's an error to truncate away live maps */
  }
  temp = AMMPI_calloc(sizeof(ammpi_translation_t), ntrans);
  if (!temp) AMMPI_RETURN_ERR(RESOURCE);
  /* we may be growing or truncating the table */
  memcpy(temp, ea->translation, 
         sizeof(ammpi_translation_t)*MIN(ea->translationsz,ntrans));
  AMMPI_free(ea->translation);
  ea->translation = temp;
  ea->translationsz = ntrans;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationInuse(ep_t ea, int i) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);

  if (ea->translation[i].inuse) return AM_OK; /* in use */
  else return AM_ERR_RESOURCE; /* don't complain here - it's a common case */
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationTag(ep_t ea, int i, tag_t *tag) {
  AMMPI_CHECKINIT();
  if (!ea || !tag) AMMPI_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);
  if (!ea->translation[i].inuse) AMMPI_RETURN_ERR(RESOURCE);

  (*tag) = ea->translation[i].tag;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SetTranslationTag(ep_t ea, int index, tag_t tag) {
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (index < 0 || index >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);
  if (!ea->translation[index].inuse) AMMPI_RETURN_ERR(RESOURCE); /* can't change tag if not mapped */

  ea->translation[index].tag = tag;

  #if AMMPI_USE_AMTAGS
    if (ea->depth != -1) { /* after call to AM_SetExpectedResources we must update compressed table */
      ea->perProcInfo[ea->translation[index].id].tag = tag;
    }
  #endif

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetTranslationName(ep_t ea, int i, en_t *gan) {
  AMMPI_CHECKINIT();
  if (!ea || !gan) AMMPI_RETURN_ERR(BAD_ARG);
  if (i < 0 || i >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);
  if (!ea->translation[i].inuse) AMMPI_RETURN_ERR(RESOURCE);

  (*gan) = ea->translation[i].name; 
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetExpectedResources(ep_t ea, int n_endpoints, int n_outstanding_requests) {
  int retval = AM_OK;
  AMMPI_CHECKINIT();
  if (!ea) AMMPI_RETURN_ERR(BAD_ARG);
  if (ea->depth != -1) AMMPI_RETURN_ERR(RESOURCE); /* it's an error to call AM_SetExpectedResources again */
  /* n_endpoints ignored */
  /*if (n_endpoints < 1 || n_endpoints >= ea->translationsz) AMMPI_RETURN_ERR(BAD_ARG);*/
  if (n_outstanding_requests < 1 || n_outstanding_requests > AMMPI_MAX_NETWORKDEPTH) AMMPI_RETURN_ERR(BAD_ARG);

  ea->depth = n_outstanding_requests;

  #if AMMPI_FLOW_CONTROL
    ea->tokens_perhost = 2*ea->depth;
    { const char *t_str = getenv("AMMPI_CREDITS_PP");
      if (t_str) ea->tokens_perhost = atoi(t_str);
      if (ea->tokens_perhost < 1) ea->tokens_perhost = 1;
    }
    ea->tokens_slack = ea->tokens_perhost * 0.75;
    { const char *t_str = getenv("AMMPI_CREDITS_SLACK");
      if (t_str) ea->tokens_slack = atoi(t_str);
    }
    ea->tokens_slack = MIN(ea->tokens_slack,ea->tokens_perhost-1);
  #endif

  if (!AMMPI_AllocateEndpointBuffers(ea)) retval = AM_ERR_RESOURCE;

  /*  compact a copy of the translation table into our perproc info array */
  { ammpi_node_t procid = 0;
    ammpi_node_t i;
    for (i=0; i < ea->translationsz; i++) {
      if (ea->translation[i].inuse) {
        ea->perProcInfo[procid].remoteName = ea->translation[i].name;
        #if AMMPI_USE_AMTAGS
          ea->perProcInfo[procid].tag = ea->translation[i].tag;
        #endif
        #if AMMPI_FLOW_CONTROL
          ea->perProcInfo[procid].tokens_in = 0;
          ea->perProcInfo[procid].tokens_out = ea->tokens_perhost;
        #endif
        #if AMMPI_VERIFY_MPI_ORDERING 
          ea->perProcInfo[procid].seqnum[0].in = 0;
          ea->perProcInfo[procid].seqnum[0].out = 0;
          ea->perProcInfo[procid].seqnum[1].in = 0;
          ea->perProcInfo[procid].seqnum[1].out = 0;
        #endif

        ea->translation[i].id = procid;
        procid++;
        if (procid == ea->totalP) break; /*  should have all of them now */
      }
    }
  }
  AMMPI_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * Handler management
 *------------------------------------------------------------------------------------ */
extern int _AM_SetHandler(ep_t ea, handler_t handler, ammpi_handler_fn_t function) {
  AMMPI_CHECKINIT();
  if (!ea || !function) AMMPI_RETURN_ERR(BAD_ARG);
  if (AMMPI_BADHANDLERVAL(handler)) AMMPI_RETURN_ERR(BAD_ARG);

  ea->handler[handler] = function;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int _AM_SetHandlerAny(ep_t ea, handler_t *handler, ammpi_handler_fn_t function) {
  int i;
  AMMPI_CHECKINIT();
  if (!ea || !function || !handler) AMMPI_RETURN_ERR(BAD_ARG);

  for (i = 1 ; i < AMMPI_MAX_NUMHANDLERS; i++) {
    if (ea->handler[i] == ammpi_unused_handler) { /* find unused entry */
      ea->handler[i] = function;
      *handler = (handler_t)i;
      return AM_OK;
    }
  }
  AMMPI_RETURN_ERR(RESOURCE); /* all in use */
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_RegisterControlMessageHandler(ep_t ea, ammpi_handler_fn_t function) {
  AMMPI_CHECKINIT();
  if (!ea || !function) AMMPI_RETURN_ERR(BAD_ARG);
  ea->controlMessageHandler = function;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Event management
 *------------------------------------------------------------------------------------ */
extern int AM_GetEventMask(eb_t eb, int *mask) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!eb),BAD_ARG);

  *mask = eb->event_mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_SetEventMask(eb_t eb, int mask) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!eb),BAD_ARG);
  AMMPI_CHECK_ERR((mask < 0 || ((ammpi_eventmask_t)mask) >= AM_NUMEVENTMASKS),BAD_ARG);

  eb->event_mask = (uint8_t)mask;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_WaitSema(eb_t eb) {
  int retval;
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!eb),BAD_ARG);
  
  if (eb->event_mask == AM_NOEVENTS) 
    AMMPI_FatalErr("it's an error to block when the mask is not set - will never return");

  /* block here until a message arrives - this polls too */
  retval = AMMPI_Block(eb);
  if (retval != AM_OK) eb->event_mask = AM_NOEVENTS;

  AMMPI_RETURN(retval);
}
/*------------------------------------------------------------------------------------
 * Message interrogation
 *------------------------------------------------------------------------------------ */
extern int AM_GetSourceEndpoint(void *token, en_t *gan) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!token || !gan),BAD_ARG);
  AMMPI_CHECK_ERRFR((!((ammpi_buf_t *)token)->status.handlerRunning),
                    RESOURCE,AM_GetSourceEndpoint,"handler not running");

  *gan = ((ammpi_buf_t *)token)->status.sourceAddr;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_GetSourceId(void *token, int *srcid) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!token || !srcid),BAD_ARG);
  AMMPI_CHECK_ERRFR((!((ammpi_buf_t *)token)->status.handlerRunning),
                    RESOURCE,AM_GetSourceEndpoint,"handler not running");

  *srcid = ((ammpi_buf_t *)token)->status.sourceId;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetDestEndpoint(void *token, ep_t *endp) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!token || !endp),BAD_ARG);
  AMMPI_CHECK_ERRFR((!((ammpi_buf_t *)token)->status.handlerRunning),
                    RESOURCE,AM_GetSourceEndpoint,"handler not running");

  *endp = ((ammpi_buf_t *)token)->status.dest;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AM_GetMsgTag(void *token, tag_t *tagp) {
  AMMPI_CHECKINIT();
  AMMPI_CHECK_ERR((!token || !tagp),BAD_ARG);
  AMMPI_CHECK_ERRFR((!((ammpi_buf_t *)token)->status.handlerRunning),
                    RESOURCE,AM_GetSourceEndpoint,"handler not running");

  #if AMMPI_USE_AMTAGS
    *tagp = ((ammpi_buf_t *)token)->Msg.tag;
  #else
    #if AMMPI_DEBUG_VERBOSE
    { static int first = 1;
      if (first) { first = 0; fprintf(stderr,"WARNING: AM_GetMsgTag called when AM tags disabled (AMMPI_DISABLE_AMTAGS)\n");}
    }
    #endif
    *tagp = ((ammpi_buf_t *)token)->status.dest->tag;
  #endif
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SetHandlerCallbacks(ep_t ep, AMMPI_preHandlerCallback_t preHandlerCallback, 
                                              AMMPI_postHandlerCallback_t postHandlerCallback) {
  AMMPI_CHECKINIT();
  if (!ep) AMMPI_RETURN_ERR(BAD_ARG);
  ep->preHandlerCallback = preHandlerCallback;
  ep->postHandlerCallback = postHandlerCallback;
  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Statistics API
 *------------------------------------------------------------------------------------ */
extern int AMMPI_GetEndpointStatistics(ep_t ep, ammpi_stats_t *stats) { /* called by user to get statistics */
  AMMPI_CHECKINIT();
  if (!ep || !stats) AMMPI_RETURN_ERR(BAD_ARG);
  memcpy(stats, &ep->stats, sizeof(ammpi_stats_t));
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_ResetEndpointStatistics(ep_t ep) {
  AMMPI_CHECKINIT();
  if (!ep) AMMPI_RETURN_ERR(BAD_ARG);
  ep->stats = AMMPI_initial_stats;
  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_AggregateStatistics(ammpi_stats_t *runningsum, ammpi_stats_t *newvalues) {
  int category;
  AMMPI_CHECKINIT();
  if (!runningsum || !newvalues) AMMPI_RETURN_ERR(BAD_ARG);
  for (category = 0; category < ammpi_NumCategories; category++) {
    runningsum->RequestsSent[category] += newvalues->RequestsSent[category];
    runningsum->RequestsReceived[category] += newvalues->RequestsReceived[category];
    runningsum->RepliesSent[category] += newvalues->RepliesSent[category];
    runningsum->RepliesReceived[category] += newvalues->RepliesReceived[category];

    runningsum->RequestDataBytesSent[category] += newvalues->RequestDataBytesSent[category];
    runningsum->ReplyDataBytesSent[category] += newvalues->ReplyDataBytesSent[category];
    runningsum->RequestTotalBytesSent[category] += newvalues->RequestTotalBytesSent[category];
    runningsum->ReplyTotalBytesSent[category] += newvalues->ReplyTotalBytesSent[category];
  }
  runningsum->ReturnedMessages += newvalues->ReturnedMessages;
  #if AMMPI_COLLECT_LATENCY_STATS
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
static int AMMPI_StatPrecision(double val) {
    int prec = 3; 
    while (val >= 10.0 && prec > 0) { val /= 10; prec -= 1; }
    return prec;
}
/* ------------------------------------------------------------------------------------ */
extern const char *AMMPI_DumpStatistics(void *_fp, ammpi_stats_t *stats, int globalAnalysis) {
  FILE *fp = (FILE *)_fp;
  static char msg[4096];
  int64_t requestsSent = 0; 
  int64_t requestsReceived = 0; 
  int64_t repliesSent = 0; 
  int64_t repliesReceived = 0; 
  int64_t reqdataBytesSent = 0; 
  int64_t repdataBytesSent = 0; 
  int64_t reqTotalBytesSent = 0; 
  int64_t repTotalBytesSent = 0; 
  double reqavgpayload[ammpi_NumCategories];
  double repavgpayload[ammpi_NumCategories];
  double avgpayload[ammpi_NumCategories];
  int category;

  AMMPI_assert(ammpi_Initialized);
  AMMPI_assert(stats != NULL);

  #if !AMMPI_COLLECT_STATS
    sprintf(msg, "(AMMPI_COLLECT_STATS disabled)\n");
    if (fp != NULL) fprintf(fp, "%s", msg);
    return msg;
  #endif

  for (category = 0; category < ammpi_NumCategories; category++) {
    requestsSent += stats->RequestsSent[category];
    requestsReceived += stats->RequestsReceived[category];
    repliesSent += stats->RepliesSent[category];
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
  int64_t packetssent = (requestsSent + repliesSent);

  double avgreqdata = (requestsSent > 0 ?  reqdataBytesSent / (double)requestsSent : 0.0);
  double avgrepdata = (repliesSent  > 0 ?  repdataBytesSent / (double)repliesSent : 0.0);
  double avgdata = (packetssent  > 0 ?  dataBytesSent    / (double)packetssent : 0.0);

  double avgreqpacket = (requestsSent > 0 ?
      ((double)(reqTotalBytesSent)) / ((double)requestsSent)
      : 0.0);
  double avgreppacket = (repliesSent > 0 ?
      ((double)(repTotalBytesSent)) / ((double)repliesSent)
      : 0.0);
  double avgpacket =(packetssent > 0 ?
      ((double)(stats->TotalBytesSent)) / ((double)packetssent)
      : 0.0);

  /* batch lines together to improve chance of output together */
  sprintf(msg, 
    " Requests: %8lu sent, %8lu received\n"
    " Replies:  %8lu sent, %8lu received\n"
    " Returned messages:%2lu\n"
  #if AMMPI_COLLECT_LATENCY_STATS
    "Latency (request sent to reply received): \n"
    " min: %8i microseconds\n"
    " max: %8i microseconds\n"
    " avg: %8i microseconds\n"
  #endif

    "Message Breakdown:        Requests     Replies   Avg data sz (Req/Rep/Both)\n"
    " Short  (<=%5i bytes)   %8lu    %8lu   %9.*f/%.*f/%.*f bytes\n"
    " Medium (<=%5i bytes)   %8lu    %8lu   %9.*f/%.*f/%.*f bytes\n"
    " Long   (<=%5i bytes)   %8lu    %8lu   %9.*f/%.*f/%.*f bytes\n"
    " Total                                           %9.*f/%.*f/%.*f bytes\n"

    "Data bytes sent:      %lu/%lu/%lu bytes\n"
    "Total bytes sent:     %lu/%lu/%lu bytes (incl. AM overhead)\n"
    "Bandwidth overhead:   %.2f%%/%.2f%%/%.2f%%\n"        
    "Average packet size:  %.*f/%.*f/%.*f bytes (incl. AM overhead)\n"
    , 
    (unsigned long)requestsSent, (unsigned long)requestsReceived,
    (unsigned long)repliesSent, (unsigned long)repliesReceived,
    (unsigned long)stats->ReturnedMessages,
  #if AMMPI_COLLECT_LATENCY_STATS
    (int)stats->RequestMinLatency,
    (int)stats->RequestMaxLatency,
    (requestsSent>0?(int)(stats->RequestSumLatency / requestsSent):-1),
  #endif

    /* Message breakdown */
    (int)(AMMPI_MAX_SHORT*sizeof(int)),
      (unsigned long)stats->RequestsSent[ammpi_Short], (unsigned long)stats->RepliesSent[ammpi_Short], 
      AMMPI_StatPrecision(reqavgpayload[ammpi_Short]), reqavgpayload[ammpi_Short], 
      AMMPI_StatPrecision(repavgpayload[ammpi_Short]), repavgpayload[ammpi_Short], 
      AMMPI_StatPrecision(avgpayload[ammpi_Short]), avgpayload[ammpi_Short], 
    (int)(AMMPI_MAX_SHORT*sizeof(int) + AMMPI_MAX_MEDIUM),
      (unsigned long)stats->RequestsSent[ammpi_Medium], (unsigned long)stats->RepliesSent[ammpi_Medium], 
      AMMPI_StatPrecision(reqavgpayload[ammpi_Medium]), reqavgpayload[ammpi_Medium], 
      AMMPI_StatPrecision(repavgpayload[ammpi_Medium]), repavgpayload[ammpi_Medium], 
      AMMPI_StatPrecision(avgpayload[ammpi_Medium]), avgpayload[ammpi_Medium], 
    (int)(AMMPI_MAX_SHORT*sizeof(int) + AMMPI_MAX_LONG),
      (unsigned long)stats->RequestsSent[ammpi_Long], (unsigned long)stats->RepliesSent[ammpi_Long], 
      AMMPI_StatPrecision(reqavgpayload[ammpi_Long]), reqavgpayload[ammpi_Long], 
      AMMPI_StatPrecision(repavgpayload[ammpi_Long]), repavgpayload[ammpi_Long], 
      AMMPI_StatPrecision(avgpayload[ammpi_Long]), avgpayload[ammpi_Long], 

    /* avg data payload */
    AMMPI_StatPrecision(avgreqdata), avgreqdata,
    AMMPI_StatPrecision(avgrepdata), avgrepdata,
    AMMPI_StatPrecision(avgdata), avgdata,

    (unsigned long)reqdataBytesSent, (unsigned long)repdataBytesSent, (unsigned long)dataBytesSent,

    (unsigned long)reqTotalBytesSent, (unsigned long)repTotalBytesSent, (unsigned long)stats->TotalBytesSent,
    /* bandwidth overhead */
    (reqTotalBytesSent > 0 ?
      100.0*((double)(reqTotalBytesSent - reqdataBytesSent)) / 
      ((double)reqTotalBytesSent) 
      : 0.0),
    (repTotalBytesSent > 0 ?
      100.0*((double)(repTotalBytesSent - repdataBytesSent)) / 
      ((double)repTotalBytesSent) 
      : 0.0),
    (stats->TotalBytesSent > 0 ?
      100.0*((double)(stats->TotalBytesSent - dataBytesSent)) / 
      ((double)stats->TotalBytesSent) 
      : 0.0),
    /* avg packet size */
    AMMPI_StatPrecision(avgreqpacket), avgreqpacket,
    AMMPI_StatPrecision(avgreppacket), avgreppacket,
    AMMPI_StatPrecision(avgpacket), avgpacket
    );

  if (globalAnalysis) {
    int64_t packetsrecvd = (requestsReceived + repliesReceived);
    int64_t packetslost = packetssent - packetsrecvd;
    sprintf(msg+strlen(msg), "Packets unaccounted for: %6i", abs((int)packetslost));
    if (packetslost > 0) {
      sprintf(msg+strlen(msg), "  (%6.3f%%)\n", (100.0*packetslost)/packetssent);
    }
    else strcat(msg, "\n");
  } 

  if (fp != NULL) fprintf(fp, "%s", msg);
  return msg;
 }
}
/* ------------------------------------------------------------------------------------ */

