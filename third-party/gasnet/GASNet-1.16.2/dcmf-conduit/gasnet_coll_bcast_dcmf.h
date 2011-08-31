/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_bcast_dcmf.h,v $
 *     $Date: 2009/10/28 03:01:34 $
 * $Revision: 1.5 $
 * Description: GASNet broadcast implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_COLL_BCAST_DCMF_H_
#define GASNET_COLL_BCAST_DCMF_H_

#include <gasnet_extended_coll_dcmf.h>

/**
 * DCMF broadcast algorithms 
 */
typedef enum {
  TREE_BROADCAST,
  TORUS_RECTANGLE_BROADCAST,
  TORUS_BINOMIAL_BROADCAST,
  TORUS_ASYNCBROADCAST_RECTANGLE,
  TORUS_ASYNCBROADCAST_BINOMIAL,
  GASNET_DEFAULT_BROADCAST,
  G_DCMF_BCAST_PROTO_NUM 
} gasnete_dcmf_bcast_kind_t;

/** 
 * Data structure for storing information of a dcmf broadcast operation
 */
typedef struct {
  union {
    DCMF_CollectiveRequest_t coll;
    DCMF_Request_t global;
  } request;
  DCMF_Callback_t cb_done;
  DCMF_Consistency consistency;
  DCMF_Geometry_t *geometry;
  unsigned root;
  char *src;
  char *dst;
  unsigned bytes;
  volatile unsigned active;
  gasnet_team_handle_t team;
} gasnete_dcmf_bcast_data_t;

/** 
 * Register broadcast protocols.  It should be done before calling any gasnet
 * dcmf broacast function.
 */
void gasnete_coll_bcast_proto_register(void);

/**
 * Non-Blocking version of broadcast, which overloads the default gasnet
 * broadcast_nb function.
 */
gasnet_coll_handle_t 
gasnete_coll_broadcast_nb_dcmf(gasnet_team_handle_t team,
                               void *dst,
                               gasnet_image_t srcimage, 
                               void *src,
                               size_t nbytes, 
                               int flags,
                               uint32_t sequence
                               GASNETE_THREAD_FARG);

/**
 * Blocking version of broadcast, which overloads the default gasnet broadcast
 * function.
 */
void gasnete_coll_broadcast_dcmf(gasnet_team_handle_t team,
                                 void *dst,
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags GASNETE_THREAD_FARG);

/**
 * Set the DCMF broadcast protocol used by the team.
 */
int gasnete_coll_bcast_set_proto(gasnet_team_handle_t team,
                                 gasnete_dcmf_bcast_kind_t kind);

/**
 * Set the default DCMF broadcast protocol used by the team.
 */
void gasnete_coll_bcast_set_default_proto(gasnet_team_handle_t team);

/**
 * print out the internal information about the broadcast operation.
 */
void gasnete_coll_dcmf_bcast_print(FILE *fp, gasnete_dcmf_bcast_data_t *bcast);

/* 
 * number of streams used in the torus rectangular broadcast, minimum is 1,
 * maximum is 3 for mesh and 6 for torus. 
 */
extern int gasnete_dcmf_bcast_num_colors;

#endif /* GASNET_COLL_BCAST_DCMF_H_ */
