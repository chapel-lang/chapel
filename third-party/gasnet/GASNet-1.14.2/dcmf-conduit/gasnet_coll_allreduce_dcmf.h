/* $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_allreduce_dcmf.h,v $
 * $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.1.2.2 $
 * Description: GASNet allreduce implementation on DCMF
 * Copyright 2010, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef GASNET_COLL_ALLREDUCE_DCMF_H_
#define GASNET_COLL_ALLREDUCE_DCMF_H_

#include <dcmf_collectives.h>

#include <gasnet_extended_coll_dcmf.h>

/** 
 * Data structure for storing information of a dcmf allreduce operation
 */
typedef struct {
  DCMF_CollectiveRequest_t request;
  DCMF_Callback_t cb_done;
  DCMF_Consistency consistency;
  DCMF_Geometry_t *geometry;
  char *src;
  char *dst;
  unsigned count;
  DCMF_Dt datatype;
  DCMF_Op reduceop;
  volatile unsigned active;
  gasnet_team_handle_t team;
} gasnete_dcmf_allreduce_data_t;

/** 
 * Register allreduce protocols.  It should be done before calling any gasnet
 * dcmf allreduce function.
 */
void gasnete_coll_allreduce_proto_register(void);

/**
 * Non-Blocking version of allreduce, which overloads the default gasnet
 * allreduce_nb function.
 */
gasnet_coll_handle_t gasnete_coll_allreduce_nb_dcmf(gasnet_team_handle_t team,
                                                    void * dst,
                                                    void *src,
                                                    size_t count, 
                                                    DCMF_Dt datatype,
                                                    DCMF_Op reduceop,
                                                    int flags,
                                                    uint32_t sequence
                                                    GASNETE_THREAD_FARG);

/**
 * Set the DCMF allreduce protocol used by the team.
 */
int gasnete_coll_allreduce_set_proto(gasnet_team_handle_t team,
                                     DCMF_Allreduce_Protocol kind);

/**
 * Set the default DCMF allreduce protocol used by the team.
 */
void gasnete_coll_allreduce_set_default_proto(gasnet_team_handle_t team);


/**
 * Set the DCMF named barrier protocol uesd by the team.
 */
int gasnete_coll_namedbarrier_set_proto(gasnet_team_handle_t team,
                                        DCMF_Allreduce_Protocol kind);


/**
 * Set the default DCMF named barrier protocol, which is implemented
 * with allreduce.
 */
void gasnete_coll_namedbarrier_set_default_proto(gasnet_team_handle_t team);


#endif /* GASNET_COLL_ALLREDUCE_DCMF_H_ */
