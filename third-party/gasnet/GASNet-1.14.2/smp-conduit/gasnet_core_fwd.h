/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2010/04/16 23:22:53 $
 * $Revision: 1.16.2.1 $
 * Description: GASNet header for smp conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.8
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         SMP
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)

#define GASNET_CONDUIT_SMP       1

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
/* segment alignment on smp is moot, as there is only a single node */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1
#endif

#if !defined(GASNETE_PUTGET_ALWAYSREMOTE) && !defined(GASNETE_PUTGET_ALWAYSLOCAL) && !GASNET_PSHM
  #define GASNETE_PUTGET_ALWAYSLOCAL 1
#endif

#if GASNETI_THROTTLE_FEATURE_ENABLED
/* polling is a no-op on smp-conduit, so never throttle it */ 
#undef GASNETI_THROTTLE_FEATURE_ENABLED
#endif

#define GASNETI_GASNETC_AMPOLL
#if GASNET_PSHM
  GASNETI_EXTERNC int gasnetc_AMPoll(void);
#else
  #define gasnetc_AMPoll()        GASNET_OK  /* nothing to do */
#endif

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
/* #define GASNETI_CONDUIT_THREADS 1 */

  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
/* #define GASNETC_USE_INTERRUPTS 1 */

  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
/* #define GASNETC_GET_HANDLER 1 */
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME) 

#endif
