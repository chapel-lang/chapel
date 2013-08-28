/*
 * Copyright (c) 2012  Mellanox Technologies, LTD. All rights reserved.
 */

#include "gasnet_fca.h"
#include <stdio.h>
#include <string.h>
#include <gasnet_coll_internal.h>
int gasnet_fca_barrier(gasnet_team_handle_t team)
{
    fca_comm_data_t *fca_comm_data = &team->fca_comm_data;

    FCA_VERBOSE(5,"Using FCA Barrier");
    return fca_do_barrier(fca_comm_data->fca_comm);
}

int gasnet_fca_broadcast(void *src, void *dst, int root, size_t size,
                                gasnet_team_handle_t team, int flags)
{
    fca_comm_data_t *fca_comm_data = &team->fca_comm_data;
    fca_bcast_spec_t spec;
    int ret;
    if (GASNET_COLL_IN_ALLSYNC & flags){
        gasnet_fca_barrier(team);
    }
    FCA_VERBOSE(5,"DOING FCA BCAST, root %i, src %p, dst %p", root, src, dst);
    spec.root = root;

    
    if (fca_comm_data->my_rank == spec.root)
    {
      spec.buf = src;
    }
    else{
        spec.buf = dst;
    }
    spec.size = size;
    if (spec.size > fca_comm_data->fca_comm_caps.max_payload) {
        FCA_VERBOSE(1, "Unsupported bcast operation size %d, using fallback",
                spec.size);
        return GASNET_FCA_ERROR;
    }
    ret = fca_do_bcast(fca_comm_data->fca_comm, &spec);
    if (fca_comm_data->my_rank == spec.root){
        if (src !=  dst)
        {
            memcpy(dst,src,size);
        }
    }
    if (ret < 0) {
        FCA_VERBOSE(5,"FCA Broadcast failed, using original Broadcast");
    }
    if (GASNET_COLL_OUT_ALLSYNC & flags){
        gasnet_fca_barrier(team);
    }
    return ret;
}

int gasnet_fca_all_gather_all(void *dst,
        void *src,
        size_t nbytes, gasnet_team_handle_t team, int flags)
{
#if GASNET_FCA_ALLGATHER
    fca_comm_data_t *fca_comm_data = &team->fca_comm_data;
    fca_gather_spec_t spec = {0,};
    int ret;
    if (GASNET_COLL_IN_ALLSYNC & flags){
        gasnet_fca_barrier(team);
    }
    FCA_VERBOSE(5,"DOING FCA_ALL_GATHER");

    spec.size = (int)nbytes;
    spec.rbuf = dst;
    spec.sbuf = src;

    ret = fca_do_allgather(fca_comm_data->fca_comm, &spec);

    if (GASNET_COLL_OUT_ALLSYNC & flags){
        gasnet_fca_barrier(team);
    }
    return ret;

#endif
    return GASNET_FCA_ERROR;
}

int gasnet_fca_reduce(int root,  void *target, const void *source,
                      int fca_op, int fca_dtype, int length,
                      gasnet_team_handle_t team, int flags)
{
    fca_reduce_spec_t spec;
    fca_comm_data_t *fca_comm_data = &team->fca_comm_data;
    int ret;
    if (GASNET_COLL_IN_ALLSYNC & flags){
        FCA_VERBOSE(5,"FCA REDUCE: performing GASNET_COLL_IN_ALLSYNC barrier");
        gasnet_fca_barrier(team);
    }
    FCA_VERBOSE(5,"DOING FCA_ALL_GATHER");
    spec.sbuf = (void *)source;
    spec.rbuf = target;
    spec.dtype = (enum fca_reduce_dtype_t)fca_dtype;
    spec.op = (enum fca_reduce_op_t)fca_op;
    spec.root = root;
    spec.length = length;
    ret = fca_do_reduce(fca_comm_data->fca_comm, &spec);
    if (GASNET_COLL_OUT_ALLSYNC & flags){
        FCA_VERBOSE(5,"FCA REDUCE: performing GASNET_COLL_OUT_ALLSYNC barrier");
        gasnet_fca_barrier(team);
    }
    return ret;
}

int gasnet_fca_reduce_all( void *target, const void *source, int fca_op,
                           int fca_dtype, int length,
                           gasnet_team_handle_t team, int flags)
{
    fca_reduce_spec_t spec;
    fca_comm_data_t *fca_comm_data = &team->fca_comm_data;
    int ret;
    if (GASNET_COLL_IN_ALLSYNC & flags){
        FCA_VERBOSE(5,"FCA ALL REDUCE: performing GASNET_COLL_IN_ALLSYNC barrier");
        gasnet_fca_barrier(team);
    }
    FCA_VERBOSE(5,"DOING FCA_REDUCE_ALL");
    spec.sbuf = (void *)source;
    spec.rbuf = target;
    spec.dtype = (enum fca_reduce_dtype_t)fca_dtype;
    spec.op = (enum fca_reduce_op_t)fca_op;
    spec.length = length;
    ret = fca_do_all_reduce(fca_comm_data->fca_comm, &spec);
    if (GASNET_COLL_OUT_ALLSYNC & flags){
        FCA_VERBOSE(5,"FCA ALL REDUCE: performing GASNET_COLL_OUT_ALLSYNC barrier");
        gasnet_fca_barrier(team);
    }
    return ret;
}
