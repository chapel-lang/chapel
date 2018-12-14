/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_coll_fwd.h $
 * Description: GASNet Collectives Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_COLL_FWD_H
#define _GASNET_COLL_FWD_H

#ifndef GASNETI_COLL_PROGRESSFNS
  extern void gasneti_coll_progressfn(void);
  #define GASNETI_COLL_PROGRESSFNS(FN) \
      FN(gasneti_pf_coll, BOOLEAN, gasneti_coll_progressfn)
#endif

/* stats needed by the COLL reference implementation */
#ifndef GASNETI_COLL_STATS
  #define GASNETI_COLL_STATS(CNT,VAL,TIME)    \
        VAL(W, COLL_BROADCAST, sz)            \
        VAL(W, COLL_BROADCAST_NB, sz)         \
        VAL(W, COLL_SCATTER, sz)              \
        VAL(W, COLL_SCATTER_NB, sz)           \
        VAL(W, COLL_GATHER, sz)               \
        VAL(W, COLL_GATHER_NB, sz)            \
        VAL(W, COLL_GATHER_ALL, sz)           \
        VAL(W, COLL_GATHER_ALL_NB, sz)        \
        VAL(W, COLL_EXCHANGE, sz)             \
        VAL(W, COLL_EXCHANGE_NB, sz)          \
        VAL(W, COLL_REDUCE_NB, cnt)           \
        VAL(W, COLL_REDUCE_ALL_NB, cnt)       \
        /*END*/
#endif

#define GASNETE_COLL_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_coll_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_COLL_AUXSEG_FNS() gasnete_coll_auxseg_alloc, 

// For use in gasneti_threaddata_t
struct gasnete_coll_threaddata_t_;
typedef struct gasnete_coll_threaddata_t_ gasnete_coll_threaddata_t;

#endif
