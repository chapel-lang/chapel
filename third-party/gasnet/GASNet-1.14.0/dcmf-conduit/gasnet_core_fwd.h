/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2009/10/28 00:09:02 $
 * $Revision: 1.5 $
 * Description: GASNet header for dcmf conduit core (forward definitions)
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>
 *                 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.0
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         DCMF
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_DCMF 1

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
 #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
 #define GASNET_ALIGNED_SEGMENTS   1
#endif

/* conduit allows internal GASNet fns to issue put/get for remote addrs out of segment */
#define GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET 1


  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
#if 0
#define GASNETI_CONDUIT_THREADS 1
#endif

  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
/*#if 0
#define GASNETC_USE_INTERRUPTS 1
#endif
*/
void gasnetc_myFatalSignalCallback(int id);
#define GASNETC_FATALSIGNAL_CALLBACK(SIG) gasnetc_myFatalSignalCallback((SIG))

  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME) \
VAL(C, NACK_LIST_LEN, len) \
CNT(C, AMREQ_REJECT, cnt) \
VAL(C, AMREQ_RESEND_COUNT, cnt) \
CNT(C, DCMF_NACK_SENT, cnt) \
CNT(C, DCMF_ACK_SENT, cnt) \
VAL(C, DCMF_POLL_NUM_PROCESSED, cnt) \
CNT(C, DCMF_POLL_CALLS, cnt) \
VAL(C, AMHANDLER_LIST_LEN, len) \
TIME(C, GET_REPLAY_BUFFER_STALL, waittime) \
TIME(C, REQ_AMHANDLER_WAIT, waittime) \
TIME(C, REP_AMHANDLER_WAIT, waittime) 
#endif
