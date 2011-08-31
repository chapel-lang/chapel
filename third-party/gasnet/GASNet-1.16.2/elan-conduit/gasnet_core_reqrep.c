/*   $Source: /var/local/cvs/gasnet/elan-conduit/gasnet_core_reqrep.c,v $
 *     $Date: 2009/09/20 23:34:19 $
 * $Revision: 1.36 $
 * Description: GASNet elan conduit - AM request/reply implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <elan3/elan3.h> /* for ELAN_POLL_EVENT */
#include <unistd.h>

/* 
  Basic design of the core implementation:
  =======================================

  All Shorts/All Longs/Mediums <= GASNETC_ELAN_MAX_QUEUEMSG(320/2048):
    sent using an elan queue of length LIBELAN_TPORT_NSLOTS
    Longs use a blocking elan_put before queuing to ensure ordering  
      use a bounce-buffer if > GASNETC_ELAN_SMALLPUTSZ and not elan-mapped
    AMPoll checks for incoming queue entries 
    All mediums are argument-padded to ensure payload alignment on recvr

  Mediums > GASNETC_ELAN_MAX_QUEUEMSG(320/2048):
    sent using a tport message in a pre-allocated buffer
    Keep tport Tx buffers in a FIFO of length LIBELAN_TPORT_NSLOTS - 
      poll for Tx completion starting at oldest Tx buffer whenever we need one
      may spin-poll during a send if all Tx buffers occupied
    Keep a FIFO of posted tport Rx bufs, which are guaranteed to arrive in order
      AMPoll checks the head for completion
    every tport buffer has a dedicated descriptor (gasnetc_bufdesc_t)
      holds ELAN_EVENT for pending Tx/Rx
      pointer to the buffer (gasnetc_buf_t) and possibly a system Rx buffer

  AMPoll handles up to GASNETC_MAX_RECVMSGS_PER_POLL messages from 
    either the queue or tport (giving precedence to the queue)

  All loopback AM messages are run synchronously inside the request/reply
*/

#define GASNETC_MEDHEADER_PADARG(numargs) \
        ((numargs & 0x1) ^ ((GASNETC_MED_HEADERSZ>>2) & 0x1))

/* ------------------------------------------------------------------------------------ */
static ELAN_QUEUE *gasnetc_queue = NULL;
#if GASNETC_USE_MAINQUEUE
  static ELAN_MAIN_QUEUE *gasnetc_mainqueue = NULL;
#else
  static ELAN_QUEUE_TX *gasnetc_queuetx = NULL;
  static ELAN_QUEUE_RX *gasnetc_queuerx = NULL;
  #if GASNETC_OVERLAP_AMQUEUE
    static gasnete_evtbin_t gasnetc_am_evtbin;
    static int gasnetc_am_throttle = 0;
    #ifndef GASNETC_DEFAULT_AM_THROTTLE
    #define GASNETC_DEFAULT_AM_THROTTLE 8
    #endif
  #endif
#endif
static int gasnetc_queuesz = 0; /* queue size for main queue and tport bufs */

static gasnetc_bufdesc_t *gasnetc_tportTxFree = NULL; /* list of free tx bufs (from startup) */
static gasnetc_bufdesc_t *gasnetc_tportTxFIFOHead = NULL; /* list of tx's in progress, */
static gasnetc_bufdesc_t *gasnetc_tportTxFIFOTail = NULL; /* and some will NULL events that havent launched yet */

static gasnetc_bufdesc_t *gasnetc_tportRxFIFOHead = NULL; /* list of rx's waiting */
static gasnetc_bufdesc_t *gasnetc_tportRxFIFOTail = NULL;
/* ------------------------------------------------------------------------------------ */
static gasnetc_bufdesc_t *gasnetc_tportGetTxBuf(int addToFifo) {
  /* pop send fifo head buffer and wait for completion,
     add to send fifo with null event 
     assumes elan lock NOT held
  */
  gasnetc_bufdesc_t *desc = NULL;
  ASSERT_ELAN_UNLOCKED();
  ASSERT_SENDFIFO_UNLOCKED();
  LOCK_SENDFIFO();
  while (!desc) {
    if (gasnetc_tportTxFree) { /* free list contains some bufs */
      GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf returned a tportTx buf from the free list"));
      desc = gasnetc_tportTxFree;
      gasnetc_tportTxFree = desc->next;
      gasneti_assert(desc->event == NULL);
    } else { /* need to reap some tx bufs */
      LOCK_ELAN_WEAK();
      if (gasnetc_tportTxFIFOHead && 
          gasnetc_tportTxFIFOHead->event &&
          elan_tportTxDone(gasnetc_tportTxFIFOHead->event)) {
        /* common case - oldest tx is complete */
        GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf obtained a tportTx buf by completing the TxFIFO head"));
        desc = gasnetc_tportTxFIFOHead;
        #if 0
          /* according to undocumented info from quadrics, 
             it's illegal to call TxWait after successful TxDone */
          elan_tportTxWait(desc->event); 
        #endif
        gasnetc_tportTxFIFOHead = gasnetc_tportTxFIFOHead->next;
        if (gasnetc_tportTxFIFOHead == NULL) {
          GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf emptied the TxFIFO"));
          gasnetc_tportTxFIFOTail = NULL;
        }
      } else { /* poop - head busy, need to scan for tx */
        if (gasnetc_tportTxFIFOHead) {
          gasnetc_bufdesc_t *lastdesc = gasnetc_tportTxFIFOHead;
          while (lastdesc->next) {
            gasnetc_bufdesc_t *tmp = lastdesc->next;
            if (tmp->event && elan_tportTxDone(tmp->event)) { /* found one */
              GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf obtained a tportTx buf by completing the non-head of TxFIFO"));
              desc = tmp;
              lastdesc->next = tmp->next;
              if (lastdesc->next == NULL) {
                GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf emptied the TxFIFO"));
                gasnetc_tportTxFIFOTail = lastdesc;
              }
              #if 0
                /* according to undocumented info from quadrics, 
                   it's illegal to call TxWait after successful TxDone */
                elan_tportTxWait(desc->event); 
              #endif
              break;
            }
            lastdesc = lastdesc->next;
          }
          gasneti_assert(desc || lastdesc == gasnetc_tportTxFIFOTail);
        } else {
          GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf encountered an empty TxFIFO when looking for a tportTx buf"));
        }
        if (!desc) { /* nothing available now - poll */
          GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf failed to obtain a tportTx buf, polling..."));
          UNLOCK_ELAN_WEAK();
          UNLOCK_SENDFIFO();
          gasneti_AMPoll();
          LOCK_SENDFIFO();
          LOCK_ELAN_WEAK();
        }
      }
      UNLOCK_ELAN_WEAK();
    }
  }

  desc->event = NULL;
  desc->next = NULL;

  /* add to send fifo - event will be filled in later by caller */
  if (addToFifo) {
    if (gasnetc_tportTxFIFOTail) { /* fifo non-empty */
      gasneti_assert(gasnetc_tportTxFIFOHead);
      gasneti_assert(gasnetc_tportTxFIFOTail->next == NULL);
      gasnetc_tportTxFIFOTail->next = desc;
      gasnetc_tportTxFIFOTail = desc;
    } else {
      GASNETI_TRACE_PRINTF(C,("gasnetc_tportGetTxBuf pushed the first element onto an empty TxFIFO"));
      gasneti_assert(!gasnetc_tportTxFIFOHead);
      gasnetc_tportTxFIFOHead = desc;
      gasnetc_tportTxFIFOTail = desc;
    }
  }

  UNLOCK_SENDFIFO();

  return desc;
}
/* ------------------------------------------------------------------------------------ */
static void gasnetc_tportReleaseTxBuf(gasnetc_bufdesc_t *desc) {
  /* release a Tx buf without sending it - must NOT be in FIFO
   */
  LOCK_SENDFIFO();
    gasneti_assert(desc->event == NULL);
    desc->next = gasnetc_tportTxFree;
    gasnetc_tportTxFree = desc;
  UNLOCK_SENDFIFO();
}
/* ------------------------------------------------------------------------------------ */
static gasnetc_bufdesc_t *gasnetc_tportCheckRx(void) {
 /* return a buffer if there's an incoming tport msg 
     assumes elan lock is held 
  */
  gasnetc_bufdesc_t *desc = gasnetc_tportRxFIFOHead;
  ASSERT_ELAN_LOCKED();

  if (desc && 
    #if HAVE_ELAN_DONE
      /* shaves 0.5 us off do-nothing elan_tportRxDone by avoiding a deviceCheck */
      elan_done(desc->event, 0) && 
    #endif
      elan_tportRxDone(desc->event)) {
    int sender,tag;
    ELAN_SIZE_T size;

    gasnetc_buf_t *buf = elan_tportRxWait(desc->event, &sender, &tag, &size);
    if (buf != desc->buf_owned)
      GASNETI_TRACE_PRINTF(C,("elan_tportRxWait recieved a tport message in a system buffer"));
    desc->buf = buf;
    desc->event = NULL;
    gasnetc_tportRxFIFOHead = desc->next;
    desc->next = NULL;
    if_pf (gasnetc_tportRxFIFOHead == NULL) {
      GASNETI_TRACE_PRINTF(C,("gasnetc_tportCheckRx returned the last tportRx buffer, leaving an empty tportRx FIFO"));
      gasnetc_tportRxFIFOTail = NULL;
    }
    return desc;
  } 
  else return NULL;
}
/* ------------------------------------------------------------------------------------ */
static void gasnetc_tportAddRxBuf(gasnetc_bufdesc_t *desc) {
  /* issue an rx and return the buffer to the rx queue
     assumes elan lock is held 
   */
  ASSERT_ELAN_LOCKED();

  if (desc->buf != desc->buf_owned) {/* free a system buffer, if neccessary */
    GASNETI_TRACE_PRINTF(C,("gasnetc_tportAddRxBuf returned a system buffer using elan_tportBufFree"));
    elan_tportBufFree(TPORT(),desc->buf);
    desc->buf = desc->buf_owned;
  }

  /* post a new recv */
  gasneti_assert(!desc->event);
  desc->event = elan_tportRxStart(TPORT(), 
                    ELAN_TPORT_RXBUF | ELAN_TPORT_RXANY, 
                    0, 0, 0, 0,
                    desc->buf, sizeof(gasnetc_buf_t));
  gasneti_assert(desc->event);

  /* push on tail of queue */
  desc->next = NULL;
  if_pt (gasnetc_tportRxFIFOTail) {
    gasneti_assert(gasnetc_tportRxFIFOHead);
    gasnetc_tportRxFIFOTail->next = desc;
    gasnetc_tportRxFIFOTail = desc;
  } else { /* list empty (rare case..) */
    GASNETI_TRACE_PRINTF(C,("gasnetc_tportAddRxBuf pushed an Rx buffer onto an empty tportRx FIFO"));
    gasneti_assert(gasnetc_tportRxFIFOHead == NULL);
    gasnetc_tportRxFIFOHead = desc;
    gasnetc_tportRxFIFOTail = desc;
  }
}

/* ------------------------------------------------------------------------------------ */
extern void gasnetc_initbufs(void) {
  /* create a tport message queue */
  ELAN_QUEUE *tport_queue;
  LOCK_ELAN();
  #ifdef ELAN_VER_1_2
    tport_queue = elan_gallocQueue(BASE()->galloc, GROUP());
  #else
    tport_queue = elan_gallocQueue(BASE(), GROUP());
  #endif
  if (tport_queue == NULL) gasneti_fatalerror("elan_gallocQueue() failed");

  GASNETI_TRACE_PRINTF(D,("TPORT queue: main="GASNETI_LADDRFMT
                                     "  elan="GASNETI_LADDRFMT,
                                     GASNETI_LADDRSTR(tport_queue), 
                                     GASNETI_LADDRSTR(elan_main2elan(STATE(),tport_queue))));

  if (GASNETC_MAX_TPORT_MSG != BASE()->tport_bigmsg) 
    GASNETI_TRACE_PRINTF(I,("Warning: overridding BASE()->tport_bigmsg=%i with GASNETC_MAX_TPORT_MSG=%i",
                                       (int)BASE()->tport_bigmsg, (int)GASNETC_MAX_TPORT_MSG));

  /* init tport with the default values we got in base */
  gasnetc_elan_tport = elan_tportInit(STATE(), 
                                      tport_queue, 
    #ifdef ELAN_VER_1_2
                                      elan_main2elan(STATE(),tport_queue),
    #endif
                                      BASE()->tport_nslots,
                                      BASE()->tport_smallmsg,
                                    #if 0
                                      BASE()->tport_bigmsg,
                                    #else
                                      /* need a compile-time const here for maxmedium */
                                      GASNETC_MAX_TPORT_MSG,
                                    #endif
    #if ELAN_VERSION_GE(1,4,8)
                                      BASE()->tport_stripemsg,
    #endif
                                      BASE()->waitType,
                                      BASE()->retryCount
    #if ELAN_VERSION_GE(1,2,0)
                                    , &(BASE()->shm_key),
                                      BASE()->shm_fifodepth,
                                      BASE()->shm_fragsize
    #endif
    #if ELAN_VERSION_GE(1,4,8)
                                    , BASE()->tport_flags
    #endif
                                      );

  /* TODO: is this a good size? */
  gasnetc_queuesz = BASE()->tport_nslots;

  /* setup main queue */
  #ifdef ELAN_VER_1_2
    gasnetc_queue = elan_gallocQueue(BASE()->galloc,GROUP());
  #else
    gasnetc_queue = elan_gallocQueue(BASE(),GROUP());
  #endif
  if_pf(gasnetc_queue == NULL) 
    gasneti_fatalerror("error on elan_gallocQueue in gasnetc_initbufs()");
  #if GASNETC_USE_MAINQUEUE
    gasnetc_mainqueue = elan_mainQueueInit(STATE(), gasnetc_queue, gasnetc_queuesz, GASNETC_ELAN_MAX_QUEUEMSG
      #if ELAN_VERSION_GE(1,4,8)
                                        , BASE()->mqueue_flags
      #endif
      );
    if_pf(gasnetc_mainqueue == NULL) 
      gasneti_fatalerror("error on elan_mainQueueInit in gasnetc_initbufs()");
  #else
    /* TODO: try removing LIBELAN_QUEUEREUSEBUF and doing our own buffer mgt
             to avoid a mandatory memcpy on entry to elan_queueTx */
    gasnetc_queuetx = elan_queueTxInit(STATE(), gasnetc_queue,
                                       ELAN_RAIL_ALL, 
                                    #if GASNETC_OVERLAP_AMQUEUE
                                       LIBELAN_QUEUEREUSEBUF
                                    #else 
                                       0
                                    #endif
                                       );
    if_pf(gasnetc_queuetx == NULL) 
      gasneti_fatalerror("error on elan_queueTxInit in gasnetc_initbufs()");

    #if GASNETC_OVERLAP_AMQUEUE
      gasnetc_am_throttle = gasnett_getenv_int_withdefault("GASNET_AM_THROTTLE", GASNETC_DEFAULT_AM_THROTTLE, 0);
      if (gasnetc_am_throttle < 1) gasnetc_am_throttle = GASNETC_DEFAULT_AM_THROTTLE;
      gasnete_evtbin_init(&gasnetc_am_evtbin, gasnetc_am_throttle, gasneti_malloc(gasnetc_am_throttle*sizeof(ELAN_EVENT*)));
    #endif

    gasnetc_queuerx = elan_queueRxInit(STATE(), gasnetc_queue,
                                       gasnetc_queuesz, GASNETC_ELAN_MAX_QUEUEMSG,
                                       ELAN_RAIL_ALL, 0);
    if_pf(gasnetc_queuerx == NULL) 
      gasneti_fatalerror("error on elan_queueRxInit in gasnetc_initbufs()");
  #endif

  { /* setup buffers */
    gasnetc_bufdesc_t *txdesc = elan_allocMain(STATE(), 8, gasnetc_queuesz*sizeof(gasnetc_bufdesc_t));
    gasnetc_bufdesc_t *rxdesc = elan_allocMain(STATE(), 8, gasnetc_queuesz*sizeof(gasnetc_bufdesc_t));
    int bufsize = GASNETI_ALIGNUP(sizeof(gasnetc_buf_t), 64);
    uint8_t *txbuf = elan_allocMain(STATE(), 64, gasnetc_queuesz*bufsize);
    uint8_t *rxbuf = elan_allocMain(STATE(), 64, gasnetc_queuesz*bufsize);
    int i;

    if (!txdesc || !rxdesc || !txbuf || !rxbuf)
      gasneti_fatalerror("Elan-conduit failed to allocate network buffers!");

    /* Tx buffers */
    gasneti_assert(gasnetc_tportTxFree == NULL);
    gasneti_assert(gasnetc_tportTxFIFOHead == NULL);
    gasneti_assert(gasnetc_tportTxFIFOTail == NULL);
    for (i = gasnetc_queuesz-1; i >= 0 ; i--) {
      gasnetc_buf_t *buf = (gasnetc_buf_t *)(txbuf + (i*bufsize));
      txdesc[i].buf = buf;
      txdesc[i].buf_owned = NULL;
      txdesc[i].event = NULL;
      txdesc[i].next = gasnetc_tportTxFree;
      gasnetc_tportTxFree = &txdesc[i];
    }

    /* Rx buffers */
    gasneti_assert(gasnetc_tportRxFIFOHead == NULL);
    gasneti_assert(gasnetc_tportRxFIFOTail == NULL);
    for (i = 0; i < gasnetc_queuesz; i++) {
      gasnetc_buf_t *buf = (gasnetc_buf_t *)(rxbuf + (i*bufsize));
      rxdesc[i].buf = buf;
      rxdesc[i].buf_owned = buf;
      rxdesc[i].event = NULL;
      gasnetc_tportAddRxBuf(&rxdesc[i]);
    }

    { /* extra checking */
      int i;
      gasnetc_bufdesc_t *desc = gasnetc_tportRxFIFOHead;
      for (i=0; i < gasnetc_queuesz; i++) {
        gasneti_assert(desc == &rxdesc[i]);
        gasneti_assert(desc->event);
        desc = desc->next;
      }
      gasneti_assert(desc == NULL);
      gasneti_assert(gasnetc_tportRxFIFOTail == &rxdesc[gasnetc_queuesz-1]);
    }
  }
  UNLOCK_ELAN();
}
/* ------------------------------------------------------------------------------------ */
static void gasnetc_processPacket(gasnetc_bufdesc_t *desc) {
  gasnetc_buf_t *buf = desc->buf;
  gasnetc_msg_t *msg = &(buf->m.msg);
  gasneti_handler_fn_t handler = gasnetc_handler[msg->handlerId];
  gasnetc_category_t category = GASNETC_MSG_CATEGORY(msg);
  int numargs = GASNETC_MSG_NUMARGS(msg);
  gasneti_assert(numargs >= 0 && numargs <= GASNETC_MAX_ARGS);

  ASSERT_ELAN_UNLOCKED();

  desc->replyIssued = 0;
  desc->handlerRunning = 1;
  switch (category) {
    case gasnetc_Short:
      { gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *)(&(buf->m.msg)+1);
        GASNETI_RUN_HANDLER_SHORT(GASNETC_MSG_ISREQUEST(msg),msg->handlerId,handler,desc,pargs,numargs);
      }
    break;
    case gasnetc_Medium:
      { gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *)(&(buf->m.medmsg)+1);
        int nbytes = buf->m.medmsg.nBytes;
        void *pdata = (pargs + numargs + GASNETC_MEDHEADER_PADARG(numargs));
        GASNETI_RUN_HANDLER_MEDIUM(GASNETC_MSG_ISREQUEST(msg),msg->handlerId,handler,desc,pargs,numargs,pdata,nbytes);
      }
    break;
    case gasnetc_Long:
      { gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *)(&(buf->m.longmsg)+1);
        int nbytes = buf->m.longmsg.nBytes;
        void *pdata = (void *)(buf->m.longmsg.destLoc);
        GASNETI_RUN_HANDLER_LONG(GASNETC_MSG_ISREQUEST(msg),msg->handlerId,handler,desc,pargs,numargs,pdata,nbytes);
      }
    break;
    default: gasneti_fatalerror("unrecognized msg category");
  }
  desc->handlerRunning = 0;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_AMPoll(void) {
  int i;
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0);
#endif

  ASSERT_ELAN_UNLOCKED();

  for (i = 0; GASNETC_MAX_RECVMSGS_PER_POLL == 0 || i < GASNETC_MAX_RECVMSGS_PER_POLL; i++) {
    gasnetc_bufdesc_t *desc;

    LOCK_ELAN(); /* need "real" lock here to protect queue poll-wait linkage */
    /* TODO: this gives precedence to queue messages, which may starve tport messages 
        while both are arriving
     */
  #if GASNETC_USE_MAINQUEUE
    if (elan_queueHaveReq(gasnetc_mainqueue)) {
  #else
    if (elan_queueRxPoll(gasnetc_queuerx, 0)) {
  #endif
    gasnetc_bufdesc_t _desc;
    #if GASNETC_USE_MAINQUEUE || GASNET_PAR
      char _buf[GASNETC_ELAN_MAX_QUEUEMSG+8]; /* ensure 8-byte buf alignment */
      desc = &_desc;
      desc->buf = (gasnetc_buf_t *)( ((((uintptr_t)_buf) >> 3) << 3) + 8); 
      gasneti_assert((void *)&(desc->buf->m.msg) == (void *)desc->buf);
    #else
      desc = &_desc;
    #endif
    #if GASNETC_USE_MAINQUEUE 
      elan_queueWait(gasnetc_mainqueue, desc->buf, ELAN_POLL_EVENT);
    #elif GASNET_PAR
      /* need to memcpy message out of queue if we want to allow AM concurrency */
      /* TODO: we can perform a smarter memcpy than libelan by inspecting the message */
      elan_queueRxWait(gasnetc_queuerx, desc->buf, ELAN_POLL_EVENT);
    #else
      /* run from system buffer to avoid copy-out which always copies entire slot size */
      desc->buf = elan_queueRxWait(gasnetc_queuerx, NULL, ELAN_POLL_EVENT);
    #endif
      UNLOCK_ELAN();

      gasnetc_processPacket(desc);

    } else if ((desc = gasnetc_tportCheckRx())) {
      UNLOCK_ELAN();

      gasnetc_processPacket(desc);

      LOCK_ELAN(); /* need "real" lock here to protect Rx FIFO */
        /* set new recv and push on fifo */
        gasnetc_tportAddRxBuf(desc);
      UNLOCK_ELAN();
    } else { /* no more incoming msgs waiting */
      UNLOCK_ELAN();
      break;
    }
  }

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnetc_ReqRepGeneric)
int gasnetc_ReqRepGeneric(gasnetc_category_t category, int isReq,
                         int dest, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {
  char _shortbuf[GASNETC_ELAN_MAX_QUEUEMSG+GASNETI_MEDBUF_ALIGNMENT]; 
  gasnetc_bufdesc_t _descbuf; 
  gasnetc_bufdesc_t *desc = NULL;
  gasnetc_buf_t *buf = NULL;
  gasnet_handlerarg_t *pargs;
  int msgsz;
  gasneti_assert(numargs >= 0 && numargs <= GASNETC_MAX_ARGS);

  ASSERT_ELAN_UNLOCKED();

  switch (category) {
    case gasnetc_Short:
      { desc = &_descbuf;
        buf = (gasnetc_buf_t *)_shortbuf;
        desc->buf = buf;
        pargs = (gasnet_handlerarg_t *)(&(buf->m.msg)+1);
        msgsz = (uintptr_t)(pargs + numargs) - (uintptr_t)buf;
      }
    break;
    case gasnetc_Medium:
      { uint8_t *pdata;
        int actualargs = numargs + GASNETC_MEDHEADER_PADARG(numargs);
        msgsz = GASNETC_MED_HEADERSZ + (actualargs<<2) + nbytes;
        if (msgsz <= GASNETC_ELAN_MAX_QUEUEMSG) {
          desc = &_descbuf;
          buf = (gasnetc_buf_t *)GASNETI_ALIGNUP(_shortbuf,GASNETI_MEDBUF_ALIGNMENT);
          desc->buf = buf;
        }
        else {
          desc = gasnetc_tportGetTxBuf(dest != gasneti_mynode);
          buf = desc->buf;
        }
        pargs = (gasnet_handlerarg_t *)(&(buf->m.medmsg)+1);
        pdata = (uint8_t *)(pargs + actualargs);
        memcpy(pdata, source_addr, nbytes);
        buf->m.medmsg.nBytes = nbytes;
      }
    break;
    case gasnetc_Long:
      { desc = &_descbuf;
        buf = (gasnetc_buf_t *)_shortbuf;        
        desc->buf = buf;
        pargs = (gasnet_handlerarg_t *)(&(buf->m.longmsg)+1);
        buf->m.longmsg.nBytes = nbytes;
        buf->m.longmsg.destLoc = (uintptr_t)dest_ptr;
        msgsz = (uintptr_t)(pargs + numargs) - (uintptr_t)buf;
      }
    break;
    default: gasneti_fatalerror("unrecognized msg category");
  }
  GASNETC_MSG_SETFLAGS(&(buf->m.msg), isReq, category, numargs);
  buf->m.msg.handlerId = handler;
  buf->m.msg.sourceId = gasneti_mynode;
  { int i;
    for(i=0; i < numargs; i++) {
      pargs[i] = (gasnet_handlerarg_t)va_arg(argptr, int);
    }
  }

#if GASNET_PSHM
  /* PSHM path includes loopback */
  gasneti_assert(dest != gasneti_mynode);
#else
  if (dest == gasneti_mynode) {
    if (category == gasnetc_Long) memcpy(dest_ptr, source_addr, nbytes);
    gasnetc_processPacket(desc);
    if (desc != &_descbuf) {
      gasneti_assert(msgsz > GASNETC_ELAN_MAX_QUEUEMSG);
      gasnetc_tportReleaseTxBuf(desc);
    }
  } else
#endif
  {
    LOCK_ELAN_WEAK();
      if (category == gasnetc_Long && nbytes > 0) {
        /* do put and block for completion */
        ELAN_EVENT *putevt;
        void *bouncebuf = NULL;

        if (GASNETC_IS_SMALLPUT(nbytes) ||
            gasnetc_elan_addressable(source_addr, nbytes)) {
          /* safe to put directly from source */
          putevt = gasnete_elan_put(source_addr, dest_ptr, nbytes, dest);
          UNLOCKRELOCK_ELAN_WEAK_IFTRACE(GASNETI_TRACE_EVENT_VAL(C,AMLONG_DIRECT,nbytes));
        } else { /* need to use a bounce buffer */
          /* TODO: this may fail for unmapped segment under GASNET_SEGMENT_EVERYTHING */
          gasneti_assert(elan_addressable(STATE(), dest_ptr, nbytes));
          /* would be nice to use SDRAM here, but put interface cannot handle it... */
          #if GASNETC_PREALLOC_AMLONG_BOUNCEBUF
            bouncebuf = *gasnetc_mythread(); /* core entry is first in struct */
            gasneti_assert(bouncebuf);
          #else
            bouncebuf = elan_allocMain(STATE(), 64, nbytes);
            if (!bouncebuf) /* if we run out of mem here, we're in trouble */
              gasneti_fatalerror("Failed to elan_allocMain() %i bytes in gasnetc_ReqRepGeneric",
                nbytes);
          #endif
          memcpy(bouncebuf, source_addr, nbytes);
          putevt = gasnete_elan_put(bouncebuf, dest_ptr, nbytes, dest);
          UNLOCKRELOCK_ELAN_WEAK_IFTRACE(GASNETI_TRACE_EVENT_VAL(C,AMLONG_BUFFERED,nbytes));
        }
        /* loop until put is complete (required to ensure ordering semantics) 
           could make this totally asynchronous with lots more work, 
           but this isn't that bad because the put DMA is totally one-sided
         */
        while (!elan_poll(putevt, GASNETC_ELAN_POLLITERS_AM)) {
          UNLOCKRELOCK_ELAN_WEAK(gasneti_AMPoll());
        }
        #if !GASNETC_PREALLOC_AMLONG_BOUNCEBUF
          if (bouncebuf) elan_free(STATE(), bouncebuf);
        #endif
      }

      if (msgsz <= GASNETC_ELAN_MAX_QUEUEMSG) {
        gasneti_assert(desc == &_descbuf);
      #if GASNETC_USE_MAINQUEUE
        elan_queueReq(gasnetc_mainqueue, dest, &(buf->m.msg), msgsz);
      #else
        { ELAN_EVENT *evt; 
          evt = elan_queueTx(gasnetc_queuetx, dest, &(buf->m.msg), msgsz, ELAN_RAIL_ALL);
        #if GASNETC_OVERLAP_AMQUEUE && !GASNET_PAR
          /* TODO - add per-thread AM evtbins? */
          if (!elan_poll(evt, GASNETC_ELAN_POLLITERS_AM)) 
            gasnete_evtbin_save(&gasnetc_am_evtbin, evt);
        #else
          { /* poll-block for elan_queueTx completion */
            while (!elan_poll(evt, GASNETC_ELAN_POLLITERS_AM)) { 
              UNLOCKRELOCK_ELAN_WEAK(gasneti_AMPoll());
            }
          }
        #endif
        }
      #endif
      }
      else {
        desc->event = elan_tportTxStart(TPORT(), 0, dest, 
                                        gasneti_mynode, 0, 
                                        &(buf->m.medmsg), msgsz);
      }
    UNLOCK_ELAN_WEAK();
  }
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_RequestGeneric(gasnetc_category_t category, 
                         int dest, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {

  gasneti_AMPoll(); /* ensure progress */

#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    return gasneti_AMPSHM_RequestGeneric(category, dest, handler,
                                         source_addr, nbytes, dest_ptr,
                                         numargs, argptr);
  }
#endif

  return gasnetc_ReqRepGeneric(category, 1, dest, handler, 
                               source_addr, nbytes, dest_ptr, 
                               numargs, argptr); 
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_ReplyGeneric(gasnetc_category_t category, 
                         gasnet_token_t token, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr) {
  gasnetc_bufdesc_t *reqdesc = (gasnetc_bufdesc_t *)token;
  int retval;

#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    return gasneti_AMPSHM_ReplyGeneric(category, token, handler,
                                       source_addr, nbytes, dest_ptr,
                                       numargs, argptr);
  }
#endif

  gasneti_assert(reqdesc->handlerRunning);
  gasneti_assert(!reqdesc->replyIssued);
  gasneti_assert(GASNETC_MSG_ISREQUEST(&(reqdesc->buf->m.msg)));
  
  retval = gasnetc_ReqRepGeneric(category, 0, reqdesc->buf->m.msg.sourceId, handler, 
                                 source_addr, nbytes, dest_ptr, 
                                 numargs, argptr); 

  reqdesc->replyIssued = 1;
  return retval;
}
/* ------------------------------------------------------------------------------------ */

