/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami_scatt.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>

#if GASNET_PAMI_NATIVE_COLL

static void
gasnete_coll_pami_scatt(const gasnet_team_handle_t team, void *dst,
                        gasnet_image_t srcimage, const void *src,
                        size_t nbytes, int flags GASNETI_THREAD_FARG)
{
    const int i_am_leader = 1;

    if (i_am_leader) {
        volatile unsigned int done = 0;
        pami_result_t rc;
        pami_xfer_t op;

        if (flags & GASNET_COLL_IN_ALLSYNC) gasnetc_fast_barrier();

        op = gasnete_op_template_scatt;
        op.cookie = (void *)&done;
        op.algorithm = team->pami.scatt_alg;
        op.cmd.xfer_scatter.root = gasnetc_endpoint(GASNETE_COLL_REL2ACT(team, srcimage));
        op.cmd.xfer_scatter.sndbuf = (/*not-const*/ void *)src;
        op.cmd.xfer_scatter.stypecount = nbytes;
        op.cmd.xfer_scatter.rcvbuf = dst;
        op.cmd.xfer_scatter.rtypecount = nbytes;

        GASNETC_PAMI_LOCK(gasnetc_context);
        rc = PAMI_Collective(gasnetc_context, &op);
        GASNETC_PAMI_UNLOCK(gasnetc_context);
        GASNETC_PAMI_CHECK(rc, "initiating blocking scatter");

        gasneti_polluntil(done);
    }
      
    if (flags & GASNET_COLL_OUT_ALLSYNC) {
        if (i_am_leader) gasnetc_fast_barrier();
    }
}

extern void
gasnete_coll_scatter_pami(gasnet_team_handle_t team, void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, int flags GASNETI_THREAD_FARG)
{
  if ((team->pami.geom == PAMI_GEOMETRY_NULL) || !gasnete_use_pami_scatt) {
    /* Use generic implementation for cases we don't (yet) handle, or when disabled */
    gex_Event_t handle;
    handle = gasnete_coll_scatter_nb_default(team,dst,srcimage,src,nbytes,flags,0 GASNETI_THREAD_PASS);
    gasnete_wait(handle GASNETI_THREAD_PASS);
  } else { /* Use PAMI-specific implementation: */
    gasnete_coll_pami_scatt(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_PASS);
  }
}

#endif /* GASNET_PAMI_NATIVE_COLL */
