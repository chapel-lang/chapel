/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_gathr.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

#if GASNET_PAR
static void
gasnete_coll_pami_gathrvi(const gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          const void *src, size_t nbytes,
                          int flags GASNETE_THREAD_FARG)
{
    const int i_am_root = gasnete_coll_image_is_local(team, dstimage);
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;

    if ((flags & GASNET_COLL_LOCAL) && i_am_root) {
        /* root thread must be leader for its node */
        i_am_leader = (dstimage == td->my_image);
    }

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

        op = gasnete_op_template_gathrvi; /* gatherv_int */
        op.cookie = (void *)&done;
        op.algorithm = team->pami.gathrvi_alg;
        op.cmd.xfer_gatherv_int.root = gasnetc_endpoint(GASNETE_COLL_REL2ACT(team,gasnete_coll_image_node(team, dstimage)));
        op.cmd.xfer_gatherv_int.sndbuf = team->pami.scratch_space;
        op.cmd.xfer_gatherv_int.stypecount = nbytes * team->my_images;

        if (i_am_root) {
            op.cmd.xfer_gatherv_int.rcvbuf = dst;
            op.cmd.xfer_gatherv_int.rtypecounts = team->pami.counts;
            op.cmd.xfer_gatherv_int.rdispls = team->pami.displs;
            if (team->pami.prev_nbytes != nbytes) {
                int i;
                for (i = 0; i < team->total_ranks; ++i) {
                    op.cmd.xfer_gatherv_int.rtypecounts[i] = nbytes * team->all_images[i];
                    op.cmd.xfer_gatherv_int.rdispls[i] = nbytes * team->all_offset[i];
                }
                team->pami.prev_nbytes = nbytes;
            }
        }

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking gatherv_int");

        gasneti_polluntil(done);
    }
    (void) gasnete_coll_pami_images_barrier(team);

    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}
#endif

static void
gasnete_coll_pami_gathr(const gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        const void *src, size_t nbytes,
                        int flags GASNETE_THREAD_FARG)
{
    const int i_am_root = gasnete_coll_image_is_local(team, dstimage);

  #if GASNET_PAR
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */

    if ((flags & GASNET_COLL_LOCAL) && i_am_root) {
        /* root thread must be leader for its node */
        const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
        i_am_leader = (dstimage == td->my_image);
    }
  #else
    const int i_am_leader = 1;
  #endif

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        if (flags & GASNET_COLL_IN_ALLSYNC) gasnetc_fast_barrier();

        op = gasnete_op_template_gathr;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.gathr_alg;
        op.cmd.xfer_gather.root = gasnetc_endpoint(GASNETE_COLL_REL2ACT(team,gasnete_coll_image_node(team, dstimage)));
        op.cmd.xfer_gather.sndbuf = (/*not-const*/ void *)src;
        op.cmd.xfer_gather.stypecount = nbytes;
        op.cmd.xfer_gather.rcvbuf = dst;
        op.cmd.xfer_gather.rtypecount = nbytes;

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking gather");

        gasneti_polluntil(done);
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
}

extern void
gasnete_coll_gather_pami(gasnet_team_handle_t team,
                         gasnet_image_t dstimage, void *dst,
                         void *src, size_t nbytes,
                         int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_gathr
  #if GASNET_PAR
      /* TODO: could remove size restriction by segmenting/pipelining */
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gather_nb_default(team,dstimage,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    if (team->multi_images_any) {
      gasnete_coll_pami_gathrvi(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_gathr(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    gasnete_coll_pami_gathr(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

extern void
gasnete_coll_gatherM_pami(gasnet_team_handle_t team,
                          gasnet_image_t dstimage, void *dst,
                          void * const srclist[], size_t nbytes,
                          int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_gathr
  #if GASNET_PAR
      /* TODO: could remove size restriction by segmenting/pipelining */
      || (team->multi_images_any && (nbytes > team->pami.scratch_max_nbytes))
  #endif
     ) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_gatherM_nb_default(team,dstimage,dst,srclist,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
  #if GASNET_PAR
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
    void * const src = srclist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
    if (team->multi_images_any) {
      gasnete_coll_pami_gathrvi(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    } else {
      gasnete_coll_pami_gathr(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
    }
  #else
    void * const src = GASNETE_COLL_MY_1ST_IMAGE(team, srclist, flags);
    gasnete_coll_pami_gathr(team,dstimage,dst,src,nbytes,flags GASNETE_THREAD_PASS);
  #endif
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
