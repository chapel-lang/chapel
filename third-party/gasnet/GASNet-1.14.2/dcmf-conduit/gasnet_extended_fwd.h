/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.6.2.2 $
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

#define GASNET_EXTENDED_VERSION      1.2
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         DCMF
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1


/* predicate is true if we "bend" the rules of barrier to improve performance
   (may deadlock if threads disagree on named/anon barrier flags) */
extern int gasnete_dcmfbarrier_fast;
#define GASNETE_USING_DCMFFAST_BARRIER() ((int)gasnete_dcmfbarrier_fast)


  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    

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
#define gasnete_coll_teambarrier gasnete_coll_teambarrier_dcmf
#define gasnete_coll_teambarrier_notify gasnete_coll_teambarrier_notify_dcmf
#define gasnete_coll_teambarrier_wait gasnete_coll_teambarrier_wait_dcmf
#endif /* #if GASNET_SEQ */
#endif /* #ifndef GASNET_NO_DCMF_COLL */

extern void gasnete_coll_init_dcmf(void);
#define GASNETE_COLL_TEAM_CONDUIT_DECLS \
    extern void gasnete_coll_team_init_dcmf(gasnet_team_handle_t team); \
    extern void gasnete_coll_team_fini_dcmf(gasnet_team_handle_t team); \
    extern void gasnete_coll_teambarrier_dcmf(gasnet_team_handle_t team);

#if 0
#define GASNETE_COLL_CONDUIT_COLLECTIVES 1
#define GASNETE_COLL_CONDUIT_BROADCAST_OPS GASNETE_COLL_BROADCAST_DCMF, GASNETE_COLL_BROADCAST_DCMF_TREE
#endif

#endif

