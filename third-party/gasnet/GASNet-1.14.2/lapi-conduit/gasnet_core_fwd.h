/*   $Source: /var/local/cvs/gasnet/lapi-conduit/gasnet_core_fwd.h,v $
 *     $Date: 2009/10/28 00:09:13 $
 * $Revision: 1.33 $
 * Description: GASNet header for lapi conduit core (forward definitions)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.10
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         LAPI
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_LAPI      1


#ifdef GASNETC_LAPI_FEDERATION
  #define GASNETC_LAPI_TYPE_STR "Federation"
#elif GASNETC_LAPI_COLONY
  #define GASNETC_LAPI_TYPE_STR "Colony"
#else
  #define GASNETC_LAPI_TYPE_STR "UNKNOWN"
#endif

#ifdef GASNETC_LAPI_RDMA
  #define GASNETC_LAPI_RDMA_STR "yes"
#else
  #define GASNETC_LAPI_RDMA_STR "no"
#endif

#define GASNETC_EXTRA_CONFIG_INFO ",lapi_type=" GASNETC_LAPI_TYPE_STR ",lapi_rdma=" GASNETC_LAPI_RDMA_STR

#ifdef GASNETC_LAPI_FEDERATION
   /* Check for broken version of LAPI on early Federation HW.
    * reference bug 717.  Was fixed in version 2.3.2.0
    */
#  ifndef GASNETC_LAPI_VERSION_A
#    define GASNETC_LAPI_FED_POLLBUG_PRESENT 1
#  elif GASNETC_LAPI_VERSION_A <= 2
#    if GASNETC_LAPI_VERSION_A < 2
#      define GASNETC_LAPI_FED_POLLBUG_PRESENT 1
#    elif GASNETC_LAPI_VERSION_B <= 3
#      if GASNETC_LAPI_VERSION_B < 3
#        define GASNETC_LAPI_FED_POLLBUG_PRESENT 1
#      elif GASNETC_LAPI_VERSION_C < 2
#        define GASNETC_LAPI_FED_POLLBUG_PRESENT 1
#      endif
#    endif
#  endif
#endif
#if GASNETC_LAPI_FED_POLLBUG_PRESENT
  #error "LAPI Federation versions prior to 2.3.2.0 are not supported"
#endif
#undef GASNETC_LAPI_FED_POLLBUG_PRESENT

/* defined to be 1 if gasnet_init guarantees that the remote-access
 * memory segment will be aligned at the same virtual address on all
 * nodes. defined to 0 otherwise.
 *
 * We should be able to guarantee aligned segments on SP.  Segmented
 * memory guarantees heap and mmaped regions don't overlap.
 */
#if GASNETC_LAPI_RDMA /* bug 2176 - must use large-page malloc (and unaligned segments) for LAPI-RDMA */
   #define GASNET_ALIGNED_SEGMENTS   0 
   #undef HAVE_MMAP
#elif GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1
#endif


  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
/* lapi-conduit always has the LAPI completion and notification threads */
#define GASNETI_CONDUIT_THREADS 1

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

/* lapi-conduit does not guarantee 8-byte alignment for medium buffers,
   and PowerPC does not seem to ever require it */
#define GASNETI_MEDBUF_ALIGNMENT 4

#endif
