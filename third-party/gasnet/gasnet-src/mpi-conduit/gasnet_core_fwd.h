/*   $Source: bitbucket.org:berkeleylab/gasnet.git/mpi-conduit/gasnet_core_fwd.h $
 * Description: GASNet header for MPI conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      AMMPI_LIBRARY_VERSION
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         MPI
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_MPI       1

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
  #define GASNET_PSHM 1
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  /* mpi-conduit supports both aligned and un-aligned */
  #if defined(HAVE_MMAP) && !PLATFORM_ARCH_CRAYX1
    #define GASNET_ALIGNED_SEGMENTS   1
  #else
    #define GASNET_ALIGNED_SEGMENTS   0
  #endif
#endif

  // If this conduit is considered a "portable conduit" only *conditionally*,
  // uncomment to enable calls to gasnetc_check_portable_conduit(void) as
  // described in gasnet_internal.c.
//#define GASNETC_CHECK_PORTABLE_CONDUIT_HOOK 1

  // uncomment for each MK_CLASS which the conduit supports. leave commented otherwise
//#define GASNET_HAVE_MK_CLASS_CUDA_UVA GASNETI_MK_CLASS_CUDA_UVA_ENABLED
//#define GASNET_HAVE_MK_CLASS_HIP GASNETI_MK_CLASS_HIP_ENABLED

  /* uncomment if your conduit has "private" threads which might run conduit
     code and/or the client's AM handlers, even under GASNET_SEQ.
     this ensures locking is still done correctly, etc
   */
//#define GASNETI_CONDUIT_THREADS 1

  /* define these to 1 if your conduit needs to augment the implementation
     of gasneti_reghandler() (in gasnet_internal.c)
   */
#define GASNETC_AMREGISTER 1

  /* define this to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */

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
#define GASNETC_REQUESTV_POLLS 1

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

//#define GASNETC_SEGMENT_EXTRA_DECLS (###)
//#define GASNETC_SEGMENT_INIT_HOOK(i_segment) (###)
//#define GASNETC_SEGMENT_FINI_HOOK(i_segment) (###)
//#define GASNETC_SIZEOF_SEGMENT_T() (###)

//#define GASNETC_TM_EXTRA_DECLS (###)
//#define GASNETC_TM_INIT_HOOK(i_tm) (###)
//#define GASNETC_TM_FINI_HOOK(i_tm) (###)
//#define GASNETC_SIZEOF_TM_T() (###)

//#define GASNETC_EP_EXTRA_DECLS (###)
//#define GASNETC_EP_INIT_HOOK(i_ep) (###)
//#define GASNETC_EP_FINI_HOOK(i_ep) (###)
//#define GASNETC_SIZEOF_EP_T() (###)

  // Uncomment the following defines if conduit provides the corresponding hook.
  // See gasnet_internal.h for prototypes and brief descriptions.
//#define GASNETC_SEGMENT_ATTACH_HOOK 1
//#define GASNETC_SEGMENT_CREATE_HOOK 1
//#define GASNETC_SEGMENT_DESTROY_HOOK 1
//#define GASNETC_EP_BINDSEGMENT_HOOK 1
//#define GASNETC_EP_PUBLISHBOUNDSEGMENT_HOOK 1

  // Uncomment the following defines if conduit provides the corresponding hook.
  // See other/kinds/gasnet_kinds_internal.h for prototypes and brief descriptions.
//#define GASNETC_MK_CREATE_HOOK 1
//#define GASNETC_MK_DESTROY_HOOK 1

/* mpi-conduit supports top-level poll throttling */
#define GASNETC_USING_SUSPEND_RESUME 1

  /* enable usage correctness checking on HSL's and no-interrupt sections */
#ifndef GASNETC_HSL_ERRCHECK
  #if GASNET_DEBUG
    #define GASNETC_HSL_ERRCHECK 1
  #else
    #define GASNETC_HSL_ERRCHECK 0
  #endif
#endif

/*  override default error values to use those defined by AMMPI */
#define _GASNET_ERRORS
#define _GASNET_ERR_BASE 10000
#define GASNET_ERR_NOT_INIT             1
#define GASNET_ERR_RESOURCE             3
#define GASNET_ERR_BAD_ARG              2
#define GASNET_ERR_NOT_READY            (_GASNET_ERR_BASE+4)
#define GASNET_ERR_BARRIER_MISMATCH     (_GASNET_ERR_BASE+5)

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

#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)
extern void gasnetc_fatalsignal_callback(int _sig);

/* hook getSegmentInfo for NIS check */
#define _GASNET_GETSEGMENTINFO
struct gasneti_seginfo_s;
extern int gasnetc_getSegmentInfo(struct gasneti_seginfo_s *_seginfo_table, int _numentries);
#define gasnet_getSegmentInfo(seginfo_table, numentries) \
        gasnetc_getSegmentInfo(seginfo_table, numentries)

// No validated support for hugetlbfs w/ or w/o PSHM at this time and risk of
// issues if auto-enabled on an HPE Cray EX system (e.g. bug 4473).
#if GASNETI_ARCH_CRAYEX
#undef GASNETI_USE_HUGETLBFS
#endif

#endif
