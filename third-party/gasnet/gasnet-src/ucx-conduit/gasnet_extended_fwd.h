/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header for ucx Conduit (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H


#define GASNET_EXTENDED_VERSION      0.1
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         UCX
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

#define GASNETI_EOP_IS_HANDLE 1

/* Configure use of AM-based implementation of get/put */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */
#define GASNETE_USING_REF_EXTENDED_GET      0
#define GASNETE_USING_REF_EXTENDED_PUT      0

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
 *
 * GASNETI_DIRECT_PUT_VAL
 *   unset: gasnete_put_val() via gasnete_put()
 *   set: conduit provides it own gasnete_put_val()
 *
 * GASNETI_DIRECT_PUT_NB_VAL
 *   unset: extern gasnete_put_nb_val() in gasnet_extended.c (or a macro)
 *   set: conduit provides own gasnete_put_nb_val() as an inline
 *
 * GASNETI_DIRECT_PUT_NBI_VAL
 *   unset: gasnete_put_nbi_val() via gasnete_put_nbi()
 *   set: conduit provides own gasnete_put_nbi_val()
 *
 * GASNETI_DIRECT_GET_VAL
 *   unset: extern gasnete_get_val() in gasnet_extended.c (or a macro)
 *   set: conduit provides own gasnete_get_val() as an inline
 */

/* We perform these blocking ops w/o the overhead of eop alloc/free: */
//#define GASNETI_DIRECT_BLOCKING_GET 1
//#define GASNETI_DIRECT_BLOCKING_PUT 1

/* Implement all "base" operations directly via amref: */
/*#define gasnete_amref_get_nb        gasnete_get_nb
#define gasnete_amref_put_nb        gasnete_put_nb
#define gasnete_amref_get_nbi       gasnete_get_nbi
#define gasnete_amref_put_nbi       gasnete_put_nbi
*/

#endif
