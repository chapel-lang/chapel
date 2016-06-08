/*
 * Description: GASNet MXM conduit connection management
 * Copyright (c) 2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* ------------------------------------------------------------------------------------ */

extern gasnet_mxm_module_t   gasnet_mxm_module;

/* ------------------------------------------------------------------------------------ */

static int gasnetc_connect_static(void)
{
    int i;
    mxm_error_t mxm_status;
#if MXM_API < MXM_VERSION(2,0)
    mxm_conn_req_t * mxm_conn_reqs;

    /*
     * Allocate and fill in connection requests
     */
    mxm_conn_reqs = gasneti_calloc(gasneti_nodes, sizeof(mxm_conn_req_t));
    if (mxm_conn_reqs == NULL) {
        MXM_ERROR("Out of memory");
        return GASNET_ERR_NOT_INIT;
    }

#endif

    gasnet_mxm_module.connections = gasneti_calloc(gasneti_nodes, sizeof(mxm_conn_h));
    if (gasnet_mxm_module.connections == NULL) {
        MXM_ERROR("Out of memory");
        return GASNET_ERR_NOT_INIT;
    }


    /*
     * NOTE: we do NOT copy the data from the remote_eps buffer,
     * but use it by pointer.
     * ALSO NOTE: for the same reason, we do NOT free remote_eps
     * array - its content is used by gasnet_mxm_module.mxm_conn_reqs
     */

    for (i = 0; i < gasneti_nodes; ++i) {
#if MXM_API < MXM_VERSION(2,0)
        mxm_conn_reqs[i].ptl_addr[MXM_PTL_SELF] =
            (struct sockaddr *) &gasnet_mxm_module.remote_eps[i].ptl_addr[MXM_PTL_SELF];
        mxm_conn_reqs[i].ptl_addr[MXM_PTL_RDMA] =
            (struct sockaddr *) &gasnet_mxm_module.remote_eps[i].ptl_addr[MXM_PTL_RDMA];
#else
        mxm_status = mxm_ep_connect(gasnet_mxm_module.mxm_ep, 
                gasnet_mxm_module.remote_eps[i].ep_addr, 
                &gasnet_mxm_module.connections[i]
                );
        if (mxm_status != MXM_OK) {
             MXM_ERROR("MXM returned connect error: %s\n", mxm_error_string(mxm_status));
             return mxm_status;
        }
#endif
    }

#if MXM_API < MXM_VERSION(2,0)
    /*
     * Connect to ALL the nodes through MXM, including the local nodes
     */
    mxm_status = mxm_ep_connect(gasnet_mxm_module.mxm_ep,
                                mxm_conn_reqs, gasnet_nodes(), 1000);

    if (mxm_status != MXM_OK) {
        MXM_ERROR("MXM returned connect error: %s\n", mxm_error_string(mxm_status));
        for (i = 0; i < gasnet_nodes(); ++i) {
            if (mxm_conn_reqs[i].error != MXM_OK) {
                MXM_ERROR("MXM EP connect to GASNet node %d error: %s\n", i,
                          mxm_error_string(mxm_conn_reqs[i].error));
            }
        }
        return mxm_status;
    }

    /*
     * Save returned connections and free connection requests
     */

    for (i = 0; i < gasnet_nodes(); ++i) {
        gasnet_mxm_module.connections[i] = mxm_conn_reqs[i].conn;
    }
    gasneti_free(mxm_conn_reqs);
#endif

    /* TODO: using 0xabcd as context ID - do we have only one communicator? */
    mxm_status = mxm_mq_create(gasnet_mxm_module.mxm_context, 0xabcd, &gasnet_mxm_module.mxm_mq);
    if (mxm_status != MXM_OK) {
        MXM_ERROR("MXM mq create failed (%s)", mxm_error_string(mxm_status));
        return GASNET_ERR_NOT_INIT;
    }

    return 0;
}

/* ------------------------------------------------------------------------------------ */
/* Setup statically-connected communication */
extern int
gasnetc_connect_init(void)
{
    mxm_error_t                     mxm_status;

#if GASNETC_DYNAMIC_CONNECT
    MXM_ERROR("Dynamic Connect mode over MXM is not supported\n");
    return GASNET_ERR_NOT_INIT;
#endif

    mxm_status = (mxm_error_t)gasnetc_connect_static();
    if (mxm_status != MXM_OK) {
        MXM_ERROR("Connection initialization failed\n");
        return GASNET_ERR_NOT_INIT;
    }

    GASNETI_TRACE_PRINTF(C,("I am node group (host) %d of %d node groups",
                            gasneti_nodemap_global_rank, gasneti_nodemap_global_count));
    GASNETI_TRACE_PRINTF(C,("I am node %d of %d on-node peers",
                            gasneti_nodemap_local_rank, gasneti_nodemap_local_count));

    GASNETI_TRACE_PRINTF(I, ("Connected at startup to all remote nodes"));
    return GASNET_OK;
} /* gasnetc_connect_init */
