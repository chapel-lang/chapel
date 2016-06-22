/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_bcast.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

static void
gasnete_coll_pami_bcast(const gasnet_team_handle_t team, void *dst,
                        gasnet_image_t srcimage, const void *src,
                        size_t nbytes, int flags GASNETE_THREAD_FARG)
{
    const int i_am_root = gasnete_coll_image_is_local(team, srcimage);

  #if GASNET_PAR
    int i_am_leader = gasnete_coll_pami_images_barrier(team); /* XXX: over-synced for IN_NO and IN_MY */

    if ((flags & GASNET_COLL_LOCAL) && i_am_root) {
        /* root thread must be leader for its node */
        const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
        i_am_leader = (srcimage == td->my_image);
    }
  #else
    const int i_am_leader = 1;
  #endif

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        if (flags & GASNET_COLL_IN_ALLSYNC) gasnetc_fast_barrier();

        op = gasnete_op_template_bcast;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.bcast_alg;
        op.cmd.xfer_broadcast.root = gasnetc_endpoint(GASNETE_COLL_REL2ACT(team,gasnete_coll_image_node(team, srcimage)));
        op.cmd.xfer_broadcast.buf  = i_am_root ? (/*not-const*/ void *)src : dst;
        op.cmd.xfer_broadcast.typecount = nbytes;

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking broadcast");

        if (i_am_root) {
          if (dst != src) GASNETE_FAST_UNALIGNED_MEMCPY(dst, src, nbytes);
        }

        gasneti_polluntil(done);
    }

  #if GASNET_PAR
    if (i_am_leader) {
        gasneti_assert(NULL == team->pami.tmp_addr);
        gasneti_sync_writes();
        team->pami.tmp_addr = dst; /* wakes pollers, below */
        (void) gasnete_coll_pami_images_barrier(team); /* matches instance below vvvv */
        team->pami.tmp_addr = NULL;
    } else {
        gasneti_waitwhile(NULL == team->pami.tmp_addr);
        GASNETE_FAST_UNALIGNED_MEMCPY(dst, team->pami.tmp_addr, nbytes);
        (void) gasnete_coll_pami_images_barrier(team); /* matches instance above ^^^^ */
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
        (void) gasnete_coll_pami_images_barrier(team);
    }
  #else
    if (flags & GASNET_COLL_OUT_ALLSYNC) gasnetc_fast_barrier();
  #endif
}

extern void
gasnete_coll_broadcast_pami(gasnet_team_handle_t team, void *dst,
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_bcast) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_broadcast_nb_default(team,dst,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else {
    /* Use PAMI-specific implementation */
    gasnete_coll_pami_bcast(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
  }
}

extern void
gasnete_coll_broadcastM_pami(gasnet_team_handle_t team,
                             void * const dstlist[],
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_bcast) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gasnet_coll_handle_t handle;
    handle = gasnete_coll_broadcastM_nb_default(team,dstlist,srcimage,src,nbytes,flags,0 GASNETE_THREAD_PASS);
    gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
  } else {
    /* Use PAMI-specific implementation */
  #if GASNET_PAR
    const gasnete_coll_threaddata_t * const td = GASNETE_COLL_MYTHREAD_NOALLOC;
    void * const dst = dstlist[((flags & GASNET_COLL_LOCAL) ? td->my_local_image : td->my_image)];
  #else
    void * const dst = GASNETE_COLL_MY_1ST_IMAGE(team, dstlist, flags);
  #endif

    gasnete_coll_pami_bcast(team,dst,srcimage,src,nbytes,flags GASNETE_THREAD_PASS);
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
