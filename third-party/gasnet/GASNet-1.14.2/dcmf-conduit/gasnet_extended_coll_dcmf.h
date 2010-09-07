/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_extended_coll_dcmf.h,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.5.2.1 $
 * Description: GASNet extended collectives implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_EXTENDED_COLL_DCMF_H_
#define GASNET_EXTENDED_COLL_DCMF_H_

#include <gasnet_core_internal.h>
#include <gasnet_extended_refcoll.h>
#include <gasnet_coll.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_autotune_internal.h>

#include <gasnet_coll_barrier_dcmf.h>

/* The following function prototypes should be declared in
 * gasnet_extended_refcoll.h, however they only appear in
 * gasnet_extended_refcoll.c.  Therefore, they are included here for
 * referencing them in the dcmf collective code correctly. */
extern gasnet_coll_handle_t
gasnete_coll_exchange_nb_default(gasnet_team_handle_t team,
                                 void *dst, void *src,
                                 size_t nbytes, int flags, uint32_t sequence
                                 GASNETE_THREAD_FARG);


extern gasnet_coll_handle_t
gasnete_coll_broadcast_nb_default(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags, uint32_t sequence
                                  GASNETE_THREAD_FARG);

extern gasnet_coll_handle_t
gasnete_coll_op_generic_init_with_scratch(gasnete_coll_team_t team, int flags,
                                          gasnete_coll_generic_data_t *data, 
                                          gasnete_coll_poll_fn poll_fn,
                                          uint32_t sequence, 
                                          gasnete_coll_scratch_req_t *scratch_req, 
                                          int num_params, 
                                          uint32_t *param_list, 
                                          gasnete_coll_tree_data_t *tree_info 
                                          GASNETE_THREAD_FARG);
/* end of adhoc function prototype declarations */

/**
 * data structure for storing dcmf team information  
 */
typedef struct {
  /* struct gasnete_coll_team_t_ baseteam; */
  DCMF_CollectiveRequest_t barrier_req;
  DCMF_CollectiveRequest_t named_barrier_req;
  DCMF_Geometry_t geometry;
  DCMF_CollectiveProtocol_t *bcast_proto;
  DCMF_CollectiveProtocol_t *a2a_proto;
  DCMF_CollectiveProtocol_t *allreduce_proto;
  DCMF_CollectiveProtocol_t *named_barrier_proto;
  volatile int in_barrier;
} gasnete_coll_team_dcmf_t;  

extern int gasnete_coll_dcmf_inited;

/* global flag to indicate whether to use the DCMF collectives or not */
extern int gasnete_use_dcmf_coll;

/* global flag to indicate if there is a DCMF collective operation executing */
extern volatile int gasnete_dcmf_busy;

GASNETE_COLL_TEAM_CONDUIT_DECLS;

/**
 * Initialize the dcmf data structures for gasnet team, used in the
 * DCMF conduit.
 */
void gasnete_dcmf_team_init(gasnet_team_handle_t team,
                            DCMF_CollectiveProtocol_t ** bar_protos,
                            int num_barriers,
                            DCMF_CollectiveProtocol_t ** lbar_protos, 
                            int num_localbarriers);

/**
 * Get the DCMF geometry of the team with team_id
 */
DCMF_Geometry_t * gasnete_dcmf_get_geometry(int team_id);

#endif /* GASNET_EXTENDED_COLL_DCMF_H_ */
