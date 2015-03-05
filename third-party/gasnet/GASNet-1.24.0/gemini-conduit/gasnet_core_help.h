/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gemini-conduit/gasnet_core_help.h $
 * Description: GASNet gemini conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

GASNETI_BEGIN_EXTERNC

#if defined(GASNET_PAR) && GASNETC_GNI_MULTI_DOMAIN
  /* Poll without progress functions */
  #define gasnetc_AMPoll() gasnetc_AMPoll_core(GASNETE_THREAD_GET_ALONE)

  /* GASNet internal code calls gasneti_AMPoll(), which does not trace */
  #define gasneti_AMPoll() gasnetc_AMPoll_internal(GASNETE_THREAD_GET_ALONE)
  #define GASNETI_GASNETI_AMPOLL

  /* GASNet client calls gasnet_AMPoll(), which traces */
  #define gasnet_AMPoll() gasnetc_AMPoll_client(GASNETE_THREAD_GET_ALONE)
  #define _GASNET_AMPOLL

  /* No spin pollers */
  #define gasneti_suspend_spinpollers() ((void)0)
  #define gasneti_resume_spinpollers()  ((void)0)
#endif

#include <gasnet_help.h>

#if defined(GASNET_PAR) && GASNETC_GNI_MULTI_DOMAIN 
  /* Too early to use GASNETE_THREAD_*, though GASNETI_THREADINFO_OPT is defined when applicable */
  #ifdef GASNETI_THREADINFO_OPT
    #define GASNETC_AM_POLL_FARG void *_threadinfo
    #define GASNETC_AM_POLL_PASS _threadinfo
  #else
    #define GASNETC_AM_POLL_FARG void
    #define GASNETC_AM_POLL_PASS /*empty*/
  #endif

  extern int gasnetc_AMPoll_core(GASNETC_AM_POLL_FARG);

  GASNETI_INLINE(gasnetc_AMPoll_internal)
  int gasnetc_AMPoll_internal(GASNETC_AM_POLL_FARG) {
    int retval;
    gasneti_memcheck_one();
    retval = gasnetc_AMPoll_core(GASNETC_AM_POLL_PASS);
    GASNETI_PROGRESSFNS_RUN();
    return retval;
  }

  GASNETI_INLINE(gasnetc_AMPoll_client)
  int gasnetc_AMPoll_client(GASNETC_AM_POLL_FARG) {
    GASNETI_TRACE_EVENT(I, AMPOLL);
    return gasnetc_AMPoll_internal(GASNETC_AM_POLL_PASS);
  }
#endif

GASNETI_END_EXTERNC

#endif
