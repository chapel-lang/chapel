/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#include <firehose_trace.h>

#define GASNET_EXTENDED_VERSION      1.13
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         IBV
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

/* Addition(s) to barrier-types enum: */
#define GASNETE_COLL_CONDUIT_BARRIERS \
        GASNETE_COLL_BARRIER_IBDISSEM

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
  /* Each RCV thread needs a slot in the threadtable.  The CONN thread doesn't. */
#if GASNETC_IBV_RCV_THREAD
 #ifdef GASNETC_IBV_MAX_HCAS
  #define GASNETE_CONDUIT_THREADS_USING_TD GASNETC_IBV_MAX_HCAS
 #else
  #define GASNETE_CONDUIT_THREADS_USING_TD 1
 #endif
#endif

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
	GASNETI_COLL_STATS(CNT,VAL,TIME)     \
	GASNETI_FIREHOSE_STATS(CNT,VAL,TIME) \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)           

#define GASNETE_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc, 

/* We perform these blocking ops w/o the overhead of eop alloc/free: */
#define GASNETI_DIRECT_GET_BULK 1
#define GASNETI_DIRECT_PUT_BULK 1

#define GASNETE_EOP_COUNTED 1
#define GASNETE_EXTENDED_NEEDS_CORE 1

/* Configure use of AM-based implementation of get/put/memset */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */
#define GASNETE_USING_REF_EXTENDED_MEMSET   1

/* in order to use reference memset with our conduit-specific eop */
#define GASNETE_AMREF_USE_MARKDONE          1

/* Conduit implements memset directly via amref: */
#define gasnete_amref_memset_nb     gasnete_memset_nb
#define gasnete_amref_memset_nbi    gasnete_memset_nbi

#endif

