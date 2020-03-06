/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#include <firehose_trace.h>

#define GASNET_EXTENDED_VERSION      2.5
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         IBV
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

/* Addition(s) to barrier-types enum: */
#define GASNETE_COLL_CONDUIT_BARRIERS \
        GASNETE_COLL_BARRIER_IBDISSEM

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
        GASNETI_RATOMIC_STATS(CNT,VAL,TIME)  \
	GASNETI_FIREHOSE_STATS(CNT,VAL,TIME) \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)           

#define GASNETE_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc, 

/* We perform these blocking ops w/o the overhead of eop alloc/free: */
#define GASNETI_DIRECT_BLOCKING_GET 1
#define GASNETI_DIRECT_BLOCKING_PUT 1

/* Configure use of AM-based implementation of get/put */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */
// We want to call the amref versions for out-of-segment cases
#define GASNETE_BUILD_AMREF_GET_HANDLERS 1
#define GASNETE_BUILD_AMREF_GET 1
#define GASNETE_BUILD_AMREF_PUT_HANDLERS 1
#define GASNETE_BUILD_AMREF_PUT 1

#if !defined(GASNET_DISABLE_MUNMAP_DEFAULT) && PLATFORM_ARCH_64
 // default to disabling munmap for bug 955 if firehose might be used
 #if GASNET_SEGMENT_FAST && GASNETC_IBV_ODP
   #define GASNET_DISABLE_MUNMAP_DEFAULT (!gasnetc_use_odp)
 #else
   #define GASNET_DISABLE_MUNMAP_DEFAULT 1
 #endif
#endif
// this VIS algorithm uses put/get with local-side buffers that are dynamically malloced and freed, 
// thus is only safe if we disabled malloc munmap to avoid running afowl of firehose bug3364/bug955
#ifdef PLATFORM_OS_SOLARIS // Does not suffer from bug 955
#define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER_DEFAULT 1
#else // Linux and others, assume the worst
#define GASNETE_USE_REMOTECONTIG_GATHER_SCATTER_DEFAULT gasneti_malloc_munmap_disabled
#endif

// Configure default VIS tuning knobs
// 12/15/17: Measurements on multiple systems show 256 is a good value
#define GASNETE_VIS_MAXCHUNK_DEFAULT 256

#endif

