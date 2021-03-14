/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_reqrep.cpp $
 * Description: AMUDP Implementations of request/reply operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "amudp_internal.h" // must come after any other headers

/* forward decls */
static int AMUDP_RequestGeneric(amudp_category_t category, 
                          ep_t ep, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);
static int AMUDP_ReplyGeneric(amudp_category_t category, 
                          amudp_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);

#if AMUDP_EXTRA_CHECKSUM
  static void AMUDP_SetChecksum(amudp_msg_t *m, size_t len);
  static void AMUDP_ValidateChecksum(amudp_msg_t const *m, size_t len);
#endif

/*------------------------------------------------------------------------------------
 * Private helpers
 *------------------------------------------------------------------------------------ */
static uint64_t intpow(uint64_t val, uint64_t exp) {
  uint64_t retval = 1;
  for ( ; exp ; exp--) retval *= val;
  return retval;
}
#ifndef STATIC_RETRIES
#define STATIC_RETRIES 30
#endif
static amx_tick_t retryToticks[STATIC_RETRIES];
#define REQUEST_TIMEOUT_TICKS(retrycnt) (                              \
  AMX_assert(retryToticks[0]),                                         \
  (AMX_PREDICT_TRUE((retrycnt) < STATIC_RETRIES) ?                     \
    retryToticks[(retrycnt)] :                                         \
    retryToticks[0] * intpow(AMUDP_RequestTimeoutBackoff,(retrycnt)))  \
  )
extern void AMUDP_InitRetryCache() {
  AMX_assert(!retryToticks[0]);
  if (AMUDP_InitialRequestTimeout_us == AMUDP_TIMEOUT_INFINITE) return;
  amx_tick_t tickout = AMX_us2ticks(AMUDP_InitialRequestTimeout_us);
  amx_tick_t maxticks = AMX_us2ticks(AMUDP_MaxRequestTimeout_us);
  for (int i=0; i < STATIC_RETRIES; i++) {
    AMX_assert(tickout > 0 && tickout <= maxticks);
    retryToticks[i] = tickout;
    tickout = MIN(tickout * AMUDP_RequestTimeoutBackoff, maxticks);
  }
  #if 0  // for debugging retry calc
    for (int i=0; i < STATIC_RETRIES*2; i++) {
      amx_tick_t tick = REQUEST_TIMEOUT_TICKS(i);
      printf("Timeout %2i: %9" PRIu64 " us, %9" PRIu64 " ticks\n",i,tick/AMX_us2ticks(1),tick);
    }
  #endif
}
/* ------------------------------------------------------------------------------------ */
typedef enum { REQUESTREPLY_PACKET, RETRANSMISSION_PACKET, REFUSAL_PACKET } packet_type;
static int sendPacket(ep_t ep, amudp_msg_t *msg, size_t msgsz, en_t destaddress, packet_type type) {
  AMX_assert(ep && msg && msgsz > 0);
  AMX_assert(msgsz <= AMUDP_MAX_MSG);
  AMX_assert(!enEqual(destaddress, ep->name)); // should never be called for loopback

  #if AMX_DEBUG_VERBOSE
    { static int firsttime = 1;
      static int verbosesend = 0;
      if_pf (firsttime) { verbosesend = !!AMUDP_getenv_prefixed("VERBOSE_SEND"); firsttime = 0; }
      if (verbosesend) { 
        AMX_VERBOSE_INFO(("sending %i-byte packet to (%s)", (int)msgsz, AMUDP_enStr(destaddress, 0)));
      }
    }
  #endif

  #if AMUDP_EXTRA_CHECKSUM
    AMUDP_SetChecksum(msg, msgsz);
  #endif

  int retry = 0;
  while (1) { 
    if_pt (sendto(ep->s, (char *)msg, msgsz, /* Solaris requires cast to char* */
                   0, (struct sockaddr *)&destaddress, sizeof(en_t)) > 0 ) { 
      // success
      AMUDP_STATS(ep->stats.TotalBytesSent += msgsz);
      return AM_OK;
    }
    int err = errno;
    if (err == EPERM) {
       /* Linux intermittently gets EPERM failures here at startup for no apparent reason -
          so allow a retry */
      if (retry++ < 5) {
        AMX_VERBOSE_INFO(("Got a '%s'(%i) on sendto(), retrying...", strerror(err), err)); 
        sleep(1);
      } else { // something more serious appears to be wrong..
        AMX_RETURN_ERRFR(RESOURCE, sendPacket, strerror(err));
      }
    } else if (err == ENOBUFS || err == ENOMEM) {
      /* some linuxes also generate ENOBUFS for localhost backpressure - 
         ignore it and treat it as a drop, let retransmisison handle if necessary */
      AMX_DEBUG_WARN(("Got a '%s'(%i) on sendto(%i), ignoring...", strerror(err), err, (int)msgsz)); 
      return AM_OK;
    } else AMX_RETURN_ERRFR(RESOURCE, sendPacket, strerror(err));
  }

}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_GetOpcode(int isrequest, amudp_category_t cat) {
  switch (cat) {
    case amudp_Short:
      if (isrequest) return AM_REQUEST_M;
      else return AM_REPLY_M;
    case amudp_Medium:
      if (isrequest) return AM_REQUEST_IM;
      else return AM_REPLY_IM;
    case amudp_Long:
      if (isrequest) return AM_REQUEST_XFER_M;
      else return AM_REPLY_XFER_M; 
    default: AMX_FatalErr("unrecognized opcode in AMUDP_GetOpcode");
      return -1;
  }
}
/* ------------------------------------------------------------------------------------ */
#define INVALID_NODE ((amudp_node_t)-1)
//  return source id in ep perproc table of this remote addr, or INVALID_NODE for not found 
//  optional hint optimizes lookup
static amudp_node_t sourceAddrToId(ep_t ep, en_t sourceAddr, amudp_node_t hint) {
  amudp_perproc_info_t * const pinfo = ep->perProcInfo;
  // hint values are 8-bit, try all the matching entries
  for (amudp_node_t i = hint; i < ep->P; i += 256) {
    en_t const name = pinfo[i].remoteName;
    if (enEqual(name, sourceAddr)) return i;
  }
  AMX_VERBOSE_INFO(("sourceAddrToId hint missed: hint=%i",(int)hint));
  // hint may be wrong with non-uniform translation tables, brute-force scan
  for (amudp_node_t i = 0; i < ep->P; i++) {
    en_t const name = pinfo[i].remoteName;
    if (enEqual(name, sourceAddr)) return i;
  }
  return INVALID_NODE;
}
/* ------------------------------------------------------------------------------------ */
/* ioctl UDP fiasco:
 * According to POSIX, ioctl(I_NREAD) on a SOCK_DGRAM should report the EXACT size of
 * the next message waiting (or 0), not the number of bytes available on the socket. 
 * We can use this as an optimization in choosing the recv buffer size.
 * Linux (FIONREAD) and Solaris (I_NREAD) get this right, 
 * but all other systems seem to get it wrong, one way or another.
 * Cygwin: (bug 3284) not implemented
 * WSL (4/8/17) returns raw byte count, which can over or under-report
 * FreeBSD: (bug 2827) returns raw byte count, which can over or under-report
 * others: over-report by returning total bytes in all messages waiting
 */
#ifndef IOCTL_WORKS
 #if PLATFORM_OS_LINUX || PLATFORM_OS_SOLARIS || PLATFORM_OS_DARWIN
  #define IOCTL_WORKS 1
 #else
  #define IOCTL_WORKS 0
 #endif
#endif

/* ------------------------------------------------------------------------------------ */
/*  AMUDP_DrainNetwork - read anything outstanding from hardware/kernel buffers into app space */
static int AMUDP_DrainNetwork(ep_t ep) {
    int totalBytesDrained = 0;
    while (1) {
      IOCTL_FIONREAD_ARG_T bytesAvail = 0;
      #if IOCTL_WORKS
        #if PLATFORM_OS_DARWIN // Apple-specific getsockopt(SO_NREAD) returns what we need
          GETSOCKOPT_LENGTH_T junk = sizeof(bytesAvail);
          if_pf (SOCK_getsockopt(ep->s, SOL_SOCKET, SO_NREAD, &bytesAvail, &junk) == SOCKET_ERROR)
            AMX_RETURN_ERRFR(RESOURCE, "getsockopt(SO_NREAD)", strerror(errno));
        #else
          if_pf (SOCK_ioctlsocket(ep->s, _FIONREAD, &bytesAvail) == SOCKET_ERROR)
            AMX_RETURN_ERRFR(RESOURCE, "ioctl(FIONREAD)", strerror(errno));
        #endif

        // sanity check
        if_pf ((size_t)bytesAvail > AMUDP_MAX_MSG) {
          char x;
          int retval = recvfrom(ep->s, (char *)&x, 1, MSG_PEEK, NULL, NULL);
          AMX_Err("bytesAvail=%lu  recvfrom(MSG_PEEK)=%i", (unsigned long)bytesAvail, retval);
          AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: received message that was too long", strerror(errno));
        }
      #else
        if (inputWaiting(ep->s, false)) bytesAvail = AMUDP_MAX_MSG; // conservative assumption
      #endif
      if (bytesAvail == 0) break; 

        /* TODO: another possible workaround for !IOCTL_WORKS:
         * Use a MSG_PEEK of the header to retrieve the header and GET_MSG_SZ
         * to allocate an exact-sized buffer. 
         * Probably not worth the overhead for a short-lived Rx buffer, 
         * especially since some OSs will buffer overrun on MSG_PEEK of a partial datagram.
         * However this same strategy could be used (possibly on a dedicated socket) on any OS
         * to scatter-recv AMLong payloads directly into their final destination, saving a copy.
         */

      /* something waiting, acquire a buffer for it */
      size_t const msgsz = bytesAvail;
      if (ep->rxCnt >= ep->recvDepth) { /* out of buffers - postpone draining */
        AMX_DEBUG_WARN_TH("Receive buffer full - unable to drain network. Consider raising RECVDEPTH or polling more often.");
        break;
      }
      amudp_buf_t *destbuf = AMUDP_AcquireBuffer(ep, MSGSZ_TO_BUFFERSZ(msgsz));

      #if AMUDP_EXTRA_CHECKSUM && AMX_DEBUG
        memset((char *)&destbuf->msg, 0xCC, msgsz); // init recv buffer to a known value
      #endif

      /* perform the receive */
      struct sockaddr sa;
      int sz = sizeof(en_t);
      int retval = myrecvfrom(ep->s, (char *)&destbuf->msg, msgsz, 0, &sa, &sz);

      #if IOCTL_WORKS
        if_pt (retval == (int)msgsz) ; // success
      #else
        if_pt (retval <= (int)msgsz) ; // success
      #endif
        else if_pf (retval == SOCKET_ERROR)
          AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom()", strerror(errno));
        else if_pf (retval == 0)
          AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom() returned zero", strerror(errno));
        else if_pf ((size_t)retval < AMUDP_MIN_MSG) 
          AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: incomplete message received in recvfrom()", strerror(errno));
        else if_pf ((size_t)retval > msgsz) 
            AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: buffer overrun in recvfrom()", strerror(errno));
        else { /* detect broken ioctl implementations */
          AMX_assert(IOCTL_WORKS && retval != (int)bytesAvail);
          AMX_Warn("ioctl() is probably broken: bytesAvail=%i  recvfrom returned=%i", (int)bytesAvail, retval);
        }
      #if AMX_DEBUG
        if_pf (sz != sizeof(en_t)) // should never happen
          AMX_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom() returned wrong sockaddr size", strerror(errno));
      #endif

      #if AMUDP_EXTRA_CHECKSUM
        // the following lines can be uncommented to inject errors and verify the checksum support is working
        //memset(((char*)destbuf)+retval-8, 0, 8);
        //destbuf->msg.chk2 = 4;
        //destbuf->msg.packetlen = 4;
        AMUDP_ValidateChecksum(&(destbuf->msg), retval);
      #endif

      destbuf->status.rx.sourceAddr = *(en_t *)&sa;
      destbuf->status.rx.dest = ep; /* remember which ep recvd this message */
      destbuf->status.rx.sourceId = sourceAddrToId(ep, *(en_t *)&sa, destbuf->msg.systemMessageArg);

      // add it to the recv queue
      destbuf->status.rx.next = NULL;
      if (!ep->rxCnt) { // first element
        AMX_assert(!ep->rxHead && !ep->rxTail);
        ep->rxTail = ep->rxHead = destbuf;
      } else { // append to FIFO
        AMX_assert(ep->rxHead && ep->rxTail);
        AMX_assert(ep->rxHead != ep->rxTail || ep->rxCnt == 1);
        ep->rxTail->status.rx.next = destbuf;
        ep->rxTail = destbuf;
      }
      ep->rxCnt++;

      totalBytesDrained += retval;
    } // drain recv loop

    #if USE_SOCKET_RECVBUFFER_GROW
      /* heuristically decide whether we should expand the OS socket recv buffers */
      if (totalBytesDrained + AMUDP_MAX_MSG > ep->socketRecvBufferSize) {
        /* it's possible we dropped something due to insufficient OS socket buffer space */
        if (!ep->socketRecvBufferMaxedOut) { /* try to do something about it */
          /* TODO: we may want to add some hysterisis here to prevent artifical inflation
           * due to retransmits after a long period of no polling 
           */
          int newsize = 2 * ep->socketRecvBufferSize;

          if (newsize > AMUDP_SOCKETBUFFER_MAX) { /* create a semi-sane upper bound */
            newsize = AMUDP_SOCKETBUFFER_MAX;
            ep->socketRecvBufferMaxedOut = 1;
          }
          ep->socketRecvBufferMaxedOut += AMUDP_growSocketBufferSize(ep, newsize, SO_RCVBUF, "SO_RCVBUF");
        }
      }
    #endif
    return AM_OK; /* done */
}
static int AMUDP_WaitForEndpointActivity(eb_t eb, struct timeval *tv) {
    /* drain network and block up to tv time for endpoint recv buffers to become non-empty (NULL to block)
     * return AM_OK for activity, AM_ERR_ for other error, -1 for timeout 
     * wakeupOnControlActivity controls whether we return on control socket activity (for blocking)
     */

    /* drain network and see if some receive buffer already non-empty */
    for (int i = 0; i < eb->n_endpoints; i++) {
      ep_t ep = eb->endpoints[i];
      int retval = AMUDP_DrainNetwork(ep);
      if (retval != AM_OK) AMX_RETURN(retval);
      if (ep->rxCnt) return AM_OK;
    }

    while (1) {
      fd_set sockset;
      fd_set* psockset = &sockset;
      int maxfd = 0;

      FD_ZERO(psockset);
      for (int i = 0; i < eb->n_endpoints; i++) {
        SOCKET s = eb->endpoints[i]->s;
        FD_SET(s, psockset);
        if ((int)s > maxfd) maxfd = s;
      }
      if (AMUDP_SPMDControlSocket != INVALID_SOCKET) {
        ASYNC_TCP_DISABLE();
        FD_SET(AMUDP_SPMDControlSocket, psockset);
        if ((int)AMUDP_SPMDControlSocket > maxfd) maxfd = AMUDP_SPMDControlSocket;
      }
      /* wait for activity */
      amx_tick_t starttime = AMX_getCPUTicks();
      int retval = select(maxfd+1, psockset, NULL, NULL, tv);
      if (AMUDP_SPMDControlSocket != INVALID_SOCKET) ASYNC_TCP_ENABLE();
      if_pf (retval == SOCKET_ERROR) { 
        AMX_RETURN_ERRFR(RESOURCE, "AMUDP_Block: select()", strerror(errno));
      }
      else if (retval == 0) return -1; /* time limit expired */
      else if_pf (FD_ISSET(AMUDP_SPMDControlSocket, psockset)) {
        AMUDP_SPMDIsActiveControlSocket = TRUE; /* we may have missed a signal */
        AMUDP_SPMDHandleControlTraffic(NULL);
        if (AMUDP_SPMDwakeupOnControlActivity) return AM_OK;
      }
      else return AM_OK; /* activity on some endpoint in bundle */
      amx_tick_t endtime = AMX_getCPUTicks();

      if (tv) { /* readjust remaining time */
        int64_t elapsedtime = AMX_ticks2us(endtime - starttime);
        if (elapsedtime < tv->tv_usec) tv->tv_usec -= elapsedtime;
        else {
          int64_t remainingtime = ((int64_t)tv->tv_sec) * 1000000 + tv->tv_usec;
          remainingtime -= elapsedtime;
          if (remainingtime <= 0) return -1; /* time limit expired */
          tv->tv_sec = (long)(remainingtime / 1000000);
          tv->tv_usec = (long)(remainingtime % 1000000);
        }
      }
    }
}
/* ------------------------------------------------------------------------------------ */
// Manage the doubly-linked tx ring
static void AMUDP_EnqueueTxBuffer(ep_t ep, amudp_buf_t *buf) {
  if (!ep->timeoutCheckPosn) { // empty ring
    AMX_assert(ep->outstandingRequests == 0);
    ep->timeoutCheckPosn = buf;
    buf->status.tx.next = buf;
    buf->status.tx.prev = buf;
    ep->outstandingRequests = 1;
  } else { // insert "behind" current check posn
    AMX_assert(ep->outstandingRequests >= 1);
    buf->status.tx.next = ep->timeoutCheckPosn;
    buf->status.tx.prev = ep->timeoutCheckPosn->status.tx.prev;
    ep->timeoutCheckPosn->status.tx.prev = buf;
    buf->status.tx.prev->status.tx.next = buf;
    ep->outstandingRequests++;
    AMX_assert(ep->outstandingRequests <= ep->sendDepth);
  }
}
static void AMUDP_DequeueTxBuffer(ep_t ep, amudp_buf_t *buf) {
  AMX_assert(buf->status.tx.next);
  AMX_assert(buf->status.tx.prev);
  AMX_assert(ep->timeoutCheckPosn);
  if (buf->status.tx.next == buf) { // removing last element
    AMX_assert(ep->outstandingRequests == 1);
    AMX_assert(buf->status.tx.prev == buf);
    AMX_assert(ep->timeoutCheckPosn == buf);
    ep->timeoutCheckPosn = NULL;
    ep->outstandingRequests = 0;
  } else { // extract from ring
    AMX_assert(ep->outstandingRequests > 1);
    if (ep->timeoutCheckPosn == buf) // advance posn
      ep->timeoutCheckPosn = buf->status.tx.next;
    buf->status.tx.prev->status.tx.next = buf->status.tx.next;
    buf->status.tx.next->status.tx.prev = buf->status.tx.prev;
    ep->outstandingRequests--;
  }
  #if AMX_DEBUG
    buf->status.tx.next = NULL;
    buf->status.tx.prev = NULL;
  #endif
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_HandleRequestTimeouts(ep_t ep, int numtocheck) {
  /* check the next numtocheck requests for timeout (or -1 for all)
   * and retransmit as necessary. return AM_OK or AM_ERR_XXX
   */
  amudp_buf_t *buf = ep->timeoutCheckPosn;

  if (!buf) { // tx ring empty
    AMX_assert(ep->outstandingRequests == 0);
    return AM_OK; 
  }

  amx_tick_t now = AMX_getCPUTicks();

  AMX_assert(ep->outstandingRequests > 0);
  AMX_assert(ep->outstandingRequests <= ep->PD); // sanity: weak test b/c ignores loopback
  if (numtocheck == -1) numtocheck = ep->outstandingRequests;
  else numtocheck = MIN(numtocheck, ep->outstandingRequests);
  for (int i = 0; i < numtocheck; i++) {
    if_pf (buf->status.tx.timestamp <= now) {
      AMX_assert(AMUDP_InitialRequestTimeout_us != AMUDP_TIMEOUT_INFINITE);

      static uint32_t max_retryCount = 0;
      if_pf (!max_retryCount) { // init precomputed values
        if (AMUDP_MaxRequestTimeout_us == AMUDP_TIMEOUT_INFINITE) {
          max_retryCount = (uint32_t)-1;
        } else {
          uint64_t temp = AMUDP_InitialRequestTimeout_us;
          while (temp <= AMUDP_MaxRequestTimeout_us) {
            temp *= AMUDP_RequestTimeoutBackoff;
            max_retryCount++;
          }
        }
      }

      amudp_msg_t * const msg = &buf->msg;
      amudp_category_t const cat = AMUDP_MSG_CATEGORY(msg);
      AMX_assert(AMUDP_MSG_ISREQUEST(msg));
      amudp_node_t const destP = buf->status.tx.destId;

      if_pf (buf->status.tx.retryCount >= max_retryCount) {
        /* we already waited too long - request is undeliverable */
        amx_returned_handler_fn_t handlerfn = (amx_returned_handler_fn_t)ep->handler[0];
        int opcode = AMUDP_GetOpcode(1, cat);

        AMUDP_DequeueTxBuffer(ep, buf);
        amudp_bufdesc_t *txdesc = GET_REQ_DESC(ep, destP, AMUDP_MSG_INSTANCE(msg));
        txdesc->buffer = NULL; // free tx descriptor

        /* pretend this is a bounced recv buffer */
        /* note that source/dest for returned mesgs reflect the virtual "message denied" packet 
         * although it doesn't really matter because the AM2 spec is too vague
         * about the argblock returned message argument for it to be of any use to anyone
         */
        buf->status.rx.sourceId = destP; 
        buf->status.rx.sourceAddr = ep->perProcInfo[destP].remoteName;
        buf->status.rx.dest = ep;

        buf->status.rx.replyIssued = TRUE; /* prevent any reply */
        buf->status.rx.handlerRunning = TRUE;
        AMX_assert(handlerfn != NULL);
        (*handlerfn)(ECONGESTION, opcode, (void *)buf);
        buf->status.rx.handlerRunning = FALSE;

        AMUDP_ReleaseBuffer(ep, buf);
        AMUDP_STATS(ep->stats.ReturnedMessages++);
      } else {
        /* retransmit */
        size_t msgsz = GET_MSG_SZ(msg);
        en_t destaddress = ep->perProcInfo[destP].remoteName;
        /* tag should NOT be changed for retransmit */
        AMX_VERBOSE_INFO(("Retransmitting a request..."));
        int retval = sendPacket(ep, msg, msgsz, destaddress, RETRANSMISSION_PACKET);
        if (retval != AM_OK) AMX_RETURN(retval);        

        uint32_t const retry = buf->status.tx.retryCount + 1;
        buf->status.tx.retryCount = retry;

        now = AMX_getCPUTicks(); // may have blocked in send
        buf->status.tx.timestamp = now + REQUEST_TIMEOUT_TICKS(retry);

        AMUDP_STATS(ep->stats.RequestsRetransmitted[cat]++);
        AMUDP_STATS(ep->stats.RequestTotalBytesSent[cat] += msgsz);
      }
    } // time expired

    buf = buf->status.tx.next; // advance
    AMX_assert(buf);
  }
  
  /* advance checked posn */
  ep->timeoutCheckPosn = buf;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
#define MAXINT64    ((((uint64_t)1) << 63) - 1)
static amx_tick_t AMUDP_FindEarliestRequestTimeout(eb_t eb) {
  /* return the soonest timeout value for an active request
   * (which may have already passed)
   * return 0 for no outstanding requests
   */
  amx_tick_t earliesttime = (amx_tick_t)MAXINT64;
  for (int i = 0; i < eb->n_endpoints; i++) {
    ep_t ep = eb->endpoints[i];
    amudp_buf_t * const startpos = ep->timeoutCheckPosn;
    if (!startpos) continue;
    amudp_buf_t *buf = startpos;
    do { 
      amx_tick_t timestamp = buf->status.tx.timestamp;
      if (timestamp < earliesttime) earliesttime = timestamp;
      buf = buf->status.tx.next;
    } while (buf != startpos);
  }
  if (earliesttime == MAXINT64) return 0;
  else return earliesttime;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_Block(eb_t eb) {
  /* block until some endpoint receive buffer becomes non-empty
   * does not poll, but does handle SPMD control socket events
   */

  /* first, quickly determine if something is already waiting */
  { struct timeval tv = {0,0};
    int retval = AMUDP_WaitForEndpointActivity(eb, &tv);
    if (retval != -1) AMX_RETURN(retval); /* error or something waiting */
  }

  while (1) {
    /* we need to be careful we don't sleep longer than the next packet timeout */
    amx_tick_t nexttimeout = AMUDP_FindEarliestRequestTimeout(eb);
    int retval;
    if (nexttimeout) {
      struct timeval tv;
      amx_tick_t now = AMX_getCPUTicks();
      if (nexttimeout < now) goto timeout; /* already have a request timeout */
      uint32_t const uspause = (uint32_t)AMX_ticks2us(nexttimeout - now);
      tv.tv_sec = (long)(uspause / 1000000);
      tv.tv_usec = (long)(uspause % 1000000);
      retval = AMUDP_WaitForEndpointActivity(eb, &tv);
    } else /* no outstanding requests, so just block */
      retval = AMUDP_WaitForEndpointActivity(eb, NULL); 
    if (retval != -1) AMX_RETURN(retval); /* error or something waiting */
     
    /* some request has timed out - handle it */
    timeout:
    { int i;
      for (i = 0; i < eb->n_endpoints; i++) {
        ep_t ep = eb->endpoints[i];
        if (ep->depth != -1) {
          int retval = AMUDP_HandleRequestTimeouts(ep, -1);
          if (retval != AM_OK) AMX_RETURN(retval);
        }
      }
    }
  }

}
/* ------------------------------------------------------------------------------------ */
#if AMX_DEBUG
  #define REFUSE_NOTICE(reason) AMX_Err("I just refused a message and returned to sender. Reason: %s", reason)
#else
  #define REFUSE_NOTICE(reason) (void)0
#endif

/* this is a local-use-only macro for AMUDP_processPacket */
#define AMUDP_REFUSEMESSAGE(errcode) do {                                       \
    msg->systemMessageType = (uint8_t)amudp_system_returnedmessage;             \
    msg->systemMessageArg = (uint8_t)errcode;                                   \
    if (isloopback) {                                                           \
      AMUDP_processPacket(buf, 1);                                              \
    } else {                                                                    \
      int retval = sendPacket(ep, msg, GET_MSG_SZ(msg),                         \
                        buf->status.rx.sourceAddr, REFUSAL_PACKET);             \
       /* ignore errors sending this */                                         \
      if (retval != AM_OK) AMX_Err("failed to sendPacket to refuse message");   \
      else REFUSE_NOTICE(#errcode);                                             \
    }                                                                           \
    return;                                                                     \
  } while(0)

// Process an incoming buffer from any source, and return when complete
// Does NOT release the buffer
void AMUDP_processPacket(amudp_buf_t * const buf, int isloopback) {
  amudp_msg_t * const msg = &buf->msg;
  ep_t const ep = buf->status.rx.dest;
  amudp_node_t const sourceID = buf->status.rx.sourceId;
  int const numargs = AMUDP_MSG_NUMARGS(msg);
  uint8_t const seqnum = AMUDP_MSG_SEQNUM(msg);
  uint16_t const instance = AMUDP_MSG_INSTANCE(msg);
  int const isrequest = AMUDP_MSG_ISREQUEST(msg);
  amudp_category_t const cat = AMUDP_MSG_CATEGORY(msg);
  int const issystemmsg = ((amudp_system_messagetype_t)msg->systemMessageType) != amudp_system_user;

  /* handle returned messages */
  if_pf (issystemmsg) { 
    amudp_system_messagetype_t type = ((amudp_system_messagetype_t)msg->systemMessageType);
    if_pf (type == amudp_system_returnedmessage) { 
      amx_returned_handler_fn_t handlerfn = (amx_returned_handler_fn_t)ep->handler[0];
      if (sourceID == INVALID_NODE) return; /*  unknown source, ignore message */
      if (isrequest && !isloopback) { /*  the returned message is a request, so free that request buffer */
        amudp_bufdesc_t * const desc = GET_REQ_DESC(ep, sourceID, instance);
        if (desc->buffer && desc->seqNum == seqnum) {
          AMUDP_DequeueTxBuffer(ep, desc->buffer);
          AMUDP_ReleaseBuffer(ep, desc->buffer);
          desc->seqNum = AMUDP_SEQNUM_INC(desc->seqNum);
          desc->buffer = NULL;
          ep->perProcInfo[sourceID].instanceHint = instance;
        }
      }
      op_t opcode = AMUDP_GetOpcode(isrequest, cat);

      /* note that source/dest for returned mesgs reflect the virtual "message denied" packet 
       * although it doesn't really matter because the AM2 spec is too vague
       * about the argblock returned message argument for it to be of any use to anyone
       */
      buf->status.rx.replyIssued = TRUE; /* prevent any reply */
      buf->status.rx.handlerRunning = TRUE;
        AMX_assert(handlerfn != NULL);
        (*handlerfn)(msg->systemMessageArg, opcode, (void *)buf);
      buf->status.rx.handlerRunning = FALSE;
      AMUDP_STATS(ep->stats.ReturnedMessages++);
      return;
    }
  }

  if (!isloopback) {
    if (isrequest) AMUDP_STATS(ep->stats.RequestsReceived[cat]++);
    else AMUDP_STATS(ep->stats.RepliesReceived[cat]++);
  }

  /* perform acceptance checks */

  if_pf (ep->tag == AM_NONE || 
     (ep->tag != msg->tag && ep->tag != AM_ALL))
      AMUDP_REFUSEMESSAGE(EBADTAG);
  if_pf (instance >= ep->depth)
      AMUDP_REFUSEMESSAGE(EUNREACHABLE);
  if_pf (ep->handler[msg->handlerId] == amx_unused_handler &&
      !issystemmsg && msg->handlerId != 0)
      AMUDP_REFUSEMESSAGE(EBADHANDLER);

  switch (cat) {
    case amudp_Short:
      if_pf (msg->nBytes > 0 || msg->destOffset > 0)
        AMUDP_REFUSEMESSAGE(EBADLENGTH);
      break;
    case amudp_Medium:
      if_pf (msg->nBytes > AMUDP_MAX_MEDIUM || msg->destOffset > 0)
        AMUDP_REFUSEMESSAGE(EBADLENGTH);
      break;
    case amudp_Long: 
      /* check segment limits */
      if_pf (msg->nBytes > AMUDP_MAX_LONG)
        AMUDP_REFUSEMESSAGE(EBADLENGTH);
      if_pf ( ep->segLength == 0 || /* empty seg */
              ((uintptr_t)ep->segAddr + msg->destOffset) == 0) /* NULL target */
        AMUDP_REFUSEMESSAGE(EBADSEGOFF);
      if_pf (msg->destOffset + msg->nBytes > ep->segLength)
        AMUDP_REFUSEMESSAGE(EBADLENGTH);
      break;
    default: AMX_unreachable();
  }

  /*  check the source id */
  if_pf (sourceID == INVALID_NODE) AMUDP_REFUSEMESSAGE(EBADENDPOINT);

  // fetch the descriptor relevant to this network message
  amudp_bufdesc_t * const desc = (isloopback ? NULL :
                       AMUDP_get_desc(ep, sourceID, instance, 
                                      !isrequest,  // the alternate descriptor is the relevant one
                                      isrequest)); // should only need to allocate if this is a request

  if (!isloopback) {
    static const char *OOOwarn = "Detected arrival of out-of-order %s!\n"
      " It appears your system is delivering IP packets out-of-order between worker nodes,\n"
      " most likely due to striping over multiple adapters or links.\n"
      " This might (rarely) lead to corruption of AMUDP traffic.";
    /* check sequence number to see if this is a new request/reply or a duplicate */
    if (isrequest) {
      if_pf (seqnum != desc->seqNum) { 
        if_pf (AMUDP_SEQNUM_INC(seqnum) != desc->seqNum) {
          if (OOOwarn) {
            AMX_Warn(OOOwarn, "request");
            OOOwarn = NULL;
          }
          // Out-of-order message can only be a "slow" copy from a previously-completed instance 
          // that included retransmits. Hence, should always be discarded.
          AMUDP_STATS(ep->stats.OutOfOrderRequests++);
          AMX_VERBOSE_INFO(("Ignoring an Out-of-order request."));
          return;
        }
        /* request resent or reply got dropped - resend reply */
        amudp_buf_t * const replybuf = desc->buffer;
        AMX_assert(replybuf);
        amudp_msg_t * const replymsg = &replybuf->msg;
        int cat = AMUDP_MSG_CATEGORY(replymsg);
       
        if (!ep->replyEpoch) ep->replyEpoch = AMX_getCPUTicks();
        if (replybuf->status.tx.timestamp == ep->replyEpoch) {
          // optimization: don't retransmit a reply more than once per epoch 
          // This prevents request retransmit storms that built up while we were inattentive
          // from being exacerbated into reply retransmit storms
          AMX_VERBOSE_INFO(("Got a same-epoch duplicate request - squashing reply retransmit."));
          AMUDP_STATS(ep->stats.RepliesSquashed[cat]++);
          return;
        }
        replybuf->status.tx.timestamp = ep->replyEpoch;

        size_t msgsz = GET_MSG_SZ(replymsg);
        AMX_VERBOSE_INFO(("Got a duplicate request - resending previous reply."));
        int retval = sendPacket(ep, replymsg, msgsz,
            ep->perProcInfo[sourceID].remoteName, RETRANSMISSION_PACKET);
        if (retval != AM_OK) AMX_Err("sendPacket failed while resending a reply");
        AMUDP_STATS(ep->stats.RepliesRetransmitted[cat]++);
        AMUDP_STATS(ep->stats.ReplyTotalBytesSent[cat] += msgsz);
        return;
      }
    } else {
      if (seqnum != desc->seqNum) { /*  duplicate reply, we already ran handler - ignore it */
        if_pf (AMUDP_SEQNUM_INC(seqnum) != desc->seqNum) {
          if (OOOwarn) {
            AMX_Warn(OOOwarn, "reply");
            OOOwarn = NULL;
          }
          // Out-of-order message can only be a "slow" copy from a previously-completed instance 
          // that included retransmits. Hence, should always be discarded.
          AMUDP_STATS(ep->stats.OutOfOrderReplies++);
          AMX_VERBOSE_INFO(("Ignoring an Out-of-order reply."));
          return;
        }
        AMX_VERBOSE_INFO(("Ignoring a duplicate reply."));
        return;
      }
    }

    /* --- message accepted --- */

    if (isrequest) { //  alternate the reply sequence number so duplicates of this request get ignored
        desc->seqNum = AMUDP_SEQNUM_INC(desc->seqNum);
    } else { /* it's a reply, free the corresponding request */
      amudp_buf_t * const reqbuf = desc->buffer;
      if_pt (reqbuf) { 
        #if AMUDP_COLLECT_LATENCY_STATS && AMUDP_COLLECT_STATS
          { /* gather some latency statistics */
            amx_tick_t now = AMX_getCPUTicks();
            amx_tick_t latency = (now - reqbuf->status.tx.firstSendTime);
            ep->stats.RequestSumLatency += latency;
            if (latency < ep->stats.RequestMinLatency) ep->stats.RequestMinLatency = latency;
            if (latency > ep->stats.RequestMaxLatency) ep->stats.RequestMaxLatency = latency;
          }
        #endif
        AMUDP_DequeueTxBuffer(ep, reqbuf);
        AMUDP_ReleaseBuffer(ep, reqbuf);
        desc->seqNum = AMUDP_SEQNUM_INC(desc->seqNum);
        desc->buffer = NULL;
        ep->perProcInfo[sourceID].instanceHint = instance;
      } else { /* request timed out and we decided it was undeliverable, then a reply arrived */
        desc->seqNum = AMUDP_SEQNUM_INC(desc->seqNum);
        /* TODO: seq numbers may get out of sync on timeout 
         * if request got through but replies got lost 
         * we also may do bad things if a reply to an undeliverable message 
         * arrives after we've reused the request buffer (very unlikely)
         * possible soln: add an epoch number
         */
        return; /* reply handler should NOT be run in this situation */
      }
    }
  }

  { /*  run the handler */
    buf->status.rx.replyIssued = FALSE;
    buf->status.rx.handlerRunning = TRUE;
    if (issystemmsg) { /* an AMUDP system message */
      amudp_system_messagetype_t type = ((amudp_system_messagetype_t)(msg->systemMessageType & 0xF));
      switch (type) {
        case amudp_system_autoreply:
          AMX_assert(!isloopback);
          return; /*  already taken care of */
        default: AMX_unreachable();
      }
    } else { /* a user message */
      uint32_t * const pargs = GET_MSG_ARGS(msg);
      handler_t const hid = msg->handlerId;
      switch (cat) {
        case amudp_Short: 
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Short, isrequest, hid, buf, 
                                   NULL, 0, numargs, pargs);
          AMX_RUN_HANDLER_SHORT(ep->handler[hid], buf, pargs, numargs);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        case amudp_Medium: {
          uint8_t * const pData = GET_MSG_DATA(msg);
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Medium, isrequest, hid, buf, 
                                   pData, msg->nBytes, numargs, pargs);
          AMX_RUN_HANDLER_MEDIUM(ep->handler[hid], buf, pargs, numargs, pData, msg->nBytes);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        }
        case amudp_Long: {
          uint8_t * const pData = ((uint8_t *)ep->segAddr) + msg->destOffset;
          /*  a single-message bulk transfer. do the copy */
          if (!isloopback) memcpy(pData, GET_MSG_DATA(msg), msg->nBytes);
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Long, isrequest, hid, buf, 
                                   pData, msg->nBytes, numargs, pargs);
          AMX_RUN_HANDLER_LONG(ep->handler[hid], buf, pargs, numargs, pData, msg->nBytes);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        }
        default: AMX_unreachable();
      }
    }
    buf->status.rx.handlerRunning = FALSE;
    if (!isloopback && isrequest && !buf->status.rx.replyIssued) {
        static va_list va_dummy; /* dummy value - static to prevent uninit warnings */
        /*  user didn't reply, so issue an auto-reply */
        if_pf (AMUDP_ReplyGeneric(amudp_Short, buf, 0, 0, 0, 0, 0, va_dummy, amudp_system_autoreply, 0) 
            != AM_OK) /*  should never happen - don't return here to prevent leaking buffer */
          AMX_Err("Failed to issue auto reply in AMUDP_ServiceIncomingMessages");
    }
  }
}
#undef AMUDP_REFUSEMESSAGE  /* this is a local-use-only macro */
/* ------------------------------------------------------------------------------------ */
/* main message receive workhorse - 
 * drain network once and service available incoming messages, up to AMUDP_MAX_RECVMSGS_PER_POLL
 */
static int AMUDP_ServiceIncomingMessages(ep_t ep) {
  /* drain network */
  int retval = AMUDP_DrainNetwork(ep);
  if (retval != AM_OK) AMX_RETURN(retval);

  ep->replyEpoch = 0;

  for (int i = 0; AMUDP_MAX_RECVMSGS_PER_POLL == 0 || i < MAX(AMUDP_MAX_RECVMSGS_PER_POLL, ep->depth); i++) {
      amudp_buf_t * const buf = ep->rxHead;

      if (!buf) return AM_OK; /* nothing else waiting */

      /* we have a real message waiting - dequeue it */
      ep->rxHead = buf->status.rx.next;
      AMX_assert(ep->rxCnt > 0);
      ep->rxCnt--;
      if (ep->rxCnt == 0) {
        AMX_assert(!ep->rxHead);
        ep->rxTail = NULL;
      }

      if (AMUDP_FaultInjectionEnabled) { /* allow fault injection to drop some revcd messages */
        double randval = rand() / (double)RAND_MAX;
        AMX_assert(randval >= 0.0 && AMUDP_FaultInjectionRate >= 0.0);
        if (randval < AMUDP_FaultInjectionRate) {
          AMX_VERBOSE_INFO(("Fault injection dropping a packet.."));
          goto donewithmessage;
        }
      }
  
      AMUDP_processPacket(buf, 0);
      donewithmessage: /* message handled - continue to next one */

      /* free the handled buffer */
      AMUDP_ReleaseBuffer(ep, buf);

  }  /*  for */
  return AM_OK;
} /*  AMUDP_ServiceIncomingMessages */
/*------------------------------------------------------------------------------------
 * Poll
 *------------------------------------------------------------------------------------ */
extern int AM_Poll(eb_t eb) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!eb, BAD_ARG);

  for (int i = 0; i < eb->n_endpoints; i++) {
    int retval;
    ep_t ep = eb->endpoints[i];

    if_pt (ep->depth != -1) { /* only poll endpoints which have buffers */

      #if USE_ASYNC_TCP_CONTROL
        if_pf (AMUDP_SPMDIsActiveControlSocket) /*  async check */
      #endif
      { retval = AMUDP_SPMDHandleControlTraffic(NULL);
        if_pf (retval != AM_OK) AMX_RETURN(retval);
      }

      retval = AMUDP_ServiceIncomingMessages(ep); /* drain network and check for activity */
      if_pf (retval != AM_OK) AMX_RETURN(retval);

      retval = AMUDP_HandleRequestTimeouts(ep, AMUDP_TIMEOUTS_CHECKED_EACH_POLL);
      if_pf (retval != AM_OK) AMX_RETURN(retval);
    }
  }

  return AM_OK;
}
// poll/block eb while awaiting resource cond
// upon error, execute cleanup and return it
#define BLOCKUNTIL(eb, cond, cleanup) while (!(cond)) { \
   int _retval = AM_OK;                                 \
   if (AMUDP_PoliteSync) {                              \
      _retval = AMUDP_Block(eb);                        \
   }                                                    \
   if_pt (_retval == AM_OK) _retval = AM_Poll(eb);      \
   if_pf (_retval != AM_OK) {                           \
     cleanup;                                           \
     AMX_RETURN(_retval);                               \
   }                                                    \
  }
#define TRANSID_TO_NODEID(ep, transid) (                       \
  AMX_PREDICT_TRUE(!(ep)->translation) ? (amudp_node_t)(transid) : \
    (AMX_assert((transid) < (ep)->translationsz),              \
     (ep)->translation[transid].id)                            \
  )

/*------------------------------------------------------------------------------------
 * Generic Request/Reply
 *------------------------------------------------------------------------------------ */
static int AMUDP_RequestGeneric(amudp_category_t category, 
                          ep_t ep, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr, 
                          uint8_t systemType, uint8_t systemArg) {

  amudp_node_t const destP = TRANSID_TO_NODEID(ep, reply_endpoint);
  amudp_perproc_info_t * const perProcInfo = &ep->perProcInfo[destP];
  en_t const destaddress = perProcInfo->remoteName;
  const int isloopback = enEqual(destaddress, ep->name);

  uint16_t instance;
  amudp_bufdesc_t *outgoingdesc = NULL;

  /*  always poll before sending a request */
  int retval = AM_Poll(ep->eb);
  if_pf (retval != AM_OK) AMX_RETURN(retval);

  size_t const msgsz = COMPUTE_MSG_SZ(numargs, nbytes);
  size_t const buffersz = MSGSZ_TO_BUFFERSZ(msgsz);
  amudp_buf_t * const outgoingbuf = AMUDP_AcquireBuffer(ep, buffersz);

  if (isloopback) {
    instance = 0; /* not used */
  } else { /*  acquire a free request buffer */
    int const depth = ep->depth;
    amudp_bufdesc_t * const descs = GET_REQ_DESC_ALLOC(ep, destP, 0);

    while(1) { // send descriptor acquisition loop
      uint16_t const hint = perProcInfo->instanceHint;
      AMX_assert(hint <= depth);
      amudp_bufdesc_t * const hintdesc = &descs[hint];

      if_pt (!hintdesc->buffer) { /*  hint is right */
        instance = hint;
        outgoingdesc = hintdesc;
        perProcInfo->instanceHint = (hint+1==depth?0:hint+1);
        goto gotinstance;
      } else { /*  hint is wrong */
        /*  search for a free instance */
        instance = hint; 
        do {
          instance = ((instance+1)==depth?0:instance+1);
          amudp_bufdesc_t * const tdesc = &descs[hint];
          if (!tdesc->buffer) {
            outgoingdesc = tdesc;
            goto gotinstance;
          }
        } while (instance != hint);

        /*  no buffers available - wait until one is open 
         *  (hint will point to a free buffer) 
         */
        BLOCKUNTIL(ep->eb, descs[perProcInfo->instanceHint].buffer == NULL,
                  AMUDP_ReleaseBuffer(ep, outgoingbuf)); // prevent leak on error return
      }
    } 

  gotinstance:
    AMX_assert(outgoingdesc);
    AMX_assert(!outgoingdesc->buffer);

    // wait for sendDepth, if necessary
    BLOCKUNTIL(ep->eb, ep->outstandingRequests < ep->sendDepth, 
                  AMUDP_ReleaseBuffer(ep, outgoingbuf)); // prevent leak on error return

    AMX_assert(!outgoingdesc->buffer);
    outgoingdesc->buffer = outgoingbuf; // claim desc
  }

  /*  setup message meta-data */
  amudp_msg_t * const msg = &outgoingbuf->msg;
  if (isloopback) AMUDP_MSG_SETFLAGS(msg, TRUE, category, numargs, 0, 0);
  else AMUDP_MSG_SETFLAGS(msg, TRUE, category, numargs, outgoingdesc->seqNum, instance);
  msg->destOffset = dest_offset;
  msg->handlerId = handler;
  msg->nBytes = (uint16_t)nbytes;
  AMX_assert(systemType == amudp_system_user);
  AMX_assert(systemArg == 0);
  msg->systemMessageType = systemType;
  msg->systemMessageArg = (uint8_t)ep->idHint;
  msg->tag = perProcInfo->tag;
  AMX_assert(GET_MSG_SZ(msg) == msgsz);

  { /*  setup args */
    int i;
    uint32_t *args = GET_MSG_ARGS(msg);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMUDP_MAX_SHORT) args[i] = 0;
    #endif
  }

  if (isloopback) { /* run handler synchronously */
    if (nbytes > 0) { /* setup data */
      if (category == amudp_Long) { /* one-copy: buffer was overallocated, could be reduced with more complexity */
        AMX_CHECK_ERRFRC(dest_offset + nbytes > ep->segLength, BAD_ARG, 
                           "AMRequestXfer", "segment overflow", 
                           AMUDP_ReleaseBuffer(ep, outgoingbuf));
        memmove(((int8_t *)ep->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_MSG_DATA(msg), source_addr, nbytes);
      }
    }
    /* pretend its a recv buffer */
    outgoingbuf->status.rx.dest = ep;
    outgoingbuf->status.rx.sourceId = reply_endpoint;
    outgoingbuf->status.rx.sourceAddr = destaddress;

    AMUDP_processPacket(outgoingbuf, 1);

    AMUDP_ReleaseBuffer(ep, outgoingbuf);
  } else { /* perform the send */

    /*  setup data */
    if (nbytes > 0) {
      memcpy(GET_MSG_DATA(msg), source_addr, nbytes);
    }

    int retval = sendPacket(ep, msg, msgsz, destaddress, REQUESTREPLY_PACKET);
    if_pf (retval != AM_OK) {
      outgoingdesc->buffer = NULL; /*  send failed, so message rejected - release buffer */
      AMUDP_ReleaseBuffer(ep, outgoingbuf);
      perProcInfo->instanceHint = instance;
      AMX_RETURN(retval);
    }

    amx_tick_t now = AMX_getCPUTicks();
    if (AMUDP_InitialRequestTimeout_us == AMUDP_TIMEOUT_INFINITE) { // never timeout
      outgoingbuf->status.tx.timestamp = (amx_tick_t)-1;
    } else {
      outgoingbuf->status.tx.timestamp = now + REQUEST_TIMEOUT_TICKS(0);
    }
    #if AMUDP_COLLECT_LATENCY_STATS
      outgoingbuf->status.tx.firstSendTime = now;
    #endif

    outgoingbuf->status.tx.retryCount = 0;
    outgoingbuf->status.tx.destId = destP;
    AMUDP_EnqueueTxBuffer(ep, outgoingbuf);

    AMUDP_STATS(ep->stats.RequestsSent[category]++);
    AMUDP_STATS(ep->stats.RequestDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMUDP_STATS(ep->stats.RequestTotalBytesSent[category] += msgsz);
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_ReplyGeneric(amudp_category_t category, 
                          amudp_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg) {
  ep_t const ep = requestbuf->status.rx.dest;
  amudp_node_t const destP = requestbuf->status.rx.sourceId;
  const int isloopback = enEqual(requestbuf->status.rx.sourceAddr, ep->name);
  amudp_perproc_info_t const * const perProcInfo = &ep->perProcInfo[destP];

  /*  we don't poll within a reply because by definition we are already polling somewhere in the call chain */

  size_t const msgsz = COMPUTE_MSG_SZ(numargs, nbytes);
  size_t const buffersz = MSGSZ_TO_BUFFERSZ(msgsz);
  amudp_buf_t * const outgoingbuf = AMUDP_AcquireBuffer(ep, buffersz);
  amudp_bufdesc_t *outgoingdesc;
  uint16_t instance;

  if (isloopback) {
    #if AMX_DEBUG
      outgoingdesc = NULL; /* not used */
      instance = 0; /* not used */
    #endif
  } else {
    /*  acquire a free descriptor  */
    /*  trivial because replies always overwrite previous reply in request instance */
    instance = AMUDP_MSG_INSTANCE(&(requestbuf->msg)); 
    outgoingdesc = GET_REP_DESC(ep, destP, instance); // reply desc alloc in processPacket

    if (outgoingdesc->buffer) { /* free buffer of previous reply */
      AMUDP_ReleaseBuffer(ep, outgoingdesc->buffer);
    }
    outgoingdesc->buffer = outgoingbuf;
  }

  /*  setup message meta-data */
  amudp_msg_t * const msg = &outgoingbuf->msg;
  if (isloopback) AMUDP_MSG_SETFLAGS(msg, FALSE, category, numargs, 0, 0);
  else AMUDP_MSG_SETFLAGS(msg, FALSE, category, numargs, 
                          AMUDP_MSG_SEQNUM(&requestbuf->msg), // clone request seqnum, as rep_desc already inc
                          instance);
  msg->destOffset = dest_offset;
  msg->handlerId = handler;
  msg->nBytes = (uint16_t)nbytes;
  AMX_assert(systemType == amudp_system_user || systemType == amudp_system_autoreply);
  AMX_assert(systemArg == 0);
  msg->systemMessageType = systemType;
  msg->systemMessageArg = (uint8_t)ep->idHint;
  msg->tag = perProcInfo->tag;
  AMX_assert(GET_MSG_SZ(msg) == msgsz);

  { /*  setup args */
    int i;
    uint32_t *args = GET_MSG_ARGS(msg);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMUDP_MAX_SHORT) args[i] = 0;
    #endif
  }

  en_t const destaddress = perProcInfo->remoteName;
  if (isloopback) { /* run handler synchronously */
    if (nbytes > 0) { /* setup data */
      if (category == amudp_Long) { /* one-copy */
        AMX_CHECK_ERRFRC(dest_offset + nbytes > ep->segLength, BAD_ARG, 
                           "AMRequestXfer", "segment overflow",
                           AMUDP_ReleaseBuffer(ep, outgoingbuf));
        memmove(((int8_t *)ep->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_MSG_DATA(msg), source_addr, nbytes);
      }
    }

    /* pretend its a recv buffer */
    outgoingbuf->status.rx.dest = ep;
    outgoingbuf->status.rx.sourceId = destP;
    outgoingbuf->status.rx.sourceAddr = destaddress;

    AMUDP_processPacket(outgoingbuf, 1);

    AMUDP_ReleaseBuffer(ep, outgoingbuf);
  } else { /* perform the send */
    /*  setup data */
    memcpy(GET_MSG_DATA(msg), source_addr, nbytes);

    int retval = sendPacket(ep, msg, msgsz, destaddress, REQUESTREPLY_PACKET);
    if_pf (retval != AM_OK) AMX_RETURN(retval);

    if (!ep->replyEpoch) ep->replyEpoch = AMX_getCPUTicks();
    outgoingbuf->status.tx.timestamp = ep->replyEpoch;
    AMUDP_STATS(ep->stats.RepliesSent[category]++);
    AMUDP_STATS(ep->stats.ReplyDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMUDP_STATS(ep->stats.ReplyTotalBytesSent[category] += msgsz);
  }

  requestbuf->status.rx.replyIssued = TRUE;
  return AM_OK;
}

/*------------------------------------------------------------------------------------
 * Request
 *------------------------------------------------------------------------------------ */
extern int AMUDP_RequestVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!request_endpoint, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->depth == -1, NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR(reply_endpoint >= request_endpoint->translationsz, BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->translation && !request_endpoint->translation[reply_endpoint].inuse, BAD_ARG);
  AMX_CHECK_ERR(!request_endpoint->translation && reply_endpoint >= request_endpoint->P, BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  return AMUDP_RequestGeneric(amudp_Short, 
                                  request_endpoint, reply_endpoint, handler, 
                                  NULL, 0, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);

}
extern int AMUDP_Request(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                         int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMUDP_RequestVA(request_endpoint, reply_endpoint, handler, 
                           numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_RequestIVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!request_endpoint, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->depth == -1, NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR(reply_endpoint >= request_endpoint->translationsz, BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->translation && !request_endpoint->translation[reply_endpoint].inuse, BAD_ARG);
  AMX_CHECK_ERR(!request_endpoint->translation && reply_endpoint >= request_endpoint->P, BAD_ARG);
  AMX_CHECK_ERR(!source_addr && nbytes > 0, BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMUDP_MAX_MEDIUM, BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  return AMUDP_RequestGeneric(amudp_Medium, 
                                  request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_RequestI(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMUDP_RequestIVA(request_endpoint, reply_endpoint, handler, 
                              source_addr, nbytes,
                              numargs, argptr);
    va_end(argptr);
    return retval; 
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_RequestXferVA(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async, 
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!request_endpoint, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->depth == -1, NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR(reply_endpoint >= request_endpoint->translationsz, BAD_ARG);
  AMX_CHECK_ERR(request_endpoint->translation && !request_endpoint->translation[reply_endpoint].inuse, BAD_ARG);
  AMX_CHECK_ERR(!request_endpoint->translation && reply_endpoint >= request_endpoint->P, BAD_ARG);
  AMX_CHECK_ERR(!source_addr && nbytes > 0, BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMUDP_MAX_LONG, BAD_ARG);
  AMX_CHECK_ERR(dest_offset > AMUDP_MAX_SEGLENGTH, BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  amudp_node_t const destP = TRANSID_TO_NODEID(request_endpoint, reply_endpoint);
  amudp_perproc_info_t const * const perProcInfo = &request_endpoint->perProcInfo[destP];
  const int isloopback = enEqual(perProcInfo->remoteName, request_endpoint->name);

  if (async && !isloopback) { /*  decide if we can satisfy request without blocking */
      /* it's unclear from the spec whether we should poll before an async failure,
       * but by definition the app must be prepared for handlers to run when calling this 
       * request, so it shouldn't cause anything to break, and the async request is more likely
       * to succeed if we do. so:
       */
      AM_Poll(request_endpoint->eb);

      /* check senddepth */
      if (request_endpoint->outstandingRequests >= request_endpoint->sendDepth)
        AMX_RETURN_ERRFR(IN_USE, AMUDP_RequestXferAsync, "Request can't be satisfied without blocking right now");

      /* see if there's a free buffer */
      amudp_bufdesc_t * const desc = GET_REQ_DESC_ALLOC(request_endpoint, destP, 0);
      uint16_t const hint = perProcInfo->instanceHint;
      int const depth = request_endpoint->depth;
      int i = hint;
      AMX_assert(i >= 0 && i < depth);
      while (1) {
        if (!desc[i].buffer) break;
        i = (i+1==depth ? 0 : i+1);
        if (i == hint) AMX_RETURN_ERRFR(IN_USE, AMUDP_RequestXferAsync, 
                                         "Request can't be satisfied without blocking right now");
      }
  }

  /* perform the send */
  return AMUDP_RequestGeneric(amudp_Long, 
                                  request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_RequestXfer(ep_t request_endpoint, amudp_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async, 
                          int numargs, ...) {
      int retval;
      va_list argptr;
      va_start(argptr, numargs); /*  pass in last argument */
      retval = AMUDP_RequestXferVA(request_endpoint, reply_endpoint, handler, 
                                source_addr, nbytes, dest_offset,
                                async,
                                numargs, argptr);
      va_end(argptr);
      return retval; 
}
/*------------------------------------------------------------------------------------
 * Reply
 *------------------------------------------------------------------------------------ */
extern int AMUDP_ReplyVA(void *token, handler_t handler, 
                       int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!token, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  amudp_buf_t * const buf = (amudp_buf_t *)token;
  amudp_msg_t * const msg = &buf->msg;

  //  semantic checking on reply
  AMX_CHECK_ERR(!AMUDP_MSG_ISREQUEST(msg), RESOURCE);       /* token is not a request */
  AMX_CHECK_ERR(!buf->status.rx.handlerRunning, RESOURCE); /* token is not for an active request */
  AMX_CHECK_ERR(buf->status.rx.replyIssued, RESOURCE);     /* already issued a reply */
  AMX_CHECK_ERR(((amudp_system_messagetype_t)msg->systemMessageType) != amudp_system_user,
                    RESOURCE); /* can't reply to a system message (returned message) */

  return AMUDP_ReplyGeneric(amudp_Short, 
                                  buf, handler, 
                                  NULL, 0, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_Reply(void *token, handler_t handler, 
                       int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMUDP_ReplyVA(token, handler,
                                  numargs, argptr);
    va_end(argptr);
    return retval; 
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_ReplyIVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!token, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_CHECK_ERR(!source_addr && nbytes > 0, BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMUDP_MAX_MEDIUM, BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  amudp_buf_t * const buf = (amudp_buf_t *)token;
  amudp_msg_t * const msg = &buf->msg;

  //  semantic checking on reply
  AMX_CHECK_ERR(!AMUDP_MSG_ISREQUEST(msg), RESOURCE);       /* token is not a request */
  AMX_CHECK_ERR(!buf->status.rx.handlerRunning, RESOURCE); /* token is not for an active request */
  AMX_CHECK_ERR(buf->status.rx.replyIssued, RESOURCE);     /* already issued a reply */
  AMX_CHECK_ERR(((amudp_system_messagetype_t)msg->systemMessageType) != amudp_system_user,
                    RESOURCE); /* can't reply to a system message (returned message) */

  return AMUDP_ReplyGeneric(amudp_Medium, 
                                  buf, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_ReplyI(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMUDP_ReplyIVA(token, handler,
                                  source_addr, nbytes,
                                  numargs, argptr);
    va_end(argptr);
    return retval; 
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_ReplyXferVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR(!token, BAD_ARG);
  AMX_CHECK_ERR(AMUDP_BADHANDLERVAL(handler), BAD_ARG);
  AMX_CHECK_ERR(!source_addr && nbytes > 0, BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMUDP_MAX_LONG, BAD_ARG);
  AMX_CHECK_ERR(dest_offset > AMUDP_MAX_SEGLENGTH, BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  amudp_buf_t * const buf = (amudp_buf_t *)token;
  amudp_msg_t * const msg = &buf->msg;

  //  semantic checking on reply
  AMX_CHECK_ERR(!AMUDP_MSG_ISREQUEST(msg), RESOURCE);       /* token is not a request */
  AMX_CHECK_ERR(!buf->status.rx.handlerRunning, RESOURCE); /* token is not for an active request */
  AMX_CHECK_ERR(buf->status.rx.replyIssued, RESOURCE);     /* already issued a reply */
  AMX_CHECK_ERR(((amudp_system_messagetype_t)msg->systemMessageType) != amudp_system_user,
                    RESOURCE); /* can't reply to a system message (returned message) */

  return AMUDP_ReplyGeneric(amudp_Long, 
                                  buf, handler, 
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMUDP_ReplyXferVA(token, handler,
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr);
    va_end(argptr);
    return retval; 
}
/* ------------------------------------------------------------------------------------ */
extern void AMUDP_DefaultReturnedMsg_Handler(int status, op_t opcode, void *token) {
  const char *statusStr = "*unknown*";
  const char *opcodeStr = "*unknown*";
  amudp_buf_t * const buf = (amudp_buf_t *)token;
  amudp_msg_t * const msg = &buf->msg;
  int numArgs = AMUDP_MSG_NUMARGS(msg);
  uint32_t const * const args = GET_MSG_ARGS(msg);
  char argStr[255];

  #define STATCASE(name, desc) case name: statusStr = #name ": " desc; break;
  switch (status) {
    STATCASE(EBADARGS      , "Arguments to request or reply function invalid    ");
    STATCASE(EBADENTRY     , "X-lation table index selected unbound table entry ");
    STATCASE(EBADTAG       , "Sender's tag did not match the receiver's EP tag  "); 
    STATCASE(EBADHANDLER   , "Invalid index into the recv.'s handler table      "); 
    STATCASE(EBADSEGOFF    , "Offset into the dest-memory VM segment invalid    ");
    STATCASE(EBADLENGTH    , "Bulk xfer length goes beyond a segment's end      ");
    STATCASE(EBADENDPOINT  , "Destination endpoint does not exist               ");
    STATCASE(ECONGESTION   , "Congestion at destination endpoint                ");
    STATCASE(EUNREACHABLE  , "Destination endpoint unreachable                  ");
    STATCASE(EREPLYREJECTED, "Destination endpoint refused reply message        ");
    }
  #define OPCASE(name) case name: opcodeStr = #name; break;
  switch (opcode) {
    OPCASE(AM_REQUEST_M);
    OPCASE(AM_REQUEST_IM);
    OPCASE(AM_REQUEST_XFER_M);
    OPCASE(AM_REPLY_M);
    OPCASE(AM_REPLY_IM);
    OPCASE(AM_REPLY_XFER_M);
  }

  argStr[0] = '\0';
  for (int i=0; i < numArgs; i++) {
    char tmp[20];
    sprintf(tmp, "0x%08x  ", (int)args[i]);
    strcat(argStr, tmp);
  }
  AMX_FatalErr("An active message was returned to sender,\n"
             "    and trapped by the default returned message handler (handler 0):\n"
             "Error Code: %s\n"
             "Message type: %s\n"
             "Destination: %s (%i)\n"
             "Handler: %i\n"
             "Tag: %s\n"
             "Arguments(%i): %s\n"
             "Aborting...",
             statusStr, opcodeStr, 
             AMUDP_enStr(buf->status.rx.sourceAddr, 0), buf->status.rx.sourceId,
             msg->handlerId, AMUDP_tagStr(msg->tag, 0),
             numArgs, argStr);
}
/* ------------------------------------------------------------------------------------ */
#if AMUDP_EXTRA_CHECKSUM
static uint16_t checksum(uint8_t const * const data, size_t len) {
  uint16_t val = 0;
  for (size_t i=0; i < len; i++) { // a simple, fast, non-secure checksum
    uint8_t stir = (uint8_t)(i & 0xFF);
    val = (val << 8) | 
          ( ((val >> 8) & 0xFF) ^ data[i] ^ stir );
  }
  return val;
}
static void AMUDP_SetChecksum(amudp_msg_t * const m, size_t len) {
  AMX_assert(len > 0 && len <= AMUDP_MAX_MSG);
  m->packetlen = (uint32_t)len;
  uint8_t *data = (uint8_t *)&(m->packetlen); 
  uint16_t chk = checksum(data, len - 4); // checksum includes chk* fields
  m->chk1 = chk;
  m->chk2 = chk;
}
static void AMUDP_ValidateChecksum(amudp_msg_t const * const m, size_t len) {
  static char report[512];
  int failed = 0;

  { static int firstcall = 1;
    if (firstcall) AMX_Warn("AMUDP_EXTRA_CHECKSUM is enabled. This mode is ONLY intended for debugging system problems.");
    firstcall = 0;
  }

  if_pf (m->chk1 != m->chk2) {
    strcat(report, " : Checksum field corrupted");
    failed = 1;
  }
  if_pf (len != m->packetlen) {
    strcat(report, " : Length mismatch");
    failed = 1;
  }
  if_pf (len < AMUDP_MIN_MSG || len > AMUDP_MAX_MSG) {
    strcat(report, " : Packet length illegal");
    failed = 1;
  }

  uint8_t const * const data = (uint8_t const *)&(m->packetlen); 
  size_t datalen = len-4;
  uint16_t recvchk = checksum(data, datalen);

  if_pf (recvchk != m->chk1) {
    strcat(report, " : Checksum mismatch on data");
    failed = 1;
  }

  if_pf (failed) {
    // further analysis
    uint8_t val = data[datalen-1];
    int rep = 0;
    for (int i=datalen-1; i >= 0; i--) {
      if (data[i] == val) rep++;
      else break;
    }
    if (rep > 1) {
      char tmp[80];
      sprintf(tmp," : Final %d bytes are 0x%02x",rep,val);
      strcat(report,tmp);
    }
    AMX_FatalErr("UDP packet failed checksum!\n  recvLen: %d  packetlen: %d\n  chk1:0x%04x  chk2:0x%04x  recvchk:0x%04x\n  Analysis%s",
                    (int)len, (int)m->packetlen, m->chk1, m->chk2, recvchk, report);
  }
}
#endif
