/*   $Source: /var/local/cvs/gasnet/mpi-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2009/09/18 23:33:34 $
 * $Revision: 1.29 $
 * Description: GASNet header for MPI conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
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

/* conduit allows internal GASNet fns to issue put/get for remote addrs out of segment */
/* XXX: This is currently true even for the GASNET_PSHM==1 case, but only because there
 * is no shared-memory bypass of the put/get code */
#define GASNETI_SUPPORTS_OUTOFSEGMENT_PUTGET 1

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

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME) 

#define GASNETC_FATALSIGNAL_CALLBACK(sig) gasnetc_fatalsignal_callback(sig)
extern void gasnetc_fatalsignal_callback(int sig);

/* hook getSegmentInfo for NIS check */
#define _GASNET_GETSEGMENTINFO
struct gasneti_seginfo_s;
GASNETI_EXTERNC int gasnetc_getSegmentInfo(struct gasneti_seginfo_s *seginfo_table, int numentries);
#define gasnet_getSegmentInfo(seginfo_table, numentries) \
        gasnetc_getSegmentInfo(seginfo_table, numentries)

#endif
