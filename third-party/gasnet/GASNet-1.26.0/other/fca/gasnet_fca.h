/*
 * Copyright (c) 2012 Mellanox Technologies LTD. All rights reserved.
 */

#ifndef GASNET_FCA_H
#define GASNET_FCA_H

#ifndef _INCLUDED_GASNET_H
#include <gasnet_internal.h>
#endif

#if defined(GASNET_SEQ) && (GASNET_CONDUIT_MXM || GASNET_CONDUIT_IBV)

#include "gasnet_fca_api.h"
#define GASNETI_USE_FCA 1
#define GASNET_FCA_ERROR -1
#define GASNET_FCA_SUCCESS 0

#define GASNETI_FCA_SPECFILE "/etc/fca_mpi_spec.ini"

typedef struct gasnet_fca_component_ {
   int is_initialized;

   /** Path to fca spec file */
   char* fca_spec_file;

   /** FCA device */
   char* fca_dev;

   /** Enable FCA */
   int   fca_enable;

   /** Enable FCA Barrier */
   int   fca_enable_barrier;

   /** Enable FCA Bcast */
   int   fca_enable_bcast;

   /** Enable FCA Allreduce */
   int   fca_enable_allreduce;

   /** Enable FCA Reduce */
   int   fca_enable_reduce;

   /** Enable FCA Allgather */
   int   fca_enable_allgather;

   /** FCA NP */
   int   fca_np;

   /* FCA global stuff */
   fca_t *fca_context;                                 /* FCA context handle */
} gasnet_fca_component_t;

typedef struct fca_comm_data_ {
    fca_comm_t *fca_comm;
    fca_comm_desc_t     fca_comm_desc;
    fca_comm_caps_t     fca_comm_caps;
    int my_rank;
    int local_proc_idx;
    int num_local_procs;
    int *local_ranks;
    int proc_count;
} fca_comm_data_t;


typedef enum gasnet_fca_coll_t{
    _FCA_BARRIER    = 0,
    _FCA_BCAST      = 1,
    _FCA_REDUCE     = 2,
    _FCA_ALLREDUCE  = 3,
    _FCA_ALLGATHER  = 4
} gasnet_fca_coll_t;

extern int gasnet_team_fca_is_active(gasnet_team_handle_t team, gasnet_fca_coll_t coll);



extern gasnet_fca_component_t gasnet_fca_component;
extern int gasnet_fca_open(int my_rank);
extern int gasnet_fca_close(void);
extern int gasnet_team_fca_enable(gasnet_team_handle_t team);
extern int gasnet_fca_verbose_level;

extern int gasnet_fca_broadcast(void *src, void *dst, int root, size_t size,
                                gasnet_team_handle_t team, int flags);
extern int gasnet_fca_all_gather_all(void *dst,
                                     void *src,
                                     size_t nbytes, gasnet_team_handle_t team, int flags);
extern int gasnet_fca_reduce(int root,  void *target, const void *source,
                             int fca_op, int fca_dtype, int length,
                             gasnet_team_handle_t team, int flags);
extern int gasnet_fca_reduce_all( void *target, const void *source, int fca_op,
                                  int fca_dtype, int length,
                                  gasnet_team_handle_t team, int flags);
extern int gasnet_fca_barrier(gasnet_team_handle_t team);

extern int gasnete_fca_barrier(gasnet_team_handle_t team, int *id_p, int *flags_p);

#define FCA_ERROR(format, ... ) FCA_VERBOSE(0,format, ## __VA_ARGS__)
#define FCA_VERBOSE(level,format, ... ) do{ \
    if (level <= gasnet_fca_verbose_level) \
    fprintf(stderr,"%s:%d - %s(): rank %d: " format "\n", __FILE__, __LINE__, __FUNCTION__, \
                gasnet_mynode(), ## __VA_ARGS__); \
}while(0);

/* API functions */

#endif /* seq + a supported conduit */
#endif
