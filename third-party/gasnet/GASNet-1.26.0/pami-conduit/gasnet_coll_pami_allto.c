/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_allto.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

#if GASNET_PAR
static void
gasnete_coll_pami_alltovi(const gasnet_team_handle_t team,
                          void *dst, const void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG)
{
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
    const size_t local_len = nbytes * team->my_images;
    void * const sndbuf = team->pami.scratch_space;
    void * const rcvbuf = (void*)((uintptr_t)team->pami.scratch_space + local_len * team->total_images);
    int i;

    if (flags & GASNET_COLL_IN_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }

    {
        const uint8_t *stmp = src;
        uint8_t *dtmp = (uint8_t*) sndbuf + nbytes * td->my_local_image;
        for (i = 0; i < team->total_images; ++i) {
            GASNETE_FAST_UNALIGNED_MEMCPY(dtmp, stmp, nbytes);
            stmp += nbytes;
            dtmp += local_len;
        }
    }
    (void) gasnete_coll_pami_images_barrier(team);

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        op = gasnete_op_template_alltovi; /* alltoallv_int */
        op.cookie = (void *)&done;
        op.algorithm = team->pami.alltovi_alg;
        op.cmd.xfer_alltoallv_int.sndbuf = sndbuf;
        op.cmd.xfer_alltoallv_int.stypecounts = team->pami.counts;
        op.cmd.xfer_alltoallv_int.sdispls = team->pami.displs;
        op.cmd.xfer_alltoallv_int.rcvbuf = rcvbuf;
        op.cmd.xfer_alltoallv_int.rtypecounts = team->pami.counts;
        op.cmd.xfer_alltoallv_int.rdispls = team->pami.displs;
        if (team->pami.prev_nbytes != local_len) {
            for (i = 0; i < team->total_ranks; ++i) {
                op.cmd.xfer_alltoallv_int.stypecounts[i] = local_len * team->all_images[i];
                op.cmd.xfer_alltoallv_int.sdispls[i] = local_len * team->all_offset[i];
            }
            team->pami.prev_nbytes = local_len;
        }

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking alltoallv_int");

        gasneti_polluntil(done);

        gasneti_assert(NULL == team->pami.tmp_addr);
        gasneti_sync_writes(); /* XXX: is this necessary? */
        team->pami.tmp_addr = rcvbuf; /* wakes pollers, below */
    } else {
        gasneti_waitwhile(NULL == team->pami.tmp_addr);
    }

    {
        uint8_t *dtmp = dst;
        for (i = 0; i < team->total_ranks; ++i) {
            const size_t len = nbytes * team->all_images[i];
            const uint8_t *stmp = (uint8_t*) rcvbuf
                                           + td->my_local_image * len
                                           + team->pami.displs[i];
            GASNETE_FAST_UNALIGNED_MEMCPY(dtmp, stmp, len);
            dtmp += len;
        }
    }
    (void) gasnete_coll_pami_images_barrier(team);
    
    if (i_am_leader) team->pami.tmp_addr = NULL;
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}
#endif

static void
gasnete_coll_pami_allto(const gasnet_team_handle_t team,
                        void *dst, const void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  #if GASNET_PAR
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */
  #else
    const int i_am_leader = 1;
  #endif

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        if (flags & GASNET_COLL_IN_ALLSYNC) gasnetc_fast_barrier();

        op = gasnete_op_template_allto;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.allto_alg;
        op.cmd.xfer_alltoall.sndbuf = (/*not-const*/ void *)src;
        op.cmd.xfer_alltoall.stypecount = nbytes;
        op.cmd.xfer_alltoall.rcvbuf = dst;
        op.cmd.xfer_alltoall.rtypecount = nbytes;

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking alltoall");

        gasneti_polluntil(done);
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}

extern void
gasnete_coll_exchange_pami(gasnet_team_handle_t team,
                           void *dst, void *src, size_t nbytes,
                           int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_allto
  #if GASNET_PAR
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes_allto))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_exchange_nb_default(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    if (team->multi_images_any) {
      gasnete_coll_pami_alltovi(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_allto(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    gasnete_coll_pami_allto(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

extern void
gasnete_coll_exchangeM_pami(gasnet_team_handle_t team,
                            void * const dstlist[],
                            void * const srclist[],
                            size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_allto
  #if GASNET_PAR
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes_allto))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_exchangeM_nb_default(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
    void * const dst = dstlist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
    void * const src = srclist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
    if (team->multi_images_any) {
      gasnete_coll_pami_alltovi(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_allto(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    void * const dst = GASNETE_COLL_MY_1ST_IMAGE(team, dstlist, flags);
    void * const src = GASNETE_COLL_MY_1ST_IMAGE(team, srclist, flags);
    gasnete_coll_pami_allto(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
