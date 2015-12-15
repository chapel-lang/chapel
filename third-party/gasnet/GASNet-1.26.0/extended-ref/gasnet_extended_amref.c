/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_amref.c $
 * Description: GASNet Extended API Reference Implementation: AM-base Get/Put/Memset
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_handler.h>

/* 
 * Guidance for conduit writers.
 * (See also "Tuning Parameters", below).
 *
 * Conduits are NOT expected to clone this file.
 * Instead they may #include it or not at their discretion.
 * A conduit may select which portions of the code are built using
 * either fine-grained or coarse-grained controls.
 *
 * The coarse-grained controls just define the fine-grained ones:
 *
 *    #define GASNETE_USING_REF_EXTENDED_GET_BULK 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_GET_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_GET_BULK 1
 *
 *    #define GASNETE_USING_REF_EXTENDED_PUT_BULK 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_PUT_BULK 1
 *
 *    #define GASNETE_USING_REF_EXTENDED_PUT 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_PUT 1
 *
 *    #define GASNETE_USING_REF_EXTENDED_MEMSET 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_MEMSET_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_MEMSET 1
 *
 * The fine-grained controls:
 *
 *    #define GASNETE_BUILD_AMREF_GET_BULK 1
 *      To build
 *         gasnete_amref_get_nb_bulk
 *         gasnete_amref_get_nbi_bulk
 *
 *    #define GASNETE_BUILD_AMREF_PUT_BULK 1
 *      To build
 *         gasnete_amref_put_nb_bulk
 *         gasnete_amref_put_nbi_bulk
 *
 *    #define GASNETE_BUILD_AMREF_PUT 1
 *      To build
 *         gasnete_amref_put_nb
 *         gasnete_amref_put_nbi
 *
 *    #define GASNETE_BUILD_AMREF_MEMSET 1
 *      To build
 *         gasnete_amref_memset_nb
 *         gasnete_amref_memset_nbi
 *
 *    #define GASNETE_BUILD_AMREF_GET_HANDLERS 1
 *      To build the corresponding supporting AM handlers
 *
 *    #define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
 *      To build the corresponding supporting AM handlers (bulk and non-bulk)
 *
 *    #define GASNETE_BUILD_AMREF_MEMSET_HANDLERS 1
 *      To build the corresponding supporting AM handlers
 *
 * If one does not define ANY of those, then this file contains no code.
 *
 * A conduit can use these directly by using one or more of the following
 * in its gasnet_extended_fwd.h name-shift the functions listed above:
 *    #define gasnete_amref_get_nb_bulk   gasnete_get_nb_bulk
 *    #define gasnete_amref_get_nbi_bulk  gasnete_get_nbi_bulk
 *    #define gasnete_amref_put_nb_bulk   gasnete_put_nb_bulk
 *    #define gasnete_amref_put_nbi_bulk  gasnete_put_nbi_bulk
 *    #define gasnete_amref_put_nb        gasnete_put_nb
 *    #define gasnete_amref_put_nbi       gasnete_put_nbi
 *    #define gasnete_amref_memset_nb     gasnete_memset_nb
 *    #define gasnete_amref_memset_nbi    gasnete_memset_nbi
 *
 * When not using those defines a conduit can call the functions in this
 * file from its own gasnete_{get,put,memset}*() as needed (for instance
 * for dealing with out-of-segment arguments which cannot be dealt with
 * using native RDMA mechanisms).
 *
 * By default, the code here uses counter increments directly when one
 * has defined GASNETE_EOP_COUNTED, and calls to gasnete_op_markdone()
 * for the (default) GASNETE_EOP_BOOLEAN case.  If a conduit defines
 * GASNETE_AMREF_USE_MARKDONE to 1, then this code will use calls to
 * gasnete_op_markdone() unconditionally.
 *
 * NOTE: If tempted to clone this file into your conduit in order to get
 * finer-grained control over what is build, then instead *please* email
 * upc-devel@lbl.gov to request even finer-grained GASNETE_BUILD_AMREF_*.
 */

/* ------------------------------------------------------------------------------------ */
/* Implement the coarse-grained build controls: */

#if GASNETE_USING_REF_EXTENDED_GET_BULK
#define GASNETE_BUILD_AMREF_GET_HANDLERS 1
#define GASNETE_BUILD_AMREF_GET_BULK 1
#endif
#if GASNETE_USING_REF_EXTENDED_PUT_BULK
#define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
#define GASNETE_BUILD_AMREF_PUT_BULK 1
#endif
#if GASNETE_USING_REF_EXTENDED_PUT
#define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
#define GASNETE_BUILD_AMREF_PUT 1
#endif
#if GASNETE_USING_REF_EXTENDED_MEMSET
#define GASNETE_BUILD_AMREF_MEMSET_HANDLERS 1
#define GASNETE_BUILD_AMREF_MEMSET 1
#endif

/* ------------------------------------------------------------------------------------ */
/*
 * Design/Approach for gets/puts in Extended Reference API in terms of Core
 * ========================================================================
 *
 * The extended API implements gasnet_put and gasnet_put_nbi differently, 
 * all in terms of 'nbytes', the number of bytes to be transferred as 
 * payload.
 *
 * The core usually implements AMSmall and AMMedium as host-side copies and
 * AMLongs are implemented according to the implementation.  Some conduits 
 * may optimize AMLongRequest/AMLongRequestAsync/AMLongReply with DMA
 * operations.
 *
 * gasnet_put(_bulk) is translated to a gasnete_put_nb(_bulk) + sync
 * gasnet_get(_bulk) is translated to a gasnete_get_nb(_bulk) + sync
 *
 * gasnete_put_nb(_bulk) translates to
 *    if nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMMedium(payload)
 *  #if GASNETE_USE_LONG_PUTS
 *    else if nbytes < AMMaxLongRequest
 *      AMLongRequest(payload)
 *  #endif
 *    else
 *      gasnete_put_nbi(_bulk)(payload)
 *
 * gasnete_get_nb(_bulk) translates to
 *    if nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMSmall request + AMMedium(payload) reply
 *    else
 *      gasnete_get_nbi(_bulk)()
 *
 * gasnete_put_nbi(_bulk) translates to
 *    if nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMMedium(payload)
 *  #if GASNETE_USE_LONG_PUTS
 *    else if nbytes < AMMaxLongRequest
 *      AMLongRequest(payload)
 *    else
 *      chunks of AMMaxLongRequest with AMLongRequest()
 *      AMLongRequestAsync is used instead of AMLongRequest for put_bulk
 *  #else
 *    else
 *      chunks of AMMaxMedium with AMMediumRequest()
 *  #endif
 *
 * gasnete_get_nbi(_bulk) translates to
 *    if nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
 *      AMSmall request + AMMedium(payload) reply
 *    else
 *      chunks of AMMaxMedium with AMSmall request + AMMedium() reply
 *
 * The current implementation uses AMLongs for large puts because the 
 * destination is guaranteed to fall within the registered GASNet segment.
 * This can be disabled by setting GASNETE_USE_LONG_PUTS to zero.
 *
 * The spec allows gets to be received anywhere into the virtual memory space,
 * so we can only use AMLong when the destination happens to fall within the 
 * segment - GASNETE_USE_LONG_GETS indicates whether or not we should try to do this.
 * (conduits which can support AMLongs to areas outside the segment
 * could improve on this through the use of this conduit-specific information).
 * 
 */

/* ------------------------------------------------------------------------------------ */
/*
   Tuning Parameters
   =================
   Conduits may choose to override the default tuning parameters below by defining them
   in their gasnet_core_fwd.h.  See the Design description above for how to use these.
 */

/* the size threshold where gets/puts stop using medium messages and start using longs */
#ifndef GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gasnet_AMMaxMedium()
#endif

/* true if we should try to use Long replies in gets (only possible if dest falls in segment) */
#ifndef GASNETE_USE_LONG_GETS
#define GASNETE_USE_LONG_GETS 1
#endif

/* true if we should try to use Long requests in puts */
#ifndef GASNETE_USE_LONG_PUTS
#define GASNETE_USE_LONG_PUTS 1
#endif

/* ------------------------------------------------------------------------------------ */
/*
  AM handlers
  ===========
*/

#if GASNETE_EOP_COUNTED && !GASNETE_AMREF_USE_MARKDONE
  #define PACK_EOP_DONE(_eop)         PACK(&(_eop)->completed_cnt)
  #define PACK_IOP_DONE(_iop,_getput) PACK(&(_iop)->completed_##_getput##_cnt)
  #define MARK_DONE(_ptr,_isget)      gasneti_weakatomic_increment((gasneti_weakatomic_t *)(_ptr), \
                                                                   (_isget) ? GASNETI_ATOMIC_REL : 0)
#else
  #define PACK_EOP_DONE(_eop)         PACK(_eop)
  #define PACK_IOP_DONE(_iop,_getput) PACK(_iop)
  #define MARK_DONE(_ptr,_isget)      do {                                                      \
                                         if (_isget) gasneti_sync_writes();                     \
                                         gasnete_op_markdone((gasnete_op_t *)(_ptr), (_isget)); \
                                      } while (0)
#endif

/* ------------------------------------------------------------------------------------ */
#if GASNETE_BUILD_AMREF_GET_HANDLERS

GASNETI_INLINE(gasnete_amref_get_reqh_inner)
void gasnete_amref_get_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t nbytes, void *dest, void *src, void *done) {
  gasneti_assert(nbytes <= gasnet_AMMaxMedium());
  GASNETI_SAFE(
    MEDIUM_REP(2,4,(token, gasneti_handleridx(gasnete_amref_get_reph),
                  src, nbytes, 
                  PACK(dest), PACK(done))));
}
SHORT_HANDLER(gasnete_amref_get_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

GASNETI_INLINE(gasnete_amref_get_reph_inner)
void gasnete_amref_get_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *dest, void *done) {
  GASNETE_FAST_UNALIGNED_MEMCPY(dest, addr, nbytes);
  MARK_DONE(done,1);
}
MEDIUM_HANDLER(gasnete_amref_get_reph,2,4,
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)    ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));

GASNETI_INLINE(gasnete_amref_getlong_reqh_inner)
void gasnete_amref_getlong_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t nbytes, void *dest, void *src, void *done) {

  GASNETI_SAFE(
    LONG_REP(1,2,(token, gasneti_handleridx(gasnete_amref_getlong_reph),
                  src, nbytes, dest,
                  PACK(done))));
}

SHORT_HANDLER(gasnete_amref_getlong_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

GASNETI_INLINE(gasnete_amref_getlong_reph_inner)
void gasnete_amref_getlong_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes, 
  void *done) {
  MARK_DONE(done,1);
}
LONG_HANDLER(gasnete_amref_getlong_reph,1,2,
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));

#endif /* GASNETE_BUILD_AMREF_GET_HANDLERS */
/* ------------------------------------------------------------------------------------ */
#if GASNETE_BUILD_AMREF_PUT_HANDLERS

GASNETI_INLINE(gasnete_amref_put_reqh_inner)
void gasnete_amref_put_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *dest, void *done) {
  GASNETE_FAST_UNALIGNED_MEMCPY(dest, addr, nbytes);
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_amref_markdone_reph),
                  PACK(done))));
}
MEDIUM_HANDLER(gasnete_amref_put_reqh,2,4, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)     ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));

GASNETI_INLINE(gasnete_amref_putlong_reqh_inner)
void gasnete_amref_putlong_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *done) {
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_amref_markdone_reph),
                  PACK(done))));
}
LONG_HANDLER(gasnete_amref_putlong_reqh,1,2, 
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));

#endif /* GASNETE_BUILD_AMREF_PUT_HANDLERS */
/* ------------------------------------------------------------------------------------ */
#if GASNETE_BUILD_AMREF_MEMSET_HANDLERS

GASNETI_INLINE(gasnete_amref_memset_reqh_inner)
void gasnete_amref_memset_reqh_inner(gasnet_token_t token, 
  gasnet_handlerarg_t val, void *nbytes_arg, void *dest, void *done) {
  size_t nbytes = (uintptr_t)nbytes_arg;
  memset(dest, (int)(uint32_t)val, nbytes);
  gasneti_sync_writes();
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_amref_markdone_reph),
                  PACK(done))));
}
SHORT_HANDLER(gasnete_amref_memset_reqh,4,7,
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

#endif /* GASNETE_BUILD_AMREF_MEMSET_HANDLERS */
/* ------------------------------------------------------------------------------------ */
#if GASNETE_BUILD_AMREF_PUT_HANDLERS || GASNETE_BUILD_AMREF_MEMSET_HANDLERS

GASNETI_INLINE(gasnete_amref_markdone_reph_inner)
void gasnete_amref_markdone_reph_inner(gasnet_token_t token, 
  void *done) {
  MARK_DONE(done,0);
}
SHORT_HANDLER(gasnete_amref_markdone_reph,1,2,
              (token, UNPACK(a0)    ),
              (token, UNPACK2(a0, a1)));

#endif /* GASNETE_BUILD_AMREF_PUT_HANDLERS || GASNETE_BUILD_AMREF_MEMSET_HANDLERS */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit handle)
  ==========================================================
*/

/* Forward declarations of _nbi for (potential) use by _nb */
#if GASNETE_BUILD_AMREF_GET_BULK
extern void gasnete_amref_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif
#if GASNETE_BUILD_AMREF_PUT_BULK
extern void gasnete_amref_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif
#if GASNETE_BUILD_AMREF_PUT
extern void gasnete_amref_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG);
#endif

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_GET_BULK
extern gasnet_handle_t gasnete_amref_get_nb_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_GET(UNALIGNED,H);
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    GASNETI_SAFE(
      SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_amref_get_reqh), 
                   (gasnet_handlerarg_t)nbytes, PACK(dest), PACK(src), PACK_EOP_DONE(op))));

    return (gasnet_handle_t)op;
  } else {
    /* TODO: don't need the iop for large xfers in the GASNETE_EOP_COUNTED case */
    /*  need many messages - use an access region to coalesce them into a single handle */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    gasnete_begin_nbi_accessregion(1 /* enable recursion */ GASNETE_THREAD_PASS);
    gasnete_amref_get_nbi_bulk(dest, node, src, nbytes GASNETE_THREAD_PASS);
    return gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
  }
}
#endif /* GASNETE_BUILD_AMREF_GET_BULK */

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_PUT_BULK || GASNETE_BUILD_AMREF_PUT
GASNETI_INLINE(gasnete_amref_put_nb_inner)
gasnet_handle_t gasnete_amref_put_nb_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    GASNETI_SAFE(
      MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_amref_put_reqh),
                    src, nbytes,
                    PACK(dest), PACK_EOP_DONE(op))));

    return (gasnet_handle_t)op;
#if GASNETE_USE_LONG_PUTS
  } else if (nbytes <= gasnet_AMMaxLongRequest()) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

    if (isbulk) {
      GASNETI_SAFE(
        LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                    src, nbytes, dest,
                    PACK_EOP_DONE(op))));
    } else {
      GASNETI_SAFE(
        LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                    src, nbytes, dest,
                    PACK_EOP_DONE(op))));
    }

    return (gasnet_handle_t)op;
#endif
  } else { 
    /* TODO: don't need the iop for large xfers in the GASNETE_EOP_COUNTED case */
    /*  need many messages - use an access region to coalesce them into a single handle */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    gasnete_begin_nbi_accessregion(1 /* enable recursion */ GASNETE_THREAD_PASS);
    #if GASNETE_BUILD_AMREF_PUT_BULK && GASNETE_BUILD_AMREF_PUT
      if (isbulk) gasnete_amref_put_nbi_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
      else        gasnete_amref_put_nbi    (node, dest, src, nbytes GASNETE_THREAD_PASS);
    #elif GASNETE_BUILD_AMREF_PUT_BULK
                  gasnete_amref_put_nbi_bulk(node, dest, src, nbytes GASNETE_THREAD_PASS);
    #else
                  gasnete_amref_put_nbi    (node, dest, src, nbytes GASNETE_THREAD_PASS);
    #endif
    return gasnete_end_nbi_accessregion(GASNETE_THREAD_PASS_ALONE);
  }
}
#endif /* GASNETE_BUILD_AMREF_PUT_BULK || GASNETE_BUILD_AMREF_PUT */

#if GASNETE_BUILD_AMREF_PUT
extern gasnet_handle_t gasnete_amref_put_nb      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,H);
  return gasnete_amref_put_nb_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}
#endif /* GASNETE_BUILD_AMREF_PUT */

#if GASNETE_BUILD_AMREF_PUT_BULK
extern gasnet_handle_t gasnete_amref_put_nb_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,H);
  return gasnete_amref_put_nb_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}
#endif /* GASNETE_BUILD_AMREF_PUT_BULK */

/* ------------------------------------------------------------------------------------ */

#ifdef GASNETE_BUILD_AMREF_MEMSET
extern gasnet_handle_t gasnete_amref_memset_nb   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
 GASNETI_CHECKPSHM_MEMSET(H);
 {
  gasnete_eop_t *op = gasnete_eop_new(GASNETE_MYTHREAD);

  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_amref_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK_EOP_DONE(op))));

  return (gasnet_handle_t)op;
 }
}
#endif /* GASNETE_BUILD_AMREF_MEMSET */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit handle)
  ==========================================================
  each message sends an ack - we count the number of implicit ops launched and compare
    with the number acknowledged
  Another possible design would be to eliminate some of the acks (at least for puts) 
    by piggybacking them on other messages (like get replies) or simply aggregating them
    the target until the source tries to synchronize
*/

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_GET_BULK
extern void gasnete_amref_get_nbi_bulk (void *dest, gasnet_node_t node, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  GASNETI_CHECKPSHM_GET(UNALIGNED,V);
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_get_cnt++;
  
    GASNETI_SAFE(
      SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_amref_get_reqh), 
                   (gasnet_handlerarg_t)nbytes, PACK(dest), PACK(src), PACK_IOP_DONE(op,get))));
    return;
  } else {
    size_t chunksz;
    gasnet_handler_t reqhandler;
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    #if GASNETE_USE_LONG_GETS
      gasneti_memcheck(gasneti_seginfo);
      if (gasneti_in_segment(gasneti_mynode, dest, nbytes)) {
        chunksz = gasnet_AMMaxLongReply();
        reqhandler = gasneti_handleridx(gasnete_amref_getlong_reqh);
      }
      else 
    #endif
      { reqhandler = gasneti_handleridx(gasnete_amref_get_reqh);
        chunksz = gasnet_AMMaxMedium();
      }
    for (;;) {
      op->initiated_get_cnt++;
      if (nbytes > chunksz) {
        GASNETI_SAFE(
          SHORT_REQ(4,7,(node, reqhandler, 
                       (gasnet_handlerarg_t)chunksz, PACK(pdest), PACK(psrc), PACK_IOP_DONE(op,get))));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        GASNETI_SAFE(
          SHORT_REQ(4,7,(node, reqhandler, 
                       (gasnet_handlerarg_t)nbytes, PACK(pdest), PACK(psrc), PACK_IOP_DONE(op,get))));
        break;
      }
    }
    return;
  }
}
#endif /* GASNETE_BUILD_AMREF_GET_BULK */

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_PUT_BULK || GASNETE_BUILD_AMREF_PUT
GASNETI_INLINE(gasnete_amref_put_nbi_inner)
void gasnete_amref_put_nbi_inner(gasnet_node_t node, void *dest, void *src, size_t nbytes, int isbulk GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;

  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_put_cnt++;

    GASNETI_SAFE(
      MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_amref_put_reqh),
                    src, nbytes,
                    PACK(dest), PACK_IOP_DONE(op,put))));
    return;
  } else
#if GASNETE_USE_LONG_PUTS
  if (nbytes <= gasnet_AMMaxLongRequest()) {
    op->initiated_put_cnt++;

    if (isbulk) {
      GASNETI_SAFE(
        LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                      src, nbytes, dest,
                      PACK_IOP_DONE(op,put))));
    } else {
      GASNETI_SAFE(
        LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                      src, nbytes, dest,
                      PACK_IOP_DONE(op,put))));
    }

    return;
  } else {
    const size_t chunksz = gasnet_AMMaxLongRequest();
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    for (;;) {
      op->initiated_put_cnt++;
      if (nbytes > chunksz) {
        if (isbulk) {
          GASNETI_SAFE(
            LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                          psrc, chunksz, pdest,
                          PACK_IOP_DONE(op,put))));
        } else {
          GASNETI_SAFE(
            LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                          psrc, chunksz, pdest,
                          PACK_IOP_DONE(op,put))));
        }
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        if (isbulk) {
          GASNETI_SAFE(
            LONGASYNC_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                          psrc, nbytes, pdest,
                          PACK_IOP_DONE(op,put))));
        } else {
          GASNETI_SAFE(
            LONG_REQ(1,2,(node, gasneti_handleridx(gasnete_amref_putlong_reqh),
                          psrc, nbytes, pdest,
                          PACK_IOP_DONE(op,put))));
        }
        break;
      }
    }
    return;
  }
#else /* ! GASNETE_USE_LONG_PUTS */
  {
    const size_t chunksz = gasnet_AMMaxMedium();
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    for (;;) {
      op->initiated_put_cnt++;
      if (nbytes > chunksz) {
        GASNETI_SAFE(
          MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_amref_put_reqh),
                          psrc, chunksz, PACK(pdest),
                          PACK_IOP_DONE(op,put))));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        GASNETI_SAFE(
          MEDIUM_REQ(2,4,(node, gasneti_handleridx(gasnete_amref_put_reqh),
                          psrc, nbytes, PACK(pdest),
                          PACK_IOP_DONE(op,put))));
        break;
      }
    }
  }
#endif /* GASNETE_USE_LONG_PUTS */
}
#endif /* GASNETE_BUILD_AMREF_PUT_BULK || GASNETE_BUILD_AMREF_PUT */

#if GASNETE_BUILD_AMREF_PUT
extern void gasnete_amref_put_nbi      (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(ALIGNED,V);
  gasnete_amref_put_nbi_inner(node, dest, src, nbytes, 0 GASNETE_THREAD_PASS);
}
#endif /* GASNETE_BUILD_AMREF_PUT */

#if GASNETE_BUILD_AMREF_PUT_BULK
extern void gasnete_amref_put_nbi_bulk (gasnet_node_t node, void *dest, void *src, size_t nbytes GASNETE_THREAD_FARG) {
  GASNETI_CHECKPSHM_PUT(UNALIGNED,V);
  gasnete_amref_put_nbi_inner(node, dest, src, nbytes, 1 GASNETE_THREAD_PASS);
}
#endif /* GASNETE_BUILD_AMREF_PUT_BULK */

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_MEMSET
extern void gasnete_amref_memset_nbi   (gasnet_node_t node, void *dest, int val, size_t nbytes GASNETE_THREAD_FARG) {
  gasnete_threaddata_t * const mythread = GASNETE_MYTHREAD;
  gasnete_iop_t *op = mythread->current_iop;
  GASNETI_CHECKPSHM_MEMSET(V);

  op->initiated_put_cnt++;

  GASNETI_SAFE(
    SHORT_REQ(4,7,(node, gasneti_handleridx(gasnete_amref_memset_reqh),
                 (gasnet_handlerarg_t)val, PACK(nbytes),
                 PACK(dest), PACK_IOP_DONE(op,put))));
}
#endif /* GASNETE_BUILD_AMREF_MEMSET */

/* ------------------------------------------------------------------------------------ */

void gasnete_check_config_amref(void) {
#if GASNETE_BUILD_AMREF_GET_BULK || GASNETE_BUILD_AMREF_PUT_BULK || GASNETE_BUILD_AMREF_PUT
  /* This ensures chunks sent as Medium payloads don't exceed the maximum */
  gasneti_assert_always(GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD <= gasnet_AMMaxMedium());
#endif

#if GASNETE_BUILD_AMREF_GET_BULK
  /* These ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
  gasneti_assert_always(gasnet_AMMaxMedium() <= (size_t)0xffffffff);
 #if GASNETE_USE_LONG_GETS
  gasneti_assert_always(gasnet_AMMaxLongReply() <= (size_t)0xffffffff);
 #endif
#endif
}
