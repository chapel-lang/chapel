/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_vis_indexed.c,v $
 *     $Date: 2009/05/12 02:16:38 $
 * $Revision: 1.23 $
 * Description: GASNet Indexed implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_extended_refvis.c
#endif

/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_ADDRLIST_PACK(copy) {                                       \
  size_t i;                                                                  \
  uint8_t *ploc = (uint8_t *)buf;                                            \
  gasneti_assert(count > 0 && list && len > 0 && buf && first_offset < len); \
  if (last_len == (size_t)-1) last_len = len;                                \
  if (count == 1) {                                                          \
    copy(ploc, ((uint8_t*)list[0])+first_offset, last_len);                  \
    ploc += last_len;                                                        \
  } else {                                                                   \
    size_t firstlen = len - first_offset;                                    \
    copy(ploc, ((uint8_t*)list[0])+first_offset, firstlen);                  \
    ploc += firstlen;                                                        \
    for (i = 1; i < count-1; i++) {                                          \
      copy(ploc, list[i], len);                                              \
      ploc += len;                                                           \
    }                                                                        \
    copy(ploc, list[count-1], last_len);                                     \
    ploc += last_len;                                                        \
  }                                                                          \
  return ploc;                                                               \
}

/* pack a addrlist list into a contiguous buffer, using the provided byte offset into the first element
   if last_len is (size_t)-1, then last_len is ignored
   otherwise, last_len is used in place of len for the last entry
     (and is never adjusted based on first_offset, even if count == 1)
   return a pointer into the packed buffer, which points just after the last byte used
*/
void *gasnete_addrlist_pack(size_t count, void * const list[], size_t len, void *buf, 
                           size_t first_offset, size_t last_len) _GASNETE_ADDRLIST_PACK(_GASNETE_PACK_HELPER)
void *gasnete_addrlist_unpack(size_t count, void * const list[], size_t len, void const *buf, 
                             size_t first_offset, size_t last_len) _GASNETE_ADDRLIST_PACK(_GASNETE_UNPACK_HELPER)

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
size_t gasnete_packetize_addrlist(size_t remotecount, size_t remotelen,
                                  size_t localcount, size_t locallen,
                                  gasnete_packetdesc_t **premotept,
                                  gasnete_packetdesc_t **plocalpt,
                                  size_t maxpayload, int sharedpacket) {
  size_t ptidx;
  int done = 0;
  size_t ridx = 0, roffset = 0, lidx = 0, loffset = 0;
  size_t const metadatasz = sizeof(void *);
  size_t const runit = (sharedpacket ? metadatasz + remotelen : MAX(metadatasz,remotelen));
  size_t ptsz = (runit <= maxpayload ? /* conservative upper bound on packet count */
                 remotecount / (maxpayload / runit) + 1 : 
                 remotelen*remotecount / (maxpayload - 2*metadatasz) + 1); 
  gasnete_packetdesc_t *remotept = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasnete_packetdesc_t *localpt = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasneti_assert(premotept && plocalpt && remotecount && remotelen && localcount && locallen);
  gasneti_assert(remotecount*remotelen == localcount*locallen);
  gasneti_assert(remotecount*remotelen > 0);

  for (ptidx = 0; ; ptidx++) {
    ssize_t packetremain = maxpayload;
    ssize_t packetdata = 0;
    size_t rdatasz, ldatasz; 

    gasneti_assert(ptidx < ptsz);

    /* begin remote packet */
    remotept[ptidx].firstidx = ridx;
    remotept[ptidx].firstoffset = roffset;
    /* begin local packet */
    if_pf (lidx == localcount) localpt[ptidx].firstidx = lidx-1; 
    else                       localpt[ptidx].firstidx = lidx;
    localpt[ptidx].firstoffset = loffset;

    if (roffset > 0) { /* initial partial entry */
      gasneti_assert(roffset < remotelen);
      rdatasz = remotelen - roffset; /* data left in current entry */
      /* try to add the entire entry to packet */
      if (sharedpacket) packetremain -= (metadatasz + rdatasz);
      else              packetremain -= MAX(metadatasz, rdatasz);
      if (packetremain < 0) { /* overflowed - finished a packet, and spill to next */
        rdatasz += packetremain; /* compute truncated datasz that fits in this packet */
        roffset += rdatasz; /* update offset into current entry */
        packetdata += rdatasz;
        goto rend;
      } else {
        packetdata += rdatasz;
        roffset = 0; /* finished an entry */
        ridx++;
        if (ridx == remotecount) { done = 1; goto rend; } /* done - this is last packet */
      }
    }
    if (packetremain >= runit) { /* whole entries */
      size_t numunits = packetremain / runit;
      if (ridx + numunits > remotecount) numunits = remotecount - ridx;
      rdatasz = remotelen;
      packetremain -= runit*numunits;
      packetdata += remotelen*numunits;
      ridx += numunits;
      gasneti_assert(roffset == 0);
      if (ridx == remotecount) { done = 1; goto rend; } /* done - this is last packet */
    }
    if (packetremain > metadatasz) { /* trailing partial entry */
      gasneti_assert(packetremain < runit);
      if (sharedpacket) rdatasz = packetremain - metadatasz;
      else              rdatasz = packetremain;
      packetdata += rdatasz;
      roffset = rdatasz;
    }
    rend:
    /* end remote packet */
    if (roffset == 0) remotept[ptidx].lastidx = ridx-1;
    else              remotept[ptidx].lastidx = ridx;
    remotept[ptidx].lastlen = rdatasz;

    #if GASNET_DEBUG /* verify packing properties */
      gasnete_packetize_verify(remotept, ptidx, done, remotecount, remotelen, 0);
      { size_t datachk = 0, i;
        size_t entries = remotept[ptidx].lastidx - remotept[ptidx].firstidx + 1;
        for (i = remotept[ptidx].firstidx; i <= remotept[ptidx].lastidx; i++) {
          if (i == remotept[ptidx].lastidx) datachk += remotept[ptidx].lastlen;
          else if (i == remotept[ptidx].firstidx) datachk += (remotelen - remotept[ptidx].firstoffset);
          else datachk += remotelen;
        }
        gasneti_assert(packetdata == datachk);
        if (sharedpacket) { 
          gasneti_assert((metadatasz*entries + packetdata) <= maxpayload); /* not overfull */
          gasneti_assert(((metadatasz*entries + packetdata) >= maxpayload - metadatasz) || done); /* not underfull */
        } else {
          gasneti_assert(MAX(metadatasz*entries,packetdata) <= maxpayload); /* not overfull */
          gasneti_assert((MAX(metadatasz*entries,packetdata) >= maxpayload - 2*metadatasz) || done); /* not underfull */
        }
      }
    #endif

    ldatasz = 0;
    if (loffset > 0) { /* initial partial entry */
      gasneti_assert(loffset < locallen);
      ldatasz = locallen - loffset; /* data left in current entry */
      packetdata -= ldatasz;
      if (packetdata < 0) { /* overflowed - this entry spills into next packet */
        ldatasz += packetdata; /* compute truncated datasz that fits in this packet */
        loffset += ldatasz; /* update offset into current entry */
        packetdata = 0;
      } else {
        loffset = 0; /* finished an entry */
        lidx++;
        gasneti_assert(lidx < localcount || (lidx == localcount && packetdata == 0));
      }
    }
    if (packetdata >= locallen) { /* whole entries */
      size_t numunits = packetdata / locallen;
      if (lidx + numunits > localcount) numunits = localcount - lidx;
      ldatasz = locallen;
      packetdata -= locallen*numunits;
      lidx += numunits;
      gasneti_assert(lidx < localcount || (lidx == localcount && packetdata == 0));
      gasneti_assert(loffset == 0);
    }
    if (packetdata > 0) { /* trailing partial entry */
      gasneti_assert(packetdata < locallen);
      ldatasz = packetdata;
      loffset = ldatasz;
    }
    /* end local packet */
    if (loffset == 0) localpt[ptidx].lastidx = lidx-1;
    else              localpt[ptidx].lastidx = lidx;
    localpt[ptidx].lastlen = ldatasz;

    #if GASNET_DEBUG /* verify packing properties */
      gasnete_packetize_verify(localpt, ptidx, done, localcount, locallen, 0);
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
/* ***  Indexed *** */
/*---------------------------------------------------------------------------------*/

/* simple gather put, remotely contiguous */
#ifndef GASNETE_PUTI_GATHER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_puti_gather(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode, 
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = dstlen;
  gasneti_assert(dstcount == 1 && srccount > 1); /* only supports gather put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, PUTI_GATHER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_addrlist_pack(srccount, srclist, srclen, packedbuf, 0, (size_t)-1);
    visop->type = GASNETI_VIS_CAT_PUTI_GATHER;
    visop->handle = gasnete_put_nb_bulk(dstnode, dstlist[0], packedbuf, nbytes GASNETE_THREAD_PASS);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0);
  }
}
  #define GASNETE_PUTI_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
    if (gasnete_vis_use_remotecontig && dstcount == 1 && srccount > 1)                                   \
      return gasnete_puti_gather(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTI_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETI_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gasnet_handle_t gasnete_geti_scatter(gasnete_synctype_t synctype,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gasnet_node_t srcnode,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = srclen;
  gasneti_assert(srccount == 1 && dstcount > 1); /* only supports scatter get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, GETI_SCATTER);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t)+dstcount*sizeof(void *)+nbytes);
    void * * const savedlst = (void * *)(visop + 1);
    void * const packedbuf = (void *)(savedlst + dstcount);
    memcpy(savedlst, dstlist, dstcount*sizeof(void *));
    visop->type = GASNETI_VIS_CAT_GETI_SCATTER;
    visop->count = dstcount;
    visop->len = dstlen;
    visop->handle = gasnete_get_nb_bulk(packedbuf, srcnode, srclist[0], nbytes GASNETE_THREAD_PASS);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1);
  }
}
  #define GASNETE_GETI_SCATTER_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) \
    if (gasnete_vis_use_remotecontig && srccount == 1 && dstcount > 1)                                    \
      return gasnete_geti_scatter(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETI_SCATTER_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) ((void)0)
#endif
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTI_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gasnet_handle_t gasnete_puti_AMPipeline(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode, 
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasneti_assert(dstcount > 1); /* supports scatter put */
  gasneti_assert(dstnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, PUTI_AMPIPELINE);
  GASNETE_START_NBIREGION(synctype, 0);

  { void * * packedbuf = gasneti_malloc(gasnet_AMMaxMedium());
    gasnete_packetdesc_t *remotept;
    gasnete_packetdesc_t *localpt;
    size_t packetidx;
    size_t const packetcnt = gasnete_packetize_addrlist(dstcount, dstlen, srccount, srclen, 
                                                &remotept, &localpt, gasnet_AMMaxMedium(), 1);
    gasneti_iop_t *iop = gasneti_iop_register(packetcnt,0 GASNETE_THREAD_PASS);

    for (packetidx = 0; packetidx < packetcnt; packetidx++) {
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      gasnete_packetdesc_t * const lpacket = &localpt[packetidx];
      size_t const rnum = rpacket->lastidx - rpacket->firstidx + 1;
      size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
      uint8_t *end;
      /* fill packet with remote metadata */
      memcpy(packedbuf, &dstlist[rpacket->firstidx], rnum*sizeof(void *));
      /* gather data payload from sourcelist into packet */
      end = gasnete_addrlist_pack(lnum, &srclist[lpacket->firstidx], srclen, &packedbuf[rnum], 
                                  lpacket->firstoffset, lpacket->lastlen);

      /* send AM(rnum, iop) from packedbuf */
      GASNETI_SAFE(
        MEDIUM_REQ(5,6,(dstnode, gasneti_handleridx(gasnete_puti_AMPipeline_reqh),
                      packedbuf, end - (uint8_t *)packedbuf,
                      PACK(iop), rnum, dstlen, rpacket->firstoffset, rpacket->lastlen)));
    }

    gasneti_free(remotept);
    gasneti_free(localpt);
    gasneti_free(packedbuf);
    GASNETE_END_NBIREGION_AND_RETURN(synctype, 0);
  }
}
  #define GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) \
    if (gasnete_vis_use_ampipe && dstcount > 1 && dstlen == (uint32_t)(dstlen) &&                            \
        (srclen <= gasnete_vis_maxchunk || dstlen <= gasnete_vis_maxchunk))                                  \
      return gasnete_puti_AMPipeline(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS)
#else
  #define GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_puti_AMPipeline_reqh_inner)
void gasnete_puti_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *iop, gasnet_handlerarg_t rnum, 
  gasnet_handlerarg_t dstlen, gasnet_handlerarg_t firstoffset, gasnet_handlerarg_t lastlen) {
  void * const * const rlist = addr;
  uint8_t * const data = (uint8_t *)(&rlist[rnum]);
  GASNETI_UNUSED_UNLESS_DEBUG /* but still need side-effects */
  uint8_t * const end = gasnete_addrlist_unpack(rnum, rlist, dstlen, data, firstoffset, lastlen);
  gasneti_assert(end - (uint8_t *)addr <= gasnet_AMMaxMedium());
  gasneti_sync_writes();
  /* TODO: coalesce acknowledgements - need a per-srcnode, per-op seqnum & packetcnt */
  GASNETI_SAFE(
    SHORT_REP(1,2,(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph),
                  PACK(iop))));
}
MEDIUM_HANDLER(gasnete_puti_AMPipeline_reqh,5,6, 
              (token,addr,nbytes, UNPACK(a0),      a1,a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), a2,a3,a4,a5));
#endif
/* ------------------------------------------------------------------------------------ */
/* Pipelined AM gather-scatter get */
#ifndef GASNETE_GETI_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gasnet_handle_t gasnete_geti_AMPipeline(gasnete_synctype_t synctype,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gasnet_node_t srcnode,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasneti_assert(srccount > 1); /* supports gather get */
  gasneti_assert(srcnode != gasneti_mynode); /* silly to use for local cases */
  GASNETI_TRACE_EVENT(C, GETI_AMPIPELINE);

  { gasneti_vis_op_t * const visop = gasneti_malloc(sizeof(gasneti_vis_op_t) +
                                                    dstcount*sizeof(void *) + 
                                                    gasnet_AMMaxMedium());
    void * * const savedlst = (void * *)(visop + 1);
    void * * const packedbuf = savedlst + dstcount;
    gasnete_packetdesc_t *remotept;
    gasnete_packetdesc_t *localpt;
    gasneti_eop_t *eop;
    size_t packetidx;
    size_t const packetcnt = gasnete_packetize_addrlist(srccount, srclen, dstcount, dstlen,  
                                                &remotept, &localpt, gasnet_AMMaxMedium(), 0);
    GASNETE_VISOP_SETUP(visop, synctype, 1);
    #if GASNET_DEBUG
      visop->type = GASNETI_VIS_CAT_GETI_AMPIPELINE;
      visop->count = dstcount;
    #endif
    gasneti_assert(packetcnt <= GASNETI_ATOMIC_MAX);
    gasneti_assert(packetcnt == (gasnet_handlerarg_t)packetcnt);
    visop->len = dstlen;
    visop->addr = localpt;
    memcpy(savedlst, dstlist, dstcount*sizeof(void *));
    gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);
    eop = visop->eop; /* visop may disappear once the last AM is launched */

    for (packetidx = 0; packetidx < packetcnt; packetidx++) {
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      size_t const rnum = rpacket->lastidx - rpacket->firstidx + 1;
      /* fill packet with remote metadata */
      memcpy(packedbuf, &srclist[rpacket->firstidx], rnum*sizeof(void *));

      /* send AM(visop) from packedbuf */
      GASNETI_SAFE(
        MEDIUM_REQ(5,6,(srcnode, gasneti_handleridx(gasnete_geti_AMPipeline_reqh),
                      packedbuf, rnum*sizeof(void *),
                      PACK(visop), packetidx, srclen, rpacket->firstoffset, rpacket->lastlen)));
    }

    gasneti_free(remotept);
    GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
  }
}
  #define GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) \
    if (gasnete_vis_use_ampipe && srccount > 1 &&                                                            \
        (srclen <= gasnete_vis_maxchunk || dstlen <= gasnete_vis_maxchunk))                                  \
      return gasnete_geti_AMPipeline(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS)
#else
  #define GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_geti_AMPipeline_reqh_inner)
void gasnete_geti_AMPipeline_reqh_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, gasnet_handlerarg_t packetidx,
  gasnet_handlerarg_t dstlen, gasnet_handlerarg_t firstoffset, gasnet_handlerarg_t lastlen) {
  void * const * const rlist = addr;
  size_t const rnum = nbytes / sizeof(void *);
  uint8_t * const packedbuf = gasneti_malloc(gasnet_AMMaxMedium());
  /* gather data payload from sourcelist into packet */
  uint8_t * const end = gasnete_addrlist_pack(rnum, rlist, dstlen, packedbuf, firstoffset, lastlen);
  size_t const repbytes = end - packedbuf;
  gasneti_assert(repbytes <= gasnet_AMMaxMedium());
  GASNETI_SAFE(
    MEDIUM_REP(2,3,(token, gasneti_handleridx(gasnete_geti_AMPipeline_reph),
                  packedbuf, repbytes,
                  PACK(_visop),packetidx)));
  gasneti_free(packedbuf);
}
MEDIUM_HANDLER(gasnete_geti_AMPipeline_reqh,5,6, 
              (token,addr,nbytes, UNPACK(a0),      a1,a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), a2,a3,a4,a5));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_geti_AMPipeline_reph_inner)
void gasnete_geti_AMPipeline_reph_inner(gasnet_token_t token, 
  void *addr, size_t nbytes,
  void *_visop, gasnet_handlerarg_t packetidx) {
  gasneti_vis_op_t * const visop = _visop;
  void * const * const savedlst = (void * *)(visop + 1);
  gasnete_packetdesc_t * const lpacket = ((gasnete_packetdesc_t *)visop->addr) + packetidx;
  size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
  gasneti_assert(visop->type == GASNETI_VIS_CAT_GETI_AMPIPELINE);
  gasneti_assert(lpacket->lastidx < visop->count);
  { GASNETI_UNUSED_UNLESS_DEBUG /* but still need side-effects */
    uint8_t * const end = gasnete_addrlist_unpack(lnum, savedlst+lpacket->firstidx, visop->len, addr, lpacket->firstoffset, lpacket->lastlen);
    gasneti_assert(end - (uint8_t *)addr == nbytes);
  }
  if (gasneti_weakatomic_decrement_and_test(&(visop->packetcnt), 
                                            GASNETI_ATOMIC_WMB_PRE|GASNETI_ATOMIC_WEAK_FENCE)) {
    /* last response packet completes operation and cleans up */
    GASNETE_VISOP_SIGNAL(visop, 1);
    gasneti_free(visop->addr); /* free localpt */
    gasneti_free(visop); /* free visop, savedlst and send buffer */
  }
}
MEDIUM_HANDLER(gasnete_geti_AMPipeline_reph,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
#endif
/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts */
gasnet_handle_t gasnete_puti_ref_indiv(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode, 
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  const int islocal = (dstnode == gasneti_mynode);
  GASNETI_TRACE_EVENT(C, PUTI_REF_INDIV);
  gasneti_assert(srccount > 0 && dstcount > 0 && ((uintptr_t)dstcount)*dstlen == ((uintptr_t)srccount)*srclen);
  gasneti_assert(srclen > 0 && dstlen > 0);
  GASNETE_START_NBIREGION(synctype, islocal);

  if (dstlen == srclen) { /* matched sizes (fast case) */
    size_t i;
    gasneti_assert(dstcount == srccount);
    for (i = 0; i < dstcount; i++) {
      GASNETE_PUT_INDIV(islocal, dstnode, dstlist[i], srclist[i], dstlen);
    }
  } else if (dstcount == 1) { /* dst is contiguous buffer */
    uintptr_t pdst = (uintptr_t)(dstlist[0]);
    size_t i;
    for (i = 0; i < srccount; i++) {
      GASNETE_PUT_INDIV(islocal, dstnode, (void *)pdst, srclist[i], srclen);
      pdst += srclen;
    }
    gasneti_assert(pdst == (uintptr_t)(dstlist[0])+dstlen);
  } else if (srccount == 1) { /* src is contiguous buffer */
    uintptr_t psrc = (uintptr_t)(srclist[0]);
    size_t i;
    for (i = 0; i < dstcount; i++) {
      GASNETE_PUT_INDIV(islocal, dstnode, dstlist[i], (void *)psrc, dstlen);
      psrc += dstlen;
    }
    gasneti_assert(psrc == (uintptr_t)(srclist[0])+srclen);
  } else { /* mismatched sizes (general case) */
    size_t srcidx = 0;
    size_t dstidx = 0;
    size_t srcoffset = 0;
    size_t dstoffset = 0;
    while (srcidx < srccount) {
      const size_t srcremain = srclen - srcoffset;
      const size_t dstremain = dstlen - dstoffset;
      gasneti_assert(dstidx < dstcount);
      gasneti_assert(srcremain > 0 && dstremain > 0);
      if (srcremain < dstremain) {
        GASNETE_PUT_INDIV(islocal, dstnode, 
          (void *)(((uintptr_t)dstlist[dstidx])+dstoffset), 
          (void *)(((uintptr_t)srclist[srcidx])+srcoffset), 
          srcremain);
        srcidx++;
        srcoffset = 0;
        dstoffset += srcremain;
      } else {
        GASNETE_PUT_INDIV(islocal, dstnode, 
          (void *)(((uintptr_t)dstlist[dstidx])+dstoffset), 
          (void *)(((uintptr_t)srclist[srcidx])+srcoffset), 
          dstremain);
        dstidx++;
        dstoffset = 0;
        if (srcremain == dstremain) {
          srcidx++;
          srcoffset = 0;
        } else srcoffset += dstremain;
      }
    } 
    gasneti_assert(srcidx == srccount && dstidx == dstcount && srcoffset == 0 && dstoffset == 0);
  }

  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}

/* reference version that uses individual gets */
gasnet_handle_t gasnete_geti_ref_indiv(gasnete_synctype_t synctype,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gasnet_node_t srcnode,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  const int islocal = (srcnode == gasneti_mynode);
  GASNETI_TRACE_EVENT(C, GETI_REF_INDIV);
  gasneti_assert(srccount > 0 && dstcount > 0 && ((uintptr_t)dstcount)*dstlen == ((uintptr_t)srccount)*srclen);
  gasneti_assert(srclen > 0 && dstlen > 0);
  GASNETE_START_NBIREGION(synctype, islocal);

  if (dstlen == srclen) { /* matched sizes (fast case) */
    size_t i;
    gasneti_assert(dstcount == srccount);
    for (i = 0; i < dstcount; i++) {
      GASNETE_GET_INDIV(islocal, dstlist[i], srcnode, srclist[i], dstlen);
    }
  } else if (dstcount == 1) { /* dst is contiguous buffer */
    uintptr_t pdst = (uintptr_t)(dstlist[0]);
    size_t i;
    for (i = 0; i < srccount; i++) {
      GASNETE_GET_INDIV(islocal, (void *)pdst, srcnode, srclist[i], srclen);
      pdst += srclen;
    }
    gasneti_assert(pdst == (uintptr_t)(dstlist[0])+dstlen);
  } else if (srccount == 1) { /* src is contiguous buffer */
    uintptr_t psrc = (uintptr_t)(srclist[0]);
    size_t i;
    for (i = 0; i < dstcount; i++) {
      GASNETE_GET_INDIV(islocal, dstlist[i], srcnode, (void *)psrc, dstlen);
      psrc += dstlen;
    }
    gasneti_assert(psrc == (uintptr_t)(srclist[0])+srclen);
  } else { /* mismatched sizes (general case) */
    size_t srcidx = 0;
    size_t dstidx = 0;
    size_t srcoffset = 0;
    size_t dstoffset = 0;
    while (srcidx < srccount) {
      const size_t srcremain = srclen - srcoffset;
      const size_t dstremain = dstlen - dstoffset;
      gasneti_assert(dstidx < dstcount);
      gasneti_assert(srcremain > 0 && dstremain > 0);
      if (srcremain < dstremain) {
        GASNETE_GET_INDIV(islocal, 
          (void *)(((uintptr_t)dstlist[dstidx])+dstoffset), 
          srcnode, 
          (void *)(((uintptr_t)srclist[srcidx])+srcoffset), 
          srcremain);
        srcidx++;
        srcoffset = 0;
        dstoffset += srcremain;
      } else {
        GASNETE_GET_INDIV(islocal,  
          (void *)(((uintptr_t)dstlist[dstidx])+dstoffset), 
          srcnode, 
          (void *)(((uintptr_t)srclist[srcidx])+srcoffset), 
          dstremain);
        dstidx++;
        dstoffset = 0;
        if (srcremain == dstremain) {
          srcidx++;
          srcoffset = 0;
        } else srcoffset += dstremain;
      }
    } 
    gasneti_assert(srcidx == srccount && dstidx == dstcount && srcoffset == 0 && dstoffset == 0);
  }

  GASNETE_END_NBIREGION_AND_RETURN(synctype, islocal);
}

/*---------------------------------------------------------------------------------*/
/* reference version that uses vector interface */
gasnet_handle_t gasnete_puti_ref_vector(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode, 
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnet_memvec_t *newdstlist = gasneti_malloc(sizeof(gasnet_memvec_t)*dstcount);
  gasnet_memvec_t *newsrclist = gasneti_malloc(sizeof(gasnet_memvec_t)*srccount);
  gasnet_handle_t retval;
  size_t i;
  GASNETI_TRACE_EVENT(C, PUTI_REF_VECTOR);
  gasneti_assert(GASNETE_PUTV_ALLOWS_VOLATILE_METADATA);
  for (i=0; i < dstcount; i++) {
    newdstlist[i].addr = dstlist[i];
    newdstlist[i].len = dstlen;
  }
  for (i=0; i < srccount; i++) {
    newsrclist[i].addr = srclist[i];
    newsrclist[i].len = srclen;
  }
  retval = gasnete_putv(synctype,dstnode,dstcount,newdstlist,srccount,newsrclist GASNETE_THREAD_PASS);
  gasneti_free(newdstlist);
  gasneti_free(newsrclist);
  return retval;
}
/* reference version that uses vector interface */
gasnet_handle_t gasnete_geti_ref_vector(gasnete_synctype_t synctype,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gasnet_node_t srcnode,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasnet_memvec_t *newdstlist = gasneti_malloc(sizeof(gasnet_memvec_t)*dstcount);
  gasnet_memvec_t *newsrclist = gasneti_malloc(sizeof(gasnet_memvec_t)*srccount);
  gasnet_handle_t retval;
  size_t i;
  GASNETI_TRACE_EVENT(C, GETI_REF_VECTOR);
  gasneti_assert(GASNETE_GETV_ALLOWS_VOLATILE_METADATA);
  for (i=0; i < dstcount; i++) {
    newdstlist[i].addr = dstlist[i];
    newdstlist[i].len = dstlen;
  }
  for (i=0; i < srccount; i++) {
    newsrclist[i].addr = srclist[i];
    newsrclist[i].len = srclen;
  }
  retval = gasnete_getv(synctype,dstcount,newdstlist,srcnode,srccount,newsrclist GASNETE_THREAD_PASS);
  gasneti_free(newdstlist);
  gasneti_free(newsrclist);
  return retval;
}

/*---------------------------------------------------------------------------------*/
/* top-level gasnet_puti_* entry point */
#ifndef GASNETE_PUTI_OVERRIDE
extern gasnet_handle_t gasnete_puti(gasnete_synctype_t synctype,
                                   gasnet_node_t dstnode, 
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  /* catch silly degenerate cases */
  if_pf (dstcount + srccount <= 2 ||  /* empty or fully contiguous */
         dstnode == gasneti_mynode) { /* purely local */ 
    if (dstcount == 0) return GASNET_INVALID_HANDLE;
    else return gasnete_puti_ref_indiv(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_PUTI_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_PUTI_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen) do {                        \
        switch (rand() % 3) {                                                                                                     \
          case 0:                                                                                                                 \
            GASNETE_PUTI_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);                       \
          case 1:                                                                                                                 \
            GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);                   \
          case 2:                                                                                                                 \
            return gasnete_puti_ref_indiv(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS);  \
          case 3:                                                                                                                 \
            return gasnete_puti_ref_vector(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS); \
        } } while (0)
    #else
      #define GASNETE_PUTI_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen)       \
        GASNETE_PUTI_GATHER_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);     \
        GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen); \
        return gasnete_puti_ref_indiv(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen GASNETE_THREAD_PASS)
    #endif
  #endif
  GASNETE_PUTI_SELECTOR(synctype,dstnode,dstcount,dstlist,dstlen,srccount,srclist,srclen);
  gasneti_fatalerror("failure in GASNETE_PUTI_SELECTOR - should never reach here");
  return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */
}
#endif
/* top-level gasnet_geti_* entry point */
#ifndef GASNETE_GETI_OVERRIDE
extern gasnet_handle_t gasnete_geti(gasnete_synctype_t synctype,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gasnet_node_t srcnode,
                                   size_t srccount, void * const srclist[], size_t srclen GASNETE_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  /* catch silly degenerate cases */
  if_pf (dstcount + srccount <= 2 ||  /* empty or fully contiguous */
         srcnode == gasneti_mynode) { /* purely local */ 
    if (dstcount == 0) return GASNET_INVALID_HANDLE;
    else return gasnete_geti_ref_indiv(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS);
  }

  /* select algorithm */
  #ifndef GASNETE_GETI_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_GETI_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen) do {                        \
        switch (rand() % 3) {                                                                                                     \
          case 0:                                                                                                                 \
            GASNETE_GETI_SCATTER_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen);                      \
          case 1:                                                                                                                 \
            GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen);                   \
          case 2:                                                                                                                 \
            return gasnete_geti_ref_indiv(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS);  \
          case 3:                                                                                                                 \
            return gasnete_geti_ref_vector(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS); \
        } } while (0)
    #else
      #define GASNETE_GETI_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen)       \
        GASNETE_GETI_SCATTER_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen);    \
        GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen); \
        return gasnete_geti_ref_indiv(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen GASNETE_THREAD_PASS)
    #endif
  #endif
  GASNETE_GETI_SELECTOR(synctype,dstcount,dstlist,dstlen,srcnode,srccount,srclist,srclen);
  gasneti_fatalerror("failure in GASNETE_GETI_SELECTOR - should never reach here");
  return GASNET_INVALID_HANDLE; /* avoid warning on MIPSPro */
}
#endif
