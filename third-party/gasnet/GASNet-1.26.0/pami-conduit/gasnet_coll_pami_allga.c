/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_allga.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

#if GASNET_PAR
static void
gasnete_coll_pami_allgavi(const gasnet_team_handle_t team,
                          void *dst, const void *src,
                          size_t nbytes, int flags GASNETE_THREAD_FARG)
{
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;

    if (flags & GASNET_COLL_IN_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }

    GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(team->pami.scratch_space,
                                                         td->my_local_image,
                                                         nbytes),
                                  src, nbytes);
    (void) gasnete_coll_pami_images_barrier(team);

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        op = gasnete_op_template_allgavi; /* allgatherv_int */
        op.cookie = (void *)&done;
        op.algorithm = team->pami.allgavi_alg;
        op.cmd.xfer_allgatherv_int.sndbuf = team->pami.scratch_space;
        op.cmd.xfer_allgatherv_int.stypecount = nbytes * team->my_images;

        op.cmd.xfer_allgatherv_int.rcvbuf = dst;
        op.cmd.xfer_allgatherv_int.rtypecounts = team->pami.counts;
        op.cmd.xfer_allgatherv_int.rdispls = team->pami.displs;
        if (team->pami.prev_nbytes != nbytes) {
            int i;
            for (i = 0; i < team->total_ranks; ++i) {
                op.cmd.xfer_allgatherv_int.rtypecounts[i] = nbytes * team->all_images[i];
                op.cmd.xfer_allgatherv_int.rdispls[i] = nbytes * team->all_offset[i];
            }
            team->pami.prev_nbytes = nbytes;
        }

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking allgatherv_int");

        gasneti_polluntil(done);

        gasneti_assert(NULL == team->pami.tmp_addr);
        gasneti_sync_writes(); /* XXX: is this necessary? */
        team->pami.tmp_addr = dst; /* wakes pollers, below */
        (void) gasnete_coll_pami_images_barrier(team); /* matches instance below vvvv */
        team->pami.tmp_addr = NULL;
    } else {
        gasneti_waitwhile(NULL == team->pami.tmp_addr);
        GASNETE_FAST_UNALIGNED_MEMCPY(dst, team->pami.tmp_addr, nbytes * team->total_images);
        (void) gasnete_coll_pami_images_barrier(team); /* matches instance above ^^^^ */
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}
#endif

static void
gasnete_coll_pami_allga(const gasnet_team_handle_t team,
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

        op = gasnete_op_template_allga;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.allga_alg;
        op.cmd.xfer_allgather.sndbuf = (/*not-const*/ void *)src;
        op.cmd.xfer_allgather.stypecount = nbytes;
        op.cmd.xfer_allgather.rcvbuf = dst;
        op.cmd.xfer_allgather.rtypecount = nbytes;

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking allgather");

        gasneti_polluntil(done);
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}

extern void
gasnete_coll_gather_all_pami(gasnet_team_handle_t team,
                             void *dst, void *src, size_t nbytes,
                             int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_allga
  #if GASNET_PAR
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_all_nb_default(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    if (team->multi_images_any) {
      gasnete_coll_pami_allgavi(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_allga(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    gasnete_coll_pami_allga(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

extern void
gasnete_coll_gather_allM_pami(gasnet_team_handle_t team,
                              void * const dstlist[],
                              void * const srclist[],
                              size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_allga
  #if GASNET_PAR
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_allM_nb_default(team,dstlist,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
    void * const dst = dstlist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
    void * const src = srclist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
    if (team->multi_images_any) {
      gasnete_coll_pami_allgavi(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_allga(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    void * const dst = GASNETE_COLL_MY_1ST_IMAGE(team, dstlist, flags);
    void * const src = GASNETE_COLL_MY_1ST_IMAGE(team, srclist, flags);
    gasnete_coll_pami_allga(team,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
