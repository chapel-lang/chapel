/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_fwd.h $
 * Description: GASNet header for ucx conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      0.2
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME        UCX
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_UCX 1

#define GASNETC_MAX_MED             4096
#define GASNETC_MAX_MEDIUM_NBRHD    GASNETC_MAX_MED

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
#define GASNET_PSHM 1
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1//###
#endif

  /* define to 1 if conduit allows internal GASNet fns to issue put/get for remote
     addrs out of segment - not true when PSHM is used */
#if !GASNET_PSHM && 0
#define GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET 1
#endif


  // uncomment for each MK_CLASS which the conduit supports. leave commented otherwise
//#define GASNET_HAVE_MK_CLASS_CUDA_UVA GASNETI_MK_CLASS_CUDA_UVA_ENABLED

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */

#if 0
#define GASNETI_CONDUIT_THREADS 1
#endif

  /* define these to 1 if your conduit needs to augment the implementation
     of gasneti_reghandler() (in gasnet_internal.c)
   */

#if 0
#define GASNETC_AMREGISTER 1
#endif

  /* define this to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */

#if 0
#define GASNETC_GET_HANDLER 1
#endif

  /* uncomment for each {Request,Reply} X {Medium,Long} pair for which your
     conduit implements the corresponding gasnetc_AM_{Prepare,Commit}*().
     If unset, a conduit-independent implementation in terms of the internal
     functions gasnetc_AM{Request,Reply}{Medium,Long}V() will be used, and
     your conduit must provide the V-suffixed functions for any of these that
     are not defined.
   */
/* #define GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REP_MEDIUM 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REQ_LONG 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REP_LONG 1 */

  /* uncomment for each GASNET_NATIVE_NP_ALLOC_* enabled above if the Commit function
     has the numargs argument even in an NDEBUG build (it is always passed in
     DEBUG builds).
   */
//#define GASNETC_AM_COMMIT_REQ_MEDIUM_NARGS 1
//#define GASNETC_AM_COMMIT_REP_MEDIUM_NARGS 1
//#define GASNETC_AM_COMMIT_REQ_LONG_NARGS 1
//#define GASNETC_AM_COMMIT_REP_LONG_NARGS 1

  /* uncomment if your conduit's gasnetc_AMRequest{Short,Medium,Long}V()
     include a call to gasneti_AMPoll (or equivalent) for progress.
     The preferred implementation is to Poll only in the M-suffixed calls
     and not the V-suffixed calls (and GASNETC_REQUESTV_POLLS undefined).
     Used if (and only if) any of the GASNET_NATIVE_NP_ALLOC_* values above are unset.
   */
/* #define GASNETC_REQUESTV_POLLS 1 */

  // uncomment if conduit provides a gasnetc-prefixed override
  // TODO: this should be a hook rather than an override
#define GASNETC_HAVE_EP_PUBLISHBOUNDSEGMENT 1

  /* If your conduit uses conduit-specific extensions to the basic object
     types, then define the corresponding SIZEOF macros below to return
     the total length of the conduit-specific object, including the prefix
     portion which must be the matching GASNETI_[OBJECT]_COMMON fields.
     Similarly, *_HOOK macros should be defined as callbacks to perform
     conduit-specific initialization and finalization tasks, if any.
     If a given SIZEOF macro is defined, but the corresponding INIT_HOOK is
     not, then space beyond the COMMON fields will be zero-initialized.
     In all cases, GASNETC_[OBJECT]_EXTRA_DECLS provides the place to
     provide necessary declarations (since this file is included very early).
    */

//#define GASNETC_CLIENT_EXTRA_DECLS (###)
//#define GASNETC_CLIENT_INIT_HOOK(i_client) (###)
//#define GASNETC_CLIENT_FINI_HOOK(i_client) (###)
//#define GASNETC_SIZEOF_CLIENT_T() (###)

#define GASNETC_SEGMENT_EXTRA_DECLS \
  extern size_t gasnetc_sizeof_segment_t(void);
//#define GASNETC_SEGMENT_INIT_HOOK(i_segment) (###)
//#define GASNETC_SEGMENT_FINI_HOOK(i_segment) (###)
#define GASNETC_SIZEOF_SEGMENT_T() \
  gasnetc_sizeof_segment_t()

//#define GASNETC_TM_EXTRA_DECLS (###)
//#define GASNETC_TM_INIT_HOOK(i_tm) (###)
//#define GASNETC_TM_FINI_HOOK(i_tm) (###)
//#define GASNETC_SIZEOF_TM_T() (###)

//#define GASNETC_EP_EXTRA_DECLS (###)
//#define GASNETC_EP_INIT_HOOK(i_ep) (###)
//#define GASNETC_EP_FINI_HOOK(i_ep) (###)
//#define GASNETC_SIZEOF_EP_T() (###)

// If conduit supports GASNET_MAXEPS!=1, set default and (optional) max values here.
// Leaving GASNETC_MAXEPS_DFLT unset will result in GASNET_MAXEPS=1, independent
// of all other settings (appropriate for conduits without multi-ep support).
// If set, GASNETC_MAXEPS_MAX it is used to limit a user's --with-maxeps (and a
// global default limit is used otherwise).
//#define GASNETC_MAXEPS_DFLT ### // default num endpoints this conduit supports, undef means no multi-ep support
//#define GASNETC_MAXEPS_MAX ### // leave unset for default

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME) 

#endif
