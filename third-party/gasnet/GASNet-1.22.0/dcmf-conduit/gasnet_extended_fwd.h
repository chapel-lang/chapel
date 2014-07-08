/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2013/06/29 04:56:57 $
 * $Revision: 1.17 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>
 *                 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.3
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         DCMF
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1

  /* if conduit-internal threads may call the Extended API and/or they may run
     progress functions, then define GASNETE_CONDUIT_THREADS_USING_TD to the
     maximum COUNT of such threads to allocate space for their threaddata
   */
#if 0
  #define GASNETE_CONDUIT_THREADS_USING_TD ###
#endif

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    

#define GASNETE_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc, 

#define GASNETE_COLL_CONDUIT_BARRIERS GASNETE_COLL_BARRIER_DCMF

/*all the conduit collective information*/
#define GASNETE_COLL_TEAM_EXTRA   void * dcmf_tp;

/* conduit specific collective functions overiding the default
   reference implementation in gasnet/extended-ref */
#ifndef GASNET_NO_DCMF_COLL
#if GASNET_SEQ  
#define gasnete_coll_init_conduit gasnete_coll_init_dcmf
#define gasnete_coll_team_init_conduit gasnete_coll_team_init_dcmf
#define gasnete_coll_team_fini_conduit gasnete_coll_team_fini_dcmf
#define gasnete_coll_exchange_nb gasnete_coll_exchange_nb_dcmf
#define gasnete_coll_exchange gasnete_coll_exchange_dcmf
#define gasnete_coll_broadcast_nb gasnete_coll_broadcast_nb_dcmf
#define gasnete_coll_broadcast gasnete_coll_broadcast_dcmf 
#endif /* #if GASNET_SEQ */
#endif /* #ifndef GASNET_NO_DCMF_COLL */

#if 0
#define GASNETE_COLL_CONDUIT_COLLECTIVES 1
#define GASNETE_COLL_CONDUIT_BROADCAST_OPS GASNETE_COLL_BROADCAST_DCMF, GASNETE_COLL_BROADCAST_DCMF_TREE
#endif

#define GASNETE_EXTENDED_NEEDS_CORE 1
#define GASNETE_CONDUIT_EOP_FIELDS \
  /*make sure the eops are sep. by atleast one cacheline*/ \
  char _pad[GASNETI_CACHE_PAD(sizeof(uint8_t) + sizeof(gasnete_threadidx_t) + sizeof(gasnete_eopaddr_t))]; \
  DCMF_Request_t dcmf_req;

#endif

