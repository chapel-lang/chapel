/*   $Source: /var/local/cvs/gasnet/elan-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2010/05/13 23:32:27 $
 * $Revision: 1.27.2.2 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.9
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         ELAN
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

/* ------------------------------------------------------------------------------------ */
/*
  Extended API Tuning Parameters
  ==============================
*/
#define GASNETE_MAX_COPYBUFFER_SZ  1048576    /* largest temp buffer we'll allocate for put/get */

#ifndef GASNETE_DEFAULT_NBI_THROTTLE
  #define GASNETE_DEFAULT_NBI_THROTTLE 1024
#endif

/* the size threshold where gets/puts stop using medium messages and start using longs */
#ifndef GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD   gasnet_AMMaxMedium()
#endif

/* true if we should try to use Long replies in gets (only possible if dest falls in segment) */
#ifndef GASNETE_USE_LONG_GETS
#define GASNETE_USE_LONG_GETS 1
#endif

/* true if we should use elan put/get (setting to zero means all put/gets use AM only) */
#ifndef GASNETE_USE_ELAN_PUTGET
#define GASNETE_USE_ELAN_PUTGET 1
#endif

/* predicate is true if we "bend" the rules of barrier to improve performance
   (may deadlock if threads disagree on named/anon barrier flags) */
extern int gasnete_elanbarrier_fast;
#define GASNETE_USING_ELANFAST_BARRIER() ((int)gasnete_elanbarrier_fast)

/* Ratio of elan pollfn callbacks to true AMPolls while barrier blocking
   must be power of two : BEWARE - raising this value hurts attentiveness at barriers
*/
#ifndef GASNETE_BARRIERBLOCKING_POLLFREQ
#if GASNETC_ELAN3
  #define GASNETE_BARRIERBLOCKING_POLLFREQ 1
#else
  #define GASNETE_BARRIERBLOCKING_POLLFREQ 1
#endif
#endif

#define GASNETE_COLL_CONDUIT_BARRIERS GASNETE_COLL_BARRIER_ELANFAST, GASNETE_COLL_BARRIER_ELANSLOW

/* ------------------------------------------------------------------------------------ */

#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    \
        CNT(C, POLL_CALLBACK_BARRIER, cnt)   \
        CNT(C, POLL_CALLBACK_NOOP, cnt)      \
        CNT(C, EXHAUSTED_ELAN_MEMORY, cnt)

#endif

