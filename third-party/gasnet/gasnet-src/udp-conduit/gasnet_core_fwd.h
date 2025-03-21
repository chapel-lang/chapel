/*   $Source: bitbucket.org:berkeleylab/gasnet.git/udp-conduit/gasnet_core_fwd.h $
 * Description: GASNet header for UDP conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      AMUDP_LIBRARY_VERSION
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         UDP
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_UDP       1

//#define GASNETC_DEFAULT_SPAWNER  ###

  /* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
  #define GASNET_PSHM 1
#endif

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  /* udp-conduit supports both aligned and un-aligned */
  #if defined(HAVE_MMAP) && !PLATFORM_ARCH_CRAYX1
    #define GASNET_ALIGNED_SEGMENTS   1
  #else
    #define GASNET_ALIGNED_SEGMENTS   0
  #endif
#endif

#define GASNET_MAXNODES AMUDP_MAX_NUMTRANSLATIONS

  // If this conduit is considered a "portable conduit" only *conditionally*,
  // uncomment to enable calls to gasnetc_check_portable_conduit(void) as
  // described in gasnet_internal.c.
//#define GASNETC_CHECK_PORTABLE_CONDUIT_HOOK 1

  // uncomment for each MK_CLASS which the conduit supports. leave commented otherwise
//#define GASNET_HAVE_MK_CLASS_CUDA_UVA GASNETI_MK_CLASS_CUDA_UVA_ENABLED
//#define GASNET_HAVE_MK_CLASS_HIP GASNETI_MK_CLASS_HIP_ENABLED
//#define GASNET_HAVE_MK_CLASS_ZE GASNETI_MK_CLASS_ZE_ENABLED

  // define to 1 if your conduit has "private" thread(s) which can run AM handlers
//#define GASNET_RCV_THREAD 1

  // define to 1 if your conduit has "private" thread(s) which progress sends of RMA and/or AM
//#define GASNET_SND_THREAD 1

  /* uncomment if your conduit has "private" threads which might run conduit
     code and/or the client's AM handlers, even under GASNET_SEQ.
     this ensures locking is still done correctly, etc
   */
//#define GASNETI_CONDUIT_THREADS 1

/* udp-conduit supports top-level poll throttling */
#define GASNETC_USING_SUSPEND_RESUME 1

/*  override default error values to use those defined by AMUDP */
#define _GASNET_ERRORS
#define _GASNET_ERR_BASE 10000
#define GASNET_ERR_NOT_INIT             1
#define GASNET_ERR_RESOURCE             3
#define GASNET_ERR_BAD_ARG              2
#define GASNET_ERR_NOT_READY            (_GASNET_ERR_BASE+4)
#define GASNET_ERR_BARRIER_MISMATCH     (_GASNET_ERR_BASE+5)

  /* define these to 1 if your conduit needs to augment the implementation
     of gasneti_reghandler() (in gasnet_internal.c)
   */
#define GASNETC_AMREGISTER 1

  /* define this to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */

  /* uncomment each line for which your conduit supports the
     corresponding token info query.
  */
#define GASNET_SUPPORTS_TI_SRCRANK 1
#define GASNET_SUPPORTS_TI_EP 1
#define GASNET_SUPPORTS_TI_ENTRY 1
#define GASNET_SUPPORTS_TI_IS_REQ 1
#define GASNET_SUPPORTS_TI_IS_LONG 1

  /* uncomment for each {Request,Reply} X {Medium,Long} pair for which your
     conduit implements the corresponding gasnetc_AM_{Prepare,Commit}*() in
     a "native" manner which "can avoid one or more payload copies relative
     to the corresponding fixed-payload AM call under the right conditions".
     See also "GASNETC_BUILD_NP_*", immediately below.
   */
/* #define GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REP_MEDIUM 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REQ_LONG 1 */
/* #define GASNET_NATIVE_NP_ALLOC_REP_LONG 1 */

  /* conduits may define to '1' (or '0') for {Request,Reply} X {Medium,Long}
     pairs to force (or prevent) compilation of the corresponding pieces of
     the conduit-independent reference implementation.
     If unset, the default is equivalent to '!GASNET_NATIVE_NP_ALLOC_[foo]'.
     In other words: by default each reference implementation is built if and
     only if the conduit is not claiming a "native" implementation.
     This default is correct for most conduits.

     The conduit-independent implementation works in terms of the internal
     functions gasnetc_AM{Request,Reply}{Medium,Long}V().  Therefore, your
     conduit must provide the V-suffixed functions for any case with the
     corresponding GASNETC_BUILD_NP_* equal to '1' (explicitly or by default).
   */
/* #define GASNETC_BUILD_NP_REQ_MEDIUM (###) */
/* #define GASNETC_BUILD_NP_REP_MEDIUM (###) */
/* #define GASNETC_BUILD_NP_REQ_LONG (###) */
/* #define GASNETC_BUILD_NP_REP_LONG (###) */

  /* uncomment for each conduit-provided Commit{Req,Rep}{Medium,Long}() which
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
     Used only by reference implementations (if any) of Prepare/Commit.
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

extern void gasnetc_stats_dump(int _reset);
#define GASNETC_STATS_DUMP(reset) gasnetc_stats_dump(reset)

#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)
extern void gasnetc_fatalsignal_callback(int _sig);

extern void _gasnetc_set_waitmode(int _wait_mode);
#define gasnetc_set_waitmode(wait_mode) _gasnetc_set_waitmode(wait_mode)

/* udp-conduit's default spawner produces random node placements.
   could in theory leave at 0 when SPAWNFN='L' */
#define GASNETC_DEFAULT_NODEMAP_EXACT 1

/* Configure gasnet_handle.[ch] */

// No validated support for hugetlbfs w/ or w/o PSHM at this time and risk of
// issues if auto-enabled on an HPE Cray EX system (e.g. bug 4473).
#if GASNETI_ARCH_CRAYEX
#undef GASNETI_USE_HUGETLBFS
#endif

#endif
