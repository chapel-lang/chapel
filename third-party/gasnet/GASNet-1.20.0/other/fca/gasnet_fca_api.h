/*
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 */

#include <fca_api.h>
#include <fca_version.h>

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


typedef struct gasnet_fca_ops_t {

    /* FCA Context operations */
    int (*init)(fca_init_spec_t *spec, fca_t **context);
    void (*cleanup)(fca_t *context);

    /* Fabric communicator creation */
    int (*comm_new)(fca_t *context, fca_comm_new_spec_t *spec, fca_comm_desc_t *comm_desc);
    int (*comm_end)(fca_t *context, int comm_id);
    void* (*get_rank_info)(fca_t *context, int *size);
    void (*free_rank_info)(void *rank_info);

    /* Local communicator creation */
    int (*comm_init)(fca_t *context, int proc_idx, int num_procs, int comm_size,
            fca_comm_desc_t *comm_desc, fca_comm_t** fca_comm);
    void (*comm_destroy)(fca_comm_t *comm);
    int (*comm_get_caps)(fca_comm_t *comm, fca_comm_caps_t *caps);

    /* Collectives supported by FCA */
    int (*do_reduce)(fca_comm_t *comm, fca_reduce_spec_t *spec);
    int (*do_all_reduce)(fca_comm_t *comm, fca_reduce_spec_t *spec);
    int (*do_bcast)(fca_comm_t *comm, fca_bcast_spec_t *spec);
    int (*do_barrier)(fca_comm_t *comm);

    /* Helper functions */
    unsigned long (*get_version)(void);
    char * (*get_version_string)(void);
    fca_init_spec_t *(*parse_spec_file)(char* spec_ini_file);
    void (*free_init_spec)(fca_init_spec_t *fca_init_spec);
    int (*translate_mpi_op)(char *mpi_op);
    int (*translate_mpi_dtype)(char *mpi_dtype);
    int (*get_dtype_size)(int dtype);
    const char* (*strerror)(int code);
} gasnet_fca_ops_t;
static inline int gasnet_fca_comm_init(gasnet_fca_ops_t *fca_ops,
        fca_t *fca_context, int rank, int comm_size,
        int local_proc_idx, int num_local_procs,
        fca_comm_desc_t *comm_desc,
        fca_comm_t **fca_comm)
{
    return fca_ops->comm_init(fca_context, local_proc_idx, num_local_procs,
            comm_size, comm_desc, fca_comm);
}
#elif GASNET_FCA_VERSION == 20 || GASNET_FCA_VERSION == 21 || GASNET_FCA_VERSION == 22 || GASNET_FCA_VERSION == 25

#define GASNET_FCA_ALLGATHER          1
#define GASNET_FCA_PROGRESS           1
#define EUSEGASNET                     287


typedef struct gasnet_fca_ops_t {

    /* FCA Context operations */
    int (*init)(fca_init_spec_t *spec, fca_t **context);
    void (*cleanup)(fca_t *context);
    void (*progress)(fca_t *context);

    /* Fabric communicator creation */
    int (*comm_new)(fca_t *context, fca_comm_new_spec_t *spec, fca_comm_desc_t *comm_desc);
    int (*comm_end)(fca_t *context, int comm_id);
    void* (*get_rank_info)(fca_t *context, int *size);
    void (*free_rank_info)(void *rank_info);

    /* Local communicator creation */
    int (*comm_init)(fca_t *context, fca_comm_init_spec_t *spec, fca_comm_t** fca_comm);
    void (*comm_destroy)(fca_comm_t *comm);
    int (*comm_get_caps)(fca_comm_t *comm, fca_comm_caps_t *caps);

    /* Collectives supported by FCA */
    int (*do_all_reduce)(fca_comm_t *comm, fca_reduce_spec_t *spec);
    int (*do_reduce)(fca_comm_t *comm, fca_reduce_spec_t *spec);
    int (*do_bcast)(fca_comm_t *comm, fca_bcast_spec_t *spec);
    int (*do_barrier)(fca_comm_t *comm);
    int (*do_allgather)(fca_comm_t *comm, fca_gather_spec_t *spec);
    int (*do_allgatherv)(fca_comm_t *comm, fca_gatherv_spec_t *spec);
    /* Helper functions */
    unsigned long (*get_version)(void);
    char * (*get_version_string)(void);
    fca_init_spec_t *(*parse_spec_file)(char* spec_ini_file);
    void (*free_init_spec)(fca_init_spec_t *fca_init_spec);
    int (*translate_mpi_op)(char *mpi_op);
    int (*translate_mpi_dtype)(char *mpi_dtype);
    int (*get_dtype_size)(int dtype);
    const char* (*strerror)(int code);
} gasnet_fca_ops_t;

static inline int gasnet_fca_comm_init(gasnet_fca_ops_t *fca_ops,
        fca_t *fca_context, int rank, int comm_size,
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
    return fca_ops->comm_init(fca_context, &spec, fca_comm);
}
#else

#error "FCA API version is unsupported"

#endif
