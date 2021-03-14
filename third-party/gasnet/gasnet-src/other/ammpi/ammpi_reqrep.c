/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/ammpi/ammpi_reqrep.c $
 * Description: AMMPI Implementations of request/reply operations
 * Copyright 2000, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#if _FORTIFY_SOURCE > 0 && __OPTIMIZE__ <= 0 /* silence an annoying MPICH/Linux warning */
#undef _FORTIFY_SOURCE
#endif

#include <math.h>
#include <fcntl.h>

#include "ammpi_internal.h" /* must come after any other headers */

/* forward decls */
static int AMMPI_RequestGeneric(ammpi_category_t category, 
                          ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);
static int AMMPI_ReplyGeneric(ammpi_category_t category, 
                          ammpi_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg);
/* ------------------------------------------------------------------------------------ */
/* mpihandle points to the MPI_Request to receive the non-blocking send handle, 
 * or null to use a blocking send
 */
extern int AMMPI_syncsend_thresh;
static int sendPacket(ep_t ep, ammpi_virtual_network_t *activeNet, void *packet, int packetlength, 
                      en_t destaddress, MPI_Request *mpihandle) {
  AMX_assert(ep && activeNet && packet && packetlength > 0);
  AMX_assert(packetlength <= AMMPI_MAX_NETWORK_MSG);

  AMX_VERBOSE_INFO(("sending %i byte packet to (%s)", packetlength, AMMPI_enStr(destaddress, 0)));

  #if AMMPI_NONBLOCKING_SENDS
    if_pt (mpihandle && *mpihandle == MPI_REQUEST_NULL) {
      if (packetlength >= AMMPI_syncsend_thresh) {
        /* synchronous mode non-blocking send - for MPI implementations lacking 
           a reasonable implementation of back-pressure. This doesn't guarantee we dont
           get unexpected messages if the target is inattentive, but it at least 
           limits the max number of messages in the unexpected message queue which
           exceed the syncsend threshold - limit is one depth of such messages
           (and an unlimited number of messages smaller than the threshold)
         */
        MPI_SAFE(MPI_Issend(packet, packetlength, MPI_BYTE, destaddress.mpirank, destaddress.mpitag, *activeNet->mpicomm, mpihandle));
      } else {
        MPI_SAFE(MPI_Isend(packet, packetlength, MPI_BYTE, destaddress.mpirank, destaddress.mpitag, *activeNet->mpicomm, mpihandle));
      }
    } else
  #endif
    {
      MPI_SAFE(MPI_Bsend(packet, packetlength, MPI_BYTE, destaddress.mpirank, destaddress.mpitag, *activeNet->mpicomm));
    }

  AMMPI_STATS(ep->stats.TotalBytesSent += packetlength);

  if_pt (mpihandle) { 
    #if AMMPI_RECV_REPOST_SLACK && AMMPI_PREPOST_RECVS
    { /* use the send delay slot to catch up on deferred recv buffer reposting work */ 
      /* check the opposite net, because a reply send means we just got a request,
         and a request send means we're likely to have recently received a reply */
      ammpi_virtual_network_t * const altNet = ( (activeNet == &(ep->Req)) ? &(ep->Rep) : &(ep->Req) );
      while (altNet->rxPostSlack > 0) {
        int altidx = altNet->rxCurr - altNet->rxPostSlack;
        if (altidx < 0) altidx += altNet->rxNumBufs;
        AMX_assert(altidx >= 0 && altidx < altNet->rxNumBufs && altidx != altNet->rxCurr);
        if (AMMPI_PostRecvBuffer(&altNet->rxBuf[altidx],
                                 &altNet->rxHandle[altidx],
                                 altNet->mpicomm)) AMX_RETURN_ERR(RESOURCE); 
        altNet->rxPostSlack--;
      }
    }
    #endif
    #if AMMPI_SEND_EARLYCOMPLETE && AMMPI_NONBLOCKING_SENDS
    { /* use the send delay slot to catch up on send completion work */ 
      ammpi_sendbuffer_pool_t * const pool = 
        ( (packetlength <= AMMPI_SMALL_SENDBUF_SZ) ? 
          &activeNet->sendPool_small : &activeNet->sendPool_large );
      if (pool->numActive >= AMMPI_SEND_EARLYCOMPLETE) {
        int retval = AMMPI_ReapSendCompletions(pool);
        if_pf (retval != AM_OK) AMX_RETURN(retval);
      }
    }
    #endif
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_GetOpcode(int isrequest, ammpi_category_t cat) {
  switch (cat) {
    case ammpi_Short:
      if (isrequest) return AM_REQUEST_M;
      else return AM_REPLY_M;
    case ammpi_Medium:
      if (isrequest) return AM_REQUEST_IM;
      else return AM_REPLY_IM;
    case ammpi_Long:
      if (isrequest) return AM_REQUEST_XFER_M;
      else return AM_REPLY_XFER_M; 
    default: AMX_FatalErr("unrecognized opcode in AMMPI_GetOpcode");
      return -1;
  }
}
/* ------------------------------------------------------------------------------------ */
static int sourceAddrToId(ep_t ep, en_t sourceAddr) {
  /*  return source id in ep perproc table of this remote addr, or -1 for not found */
  ammpi_node_t i; 
  
  /* try the common case where mapping ids match MPI rank */
  if (sourceAddr.mpirank < ep->totalP &&
      AMMPI_enEqual(ep->perProcInfo[sourceAddr.mpirank].remoteName, sourceAddr)) 
    return sourceAddr.mpirank;

  /* failed - use linear search */
  for (i = 0; i < ep->totalP; i++) {
    if (AMMPI_enEqual(ep->perProcInfo[i].remoteName, sourceAddr))
      return i;
  }
  return -1;
}
/* ------------------------------------------------------------------------------------ */
/* accessors for packet args, data and length
 * the only complication here is we want data to be double-word aligned, so we may add
 * an extra unused 4-byte argument to make sure the data lands on a double-word boundary
 * TODO: remove padding arg for shorts and longs, where it's irrelevant
 */
#if 0
#define HEADER_EVEN_WORDLENGTH  (((int)(uintptr_t)((&((ammpi_buf_t *)NULL)->_Data)-1))%8==0?1:0)
#else
#define HEADER_EVEN_WORDLENGTH \
    ( ( (((uint8_t *)&(((ammpi_buf_t *)NULL)->_Data)) - ((uint8_t *)NULL)) & 0x7) == 0 ? 1 : 0)
#endif
#define ACTUAL_NUM_ARGS(pMsg) (AMMPI_MSG_NUMARGS(pMsg)%2==0?       \
                            AMMPI_MSG_NUMARGS(pMsg)+!HEADER_EVEN_WORDLENGTH:  \
                            AMMPI_MSG_NUMARGS(pMsg)+HEADER_EVEN_WORDLENGTH)

#define GET_PACKET_LENGTH(pbuf)                                       \
  (((char *)&pbuf->_Data[4*ACTUAL_NUM_ARGS(&pbuf->Msg) + pbuf->Msg.nBytes]) - ((char *)pbuf))
#define PREDICT_PACKET_LENGTH_(nArgs,nBytes)  /* conservative estimate of packet size */  \
  ((int)(uintptr_t)(char *)&(((ammpi_buf_t*)NULL)->_Data[4*(nArgs+1) + nBytes]))
#if AMX_DEBUG
  /* If we conservatively over-estimate too near the MAX, then we get an assertion failure */
  #define PREDICT_PACKET_LENGTH(nArgs,nBytes) \
      MIN(PREDICT_PACKET_LENGTH_(nArgs,nBytes),AMMPI_MAX_NETWORK_MSG)
#else
  #define PREDICT_PACKET_LENGTH(nArgs,nBytes) PREDICT_PACKET_LENGTH_(nArgs,nBytes)
#endif
#define GET_PACKET_DATA(pbuf)                                         \
  (&pbuf->_Data[4*ACTUAL_NUM_ARGS(&pbuf->Msg)])
#define GET_PACKET_ARGS(pbuf)                                         \
  ((uint32_t *)pbuf->_Data)

#if AMMPI_VERIFY_MPI_ORDERING  /* debugging aid for MPI implementations */
  #define AMMPI_VERIFY_MPI_SETSEQNUM(msg, isReq, ep, remoteid) ((msg).seqnum = ++((ep)->perProcInfo[remoteid]).seqnum[isReq].out)
#else
  #define AMMPI_VERIFY_MPI_SETSEQNUM(msg, isReq, ep, remoteid) ((void)0)
#endif

/* ------------------------------------------------------------------------------------ */
#if AMX_DEBUG
  #define REFUSE_NOTICE(reason) AMX_Err("I just refused a message and returned to sender. Reason: %s", reason)
#else
  #define REFUSE_NOTICE(reason) (void)0
#endif

/* this is a local-use-only macro for AMMPI_processPacket */
#define AMMPI_REFUSEMESSAGE(ep, buf, errcode) do {                            \
    int retval;                                                               \
    buf->Msg.systemMessageType = (uint8_t)ammpi_system_returnedmessage;       \
    buf->Msg.systemMessageArg = (uint8_t)errcode;                             \
    retval = sendPacket(ep, &ep->Rep, buf, GET_PACKET_LENGTH(buf),            \
                        (buf)->status.sourceAddr, NULL);                      \
    if (retval != AM_OK) AMX_Err("failed to sendPacket to refuse message"); \
    else REFUSE_NOTICE(#errcode);                                             \
    return;                                                                   \
  } while(0)

void AMMPI_processPacket(ammpi_buf_t *buf, int isloopback) {
  ep_t const ep = buf->status.dest;
  ammpi_msg_t * const msg = &buf->Msg;
  ammpi_bufstatus_t * const status = &buf->status; 
  int const numargs = AMMPI_MSG_NUMARGS(msg);
  int const isrequest = AMMPI_MSG_ISREQUEST(msg);
  ammpi_category_t const cat = AMMPI_MSG_CATEGORY(msg);
  ammpi_node_t const sourceId = status->sourceId;
  int const issystemmsg = ((ammpi_system_messagetype_t)msg->systemMessageType) != ammpi_system_user;

  /* handle returned messages */
  if_pf (issystemmsg) { 
    ammpi_system_messagetype_t type = ((ammpi_system_messagetype_t)msg->systemMessageType);
    if (type == ammpi_system_returnedmessage) { 
      amx_returned_handler_fn_t handlerfn = (amx_returned_handler_fn_t)ep->handler[0];
      op_t opcode;
      if (sourceId == (ammpi_node_t)-1) return; /*  unknown source, ignore message */
      opcode = AMMPI_GetOpcode(isrequest, cat);

      /* note that source/dest for returned mesgs reflect the virtual "message denied" packet 
       * although it doesn't really matter because the AM2 spec is too vague
       * about the argblock returned message argument for it to be of any use to anyone
       */
      status->replyIssued = TRUE; /* prevent any reply */
      status->handlerRunning = TRUE;
      AMX_assert(handlerfn);
      (*handlerfn)(msg->systemMessageArg, opcode, (void *)buf);
      status->handlerRunning = FALSE;
      AMMPI_STATS(ep->stats.ReturnedMessages++);
      return;
    }
  }

  if (!isloopback) {
    if (isrequest) AMMPI_STATS(ep->stats.RequestsReceived[cat]++);
    else AMMPI_STATS(ep->stats.RepliesReceived[cat]++);
  }

  if_pf (sourceId == (ammpi_node_t)-1) AMMPI_REFUSEMESSAGE(ep, buf, EBADENDPOINT);

  #if AMMPI_VERIFY_MPI_ORDERING  /* debugging aid for MPI implementations */
    if (!isloopback) { 
      uint64_t seqnum = msg->seqnum;
      uint64_t seqnum_exp = ++(ep->perProcInfo[sourceId].seqnum[isrequest].in);
      if_pf (seqnum != seqnum_exp)
        AMX_FatalErr("MPI message ordering violation detected on %s arrival: \n"
                       "  myMPIrank=%i sourceId=%i remoteMPIrank=%i\n"
                       "  message seqnum: %"PRIu64", expected: %"PRIu64,
                       (isrequest?"request":"reply"),
                        (int)ep->name.mpirank, (int)sourceId,
                        (int)ep->perProcInfo[sourceId].remoteName.mpirank,
                        seqnum, seqnum_exp);
    }
  #endif

#if AMMPI_USE_AMTAGS
  if_pf (ep->tag == AM_NONE || 
     (ep->tag != msg->tag && ep->tag != AM_ALL))
      AMMPI_REFUSEMESSAGE(ep, buf, EBADTAG);
#endif
  if_pf (ep->handler[msg->handlerId] == amx_unused_handler &&
      !issystemmsg && msg->handlerId != 0)
      AMMPI_REFUSEMESSAGE(ep, buf, EBADHANDLER);

  switch (cat) {
    case ammpi_Short:
      if_pf (msg->nBytes > 0 || msg->destOffset > 0)
        AMMPI_REFUSEMESSAGE(ep, buf, EBADLENGTH);
      break;
    case ammpi_Medium:
      if_pf (msg->nBytes > AMMPI_MAX_MEDIUM || msg->destOffset > 0)
        AMMPI_REFUSEMESSAGE(ep, buf, EBADLENGTH);
      break;
    case ammpi_Long: 
      /* check segment limits */
      if_pf (((uintptr_t)ep->segAddr + msg->destOffset) == 0 || ep->segLength == 0)
        AMMPI_REFUSEMESSAGE(ep, buf, EBADSEGOFF);
      if_pf (msg->destOffset + msg->nBytes > ep->segLength || msg->nBytes > AMMPI_MAX_LONG)
        AMMPI_REFUSEMESSAGE(ep, buf, EBADLENGTH);
      break;
    default: AMX_unreachable();
  }


  /* --- message accepted --- */
  #if AMMPI_COLLECT_LATENCY_STATS && AMMPI_COLLECT_STATS
    if (!isrequest && !isloopback) { 
      /* gather some latency statistics */
      uint64_t now = AMX_getMicrosecondTimeStamp();
      uint64_t latency = (now - desc->firstSendTime);
      ep->stats.RequestSumLatency += latency;
      if (latency < ep->stats.RequestMinLatency) ep->stats.RequestMinLatency = latency;
      if (latency > ep->stats.RequestMaxLatency) ep->stats.RequestMaxLatency = latency;
    }
  #endif

  /*  run the handler */
  status->replyIssued = FALSE;
  status->handlerRunning = TRUE;
  uint32_t * const pargs = GET_PACKET_ARGS(buf);
  if_pf (issystemmsg) { /* an AMMPI system message */
    ammpi_system_messagetype_t type = ((ammpi_system_messagetype_t)(msg->systemMessageType & 0xF));
    switch (type) {
      case ammpi_system_autoreply:
        /*  do nothing, already taken care of */
        #if AMMPI_FLOW_CONTROL
          AMX_assert(!isloopback);
          ep->perProcInfo[sourceId].tokens_out += msg->systemMessageArg; /* returned tokens */
          AMX_assert(ep->perProcInfo[sourceId].tokens_out <= ep->tokens_perhost);
        #endif
        break;
      case ammpi_system_controlmessage:
        /*  run a control handler */
        if (ep->controlMessageHandler == NULL || ep->controlMessageHandler == amx_unused_handler)
          AMX_Err("got an AMMPI control message, but no controlMessageHandler is registered. Ignoring...");
        else {
          AMX_RUN_HANDLER_SHORT(ep->controlMessageHandler, buf, pargs, numargs);
        }
        break;
      default: AMX_unreachable();
    }
  } else { /* a user message */
    #if AMMPI_FLOW_CONTROL
      if (!isloopback) {
        if (isrequest) ep->perProcInfo[sourceId].tokens_in++; /* coalesce tokens */
        AMX_assert(ep->perProcInfo[sourceId].tokens_in <= ep->tokens_perhost);
        ep->perProcInfo[sourceId].tokens_out += msg->systemMessageArg; /* returned tokens */
        AMX_assert(ep->perProcInfo[sourceId].tokens_out <= ep->tokens_perhost);
      }
    #endif

    handler_t const hid = msg->handlerId;
    switch (cat) {
      case ammpi_Short: 
        if (ep->preHandlerCallback) 
          ep->preHandlerCallback(ammpi_Short, isrequest, hid, buf, 
                                 NULL, 0, numargs, pargs);
        AMX_RUN_HANDLER_SHORT(ep->handler[hid], buf, pargs, numargs);
        if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
        break;
      case ammpi_Medium: {
        uint8_t * const pData = GET_PACKET_DATA(buf);
        if (ep->preHandlerCallback) 
          ep->preHandlerCallback(ammpi_Medium, isrequest, hid, buf, 
                                 pData, msg->nBytes, numargs, pargs);
        AMX_RUN_HANDLER_MEDIUM(ep->handler[hid], buf, pargs, numargs, pData, msg->nBytes);
        if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
        break;
      }
      case ammpi_Long: {
        int8_t * const pData = ((int8_t *)ep->segAddr) + msg->destOffset;
        if (!isloopback) /*  a single-message bulk transfer. do the copy */
          memcpy(pData, GET_PACKET_DATA(buf), msg->nBytes);
        if (ep->preHandlerCallback) 
          ep->preHandlerCallback(ammpi_Long, isrequest, hid, buf, 
                                 pData, msg->nBytes, numargs, pargs);
        AMX_RUN_HANDLER_LONG(ep->handler[hid], buf, pargs, numargs, pData, msg->nBytes);
        if (ep->postHandlerCallback) ep->postHandlerCallback(cat, isrequest);
        break;
      }
      default: AMX_unreachable();
    }
  }
  status->handlerRunning = FALSE;

  #if AMMPI_FLOW_CONTROL || AMMPI_COLLECT_LATENCY_STATS
    if (isrequest && !status->replyIssued &&
        ep->perProcInfo[sourceId].tokens_in > ep->tokens_slack) { 
      static va_list va_dummy; va_list *p_dummy = &va_dummy; p_dummy++; /* dummy value */
      /*  user didn't reply, so issue an auto-reply */
      if_pf (AMMPI_ReplyGeneric(ammpi_Short, buf, 0, 0, 0, 0, 0, va_dummy, 
                                ammpi_system_autoreply, 0) != AM_OK) /*  should never happen */
        AMX_Err("Failed to issue auto reply in AMMPI_ServiceIncomingMessages");
    }
  #endif
} 
#undef AMMPI_REFUSEMESSAGE  /* this is a local-use-only macro */

/* main message receive workhorse - 
 * service available incoming messages, up to AMMPI_MAX_RECVMSGS_PER_POLL
 * note this is NOT reentrant - only one call to this method should be in progress at any time
 * if blockForActivity, then block until something happens
 * sets numUserHandlersRun to number of user handlers that got run
 */
#if AMX_DEBUG 
  /* enforce lack of reentrancy */
  extern int _AMMPI_ServiceIncomingMessages(ep_t ep, int blockForActivity, int repliesOnly);
  extern int AMMPI_ServiceIncomingMessages(ep_t ep, int blockForActivity, int repliesOnly) {
    static int inServiceIncomingMessages = 0;
    int retval;
    AMX_assert(inServiceIncomingMessages == 0 || 
      (inServiceIncomingMessages == 1 && repliesOnly));
    inServiceIncomingMessages++;
    retval = _AMMPI_ServiceIncomingMessages(ep, blockForActivity, repliesOnly);
    inServiceIncomingMessages--;
    return retval;
  }
#else
  #define _AMMPI_ServiceIncomingMessages AMMPI_ServiceIncomingMessages
#endif
extern int _AMMPI_ServiceIncomingMessages(ep_t ep, int blockForActivity, int repliesOnly) {
  int numUserHandlersRun = 0;

  do {
    #if AMMPI_PREPOST_RECVS
      ammpi_virtual_network_t *activeNet;
      int activeidx;
    #else
      static ammpi_buf_t _recvBuffer[2];
    #endif  
    ammpi_buf_t *buf = NULL; /* the buffer that holds the incoming msg */
    MPI_Status mpistatus;

    /* check for message */
    #if AMMPI_PREPOST_RECVS
      { int msgready = 0;
        #if AMMPI_MPIIRECV_ORDERING_BUGCHECK
          static int recvorder_bugcheck = 0;
          if (recvorder_bugcheck < 0 ||
              recvorder_bugcheck++ == AMMPI_MPIIRECV_ORDERING_BUGCHECK) { 
            int idxready;
            static int alt = 0;
            activeNet = &ep->Rep;
            if (!repliesOnly && ((alt++)&1)) activeNet = &ep->Req;
            MPI_SAFE(MPI_Testany(activeNet->rxNumBufs, activeNet->rxHandle, &idxready, &msgready, &mpistatus));
            if (msgready) {
              if (idxready != activeNet->rxCurr && recvorder_bugcheck > 0) { 
                AMX_DEBUG_WARN(("Detected bug in MPI recv ordering - activating workaround (%s,%i,%i)", 
                    (activeNet == &ep->Req ? "Req":"Rep"), idxready, activeNet->rxCurr)); 
                recvorder_bugcheck = -1; /* ordering is now messed up - enable the hack for the remainder of this run */
              }
              activeNet->rxCurr = idxready;
              goto gotone;
            } else if (recvorder_bugcheck > 0) recvorder_bugcheck = 0;
          }
        #endif
        #if AMMPI_SEPARATE_TEST /* use separate test calls */
            #if AMMPI_SEPARATE_TEST_BOUNCE
              static int bounce = 0; /* bounce back and forth between pools */
              if (!repliesOnly && ((bounce++)&1)) goto testreq;
            #endif

            activeNet = &ep->Rep;
            AMX_assert(activeNet->rxHandle[activeNet->rxCurr] != MPI_REQUEST_NULL);
            MPI_SAFE(MPI_Test(&activeNet->rxHandle[activeNet->rxCurr], &msgready, &mpistatus));
            if (msgready) goto gotone;

            #if AMMPI_SEPARATE_TEST_BOUNCE
              goto testdone; 
            #endif

          if (!repliesOnly) {
          testreq:
            activeNet = &ep->Req;
            AMX_assert(activeNet->rxHandle[activeNet->rxCurr] != MPI_REQUEST_NULL);
            MPI_SAFE(MPI_Test(&activeNet->rxHandle[activeNet->rxCurr], &msgready, &mpistatus));
            if (msgready) goto gotone;
          }
          testdone:
          if_pt (!blockForActivity) return AM_OK; /* nothing else waiting */
        #endif

        { MPI_Request rxCheck[2];
          int idxready;
          int numToCheck = 1;
          rxCheck[0] = ep->Rep.rxHandle[ep->Rep.rxCurr];
          AMX_assert(rxCheck[0] != MPI_REQUEST_NULL);
          if_pt (!repliesOnly) {
            rxCheck[1] = ep->Req.rxHandle[ep->Req.rxCurr];
            AMX_assert(rxCheck[1] != MPI_REQUEST_NULL);
            numToCheck++;
          }
          if_pf (blockForActivity) {
            msgready = TRUE;
            MPI_SAFE(MPI_Waitany(numToCheck, rxCheck, &idxready, &mpistatus));
          } else {
            MPI_SAFE(MPI_Testany(numToCheck, rxCheck, &idxready, &msgready, &mpistatus));
          }
          if (msgready) {
            activeNet = (idxready ? &ep->Req : &ep->Rep);
            AMX_assert(rxCheck[idxready] == MPI_REQUEST_NULL);
            activeNet->rxHandle[activeNet->rxCurr] = MPI_REQUEST_NULL; /* required by AMMPI_FreeEndpointBuffers */
          } else return AM_OK; /* nothing else waiting */
        }
      gotone:
        #if AMMPI_MPIIRECV_ORDERING_BUGCHECK
          if (recvorder_bugcheck > 0) recvorder_bugcheck = 0; /* reset */
        #endif
        AMX_assert(activeNet == &ep->Rep || !repliesOnly);
        activeidx = activeNet->rxCurr;
        AMX_assert(activeNet->rxHandle[activeidx] == MPI_REQUEST_NULL);
        buf = &activeNet->rxBuf[activeidx];
      }
    #else
      do { /* we can't make a blocking probe on two separate communicators, so we need to spin bouncing between them */
        int msgready;
        AMX_assert(repliesOnly == 0 || repliesOnly == 1);
        MPI_SAFE(MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, *ep->Rep.mpicomm, &msgready, &mpistatus));
        if (msgready) {
          buf = &_recvBuffer[repliesOnly];
          MPI_SAFE(MPI_Recv(buf, AMMPI_MAX_NETWORK_MSG, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, *ep->Rep.mpicomm, &mpistatus));
          break;
        }
        if (!repliesOnly) {
          MPI_SAFE(MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, *ep->Req.mpicomm, &msgready, &mpistatus));
          if (msgready) {
            buf = &_recvBuffer[repliesOnly];
            MPI_SAFE(MPI_Recv(buf, AMMPI_MAX_NETWORK_MSG, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, *ep->Req.mpicomm, &mpistatus));
            break;
          }
        }
      } while (blockForActivity);
      if (!buf) return AM_OK; /* nothing else waiting */
    #endif

    AMX_assert(buf);

    /* we have a real message waiting - get it */
    { ammpi_bufstatus_t* status = &(buf->status); /* the status block for this buffer */
     #if AMX_DEBUG || AMX_DEBUG_VERBOSE
      int recvlen;
     #endif

      if_pf (mpistatus.MPI_TAG != ep->name.mpitag) {
        AMX_DEBUG_WARN(("ignoring a stray MPI message (wrong MPI tag)..."));
        goto donewithmessage;
      }

      #if AMX_DEBUG 
      { /* MPI-specific sanity checks */
        MPI_SAFE(MPI_Get_count(&mpistatus, MPI_BYTE, &recvlen));
        AMX_CHECK_ERRFR((recvlen > AMMPI_MAX_NETWORK_MSG),
          RESOURCE, AMMPI_ServiceIncomingMessages, "buffer overrun - received message too long");
        AMX_CHECK_ERRFR((recvlen  < AMMPI_MIN_NETWORK_MSG),
          RESOURCE, AMMPI_ServiceIncomingMessages, "incomplete message received");
      }
      #endif

      /* remember which ep sent/recvd this message */
      status->sourceAddr.mpirank = mpistatus.MPI_SOURCE;
      status->dest = ep; 

      { /*  find the source id */
        int32_t sourceId; /* id in perProcInfo of sender */
        int mpi_id = status->sourceAddr.mpirank;
        /* can't use sourceAddrToId because we don't know full en_t (remote mpitag) */
        if_pt (mpi_id < ep->totalP && /* first check common case where rank matches mapping */
          mpi_id == ep->perProcInfo[mpi_id].remoteName.mpirank) {
          sourceId = mpi_id;
          status->sourceAddr.mpitag = ep->perProcInfo[sourceId].remoteName.mpitag;
        } else { /* do linear search - leave as -1 if unfound */
          for (sourceId = ep->totalP-1; sourceId >= 0; sourceId--) {
            if (mpi_id == ep->perProcInfo[sourceId].remoteName.mpirank) break;
          }
          if (sourceId >= 0)
            status->sourceAddr.mpitag = ep->perProcInfo[sourceId].remoteName.mpitag;
        }
        status->sourceId = (ammpi_node_t)sourceId;
      }

      AMX_VERBOSE_INFO(("MPI_Recv got buflen=%i sourceAddr=%s", recvlen, AMMPI_enStr(status->sourceAddr, 0)));

      if (repliesOnly) AMX_assert(!AMMPI_MSG_ISREQUEST(&buf->Msg));

      AMMPI_processPacket(buf, 0);
      numUserHandlersRun++;

      donewithmessage: ; /* message handled - continue to next one */
      #if AMMPI_PREPOST_RECVS
        /* repost the recv */
        AMX_assert(activeidx == activeNet->rxCurr);
        #if AMMPI_RECV_REPOST_SLACK
          /* postpone it until later if possible, to maximize overlap */
          if (activeNet->rxPostSlack < activeNet->rxPostSlackMax) activeNet->rxPostSlack++;
          else { /* too far behind, repost oldest now */
            int oldestidx = activeNet->rxCurr - activeNet->rxPostSlack;
            if (oldestidx < 0) oldestidx += activeNet->rxNumBufs;
            AMX_assert(oldestidx >= 0 && oldestidx < activeNet->rxNumBufs);
            if (AMMPI_PostRecvBuffer(&activeNet->rxBuf[oldestidx],
                                    &activeNet->rxHandle[oldestidx],
                                    activeNet->mpicomm)) AMX_RETURN_ERR(RESOURCE); 
          }
        #else
          if (AMMPI_PostRecvBuffer(&activeNet->rxBuf[activeidx],
                                  &activeNet->rxHandle[activeidx],
                                  activeNet->mpicomm)) AMX_RETURN_ERR(RESOURCE); 
        #endif
        activeidx++;
        if (activeidx >= activeNet->rxNumBufs) activeidx = 0;
        activeNet->rxCurr = activeidx;
      #endif
      } /*  message waiting */

      if_pf (blockForActivity && numUserHandlersRun > 0) return AM_OK; /* got one - done blocking */
    } while (numUserHandlersRun < ((unsigned int)AMMPI_MAX_RECVMSGS_PER_POLL));
  return AM_OK;
} /*  AMMPI_ServiceIncomingMessages */
/*------------------------------------------------------------------------------------
 * Poll
 *------------------------------------------------------------------------------------ */
extern int AM_Poll(eb_t eb) {
  int i;
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!eb),BAD_ARG);

  for (i = 0; i < eb->n_endpoints; i++) {
    int retval;
    ep_t ep = eb->endpoints[i];

    if_pt (ep->depth != -1) { /* only poll endpoints which have buffers */
      retval = AMMPI_ServiceIncomingMessages(ep, FALSE, FALSE); /* drain network and check for activity */
      if_pf (retval != AM_OK) AMX_RETURN(retval);
    }
  }

  return AM_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_Block(eb_t eb) {
  /* block until some endpoint receive buffer becomes non-empty with a valid user message
   * may poll, and does handle SPMD control events
   */
  int retval = AM_OK;
  if (eb->n_endpoints == 1) {
    /* drain network and check for activity */
    retval = AMMPI_ServiceIncomingMessages(eb->endpoints[0], TRUE, FALSE); 
  } else {
    /* we could implement this (at least for AMMPI_PREPOST_RECVS) by combining the handle vectors, 
     * but it doesn't seem worthwhile right now
     */
    AMX_FatalErr("unimplemented: tried to AMMPI_Block on an endpoint-bundle containing multiple endpoints...");
  }
  return retval;
}
/*------------------------------------------------------------------------------------
 * Generic Request/Reply
 *------------------------------------------------------------------------------------ */
static int AMMPI_RequestGeneric(ammpi_category_t category, 
                          ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr, 
                          uint8_t systemType, uint8_t systemArg) {
  static char _stagingbuf[sizeof(ammpi_buf_t)+8];
  int packetlength;
  ammpi_buf_t *outgoingbuf;
  en_t destaddress = request_endpoint->translation[reply_endpoint].name;
  const int isloopback = AMMPI_enEqual(destaddress, request_endpoint->name);
  #if AMMPI_NONBLOCKING_SENDS
    int predictedsz;
  #endif

  /*  always poll before sending a request */
  AM_Poll(request_endpoint->eb);

 {
  MPI_Request *mpihandle = NULL;
  if (isloopback) {
    outgoingbuf = (ammpi_buf_t *)AMX_ALIGNUP(&_stagingbuf,8);
  } else {
    #if AMMPI_NONBLOCKING_SENDS
      /*  acquire a free request buffer */
      int retval;
      predictedsz = PREDICT_PACKET_LENGTH(numargs, nbytes);
      retval = AMMPI_AcquireSendBuffer(request_endpoint, predictedsz, TRUE, &outgoingbuf, &mpihandle);
      if_pf (retval != AM_OK) AMX_RETURN(retval);
      AMX_assert(outgoingbuf && mpihandle && *mpihandle == MPI_REQUEST_NULL);
    #else
      outgoingbuf = (ammpi_buf_t *)AMX_ALIGNUP(&_stagingbuf,8);
    #endif
    #if AMMPI_FLOW_CONTROL
    { int remoteid = request_endpoint->translation[reply_endpoint].id;
      AMX_assert(systemType == ammpi_system_user);
      AMX_assert(systemArg == 0);
      while (request_endpoint->perProcInfo[remoteid].tokens_out == 0) { /* back pressure */
        AMX_DEBUG_WARN_TH("Out of request send credits. polling...");
        AM_Poll(request_endpoint->eb);
      }
      request_endpoint->perProcInfo[remoteid].tokens_out--;
      systemArg = MIN(255,request_endpoint->perProcInfo[remoteid].tokens_in); /* return tokens */
      request_endpoint->perProcInfo[remoteid].tokens_in -= systemArg;
    }
    #endif
  }

  /*  setup message meta-data */
  { ammpi_msg_t *msg = &outgoingbuf->Msg;
    AMMPI_MSG_SETFLAGS(msg, TRUE, category, numargs);
    msg->destOffset = dest_offset;
    msg->handlerId = handler;
    msg->nBytes = (uint16_t)nbytes;
    msg->systemMessageType = systemType;
    msg->systemMessageArg = systemArg;
    #if AMMPI_USE_AMTAGS
      msg->tag = request_endpoint->translation[reply_endpoint].tag;
    #endif
  }

  { /*  setup args */
    int i;
    uint32_t *args = GET_PACKET_ARGS(outgoingbuf);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMMPI_MAX_SHORT) args[i] = 0;
      outgoingbuf->Msg._reserved = 0;
    #endif
  }

  if (isloopback) { /* run handler synchronously */
    ammpi_bufstatus_t* const status = &(outgoingbuf->status); /* the status block for this buffer */
    if (nbytes > 0) { /* setup data */
      if (category == ammpi_Long) { /* one-copy */
        memmove(((int8_t *)request_endpoint->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
      }
    }
    status->dest = request_endpoint;
    status->sourceId = reply_endpoint;
    status->sourceAddr = request_endpoint->name;

    AMMPI_processPacket(outgoingbuf, 1);
  } else { /*  perform the send */
    int retval;

    if (nbytes > 0) { /*  setup data */
      memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
    }

    AMMPI_VERIFY_MPI_SETSEQNUM(outgoingbuf->Msg, 1, request_endpoint, request_endpoint->translation[reply_endpoint].id);

    packetlength = GET_PACKET_LENGTH(outgoingbuf);
    #if AMMPI_NONBLOCKING_SENDS
      AMX_assert(packetlength <= predictedsz);
    #endif
    retval = sendPacket(request_endpoint, &request_endpoint->Req, 
                        outgoingbuf, packetlength, destaddress, mpihandle);
    if_pf (retval != AM_OK) AMX_RETURN(retval);

    #if AMMPI_COLLECT_LATENCY_STATS
      { uint64_t now = AMX_getMicrosecondTimeStamp();
        outgoingdesc->firstSendTime = now;
      }
    #endif
    AMMPI_STATS(request_endpoint->stats.RequestsSent[category]++);
    AMMPI_STATS(request_endpoint->stats.RequestDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMMPI_STATS(request_endpoint->stats.RequestTotalBytesSent[category] += packetlength);
  }

  return AM_OK;
 }
}
/* ------------------------------------------------------------------------------------ */
static int AMMPI_ReplyGeneric(ammpi_category_t category, 
                          ammpi_buf_t *requestbuf, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr,
                          uint8_t systemType, uint8_t systemArg) {
  static char _stagingbuf[sizeof(ammpi_buf_t)+8];
  ammpi_buf_t *outgoingbuf;
  ep_t const ep = requestbuf->status.dest;
  ammpi_node_t const destP = requestbuf->status.sourceId;
  const int isloopback = AMMPI_enEqual(requestbuf->status.sourceAddr, ep->name);
  #if AMMPI_NONBLOCKING_SENDS
    int predictedsz;
  #endif

  /*  we don't poll within a reply because by definition we are already polling somewhere in the call chain */

 {
  MPI_Request *mpihandle = NULL;
  if (isloopback) {
    outgoingbuf = (ammpi_buf_t *)AMX_ALIGNUP(&_stagingbuf,8);
  } else {
    #if AMMPI_NONBLOCKING_SENDS
      /*  acquire a free reply buffer */
      int retval;
      predictedsz = PREDICT_PACKET_LENGTH(numargs, nbytes);
      retval = AMMPI_AcquireSendBuffer(ep, predictedsz, FALSE, &outgoingbuf, &mpihandle);
      if_pf (retval != AM_OK) AMX_RETURN(retval);
      AMX_assert(outgoingbuf && mpihandle && *mpihandle == MPI_REQUEST_NULL);
    #else
      outgoingbuf = (ammpi_buf_t *)AMX_ALIGNUP(&_stagingbuf,8);
    #endif
    #if AMMPI_FLOW_CONTROL
      if (systemType == ammpi_system_user || systemType == ammpi_system_autoreply) {
        AMX_assert(systemArg == 0);
        systemArg = MIN(255,ep->perProcInfo[destP].tokens_in);
        ep->perProcInfo[destP].tokens_in -= systemArg;
      }
    #endif
  }

  /*  setup message meta-data */
  { ammpi_msg_t *msg = &outgoingbuf->Msg;
    AMMPI_MSG_SETFLAGS(msg, FALSE, category, numargs);
    msg->destOffset = dest_offset;
    msg->handlerId = handler;
    msg->nBytes = (uint16_t)nbytes;
    msg->systemMessageType = systemType;
    msg->systemMessageArg = systemArg;
    #if AMMPI_USE_AMTAGS
      msg->tag = ep->perProcInfo[destP].tag;
    #endif
  }

  { /*  setup args */
    int i;
    uint32_t *args = GET_PACKET_ARGS(outgoingbuf);
    for (i = 0; i < numargs; i++) {
      args[i] = (uint32_t)va_arg(argptr, int); /* must be int due to default argument promotion */
    }
    #if USE_CLEAR_UNUSED_SPACE
      if (i < AMMPI_MAX_SHORT) args[i] = 0;
      outgoingbuf->Msg._reserved = 0;
    #endif
  }

  if (isloopback) { /* run handler synchronously */
    ammpi_bufstatus_t* const status = &(outgoingbuf->status); /* the status block for this buffer */
    if (nbytes > 0) { /* setup data */
      if (category == ammpi_Long) { /* one-copy */
        memmove(((int8_t *)ep->segAddr) + dest_offset, 
                source_addr, nbytes);
      } else { /* mediums still need data copy */
        memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
      }
    }
    status->dest = ep;
    status->sourceId = destP;
    status->sourceAddr = ep->name;

    AMMPI_processPacket(outgoingbuf, 1);
  } else { /*  perform the send */
    int packetlength = GET_PACKET_LENGTH(outgoingbuf);
    en_t destaddress = ep->perProcInfo[destP].remoteName;
    int retval;

    if (nbytes > 0) { /*  setup data */
      memcpy(GET_PACKET_DATA(outgoingbuf), source_addr, nbytes);
      #if 0 /* not necessary- we never send this stuff */
        #if USE_CLEAR_UNUSED_SPACE
          memset(&(GET_PACKET_DATA(outgoingbuf)[nbytes]), 0, AMMPI_MAX_LONG - nbytes);
        #endif
      #endif
    }

    AMMPI_VERIFY_MPI_SETSEQNUM(outgoingbuf->Msg, 0, ep, destP);

    #if AMMPI_NONBLOCKING_SENDS
      AMX_assert(packetlength <= predictedsz);
    #endif
    retval = sendPacket(ep, &ep->Rep, outgoingbuf, packetlength, destaddress, mpihandle);
    if_pf (retval != AM_OK) AMX_RETURN(retval);
    AMMPI_STATS(ep->stats.RepliesSent[category]++);
    AMMPI_STATS(ep->stats.ReplyDataBytesSent[category] += sizeof(int) * numargs + nbytes);
    AMMPI_STATS(ep->stats.ReplyTotalBytesSent[category] += packetlength);
  }

  requestbuf->status.replyIssued = TRUE;
  return AM_OK;
 }
}
/*------------------------------------------------------------------------------------
 * Request
 *------------------------------------------------------------------------------------ */
extern int AMMPI_RequestVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                         int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!request_endpoint),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR((reply_endpoint >= request_endpoint->translationsz ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  /*  call the generic requestor */
  return AMMPI_RequestGeneric(ammpi_Short, 
                                request_endpoint, reply_endpoint, handler, 
                                NULL, 0, 0,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_Request(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                         int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_RequestVA(request_endpoint, reply_endpoint, handler, 
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_RequestIVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!request_endpoint),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR((reply_endpoint >= request_endpoint->translationsz ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMX_CHECK_ERR((!source_addr && nbytes > 0),BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMMPI_MAX_MEDIUM,BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  /*  call the generic requestor */
  return AMMPI_RequestGeneric(ammpi_Medium, 
                                request_endpoint, reply_endpoint, handler, 
                                source_addr, nbytes, 0,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_RequestI(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_RequestIVA(request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes,
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_RequestXferVA(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async, 
                          int numargs, va_list argptr) {
  AMX_CHECKINIT();
  AMX_CHECK_ERR((!request_endpoint),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_CHECK_ERR((request_endpoint->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  AMX_CHECK_ERR((reply_endpoint >= request_endpoint->translationsz ||
     !request_endpoint->translation[reply_endpoint].inuse),BAD_ARG);
  AMX_CHECK_ERR((!source_addr && nbytes > 0),BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMMPI_MAX_LONG,BAD_ARG);
  AMX_CHECK_ERR((dest_offset > AMMPI_MAX_SEGLENGTH),BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  if (async) { /*  decide if we can satisfy request without blocking */
    /* it's unclear from the spec whether we should poll before an async failure,
     * but by definition the app must be prepared for handlers to run when calling this 
     * request, so it shouldn't cause anything to break, and the async request is more likely
     * to succeed if we do. so:
     */
    AM_Poll(request_endpoint->eb);

    /* too hard to compute whether this will block */
    AMX_FatalErr("unimplemented: AMMPI_RequestXferAsyncM not implemented - use AMMPI_RequestXferM");
  }
  /* perform the send */
  return AMMPI_RequestGeneric(ammpi_Long, 
                                request_endpoint, reply_endpoint, handler, 
                                source_addr, nbytes, dest_offset,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_RequestXfer(ep_t request_endpoint, ammpi_node_t reply_endpoint, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int async, 
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_RequestXferVA(request_endpoint, reply_endpoint, handler, 
                                  source_addr, nbytes, dest_offset,
                                  async,
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/*------------------------------------------------------------------------------------
 * Reply
 *------------------------------------------------------------------------------------ */
extern int AMMPI_ReplyVA(void *token, handler_t handler, 
                       int numargs, va_list argptr) {
  ammpi_buf_t *requestbuf;

  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    requestbuf = (ammpi_buf_t *)token;
    AMX_CHECK_ERR((!AMMPI_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMX_CHECK_ERR((!requestbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMX_CHECK_ERR((requestbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMX_CHECK_ERR((((ammpi_system_messagetype_t)requestbuf->Msg.systemMessageType) != ammpi_system_user),
                    RESOURCE); /* can't reply to a system message (returned message) */
  }

  /*  call the generic replier */
  return AMMPI_ReplyGeneric(ammpi_Short, 
                                requestbuf, handler, 
                                NULL, 0, 0,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_Reply(void *token, handler_t handler, 
                       int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_ReplyVA(token, handler, 
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_ReplyIVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, va_list argptr) {
  ammpi_buf_t *requestbuf;

  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_CHECK_ERR((!source_addr && nbytes > 0),BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMMPI_MAX_MEDIUM,BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    requestbuf = (ammpi_buf_t *)token;
    AMX_CHECK_ERR((!AMMPI_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMX_CHECK_ERR((!requestbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMX_CHECK_ERR((requestbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMX_CHECK_ERR((((ammpi_system_messagetype_t)requestbuf->Msg.systemMessageType) != ammpi_system_user),
                    RESOURCE); /* can't reply to a system message (returned message) */
  }

  /*  call the generic replier */
  return AMMPI_ReplyGeneric(ammpi_Medium, 
                                requestbuf, handler, 
                                source_addr, nbytes, 0,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_ReplyI(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes,
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_ReplyIVA(token, handler, 
                                  source_addr, nbytes,
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_SendControlMessage(ep_t from, en_t to, int numargs, ...) {
  int dest_endpoint_index = -1;

  AMX_CHECKINIT();
  AMX_CHECK_ERR((!from),BAD_ARG);
  AMX_CHECK_ERR((numargs < 0 || numargs > AMMPI_MAX_SHORT),BAD_ARG);
  AMX_CHECK_ERR((from->depth == -1),NOT_INIT); /* it's an error to call before AM_SetExpectedResources */
  dest_endpoint_index = sourceAddrToId(from, to);
  AMX_CHECK_ERR((dest_endpoint_index == -1),BAD_ARG); /* can only send to a mapped peer */

  { /*  control messages use the Reply mechanism in order to be safe in 
        AM handler context, where it's unsafe to poll
     */
    int retval;
    ammpi_buf_t fakeRequestBuf;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    fakeRequestBuf.status.dest = from; /* pretend we're servicing a request from target node */
    fakeRequestBuf.status.sourceId = dest_endpoint_index;
    fakeRequestBuf.status.sourceAddr = to;
    fakeRequestBuf.status.handlerRunning = 1;
    retval = AMMPI_ReplyGeneric(ammpi_Short, 
                                  &fakeRequestBuf,
                                  0, NULL, 0, 0,
                                  numargs, argptr,
                                  ammpi_system_controlmessage, 0);
    va_end(argptr);
    return retval;
  }
}
/* ------------------------------------------------------------------------------------ */
extern int AMMPI_ReplyXferVA(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, va_list argptr) {
  ammpi_buf_t *requestbuf;

  AMX_CHECKINIT();
  AMX_CHECK_ERR((!token),BAD_ARG);
  AMX_CHECK_ERR((AMMPI_BADHANDLERVAL(handler)),BAD_ARG);
  AMX_CHECK_ERR((!source_addr && nbytes > 0),BAD_ARG);
  AMX_CHECK_ERR(nbytes > AMMPI_MAX_LONG,BAD_ARG);
  AMX_CHECK_ERR((dest_offset > AMMPI_MAX_SEGLENGTH),BAD_ARG);
  AMX_assert(numargs >= 0 && numargs <= AMMPI_MAX_SHORT);

  { /*  semantic checking on reply (are we in a handler, is this the first reply, etc.) */
    requestbuf = (ammpi_buf_t *)token;
    AMX_CHECK_ERR((!AMMPI_MSG_ISREQUEST(&requestbuf->Msg)),RESOURCE); /* token is not a request */
    AMX_CHECK_ERR((!requestbuf->status.handlerRunning),RESOURCE); /* token is not for an active request */
    AMX_CHECK_ERR((requestbuf->status.replyIssued),RESOURCE);     /* already issued a reply */
    AMX_CHECK_ERR((((ammpi_system_messagetype_t)requestbuf->Msg.systemMessageType) != ammpi_system_user),
                    RESOURCE); /* can't reply to a system message (returned message) */
  }


  /*  call the generic replier */
  return AMMPI_ReplyGeneric(ammpi_Long, 
                                requestbuf, handler, 
                                source_addr, nbytes, dest_offset,
                                numargs, argptr,
                                ammpi_system_user, 0);
}
extern int AMMPI_ReplyXfer(void *token, handler_t handler, 
                          void *source_addr, size_t nbytes, uintptr_t dest_offset, 
                          int numargs, ...) {
    int retval;
    va_list argptr;
    va_start(argptr, numargs); /*  pass in last argument */
    retval = AMMPI_ReplyXferVA(token, handler, 
                                  source_addr, nbytes, dest_offset,
                                  numargs, argptr);
    va_end(argptr);
    return retval;
}
/* ------------------------------------------------------------------------------------ */
extern void AMMPI_DefaultReturnedMsg_Handler(int status, op_t opcode, void *token) {
  const char *statusStr = "*unknown*";
  const char *opcodeStr = "*unknown*";
  ammpi_buf_t *msgbuf = (ammpi_buf_t *)token;
  int numArgs = AMMPI_MSG_NUMARGS(&msgbuf->Msg);
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
    sprintf(tmp, "0x%08x  ", (unsigned int)args[i]);
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
             AMMPI_enStr(msgbuf->status.sourceAddr, 0), (int)msgbuf->status.sourceId,
             msgbuf->Msg.handlerId, 
             #if AMMPI_USE_AMTAGS
               AMMPI_tagStr(msgbuf->Msg.tag, 0),
             #else
               "<AM tags disabled>",
             #endif
             numArgs, argStr);
}
/* ------------------------------------------------------------------------------------ */
