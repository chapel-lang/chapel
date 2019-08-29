/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_indexed.c $
 * Description: GASNet Indexed implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_refvis.c
#endif

/*---------------------------------------------------------------------------------*/
/* packing/unpacking helpers */
#define _GASNETE_ADDRLIST_PACK(copy) {                                       \
  size_t i;                                                                  \
  uint8_t *ploc = (uint8_t *)buf;                                            \
  gasneti_assert(count > 0 && list && len > 0 && buf);                       \
  gasneti_assert_uint(first_offset ,<, len);                                 \
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
  gasneti_assert_uint(maxpayload ,>, metadatasz);
  gasneti_assert_uint(remotecount*remotelen ,==, localcount*locallen);
  gasneti_assert(remotecount*remotelen > 0);

  for (ptidx = 0; ; ptidx++) {
    ssize_t packetremain = maxpayload;
    ssize_t packetdata = 0;
    size_t ldatasz; 
    size_t rdatasz = 0; // init to avoid a warning on gcc -O3 -Wall

    gasneti_assert_uint(ptidx ,<, ptsz);

    /* begin remote packet */
    remotept[ptidx].firstidx = ridx;
    remotept[ptidx].firstoffset = roffset;
    /* begin local packet */
    if_pf (lidx == localcount) localpt[ptidx].firstidx = lidx-1; 
    else                       localpt[ptidx].firstidx = lidx;
    localpt[ptidx].firstoffset = loffset;

    if (roffset > 0) { /* initial partial entry */
      gasneti_assert_uint(roffset ,<, remotelen);
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
      gasneti_assert_int(packetremain ,<, runit);
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
        gasneti_assert_uint(packetdata ,==, datachk);
        if (sharedpacket) { 
          gasneti_assert_int((metadatasz*entries + packetdata) ,<=, maxpayload); /* not overfull */
          if (!done) gasneti_assert_int((metadatasz*entries + packetdata) ,>=, maxpayload - metadatasz); /* not underfull */
        } else {
          gasneti_assert_int(MAX(metadatasz*entries,packetdata) ,<=, maxpayload); /* not overfull */
          if (!done) gasneti_assert_int(MAX(metadatasz*entries,packetdata) ,>=, maxpayload - 2*metadatasz); /* not underfull */
        }
      }
    #endif

    ldatasz = 0;
    if (loffset > 0) { /* initial partial entry */
      gasneti_assert_uint(loffset ,<, locallen);
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
      gasneti_assert_uint(packetdata ,<, locallen);
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
gex_Event_t gasnete_puti_gather(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = dstlen;
  gasneti_assert(dstcount == 1 && srccount > 1); /* only supports gather put */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, PUTI_GATHER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+nbytes);
    void * const packedbuf = visop + 1;
    gasnete_addrlist_pack(srccount, srclist, srclen, packedbuf, 0, (size_t)-1);
    visop->type = GASNETI_VIS_CAT_PUTI_GATHER;
    visop->event = gasnete_put_nb(tm, rank, dstlist[0], packedbuf, nbytes, GEX_EVENT_DEFER, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 0, (void)0);
  }
}
  #define GASNETE_PUTI_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    if (gasnete_vis_use_remotecontig && dstcount == 1 && srccount > 1)                                         \
      return gasnete_puti_gather(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTI_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) ((void)0)
#endif
#endif

/* simple scatter get, remotely contiguous */
#ifndef GASNETE_GETI_SCATTER_SELECTOR
#if GASNETE_USE_REMOTECONTIG_GATHER_SCATTER
gex_Event_t gasnete_geti_scatter(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = srclen;
  gasneti_assert(srccount == 1 && dstcount > 1); /* only supports scatter get */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  gasneti_assert(nbytes > 0);
  GASNETI_TRACE_EVENT(C, GETI_SCATTER);

  { gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t)+dstcount*sizeof(void *)+nbytes);
    void * * const savedlst = (void * *)(visop + 1);
    void * const packedbuf = (void *)(savedlst + dstcount);
    GASNETI_MEMCPY(savedlst, dstlist, dstcount*sizeof(void *));
    visop->type = GASNETI_VIS_CAT_GETI_SCATTER;
    visop->count = dstcount;
    visop->len = dstlen;
    visop->event = gasnete_get_nb(tm, packedbuf, rank, srclist[0], nbytes, 0 GASNETI_THREAD_PASS);
    gasneti_assert(visop->event != GEX_EVENT_INVALID);
    GASNETE_PUSH_VISOP_RETURN(td, visop, synctype, 1, (void)0);
  }
}
  #define GASNETE_GETI_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    if (gasnete_vis_use_remotecontig && srccount == 1 && dstcount > 1)                                          \
      return gasnete_geti_scatter(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETI_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) ((void)0)
#endif
#endif
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTI_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gex_Event_t gasnete_puti_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(dstcount > 1); /* supports scatter put */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, PUTI_AMPIPELINE);
  GASNETE_START_NBIREGION(synctype);

  size_t const maxpacket = gex_AM_MaxRequestMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                   (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                   HARGS(5,6));
  gasnete_packetdesc_t *remotept;
  gasnete_packetdesc_t *localpt;
  size_t const packetcnt = gasnete_packetize_addrlist(dstcount, dstlen, srccount, srclen, 
                                                &remotept, &localpt,
                                                maxpacket,
                                                1);
  gasneti_iop_t *iop = gasneti_iop_register(packetcnt,0 GASNETI_THREAD_PASS);

  #if GASNETE_VIS_NPAM == 0
    void * * const packedbuf = gasnete_visbuf_malloc(maxpacket);
  #endif

  for (size_t packetidx = 0; packetidx < packetcnt; packetidx++) {
    #if GASNETE_VIS_NPAM  // NPAM 1 or 2 (currently treated as 1)
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, maxpacket, maxpacket, NULL, 0, HARGS(5,6));
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxpacket);
      void * * const packedbuf = gex_AM_SrcDescAddr(sd);
    #endif
     
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      gasnete_packetdesc_t * const lpacket = &localpt[packetidx];
      size_t const rnum = rpacket->lastidx - rpacket->firstidx + 1;
      size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
      /* fill packet with remote metadata */
      GASNETI_MEMCPY(packedbuf, &dstlist[rpacket->firstidx], rnum*sizeof(void *));
      /* gather data payload from sourcelist into packet */
      uint8_t * const end = gasnete_addrlist_pack(lnum, &srclist[lpacket->firstidx], srclen, &packedbuf[rnum], 
                                  lpacket->firstoffset, lpacket->lastlen);
      size_t const nbytes = end - (uint8_t *)packedbuf;
      gasneti_assert_uint(nbytes ,<=, maxpacket);

      /* send AM(rnum, iop) from packedbuf */
    #define ARGS PACK(iop), rnum, dstlen, rpacket->firstoffset, rpacket->lastlen
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_puti_AMPipeline_reqh),
                               packedbuf, nbytes, GEX_EVENT_NOW, 0, ARGS);
    #else
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_puti_AMPipeline_reqh), nbytes, ARGS);
    #endif
    #undef ARGS
  }

  gasneti_free(remotept);
  gasneti_free(localpt);
  #if GASNETE_VIS_NPAM == 0
    gasneti_free(packedbuf);
  #endif
  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}
  #define GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    if (dstcount > 1 && GASNETT_PREDICT_TRUE(dstlen == (uint32_t)(dstlen)) &&                                      \
        MIN(srclen,dstlen) <= gasnete_vis_put_maxchunk)                                                            \
      return gasnete_puti_AMPipeline(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_puti_AMPipeline_reqh_inner)
void gasnete_puti_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *iop, gex_AM_Arg_t rnum,
  gex_AM_Arg_t dstlen, gex_AM_Arg_t firstoffset, gex_AM_Arg_t lastlen) {
  void * const * const rlist = addr;
  uint8_t * const data = (uint8_t *)(&rlist[rnum]);
  uint8_t * const end = gasnete_addrlist_unpack(rnum, rlist, dstlen, data, firstoffset, lastlen);
  gasneti_assert_uint(end - (uint8_t *)addr ,==, nbytes);
  /* TODO: coalesce acknowledgements - need a per-rank, per-op seqnum & packetcnt */
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph), 0, PACK(iop));
}
MEDIUM_HANDLER(gasnete_puti_AMPipeline_reqh,5,6, 
              (token,addr,nbytes, UNPACK(a0),      a1,a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), a2,a3,a4,a5));
#endif
/* ------------------------------------------------------------------------------------ */
/* Pipelined AM gather-scatter get */
#ifndef GASNETE_GETI_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gex_Event_t gasnete_geti_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(srccount > 1); /* supports gather get */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, GETI_AMPIPELINE);

  size_t const maxrequest = gex_AM_MaxRequestMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                    (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                    HARGS(5,6));
  size_t const maxreply   = gex_AM_MaxReplyMedium  (tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                    (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                    HARGS(2,3));

  gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t) + dstcount*sizeof(void *) +
                                                  (GASNETE_VIS_NPAM ? 0 : maxrequest));
  void * * const savedlst = (void * *)(visop + 1);
  #if GASNETE_VIS_NPAM == 0
    void * * const packedbuf = savedlst + dstcount;
  #endif
  gasnete_packetdesc_t *remotept;
  gasnete_packetdesc_t *localpt;
  size_t const packetcnt = gasnete_packetize_addrlist(srccount, srclen, dstcount, dstlen,  
                                                &remotept, &localpt,
                                                // TODO-EX: Packetization logic should take both into account
                                                MIN(maxrequest,maxreply),
                                                0);
  GASNETE_VISOP_SETUP(visop, synctype, 1);
  #if GASNET_DEBUG
    visop->type = GASNETI_VIS_CAT_GETI_AMPIPELINE;
    visop->count = dstcount;
  #endif
  gasneti_assert_uint(packetcnt ,<=, GASNETI_ATOMIC_MAX);
  gasneti_assert_uint(packetcnt ,==, (gex_AM_Arg_t)packetcnt);
  visop->len = dstlen;
  visop->addr = localpt;
  GASNETI_MEMCPY(savedlst, dstlist, dstcount*sizeof(void *));
  gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);
  gasneti_eop_t *eop = visop->eop; /* visop may disappear once the last AM is launched */

  for (size_t packetidx = 0; packetidx < packetcnt; packetidx++) {
    #if GASNETE_VIS_NPAM  // NPAM 1 or 2 (currently treated as 1)
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, maxrequest, maxrequest, NULL, 0, HARGS(5,6));
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxrequest);
      void * * const packedbuf = gex_AM_SrcDescAddr(sd);
    #endif
      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      size_t const rnum = rpacket->lastidx - rpacket->firstidx + 1;
      /* fill packet with remote metadata */
      GASNETI_MEMCPY(packedbuf, &srclist[rpacket->firstidx], rnum*sizeof(void *));

      /* send AM(visop) from packedbuf */
      size_t const nbytes = rnum*sizeof(void *);
    #define ARGS PACK(visop), packetidx, srclen, rpacket->firstoffset, rpacket->lastlen
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_geti_AMPipeline_reqh),
                               packedbuf, nbytes, GEX_EVENT_NOW, 0, ARGS);
    #else
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_geti_AMPipeline_reqh), nbytes, ARGS);
    #endif
    #undef ARGS
  }

  gasneti_free(remotept);
  GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
}
  #define GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) \
    if (srccount > 1 && MIN(srclen,dstlen) <= gasnete_vis_get_maxchunk)                                            \
      return gasnete_geti_AMPipeline(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_geti_AMPipeline_reqh_inner)
void gasnete_geti_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t packetidx,
  gex_AM_Arg_t dstlen, gex_AM_Arg_t firstoffset, gex_AM_Arg_t lastlen) {
  void * const * const rlist = addr;
  size_t const rnum = nbytes / sizeof(void *);
  gasneti_assert_uint(nbytes ,==, rnum * sizeof(void *));
  size_t const maxreply = gex_Token_MaxReplyMedium(token, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                       (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                       HARGS(2,3));
  #if GASNETE_VIS_NPAM == 0
    uint8_t * const packedbuf = gasnete_visbuf_malloc(maxreply);
  #else // NPAM 1 or 2
    gex_AM_SrcDesc_t sd = gex_AM_PrepareReplyMedium(token, NULL, maxreply, maxreply, NULL, 0, HARGS(2,3));
    gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxreply);
    uint8_t * const packedbuf = gex_AM_SrcDescAddr(sd);
  #endif

  /* gather data payload from sourcelist into packet */
  uint8_t * const end = gasnete_addrlist_pack(rnum, rlist, dstlen, packedbuf, firstoffset, lastlen);
  size_t const replysz = end - packedbuf;
  gasneti_assert_uint(replysz ,<=, maxreply);

  // send packet
  #define ARGS PACK(_visop), packetidx
  #if GASNETE_VIS_NPAM == 0
    gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_geti_AMPipeline_reph),
                           packedbuf, replysz, GEX_EVENT_NOW, 0, ARGS);
    gasneti_free(packedbuf);
  #else
    gex_AM_CommitReplyMedium(sd, gasneti_handleridx(gasnete_geti_AMPipeline_reph), replysz, ARGS);
  #endif
  #undef ARGS
}
MEDIUM_HANDLER(gasnete_geti_AMPipeline_reqh,5,6, 
              (token,addr,nbytes, UNPACK(a0),      a1,a2,a3,a4),
              (token,addr,nbytes, UNPACK2(a0, a1), a2,a3,a4,a5));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_geti_AMPipeline_reph_inner)
void gasnete_geti_AMPipeline_reph_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t packetidx) {
  gasneti_vis_op_t * const visop = _visop;
  void * const * const savedlst = (void * *)(visop + 1);
  gasnete_packetdesc_t * const lpacket = ((gasnete_packetdesc_t *)visop->addr) + packetidx;
  size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
  gasneti_assert_uint(visop->type ,==, GASNETI_VIS_CAT_GETI_AMPIPELINE);
  gasneti_assert_uint(lpacket->lastidx ,<, visop->count);
  uint8_t * const end = gasnete_addrlist_unpack(lnum, savedlst+lpacket->firstidx, visop->len, addr, lpacket->firstoffset, lpacket->lastlen);
  gasneti_assert_uint(end - (uint8_t *)addr ,==, nbytes);
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
// GASNETE_INDEXED_HELPER iterates over 2-sided indexed metadata in order and invokes
// the provided macro action(p1,p2,len) on each contiguous matching chunk of the sequences
#define GASNETE_INDEXED_HELPER(count1, list1, len1, count2, list2, len2, action) do { \
  size_t const _count1 = (count1); void * const * const _list1 = (list1); size_t const _len1 = (len1); \
  size_t const _count2 = (count2); void * const * const _list2 = (list2); size_t const _len2 = (len2); \
  if (_len1 == _len2) { /* matched sizes (fast case) */                      \
    gasneti_assert_uint(_count1 ,==, _count2);                               \
    for (size_t _i = 0; _i < _count1; _i++) {                                \
      void * const _p1 = _list1[_i]; void * const _p2 = _list2[_i];          \
      action(_p1, _p2, _len1);                                               \
    }                                                                        \
  } else if (_count1 == 1) { /* 1 is contiguous buffer */                    \
    uintptr_t _pp1 = (uintptr_t)_list1[0];                                   \
    for (size_t _i = 0; _i < _count2; _i++) {                                \
      void * const _p1 = (void *)_pp1;                                       \
      void * const _p2 = _list2[_i];                                         \
      action(_p1, _p2, _len2);                                               \
      _pp1 += _len2;                                                         \
    }                                                                        \
    gasneti_assert_uint(_pp1 ,==, (uintptr_t)_list1[0]+_len1);               \
  } else if (_count2 == 1) { /* 2 is contiguous buffer */                    \
    uintptr_t _pp2 = (uintptr_t)_list2[0];                                   \
    for (size_t _i = 0; _i < _count1; _i++) {                                \
      void * const _p1 = _list1[_i];                                         \
      void * const _p2 = (void *)_pp2;                                       \
      action(_p1, _p2, _len1);                                               \
      _pp2 += _len1;                                                         \
    }                                                                        \
    gasneti_assert_uint(_pp2 ,==, (uintptr_t)_list2[0]+_len2);               \
  } else { /* mismatched sizes (general case) */                             \
    size_t _idx1 = 0; size_t _idx2 = 0;                                      \
    size_t _off1 = 0; size_t _off2 = 0;                                      \
    while (_idx1 < _count1) {                                                \
      gasneti_assert_uint(_idx2 ,<, _count2);                                \
      size_t const _rem1 = _len1 - _off1;                                    \
      size_t const _rem2 = _len2 - _off2;                                    \
      gasneti_assert(_rem1 > 0); gasneti_assert(_rem2 > 0);                  \
      void * const _p1 = (uint8_t*)(_list1[_idx1]) + _off1;                  \
      void * const _p2 = (uint8_t*)(_list2[_idx2]) + _off2;                  \
      if (_rem1 < _rem2) {                                                   \
        action(_p1, _p2, _rem1);                                             \
        _idx1++;                                                             \
        _off1 = 0;                                                           \
        _off2 += _rem1;                                                      \
      } else if (_rem2 < _rem1) {                                            \
        action(_p1, _p2, _rem2);                                             \
        _idx2++;                                                             \
        _off2 = 0;                                                           \
        _off1 += _rem2;                                                      \
      } else { gasneti_assert_uint(_rem1 ,==, _rem2);                        \
        action(_p1, _p2, _rem1);                                             \
        _idx1++; _idx2++;                                                    \
        _off1 = 0; _off2 = 0;                                                \
      }                                                                      \
    }                                                                        \
    gasneti_assert_uint(_idx1 ,==, _count1);                                 \
    gasneti_assert_uint(_idx2 ,==, _count2);                                 \
    gasneti_assert(_off1 == 0 && _off2 == 0);                                \
  }                                                                          \
} while (0)
/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_indexed_memcpy)
void gasnete_indexed_memcpy(gex_Rank_t jobrank, int isput,
                            size_t dstcount, void * const dstlist[], size_t dstlen,
                            size_t srccount, void * const srclist[], size_t srclen,
                            gex_Flags_t flags) {
  gasneti_assert(GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));

  // TODO-EX: this assumes all addresses in the peer list reside in the same segment
  // and hoists the address translation. Multi-segment will need to push this down.
  gasneti_assert(!(flags & (GEX_FLAG_PEER_SEG_SOME|GEX_FLAG_PEER_SEG_UNKNOWN)));
  if (isput) {
    uint8_t *refptr = GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,dstlist[0]); 
    ptrdiff_t const offset = refptr - (uint8_t*)dstlist[0];
    #define ACTION(p1,p2,len) GASNETI_MEMCPY((uint8_t*)p1+offset,p2,len)
    GASNETE_INDEXED_HELPER(dstcount, dstlist, dstlen, srccount, srclist, srclen, ACTION);
    #undef ACTION
  } else {
    uint8_t *refptr = GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,srclist[0]); 
    ptrdiff_t const offset = refptr - (uint8_t*)srclist[0];
    #define ACTION(p1,p2,len) GASNETI_MEMCPY(p1,(uint8_t*)p2+offset,len)
    GASNETE_INDEXED_HELPER(dstcount, dstlist, dstlen, srccount, srclist, srclen, ACTION);
    #undef ACTION
  }
}
/*---------------------------------------------------------------------------------*/
/* reference version that uses individual puts */
gex_Event_t gasnete_puti_ref_indiv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTI_REF_INDIV);
  gasneti_assert(srccount > 0); gasneti_assert(dstcount > 0);
  gasneti_assert(srclen > 0); gasneti_assert(dstlen > 0);
  gasneti_assert_uint(((uintptr_t)dstcount)*dstlen ,==, ((uintptr_t)srccount)*srclen);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
  gex_Event_t * const lc_opt = (flags & GEX_FLAG_ENABLE_LEAF_LC) ? GEX_EVENT_GROUP : GEX_EVENT_DEFER;
  GASNETE_START_NBIREGION(synctype);

  #define ACTION(p1,p2,len) GASNETE_PUT_INDIV(tm, rank, p1, p2, len, lc_opt)
  GASNETE_INDEXED_HELPER(dstcount, dstlist, dstlen, srccount, srclist, srclen, ACTION);
  #undef ACTION

  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}

/* reference version that uses individual gets */
gex_Event_t gasnete_geti_ref_indiv(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETI_REF_INDIV);
  gasneti_assert(srccount > 0); gasneti_assert(dstcount > 0);
  gasneti_assert(srclen > 0); gasneti_assert(dstlen > 0);
  gasneti_assert_uint(((uintptr_t)dstcount)*dstlen ,==, ((uintptr_t)srccount)*srclen);
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
  GASNETE_START_NBIREGION(synctype);

  #define ACTION(p1,p2,len) GASNETE_GET_INDIV(tm, rank, p1, p2, len)
  GASNETE_INDEXED_HELPER(dstcount, dstlist, dstlen, srccount, srclist, srclen, ACTION);
  #undef ACTION

  GASNETE_END_NBIREGION_AND_RETURN(synctype);
}

/*---------------------------------------------------------------------------------*/
GASNETI_INLINE(gasnete_convert_indexed_to_memvec)
void *gasnete_convert_indexed_to_memvec(gex_Memvec_t **pvec1, gex_Memvec_t **pvec2,
                                   size_t count1, void * const list1[], size_t len1,
                                   size_t count2, void * const list2[], size_t len2) {
  void * const mem = gasnete_visbuf_malloc(sizeof(gex_Memvec_t)*(count1 + count2));
  gex_Memvec_t * const vec1 = mem;
  gex_Memvec_t * const vec2 = vec1 + count1;
  for (size_t i=0; i < count1; i++) {
    vec1[i].gex_addr = list1[i];
    vec1[i].gex_len = len1;
  }
  for (size_t i=0; i < count2; i++) {
    vec2[i].gex_addr = list2[i];
    vec2[i].gex_len = len2;
  }
  *pvec1 = vec1;
  *pvec2 = vec2;
  return mem;
}
/* reference version that uses vector interface */
gex_Event_t gasnete_puti_ref_vector(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, PUTI_REF_VECTOR);
  gasneti_assert(GASNETE_PUTV_ALLOWS_VOLATILE_METADATA);
  gex_Memvec_t *dstvec; gex_Memvec_t *srcvec;
  void *mem = gasnete_convert_indexed_to_memvec(&dstvec, &srcvec,
                                                dstcount, dstlist, dstlen,
                                                srccount, srclist, srclen);
  gex_Event_t retval = gasnete_putv(synctype,tm,rank,dstcount,dstvec,srccount,srcvec,flags GASNETI_THREAD_PASS);
  gasneti_free(mem);
  return retval;
}
/* reference version that uses vector interface */
gex_Event_t gasnete_geti_ref_vector(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  GASNETI_TRACE_EVENT(C, GETI_REF_VECTOR);
  gasneti_assert(GASNETE_GETV_ALLOWS_VOLATILE_METADATA);
  gex_Memvec_t *dstvec; gex_Memvec_t *srcvec;
  void *mem = gasnete_convert_indexed_to_memvec(&dstvec, &srcvec,
                                                dstcount, dstlist, dstlen,
                                                srccount, srclist, srclen);
  gex_Event_t retval = gasnete_getv(synctype,tm,dstcount,dstvec,rank,srccount,srcvec,flags GASNETI_THREAD_PASS);
  gasneti_free(mem);
  return retval;
}

/*---------------------------------------------------------------------------------*/
/* top-level gasnet_puti_* entry point */
#ifndef GASNETE_PUTI_OVERRIDE
extern gex_Event_t gasnete_puti(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(dstcount*dstlen > 0);
  gasneti_assert(srccount*srclen > 0);
  gasneti_assert(dstcount > 1 || srccount > 1);
  gasneti_assert_uint(((uintptr_t)dstcount)*dstlen ,==, ((uintptr_t)srccount)*srclen);
  gasneti_assert(dstlist); gasneti_assert(srclist);
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  if (GASNETI_NBRHD_LOCAL(tm,rank)) { /* purely local */ 
    GASNETI_TRACE_EVENT(C, PUTI_NBRHD);
    gasnete_indexed_memcpy(rank, 1,
                           dstcount,dstlist,dstlen,srccount,srclist,srclen,
                           flags);
    return GEX_EVENT_INVALID;
  }

  /* select algorithm */
  #ifndef GASNETE_PUTI_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_PUTI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) do {                        \
        switch (rand() % 4) {                                                                                                           \
          case 0:                                                                                                                       \
            GASNETE_PUTI_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);                       \
          case 1:                                                                                                                       \
            GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);                   \
          case 2:                                                                                                                       \
            return gasnete_puti_ref_indiv(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS);  \
          case 3:                                                                                                                       \
            return gasnete_puti_ref_vector(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS); \
          default: gasneti_unreachable();                                                                                               \
        } } while (0)
    #else
      #define GASNETE_PUTI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags)       \
        GASNETE_PUTI_GATHER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);     \
        GASNETE_PUTI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags); \
        return gasnete_puti_ref_indiv(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
    #endif
  #endif
  GASNETE_PUTI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);
  gasneti_fatalerror("failure in GASNETE_PUTI_SELECTOR - should never reach here");
  return GEX_EVENT_INVALID; /* avoid warning on MIPSPro */
}
#endif
/* top-level gasnet_geti_* entry point */
#ifndef GASNETE_GETI_OVERRIDE
extern gex_Event_t gasnete_geti(gasnete_synctype_t synctype,
                                   gex_TM_t const tm,
                                   size_t dstcount, void * const dstlist[], size_t dstlen,
                                   gex_Rank_t const rank,
                                   size_t srccount, void * const srclist[], size_t srclen,
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(gasnete_vis_isinit);
  gasneti_assert(dstcount*dstlen > 0);
  gasneti_assert(srccount*srclen > 0);
  gasneti_assert(dstcount > 1 || srccount > 1);
  gasneti_assert_uint(((uintptr_t)dstcount)*dstlen ,==, ((uintptr_t)srccount)*srclen);
  gasneti_assert(dstlist); gasneti_assert(srclist);
  flags &= ~GEX_FLAG_IMMEDIATE; // TODO-EX

  if (GASNETI_NBRHD_LOCAL(tm,rank)) { /* purely local */ 
    GASNETI_TRACE_EVENT(C, GETI_NBRHD);
    gasnete_indexed_memcpy(rank, 0,
                           dstcount,dstlist,dstlen,srccount,srclist,srclen,
                           flags);
    return GEX_EVENT_INVALID;
  }

  /* select algorithm */
  #ifndef GASNETE_GETI_SELECTOR
    #if GASNETE_RANDOM_SELECTOR
      #define GASNETE_GETI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags) do {                        \
        switch (rand() % 4) {                                                                                                           \
          case 0:                                                                                                                       \
            GASNETE_GETI_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);                      \
          case 1:                                                                                                                       \
            GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);                   \
          case 2:                                                                                                                       \
            return gasnete_geti_ref_indiv(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS);  \
          case 3:                                                                                                                       \
            return gasnete_geti_ref_vector(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS); \
          default: gasneti_unreachable();                                                                                               \
        } } while (0)
    #else
      #define GASNETE_GETI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags)       \
        GASNETE_GETI_SCATTER_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);    \
        GASNETE_GETI_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags); \
        return gasnete_geti_ref_indiv(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags GASNETI_THREAD_PASS)
    #endif
  #endif
  GASNETE_GETI_SELECTOR(synctype,tm,rank,dstcount,dstlist,dstlen,srccount,srclist,srclen,flags);
  gasneti_fatalerror("failure in GASNETE_GETI_SELECTOR - should never reach here");
  return GEX_EVENT_INVALID; /* avoid warning on MIPSPro */
}
#endif
