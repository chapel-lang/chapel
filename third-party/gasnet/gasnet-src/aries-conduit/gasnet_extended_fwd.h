/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header for aries Conduit (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      2.3
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         ARIES
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


/* Addition(s) to barrier-types enum: */
#define GASNETE_COLL_CONDUIT_BARRIERS \
        GASNETE_COLL_BARRIER_GNIDISSEM

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
        GASNETI_RATOMIC_STATS(CNT,VAL,TIME)  \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    

#define GASNETE_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc, 

/*
 * When implementing a conduit-specific implementation of the Extended API, one
 * can #define the following to 1 to change certain behaviors in gasnet_extended.h.
 * Alternatively, one can #define GASNETE_HAVE_EXTENDED_HELP_EXTRA_H and defined
 * these in a conduit-specific gasnet_extended_help_extra.h.
 *
 * GASNETI_DIRECT_BLOCKING_GET
 *   unset: gasnete_get() via gasnete_wait(gasnete_get_nb())
 *   set: conduit provides it own gasnete_get()
 *
 * GASNETI_DIRECT_BLOCKING_PUT
 *   unset: gasnete_put() via gasnete_wait(gasnete_put_nb())
 *   set: conduit provides it own gasnete_put()
 */
#define GASNETI_DIRECT_PUT_VAL 1
/*   unset: gasnete_put_val() via gasnete_put()
 *   set: conduit provides it own gasnete_put_val()
 */
#define GASNETI_DIRECT_PUT_NB_VAL 1
/*   unset: extern gasnete_put_nb_val() in gasnet_extended.c (or a macro)
 *   set: conduit provides own gasnete_put_nb_val() as an inline
 */
#define GASNETI_DIRECT_PUT_NBI_VAL 1
/*   unset: gasnete_put_nbi_val() via gasnete_put_nbi()
 *   set: conduit provides own gasnete_put_nbi_val()
 */
#define GASNETI_DIRECT_GET_VAL 1
/*   unset: extern gasnete_get_val() in gasnet_extended.c (or a macro)
 *   set: conduit provides own gasnete_get_val() as an inline
 */


/* Configure use of AM-based implementation of get/put */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */

#if defined(GASNET_PAR) && GASNETC_GNI_MULTI_DOMAIN
#  define GASNETE_TD_DOMAIN_IDX int domain_idx;
#else
#  define GASNETE_TD_DOMAIN_IDX /* empty */
#endif

#define GASNETE_CONDUIT_THREADDATA_FIELDS \
        GASNETE_TD_DOMAIN_IDX

// Configure default VIS tuning knobs
// 3/21/18: Measurements on NERSC Cori using the EX implementation for aries-conduit
// w/ default GASNETE_VIS_NPAM=1 and 4032 MaxMedium show the ideal MAXCHUNK for I+S on both
// KNL and Haswell to be the following values:
#define GASNETE_VIS_PUT_MAXCHUNK_DEFAULT 1330  // 3 strided chunks at 3d
#define GASNETE_VIS_GET_MAXCHUNK_DEFAULT 2028  // 2 strided chunks at 3d

// Aries CE support definitions:
#define GASNETE_COLL_NEEDS_CORE 1

#endif
