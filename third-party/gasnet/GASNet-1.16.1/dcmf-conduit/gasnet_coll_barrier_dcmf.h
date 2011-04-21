/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_barrier_dcmf.h,v $
 *     $Date: 2009/10/28 03:01:34 $
 * $Revision: 1.5 $
 * Description:  GASNet team barrier implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_COLL_BARRIER_DCMF_H_
#define GASNET_COLL_BARRIER_DCMF_H_

/* Barrier protocols */
typedef enum {
  GI_BARRIER=0,
  TREE_BARRIER,
  TORUS_RECTANGLE_BARRIER,
  TORUS_RECTANGLE_LB_BARRIER,
  TORUS_BINOMIAL_BARRIER,
  LOCKBOX_BARRIER, /* local barrier */
  G_DCMF_BARRIER_PROTO_NUM
} gasnete_dcmf_barrier_proto_t;

extern DCMF_CollectiveProtocol_t *gasnete_dcmf_barriers[G_DCMF_BARRIER_PROTO_NUM];
extern int gasnete_dcmf_num_barriers; /**< num. of available barrier protocols */

extern DCMF_CollectiveProtocol_t *gasnete_dcmf_localbarriers[G_DCMF_BARRIER_PROTO_NUM];
extern int gasnete_dcmf_num_localbarriers; /**< num. of available local barrier protocols */

/** Register DCMF barrier protocols */
void gasnete_coll_barrier_proto_register(void);

void gasnete_coll_teambarrier_dcmf(gasnet_team_handle_t team);

void gasnete_coll_teambarrier_notify_dcmf(gasnet_team_handle_t team);

void gasnete_coll_teambarrier_wait_dcmf(gasnet_team_handle_t team);

int gasnete_coll_teambarrier_try_dcmf(gasnet_team_handle_t team);

#endif /* GASNET_COLL_BARRIER_DCMF_H_ */
