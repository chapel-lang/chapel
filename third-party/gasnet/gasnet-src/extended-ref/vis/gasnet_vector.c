/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/vis/gasnet_vector.c $
 * Description: GASNet Vector implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_REFVIS_C
  #error This file not meant to be compiled directly - included by gasnet_refvis.c
#endif

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
size_t gasnete_packetize_memvec(size_t remotecount, gex_Memvec_t const remotelist[],
                                size_t localcount, gex_Memvec_t const locallist[],
                                gasnete_packetdesc_t **premotept,
                                gasnete_packetdesc_t **plocalpt,
                                size_t maxpayload, int sharedpacket) {
  size_t ptidx;
  int done = 0;
  size_t ridx = 0, roffset = 0, lidx = 0, loffset = 0;
  size_t const metadatasz = sizeof(gex_Memvec_t);
  size_t ptsz = 4; /* initial size guess - no fast way to know for sure */
  gasneti_assert_uint(maxpayload ,>, metadatasz);
  gasnete_packetdesc_t *remotept = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasnete_packetdesc_t *localpt = gasneti_malloc(ptsz*sizeof(gasnete_packetdesc_t));
  gasneti_assert(premotept && plocalpt && remotecount && localcount);
  gasneti_assert_uint(gasnete_memveclist_totalsz(remotecount,remotelist) ,==, 
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
      gasneti_assert(roffset < remotelist[ridx].gex_len || (remotelist[ridx].gex_len == 0 && roffset == 0));
      rdatasz = remotelist[ridx].gex_len - roffset; /* data left in current entry */
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
          else if (i == remotept[ptidx].firstidx) datachk += (remotelist[i].gex_len - remotept[ptidx].firstoffset);
          else datachk += remotelist[i].gex_len;
        }
        gasneti_assert_uint(packetdata ,==, datachk);
        if (sharedpacket) { 
          gasneti_assert_uint((metadatasz*entries + packetdata) ,<=, maxpayload); /* not overfull */
          if (!done) gasneti_assert_uint((metadatasz*entries + packetdata) ,>=, maxpayload - metadatasz); /* not underfull */
        } else {
          gasneti_assert_uint(MAX(metadatasz*entries,packetdata) ,<=, maxpayload); /* not overfull */
          /* algorithm currently may underfill for !sharedpacket, because it effectively always 
             subtracts the MAX(metadatasz, datasz) from *both* packets being managed simultaneously in packetremain,
             rather than maintaining independent packetremains and updating each accordingly (increasing arithmetic complexity)
             In vectors whose entries are dominated by datasz or metadatasz, the effect should be neglible
             In perverse cases we might end up with a packet which where the maximal packet is only 2/3 full
             this means in datasz dominated vectors with a few entries where datasz < metadatasz (or vice-versa)
           */
          if (!done) gasneti_assert_uint(MAX(metadatasz*entries,packetdata) ,>=, (maxpayload - metadatasz)/2); /* not underfull */
        }
      }
    #endif

    ldatasz = 0;
    while (packetdata > 0 || (lidx < localcount && locallist[lidx].gex_len == 0)) {
      gasneti_assert(loffset < locallist[lidx].gex_len || (locallist[lidx].gex_len == 0 && loffset == 0));
      ldatasz = locallist[lidx].gex_len - loffset; /* data left in current entry */
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
gex_Event_t gasnete_putv_gather(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasnete_vis_threaddata_t * const td = GASNETE_VIS_MYTHREAD;
  size_t const nbytes = dstlist[0].gex_len;
  gasneti_assert(dstcount == 1 && srccount > 1); /* only supports gather put */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
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
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
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
/*---------------------------------------------------------------------------------*/
/* Pipelined AM gather-scatter put */
#ifndef GASNETE_PUTV_AMPIPELINE_SELECTOR
#if GASNETE_USE_AMPIPELINE
gex_Event_t gasnete_putv_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(srccount >= 1);
  gasneti_assert(dstcount > 1); /* supports scatter put */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, PUTV_AMPIPELINE);
  if_pf (srclist[0].gex_len == 0) { /* detect empty list */
    for (size_t i = 1; i < srccount; i++) { 
      if_pt (srclist[i].gex_len > 0) goto nonempty;
    }
    return GEX_EVENT_INVALID;
    nonempty: ;
  }
  GASNETE_START_NBIREGION(synctype);

  size_t const maxpacket = gex_AM_MaxRequestMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                   (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                   HARGS(2,3));
  gasnete_packetdesc_t *remotept;
  gasnete_packetdesc_t *localpt;
  size_t const packetcnt = gasnete_packetize_memvec(dstcount, dstlist, srccount, srclist,
                                                &remotept, &localpt,
                                                maxpacket,
                                                1);
  gasneti_iop_t *iop = gasneti_iop_register(packetcnt,0 GASNETI_THREAD_PASS);

  #if GASNETE_VIS_NPAM == 0
    gex_Memvec_t * const packedbuf = gasnete_visbuf_malloc(maxpacket);
  #endif

  for (size_t packetidx = 0; packetidx < packetcnt; packetidx++) {
    #if GASNETE_VIS_NPAM  // NPAM 1 or 2 (currently treated as 1)
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, maxpacket, maxpacket, NULL, 0, HARGS(2,3));
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxpacket);
      gex_Memvec_t * const packedbuf = gex_AM_SrcDescAddr(sd);
    #endif

      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      gasnete_packetdesc_t * const lpacket = &localpt[packetidx];
      size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
      // fill packet with remote metadata, filtering empty iovecs as needed
      size_t const firstridx = rpacket->firstidx;
      size_t const lastridx = rpacket->lastidx;
      size_t firstoffset = rpacket->firstoffset;
      size_t rnum = 0; // count remote iovecs
      for (size_t ri = firstridx; ri <= lastridx; ri++) {
        size_t   vlen  = dstlist[ri].gex_len;
        uint8_t *vaddr = dstlist[ri].gex_addr;
        if (firstoffset) { // adjust first
          gasneti_assert(rnum == 0);
          gasneti_assert_uint(ri ,==, firstridx);
          gasneti_assert_uint(firstoffset ,<=, vlen);
          vaddr += firstoffset;
          vlen  -= firstoffset;
          firstoffset = 0;
        }
        if (ri == lastridx) { // truncate last
          gasneti_assert_uint(rpacket->lastlen ,<=, vlen);
          vlen = rpacket->lastlen;
        }
        if_pt (vlen) { // add this non-empty iovec to the wire
          packedbuf[rnum].gex_len = vlen;
          packedbuf[rnum].gex_addr = vaddr;
          rnum++; 
        }
      }
      size_t packetlen;
      if_pf (rnum == 0) { // entire packet is empty, skip send
        gasneti_iop_markdone(iop, 1, 0);
        #if GASNETE_VIS_NPAM == 0
          continue;
        #else // NPAM lacks a cancel, so must send to empty packet sink, but skip the ACK
          packetlen = 0;
          goto npamsend; 
        #endif
      }
      { /* gather data payload from sourcelist into packet */
        uint8_t * const end = gasnete_memvec_pack(lnum, &srclist[lpacket->firstidx], &packedbuf[rnum], 
                                lpacket->firstoffset, lpacket->lastlen);

        packetlen = end - (uint8_t *)packedbuf;
      }

      #if GASNET_DEBUG
      { // assert we don't send empty iovecs on the wire (bug3411)
        size_t datalen = 0; 
        for (size_t i=0; i < rnum; i++) {
          size_t const thislen = packedbuf[i].gex_len;
          gasneti_assert(thislen > 0);
          datalen += thislen;
        }
        gasneti_assert(datalen > 0); 
        gasneti_assert_uint(packetlen ,==, rnum*sizeof(gex_Memvec_t)+datalen);
        gasneti_assert_uint(packetlen ,<=, maxpacket);
      }
      #endif

      /* send AM(rnum, iop) from packedbuf */
    #define ARGS PACK(iop), rnum
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_putv_AMPipeline_reqh),
                               packedbuf, packetlen, GEX_EVENT_NOW, 0, ARGS);
    #else
      npamsend:
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_putv_AMPipeline_reqh), packetlen, ARGS);
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
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_ampipe && dstcount > 1)                                                      \
      return gasnete_putv_AMPipeline(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_PUTV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_putv_AMPipeline_reqh_inner)
void gasnete_putv_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *iop, gex_AM_Arg_t rnum) {
  #if GASNETE_VIS_NPAM 
    if_pf (nbytes == 0) return; // empty packet sink
  #endif
  gasneti_assert(addr && nbytes > 0 && rnum > 0);
  gex_Memvec_t * const rlist = addr;
  uint8_t * const data = (uint8_t *)(&rlist[rnum]);
  uint8_t * const end = gasnete_memvec_unpack_noempty(rnum, rlist, data, 0, (size_t)-1);
  gasneti_assert_uint(end - (uint8_t *)addr ,==, nbytes);
  /* TODO: coalesce acknowledgements - need a per-rank, per-op seqnum & packetcnt */
  gex_AM_ReplyShort(token, gasneti_handleridx(gasnete_putvis_AMPipeline_reph), 0, PACK(iop));
}
MEDIUM_HANDLER(gasnete_putv_AMPipeline_reqh,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
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

gex_Event_t gasnete_getv_AMPipeline(gasnete_synctype_t synctype,
                                   gex_TM_t const tm, gex_Rank_t const rank,
                                   size_t dstcount, gex_Memvec_t const dstlist[], 
                                   size_t srccount, gex_Memvec_t const srclist[],
                                   gex_Flags_t flags GASNETI_THREAD_FARG) {
  gasneti_assert(dstcount >= 1);
  gasneti_assert(srccount > 1); /* supports gather get */
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank)); // silly to use for local cases
  GASNETI_TRACE_EVENT(C, GETV_AMPIPELINE);
  if_pf (dstlist[0].gex_len == 0) { /* detect empty list */
    for (size_t i = 1; i < dstcount; i++) { 
      if_pt (dstlist[i].gex_len > 0) goto nonempty;
    }
    return GEX_EVENT_INVALID;
    nonempty: ;
  }

  size_t const maxrequest = gex_AM_MaxRequestMedium(tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                    (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                    HARGS(2,3));
  size_t const maxreply   = gex_AM_MaxReplyMedium  (tm,rank, (GASNETE_VIS_NPAM ? NULL : GEX_EVENT_NOW),
                                                    (GASNETE_VIS_NPAM ? GEX_FLAG_AM_PREPARE_LEAST_ALLOC : 0),
                                                    HARGS(2,3));

  gasneti_vis_op_t * const visop = gasnete_visbuf_malloc(sizeof(gasneti_vis_op_t) + dstcount*sizeof(gex_Memvec_t) +
                                                  (GASNETE_VIS_NPAM ? 0 : maxrequest));
  gex_Memvec_t * const savedlst = (gex_Memvec_t *)(visop + 1);
  #if GASNETE_VIS_NPAM == 0
    gex_Memvec_t * const packedbuf = savedlst + dstcount;
  #endif
  gasnete_packetdesc_t *remotept;
  gasnete_packetdesc_t *localpt;
  size_t const packetcnt = gasnete_packetize_memvec(srccount, srclist, dstcount, dstlist,  
                                                &remotept, &localpt,
                                                // TODO-EX: Packetization logic should take both into account
                                                MIN(maxrequest,maxreply),
                                                0);
  GASNETE_VISOP_SETUP(visop, synctype, 1);
  #if GASNET_DEBUG
    visop->type = GASNETI_VIS_CAT_GETV_AMPIPELINE;
    visop->count = dstcount;
  #endif
  gasneti_assert_uint(packetcnt ,<=, GASNETI_ATOMIC_MAX);
  gasneti_assert_uint(packetcnt ,==, (gex_AM_Arg_t)packetcnt);
  visop->addr = localpt;
  GASNETI_MEMCPY(savedlst, dstlist, dstcount*sizeof(gex_Memvec_t));
  gasneti_weakatomic_set(&(visop->packetcnt), packetcnt, GASNETI_ATOMIC_WMB_POST);
  gasneti_eop_t *eop = visop->eop; /* visop may disappear once the last AM is launched */

  for (size_t packetidx = 0; packetidx < packetcnt; packetidx++) {
    #if GASNETE_VIS_NPAM  // NPAM 1 or 2 (currently treated as 1)
      gex_AM_SrcDesc_t sd = gex_AM_PrepareRequestMedium(tm, rank, NULL, maxrequest, maxrequest, NULL, 0, HARGS(2,3));
      gasneti_assert_uint(gex_AM_SrcDescSize(sd) ,>=, maxrequest);
      gex_Memvec_t * const packedbuf = gex_AM_SrcDescAddr(sd);
    #endif

      gasnete_packetdesc_t * const rpacket = &remotept[packetidx];
      // fill packet with remote metadata, filtering empty iovecs as needed
      size_t const firstridx = rpacket->firstidx;
      size_t const lastridx = rpacket->lastidx;
      size_t firstoffset = rpacket->firstoffset;
      size_t rnum = 0; // count remote iovecs
      for (size_t ri = firstridx; ri <= lastridx; ri++) {
        size_t   vlen  = srclist[ri].gex_len;
        uint8_t *vaddr = srclist[ri].gex_addr;
        if (firstoffset) { // adjust first
          gasneti_assert(rnum == 0);
          gasneti_assert_uint(ri ,==, firstridx);
          gasneti_assert_uint(firstoffset ,<=, vlen);
          vaddr += firstoffset;
          vlen  -= firstoffset;
          firstoffset = 0;
        }
        if (ri == lastridx) { // truncate last
          gasneti_assert_uint(rpacket->lastlen ,<=, vlen);
          vlen = rpacket->lastlen;
        }
        if_pt (vlen) { // add this non-empty iovec to the wire
          packedbuf[rnum].gex_len = vlen;
          packedbuf[rnum].gex_addr = vaddr;
          rnum++; 
        }
      }
      size_t nbytes;
      if_pf (rnum == 0) { // entire packet is empty, skip send
        gasnete_getv_AMPipeline_visop_signal(visop);
        #if GASNETE_VIS_NPAM == 0
          continue;
        #else // NPAM lacks a cancel, so must send to empty packet sink, but skip the ACK
          nbytes = 0;
          goto npamsend; 
        #endif
      }

      #if GASNET_DEBUG
      { // assert we don't send empty iovecs on the wire (bug3411)
        size_t datalen = 0; 
        for (size_t i=0; i < rnum; i++) {
          size_t const thislen = packedbuf[i].gex_len;
          gasneti_assert(thislen > 0);
          datalen += thislen;
        }
        gasneti_assert(datalen > 0); 
      }
      #endif

      /* send AM(visop) from packedbuf */
      nbytes = rnum*sizeof(gex_Memvec_t);
    #define ARGS PACK(visop), packetidx
    #if GASNETE_VIS_NPAM == 0
      gex_AM_RequestMedium(tm, rank, gasneti_handleridx(gasnete_getv_AMPipeline_reqh),
                               packedbuf, nbytes, GEX_EVENT_NOW, 0, ARGS);
    #else
      npamsend:
      gex_AM_CommitRequestMedium(sd, gasneti_handleridx(gasnete_getv_AMPipeline_reqh), nbytes, ARGS);
    #endif
    #undef ARGS
  }

  gasneti_free(remotept);
  GASNETE_VISOP_RETURN_VOLATILE(eop, synctype);
}
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) \
    if (gasnete_vis_use_ampipe && srccount > 1)                                                      \
      return gasnete_getv_AMPipeline(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags GASNETI_THREAD_PASS)
#else
  #define GASNETE_GETV_AMPIPELINE_SELECTOR(synctype,tm,rank,dstcount,dstlist,srccount,srclist,flags) ((void)0)
#endif
#endif
/* ------------------------------------------------------------------------------------ */
#if GASNETE_USE_AMPIPELINE
GASNETI_INLINE(gasnete_getv_AMPipeline_reqh_inner)
void gasnete_getv_AMPipeline_reqh_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t packetidx) {
  #if GASNETE_VIS_NPAM 
    if_pf (nbytes == 0) return; // empty packet sink
  #endif
  gasneti_assert(addr && nbytes > 0);
  gex_Memvec_t * const rlist = addr;
  size_t const rnum = nbytes / sizeof(gex_Memvec_t);
  gasneti_assert_uint(nbytes ,==, rnum * sizeof(gex_Memvec_t));
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
  gasneti_assert(packedbuf);

  /* gather data payload from sourcelist into packet */
  uint8_t * const end = gasnete_memvec_pack_noempty(rnum, rlist, packedbuf, 0, (size_t)-1);
  size_t const replysz = end - packedbuf;
  gasneti_assert_uint(replysz ,<=, maxreply);
  gasneti_assert(replysz > 0);

  // send packet
  #define ARGS PACK(_visop), packetidx
  #if GASNETE_VIS_NPAM == 0
    gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_getv_AMPipeline_reph),
                           packedbuf, replysz, GEX_EVENT_NOW, 0, ARGS);
    gasneti_free(packedbuf);
  #else
    gex_AM_CommitReplyMedium(sd, gasneti_handleridx(gasnete_getv_AMPipeline_reph), replysz, ARGS);
  #endif
  #undef ARGS
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reqh,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
/* ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasnete_getv_AMPipeline_reph_inner)
void gasnete_getv_AMPipeline_reph_inner(gex_Token_t token,
  void *addr, size_t nbytes,
  void *_visop, gex_AM_Arg_t packetidx) {
  gasneti_vis_op_t * const visop = _visop;
  gex_Memvec_t * const savedlst = (gex_Memvec_t *)(visop + 1);
  gasnete_packetdesc_t * const lpacket = ((gasnete_packetdesc_t *)visop->addr) + (uint32_t)packetidx;
  size_t const lnum = lpacket->lastidx - lpacket->firstidx + 1;
  gasneti_assert_uint(visop->type ,==, GASNETI_VIS_CAT_GETV_AMPIPELINE);
  gasneti_assert_uint(lpacket->lastidx ,<, visop->count);
  gasneti_assert(addr && nbytes > 0);
  uint8_t * const end = gasnete_memvec_unpack(lnum, savedlst+lpacket->firstidx, addr, lpacket->firstoffset, lpacket->lastlen);
  gasneti_assert_uint(end - (uint8_t *)addr ,==, nbytes);
  gasnete_getv_AMPipeline_visop_signal(visop);
}
MEDIUM_HANDLER(gasnete_getv_AMPipeline_reph,2,3, 
              (token,addr,nbytes, UNPACK(a0),      a1),
              (token,addr,nbytes, UNPACK2(a0, a1), a2));
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
void gasnete_vector_memcpy(gex_Rank_t jobrank, int isput,
                            size_t dstcount, gex_Memvec_t const dstlist[],
                            size_t srccount, gex_Memvec_t const srclist[],
                            gex_Flags_t flags) {
  gasneti_assert(GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));

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
  uint8_t const * const refptr = GASNETI_NBRHD_JOBRANK_LOCAL_ADDR(jobrank,rawptr);
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
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
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
  gasneti_assert(!GASNETI_NBRHD_LOCAL(tm,rank));
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

  if (GASNETI_NBRHD_LOCAL(tm,rank)) { /* purely local */
    GASNETI_TRACE_EVENT(C, PUTV_NBRHD);
    gasnete_vector_memcpy(rank, 1,
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

  if (GASNETI_NBRHD_LOCAL(tm,rank)) { /* purely local */
    GASNETI_TRACE_EVENT(C, GETV_NBRHD);
    gasnete_vector_memcpy(rank, 0,
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
