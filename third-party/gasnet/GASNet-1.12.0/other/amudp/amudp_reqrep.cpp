/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp_reqrep.cpp,v $
 *     $Date: 2007/03/18 01:10:42 $
 * $Revision: 1.45 $
 * Description: AMUDP Implementations of request/reply operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <amudp_internal.h>

#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#if !PLATFORM_OS_MSWINDOWS
  #include <sys/time.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

/* forward decls */
static int AMUDP_RequestGeneric(amudp_category_t category, 
                          ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);
static int AMUDP_ReplyGeneric(amudp_category_t category, 
                          amudp_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);
/*------------------------------------------------------------------------------------
 * Private helpers
 *------------------------------------------------------------------------------------ */
static int intpow(int val, int exp) {
  int retval = 1;
  int i;
  AMUDP_assert(exp >= 0);
  for (i = 0; i < exp; i++) retval *= val;
  return retval;
}
/* ------------------------------------------------------------------------------------ */
typedef enum { REQUESTREPLY_PACKET, RETRANSMISSION_PACKET, REFUSAL_PACKET } packet_type;
static int sendPacket(ep_t ep, amudp_buf_t *packet, int packetlength, en_t destaddress, packet_type packettype) {
  AMUDP_assert(ep && packet && packetlength > 0);
  #if USE_TRUE_BULK_XFERS
    AMUDP_assert(packetlength <= AMUDP_MAXBULK_NETWORK_MSG);
  #else
    AMUDP_assert(packetlength <= AMUDP_MAX_NETWORK_MSG);
  #endif

  #if AMUDP_DEBUG_VERBOSE
    { static int firsttime = 1;
      static int verbosesend = 0;
      if (firsttime) { verbosesend = !!AMUDP_getenv_prefixed("VERBOSE_SEND"); firsttime = 0; }
      if (verbosesend) { 
        char temp[80];
        fprintf(stderr, "sending packet to (%s)\n", AMUDP_enStr(destaddress, temp)); fflush(stderr);
      }
    }
  #endif

  #ifdef UETH
    switch (packettype) {
      case RETRANSMISSION_PACKET:
      case REQUESTREPLY_PACKET: /*  address is pre-set */
        if (ueth_send_preset(packet, packetlength, &packet->bufhandle) != UETH_OK) {
          AMUDP_RETURN_ERRFR(RESOURCE, sendPacket, "ueth_send_preset() failed");
        }
        break;
      case REFUSAL_PACKET: /*  address is not pre-set */
        if (ueth_send(packet, packetlength, &destaddress, &packet->bufhandle) != UETH_OK) {
          AMUDP_RETURN_ERRFR(RESOURCE, sendPacket, "ueth_send() failed");
        }
      break;
      default: AMUDP_FatalErr("bad UETH packet type");
    }
  #else
    if (sendto(ep->s, (char *)packet, packetlength, /* Solaris requires cast to char* */
               0, (struct sockaddr *)&destaddress, sizeof(en_t)) == SOCKET_ERROR) {
      int err = errno;
      int i = 0;
      while (err == EPERM && i++ < 5) {
         /* Linux intermittently gets EPERM failures here at startup for no apparent reason -
            so allow a retry */
        #if AMUDP_DEBUG_VERBOSE
           AMUDP_Warn("Got a '%s'(%i) on sendto(), retrying...", strerror(err), err); 
        #endif
        sleep(1);
        if (sendto(ep->s, (char *)packet, packetlength,
               0, (struct sockaddr *)&destaddress, sizeof(en_t)) != SOCKET_ERROR) goto success;
        err = errno;
      }
      if (err == ENOBUFS || err == ENOMEM) {
        /* some linuxes also generate ENOBUFS for localhost backpressure - 
           ignore it and treat it as a drop, let retransmisison handle if necessary */
        AMUDP_Warn("Got a '%s'(%i) on sendto(%i), ignoring...", strerror(err), err,packetlength); 
        goto success;
      }
      AMUDP_RETURN_ERRFR(RESOURCE, sendPacket, sockErrDesc());
      success: ;
    }
  #endif
  AMUDP_STATS(ep->stats.TotalBytesSent += packetlength);
  return AM_OK;
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
    default: AMUDP_FatalErr("bad AM category");
      return -1;
  }
}
/* ------------------------------------------------------------------------------------ */
static int sourceAddrToId(ep_t ep, en_t sourceAddr) {
  /*  return source id in ep perproc table of this remote addr, or -1 for not found */
  int i; /*  TODO: make this more efficient */
  for (i = 0; i < ep->P; i++) {
    if (enEqual(ep->perProcInfo[i].remoteName, sourceAddr))
      return i;
  }
  return -1;
}
/* ------------------------------------------------------------------------------------ */
#define RUN_HANDLER_SHORT(phandlerfn, token, pArgs, numargs) do {                       \
  AMUDP_assert(phandlerfn != NULL);                                                                   \
  if (numargs == 0) (*(AMUDP_HandlerShort)phandlerfn)((void *)token);                   \
  else {                                                                                \
    uint32_t *args = (uint32_t *)(pArgs); /* eval only once */                          \
    switch (numargs) {                                                                  \
      case 1:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0]); break;         \
      case 2:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1]); break;\
      case 3:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2]); break; \
      case 4:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3]); break; \
      case 5:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4]); break; \
      case 6:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5]); break; \
      case 7:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break; \
      case 8:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break; \
      case 9:  (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]); break; \
      case 10: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]); break; \
      case 11: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]); break; \
      case 12: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11]); break; \
      case 13: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12]); break; \
      case 14: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13]); break; \
      case 15: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14]); break; \
      case 16: (*(AMUDP_HandlerShort)phandlerfn)((void *)token, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15]); break; \
      default: AMUDP_FatalErr("bad AM arg count");                                                                 \
    }                                                                                   \
  }                                                                                     \
} while (0)
/* ------------------------------------------------------------------------------------ */
#define _RUN_HANDLER_MEDLONG(phandlerfn, token, pArgs, numargs, pData, datalen) do {   \
  AMUDP_assert(phandlerfn != NULL);                                                         \
  if (numargs == 0) (*phandlerfn)(token, pData, datalen);                     \
  else {                                                                      \
    uint32_t *args = (uint32_t *)(pArgs); /* eval only once */                \
    switch (numargs) {                                                        \
      case 1:  (*phandlerfn)(token, pData, datalen, args[0]); break;         \
      case 2:  (*phandlerfn)(token, pData, datalen, args[0], args[1]); break;\
      case 3:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2]); break; \
      case 4:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3]); break; \
      case 5:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4]); break; \
      case 6:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5]); break; \
      case 7:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break; \
      case 8:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break; \
      case 9:  (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]); break; \
      case 10: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]); break; \
      case 11: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]); break; \
      case 12: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11]); break; \
      case 13: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12]); break; \
      case 14: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13]); break; \
      case 15: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14]); break; \
      case 16: (*phandlerfn)(token, pData, datalen, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15]); break; \
      default: AMUDP_FatalErr("bad AM arg count");                                                                 \
    }                                                                                   \
  }                                                                                     \
} while (0)
#define RUN_HANDLER_MEDIUM(phandlerfn, token, pArgs, numargs, pData, datalen) do {      \
    AMUDP_assert(((int)(uintptr_t)pData) % 8 == 0);  /* we guarantee double-word alignment for data payload of medium xfers */ \
    _RUN_HANDLER_MEDLONG((AMUDP_HandlerMedium)phandlerfn, (void *)token, pArgs, numargs, (void *)pData, (int)datalen); \
    } while(0)
#define RUN_HANDLER_LONG(phandlerfn, token, pArgs, numargs, pData, datalen)             \
  _RUN_HANDLER_MEDLONG((AMUDP_HandlerLong)phandlerfn, (void *)token, pArgs, numargs, (void *)pData, (int)datalen)
/* ------------------------------------------------------------------------------------ */
#ifdef UETH
  #define AMUDP_DrainNetwork(ep) AM_OK  /*  already happens asynchronously for ueth */

  static int AMUDP_WaitForEndpointActivity(eb_t eb, struct timeval *tv) {
    /* drain network and block up to tv time for endpoint recv buffers to become non-empty (NULL to block)
     * return AM_OK for activity, AM_ERR_ for other error, -1 for timeout 
     * wakeupOnControlActivity controls whether we return on control socket activity (for blocking)
     */
    ep_t ep = eb->endpoints[0];
    AMUDP_assert(eb->n_endpoints == 1);
    
    if (tv == NULL) { /* block indefinitely */
      /* we need to wake up and poll SPMD control periodically */
      while (1) {
        int waittime = 100000; /* every 100 milliseconds */
        int retval = ueth_recv(NULL, NULL, waittime, NULL);    
        if (retval == UETH_OK) return AM_OK;
        else if (retval == UETH_ERR_TIMEDOUT) {
          int activity = 0;
          AMUDP_SPMDHandleControlTraffic(&activity);
          if (activity && AMUDP_SPMDwakeupOnControlActivity) return AM_OK;
          continue;
        } else AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_WaitForEndpointActivity, "ueth_recv NULL wait failed");
      }
    } else {
      int retval;
      int waittime = tv->tv_sec * 1000000 + tv->tv_usec;
      while (waittime > 0) {
        int thiswait = MIN(waittime, 100000); /* never stop for more than 100 ms or we might miss a shutdown */
        retval = ueth_recv(NULL, NULL, thiswait, NULL);    
        if (retval == UETH_OK) return AM_OK;
        else if (retval == UETH_ERR_TIMEDOUT) {
          int activity = 0;
          AMUDP_SPMDHandleControlTraffic(&activity);
          if (activity && AMUDP_SPMDwakeupOnControlActivity) return AM_OK;
        } else AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_WaitForEndpointActivity, "ueth_recv NULL wait failed");

        waittime -= thiswait;
      }
      return -1; /*  timed out */
    }
  }
/* ------------------------------------------------------------------------------------ */
#else
/* ioctl UDP fiasco:
 * ioctlsocket(FIONREAD) on a SOCK_DGRAM should return 
 * the size of the next message waiting, not the total data
 * available on the socket. We need this to decide whether 
 * or not we have an incoming bulk message next on the queue
 * This works on Linux, but Win2K seems to botch it (despite the 
 * fact their own Winsock spec says it returns the next message size)
 */
#if PLATFORM_OS_MSWINDOWS || PLATFORM_OS_CYGWIN
  #define BROKEN_IOCTL 1
#elif PLATFORM_OS_AIX || PLATFORM_OS_IRIX || PLATFORM_OS_HPUX || PLATFORM_OS_MTA || \
      PLATFORM_OS_TRU64 || PLATFORM_OS_DARWIN || PLATFORM_OS_SUPERUX || \
      PLATFORM_OS_FREEBSD || PLATFORM_OS_NETBSD || PLATFORM_OS_OPENBSD || PLATFORM_OS_UNICOS
  #define BROKEN_IOCTL 1 /*  seems these are broken too...  */
#else 
  #define BROKEN_IOCTL 0 /*  at least Linux and Solaris work as documented */
#endif

/* ------------------------------------------------------------------------------------ */
  /*  AMUDP_DrainNetwork - read anything outstanding from hardware/kernel buffers into app space */
  static int AMUDP_DrainNetwork(ep_t ep) {
    int totalBytesDrained = 0;
    while (1) {
      IOCTL_FIONREAD_ARG_T bytesAvail = 0;
      if_pf (SOCK_ioctlsocket(ep->s, _FIONREAD, &bytesAvail) == SOCKET_ERROR)
        AMUDP_RETURN_ERRFR(RESOURCE, "ioctlsocket()", sockErrDesc());
      if (bytesAvail == 0) break; 

      #if BROKEN_IOCTL && USE_TRUE_BULK_XFERS
        if ((int)bytesAvail > AMUDP_MAX_NETWORK_MSG) { 
          /* this workaround is a HACK that lets us decide if we truly have a bulk message */
          static char *junk = NULL;
          int retval;
          /* MUST use AMUDP_MAXBULK_NETWORK_MSG here, because some OS's blatently ignore
             the message buffer len and happily overflow the input buffer in recvfrom()
           */
          if_pf (!junk) junk = (char *)AMUDP_malloc(AMUDP_MAXBULK_NETWORK_MSG);
          retval = recvfrom(ep->s, junk, AMUDP_MAXBULK_NETWORK_MSG, MSG_PEEK, NULL, NULL);
          if (retval == SOCKET_ERROR && 
            #ifdef WINSOCK
              WSAGetLastError() != WSAEMSGSIZE)
            #else
              errno != EFAULT) /* AIX */
            #endif
            AMUDP_RETURN_ERRFR(RESOURCE, "recv(MSG_PEEK) - broken ioctl Hack", sockErrDesc());
          if (retval < (int)bytesAvail) bytesAvail = retval; /* the true next message size */
        }
        /* TODO: another possible workaround for BROKEN_IOCTL && USE_TRUE_BULK_XFERS:
          use non-peek recvmsg(), with an iovec pointing first to a non-bulk buffer
          (with length AMUDP_MAX_NETWORK_MSG) and the second entry pointing to 
          offset AMUDP_MAX_NETWORK_MSG in the middle of a bulk buffer 
          (with len AMUDP_MAXBULK_NETWORK_MSG - AMUDP_MAX_NETWORK_MSG). 
          Then if we have a true bulk message (based on return value), 
          just copy the initial portion into the bulk buffer and release the normal buf
         */
      #endif

      /* something waiting, acquire a buffer for it */
      { amudp_buf_t *freebuf;
        amudp_buf_t *destbuf;
        int destbufsz = AMUDP_MAX_NETWORK_MSG;
        struct sockaddr sa;
        int retval;
        int sz = sizeof(en_t);
        if (((ep->rxFreeIdx + 1) % ep->rxNumBufs) == ep->rxReadyIdx) { 
          /* out of buffers - postpone draining */
          #if AMUDP_DEBUG
            AMUDP_Warn("Receive buffer full - unable to drain network (this is usually caused by retransmissions)");
          #endif
          break;
        }
        freebuf = &ep->rxBuf[ep->rxFreeIdx];
        #if USE_TRUE_BULK_XFERS
          #if !BROKEN_IOCTL
            /* can't do this check when ioctl is broken */
            if_pf ((int)bytesAvail > AMUDP_MAXBULK_NETWORK_MSG) {
              char x;
              int retval = recvfrom(ep->s, (char *)&x, 1, MSG_PEEK, NULL, NULL);
              fprintf(stderr, "bytesAvail=%lu  recvfrom(MSG_PEEK)=%i\n", (unsigned long)bytesAvail, retval); fflush(stderr);
              AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: received message that was too long", sockErrDesc());
            }
          #endif
          if ((int)bytesAvail > AMUDP_MAX_NETWORK_MSG) { /* this is a true bulk buffer */
            destbuf = AMUDP_AcquireBulkBuffer(ep);
            freebuf->status.bulkBuffer = destbuf;
            destbufsz = AMUDP_MAXBULK_NETWORK_MSG;
          }
          else destbuf = freebuf;
        #else
          destbuf = freebuf;
        #endif
        destbuf->status.bulkBuffer = NULL;

        retval = myrecvfrom(ep->s, (char *)destbuf, destbufsz, 0, 
                          &sa, &sz);

        if_pf (retval == SOCKET_ERROR)
          AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom()", sockErrDesc());
        else if_pf (retval == 0)
          AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom() returned zero", sockErrDesc());
        else if_pf (retval < AMUDP_MIN_NETWORK_MSG) 
          AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: incomplete message received in recvfrom()", sockErrDesc());
        else if_pf (retval > destbufsz) 
            AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: buffer overrun in recvfrom()", sockErrDesc());
        #if AMUDP_DEBUG && !BROKEN_IOCTL
        else if_pf (retval != (int)bytesAvail) { /* detect other broken ioctl implementations */
          fprintf(stderr, "bytesAvail=%i  recvfrom returned:%i  ioctl() is probably broken\n", (int)bytesAvail, retval); fflush(stderr);
        }
        #endif
        totalBytesDrained += retval;
        if (sz != sizeof(en_t))
          AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_DrainNetwork: recvfrom() returned wrong sockaddr size", sockErrDesc());
        freebuf->status.sourceAddr = *(en_t *)&sa;
        freebuf->status.handlerRunning = FALSE;
        ep->rxFreeIdx = (ep->rxFreeIdx + 1) % ep->rxNumBufs; /* mark in use */
      }
    }
    #if !defined(UETH) && USE_SOCKET_RECVBUFFER_GROW
      /* heuristically decide whether we should expand the OS socket recv buffers */
      if (totalBytesDrained + AMUDP_MAXBULK_NETWORK_MSG > ep->socketRecvBufferSize) {
        /* it's possible we dropped something due to insufficient OS socket buffer space */
        if (!ep->socketRecvBufferMaxedOut) { /* try to do something about it */
          /* TODO: we may want to add some hysterisis here to prevent artifical inflation
           * due to retransmits after a long period of no polling 
           */
          /*int newsize = ep->socketRecvBufferSize + AMUDP_MAXBULK_NETWORK_MSG; - too slow */
          int newsize = 2 * ep->socketRecvBufferSize;
          int sanitymax = AMUDP_RECVBUFFER_MAX;

          if (newsize > sanitymax) { /* create a semi-sane upper bound */
            AMUDP_growSocketBufferSize(ep, sanitymax, SO_RCVBUF, "SO_RCVBUF");
            ep->socketRecvBufferMaxedOut = 1;
          } else { 
            ep->socketRecvBufferMaxedOut = AMUDP_growSocketBufferSize(ep, newsize, SO_RCVBUF, "SO_RCVBUF");
          }
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

    {/* drain network and see if some receive buffer already non-empty */
      int i;
      for (i = 0; i < eb->n_endpoints; i++) {
        ep_t ep = eb->endpoints[i];
        int retval = AMUDP_DrainNetwork(ep);
        if (retval != AM_OK) AMUDP_RETURN(retval);
      }
      for (i = 0; i < eb->n_endpoints; i++) {
        ep_t ep = eb->endpoints[i];
        if (ep->rxReadyIdx != ep->rxFreeIdx) return AM_OK;
      }
    }

    while (1) {
      fd_set sockset;
      fd_set* psockset = &sockset;
      int i;
      int maxfd = 0;
      amudp_cputick_t starttime, endtime;

      FD_ZERO(psockset);
      for (i = 0; i < eb->n_endpoints; i++) {
        FD_SET(eb->endpoints[i]->s, psockset);
        if ((int)eb->endpoints[i]->s > maxfd) maxfd = eb->endpoints[i]->s;
      }
      if (AMUDP_SPMDControlSocket != INVALID_SOCKET) {
        ASYNC_TCP_DISABLE();
        FD_SET(AMUDP_SPMDControlSocket, psockset);
        if ((int)AMUDP_SPMDControlSocket > maxfd) maxfd = AMUDP_SPMDControlSocket;
      }
      /* wait for activity */
      starttime = getCPUTicks();
      { int retval = select(maxfd+1, psockset, NULL, NULL, tv);
        if (AMUDP_SPMDControlSocket != INVALID_SOCKET) ASYNC_TCP_ENABLE();
        if_pf (retval == SOCKET_ERROR) { 
          AMUDP_RETURN_ERRFR(RESOURCE, "AMUDP_Block: select()", sockErrDesc());
        }
        else if (retval == 0) return -1; /* time limit expired */
      }
      if (FD_ISSET(AMUDP_SPMDControlSocket, psockset)) {
        AMUDP_SPMDIsActiveControlSocket = TRUE; /* we may have missed a signal */
        AMUDP_SPMDHandleControlTraffic(NULL);
        if (AMUDP_SPMDwakeupOnControlActivity) break;
      }
      else break; /* activity on some endpoint in bundle */
      endtime = getCPUTicks();

      if (tv) { /* readjust remaining time */
        int64_t elapsedtime = ticks2us(endtime - starttime);
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

    return AM_OK; /* some endpoint activity is waiting */
  }
#endif
/* ------------------------------------------------------------------------------------ */
static int AMUDP_HandleRequestTimeouts(ep_t ep, int numtocheck) {
  /* check the next numtocheck requests for timeout (or -1 for all)
   * and retransmit as necessary. return AM_OK or AM_ERR_XXX
   */
  amudp_cputick_t now = getCPUTicks();
  static amudp_cputick_t initial_requesttimeout_cputicks = 0;
  int numdesc = ep->PD;
  int curpos = ep->timeoutCheckPosn;
  if (numtocheck <= 0 || numtocheck > numdesc) numtocheck = numdesc;

  for (int i = 0; i < numtocheck; i++) {
    amudp_bufdesc_t* rd = &ep->requestDesc[curpos];
    if_pf (rd->inuse && rd->timestamp <= now && 
           AMUDP_InitialRequestTimeout_us != AMUDP_TIMEOUT_INFINITE) {
      amudp_buf_t *basicbuf = &ep->requestBuf[curpos];
      amudp_buf_t *outgoingbuf = 
        (basicbuf->status.bulkBuffer ? 
         basicbuf->status.bulkBuffer :
         basicbuf);
      amudp_bufstatus_t *outgoingstatus = &ep->requestBuf[curpos].status;
      amudp_bufdesc_t *outgoingdesc = &ep->requestDesc[curpos];
      int retryCount = outgoingdesc->retryCount;
      static int max_retryCount = 0;
      static int firsttime = 1;
      if (firsttime) {
        if (AMUDP_MaxRequestTimeout_us == AMUDP_TIMEOUT_INFINITE) {
          max_retryCount = 0;
        } else {
          uint32_t temp = AMUDP_InitialRequestTimeout_us;
          while (temp <= AMUDP_MaxRequestTimeout_us) {
            temp *= AMUDP_RequestTimeoutBackoff;
            max_retryCount++;
          }
        }
        initial_requesttimeout_cputicks = us2ticks(AMUDP_InitialRequestTimeout_us);
        firsttime = 0;
      }
      if (retryCount >= max_retryCount && max_retryCount) {
        /* we already waited too long - request is undeliverable */
        int isrequest = AMUDP_MSG_ISREQUEST(&outgoingbuf->Msg);
        amudp_category_t cat = AMUDP_MSG_CATEGORY(&outgoingbuf->Msg);
        AMUDP_HandlerReturned handlerfn = (AMUDP_HandlerReturned)ep->handler[0];
        int opcode = AMUDP_GetOpcode(isrequest, cat);
        int destP = GET_REMOTEPROC_FROM_POS(ep, curpos);
        int instance = GET_INST_FROM_POS(ep, curpos);

        /* pretend this is a bounced recv buffer */
        /* note that source/dest for returned mesgs reflect the virtual "message denied" packet 
         * although it doesn't really matter because the AM2 spec is too vague
         * about the argblock returned message argument for it to be of any use to anyone
         */
        outgoingstatus->sourceId = (uint16_t)destP; 
        outgoingstatus->sourceAddr = ep->perProcInfo[destP].remoteName;
        outgoingstatus->dest = ep;

        outgoingstatus->replyIssued = TRUE; /* prevent any reply */
        outgoingstatus->handlerRunning = TRUE;
        AMUDP_assert(handlerfn != NULL);
        (*handlerfn)(ECONGESTION, opcode, (void *)outgoingbuf);
        outgoingstatus->handlerRunning = FALSE;

        outgoingdesc->inuse = FALSE; /* free it */  
        ep->outstandingRequests--;
        if (basicbuf->status.bulkBuffer) {
          AMUDP_ReleaseBulkBuffer(ep, basicbuf->status.bulkBuffer);
          basicbuf->status.bulkBuffer = NULL;
        }
        ep->perProcInfo[destP].instanceHint = (uint16_t)instance;
        AMUDP_STATS(ep->stats.ReturnedMessages++);
      } else {
        retryCount++;
        outgoingdesc->retryCount = retryCount;
      
        /* retransmit */
        { /*  perform the send */
          int destP = GET_REMOTEPROC_FROM_POS(ep, curpos);
          int packetlength = GET_PACKET_LENGTH(outgoingbuf);
          en_t destaddress = ep->perProcInfo[destP].remoteName;
          /* tag should NOT be changed for retransmit */
          #ifdef UETH
            if (ueth_query_send(outgoingbuf, outgoingbuf->bufhandle)) { 
              /*  previous send still waiting in outgoing FIFO, so don't bother to retransmit at this time */
              #if AMUDP_DEBUG_VERBOSE
                fprintf(stderr, "Skipping a retransmit (last send still in FIFO)..."); fflush(stderr);
              #endif
            } else
          #endif
          {
            int retval;
            int cat = AMUDP_MSG_CATEGORY(&outgoingbuf->Msg);
            #if AMUDP_DEBUG_VERBOSE
              fprintf(stderr, "Retransmitting a request..."); fflush(stderr);
            #endif
            retval = sendPacket(ep, outgoingbuf, packetlength, destaddress, RETRANSMISSION_PACKET);
            if (retval != AM_OK) AMUDP_RETURN(retval);        
            outgoingdesc->transmitCount++;
            AMUDP_STATS(ep->stats.RequestsRetransmitted[cat]++);
            AMUDP_STATS(ep->stats.RequestTotalBytesSent[cat] += packetlength);
          }

          amudp_cputick_t timetowait = initial_requesttimeout_cputicks * 
           intpow(AMUDP_RequestTimeoutBackoff, retryCount);
          outgoingdesc->timestamp = getCPUTicks() + timetowait;
        }
      }
    }
    curpos++;
    if (curpos >= numdesc) curpos = 0;
  }
  
  /* advance checked posn */
  AMUDP_assert(curpos >= 0 && curpos < numdesc);
  ep->timeoutCheckPosn = curpos;

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
#define MAXINT64    ((((uint64_t)1) << 63) - 1)
#define MAXUINT64   ((uint64_t)-1)
static amudp_cputick_t AMUDP_FindEarliestRequestTimeout(eb_t eb) {
  /* return the soonest timeout value for an active request
   * (which may have already passed)
   * return 0 for no outstanding requests
   */
  amudp_cputick_t earliesttime = (amudp_cputick_t)MAXINT64;
  int i;
  for (i = 0; i < eb->n_endpoints; i++) {
    ep_t ep = eb->endpoints[i];
    if (ep->outstandingRequests == 0) continue;
    int numdesc = ep->PD;
    int j;
    for (j = 0; j < numdesc; j++) {
      if (ep->requestDesc[j].inuse) {
        amudp_cputick_t timestamp = ep->requestDesc[j].timestamp;
        if (timestamp < earliesttime) earliesttime = timestamp;
      }
    }
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
    if (retval != -1) AMUDP_RETURN(retval); /* error or something waiting */
  }

  while (1) {
    /* we need to be careful we don't sleep longer than the next packet timeout */
    amudp_cputick_t nexttimeout = AMUDP_FindEarliestRequestTimeout(eb);
    int retval;
    if (nexttimeout) {
      struct timeval tv;
      amudp_cputick_t now = getCPUTicks();
      if (nexttimeout < now) goto timeout; /* already have a request timeout */
      uint32_t const uspause = (uint32_t)ticks2us(nexttimeout - now);
      tv.tv_sec = (long)(uspause / 1000000);
      tv.tv_usec = (long)(uspause % 1000000);
      retval = AMUDP_WaitForEndpointActivity(eb, &tv);
    } else /* no outstanding requests, so just block */
      retval = AMUDP_WaitForEndpointActivity(eb, NULL); 
    if (retval != -1) AMUDP_RETURN(retval); /* error or something waiting */
     
    /* some request has timed out - handle it */
    timeout:
    { int i;
      for (i = 0; i < eb->n_endpoints; i++) {
        ep_t ep = eb->endpoints[i];
        int retval = AMUDP_HandleRequestTimeouts(ep, -1);
        if (retval != AM_OK) AMUDP_RETURN(retval);
      }
    }
  }

}
/* ------------------------------------------------------------------------------------ */
#if AMUDP_DEBUG
  #define REFUSE_NOTICE(reason) AMUDP_Err("I just refused a message and returned to sender. Reason: %s", reason)
#else
  #define REFUSE_NOTICE(reason) (void)0
#endif

/* this is a local-use-only macro for AMUDP_ServiceIncomingMessages */
#define AMUDP_REFUSEMESSAGE(errcode) do {                                       \
    buf->Msg.systemMessageType = (uint8_t)amudp_system_returnedmessage;         \
    buf->Msg.systemMessageArg = (uint8_t)errcode;                               \
    if (isloopback) {                                                           \
      AMUDP_processPacket(buf, 1);                                              \
    } else {                                                                    \
      int retval = sendPacket(ep, buf, GET_PACKET_LENGTH(buf),                  \
                        (basicbuf)->status.sourceAddr, REFUSAL_PACKET);         \
       /* ignore errors sending this */                                         \
      if (retval != AM_OK) AMUDP_Err("failed to sendPacket to refuse message"); \
      else REFUSE_NOTICE(#errcode);                                             \
    }                                                                           \
    return;                                                                     \
  } while(0)

void AMUDP_processPacket(amudp_buf_t *basicbuf, int isloopback) {
  /* basicbuf: the (possible placeholder) buffer in the recv queue that holds status bits */
  /* buf: the true buffer or bulk buffer that holds the msg */
  amudp_buf_t *buf = (basicbuf->status.bulkBuffer ? basicbuf->status.bulkBuffer : basicbuf);
  amudp_msg_t * const msg = &buf->Msg;
  amudp_bufstatus_t * const status = &basicbuf->status; /* the status block for this buffer */
  ep_t const ep = status->dest;
  int const sourceID = status->sourceId;
  int const numargs = AMUDP_MSG_NUMARGS(msg);
  int seqnum = AMUDP_MSG_SEQNUM(msg);
  uint16_t instance = AMUDP_MSG_INSTANCE(msg);
  int const isrequest = AMUDP_MSG_ISREQUEST(msg);
  amudp_category_t const cat = AMUDP_MSG_CATEGORY(msg);
  int const issystemmsg = ((amudp_system_messagetype_t)msg->systemMessageType) != amudp_system_user;

  /* handle returned messages */
  if (issystemmsg) { 
    amudp_system_messagetype_t type = ((amudp_system_messagetype_t)msg->systemMessageType);
    if_pf (type == amudp_system_returnedmessage) { 
      AMUDP_HandlerReturned handlerfn = (AMUDP_HandlerReturned)ep->handler[0];
      op_t opcode;
      if (sourceID < 0) return; /*  unknown source, ignore message */
      if (isrequest && !isloopback) { /*  the returned message is a request, so free that request buffer */
        uint16_t instance = AMUDP_MSG_INSTANCE(msg);
        amudp_bufdesc_t *desc = GET_REQ_DESC(ep, sourceID, instance);
        amudp_buf_t *basicreqbuf = GET_REQ_BUF(ep, sourceID, instance);
        if (desc->inuse && desc->seqNum == seqnum) {
          desc->inuse = FALSE;
          ep->outstandingRequests--;
          if (basicreqbuf->status.bulkBuffer) {
            AMUDP_ReleaseBulkBuffer(ep, basicreqbuf->status.bulkBuffer);
            basicreqbuf->status.bulkBuffer = NULL;
          }
          desc->seqNum = (uint8_t)!(desc->seqNum);
          ep->perProcInfo[sourceID].instanceHint = instance;
        }
      }
      opcode = AMUDP_GetOpcode(isrequest, cat);

      /* note that source/dest for returned mesgs reflect the virtual "message denied" packet 
       * although it doesn't really matter because the AM2 spec is too vague
       * about the argblock returned message argument for it to be of any use to anyone
       */
      status->replyIssued = TRUE; /* prevent any reply */
      status->handlerRunning = TRUE;
        AMUDP_assert(handlerfn != NULL);
        (*handlerfn)(msg->systemMessageArg, opcode, (void *)basicbuf);
      status->handlerRunning = FALSE;
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
  if_pf (ep->handler[msg->handlerId] == amudp_unused_handler &&
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
      #if USE_TRUE_BULK_XFERS
        if_pf (msg->nBytes > AMUDP_MAX_LONG)
          AMUDP_REFUSEMESSAGE(EBADLENGTH);
      #else
        if_pf (msg->nBytes > AMUDP_MAX_MEDIUM)
          AMUDP_REFUSEMESSAGE(EBADLENGTH);
      #endif
      if_pf ( ep->segLength == 0 || /* empty seg */
              ((uintptr_t)ep->segAddr + msg->destOffset) == 0) /* NULL target */
        AMUDP_REFUSEMESSAGE(EBADSEGOFF);
      if_pf (msg->destOffset + msg->nBytes > ep->segLength)
        AMUDP_REFUSEMESSAGE(EBADLENGTH);
      break;
    default: AMUDP_FatalErr("bad AM category");
  }

  /*  check the source id */
  if_pf (sourceID < 0) AMUDP_REFUSEMESSAGE(EBADENDPOINT);

  if (!isloopback) {
    /* check sequence number to see if this is a new request/reply or a duplicate */
    if (isrequest) {
      amudp_bufdesc_t *desc = GET_REP_DESC(ep, sourceID, instance);
      if_pf (seqnum != desc->seqNum) { 
        /*  request resent or reply got dropped - resend reply */
        amudp_buf_t *replybuf = GET_REP_BUF(ep, sourceID, instance);
        AMUDP_assert(replybuf != NULL);
        if (replybuf->status.bulkBuffer) replybuf = replybuf->status.bulkBuffer;
        #ifdef UETH
          if (ueth_query_send(replybuf, replybuf->bufhandle)) { 
            /*  previous send still waiting in outgoing FIFO, so don't bother to retransmit at this time */
            #if AMUDP_DEBUG_VERBOSE
              fprintf(stderr, "Skipping a reply retransmit (last send still in FIFO)..."); fflush(stderr);
            #endif
          } else
        #endif
        { int retval;
          int cat = AMUDP_MSG_CATEGORY(&replybuf->Msg);
          int packetlen = GET_PACKET_LENGTH(replybuf);
          #if AMUDP_DEBUG_VERBOSE
            AMUDP_Warn("Got a duplicate request - resending previous reply.");
          #endif
          retval = sendPacket(ep, replybuf, packetlen,
            ep->perProcInfo[sourceID].remoteName, RETRANSMISSION_PACKET);
          if (retval != AM_OK) AMUDP_Err("sendPacket failed while resending a reply");
          desc->transmitCount++;
          AMUDP_STATS(ep->stats.RepliesRetransmitted[cat]++);
          AMUDP_STATS(ep->stats.ReplyTotalBytesSent[cat] += packetlen);
          /*  ignore error return */
        }
        return;
      }
    } else {
      amudp_bufdesc_t *desc = GET_REQ_DESC(ep, sourceID, instance);
      if (seqnum != desc->seqNum) { /*  duplicate reply, we already ran handler - ignore it */
        #if AMUDP_DEBUG_VERBOSE
          AMUDP_Warn("Ignoring a duplicate reply.");
        #endif
        return;
      }
    }

    /* --- message accepted --- */

    if (!isrequest) { /* it's a reply, free the corresponding request */
      amudp_bufdesc_t *desc = GET_REQ_DESC(ep, sourceID, instance);
      if_pt (desc->inuse) { 
        amudp_buf_t *basicreqbuf = GET_REQ_BUF(ep, sourceID, instance);
        desc->inuse = FALSE;
        ep->outstandingRequests--;
        if (basicreqbuf->status.bulkBuffer) {
          AMUDP_ReleaseBulkBuffer(ep, basicreqbuf->status.bulkBuffer);
          basicreqbuf->status.bulkBuffer = NULL;
        }
        desc->seqNum = (uint8_t)!(desc->seqNum); 
        ep->perProcInfo[sourceID].instanceHint = instance;
        #if AMUDP_COLLECT_LATENCY_STATS && AMUDP_COLLECT_STATS
          { /* gather some latency statistics */
            amudp_cputick_t now = getCPUTicks();
            amudp_cputick_t latency = (now - desc->firstSendTime);
            ep->stats.RequestSumLatency += latency;
            if (latency < ep->stats.RequestMinLatency) ep->stats.RequestMinLatency = latency;
            if (latency > ep->stats.RequestMaxLatency) ep->stats.RequestMaxLatency = latency;
          }
        #endif
      } else { /* request timed out and we decided it was undeliverable, then a reply arrived */
        desc->seqNum = (uint8_t)!(desc->seqNum); /* toggle the seq num */
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
    status->replyIssued = FALSE;
    status->handlerRunning = TRUE;
    if (issystemmsg) { /* an AMUDP system message */
      amudp_system_messagetype_t type = ((amudp_system_messagetype_t)(msg->systemMessageType & 0xF));
      switch (type) {
        case amudp_system_autoreply:
          AMUDP_assert(!isloopback);
          /*  do nothing, already taken care of */
          break;
        case amudp_system_bulkxferfragment:
          /*  perform bulk copy of fragment */
          AMUDP_assert(!isloopback);
          memcpy(((int8_t *)ep->segAddr) + msg->destOffset, GET_PACKET_DATA(buf), msg->nBytes);
          { /* update our slot info and see if bulk xfer is complete */
            int slotnum = msg->systemMessageType >> 4;
            bulkslot_t *slot = &ep->perProcInfo[sourceID].inboundBulkSlot[slotnum];
            if (slot->packetsRemaining == 0) {
              /*  this is the first packet of bulk xfer */
              AMUDP_assert(msg->systemMessageArg > 0);
              slot->packetsRemaining = msg->systemMessageArg;
              slot->minDestOffset = msg->destOffset;
              slot->runningLength = msg->nBytes;

              if (numargs > 0) { /* arguments arrived with this fragment - cache them */
                slot->numargs = numargs;
                memcpy(slot->args, GET_PACKET_ARGS(buf), numargs*4);
              } else slot->numargs = 0;
            } else { /*  not first, but possibly last  */
              AMUDP_assert(slot->packetsRemaining <= msg->systemMessageArg);
              slot->packetsRemaining--;
              if (msg->destOffset < slot->minDestOffset) slot->minDestOffset = msg->destOffset;
              slot->runningLength += msg->nBytes;
              if (slot->packetsRemaining == 0) { /*  just processed last message, now run handler */
                /*  mark it as a user message before running handler */
                buf->Msg.systemMessageType = amudp_system_user; 
                buf->Msg.systemMessageArg = 0;

                int tmpnumargs=0;
                uint32_t *tmpargs=NULL;
                if (numargs > 0) /* args arrived in final fragment */
                  { tmpnumargs = numargs; tmpargs = GET_PACKET_ARGS(buf); }
                else /* args were cached by a previous fragment */
                  { tmpnumargs = slot->numargs; tmpargs = slot->args; }
                
                RUN_HANDLER_LONG(ep->handler[msg->handlerId], basicbuf,
                  tmpargs, tmpnumargs,  
                  (((int8_t *)ep->segAddr) + slot->minDestOffset), slot->runningLength);
              } else if (numargs > 0) { /* arguments arrived with this fragment - cache them */
                slot->numargs = numargs;
                memcpy(slot->args, GET_PACKET_ARGS(buf), numargs*4);
              }
            }
          }
          break;
        default: AMUDP_FatalErr("bad AM type");
      }
    } else { /* a user message */
      switch (cat) {
        case amudp_Short: 
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Short, isrequest, msg->handlerId, basicbuf, 
                                   NULL, 0, numargs, GET_PACKET_ARGS(buf));
          RUN_HANDLER_SHORT(ep->handler[msg->handlerId], basicbuf, 
                            GET_PACKET_ARGS(buf), numargs);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        case amudp_Medium: 
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Medium, isrequest, msg->handlerId, basicbuf, 
                                   GET_PACKET_DATA(buf), msg->nBytes, numargs, GET_PACKET_ARGS(buf));
          RUN_HANDLER_MEDIUM(ep->handler[msg->handlerId], basicbuf, 
                             GET_PACKET_ARGS(buf), numargs, 
                             GET_PACKET_DATA(buf), msg->nBytes);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        case amudp_Long: {
          int8_t *pData = ((int8_t *)ep->segAddr) + msg->destOffset;
          /*  a single-message bulk transfer. do the copy */
          if (!isloopback) memcpy(pData, GET_PACKET_DATA(buf), msg->nBytes);
          if (ep->preHandlerCallback) 
            ep->preHandlerCallback(amudp_Long, isrequest, msg->handlerId, basicbuf, 
                                   pData, msg->nBytes, numargs, GET_PACKET_ARGS(buf));
          RUN_HANDLER_LONG(ep->handler[msg->handlerId], basicbuf, 
                             GET_PACKET_ARGS(buf), numargs, 
                             pData, msg->nBytes);
          if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
          break;
        }
        default: AMUDP_FatalErr("bad AM category");
      }
    }
    status->handlerRunning = FALSE;
    if (!isloopback) {
      if_pf (isrequest && !status->replyIssued) {
        static va_list va_dummy; /* dummy value - static to prevent uninit warnings */
        /*  user didn't reply, so issue an auto-reply */
        if (AMUDP_ReplyGeneric(amudp_Short, basicbuf, 0, 0, 0, 0, 0, va_dummy, amudp_system_autoreply, 0) 
            != AM_OK) /*  should never happen - don't return here to prevent leaking buffer */
          AMUDP_Err("Failed to issue auto reply in AMUDP_ServiceIncomingMessages");
      }
      if (isrequest) { /*  message was a request, alternate the reply sequence number so duplicates of this request get ignored */
        amudp_bufdesc_t *desc = GET_REP_DESC(ep, sourceID, instance);
        desc->seqNum = (uint8_t)!(desc->seqNum);
      }
    }
  }
}
#undef AMUDP_REFUSEMESSAGE  /* this is a local-use-only macro */
/* ------------------------------------------------------------------------------------ */
/* main message receive workhorse - 
 * drain network once and service available incoming messages, up to AMUDP_MAX_RECVMSGS_PER_POLL
 */
static int AMUDP_ServiceIncomingMessages(ep_t ep) {
  #ifndef UETH
    /* drain network */
    int retval = AMUDP_DrainNetwork(ep);
    if (retval != AM_OK) AMUDP_RETURN(retval);
  #endif

  for (int i = 0; AMUDP_MAX_RECVMSGS_PER_POLL == 0 || i < MAX(AMUDP_MAX_RECVMSGS_PER_POLL, ep->depth); i++) {
    amudp_buf_t *basicbuf; /* the (possible placeholder) buffer in the recv queue that holds status bits */
    amudp_bufstatus_t * status; /* the status block for this buffer */

    #ifdef UETH
    { unsigned int buflen;
      en_t sourceAddr;

      int retval = ueth_recv((void **)&basicbuf, &buflen, 0, &sourceAddr); 
      status = &basicbuf->status;
      switch (retval) {
        case UETH_ERR_TIMEDOUT:
          return AM_OK; /* nothing else waiting */
        case UETH_OK:
	  status->bulkBuffer = NULL; /*  TODO: support true bulk on UETH */

          #if AMUDP_DEBUG_VERBOSE
          { char temp[80];
            printf("ueth_recv returned %i buflen=%i sourceAddr=%s\n", 
              retval, buflen, AMUDP_enStr(sourceAddr, temp));
            fflush(stdout);
          }
          #endif

          AMUDP_CHECK_ERRFR((buflen > AMUDP_MAX_NETWORK_MSG),
            RESOURCE, AMUDP_ServiceIncomingMessages, "buffer overrun - received message too long");
          AMUDP_CHECK_ERRFR((buflen < AMUDP_MIN_NETWORK_MSG),
            RESOURCE, AMUDP_ServiceIncomingMessages, "incomplete message received in ueth_recv()");

          status->sourceAddr = sourceAddr;
        break;
        default:
          AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_ServiceIncomingMessages, "ueth_recv NULL wait failed");
      }
    }
    #else
      if (ep->rxReadyIdx == ep->rxFreeIdx) return AM_OK; /* nothing else waiting */
      /* we have a real message waiting - get it */
      AMUDP_assert(ep->rxReadyIdx < ep->rxNumBufs);
      AMUDP_assert(ep->rxFreeIdx < ep->rxNumBufs);
      AMUDP_assert(ep->rxReadyIdx != ep->rxFreeIdx);
      basicbuf = &ep->rxBuf[ep->rxReadyIdx];
      status = &basicbuf->status;
      AMUDP_CHECK_ERRFR((basicbuf->status.handlerRunning),
        RESOURCE, AMUDP_ServiceIncomingMessages, "user caused a poll to occur while handler on the same bundle was running");
    #endif

      status->dest = ep; /* remember which ep recvd this message */
      status->sourceId = (uint16_t)sourceAddrToId(ep, status->sourceAddr);

      if (AMUDP_FaultInjectionEnabled) { /* allow fault injection to drop some revcd messages */
        double randval = rand() / (double)RAND_MAX;
        AMUDP_assert(randval >= 0.0 && AMUDP_FaultInjectionRate >= 0.0);
        if (randval < AMUDP_FaultInjectionRate) {
          #if AMUDP_DEBUG_VERBOSE
            fprintf(stderr, "fault injection dropping a packet..\n"); fflush(stderr);
          #endif
          goto donewithmessage;
        }
      }
  
      AMUDP_processPacket(basicbuf, 0);
      donewithmessage: /* message handled - continue to next one */

      /* free the handled buffer */
      if (status->bulkBuffer) {
        AMUDP_ReleaseBulkBuffer(ep, status->bulkBuffer);
        status->bulkBuffer = NULL;
      }
      #ifdef UETH
        { int retval = ueth_freerxbuf(buf);
          AMUDP_CHECK_ERRFR((retval != UETH_OK), RESOURCE, AMUDP_ServiceIncomingMessages, "ueth_freerxbuf failed");
        }
      #else
        AMUDP_assert(ep->rxReadyIdx < ep->rxNumBufs);
        AMUDP_assert(ep->rxFreeIdx < ep->rxNumBufs);
        AMUDP_assert(ep->rxReadyIdx != ep->rxFreeIdx);
        ep->rxReadyIdx = (ep->rxReadyIdx + 1) % ep->rxNumBufs; /* remove from queue and put back on free list */
      #endif

  }  /*  for */
  return AM_OK;
} /*  AMUDP_ServiceIncomingMessages */
/*------------------------------------------------------------------------------------
 * Poll
 *------------------------------------------------------------------------------------ */
extern int AM_Poll(eb_t eb) {
  int i;
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!eb),BAD_ARG);

  for (i = 0; i < eb->n_endpoints; i++) {
    int retval;
    ep_t ep = eb->endpoints[i];

    if (ep->depth != -1) { /* only poll endpoints which have buffers */

      #if USE_ASYNC_TCP_CONTROL
        if_pf (AMUDP_SPMDIsActiveControlSocket) /*  async check */
      #endif
      { retval = AMUDP_SPMDHandleControlTraffic(NULL);
        if (retval != AM_OK) AMUDP_RETURN(retval);
      }

      retval = AMUDP_ServiceIncomingMessages(ep); /* drain network and check for activity */
      if_pf (retval != AM_OK) AMUDP_RETURN(retval);

      if (ep->outstandingRequests > 0) {
        retval = AMUDP_HandleRequestTimeouts(ep, AMUDP_TIMEOUTS_CHECKED_EACH_POLL);
        if_pf (retval != AM_OK) AMUDP_RETURN(retval);
      }
    }
  }

  return AM_OK;
}
/*------------------------------------------------------------------------------------
 * Generic Request/Reply
 *------------------------------------------------------------------------------------ */
static int AMUDP_RequestGeneric(amudp_category_t category, 
                          ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr, 
                          uint8_t systemType, uint8_t systemArg) {
  static char _stagingbuf[sizeof(amudp_buf_t)+8]; /* for loopback */
  int instance;
  amudp_buf_t *basicbuf;
  amudp_buf_t *outgoingbuf;
  amudp_bufdesc_t *outgoingdesc;
  int const destP = request_endpoint->translation[reply_endpoint].id;
  en_t const destaddress = request_endpoint->translation[reply_endpoint].name;
  const int isloopback = enEqual(destaddress, request_endpoint->name);

  /*  always poll before sending a request */
  AM_Poll(request_endpoint->eb);

  if (isloopback) {
    outgoingbuf = (amudp_buf_t *)AMUDP_ALIGNUP(&_stagingbuf,8);
    basicbuf = outgoingbuf;
    basicbuf->status.bulkBuffer = NULL;
    outgoingdesc = NULL; /* not used */
    instance = 0; /* not used */
  } else { /*  acquire a free request buffer */
    int depth = request_endpoint->depth;
    int found = FALSE;
    while (!found) {
      int hint = request_endpoint->perProcInfo[destP].instanceHint;
      if_pt (!GET_REQ_DESC(request_endpoint, destP, hint)->inuse) { /*  hint is right */
        instance = hint;
        hint++;
        request_endpoint->perProcInfo[destP].instanceHint = (uint16_t)(hint==depth?0:hint);
        found = TRUE;
      } else { /*  hint is wrong */
        /*  search for a free instance */
        instance = hint;
        do {
          instance = ((instance+1)==depth?0:instance+1);
          if (!GET_REQ_DESC(request_endpoint, destP, instance)->inuse) {
            found = TRUE;
            break;
          }
        } while (instance != hint);
        if (!found) { 
          /*  no buffers available - wait until one is open 
           *  (hint will point to a free buffer) 
           */
          do {
            int retval;
            if (AMUDP_PoliteSync) {
              retval = AMUDP_Block(request_endpoint->eb);
              if (retval != AM_OK) AMUDP_RETURN(retval);
            }
            retval = AM_Poll(request_endpoint->eb);
            if (retval != AM_OK) AMUDP_RETURN(retval);
            hint = request_endpoint->perProcInfo[destP].instanceHint;
          } while (GET_REQ_DESC(request_endpoint, destP, hint)->inuse);
        }
      }
    }
    basicbuf = GET_REQ_BUF(request_endpoint, destP, instance);
    outgoingdesc = GET_REQ_DESC(request_endpoint, destP, instance);
    AMUDP_assert(!outgoingdesc->inuse);
    outgoingdesc->inuse = TRUE; /*  grab it now to claim as ours */
    request_endpoint->outstandingRequests++;

  #ifdef UETH
    if_pf (outgoingdesc->transmitCount > 1) {
      /*  this buffer was previously used for a retransmit, and therefore could still be waiting in the  */
      /*  outgoing send FIFO - check if this is the case and cancel that message if so */
      int cancelled = ueth_cancel_send(basicbuf, basicbuf->bufhandle);
      if (cancelled) { /*  pretend it never happenned  */
        AMUDP_STATS(request_endpoint->stats.RequestsRetransmitted[AMUDP_MSG_CATEGORY(&basicbuf->Msg)]--);
        AMUDP_STATS(request_endpoint->stats.RequestTotalBytesSent -= GET_PACKET_LENGTH(basicbuf));
        AMUDP_STATS(request_endpoint->stats.TotalBytesSent -= GET_PACKET_LENGTH(basicbuf));
      }
    }
  #endif

    if (nbytes > AMUDP_MAX_MEDIUM) {
      AMUDP_assert(category == amudp_Long && USE_TRUE_BULK_XFERS);
      outgoingbuf = AMUDP_AcquireBulkBuffer(request_endpoint);
      basicbuf->status.bulkBuffer = outgoingbuf;
    } else {
      basicbuf->status.bulkBuffer = NULL;
      outgoingbuf = basicbuf;
    }
  }

  /*  setup message meta-data */
  { amudp_msg_t *msg = &outgoingbuf->Msg;
    if (isloopback) AMUDP_MSG_SETFLAGS(msg, TRUE, category, numargs, 0, 0);
    else AMUDP_MSG_SETFLAGS(msg, TRUE, category, numargs, outgoingdesc->seqNum, instance);
    msg->destOffset = dest_offset;
    msg->handlerId = handler;
    msg->nBytes = (uint16_t)nbytes;
    msg->systemMessageType = systemType;
    msg->systemMessageArg = systemArg;
    msg->tag = request_endpoint->translation[reply_endpoint].tag;
  }

  { /*  setup args */
    int i;
    uint32_t *args = GET_PACKET_ARGS(outgoingbuf);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMUDP_MAX_SHORT) args[i] = 0;
    #endif
  }

  if (isloopback) { /* run handler synchronously */
    amudp_bufstatus_t* const status = &(outgoingbuf->status); /* the status block for this buffer */
    if (nbytes > 0) { /* setup data */
      if (category == amudp_Long) { /* one-copy */
        AMUDP_CHECK_ERR((dest_offset + nbytes > request_endpoint->segLength), BAD_ARG);
        memmove(((int8_t *)request_endpoint->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
      }
    }
    status->dest = request_endpoint;
    status->sourceId = reply_endpoint;
    status->sourceAddr = request_endpoint->name;

    AMUDP_processPacket(outgoingbuf, 1);
  } else { /* perform the send */
    int retval;
    int packetlength;

    /*  setup data */
    if (nbytes > 0) {
      memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
    }

    packetlength = GET_PACKET_LENGTH(outgoingbuf);
    retval = sendPacket(request_endpoint, outgoingbuf, packetlength, destaddress, REQUESTREPLY_PACKET);
    if_pf (retval != AM_OK) {
      outgoingdesc->inuse = FALSE; /*  send failed, so message rejected - release buffer */
      request_endpoint->outstandingRequests--;
      if (basicbuf->status.bulkBuffer) {
        AMUDP_ReleaseBulkBuffer(request_endpoint, basicbuf->status.bulkBuffer);
        basicbuf->status.bulkBuffer = NULL;
      }
      request_endpoint->perProcInfo[reply_endpoint].instanceHint = (uint16_t)instance;
      AMUDP_RETURN(retval);
    }

    /* outgoingdesc->seqNum = !(outgoingdesc->seqNum); */ /* this gets handled by AMUDP_ServiceIncomingMessages */
    { amudp_cputick_t now = getCPUTicks();
      uint32_t ustimeout = AMUDP_InitialRequestTimeout_us;
      /* we carefully use 32-bit datatypes here to avoid 64-bit multiply/divide */
      static uint32_t expectedusperbyte = 0; /* cache precomputed value */
      static amudp_cputick_t ticksperus = 0;
      static int firsttime = 1;
      if_pf (firsttime) {
        ticksperus = us2ticks(1);
        expectedusperbyte = /* allow 2x of slop for reply */
          (uint32_t)((2 * 1000000.0 / 1024.0) / AMUDP_ExpectedBandwidth);
        firsttime = 0;
      }
     if (AMUDP_InitialRequestTimeout_us == AMUDP_TIMEOUT_INFINITE) {
       outgoingdesc->timestamp = (amudp_cputick_t)-1;
       outgoingdesc->retryCount = 0;
     } else {
      uint32_t expectedus = (packetlength * expectedusperbyte);
      /* bulk transfers may have a noticeable wire delay, so we grow the initial timeout
       * accordingly to allow time for the transfer to arrive and be serviced
       * These are the transfers that are really expensive to retransmit, 
       * so we want to avoid that until we're relatively certain they've really been lost
       */
      int retryCount = 0;
      outgoingdesc->transmitCount = 1;
      while (ustimeout < expectedus && ustimeout < AMUDP_MaxRequestTimeout_us) {
        ustimeout *= AMUDP_RequestTimeoutBackoff;
        retryCount++;
      }
      outgoingdesc->timestamp = now + (((amudp_cputick_t)ustimeout)*ticksperus);
      outgoingdesc->retryCount = retryCount;
     }
      #if AMUDP_COLLECT_LATENCY_STATS
        outgoingdesc->firstSendTime = now;
      #endif
    }
    AMUDP_STATS(request_endpoint->stats.RequestsSent[category]++);
    AMUDP_STATS(request_endpoint->stats.RequestDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMUDP_STATS(request_endpoint->stats.RequestTotalBytesSent[category] += packetlength);
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMUDP_ReplyGeneric(amudp_category_t category, 
                          amudp_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg) {
  static char _stagingbuf[sizeof(amudp_buf_t)+8]; /* for loopback */
  amudp_buf_t *basicbuf;
  amudp_buf_t *outgoingbuf;
  amudp_bufdesc_t *outgoingdesc;
  amudp_buf_t * const requestbasicbuf = requestbuf;
  ep_t const ep = requestbasicbuf->status.dest;
  int const destP = requestbasicbuf->status.sourceId;
  const int isloopback = enEqual(requestbuf->status.sourceAddr, ep->name);
  int instance;
  requestbuf = (requestbasicbuf->status.bulkBuffer ? requestbasicbuf->status.bulkBuffer : requestbasicbuf);

  /*  we don't poll within a reply because by definition we are already polling somewhere in the call chain */

  if (isloopback) {
    basicbuf = (amudp_buf_t *)AMUDP_ALIGNUP(&_stagingbuf,8);
    outgoingbuf = basicbuf;
    basicbuf->status.bulkBuffer = NULL;
    outgoingdesc = NULL; /* not used */
    instance = 0; /* not used */
  } else {
    /*  acquire a free buffer  */
    /*  trivial because replies can safely overwrite previous reply in request instance */
    instance = AMUDP_MSG_INSTANCE(&(requestbuf->Msg)); 
    basicbuf = GET_REP_BUF(ep, destP, instance);
    outgoingdesc = GET_REP_DESC(ep, destP, instance);

    #ifdef UETH
      if_pf (outgoingdesc->transmitCount > 1) {
        /*  this buffer was previously used for a retransmit, and therefore could still be waiting in the  */
        /*  outgoing send FIFO - check if this is the case and cancel that message if so */
        int cancelled = ueth_cancel_send(basicbuf, basicbuf->bufhandle);
        if (cancelled) { /*  pretend it never happenned  */
          AMUDP_STATS(ep->stats.RepliesRetransmitted[AMUDP_MSG_CATEGORY(&basicbuf->Msg)]--);
          AMUDP_STATS(request_endpoint->stats.ReplyTotalBytesSent -= GET_PACKET_LENGTH(basicbuf));
          AMUDP_STATS(ep->stats.TotalBytesSent -= GET_PACKET_LENGTH(basicbuf));
        }
      }
    #endif

    if (basicbuf->status.bulkBuffer) { /* free bulk buffer of previous reply */
      AMUDP_ReleaseBulkBuffer(ep, basicbuf->status.bulkBuffer);
      basicbuf->status.bulkBuffer = NULL;
    }

    if (nbytes > AMUDP_MAX_MEDIUM) {
      AMUDP_assert(category == amudp_Long && USE_TRUE_BULK_XFERS);
      outgoingbuf = AMUDP_AcquireBulkBuffer(ep);
      basicbuf->status.bulkBuffer = outgoingbuf;
    } else {
      basicbuf->status.bulkBuffer = NULL;
      outgoingbuf = basicbuf;
    }
  }

  /*  setup message meta-data */
  { amudp_msg_t *msg = &outgoingbuf->Msg;
    if (isloopback) AMUDP_MSG_SETFLAGS(msg, FALSE, category, numargs, 0, 0);
    else AMUDP_MSG_SETFLAGS(msg, FALSE, category, numargs, outgoingdesc->seqNum, instance);
    msg->destOffset = dest_offset;
    msg->handlerId = handler;
    msg->nBytes = (uint16_t)nbytes;
    msg->systemMessageType = systemType;
    msg->systemMessageArg = systemArg;
    msg->tag = ep->perProcInfo[destP].tag;
  }

  { /*  setup args */
    int i;
    uint32_t *args = GET_PACKET_ARGS(outgoingbuf);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMUDP_MAX_SHORT) args[i] = 0;
    #endif
  }

  if (isloopback) { /* run handler synchronously */
    amudp_bufstatus_t* const status = &(outgoingbuf->status); /* the status block for this buffer */
    if (nbytes > 0) { /* setup data */
      if (category == amudp_Long) { /* one-copy */
        AMUDP_CHECK_ERR((dest_offset + nbytes > ep->segLength), BAD_ARG);
        memmove(((int8_t *)ep->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
      }
    }
    status->dest = ep;
    status->sourceId = destP;
    status->sourceAddr = ep->name;

    AMUDP_processPacket(outgoingbuf, 1);
  } else { /* perform the send */
    int packetlength = GET_PACKET_LENGTH(outgoingbuf);
    en_t destaddress = ep->perProcInfo[destP].remoteName;
    int retval;
    /*  setup data */
    memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
    #if 0 /* not necessary- we never send this stuff */
      #if USE_CLEAR_UNUSED_SPACE
        memset(&(GET_PACKET_DATA(outgoingbuf)[nbytes]), 0, AMUDP_MAX_LONG - nbytes);
      #endif
    #endif
    retval = sendPacket(ep, outgoingbuf, packetlength, destaddress, REQUESTREPLY_PACKET);
    if_pf (retval != AM_OK) AMUDP_RETURN(retval);
    /* outgoingdesc->seqNum = !(outgoingdesc->seqNum); */ /* this gets handled by AMUDP_ServiceIncomingMessages */
    outgoingdesc->transmitCount = 1;
    AMUDP_STATS(ep->stats.RepliesSent[category]++);
    AMUDP_STATS(ep->stats.ReplyDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMUDP_STATS(ep->stats.ReplyTotalBytesSent[category] += packetlength);
  }

  requestbasicbuf->status.replyIssued = TRUE;
  return AM_OK;
}

/*------------------------------------------------------------------------------------
 * Request
 *------------------------------------------------------------------------------------ */
extern int AMUDP_RequestVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!request_endpoint || reply_endpoint < 0),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMUDP_CHECK_ERR((reply_endpoint >= AMUDP_MAX_NUMTRANSLATIONS ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  return AMUDP_RequestGeneric(amudp_Short, 
                                  request_endpoint, reply_endpoint, handler, 
                                  NULL, 0, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);

}
extern int AMUDP_Request(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
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
extern int AMUDP_RequestIVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!request_endpoint || reply_endpoint < 0),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMUDP_CHECK_ERR((reply_endpoint >= AMUDP_MAX_NUMTRANSLATIONS ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMUDP_CHECK_ERR((!source_addr),BAD_ARG);
  AMUDP_CHECK_ERR((nbytes < 0 || nbytes > AMUDP_MAX_MEDIUM),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  return AMUDP_RequestGeneric(amudp_Medium, 
                                  request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_RequestI(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes,
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
static int getFreeBulkSlot(ep_t ep, int destP, uint8_t *slotnum, int allowblock) { 
  /* find a free bulk slot number, possibly blocking if allowblock=TRUE
   * returns AM_OK (with slot set), AM_ERR_XXX on error, or -1 for timeout if allowblock=FALSE
   */
  AMUDP_assert(ep && slotnum);
  AMUDP_assert(destP >= 0 && destP < ep->P);
  while(1) {
    uint8_t slot;
    for (slot = 0; slot < 16; slot++) {
      int inst;
      for (inst = 0; inst < ep->depth; inst++) {
        if (GET_REQ_DESC(ep, destP, inst)->inuse) { 
          amudp_system_messagetype_t systype =
            (amudp_system_messagetype_t)(GET_REQ_BUF(ep, destP, inst)->Msg.systemMessageType & 0xF);
          uint8_t thisslot = (uint8_t)((GET_REQ_BUF(ep, destP, inst)->Msg.systemMessageType >> 4) & 0xF);
          if (systype == amudp_system_bulkxferfragment && thisslot == slot) break; /*  already taken */
        }
      }
      if (inst == ep->depth) {
        *slotnum = slot;
        return AM_OK;
      }
    }

    /*  wait for some slots to become free */
    if (allowblock) { 
      int retval;
      retval = AMUDP_Block(ep->eb);
      if (retval != AM_OK) AMUDP_RETURN(retval);
      retval = AM_Poll(ep->eb);
      if (retval != AM_OK) AMUDP_RETURN(retval);
      }
    else return -1; /*  timed out - non-blocking */
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AMUDP_RequestXferVA(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int async, 
                          int numargs, va_list argptr) {
  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!request_endpoint || reply_endpoint < 0),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMUDP_CHECK_ERR((reply_endpoint >= AMUDP_MAX_NUMTRANSLATIONS ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMUDP_CHECK_ERR((!source_addr),BAD_ARG);
  AMUDP_CHECK_ERR((nbytes < 0 || nbytes > AMUDP_MAX_LONG),BAD_ARG);
  AMUDP_CHECK_ERR((dest_offset > AMUDP_MAX_SEGLENGTH),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  int destP = request_endpoint->translation[reply_endpoint].id;
  const int isloopback = enEqual(request_endpoint->translation[reply_endpoint].name, request_endpoint->name);

  #if USE_TRUE_BULK_XFERS
    if (async && !isloopback) { /*  decide if we can satisfy request without blocking */
      int i;
      /* it's unclear from the spec whether we should poll before an async failure,
       * but by definition the app must be prepared for handlers to run when calling this 
       * request, so it shouldn't cause anything to break, and the async request is more likely
       * to succeed if we do. so:
       */
      AM_Poll(request_endpoint->eb);

      /* see if there's a free buffer */
      for (i = 0; i < request_endpoint->depth; i++) 
        if (!request_endpoint->requestDesc[destP].inuse) break;
      if (i == request_endpoint->depth)         
        AMUDP_RETURN_ERRFR(IN_USE, AMUDP_RequestXferAsync, 
          "Request can't be satisfied without blocking right now");
    }

    /* perform the send */
    return AMUDP_RequestGeneric(amudp_Long, 
                                  request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr,
                                  amudp_system_user, 0);
  #else
  { /* segmented bulk transfers - break large xfers into chunks */
    int numchunks = ((nbytes-1) / AMUDP_MAX_MEDIUM)+1; /*  number of chunks required for data */
    if (async && !isloopback) { /*  decide if we can satisfy request without blocking */
      int i, freecnt=0;

      if (numchunks > request_endpoint->depth) 
        AMUDP_RETURN_ERRFR(RESOURCE, AMUDP_RequestXferAsync, 
          "Request too large to EVER be satisfied without blocking");

      /* it's unclear from the spec whether we should poll before an async failure,
       * but by definition the app must be prepared for handlers to run when calling this 
       * request, so it shouldn't cause anything to break, and the async request is more likely
       * to succeed if we do. so:
       */
      AM_Poll(request_endpoint->eb);

      /*  see how many buffers are free */
      #if 1
        freecnt = request_endpoint->depth - request_endpoint->outstandingRequests;
      #else
        for (i = 0; i < request_endpoint->depth; i++) {
          if (!request_endpoint->requestDesc[destP].inuse) freecnt++;
          }
        AMUDP_assert(freecnt == request_endpoint->depth - request_endpoint->outstandingRequests);
      #endif
      if (numchunks > freecnt) 
        AMUDP_RETURN_ERRFR(IN_USE, AMUDP_RequestXferAsync, 
          "Request too large to be satisfied without blocking right now");
      { uint8_t slotnum; /*  see if we have a free slot */
        int retval = getFreeBulkSlot(request_endpoint, destP, &slotnum, FALSE);
        if (retval == -1)
          AMUDP_RETURN_ERRFR(IN_USE, AMUDP_RequestXferAsync, 
            "Request can't be satisfied without blocking right now - out of bulk xfer slots");
      }
    }
    if (numchunks == 1 || isloopback) { /*  single-message bulk xfer, just a user message */
      /*  call the generic requestor */
      return AMUDP_RequestGeneric(amudp_Long, 
                                    request_endpoint, reply_endpoint, handler, 
                                    source_addr, nbytes, dest_offset,
                                    numargs, argptr,
                                    amudp_system_user, 0);
    } else { 
      int fragidx;
      char *chunk_source_addr = (char*)source_addr;
      uintptr_t chunk_dest_offset = dest_offset;
      uint8_t slotnum;

      /*  get a slot (possibly poll-blocking) */
      uint8_t systype;
      int retval = getFreeBulkSlot(request_endpoint, destP, &slotnum, TRUE);
      if (retval != AM_OK) AMUDP_RETURN(retval);
      AMUDP_assert(slotnum < 16);
      systype = (slotnum << 4) | ((uint8_t)amudp_system_bulkxferfragment);

      for (fragidx = 0; fragidx < numchunks; fragidx++) {
        int retval;
        int tmpnumargs = 0;
        uint16_t chunk_nbytes = fragidx < numchunks - 1 ? 
                                AMUDP_MAX_MEDIUM :
                                ( nbytes % AMUDP_MAX_MEDIUM == 0 ? 
                                  AMUDP_MAX_MEDIUM : 
                                  nbytes % AMUDP_MAX_MEDIUM );
        if (fragidx == numchunks - 1) /* send args with last fragment */
          tmpnumargs = numargs;
        /* could use va_copy here, but not really necessary - 
           argptr will only be used at most once, when tmpnumargs > 0
         */
        retval = AMUDP_RequestGeneric(amudp_Long, 
                                      request_endpoint, reply_endpoint, handler, 
                                      chunk_source_addr, chunk_nbytes, chunk_dest_offset,
                                      tmpnumargs, argptr,
                                      systype, (uint8_t)(numchunks-1));
        if_pf (retval != AM_OK) /*  recovery here would suck, so errors here are fatal */
          AMUDP_FatalErr("Network failure in the middle of a bulk transfer");
        chunk_source_addr += chunk_nbytes;
        chunk_dest_offset += chunk_nbytes;
      }
      return AM_OK;
    }
  }
  #endif
}
extern int AMUDP_RequestXfer(ep_t request_endpoint, int reply_endpoint, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
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
  amudp_buf_t *basicbuf;
  amudp_buf_t *requestbuf;

  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    basicbuf = (amudp_buf_t *)token;
    requestbuf = (basicbuf->status.bulkBuffer ? basicbuf->status.bulkBuffer : basicbuf);
    AMUDP_CHECK_ERR((!AMUDP_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMUDP_CHECK_ERR((!basicbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMUDP_CHECK_ERR((basicbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMUDP_CHECK_ERR((((amudp_system_messagetype_t)requestbuf->Msg.systemMessageType) != amudp_system_user),
                    RESOURCE); /* can't reply to a system message (returned message) */
  }

  return AMUDP_ReplyGeneric(amudp_Short, 
                                  basicbuf, handler, 
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
                          void *source_addr, int nbytes,
                          int numargs, va_list argptr) {
  amudp_buf_t *basicbuf;
  amudp_buf_t *requestbuf;

  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_CHECK_ERR((!source_addr),BAD_ARG);
  AMUDP_CHECK_ERR((nbytes < 0 || nbytes > AMUDP_MAX_MEDIUM),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    basicbuf = (amudp_buf_t *)token;
    requestbuf = (basicbuf->status.bulkBuffer ? basicbuf->status.bulkBuffer : basicbuf);
    AMUDP_CHECK_ERR((!AMUDP_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMUDP_CHECK_ERR((!basicbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMUDP_CHECK_ERR((basicbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMUDP_CHECK_ERR((((amudp_system_messagetype_t)requestbuf->Msg.systemMessageType) != amudp_system_user),
                    RESOURCE); /* can't reply to a system message (returned message) */
  }

  return AMUDP_ReplyGeneric(amudp_Medium, 
                                  basicbuf, handler, 
                                  source_addr, nbytes, 0,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_ReplyI(void *token, handler_t handler, 
                          void *source_addr, int nbytes,
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
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr) {
  amudp_buf_t *basicbuf;
  amudp_buf_t *requestbuf;

  AMUDP_CHECKINIT();
  AMUDP_CHECK_ERR((!token),BAD_ARG);
  AMUDP_CHECK_ERR((AMUDP_BADHANDLERVAL(handler)),BAD_ARG);
  AMUDP_CHECK_ERR((!source_addr),BAD_ARG);
  AMUDP_CHECK_ERR((nbytes < 0 || nbytes > AMUDP_MAX_LONG),BAD_ARG);
  AMUDP_CHECK_ERR((dest_offset > AMUDP_MAX_SEGLENGTH),BAD_ARG);
  AMUDP_assert(numargs >= 0 && numargs <= AMUDP_MAX_SHORT);

  AMUDP_CHECK_ERRFR((nbytes > AMUDP_MAX_MEDIUM && !USE_TRUE_BULK_XFERS), 
    RESOURCE, AMUDP_ReplyXfer, 
      "ReplyXfer() exceeding AM_Medium()=" _STRINGIFY(AMUDP_MAX_MEDIUM) " bytes not implemented.");

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    basicbuf = (amudp_buf_t *)token;
    requestbuf = (basicbuf->status.bulkBuffer ? basicbuf->status.bulkBuffer : basicbuf);
    AMUDP_CHECK_ERR((!AMUDP_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMUDP_CHECK_ERR((!basicbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMUDP_CHECK_ERR((basicbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMUDP_CHECK_ERR((((amudp_system_messagetype_t)requestbuf->Msg.systemMessageType) != amudp_system_user),
      RESOURCE); /* can't reply to a system message (returned message) */
  }

  return AMUDP_ReplyGeneric(amudp_Long, 
                                  basicbuf, handler, 
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr,
                                  amudp_system_user, 0);
}
extern int AMUDP_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, int nbytes, uintptr_t dest_offset, 
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
  amudp_buf_t *msgbasicbuf = (amudp_buf_t *)token;
  amudp_buf_t *msgbuf = (msgbasicbuf->status.bulkBuffer ? msgbasicbuf->status.bulkBuffer : msgbasicbuf);
  int numArgs = AMUDP_MSG_NUMARGS(&msgbuf->Msg);
  uint32_t *args = GET_PACKET_ARGS(msgbuf);
  char argStr[255];
  int i;

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
  for (i=0; i < numArgs; i++) {
    char tmp[20];
    sprintf(tmp, "0x%08x  ", (int)args[i]);
    strcat(argStr, tmp);
  }
  { char temp1[80];
    char temp2[80];
    AMUDP_FatalErr("An active message was returned to sender,\n"
             "    and trapped by the default returned message handler (handler 0):\n"
             "Error Code: %s\n"
             "Message type: %s\n"
             "Destination: %s (%i)\n"
             "Handler: %i\n"
             "Tag: %s\n"
             "Arguments(%i): %s\n"
             "Aborting...",
             statusStr, opcodeStr, 
             AMUDP_enStr(msgbasicbuf->status.sourceAddr, temp1), msgbasicbuf->status.sourceId,
             msgbuf->Msg.handlerId, AMUDP_tagStr(msgbuf->Msg.tag, temp2),
             numArgs, argStr);
  }
}
/* ------------------------------------------------------------------------------------ */
