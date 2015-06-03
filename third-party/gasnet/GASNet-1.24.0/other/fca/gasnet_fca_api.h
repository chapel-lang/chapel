/*
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 */

#include <stdio.h>
#include <fca/fca_api.h>
#include <fca/fca_version.h>
#include <fca/config/fca_parse_specfile.h>

#ifndef FCA_API
#define GASNET_FCA_VERSION 12
#else
#define GASNET_FCA_VERSION FCA_API
#endif

/*
 *  * FCA API compatibility layer.
 *   * MPI build must define an FCA version macro.
 *    */

#define GASNET_FCA_BARRIER            1
#define GASNET_FCA_BCAST              1
#define GASNET_FCA_ALLREDUCE          1


#if GASNET_FCA_VERSION == 12

#define GASNET_FCA_ALLGATHER          0
#define FCA_API_ABI_MAJOR           1
#define FCA_API_ABI_MINOR           2
#define FCA_MAJOR_BIT               24ul
#define FCA_MINOR_BIT               16ul
#define EUSEGASNET                     287

#elif GASNET_FCA_VERSION == 20 || GASNET_FCA_VERSION == 21 || GASNET_FCA_VERSION == 22 || GASNET_FCA_VERSION == 25

#define GASNET_FCA_ALLGATHER          1
#define GASNET_FCA_PROGRESS           1
#define EUSEGASNET                     287

static inline int gasnet_fca_comm_init(fca_t *fca_context, int rank, int comm_size,
        int local_proc_idx, int num_local_procs,
        fca_comm_desc_t *comm_desc,
        fca_comm_t **fca_comm)
{
    fca_comm_init_spec_t spec;

    spec.rank = rank;
    spec.size = comm_size;
    spec.desc = *comm_desc;
    spec.proc_idx = local_proc_idx;
    spec.num_procs = num_local_procs;
    return fca_comm_init(fca_context, &spec, fca_comm);
}
#else

#error "FCA API version is unsupported"

#endif
