/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_coll_fwd.h,v $
 *     $Date: 2009/10/22 23:28:04 $
 * $Revision: 1.3 $
 * Description: GASNet Collectives Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_COLL_FWD_H
#define _GASNET_COLL_FWD_H

#define GASNETI_COLL_PROGRESSFNS(FN) 

/* stats needed by the COLL reference implementation */
#ifndef GASNETI_COLL_STATS
  #define GASNETI_COLL_STATS(CNT,VAL,TIME)    \
        VAL(X, COLL_TRY_SYNC, success)        \
        VAL(X, COLL_TRY_SYNC_ALL, success)    \
        VAL(X, COLL_TRY_SYNC_SOME, success)   \
        TIME(X, COLL_WAIT_SYNC, waittime)     \
        TIME(X, COLL_WAIT_SYNC_ALL, waittime) \
        TIME(X, COLL_WAIT_SYNC_SOME, waittime)\
        VAL(W, COLL_BROADCAST, sz)            \
        VAL(W, COLL_BROADCAST_NB, sz)         \
        VAL(W, COLL_BROADCAST_M, sz)          \
        VAL(W, COLL_BROADCAST_M_NB, sz)       \
        VAL(W, COLL_SCATTER, sz)              \
        VAL(W, COLL_SCATTER_NB, sz)           \
        VAL(W, COLL_SCATTER_M, sz)            \
        VAL(W, COLL_SCATTER_M_NB, sz)         \
        VAL(W, COLL_GATHER, sz)               \
        VAL(W, COLL_GATHER_NB, sz)            \
        VAL(W, COLL_GATHER_M, sz)             \
        VAL(W, COLL_GATHER_M_NB, sz)          \
        VAL(W, COLL_GATHER_ALL, sz)           \
        VAL(W, COLL_GATHER_ALL_NB, sz)        \
        VAL(W, COLL_GATHER_ALL_M, sz)         \
        VAL(W, COLL_GATHER_ALL_M_NB, sz)      \
        VAL(W, COLL_EXCHANGE, sz)             \
        VAL(W, COLL_EXCHANGE_NB, sz)          \
        VAL(W, COLL_EXCHANGE_M, sz)           \
        VAL(W, COLL_EXCHANGE_M_NB, sz)        \
        VAL(W, COLL_REDUCE, cnt)              \
        VAL(W, COLL_REDUCE_NB, cnt)           \
        VAL(W, COLL_REDUCE_M, cnt)            \
        VAL(W, COLL_REDUCE_M_NB, cnt)         \
        VAL(W, COLL_SCAN, cnt)                \
        VAL(W, COLL_SCAN_NB, cnt)             \
        VAL(W, COLL_SCAN_M, cnt)              \
        VAL(W, COLL_SCAN_M_NB, cnt)
#endif

#define GASNETE_AUXSEG_FNS() gasnete_coll_auxseg_alloc,

/*conduit has some collectives*/
#define GASNETE_COLL_CONDUIT_COLLECTIVES 1

#if GASNETE_COLL_CONDUIT_COLLECTIVES
#define GASNETE_COLL_CONDUIT_TUNING_PARAMETERS GASNETE_COLL_SMP_COLL_TREE_RADIX
#define GASNETE_COLL_CONDUIT_BROADCASTM_OPS GASNETE_COLL_BROADCAST_SMP_FLAT_GET, GASNETE_COLL_BROADCAST_SMP_FLAT_PUT, GASNETE_COLL_BROADCAST_SMP_TREE_INTFLAGS
#define GASNETE_COLL_CONDUIT_REDUCEM_OPS GASNETE_COLL_REDUCE_SMP_FLAT
#define GASNETE_COLL_CONDUIT_SCATTERM_OPS GASNETE_COLL_SCATTER_SMP_FLAT_PUT, GASNETE_COLL_SCATTER_SMP_FLAT_GET
#define GASNETE_COLL_CONDUIT_GATHERM_OPS GASNETE_COLL_GATHER_SMP_FLAT_PUT, GASNETE_COLL_GATHER_SMP_FLAT_GET
#define GASNETE_COLL_CONDUIT_GATHER_ALLM_OPS GASNETE_COLL_GATHER_ALL_SMP_FLAT_PUT, GASNETE_COLL_GATHER_ALL_SMP_FLAT_GET
#define GASNETE_COLL_CONDUIT_EXCHANGEM_OPS GASNETE_COLL_EXCHANGE_SMP_FLAT_PUT, GASNETE_COLL_EXCHANGE_SMP_FLAT_GET
#endif

#endif
