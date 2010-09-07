/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2009/10/28 00:39:41 $
 * $Revision: 1.10 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.3
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         PORTALS
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

/* For Portals conduit, can only have 128 threads per process since we use the
 * most significant bit of the uint8_t threadid field to distinguish whether
 * the handle it is imbedded in is an eop or an iop.  This allows us to represent
 * a reference to the handle in 24 bits when hidden in the match bits of a
 * portals opertaion
 */
#define GASNETI_MAX_THREADS 128

#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#if PLATFORM_OS_CATAMOUNT
 #define GASNETI_FIREHOSE_STATS(CNT,VAL,TIME) /*empty*/
#else
 #include <firehose_trace.h>
#endif
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        GASNETI_FIREHOSE_STATS(CNT,VAL,TIME) \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    \
	CNT(C, EOP_ALLOC, count)             \
	CNT(C, EOP_FREE, count)              \
	CNT(C, EOP_BUCKETS, cnt)

/* Define an extended API exit function to cleanup Portals
 * resources at exit time.
 * This is called from the mpi-conduit gasnetc_exit() routine
 */
extern void gasnete_exit(int exitcode);
#define GASNETE_EXIT_CALLBACK(exitcode) gasnete_exit(exitcode)

/* Hijack the gasnet core trace_finish function if it has not already been
 * defined.  (And I know it has not for the mpi_conduit)
 * Better to have one for the extended API as well...
 */
extern void gasnete_trace_finish(void);
#define GASNETC_TRACE_FINISH() gasnete_trace_finish()

#endif
