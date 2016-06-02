/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      GASNET_RELEASE_VERSION_MAJOR.GASNET_RELEASE_VERSION_MINOR
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         REFERENCE
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

/*
 * When implementing a conduit-specific implementation of the Extended API, one
 * can #define the following to 1 to change certain behaviors in gasnet_extended.h.
 * Alternatively, one can #define GASNETE_HAVE_EXTENDED_HELP_EXTRA_H and defined
 * these in a conduit-specific gasnet_extended_help_extra.h.
 *
 * GASNETI_DIRECT_GET_NB
 *   unset: gasnete_get_nb() maps to gasnete_get_nb_bulk()
 *   set: conduit provides it own gasnete_get_nb()
 *
 * GASNETI_DIRECT_GET_NBI
 *   unset: gasnete_get_nbi() maps to gasnete_get_nbi_bulk()
 *   set: conduit provides it own gasnete_get_nbi()
 *
 * GASNETI_DIRECT_WAIT_SYNCNB 
 *   unset: gasnete_wait_syncnb(h) via gasneti_pollwhile(gasnete_try_syncnb(h))
 *   set: conduit provides it own gasnete_wait_syncnb()
 *
 * GASNETI_DIRECT_WAIT_SYNCNB_SOME
 *   unset: gasnete_wait_syncnb_some(...) via gasneti_pollwhile(gasnete_try_syncnb_some(...))
 *   set: conduit provides it own gasnete_wait_syncnb_some()
 *
 * GASNETI_DIRECT_WAIT_SYNCNB_ALL
 *   unset: gasnete_wait_syncnb_all(...) via gasneti_pollwhile(gasnete_try_syncnb_all(...))
 *   set: conduit provides it own gasnete_wait_syncnb_all()
 *
 * GASNETI_DIRECT_TRY_SYNCNBI_ALL
 *   unset: gasnete_try_syncnbi_all() via calls to gasnete_try_syncnbi_{gets,puts}()
 *   set: conduit provides it own gasnete_try_syncnbi_all()
 *
 * GASNETI_DIRECT_WAIT_SYNCNBI_GETS
 *   unset: gasneti_wait_syncnbi_gets() via gasneti_pollwhile(gasnete_try_syncnbi_gets())
 *   set: conduit provides it own gasneti_wait_syncnbi_gets()
 *
 * GASNETI_DIRECT_WAIT_SYNCNBI_PUTS
 *   unset: gasneti_wait_syncnbi_puts() via gasneti_pollwhile(gasnete_try_syncnbi_puts())
 *   set: conduit provides it own gasneti_wait_syncnbi_puts()
 *
 * GASNETI_DIRECT_WAIT_SYNCNBI_ALL
 *   unset: gasnete_wait_syncnbi_all() via gasneti_pollwhile(gasnete_try_syncnbi_{gets,puts}())
 *   set: conduit provides it own gasneti_wait_syncnbi_all()
 *
 * GASNETI_DIRECT_GET
 *   unset: gasnete_get() maps to gasnete_get_bulk()
 *   set: conduit provides it own gasnete_get()
 *
 * GASNETI_DIRECT_PUT
 *   unset: gasnete_put() maps to gasnete_put_bulk()
 *   set: conduit provides it own gasnete_put()
 *
 * GASNETI_DIRECT_PUT_BULK
 *   unset: gasnete_put_bulk() via gasnete_wait_syncnb(gasnete_put_nb_bulk())
 *   set: conduit provides it own gasnete_put_bulk()
 *
 * GASNETI_DIRECT_MEMSET
 *   unset: gasnete_memset() via gasnete_wait_syncnb(gasnete_memset_nb())
 *   set: conduit provides it own gasnete_memset()
 *
 * GASNETI_DIRECT_PUT_VAL
 *   unset: gasnete_put_val() via gasnete_putTI()
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

/* Configure use of AM-based implementation of get/put/memset */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */
#define GASNETE_USING_REF_EXTENDED_GET_BULK 1
#define GASNETE_USING_REF_EXTENDED_PUT_BULK 1
#define GASNETE_USING_REF_EXTENDED_PUT      1
#define GASNETE_USING_REF_EXTENDED_MEMSET   1

/* These are the default tuning parameters for the AM-based get/put.
 * Conduits cloning this file may want to override these: */
#if 0
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gasnet_AMMaxMedium()
#define GASNETE_USE_LONG_GETS 1
#endif

/* Implement all eight "base" operations directly via amref: */
#define gasnete_amref_get_nb_bulk   gasnete_get_nb_bulk
#define gasnete_amref_get_nbi_bulk  gasnete_get_nbi_bulk
#define gasnete_amref_put_nb_bulk   gasnete_put_nb_bulk
#define gasnete_amref_put_nbi_bulk  gasnete_put_nbi_bulk
#define gasnete_amref_put_nb        gasnete_put_nb
#define gasnete_amref_put_nbi       gasnete_put_nbi
#define gasnete_amref_memset_nb     gasnete_memset_nb
#define gasnete_amref_memset_nbi    gasnete_memset_nbi

#endif

