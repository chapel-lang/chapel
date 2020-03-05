/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_amref.c $
 * Description: GASNet Extended API Reference Implementation: AM-based Get/Put
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

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
 *    #define GASNETE_USING_REF_EXTENDED_GET 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_GET_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_GET 1
 *
 *    #define GASNETE_USING_REF_EXTENDED_PUT 1
 *      Is equivalent to
 *         #define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
 *         #define GASNETE_BUILD_AMREF_PUT 1
 *
 * The fine-grained controls:
 *
 *    #define GASNETE_BUILD_AMREF_GET 1
 *      To build
 *         gasnete_amref_get_nb
 *         gasnete_amref_get_nbi
 *
 *    #define GASNETE_BUILD_AMREF_PUT 1
 *      To build
 *         gasnete_amref_put_nb
 *         gasnete_amref_put_nbi
 *
 *    #define GASNETE_BUILD_AMREF_GET_HANDLERS 1
 *      To build the corresponding supporting AM handlers
 *
 *    #define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
 *      To build the corresponding supporting AM handlers
 *
 * If one does not define ANY of those, then this file contains no code.
 *
 * A conduit can use these directly by using one or more of the following
 * in its gasnet_extended_fwd.h name-shift the functions listed above:
 *    #define gasnete_amref_get_nb        gasnete_get_nb
 *    #define gasnete_amref_get_nbi       gasnete_get_nbi
 *    #define gasnete_amref_put_nb        gasnete_put_nb
 *    #define gasnete_amref_put_nbi       gasnete_put_nbi
 *
 * When not using those defines a conduit can call the functions in this
 * file from its own gasnete_{get,put}_{nb,nbi}() as needed (for instance
 * for dealing with out-of-segment arguments which cannot be dealt with
 * using native RDMA mechanisms).
 *
 * NOTE: If tempted to clone this file into your conduit in order to get
 * finer-grained control over what is build, then instead *please* email
 * gasnet-devel@lbl.gov to request even finer-grained GASNETE_BUILD_AMREF_*.
 */

/* ------------------------------------------------------------------------------------ */
/* Implement the coarse-grained build controls: */

#if GASNETE_USING_REF_EXTENDED_GET
#define GASNETE_BUILD_AMREF_GET_HANDLERS 1
#define GASNETE_BUILD_AMREF_GET 1
#endif
#if GASNETE_USING_REF_EXTENDED_PUT
#define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
#define GASNETE_BUILD_AMREF_PUT 1
#endif

// TODO-EX: remove these legacy checks
#ifdef GASNETE_USING_REF_EXTENDED_GET_BULK
#error "out-of-date #define of GASNETE_USING_REF_EXTENDED_GET_BULK"
#endif
#ifdef GASNETE_BUILD_AMREF_GET_BULK
#error "out-of-date #define of GASNETE_BUILD_AMREF_GET_BULK"
#endif
#ifdef GASNETE_USING_REF_EXTENDED_PUT_BULK
#error "out-of-date #define of GASNETE_USING_REF_EXTENDED_PUT_BULK"
#endif
#ifdef GASNETE_BUILD_AMREF_PUT_BULK
#error "out-of-date #define of GASNETE_BUILD_AMREF_PUT_BULK"
#endif
#ifdef GASNETE_USING_REF_EXTENDED_MEMSET
#error "out-of-date #define of GASNETE_USING_REF_EXTENDED_MEMSET"
#endif
#ifdef GASNETE_BUILD_AMREF_MEMSET_HANDLERS
#error "out-of-date #define of GASNETE_BUILD_AMREF_MEMSET_HANDLERS"
#endif
#ifdef GASNETE_BUILD_AMREF_MEMSET
#error "out-of-date #define of GASNETE_BUILD_AMREF_MEMSET"
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
 * TODO-EX: update w.r.t. loss of _bulk
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
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gex_AM_LUBRequestMedium()
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

#ifndef PACK_EOP_DONE
  #define PACK_EOP_DONE(_eop)         PACK(_eop)
#endif
#ifndef PACK_IOP_DONE
  #define PACK_IOP_DONE(_iop,_getput) PACK(_iop)
#endif
#ifndef MARK_DONE
  #define MARK_DONE(_ptr,_isget)      do {                                                      \
                                         if (_isget) gasneti_sync_writes();                     \
                                         gasnete_op_markdone((gasnete_op_t *)(_ptr), (_isget)); \
                                      } while (0)
#endif

/* ------------------------------------------------------------------------------------ */
/* Forward declarations */
#if GASNETE_BUILD_AMREF_GET
int gasnete_amref_get_nbi( gex_TM_t tm,
                           void *dest,
                           gex_Rank_t rank, void *src,
                           size_t nbytes,
                           gex_Flags_t flags GASNETI_THREAD_FARG);
#endif

/* ------------------------------------------------------------------------------------ */
#if GASNETE_BUILD_AMREF_GET_HANDLERS

GASNETI_INLINE(gasnete_amref_get_reqh_inner)
void gasnete_amref_get_reqh_inner(gex_Token_t token,
  gex_AM_Arg_t nbytes, void *dest, void *src, void *done) {
  gasneti_assert_uint(nbytes ,<=, gex_AM_LUBReplyMedium());
  gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_amref_get_reph),
                         src, nbytes, GEX_EVENT_NOW, 0,
                         PACK(dest), PACK(done));
}
SHORT_HANDLER(gasnete_amref_get_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

GASNETI_INLINE(gasnete_amref_get_reph_inner)
void gasnete_amref_get_reph_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *dest, void *done) {
  GASNETI_MEMCPY(dest, addr, nbytes);
  MARK_DONE(done,1);
}
MEDIUM_HANDLER(gasnete_amref_get_reph,2,4,
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)    ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));

GASNETI_INLINE(gasnete_amref_getlong_reqh_inner)
void gasnete_amref_getlong_reqh_inner(gex_Token_t token,
  gex_AM_Arg_t nbytes, void *dest, void *src, void *done) {

  gex_AM_ReplyLong(token, gasneti_handleridx(gasnete_amref_getlong_reph),
                       src, nbytes, dest, GEX_EVENT_NOW, 0, PACK(done));
}

SHORT_HANDLER(gasnete_amref_getlong_reqh,4,7, 
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));

GASNETI_INLINE(gasnete_amref_getlong_reph_inner)
void gasnete_amref_getlong_reph_inner(gex_Token_t token,
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
void gasnete_amref_put_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *dest, void *done) {
  GASNETI_MEMCPY(dest, addr, nbytes);
  gasneti_sync_writes();
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_amref_markdone_reph), 0, PACK(done));
}
MEDIUM_HANDLER(gasnete_amref_put_reqh,2,4, 
              (token,addr,nbytes, UNPACK(a0),      UNPACK(a1)     ),
              (token,addr,nbytes, UNPACK2(a0, a1), UNPACK2(a2, a3)));

GASNETI_INLINE(gasnete_amref_putlong_reqh_inner)
void gasnete_amref_putlong_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *done) {
  gasneti_sync_writes();
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_amref_markdone_reph), 0, PACK(done));
}
LONG_HANDLER(gasnete_amref_putlong_reqh,1,2, 
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));

GASNETI_INLINE(gasnete_amref_markdone_reph_inner)
void gasnete_amref_markdone_reph_inner(gex_Token_t token,
  void *done) {
  MARK_DONE(done,0);
}
SHORT_HANDLER(gasnete_amref_markdone_reph,1,2,
              (token, UNPACK(a0)    ),
              (token, UNPACK2(a0, a1)));

#endif /* GASNETE_BUILD_AMREF_PUT_HANDLERS */

/* ------------------------------------------------------------------------------------ */
/* Common logic for _nbi, also for use by _nb */
#if GASNETE_BUILD_AMREF_GET
GASNETI_INLINE(gasnete_amref_get_nbi_inner)
void gasnete_amref_get_nbi_inner(gex_TM_t tm,
                                 void *dest,
                                 gex_Rank_t rank, void *src,
                                 size_t nbytes,
                                 gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_get_cnt++;
  
    gex_AM_RequestShort(tm, rank, gasneti_handleridx(gasnete_amref_get_reqh), 0,
                   (gex_AM_Arg_t)nbytes, PACK(dest), PACK(src), PACK_IOP_DONE(op,get));
    return;
  } else {
    size_t chunksz;
    gex_AM_Index_t reqhandler;
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    #if GASNETE_USE_LONG_GETS
      gasneti_memcheck(gasneti_seginfo); // TODO-EX: more needed to ensure gasneti_in_segment() is "ready"?
      if (gasneti_in_segment(tm, gasneti_mynode, dest, nbytes)) {
        chunksz = gex_AM_LUBReplyLong();
        reqhandler = gasneti_handleridx(gasnete_amref_getlong_reqh);
      }
      else 
    #endif
      { reqhandler = gasneti_handleridx(gasnete_amref_get_reqh);
        chunksz = gex_AM_LUBReplyMedium(); // TODO-EX: _lub_ -> _max_
      }
    for (;;) {
      op->initiated_get_cnt++;
      if (nbytes > chunksz) {
        gex_AM_RequestShort(tm, rank, reqhandler, 0,
                       (gex_AM_Arg_t)chunksz, PACK(pdest), PACK(psrc), PACK_IOP_DONE(op,get));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        gex_AM_RequestShort(tm, rank, reqhandler, 0,
                       (gex_AM_Arg_t)nbytes, PACK(pdest), PACK(psrc), PACK_IOP_DONE(op,get));
        break;
      }
    }
    return;
  }
}
#endif
#if GASNETE_BUILD_AMREF_PUT
GASNETI_INLINE(gasnete_amref_put_nbi_inner)
int gasnete_amref_put_nbi_inner (gex_TM_t tm,
                                 gex_Rank_t rank, void *dest,
                                 void *src,
                                 size_t nbytes,
                                 gex_Event_t *lc_opt,
                                 gex_Flags_t flags
                                 GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const op = mythread->current_iop;

  // TODO-EX: flags?
  // Some may pass through to the AMReques call(s).
  // Others need some special treatment.
  // The case of IMMEDIATE, in particular, requires attention to avoid
  // returning from a partially-initiated xfer in the case that we loop.
  // Currently we are passing flags==0 to all AMRequest calls.

  // There is no EVENT_DEFER for an AMRequest, but EVENT_GROUP is permitted.
  // Since (at least in the reference iop) syncnbi_{puts,all}() will
  // test/wait the LC counters, we convert EVENT_DEFER to EVENT_GROUP here.
  if (lc_opt == GEX_EVENT_DEFER) lc_opt = GEX_EVENT_GROUP;

  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    op->initiated_put_cnt++;

    return
    gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_amref_put_reqh),
                             src, nbytes, lc_opt, 0,
                             PACK(dest), PACK_IOP_DONE(op,put));
  } else
#if GASNETE_USE_LONG_PUTS
  if (nbytes <= gex_AM_LUBRequestLong()) { // TODO-EX: _lub_ -> _max_
    op->initiated_put_cnt++;

    return
    gex_AM_RequestLong(tm, rank, gasneti_handleridx(gasnete_amref_putlong_reqh),
                           src, nbytes, dest, lc_opt, 0,
                           PACK_IOP_DONE(op,put));
  } else {
    const size_t chunksz = gex_AM_LUBRequestLong(); // TODO-EX: _lub_ -> _max_
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    for (;;) {
      op->initiated_put_cnt++;
      if (nbytes > chunksz) {
        gex_AM_RequestLong(tm, rank, gasneti_handleridx(gasnete_amref_putlong_reqh),
                               psrc, chunksz, pdest, lc_opt, 0,
                               PACK_IOP_DONE(op,put));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        gex_AM_RequestLong(tm, rank, gasneti_handleridx(gasnete_amref_putlong_reqh),
                               psrc, nbytes, pdest, lc_opt, 0,
                               PACK_IOP_DONE(op,put));
        break;
      }
    }
  }
#else /* ! GASNETE_USE_LONG_PUTS */
  {
    const size_t chunksz = gex_AM_LUBRequestMedium(); // TODO-EX: _lub_ -> _max_
    uint8_t *psrc = src;
    uint8_t *pdest = dest;
    for (;;) {
      op->initiated_put_cnt++;
      if (nbytes > chunksz) {
        gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_amref_put_reqh),
                                 psrc, chunksz, lc_opt, 0,
                                 PACK(pdest), PACK_IOP_DONE(op,put));
        nbytes -= chunksz;
        psrc += chunksz;
        pdest += chunksz;
      } else {
        gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_amref_put_reqh),
                                 psrc, nbytes, lc_opt, 0,
                                 PACK(pdest), PACK_IOP_DONE(op,put));
        break;
      }
    }
  }
#endif /* GASNETE_USE_LONG_PUTS */
  return 0;
}
#endif /* GASNETE_BUILD_AMREF_PUT */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (explicit event)
  ==========================================================
*/


#if GASNETE_BUILD_AMREF_GET
extern
gex_Event_t gasnete_amref_get_nb(
                     gex_TM_t tm,
                     void *dest,
                     gex_Rank_t rank, void *src,
                     size_t nbytes,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
    gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

    gex_AM_RequestShort(tm, rank, gasneti_handleridx(gasnete_amref_get_reqh), 0,
                   (gex_AM_Arg_t)nbytes, PACK(dest), PACK(src), PACK_EOP_DONE(op));

    return (gex_Event_t)op;
  } else {
    /*  need many messages - use an access region to coalesce them into a single event */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    gasnete_begin_nbi_accessregion(0,1 /* enable recursion */ GASNETI_THREAD_PASS);
    gasnete_amref_get_nbi(tm, dest, rank, src, nbytes, flags GASNETI_THREAD_PASS);
    return gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
  }
}
#endif /* GASNETE_BUILD_AMREF_GET */

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_PUT
extern
gex_Event_t gasnete_amref_put_nb(
                     gex_TM_t tm,
                     gex_Rank_t rank, void *dest,
                     void *src,
                     size_t nbytes, gex_Event_t *lc_opt,
                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
 GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
 {
  // EVENT_DEFER is accomplished using an nbi access region, ended with EVENT_DEFER.
  // Otherwise this reference implementation has no way to portably link the
  // LC of an AM Request to a gex_Event_t.
  if (lc_opt != GEX_EVENT_DEFER) {
    // Cannot maintain connection between root and leaf
    // TODO-EX: complete implementation of GASNETI_FLAG_LC_OPT_IN
    if (gasneti_leaf_is_pointer(lc_opt)) {
      *lc_opt = GEX_EVENT_INVALID;
      lc_opt = GEX_EVENT_NOW;
    }

    if (nbytes <= GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD) {
      gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_amref_put_reqh),
                               src, nbytes, lc_opt, 0,
                               PACK(dest), PACK_EOP_DONE(op));

      return (gex_Event_t)op;
#if GASNETE_USE_LONG_PUTS
    } else if (nbytes <= gex_AM_LUBRequestLong()) { // TODO-EX: _lub_ -> _max_
      gasnete_eop_t *op = gasnete_eop_new(GASNETI_MYTHREAD);

      gex_AM_RequestLong(tm, rank, gasneti_handleridx(gasnete_amref_putlong_reqh),
                             src, nbytes, dest, lc_opt, 0,
                             PACK_EOP_DONE(op));

      return (gex_Event_t)op;
    }
#endif
    // Fall through if too large for a single AM
  }

  {
    /*  need many messages or EVENT_DEFER - use an access region to coalesce into a single event */
    /*  (note this relies on the fact that our implementation of access regions allows recursion) */
    int nbi_result;
    gex_Event_t event;
    gasnete_begin_nbi_accessregion(0,1 /* enable recursion */ GASNETI_THREAD_PASS);
    nbi_result = gasnete_amref_put_nbi_inner(tm, rank, dest, src, nbytes,
                                             GEX_EVENT_GROUP, flags GASNETI_THREAD_PASS);
    event = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
    if (nbi_result) { // "IMMEDIATE" failure
      gasnete_wait(event GASNETI_THREAD_PASS);
      event = GEX_EVENT_NO_OP;
    }
    return event;
  }
 }
}
#endif /* GASNETE_BUILD_AMREF_PUT */

/* ------------------------------------------------------------------------------------ */
/*
  Non-blocking memory-to-memory transfers (implicit event)
  ==========================================================
  each message sends an ack - we count the number of implicit ops launched and compare
    with the number acknowledged
  Another possible design would be to eliminate some of the acks (at least for puts) 
    by piggybacking them on other messages (like get replies) or simply aggregating them
    the target until the source tries to synchronize
*/

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_GET
extern
int gasnete_amref_get_nbi( gex_TM_t tm,
                           void *dest,
                           gex_Rank_t rank, void *src,
                           size_t nbytes,
                           gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_GET(tm,dest,rank,src,nbytes);
  gasnete_amref_get_nbi_inner(tm, dest, rank, src, nbytes, flags GASNETI_THREAD_PASS);
  return 0;
}
#endif /* GASNETE_BUILD_AMREF_GET */

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_PUT
extern
int gasnete_amref_put_nbi( gex_TM_t tm,
                           gex_Rank_t rank, void *dest,
                           void *src,
                           size_t nbytes, gex_Event_t *lc_opt,
                           gex_Flags_t flags GASNETI_THREAD_FARG)
{
  GASNETI_CHECKPSHM_PUT(tm,rank,dest,src,nbytes);
  gasnete_amref_put_nbi_inner(tm, rank, dest, src, nbytes, lc_opt, flags GASNETI_THREAD_PASS);
  return 0;
}
#endif /* GASNETE_BUILD_AMREF_PUT */

/* ------------------------------------------------------------------------------------ */

void gasnete_check_config_amref(void) {
#if GASNETE_BUILD_AMREF_GET || GASNETE_BUILD_AMREF_PUT
  /* This ensures chunks sent as Medium payloads don't exceed the maximum */
  gasneti_assert_always_uint(GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD ,<=, gex_AM_LUBRequestMedium());
#endif

#if GASNETE_BUILD_AMREF_GET
  // TODO-EX: these checks won't actually ensure what they should if/when we mve from _lub_ to _max_
  /* These ensure nbytes in AM-based Gets will fit in handler_arg_t (bug 2770) */
  gasneti_assert_always_uint(gex_AM_LUBReplyMedium() ,<=, (size_t)0xffffffff);
 #if GASNETE_USE_LONG_GETS
  gasneti_assert_always_uint(gex_AM_LUBReplyLong() ,<=, (size_t)0xffffffff);
 #endif
#endif
}

/* ------------------------------------------------------------------------------------ */

#if GASNETE_BUILD_AMREF_GET_HANDLERS
  #define GASNETE_AMREF_GET_HANDLERS() \
  gasneti_handler_tableentry_with_bits(gasnete_amref_get_reqh,4,7,REQUEST,SHORT,0),      \
  gasneti_handler_tableentry_with_bits(gasnete_amref_get_reph,2,4,REPLY,MEDIUM,0),      \
  gasneti_handler_tableentry_with_bits(gasnete_amref_getlong_reqh,4,7,REQUEST,SHORT,0),  \
  gasneti_handler_tableentry_with_bits(gasnete_amref_getlong_reph,1,2,REPLY,LONG,0),
#else
  #define GASNETE_AMREF_GET_HANDLERS() /*empty*/
#endif
#if GASNETE_BUILD_AMREF_PUT_HANDLERS
  #define GASNETE_AMREF_PUT_HANDLERS() \
  gasneti_handler_tableentry_with_bits(gasnete_amref_put_reqh,2,4,REQUEST,MEDIUM,0),      \
  gasneti_handler_tableentry_with_bits(gasnete_amref_putlong_reqh,1,2,REQUEST,LONG,0),  \
  gasneti_handler_tableentry_with_bits(gasnete_amref_markdone_reph,1,2,REPLY,SHORT,0),
#else
  #define GASNETE_AMREF_PUT_HANDLERS() /*empty*/
#endif

#if defined(GASNETE_BUILD_AMREF_GET_HANDLERS) || defined(GASNETE_BUILD_AMREF_PUT_HANDLERS)
  #define GASNETE_AMREF_HANDLERS() GASNETE_AMREF_GET_HANDLERS() GASNETE_AMREF_PUT_HANDLERS()
#endif
