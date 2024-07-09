/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_vector.c $
 * Description: GASNet Vector implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_refvis.c
#endif

#include "gasnet_vlide.h"

#ifndef GASNETI_VIS_VLIDE
#define GASNETI_VIS_VLIDE 1
#endif

#if GASNETI_VIS_VLIDE
  #define GASNETI_VIS_VLIDE_ENCODE GASNETI_VLIDE_ENCODE
  #define GASNETI_VIS_VLIDE_DECODE GASNETI_VLIDE_DECODE
#else
  #define GASNETI_VIS_VLIDE_ENCODE GASNETI_VLIDE_ENCODE_TRIVIAL
  #define GASNETI_VIS_VLIDE_DECODE GASNETI_VLIDE_DECODE_TRIVIAL
#endif

// whether Vector AMPipeline should respect MAXCHUNK and fall-back to RMA of individual chunks
// enabled for conduits with native RMA, disabled for amref conduits
#ifndef GASNETI_VECTOR_USE_RMA
#define GASNETI_VECTOR_USE_RMA (!GASNETE_USING_REF_EXTENDED_GET && !GASNETE_USING_REF_EXTENDED_PUT)
#endif

typedef uint16_t shortlen_t;
#define SHORTLEN_MAX ((shortlen_t)-1)

/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_MEMVEC_PACK(copy,noempty) {                       \
  uint8_t *ploc = (uint8_t *)buf;                                  \
  gasneti_assert(count > 0 && list && buf);                        \
  if (last_len == (size_t)-1) last_len = list[count-1].gex_len;    \
  if (count == 1) {                                                \
    gasneti_assert(last_len > 0);  /* never pack an empty */       \
    copy(ploc, ((uint8_t*)list[0].gex_addr)+first_offset, last_len); \
    ploc += last_len;                                              \
  } else {                                                         \
    size_t const firstlen = list[0].gex_len - first_offset;        \
    if_pt (noempty || firstlen) {                                  \
      gasneti_assert(firstlen);                                    \
      copy(ploc, ((uint8_t*)list[0].gex_addr)+first_offset, firstlen); \
      ploc += firstlen;                                            \
    } else gasneti_assert(first_offset == 0 && list[0].gex_len == 0); \
    for (size_t i = 1; i < count-1; i++) {                         \
      size_t const len = list[i].gex_len;                          \
      if_pt (noempty || len) {                                     \
        gasneti_assert(len);                                       \
        copy(ploc, list[i].gex_addr, len);                         \
        ploc += len;                                               \
      }                                                            \
    }                                                              \
    if_pt (noempty || last_len) {                                  \
      gasneti_assert(last_len > 0);                                \
      copy(ploc, list[count-1].gex_addr, last_len);                \
      ploc += last_len;                                            \
    }                                                              \
  }                                                                \
  return ploc;                                                     \
}

/* pack a memvec list into a contiguous buffer, using the provided byte offset into the first memvec
   if last_len is (size_t)-1, then last_len is ignored
   otherwise, last_len is used in place of the last memvec length 
     (and is never adjusted based on first_offset, even if count == 1)
   return a pointer into the packed buffer, which points just after the last byte used
   The aggregate list must specify a non-empty set of data, 
     but indivdual iovecs are permitted to be empty
 */
void *gasnete_memvec_pack(size_t count, gex_Memvec_t const *list, void *buf,
                           size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_PACK_HELPER,0)
void *gasnete_memvec_unpack(size_t count, gex_Memvec_t const *list, void const *buf,
                             size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_UNPACK_HELPER,0)
// these versions additionally require that all the iovecs be non-empty
void *gasnete_memvec_pack_noempty(size_t count, gex_Memvec_t const *list, void *buf,
                           size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_PACK_HELPER,1)
void *gasnete_memvec_unpack_noempty(size_t count, gex_Memvec_t const *list, void const *buf,
                             size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_UNPACK_HELPER,1)
/*---------------------------------------------------------------------------------*/

extern void gasnete_packetize_verify(gasnete_packetdesc_t *pt, size_t ptidx, int lastpacket,
                              size_t count, size_t len, gex_Memvec_t const *list) {
  size_t firstidx = pt[ptidx].firstidx;
  size_t firstoffset = pt[ptidx].firstoffset;
  size_t lastidx = pt[ptidx].lastidx;
  size_t lastlen = pt[ptidx].lastlen;
  gasneti_assert_uint(firstidx ,<=, lastidx);
  gasneti_assert_uint(lastidx ,<, count);
  if (ptidx == 0) gasneti_assert(firstidx == 0 && firstoffset == 0); /* first packet */
  else if (firstidx == lastidx && lastlen == 0) ; /* empty local packet */
  else if (firstidx == pt[ptidx-1].lastidx) { /* continued from last packet */
    gasneti_assert(firstoffset > 0);
    gasneti_assert_uint(firstoffset ,<, (list?list[firstidx].gex_len:len));
    if (pt[ptidx-1].lastidx == pt[ptidx-1].firstidx)
      gasneti_assert_uint(firstoffset ,==, pt[ptidx-1].lastlen+pt[ptidx-1].firstoffset);
    else
      gasneti_assert(firstoffset == pt[ptidx-1].lastlen);
  } else { /* packet starts a new entry */
    gasneti_assert_uint(firstidx ,==, pt[ptidx-1].lastidx + 1);
    gasneti_assert(firstoffset == 0);
    if (pt[ptidx-1].lastidx == pt[ptidx-1].firstidx)
      gasneti_assert_uint(pt[ptidx-1].lastlen ,==, (list?list[firstidx-1].gex_len:len)-pt[ptidx-1].firstoffset);
    else
      gasneti_assert_uint(pt[ptidx-1].lastlen ,==, (list?list[firstidx-1].gex_len:len));
  }
  if (lastpacket) {
    if (lastidx == firstidx) {
      if (lastlen == 0) ; /* empty local packet */
      else gasneti_assert_uint(lastlen ,==, (list?list[lastidx].gex_len:len)-firstoffset);
    }
    else gasneti_assert_uint(lastlen ,==, (list?list[lastidx].gex_len:len));
  }
}
/*---------------------------------------------------------------------------------*/
/* ***  Vector *** */
/*---------------------------------------------------------------------------------*/

/* simple gather put, remotely contiguous */
#ifndef GASNETE_PUTV_GATHER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gex_Event_t gasnete_putv_gather(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = dstlist[0].gex_len;
  gasneti_assert(dstcount == 1 && srccount > 1); /* only supports gather put */
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank)); // silly to use for local cases
  if_pf (nbytes == 0) return GEX_EVENT_INVALID; /* event empty */
  GASNETI_TRACE_EVENT(C, PUTV_GATHER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_memvec_pack(srccount, srclist, packedbuf, 0, (size_t)-1);
    visop->type = GASNETI_VIS_CAT_PUTV_GATHER;
    visop->event = gasnete_put_nb(tm, rank, dstlist[0].gex_addr, packedbuf, nbytes, GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0, (void)0);
  }
}
  #define GASNETE_PUTV_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_remotecontig && dstcount == 1 && srccount > 1)                           \
      return gasnete_putv_gather(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTV_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETV_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gex_Event_t gasnete_getv_scatter(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = srclist[0].gex_len;
  gasneti_assert(srccount == 1 && dstcount > 1); /* only supports scatter get */
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank)); // silly to use for local cases
  if_pf (nbytes == 0) return GEX_EVENT_INVALID; /* event empty */
  GASNETI_TRACE_EVENT(C, GETV_SCATTER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+dstcount*sizeof(gex_Memvec_t)+nbytes);
    gex_Memvec_t * const savedlst = (gex_Memvec_t *)(visop + 1);
    void * const packedbuf = savedlst + dstcount;
    GASNETI_MEMCPY(savedlst, dstlist, dstcount*sizeof(gex_Memvec_t));
    visop->type = GASNETI_VIS_CAT_GETV_SCATTER;
    visop->count = dstcount;
    visop->event = gasnete_get_nb(tm, packedbuf, rank, srclist[0].gex_addr, nbytes, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1, (void)0);
  }
}
  #define GASNETE_GETV_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_remotecontig && srccount == 1 && dstcount > 1)                            \
      return gasnete_getv_scatter(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETV_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif
//---------------------------------------------------------------------------------
// *** Incremental AMPipelining Vector Support ***
//
// The incremental AMpipelining vector algorithm has several design goals:
//
// 1. Employ a dynamic hybrid of AM packing and RMA-based transfers.
//    In general a vector transfer may include both small and large contiguous chunks,
//    so choose the best protocol (as determined by (GASNET_VIS_MAXCHUNK) on a per-chunk basis.
//
// 2. Perform injection incrementally during a SINGLE linear traversal of the metadata.
//    This strategy seeks to: 
//    - Improve cache efficiency for cases where the metadata is large
//    - Start injecting as early as possible, overlapping latency of earlier chunks with
//      overheads of calculating and marshalling later chunks.
//    - Avoid making copies of the metadata whenever possible
//
// 3. Filter metadata to remove empty chunks and merge spatially adjacent chunks that
//    appear consecutively in the metadata. This potentially increases chunk sizes and 
//    decreases metadata bandwidth overhead, while still preserving the single linear traversal.
//
// 4. Exploit spatial locality properties of remote addresses by performing incremental
//    compression of metadata on the wire. This decreases metadata bandwidth overhead.
//
// Future work:
//
// 1. Detect large remotely contiguous chunks corresponding to small locally
//    discontiguous chunks and transfer them via RMA through a temporary local buffer.
//
// 2. Consider double-buffering FPAM pack_buf for conduits lacking NPAM but providing
//    asynchronous LC for AM injection.
//    
//---------------------------------------------------------------------------------
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_getv_AMPipeline_visop_signal)
void gasnete_getv_AMPipeline_visop_signal(gasneti_vis_op_t * const visop) {
  if (gasneti_weakatomic_decrement_and_test(&(visop->packetcnt), 
                                            GASNETI_ATOMIC_WMB_PRE|GASNETI_ATOMIC_WEAK_FENCE)) {
    /* last response packet completes operation and cleans up */
    GASNETE_VISOP_SIGNAL(visop, 1);
    gasneti_free(visop->addr); /* free localpt */
    gasneti_free(visop); /* free visop, savedlst and send buffer */
  }
}
//---------------------------------------------------------------------------------
// Helper function that injects a single Incremental AMPipeline packet, after marshalling by the caller
GASNETI_INLINE(gasneti_AMpipeline_packetinject) // inline to allow specialization on isget
void gasneti_AMpipeline_packetinject(
            gex_TM_t const tm, gex_Rank_t const rank,
            int const isget,            
            gex_AM_SrcDesc_t sd, const void *packet_buf, 
            size_t packet_nbytes,
            gasneti_vis_op_t *visop, size_t savedlst_idx, size_t savedlst_offset
            GASNETI_THREAD_FARG) {
  gasneti_assert(packet_buf);
  gasneti_assert(packet_nbytes > 0);
  if (isget) {      
    // Assert that savedlst_idx and savedlst_offset each fit in a 32-bit AM arg
    // It's theoretically possible these could exceed 32-bit, but not worth doubling arg overhead
    // Details:
    // * savedlst_offset could only exceed 32-bit for reference conduits that disable RMA 
    //   (GASNETI_VECTOR_USE_RMA==0), with a gex_VIS_VectorGet() metadata input where a 
    //   single contiguous local chunk exceeds 2GiB.
    // * savedlst_index could only exceed 32-bit with a gex_VIS_VectorGet() metadata input 
    //   where the decomposed local chunk count (worst case `dstcount + srccount`, 
    //   but more commonly `<= dstcount`) exceed 2 billion.
    // Both limits could be avoided by adding two more AM handler arguments, wasting 16-byte
    // of wire space for cases that (IMHO) should never arise in practice. 
    // More complicated solutions that avoid adding packet overhead would incur additional 
    // branches (overhead) in the critical-path packing loop.
    gasneti_assert_uint(savedlst_idx ,==, (gex_AM_Arg_t)savedlst_idx);
    gasneti_assert_uint(savedlst_offset ,==, (gex_AM_Arg_t)savedlst_offset);
    gasneti_weakatomic_increment(&(visop->packetcnt), 0 /* no fence needed */);
    
    gex_AM_Index_t handler = gasneti_handleridx(gasnete_getv_AMPipeline_reqh);
    #define ARGS PACK(visop), savedlst_idx, savedlst_offset
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, handler,
                           (void *)packet_buf, packet_nbytes, GEX_EVENT_NOW, 0, ARGS);
    #else
      gex_AM_CommitRequestMedium(sd, handler, packet_nbytes, ARGS);
    #endif
    #undef ARGS
    
  } else { // put
    // TODO: We'd prefer to batch all the gasneti_iop_register's together to reduce overhead
    //       but naively batching and incrementing at the end risks breaking iop_check's invariant assert(iop.initiated >= iop.completed)
    //       which (assertion aside) is necessary to assure an aop is not prematurely marked complete.
    //       A better approach would be an initial increment of gasneti_iop_register(ATOMIC_MAX/4),
    //       followed by a final decrement of gasneti_iop_markdone(ATOMIC_MAX/4 - packets_injected),
    gasneti_iop_t *iop = gasneti_iop_register(1, isget GASNETI_THREAD_PASS);
           
    gex_AM_Index_t handler = gasneti_handleridx(gasnete_putv_AMPipeline_reqh);  
    #define ARGS PACK(iop)
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, handler,
                               (void *)packet_buf, packet_nbytes, GEX_EVENT_NOW, 0, ARGS);
    #else
      gex_AM_CommitRequestMedium(sd, handler, packet_nbytes, ARGS);
    #endif
    #undef ARGS  
  }
}
//---------------------------------------------------------------------------------
// gasneti_AMpipeline_incremental performs the incremental AMPipelined vector injection
// requires: 
//   - tm,rank specifies a non-local peer
//   - the caller has established an appropriate iop/aop to allow NBI injection 
//       (NBI puts use lc_opt == GEX_EVENT_GROUP)
// ensures: 
//   - all communication has been initiated (but not completed) and registered on the iop/aop
//   - lvec and rvec are fully consumed before return
GASNETI_INLINE(gasneti_AMpipeline_incremental) // inline to allow specialization on isget
void gasneti_AMpipeline_incremental(
            gex_TM_t const tm, gex_Rank_t const rank,
            int const isget, 
            size_t rvec_cnt, gex_Memvec_t const *rvec_lst,
            size_t lvec_cnt, gex_Memvec_t const *lvec_lst 
            GASNETI_THREAD_FARG) {
  
  // RVEC tracking:
  gex_Memvec_t rvec = {0,0}; // current rvec entry being processed, len==0 when none is being processed
  gex_Memvec_t const * rvec_next;   // next non-empty in rvec list, or rvec_end
  gex_Memvec_t const * const rvec_end = &rvec_lst[rvec_cnt];
  for (rvec_next = rvec_lst; GASNETT_PREDICT_FALSE(rvec_next->gex_len == 0) ; ) { 
    if (++rvec_next == rvec_end) return; // empty transfer!
  }
  
  // LVEC tracking:
  gex_Memvec_t lvec = {0,0}; // current lvec entry being processed, len==0 when none is being processed
  gex_Memvec_t const * lvec_next;   // next non-empty entry in lvec list, or lvec_end
  gex_Memvec_t const * const lvec_end = &lvec_lst[lvec_cnt];
  for (lvec_next = lvec_lst; GASNETT_PREDICT_FALSE(lvec_next->gex_len == 0); ) { 
    lvec_next++;
    gasneti_assert_ptr(lvec_next ,<, lvec_end); // lvec_lst cannot be empty if rveclst was non-empty
  }
  
  // PACKET FORMAT: (no padding)
  // Incremental vector put:
  //   RequestMedium: encoded rptr | shortlen_t | data | encoded rptr | shortlen_t | data | ...
  // Incremental vector get:
  //   RequestMedium: encoded rptr | shortlen_t | encoded rptr | shortlen_t | ...
  //   ReplyMedium:   data | data |  ...
  
  size_t const request_args = (isget ? HARGS(3,4) : HARGS(1,2));
  size_t const maxrequest = gex_AM_MaxRequestMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                   (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                   request_args);

  // Defer queue:
  #if GASNETE_VIS_NPAM && GASNETI_VECTOR_USE_RMA
    gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
    gasneti_vector_pair_t *defer_buf = td->defer_buf;
    size_t                 defer_sz  = td->defer_sz;
    size_t                 defer_cnt = 0;
  #endif

  // Pack buffer tracking:
  #if GASNETE_VIS_NPAM == 0
    uint8_t * const pack_buf = gasnete_visbuf_malloc(maxrequest); // packing buffer
    uint8_t * const pack_end = pack_buf + maxrequest; // end of current packing buffer
  #else // NPAM
    uint8_t *pack_buf = NULL;
    uint8_t *pack_end = (uint8_t *)(uintptr_t)-1;
  #endif
  uint8_t *pack_p = NULL;   // current position in packing buffer (null when there is no buffer)
  void *pack_rvec = NULL; // location of len for currently filling rvec in the packing buffer, NULL when not yet placed
  gex_AM_SrcDesc_t sd = GEX_AM_SRCDESC_NO_OP;
  
  // VLIDE state
  void * segbase = NULL;
  #if !GASNET_SEGMENT_EVERYTHING
  { size_t segsize = 0;
    gex_Event_Wait(gex_EP_QueryBoundSegmentNB(tm, rank, &segbase, 0, &segsize, 0));
    gasneti_assert(segsize != 0);  
    gasneti_assert(segbase != NULL);
  }
  #endif
  uintptr_t prev_rptr = (uintptr_t)segbase; // previous rvec.addr packed into current packet, or segbase
  
  // GET ONLY STATE:
  size_t const savedlst_max = (rvec_cnt + lvec_cnt); // worst-case breaks
  gasneti_vis_op_t * const visop = (isget ? gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)
                                                         + savedlst_max*sizeof(gex_Memvec_t)) 
                                          : NULL);
  gex_Memvec_t * const savedlst = (gex_Memvec_t *)(visop + 1);
  size_t savedlst_cnt = 0; // current number of entries in savedlst
  size_t savedlst_idx = 0, savedlst_offset = 0; // base index and offset into savedlst for current packet
  gex_Memvec_t *savedlst_pos = NULL; // current entry in savedlst corresponding to lvec, or NULL for none yet
  size_t reply_datasz = 0; // amount of data in reply to current packet
  size_t maxreply;
  if (isget) { // init get state
    maxreply = gex_AM_MaxReplyMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                     (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                     HARGS(3,4));
    #if GASNET_DEBUG
      visop->type = GASNETI_VIS_CAT_GETV_AMPIPELINE;
      visop->count = savedlst_max;
    #endif
    visop->iop = gasneti_iop_register(1,isget GASNETI_THREAD_PASS);
    gasneti_weakatomic_set(&(visop->packetcnt), 1, 0/* no fence needed */); // pre-increment
    visop->addr = NULL;
    visop->eop = NULL;
  }
  
  do {
    if (!rvec.gex_len) {
      // pick-up maximal contiguous rvec
      pack_rvec = NULL;  // break any prior packed rvec
      gasneti_assert_ptr(rvec_next ,<, rvec_end);    
      rvec = *rvec_next; gasneti_assert(rvec.gex_len); rvec_next++;
      while (rvec_next < rvec_end) { // advance rvec_next while merging-up
        if (GASNETT_PREDICT_FALSE(rvec_next->gex_len == 0)) rvec_next++;
        else if (GASNETT_PREDICT_FALSE(rvec_next->gex_addr == 
                                       (uint8_t*)rvec.gex_addr + rvec.gex_len)) { 
          rvec.gex_len += rvec_next->gex_len; // merge up
          rvec_next++;
        }
        else break; // end of contiguity
      }
    }
      
    if (!lvec.gex_len) {
      // pick-up maximal contiguous lvec   
      if (isget) savedlst_pos = NULL;
      gasneti_assert_ptr(lvec_next ,<, lvec_end); 
      lvec = *lvec_next; gasneti_assert(lvec.gex_len); lvec_next++;
      while (lvec_next < lvec_end) { // advance lvec_next while merging-up
        if (GASNETT_PREDICT_FALSE(lvec_next->gex_len == 0)) lvec_next++;
        else if (GASNETT_PREDICT_FALSE(lvec_next->gex_addr == 
                                       (uint8_t*)lvec.gex_addr + lvec.gex_len)) {
          lvec.gex_len += lvec_next->gex_len; // merge up
          lvec_next++;
        }
        else break; // end of contiguity
      }
    }
      
    size_t contig_sz = MIN(lvec.gex_len,rvec.gex_len);
  #if GASNETI_VECTOR_USE_RMA
    if (contig_sz > (isget?gasnete_vis_get_maxchunk:gasnete_vis_put_maxchunk)) { // use NBI RMA
      if (!GASNETE_VIS_NPAM || !pack_p) {
        // safe to inject right now
        if (isget) GASNETE_GET_INDIV(tm,rank,lvec.gex_addr,rvec.gex_addr,contig_sz);
        else       GASNETE_PUT_INDIV(tm,rank,rvec.gex_addr,lvec.gex_addr,contig_sz,GEX_EVENT_GROUP);
      } else { // must defer RMA injection to outside NPAM Prepare/Commit interval
        gasneti_assert(GASNETE_VIS_NPAM);
        #if GASNETE_VIS_NPAM
          if_pf (defer_cnt >= defer_sz) { // need to alloc/grow defer queue
            if_pf (defer_sz == 0) { // first use this thread
              gasneti_assert(defer_cnt == 0);
              #ifndef GASNETI_VECTOR_DEFER_INITIAL
              #define GASNETI_VECTOR_DEFER_INITIAL 8
              #endif
              defer_buf = gasneti_malloc(sizeof(gasneti_vector_pair_t)*GASNETI_VECTOR_DEFER_INITIAL);
              defer_sz = GASNETI_VECTOR_DEFER_INITIAL;
            } else {
              defer_sz <<= 1; 
              defer_buf = gasneti_realloc(defer_buf, sizeof(gasneti_vector_pair_t)*defer_sz);
            }
            td->defer_buf = defer_buf; 
            td->defer_sz = defer_sz;
          }
          gasneti_assert_uint(defer_cnt ,<, defer_sz);
          // save deferred RMA
          gasneti_vector_pair_t * const pdef = &defer_buf[defer_cnt++];
          pdef->lptr = lvec.gex_addr;
          pdef->rptr = rvec.gex_addr;
          pdef->sz = contig_sz;
          // DRAIN_DEFERRED performs deferred RMA injection, in reverse order to improve cache locality
          #define DRAIN_DEFERRED() do { \
            while (defer_cnt) { \
              gasneti_vector_pair_t * pdef = &defer_buf[--defer_cnt]; \
              if (isget) GASNETE_GET_INDIV(tm,rank,pdef->lptr,pdef->rptr,pdef->sz); \
              else       GASNETE_PUT_INDIV(tm,rank,pdef->rptr,pdef->lptr,pdef->sz,GEX_EVENT_GROUP); \
            } \
          } while (0)
        #endif
      }
  
      pack_rvec = NULL;  // break any prior packed rvec    
      if (isget && savedlst_pos) { // break any current savedlst entry
        gasneti_assert_uint(savedlst_pos->gex_len ,>, lvec.gex_len);
        // benign race here: Theoretically gasnete_getv_AMPipeline_reph_inner could be reading
        // the entry we are updating here, but under the reasonable assumption this write is atomic,
        // it doesn't matter whether the handler reads the old or new gex_len value
        savedlst_pos->gex_len -= lvec.gex_len;
        savedlst_pos = NULL;
      }
    } else
  #endif // GASNETI_VECTOR_USE_RMA
    { // pack into AM buffer
      
      // check if current buffer is full
      // TODO conservative: ignoring ptr compression and partial pack entry in pack_rvec
      if (pack_p + sizeof(void*)+sizeof(shortlen_t)+(!isget) > pack_end ||
          (isget && reply_datasz == maxreply)) {
          // send a full buffer
          gasneti_AMpipeline_packetinject(tm, rank, isget,
                                          sd, pack_buf, (pack_p-pack_buf),
                                          visop, savedlst_idx, savedlst_offset
                                          GASNETI_THREAD_PASS);
          pack_p = NULL;
          #if GASNETI_VECTOR_USE_RMA && GASNETE_VIS_NPAM
            DRAIN_DEFERRED();
          #endif
      }
      
      if (isget && !savedlst_pos) {
        // populate get's savedlst with lvec
        savedlst_pos = &savedlst[savedlst_cnt++];
        *savedlst_pos = lvec;
      } 
         
      if (!pack_p) {
        // start a new AM packet buffer
        #if GASNETE_VIS_NPAM  // NPAM 1 or 2 (currently treated as 1)
          sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, maxrequest, maxrequest, NULL, 0, request_args);
          gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxrequest);
          pack_buf = gex_AM_SrcDescAddr(sd);
          pack_end = pack_buf + maxrequest;
        #endif        
        pack_p = pack_buf;
        prev_rptr = (uintptr_t)segbase;
        pack_rvec = NULL;
        if (isget) {
          reply_datasz = 0;
          savedlst_idx = savedlst_cnt-1;
          savedlst_offset = (uint8_t*)lvec.gex_addr - (uint8_t*)savedlst_pos->gex_addr; 
        }  
      }
  
      shortlen_t already_packed;
      if (pack_rvec) { // resuming an partial pack entry
        GASNETI_MEMCPY(&already_packed, pack_rvec, sizeof(already_packed));  // read current packed entry sz
        if_pf (sizeof(shortlen_t) < 4 && already_packed == SHORTLEN_MAX) goto new_pack_entry;
      } else { // starting a new pack entry
       new_pack_entry:
        already_packed = 0;
        GASNETI_VIS_VLIDE_ENCODE(pack_p, prev_rptr, (uintptr_t)rvec.gex_addr);
        prev_rptr = (uintptr_t)rvec.gex_addr;
        pack_rvec = pack_p;
        pack_p += sizeof(shortlen_t);
      }
      // cap contig size to size limits on this packed entry
      size_t remaining_space = (isget ? maxreply - reply_datasz : pack_end - pack_p);
      gasneti_assert_uint(remaining_space ,>, 0);
      contig_sz = MIN(contig_sz, remaining_space);     
      if (sizeof(shortlen_t) < 4) { // avoid shortlen overflow
        contig_sz = MIN(contig_sz, SHORTLEN_MAX-already_packed);        
      }
  
      gasneti_assert_uint(contig_sz ,>, 0);    
      gasneti_assert_uint((size_t)already_packed+contig_sz ,<=, SHORTLEN_MAX);    
      already_packed += contig_sz;
      GASNETI_MEMCPY(pack_rvec, &already_packed, sizeof(already_packed));  // update packed entry sz   
      if (isget) reply_datasz += contig_sz;
      else { // write contig data
        GASNETI_MEMCPY(pack_p, lvec.gex_addr, contig_sz);
        pack_p += contig_sz;
      } 
    } // packed contigsz into AM
    
    // advance current vecs
    gasneti_assert_uint(lvec.gex_len ,>=, contig_sz); 
    lvec.gex_len -= contig_sz;
    lvec.gex_addr = contig_sz + (uint8_t*)lvec.gex_addr;
    gasneti_assert_uint(rvec.gex_len ,>=, contig_sz);   
    rvec.gex_len -= contig_sz;      
    rvec.gex_addr = contig_sz + (uint8_t*)rvec.gex_addr;
    
    // sanity check invariants
    gasneti_assert_ptr(pack_p ,<=, pack_end);
    if (pack_rvec) {
      gasneti_assert_ptr(pack_rvec ,>, pack_buf);
      gasneti_assert_ptr(pack_rvec ,<, pack_p);  
    }
    gasneti_assert_ptr(rvec_next ,<=, rvec_end);    
    gasneti_assert_ptr(lvec_next ,<=, lvec_end);    
    if (isget) {
      gasneti_assert_uint(reply_datasz ,<=, maxreply);  
      gasneti_assert_uint(savedlst_cnt ,<=, savedlst_max); 
      if (savedlst_pos) {
        gasneti_assert_uint(savedlst_idx ,<, savedlst_cnt);
        gasneti_assert_ptr(savedlst_pos ,==, &savedlst[savedlst_cnt-1]);   
      }
    }
      
  } while(lvec.gex_len + rvec.gex_len > 0 || rvec_next < rvec_end); 
  
  gasneti_assert_ptr(rvec_next ,==, rvec_end);
  gasneti_assert_ptr(lvec_next ,==, lvec_end);
  if (pack_p) {
    // send last buffer
    gasneti_AMpipeline_packetinject(tm, rank, isget,
                                    sd, pack_buf, (pack_p-pack_buf),
                                    visop, savedlst_idx, savedlst_offset
                                    GASNETI_THREAD_PASS);
    #if GASNETI_VECTOR_USE_RMA && GASNETE_VIS_NPAM
      DRAIN_DEFERRED();
    #endif
  }
  if (isget) {
    // trailing decrement of visop->packetcnt (that might end up freeing the visop!)
    gasnete_getv_AMPipeline_visop_signal(visop);
  }
  #if GASNETE_VIS_NPAM == 0
    gasneti_free(pack_buf);
  #endif
}
//---------------------------------------------------------------------------------
// return the base of the bound segment associated with the EP that recvd this AM token
// and optionally return the EP
GASNETI_INLINE(gasnete_token_segbase)
void *gasnete_token_segbase(gex_Token_t token, gex_EP_t *ep) {
  gex_Token_Info_t ti;
  gex_TI_t r = gex_Token_Info(token, &ti, GEX_TI_EP);
  gasneti_assert(r & GEX_TI_EP);
  gasneti_assert(ti.gex_ep != GEX_EP_INVALID);
  if (ep) *ep = ti.gex_ep;
#if GASNET_SEGMENT_EVERYTHING
  return NULL;
#else
  gex_Segment_t seg = gex_EP_QuerySegment(ti.gex_ep);
  gasneti_assert(seg != GEX_SEGMENT_INVALID);
  return gex_Segment_QueryAddr(seg);
#endif
}
//---------------------------------------------------------------------------------
#endif // GASNETE_USE_AMPIPELINE

//---------------------------------------------------------------------------------
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTV_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gex_Event_t gasnete_putv_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(srccount >= 1);
  gasneti_assert(dstcount >= 1);
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, PUTV_AMPIPELINE);
  GASNETE_START_NBIREGION(synctype);
  gasneti_AMpipeline_incremental(tm, rank, /*isget=*/0,
                                 dstcount, dstlist, srccount, srclist
                                 GASNETI_THREAD_PASS);
  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_ampipe)                                                      \
      return gasnete_putv_AMPipeline(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
// Put RequestMedium buffer format:
//    encoded rptr | shortlen_t | data | encoded rptr | shortlen_t | data | ...
GASNETI_INLINE(gasnete_putv_AMPipeline_reqh_inner)
void gasnete_putv_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *iop) {
  gex_EP_t ep;
  uintptr_t rptr = (uintptr_t)gasnete_token_segbase(token, &ep);
  #if GASNET_DEBUG
    gasneti_EP_t i_ep = gasneti_import_ep(ep);
  #endif
  uint8_t const * pbuf = addr;
  uint8_t const * const buf_end = pbuf + nbytes;
  do {
    gasneti_assert_ptr(pbuf ,<, buf_end);
    GASNETI_VIS_VLIDE_DECODE(pbuf, rptr, rptr);
    shortlen_t len;
    gasneti_assert_ptr(pbuf+sizeof(len) ,<, buf_end);
    GASNETI_MEMCPY(&len, pbuf, sizeof(len));
    pbuf += sizeof(len);
    gasneti_assert_ptr(pbuf+len ,<=, buf_end);
    gasneti_assert(gasneti_in_local_fullsegment(i_ep, (void *)rptr, len));
    GASNETI_MEMCPY((void *)rptr, pbuf, len);
    pbuf += len;
  } while (pbuf < buf_end);
  gasneti_assert_ptr(pbuf ,==, buf_end);
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph), 0, PACK(iop));
}
MEDIUM_HANDLER(gasnete_putv_AMPipeline_reqh,1,2, 
              (token,addr,nbytes, UNPACK(a0)     ),
              (token,addr,nbytes, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_putvis_AMPipeline_reph_inner)
void gasnete_putvis_AMPipeline_reph_inner(gex_Token_t token,
  void *iop) {
  gasneti_iop_markdone(iop, 1, 0);
}
SHORT_HANDLER(gasnete_putvis_AMPipeline_reph,1,2, 
              (token, UNPACK(a0)),
              (token, UNPACK2(a0, a1)));
#endif
/* ------------------------------------------------------------------------------------ */
/* Pipelined AM gather-scatter get */
#ifndef GASNETE_GETV_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gex_Event_t gasnete_getv_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(dstcount >= 1);
  gasneti_assert(srccount >= 1);
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, GETV_AMPIPELINE);
  GASNETE_START_NBIREGION(synctype);
  gasneti_AMpipeline_incremental(tm, rank, /*isget=*/1,
                                 srccount, srclist, dstcount, dstlist
                                 GASNETI_THREAD_PASS);
  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_ampipe)                                                      \
      return gasnete_getv_AMPipeline(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
// Get RequestMedium buffer format:
//    encoded rptr | shortlen_t | encoded rptr | shortlen_t | ...
GASNETI_INLINE(gasnete_getv_AMPipeline_reqh_inner)
void gasnete_getv_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t savedlst_idx, gex_AM_Arg_t savedlst_offset) {
  
  size_t const replyargs = HARGS(3,4);
  // TODO: potentially transmit the actual (possibly lower) reply datasz if that provided some benefit?
  size_t maxreply = gex_Token_MaxReplyMedium(token, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                   (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                   replyargs);  
  #if GASNETE_VIS_NPAM
    gex_AM_SrcDesc_t sd = gex_AM_PrepareReplyMedium(token, NULL, maxreply, maxreply, NULL, 0, replyargs);
    gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxreply);
    uint8_t * const databuf = gex_AM_SrcDescAddr(sd);    
  #else
    uint8_t * const databuf = gasnete_visbuf_malloc(maxreply);
  #endif                 
  uint8_t * const data_end = databuf + maxreply;  
  uint8_t * pdata = databuf;
  uint8_t * pmeta = addr;
  uint8_t * meta_end = pmeta + nbytes;
  gex_EP_t ep;  
  uintptr_t rptr = (uintptr_t)gasnete_token_segbase(token, &ep);
  #if GASNET_DEBUG
    gasneti_EP_t i_ep = gasneti_import_ep(ep);
  #endif  
  do {
    gasneti_assert_ptr(pmeta ,<, meta_end);  
    GASNETI_VIS_VLIDE_DECODE(pmeta, rptr, rptr);
    shortlen_t len;
    gasneti_assert_ptr(pmeta+sizeof(len) ,<=, meta_end);
    GASNETI_MEMCPY(&len, pmeta, sizeof(len));
    pmeta += sizeof(len);
    gasneti_assert_ptr(pdata+len ,<=, data_end); 
    gasneti_assert(gasneti_in_local_fullsegment(i_ep, (void *)rptr, len));    
    GASNETI_MEMCPY(pdata, (void *)rptr, len);
    pdata += len;
  } while (pmeta < meta_end);
  gasneti_assert_ptr(pmeta ,==, meta_end);
 
  size_t const reply_bytes = pdata - databuf;
  gasneti_assert(reply_bytes > 0);
  gasneti_assert_uint(reply_bytes ,<=, maxreply);
  #define ARGS PACK(_visop), savedlst_idx, savedlst_offset
  #if GASNETE_VIS_NPAM == 0
    gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_getv_AMPipeline_reph), 
                       databuf, reply_bytes, GEX_EVENT_NOW, 0, ARGS);
  #else
    gex_AM_CommitReplyMedium(sd, gasneti_handleridx(gasnete_getv_AMPipeline_reph), 
                             reply_bytes, ARGS);
  #endif    
  #undef ARGS
  
  #if GASNETE_VIS_NPAM == 0
    gasneti_free(databuf);
  #endif    
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reqh,3,4, 
              (token,addr,nbytes, UNPACK(a0),      a1, a2),
              (token,addr,nbytes, UNPACK2(a0, a1), a2, a3));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_getv_AMPipeline_reph_inner)
void gasnete_getv_AMPipeline_reph_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t savedlst_idx, gex_AM_Arg_t savedlst_offset) {
  gasneti_assert(addr && nbytes > 0);
  uint8_t const *pbuf = addr;
  uint8_t const * const buf_end = pbuf + nbytes;

  gasneti_vis_op_t * const visop = _visop;
  gasneti_assert_uint(visop->type ,==, GASNETI_VIS_CAT_GETV_AMPIPELINE);
  gex_Memvec_t const * const savedlst = (gex_Memvec_t *)(visop + 1);
  gasneti_assert_int(savedlst_idx ,>=, 0);
  gasneti_assert_int(savedlst_idx ,<, visop->count);
  gex_Memvec_t const * savedlst_pos = savedlst + savedlst_idx;

  gex_Memvec_t lvec = *(savedlst_pos++);
  gasneti_assert_int(savedlst_offset ,>=, 0);
  gasneti_assert_int(savedlst_offset ,<, lvec.gex_len);
  lvec.gex_addr = (uint8_t *)lvec.gex_addr + savedlst_offset;
  lvec.gex_len -= savedlst_offset;
  do {
    gasneti_assert(lvec.gex_len > 0);
    size_t const len = MIN(lvec.gex_len, buf_end - pbuf);
    GASNETI_MEMCPY(lvec.gex_addr, pbuf, len);
    pbuf += len;
  } while ((pbuf < buf_end) && 
           (lvec = *(savedlst_pos++),1));
  gasneti_assert_ptr(pbuf ,==, buf_end);
  gasnete_getv_AMPipeline_visop_signal(visop);
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reph,3,4,
              (token,addr,nbytes, UNPACK(a0),      a1, a2),
              (token,addr,nbytes, UNPACK2(a0, a1), a2, a3));
#endif
/*---------------------------------------------------------------------------------*/
#define _VEC_SKIPEMPTY(idx,count,list) \
  while (idx < count && GASNETT_PREDICT_FALSE(list[idx].gex_len == 0)) idx++

// GASNETE_VECTOR_HELPER iterates over 2-sided vector metadata in order and invokes
// the provided macro action(p1,p2,len) on each contiguous matching chunk of the sequences
#define GASNETE_VECTOR_HELPER(count1, list1, count2, list2, action) do {        \
  size_t const _count1 = (count1); gex_Memvec_t const * const _list1 = (list1); \
  size_t const _count2 = (count2); gex_Memvec_t const * const _list2 = (list2); \
  if (_count1 == 1) { /* 1 is contiguous buffer */                              \
    uintptr_t _pp1 = (uintptr_t)(_list1[0].gex_addr);                           \
    for (size_t _i = 0; _i < _count2; _i++) {                                   \
      size_t const _len2 = _list2[_i].gex_len;                                  \
      if_pt (_len2 > 0) {                                                       \
        void * const _p1 = (void *)_pp1;                                        \
        void * const _p2 = _list2[_i].gex_addr;                                 \
        action(_p1, _p2, _len2);                                                \
        _pp1 += _len2;                                                          \
      }                                                                         \
    }                                                                           \
    gasneti_assert_uint(_pp1 ,==, (uintptr_t)(_list1[0].gex_addr)+_list1[0].gex_len); \
  } else if (_count2 == 1) { /* 2 is contiguous buffer */                       \
    uintptr_t _pp2 = (uintptr_t)(_list2[0].gex_addr);                           \
    for (size_t _i = 0; _i < _count1; _i++) {                                   \
      size_t const _len1 = _list1[_i].gex_len;                                  \
      if_pt (_len1 > 0) {                                                       \
        void * const _p1 = _list1[_i].gex_addr;                                 \
        void * const _p2 = (void *)_pp2;                                        \
        action(_p1, _p2, _len1);                                                \
        _pp2 += _len1;                                                          \
      }                                                                         \
    }                                                                           \
    gasneti_assert_uint(_pp2 ,==, (uintptr_t)(_list2[0].gex_addr)+_list2[0].gex_len); \
  } else { /* general case */                                                   \
    size_t _idx1 = 0; size_t _idx2 = 0;                                         \
    size_t _off1 = 0; size_t _off2 = 0;                                         \
    _VEC_SKIPEMPTY(_idx1,_count1,_list1);                                       \
    _VEC_SKIPEMPTY(_idx2,_count2,_list2);                                       \
    while (_idx1 < _count1) {                                                   \
      gasneti_assert_uint(_idx2 ,<, _count2);                                   \
      size_t const _rem1 = _list1[_idx1].gex_len - _off1;                       \
      size_t const _rem2 = _list2[_idx2].gex_len - _off2;                       \
      gasneti_assert(_rem1 > 0); gasneti_assert(_rem2 > 0);                     \
      void * const _p1 = (void *)(((uintptr_t)_list1[_idx1].gex_addr)+_off1);   \
      void * const _p2 = (void *)(((uintptr_t)_list2[_idx2].gex_addr)+_off2);   \
      if (_rem1 < _rem2) {                                                      \
        action(_p1, _p2, _rem1);                                                \
        _idx1++; _VEC_SKIPEMPTY(_idx1,_count1,_list1);                          \
        _off1 = 0;                                                              \
        _off2 += _rem1;                                                         \
      } else if (_rem1 > _rem2) {                                               \
        action(_p1, _p2, _rem2);                                                \
        _idx2++; _VEC_SKIPEMPTY(_idx2,_count2,_list2);                          \
        _off2 = 0;                                                              \
        _off1 += _rem2;                                                         \
      } else { gasneti_assert_uint(_rem1 ,==, _rem2);                           \
        action(_p1, _p2, _rem1);                                                \
        _idx1++; _VEC_SKIPEMPTY(_idx1,_count1,_list1);                          \
        _idx2++; _VEC_SKIPEMPTY(_idx2,_count2,_list2);                          \
        _off1 = 0;                                                              \
        _off2 = 0;                                                              \
      }                                                                         \
    }                                                                           \
    gasneti_assert_uint(_idx1 ,==, _count1);                                    \
    gasneti_assert_uint(_idx2 ,==, _count2);                                    \
    gasneti_assert(_off1 == 0); gasneti_assert(_off2 == 0);                     \
  }                                                                             \
} while (0)
/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_vector_memcpy)
void gasnete_vector_memcpy(gex_TM_t const tm, gex_Rank_t const rank, int isput,
                            size_t dstcount, gex_Memvec_t const dstlist[],
                            size_t srccount, gex_Memvec_t const srclist[],
                            gex_Flags_t flags) {
  gasneti_assert(GASNETI_NBRHD_MAPPED(tm,rank));

  // TODO-EX: this assumes all addresses in the peer list reside in the same segment
  // and hoists the address translation. Multi-segment will need to push this down.
  gasneti_assert(!(flags & (GEX_FLAG_PEER_SEG_SOME|GEX_FLAG_PEER_SEG_UNKNOWN)));
  size_t const peercount = (isput ? dstcount : srccount);
  gex_Memvec_t const * const peerlist = (isput ? dstlist : srclist);

  size_t base=0;
  for ( ; ; base++) { // find a non-empty peer memvec
    if_pf (base == peercount) return; // empty list
    if_pt (peerlist[base].gex_len) break;
  }
  gasneti_assert_uint(base ,<, peercount);
  gasneti_assert(peerlist[base].gex_len > 0);
  uint8_t const * const rawptr = peerlist[base].gex_addr; 
  gasneti_assert(rawptr);
  uint8_t const * const refptr = GASNETI_NBRHD_MAPPED_ADDR(tm,rank,rawptr);
  ptrdiff_t const offset = refptr - rawptr;
  
  if (isput) {    
    #define ACTION(p1,p2,len) GASNETI_MEMCPY((uint8_t*)p1+offset,p2,len)
    GASNETE_VECTOR_HELPER(dstcount, dstlist, srccount, srclist, ACTION);
    #undef ACTION
  } else {
    #define ACTION(p1,p2,len) GASNETI_MEMCPY(p1,(uint8_t*)p2+offset,len)
    GASNETE_VECTOR_HELPER(dstcount, dstlist, srccount, srclist, ACTION);
    #undef ACTION
  }
}
/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts */
gex_Event_t gasnete_putv_ref_indiv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTV_REF_INDIV);
  gasneti_assert(dstcount > 0); gasneti_assert(srccount > 0);
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank));
  gex_Event_t * const lc_opt = (flags & GEX_FLAG_ENABLE_LEAF_LC) ? GEX_EVENT_GROUP : GEX_EVENT_DEFER;
  GASNETE_START_NBIREGION(synctype);

  #define ACTION(p1,p2,len) GASNETE_PUT_INDIV(tm, rank, p1, p2, len, lc_opt)
  GASNETE_VECTOR_HELPER(dstcount, dstlist, srccount, srclist, ACTION);
  #undef ACTION
  
  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}

/* reference version that uses individual gets */
gex_Event_t gasnete_getv_ref_indiv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETV_REF_INDIV);
  gasneti_assert(dstcount > 0); gasneti_assert(srccount > 0);
  gasneti_assert(!GASNETI_NBRHD_MAPPED(tm,rank));
  GASNETE_START_NBIREGION(synctype);

  #define ACTION(p1,p2,len) GASNETE_GET_INDIV(tm, rank, p1, p2, len)
  GASNETE_VECTOR_HELPER(dstcount, dstlist, srccount, srclist, ACTION);
  #undef ACTION

  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}
/*---------------------------------------------------------------------------------*/
/* top-level gasnet_putv_* entry point */
#ifndef GASNETE_PUTV_OVERRIDE
extern gex_Event_t gasnete_putv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(dstcount > 0); gasneti_assert(srccount > 0);
  gasneti_assert(dstcount > 1 || srccount > 1);
  gasneti_assert(dstlist); gasneti_assert(srclist);
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  if (GASNETI_NBRHD_MAPPED(tm,rank)) { // purely local
    GASNETI_TRACE_EVENT(C, PUTV_NBRHD);
    gasnete_vector_memcpy(tm,rank,1,
                           dstcount,dstlist,srccount,srclist,
                           flags);
    return GEX_EVENT_INVALID;
  }

  /* select algorithm */
  #ifndef GASNETE_PUTV_SELECTOR
    #define GASNETE_PUTV_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags)       \
      GASNETE_PUTV_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags);     \
      GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags); \
      return gasnete_putv_ref_indiv(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
  #endif
  GASNETE_PUTV_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags);
  gasneti_fatalerror("failure in GASNETE_PUTV_SELECTOR - should never reach here");
}
#endif
/* top-level gasnet_getv_* entry point */
#ifndef GASNETE_GETV_OVERRIDE
extern gex_Event_t gasnete_getv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   gex_Rank_t const rank,
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(dstcount > 0); gasneti_assert(srccount > 0);
  gasneti_assert(dstcount > 1 || srccount > 1);
  gasneti_assert(dstlist); gasneti_assert(srclist);
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  if (GASNETI_NBRHD_MAPPED(tm,rank)) { // purely local
    GASNETI_TRACE_EVENT(C, GETV_NBRHD);
    gasnete_vector_memcpy(tm,rank,0,
                           dstcount,dstlist,srccount,srclist,
                           flags);
    return GEX_EVENT_INVALID;
  }

  /* select algorithm */
  #ifndef GASNETE_GETV_SELECTOR
    #define GASNETE_GETV_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags)       \
      GASNETE_GETV_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags);    \
      GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags); \
      return gasnete_getv_ref_indiv(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
  #endif
  GASNETE_GETV_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags);
  gasneti_fatalerror("failure in GASNETE_GETV_SELECTOR - should never reach here");
}
#endif
/*---------------------------------------------------------------------------------*/
