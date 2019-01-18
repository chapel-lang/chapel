/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_gathr.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

static void
gasnete_coll_pami_gathr(const gasnet_team_handle_t team,
                        gasnet_image_t dstimage, void *dst,
                        const void *src, size_t nbytes,
                        int flags GASNETI_THREAD_FARG)
{
    const int i_am_leader = 1;

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        if (flags & GASNET_COLL_IN_ALLSYNC) gasnetc_fast_barrier();

        op = gasnete_op_template_gathr;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.gathr_alg;
        op.cmd.xfer_gather.root = gasnetc_endpoint(GASNETE_COLL_REL2ACT(team, dstimage));
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
    }
}

extern void
gasnete_coll_gather_pami(gasnet_team_handle_t team,
                         gasnet_image_t dstimage, void *dst,
                         void *src, size_t nbytes,
                         int flags GASNETI_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_gathr) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gex_Event_t handle;
    handle = gasnete_coll_gather_nb_default(team,dstimage,dst,src,nbytes,flags,0 GASNETI_THREAD_PASS);
    gasnete_wait(handle GASNETI_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
    gasnete_coll_pami_gathr(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_PASS);
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
