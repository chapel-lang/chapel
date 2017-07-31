/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_vis_vector.c $
 * Description: GASNet Vector implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_extended_refvis.c
#endif

/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_MEMVEC_PACK(copy,noempty) {                       \
  uint8_t *ploc = (uint8_t *)buf;                                  \
  gasneti_assert(count > 0 && list && buf);                        \
  if (last_len == (size_t)-1) last_len = list[count-1].len;        \
  if (count == 1) {                                                \
    gasneti_assert(last_len > 0);  /* never pack an empty */       \
    copy(ploc, ((uint8_t*)list[0].addr)+first_offset, last_len);   \
    ploc += last_len;                                              \
  } else {                                                         \
    size_t const firstlen = list[0].len - first_offset;            \
    if_pt (noempty || firstlen) {                                  \
      gasneti_assert(firstlen);                                    \
      copy(ploc, ((uint8_t*)list[0].addr)+first_offset, firstlen); \
      ploc += firstlen;                                            \
    } else gasneti_assert(first_offset == 0 && list[0].len == 0);  \
    for (size_t i = 1; i < count-1; i++) {                         \
      size_t const len = list[i].len;                              \
      if_pt (noempty || len) {                                     \
        gasneti_assert(len);                                       \
        copy(ploc, list[i].addr, len);                             \
        ploc += len;                                               \
      }                                                            \
    }                                                              \
    if_pt (noempty || last_len) {                                  \
      gasneti_assert(last_len > 0);                                \
      copy(ploc, list[count-1].addr, last_len);                    \
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
void *gasnete_memvec_pack(size_t count, gasnet_memvec_t const *list, void *buf,
                           size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_PACK_HELPER,0)
void *gasnete_memvec_unpack(size_t count, gasnet_memvec_t const *list, void const *buf,
                             size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_UNPACK_HELPER,0)
// these versions additionally require that all the iovecs be non-empty
void *gasnete_memvec_pack_noempty(size_t count, gasnet_memvec_t const *list, void *buf,
                           size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_PACK_HELPER,1)
void *gasnete_memvec_unpack_noempty(size_t count, gasnet_memvec_t const *list, void const *buf,
                             size_t first_offset, size_t last_len) _GASNETE_MEMVEC_PACK(_GASNETE_UNPACK_HELPER,1)
/*---------------------------------------------------------------------------------*/

extern void gasnete_packetize_verify(gasnete_packetdesc_t *pt, size_t ptidx, int lastpacket,
                              size_t count, size_t len, gasnet_memvec_t const *list) {
  size_t firstidx = pt[ptidx].firstidx;
  size_t firstoffset = pt[ptidx].firstoffset;
  size_t lastidx = pt[ptidx].lastidx;
  size_t lastlen = pt[ptidx].lastlen;
  gasneti_assert(firstidx <= lastidx);
  gasneti_assert(lastidx < count);
  if (ptidx == 0) gasneti_assert(firstidx == 0 && firstoffset == 0); /* first packet */
  else if (firstidx == lastidx && lastlen == 0) ; /* empty local packet */
  else if (firstidx == pt[ptidx-1].lastidx) { /* continued from last packet */
    gasneti_assert(firstoffset > 0 && firstoffset < (list?list[firstidx].len:len));
    if (pt[ptidx-1].lastidx == pt[ptidx-1].firstidx)
      gasneti_assert(firstoffset == pt[ptidx-1].lastlen+pt[ptidx-1].firstoffset);
    else
      gasneti_assert(firstoffset == pt[ptidx-1].lastlen);
  } else { /* packet starts a new entry */
    gasneti_assert(firstidx == pt[ptidx-1].lastidx + 1);
    gasneti_assert(firstoffset == 0);
    if (pt[ptidx-1].lastidx == pt[ptidx-1].firstidx)
      gasneti_assert(pt[ptidx-1].lastlen == (list?list[firstidx-1].len:len)-pt[ptidx-1].firstoffset);
    else
      gasneti_assert(pt[ptidx-1].lastlen == (list?list[firstidx-1].len:len));
  }
  if (lastpacket) {
    if (lastidx == firstidx) {
      if (lastlen == 0) ; /* empty local packet */
      else gasneti_assert(lastlen == (list?list[lastidx].len:len)-firstoffset);
    }
    else gasneti_assert(lastlen == (list?list[lastidx].len:len));
  }
}
/*---------------------------------------------------------------------------------*/
/* Packetizes remotelist into a list of gasnete_packetdesc_t entries based on maxpayload packet size
     sharedpacket  => metadata and corresponding data travel together in unified packets (put)
                      so that for each packet i: datasz_i + metadatasz_i <= maxpayload
     !sharedpacket => metadata and corresponding data travel in separate packets (get)
                      so that for each packet i: MAX(datasz_i,metadatasz_i) <= maxpayload
   A local packet table is also computed to match the remote packetization boundaries of the data
     on a byte-for-byte basis
   Allocates and populates the plocalpt and premotept arrays with the packetization information
   Returns the number of packets described by the resulting plocalpt and premotept arrays
 */
size_t gasnete_packetize_memvec(size_t remotecount, gasnet_memvec_t const remotelist[],
                                size_t localcount, gasnet_memvec_t const locallist[],
                                gasnete_packetdesc_t **premotept,
                                gasnete_packetdesc_t **plocalpt,
                                size_t maxpayload, int sharedpacket) {
  size_t ptidx;
  int done = 0;
  size_t ridx = 0, roffset = 0, lidx = 0, loffset = 0;
  size_t const metadatasz = sizeof(gasnet_memvec_t);
  size_t ptsz = 4; /* initial size guess - no fast way to know for sure */
  gasneti_assert(maxpayload > metadatasz);
  gasnete_packetdesc_t *remotept = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasnete_packetdesc_t *localpt = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasneti_assert(premotept && plocalpt && remotecount && localcount);
  gasneti_assert(gasnete_memveclist_totalsz(remotecount,remotelist) == 
                 gasnete_memveclist_totalsz(localcount,locallist));

  for (ptidx = 0; ; ptidx++) {
    ssize_t packetremain = maxpayload;
    ssize_t packetdata = 0;
    size_t ldatasz; 
    size_t rdatasz = 0; // init to avoid a warning on gcc -O3 -Wall

    if (ptidx == ptsz) { /* grow the packet tables */
      ptsz *= 2;
      remotept = gasneti_realloc(remotept, ptsz*sizeof(gasnete_packetdesc_t));
      localpt = gasneti_realloc(localpt, ptsz*sizeof(gasnete_packetdesc_t));
    }

    /* begin remote packet */
    remotept[ptidx].firstidx = ridx;
    remotept[ptidx].firstoffset = roffset;
    /* begin local packet */
    if_pf (lidx == localcount) localpt[ptidx].firstidx = lidx-1; /* might happen if remote has trailing empties */
    else                       localpt[ptidx].firstidx = lidx;
    localpt[ptidx].firstoffset = loffset;

    while (packetremain > metadatasz) { /* room for more entries */
      gasneti_assert(roffset < remotelist[ridx].len || (remotelist[ridx].len == 0 && roffset == 0));
      rdatasz = remotelist[ridx].len - roffset; /* data left in current entry */
      /* try to add the entire entry to packet */
      if (sharedpacket) packetremain -= (metadatasz + rdatasz);
      else              packetremain -= MAX(metadatasz, rdatasz);
      if (packetremain < 0) { /* overflowed - finished a packet, and spill to next */
        rdatasz += packetremain; /* compute truncated datasz that fits in this packet */
        roffset += rdatasz; /* update offset into current entry */
        packetdata += rdatasz;
        break;
      } else {
        packetdata += rdatasz;
        roffset = 0; /* finished an entry */
        ridx++;
        if (ridx == remotecount) { done = 1; break; } /* done - this is last packet */
      }
    }
    /* end remote packet */
    if (roffset == 0) remotept[ptidx].lastidx = ridx-1;
    else              remotept[ptidx].lastidx = ridx;
    remotept[ptidx].lastlen = rdatasz;

    #if GASNET_DEBUG /* verify packing properties */
      gasnete_packetize_verify(remotept, ptidx, done, remotecount, 0, remotelist);
      { size_t datachk = 0, i;
        size_t entries = remotept[ptidx].lastidx - remotept[ptidx].firstidx + 1;
        for (i = remotept[ptidx].firstidx; i <= remotept[ptidx].lastidx; i++) {
          if (i == remotept[ptidx].lastidx) datachk += remotept[ptidx].lastlen;
          else if (i == remotept[ptidx].firstidx) datachk += (remotelist[i].len - remotept[ptidx].firstoffset);
          else datachk += remotelist[i].len;
        }
        gasneti_assert(packetdata == datachk);
        if (sharedpacket) { 
          gasneti_assert((metadatasz*entries + packetdata) <= maxpayload); /* not overfull */
          gasneti_assert(((metadatasz*entries + packetdata) >= maxpayload - metadatasz) || done); /* not underfull */
        } else {
          gasneti_assert(MAX(metadatasz*entries,packetdata) <= maxpayload); /* not overfull */
          /* algorithm currently may underfill for !sharedpacket, because it effectively always 
             subtracts the MAX(metadatasz, datasz) from *both* packets being managed simultaneously in packetremain,
             rather than maintaining independent packetremains and updating each accordingly (increasing arithmetic complexity)
             In vectors whose entries are dominated by datasz or metadatasz, the effect should be neglible
             In perverse cases we might end up with a packet which where the maximal packet is only 2/3 full
             this means in datasz dominated vectors with a few entries where datasz < metadatasz (or vice-versa)
           */
          gasneti_assert((MAX(metadatasz*entries,packetdata) >= (maxpayload - metadatasz)/2) || done); /* not underfull */
        }
      }
    #endif

    ldatasz = 0;
    while (packetdata > 0 || (lidx < localcount && locallist[lidx].len == 0)) {
      gasneti_assert(loffset < locallist[lidx].len || (locallist[lidx].len == 0 && loffset == 0));
      ldatasz = locallist[lidx].len - loffset; /* data left in current entry */
      packetdata -= ldatasz;
      if (packetdata < 0) { /* overflowed - this entry spills into next packet */
        ldatasz += packetdata; /* compute truncated datasz that fits in this packet */
        loffset += ldatasz; /* update offset into current entry */
        break;
      } else {
        loffset = 0; /* finished an entry */
        lidx++;
      }
    }
    /* end local packet */
    if (loffset == 0) localpt[ptidx].lastidx = lidx-1;
    else              localpt[ptidx].lastidx = lidx;
    localpt[ptidx].lastlen = ldatasz;

    #if GASNET_DEBUG /* verify packing properties */
      gasnete_packetize_verify(localpt, ptidx, done, localcount, 0, locallist);
    #endif

    if (done) {
      gasneti_assert(ridx == remotecount && roffset == 0 && lidx == localcount && loffset == 0);
      *premotept = remotept;
      *plocalpt = localpt;
      return ptidx+1;
    }
  }
}
/*---------------------------------------------------------------------------------*/
/* ***  Vector *** */
/*---------------------------------------------------------------------------------*/

/* simple gather put, remotely contiguous */
#ifndef GASNETE_PUTV_GATHER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_putv_gather(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = dstlist[0].len;
  gasneti_assert(dstcount == 1 && srccount > 1); /* only supports gather put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  if_pf (nbytes == 0) return GASNET_INVALID_HANDLE; /* handle empty */
  GASNETI_TRACE_EVENT(C, PUTV_GATHER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_memvec_pack(srccount, srclist, packedbuf, 0, (size_t)-1);
    visop->type = GASNETI_VIS_CAT_PUTV_GATHER;
    visop->handle = gasnete_put_nb_bulk(dstnode, dstlist[0].addr, packedbuf, nbytes GASNETE_THREAD_PASS);
    gasneti_assert(visop->handle != GASNET_INVALID_HANDLE);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0);
  }
}
  #define GASNETE_PUTV_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist) \
    if (gasnete_vis_use_remotecontig && dstcount == 1 && srccount > 1)                                                     \
      return gasnete_putv_gather(synctype,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTV_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETV_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_getv_scatter(gasnete_synctype_t synctype,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   gasnet_node_t srcnode,
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = srclist[0].len;
  gasneti_assert(srccount == 1 && dstcount > 1); /* only supports scatter get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  if_pf (nbytes == 0) return GASNET_INVALID_HANDLE; /* handle empty */
  GASNETI_TRACE_EVENT(C, GETV_SCATTER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+dstcount*sizeof(gasnet_memvec_t)+nbytes);
    gasnet_memvec_t * const savedlst = (gasnet_memvec_t *)(visop + 1);
    void * const packedbuf = savedlst + dstcount;
    memcpy(savedlst, dstlist, dstcount*sizeof(gasnet_memvec_t));
    visop->type = GASNETI_VIS_CAT_GETV_SCATTER;
    visop->count = dstcount;
    visop->handle = gasnete_get_nb_bulk(packedbuf, srcnode, srclist[0].addr, nbytes GASNETE_THREAD_PASS);
    gasneti_assert(visop->handle != GASNET_INVALID_HANDLE);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1);
  }
}
  #define GASNETE_GETV_SCATTER_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist) \
    if (gasnete_vis_use_remotecontig && srccount == 1 && dstcount > 1)                                                      \
      return gasnete_getv_scatter(synctype,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETV_SCATTER_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist) ((void)0)
#endif
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTV_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gasnet_handle_t gasnete_putv_AMPipeline(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasneti_assert(srccount >= 1);
  gasneti_assert(dstcount > 1); /* supports scatter put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, PUTV_AMPIPELINE);
  if_pf (srclist[0].len == 0) { /* detect empty list */
    for (size_t i = 1; i < srccount; i++) { 
      if_pt (srclist[i].len > 0) goto nonempty;
    }
    return GASNET_INVALID_HANDLE;
    nonempty: ;
  }
  GASNETE_START_NBIREGION(synctype, 0);

  { gasnet_memvec_t * const packedbuf = gasneti_malloc(gasnet_AMMaxMedium());
    gasnete_packetdesc_t *remotept;
    gasnete_packetdesc_t *localpt;
    size_t packetidx;
    size_t const packetcnt = gasnete_packetize_memvec(dstcount, dstlist, srccount, srclist, 
                                                &remotept, &localpt, gasnet_AMMaxMedium(), 1);
    gasneti_iop_t *iop = gasneti_iop_register(packetcnt,0 GASNETE_THREAD_PASS);

    for (packetidx = 0; packetidx < packetcnt; packetidx++) {
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      gasnete_packetdesc_t * const lpacket = &localpt[packetidx];
      size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
      // fill packet with remote metadata, filtering empty iovecs as needed
      size_t const firstridx = rpacket->firstidx;
      size_t const lastridx = rpacket->lastidx;
      size_t firstoffset = rpacket->firstoffset;
      size_t rnum = 0; // count remote iovecs
      for (size_t ri = firstridx; ri <= lastridx; ri++) {
        size_t   vlen  = dstlist[ri].len;
        uint8_t *vaddr = dstlist[ri].addr;
        if (firstoffset) { // adjust first
          gasneti_assert(rnum == 0 && ri == firstridx);
          gasneti_assert(firstoffset <= vlen);
          vaddr += firstoffset;
          vlen  -= firstoffset;
          firstoffset = 0;
        }
        if (ri == lastridx) { // truncate last
          gasneti_assert(rpacket->lastlen <= vlen);
          vlen = rpacket->lastlen;
        }
        if_pt (vlen) { // add this non-empty iovec to the wire
          packedbuf[rnum].len = vlen;
          packedbuf[rnum].addr = vaddr;
          rnum++; 
        }
      }
      if_pf (rnum == 0) { // entire packet is empty, skip send
        gasneti_iop_markdone(iop, 1, 0);
        continue;
      }
      /* gather data payload from sourcelist into packet */
      uint8_t * const end = gasnete_memvec_pack(lnum, &srclist[lpacket->firstidx], &packedbuf[rnum], 
                                lpacket->firstoffset, lpacket->lastlen);

      size_t const packetlen = end - (uint8_t *)packedbuf;

      #if GASNET_DEBUG
        // assert we don't send empty iovecs on the wire (bug3411)
        size_t datalen = 0; 
        for (size_t i=0; i < rnum; i++) {
          size_t const thislen = packedbuf[i].len;
          gasneti_assert(thislen > 0);
          datalen += thislen;
        }
        gasneti_assert(datalen > 0); 
        gasneti_assert(packetlen == rnum*sizeof(gasnet_memvec_t)+datalen);
        gasneti_assert(packetlen <= gasnet_AMMaxMedium());
      #endif

      /* send AM(rnum, iop) from packedbuf */
      GASNETI_SAFE(
        MEDIUM_REQ(2,3,(dstnode, gasneti_handleridx(gasnete_putv_AMPipeline_reqh),
                      packedbuf, packetlen,
                      PACK(iop), rnum)));
    }

    gasneti_free(remotept);
    gasneti_free(localpt);
    gasneti_free(packedbuf);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, 0);
  }
}
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist) \
    if (gasnete_vis_use_ampipe && dstcount > 1)                                                \
      return gasnete_putv_AMPipeline(synctype,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_putv_AMPipeline_reqh_inner)
void gasnete_putv_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *iop, gasnet_handlerarg_t rnum) {
  gasneti_assert(addr && nbytes > 0 && rnum > 0);
  gasnet_memvec_t * const rlist = addr;
  uint8_t * const data = (uint8_t *)(&rlist[rnum]);
  uint8_t * const end = gasnete_memvec_unpack_noempty(rnum, rlist, data, 0, (size_t)-1);
  gasneti_assert(end - (uint8_t *)addr <= gasnet_AMMaxMedium());
  gasneti_sync_writes();
  /* TODO: coalesce acknowledgements - need a per-srcnode, per-op seqnum & packetcnt */
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph),
                  PACK(iop))));
}
MEDIUM_HANDLER(gasnete_putv_AMPipeline_reqh,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_putvis_AMPipeline_reph_inner)
void gasnete_putvis_AMPipeline_reph_inner(gasnet_token_t token, 
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

gasnet_handle_t gasnete_getv_AMPipeline(gasnete_synctype_t synctype,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   gasnet_node_t srcnode,
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasneti_assert(dstcount >= 1);
  gasneti_assert(srccount > 1); /* supports gather get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, GETV_AMPIPELINE);
  if_pf (dstlist[0].len == 0) { /* detect empty list */
    for (size_t i = 1; i < dstcount; i++) { 
      if_pt (dstlist[i].len > 0) goto nonempty;
    }
    return GASNET_INVALID_HANDLE;
    nonempty: ;
  }

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t) +
                                                    dstcount*sizeof(gasnet_memvec_t) + 
                                                    gasnet_AMMaxMedium());
    gasnet_memvec_t * const savedlst = (gasnet_memvec_t *)(visop + 1);
    gasnet_memvec_t * const packedbuf = savedlst + dstcount;
    gasnete_packetdesc_t *remotept;
    gasnete_packetdesc_t *localpt;
    gasneti_eop_t *eop;
    size_t packetidx;
    size_t const packetcnt = gasnete_packetize_memvec(srccount, srclist, dstcount, dstlist,  
                                                &remotept, &localpt, gasnet_AMMaxMedium(), 0);
    GASNETE_VISOP_SETUP(visop, synctype, 1);
    #if GASNET_DEBUG
      visop->type = GASNETI_VIS_CAT_GETV_AMPIPELINE;
      visop->count = dstcount;
    #endif
    gasneti_assert(packetcnt <= GASNETI_ATOMIC_MAX);
    gasneti_assert(packetcnt == (gasnet_handlerarg_t)packetcnt);
    visop->addr = localpt;
    memcpy(savedlst, dstlist, dstcount*sizeof(gasnet_memvec_t));
    gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);
    eop = visop->eop; /* visop may disappear once the last AM is launched */

    for (packetidx = 0; packetidx < packetcnt; packetidx++) {
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      // fill packet with remote metadata, filtering empty iovecs as needed
      size_t const firstridx = rpacket->firstidx;
      size_t const lastridx = rpacket->lastidx;
      size_t firstoffset = rpacket->firstoffset;
      size_t rnum = 0; // count remote iovecs
      for (size_t ri = firstridx; ri <= lastridx; ri++) {
        size_t   vlen  = srclist[ri].len;
        uint8_t *vaddr = srclist[ri].addr;
        if (firstoffset) { // adjust first
          gasneti_assert(rnum == 0 && ri == firstridx);
          gasneti_assert(firstoffset <= vlen);
          vaddr += firstoffset;
          vlen  -= firstoffset;
          firstoffset = 0;
        }
        if (ri == lastridx) { // truncate last
          gasneti_assert(rpacket->lastlen <= vlen);
          vlen = rpacket->lastlen;
        }
        if_pt (vlen) { // add this non-empty iovec to the wire
          packedbuf[rnum].len = vlen;
          packedbuf[rnum].addr = vaddr;
          rnum++; 
        }
      }
      if_pf (rnum == 0) { // entire packet is empty, skip send
        gasnete_getv_AMPipeline_visop_signal(visop);
        continue;
      }

      #if GASNET_DEBUG
        // assert we don't send empty iovecs on the wire (bug3411)
        size_t datalen = 0; 
        for (size_t i=0; i < rnum; i++) {
          size_t const thislen = packedbuf[i].len;
          gasneti_assert(thislen > 0);
          datalen += thislen;
        }
        gasneti_assert(datalen > 0); 
      #endif

      /* send AM(visop) from packedbuf */
      GASNETI_SAFE(
        MEDIUM_REQ(2,3,(srcnode, gasneti_handleridx(gasnete_getv_AMPipeline_reqh),
                      packedbuf, rnum*sizeof(gasnet_memvec_t),
                      PACK(visop), packetidx)));
    }

    gasneti_free(remotept);
    GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
  }
}
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist) \
    if (gasnete_vis_use_ampipe && srccount > 1)                                                \
      return gasnete_getv_AMPipeline(synctype,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_getv_AMPipeline_reqh_inner)
void gasnete_getv_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, gasnet_handlerarg_t packetidx) {
  gasneti_assert(addr && nbytes > 0);
  gasnet_memvec_t * const rlist = addr;
  size_t const rnum = nbytes / sizeof(gasnet_memvec_t);
  gasneti_assert(nbytes == rnum * sizeof(gasnet_memvec_t));
  gasneti_vis_op_t * const visop = _visop;
  uint8_t * const packedbuf = gasneti_malloc(gasnet_AMMaxMedium());
  /* gather data payload from sourcelist into packet */
  uint8_t * const end = gasnete_memvec_pack_noempty(rnum, rlist, packedbuf, 0, (size_t)-1);
  size_t const repbytes = end - packedbuf;
  gasneti_assert(repbytes <= gasnet_AMMaxMedium());
  gasneti_assert(packedbuf);
  gasneti_assert(repbytes > 0);
  GASNETI_SAFE(
    MEDIUM_REP(2,3,(token, gasneti_handleridx(gasnete_getv_AMPipeline_reph),
                  packedbuf, repbytes,
                  PACK(visop),packetidx)));
  gasneti_free(packedbuf);
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reqh,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_getv_AMPipeline_reph_inner)
void gasnete_getv_AMPipeline_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, gasnet_handlerarg_t packetidx) {
  gasneti_vis_op_t * const visop = _visop;
  gasnet_memvec_t * const savedlst = (gasnet_memvec_t *)(visop + 1);
  gasnete_packetdesc_t * const lpacket = ((gasnete_packetdesc_t *)visop->addr) + (uint32_t)packetidx;
  size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
  gasneti_assert(visop->type == GASNETI_VIS_CAT_GETV_AMPIPELINE);
  gasneti_assert(lpacket->lastidx < visop->count);
  gasneti_assert(addr && nbytes > 0);
  { uint8_t *end = gasnete_memvec_unpack(lnum, savedlst+lpacket->firstidx, addr, lpacket->firstoffset, lpacket->lastlen);
    gasneti_assert(end - (uint8_t *)addr == nbytes);
  }
  gasnete_getv_AMPipeline_visop_signal(visop);
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reph,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
#endif
/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts */
gasnet_handle_t gasnete_putv_ref_indiv(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  const int islocal = (dstnode == gasneti_mynode);
  GASNETI_TRACE_EVENT(C, PUTV_REF_INDIV);
  gasneti_assert(srccount > 0 && dstcount > 0);
  GASNETE_START_NBIREGION(synctype, islocal);

  if (dstcount == 1) { /* dst is contiguous buffer */
    uintptr_t pdst = (uintptr_t)(dstlist[0].addr);
    size_t i;
    for (i = 0; i < srccount; i++) {
      const size_t srclen = srclist[i].len;
      if_pt (srclen > 0)
        GASNETE_PUT_INDIV(islocal, dstnode, (void *)pdst, srclist[i].addr, srclen);
      pdst += srclen;
    }
    gasneti_assert(pdst == (uintptr_t)(dstlist[0].addr)+dstlist[0].len);
  } else if (srccount == 1) { /* src is contiguous buffer */
    uintptr_t psrc = (uintptr_t)(srclist[0].addr);
    size_t i;
    for (i = 0; i < dstcount; i++) {
      const size_t dstlen = dstlist[i].len;
      if_pt (dstlen > 0)
        GASNETE_PUT_INDIV(islocal, dstnode, dstlist[i].addr, (void *)psrc, dstlen);
      psrc += dstlen;
    }
    gasneti_assert(psrc == (uintptr_t)(srclist[0].addr)+srclist[0].len);
  } else { /* general case */
    size_t srcidx = 0;
    size_t dstidx = 0;
    size_t srcoffset = 0;
    size_t dstoffset = 0;
    
    while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
    while (dstidx < dstcount && dstlist[dstidx].len == 0) dstidx++;
    while (srcidx < srccount) {
      const size_t srcremain = srclist[srcidx].len - srcoffset;
      const size_t dstremain = dstlist[dstidx].len - dstoffset;
      gasneti_assert(dstidx < dstcount);
      gasneti_assert(srcremain > 0 && dstremain > 0);
      if (srcremain < dstremain) {
        GASNETE_PUT_INDIV(islocal, dstnode, 
          (void *)(((uintptr_t)dstlist[dstidx].addr)+dstoffset), 
          (void *)(((uintptr_t)srclist[srcidx].addr)+srcoffset), 
          srcremain);
        srcidx++;
        while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
        srcoffset = 0;
        dstoffset += srcremain;
      } else {
        GASNETE_PUT_INDIV(islocal, dstnode, 
          (void *)(((uintptr_t)dstlist[dstidx].addr)+dstoffset), 
          (void *)(((uintptr_t)srclist[srcidx].addr)+srcoffset), 
          dstremain);
        dstidx++;
        while (dstidx < dstcount && dstlist[dstidx].len == 0) dstidx++;
        dstoffset = 0;
        if (srcremain == dstremain) {
          srcidx++;
          while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
          srcoffset = 0;
        } else srcoffset += dstremain;
      }
    } 
    gasneti_assert(srcidx == srccount && dstidx == dstcount && srcoffset == 0 && dstoffset == 0);
  }
  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}

/* reference version that uses individual gets */
gasnet_handle_t gasnete_getv_ref_indiv(gasnete_synctype_t synctype,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   gasnet_node_t srcnode,
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  const int islocal = (srcnode == gasneti_mynode);
  GASNETI_TRACE_EVENT(C, GETV_REF_INDIV);
  gasneti_assert(srccount > 0 && dstcount > 0);
  GASNETE_START_NBIREGION(synctype, islocal);

  if (dstcount == 1) { /* dst is contiguous buffer */
    uintptr_t pdst = (uintptr_t)(dstlist[0].addr);
    size_t i;
    for (i = 0; i < srccount; i++) {
      const size_t srclen = srclist[i].len;
      if_pt (srclen > 0)
        GASNETE_GET_INDIV(islocal, (void *)pdst, srcnode, srclist[i].addr, srclen);
      pdst += srclen;
    }
    gasneti_assert(pdst == (uintptr_t)(dstlist[0].addr)+dstlist[0].len);
  } else if (srccount == 1) { /* src is contiguous buffer */
    uintptr_t psrc = (uintptr_t)(srclist[0].addr);
    size_t i;
    for (i = 0; i < dstcount; i++) {
      const size_t dstlen = dstlist[i].len;
      if_pt (dstlen > 0)
        GASNETE_GET_INDIV(islocal, dstlist[i].addr, srcnode, (void *)psrc, dstlen);
      psrc += dstlen;
    }
    gasneti_assert(psrc == (uintptr_t)(srclist[0].addr)+srclist[0].len);
  } else { /* general case */
    size_t srcidx = 0;
    size_t dstidx = 0;
    size_t srcoffset = 0;
    size_t dstoffset = 0;
    
    while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
    while (dstidx < dstcount && dstlist[dstidx].len == 0) dstidx++;
    while (srcidx < srccount) {
      const size_t srcremain = srclist[srcidx].len - srcoffset;
      const size_t dstremain = dstlist[dstidx].len - dstoffset;
      gasneti_assert(dstidx < dstcount);
      gasneti_assert(srcremain > 0 && dstremain > 0);
      if (srcremain < dstremain) {
        GASNETE_GET_INDIV(islocal, 
          (void *)(((uintptr_t)dstlist[dstidx].addr)+dstoffset), 
          srcnode, 
          (void *)(((uintptr_t)srclist[srcidx].addr)+srcoffset), 
          srcremain);
        srcidx++;
        while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
        srcoffset = 0;
        dstoffset += srcremain;
      } else {
        GASNETE_GET_INDIV(islocal, 
          (void *)(((uintptr_t)dstlist[dstidx].addr)+dstoffset), 
          srcnode, 
          (void *)(((uintptr_t)srclist[srcidx].addr)+srcoffset), 
          dstremain);
        dstidx++;
        while (dstidx < dstcount && dstlist[dstidx].len == 0) dstidx++;
        dstoffset = 0;
        if (srcremain == dstremain) {
          srcidx++;
          while (srcidx < srccount && srclist[srcidx].len == 0) srcidx++;
          srcoffset = 0;
        } else srcoffset += dstremain;
      }
    } 
    gasneti_assert(srcidx == srccount && dstidx == dstcount && srcoffset == 0 && dstoffset == 0);
  }

  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}
/*---------------------------------------------------------------------------------*/
/* top-level gasnet_putv_* entry point */
#ifndef GASNETE_PUTV_OVERRIDE
extern gasnet_handle_t gasnete_putv(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  /* catch silly degenerate cases */
  if_pf (dstcount == 0 || srccount == 0) /* empty (may miss some cases) */
    return GASNET_INVALID_HANDLE; 
  if (dstcount + srccount <= 2 ||  /* fully contiguous */
      GASNETI_SUPERNODE_LOCAL(dstnode)) { /* purely local */ 
    return gasnete_putv_ref_indiv(synctype,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_PUTV_SELECTOR
    #define GASNETE_PUTV_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist)   \
      GASNETE_PUTV_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist); \
      GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist); \
      return gasnete_putv_ref_indiv(synctype,dstnode,dstcount,dstlist,srccount,srclist GASNETE_THREAD_PASS)
  #endif
  GASNETE_PUTV_SELECTOR(synctype,dstnode,dstcount,dstlist,srccount,srclist);
  gasneti_fatalerror("failure in GASNETE_PUTV_SELECTOR - should never reach here");
}
#endif
/* top-level gasnet_getv_* entry point */
#ifndef GASNETE_GETV_OVERRIDE
extern gasnet_handle_t gasnete_getv(gasnete_synctype_t synctype,
                                   size_t dstcount, gasnet_memvec_t const dstlist[], 
                                   gasnet_node_t srcnode,
                                   size_t srccount, gasnet_memvec_t const srclist[] GASNETE_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  /* catch silly degenerate cases */
  if_pf (dstcount == 0 || srccount == 0) /* empty (may miss some cases) */
    return GASNET_INVALID_HANDLE; 
  if (dstcount + srccount <= 2 ||  /* fully contiguous */
      GASNETI_SUPERNODE_LOCAL(srcnode)) { /* purely local */ 
    return gasnete_getv_ref_indiv(synctype,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_GETV_SELECTOR
    #define GASNETE_GETV_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist)    \
      GASNETE_GETV_SCATTER_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist); \
      GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist); \
      return gasnete_getv_ref_indiv(synctype,dstcount,dstlist,srcnode,srccount,srclist GASNETE_THREAD_PASS)
  #endif
  GASNETE_GETV_SELECTOR(synctype,dstcount,dstlist,srcnode,srccount,srclist);
  gasneti_fatalerror("failure in GASNETE_GETV_SELECTOR - should never reach here");
}
#endif
/*---------------------------------------------------------------------------------*/
