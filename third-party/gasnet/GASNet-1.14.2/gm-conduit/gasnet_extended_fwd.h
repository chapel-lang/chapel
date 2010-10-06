/*   $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2010/05/13 23:32:29 $
 * $Revision: 1.35.2.2 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#include <gm.h>
#include <firehose_trace.h>

#define GASNET_EXTENDED_VERSION      1.11
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         GM
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1

#if GASNETI_CLIENT_THREADS
#define GASNETE_GM_IN_UNKNOWN() \
        (((gasnete_threaddata_t *)GASNET_GET_THREADINFO())->in_gm_unknown)
#define GASNETE_GM_SET_IN_UNKNOWN() \
        (((gasnete_threaddata_t *)GASNET_GET_THREADINFO())->in_gm_unknown = 1)
#define GASNETE_GM_UNSET_IN_UNKNOWN() \
        (((gasnete_threaddata_t *)GASNET_GET_THREADINFO())->in_gm_unknown = 0)
#else
#define GASNETE_GM_IN_UNKNOWN()		1
#define GASNETE_GM_SET_IN_UNKNOWN()
#define GASNETE_GM_UNSET_IN_UNKNOWN()
#endif

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME) 		\
        GASNETI_VIS_STATS(CNT,VAL,TIME)                 \
        GASNETI_COLL_STATS(CNT,VAL,TIME)                \
	GASNETI_FIREHOSE_STATS(CNT,VAL,TIME)            \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)		\
	VAL(C, FIREHOSE_MOVES, firehoses moved for puts)\
	VAL(C, FIREHOSE_MOVE_OLD_BUCKETS,		\
		number of replacement firhoses)		\
	CNT(C, FIREHOSE_VICTIM_POLLS,			\
		number of firehoses recovered by poll)	\
	VAL(C, FIREHOSE_TOUCHED, 			\
		firehoses touched for puts)		\
	VAL(C, BUCKET_LOCAL_PINS,			\
		local buckets pinned for puts/gets)	\
	VAL(C, BUCKET_LOCAL_TOUCHED, 			\
		local buckets touched for puts/gets)	\
	VAL(C, BUCKET_VICTIM_UNPINS, 			\
		number of bucket unpins in victim FIFO) \
	VAL(C, BUCKET_VICTIM_COUNT, 			\
		number of buckets in victim FIFO)	\
	VAL(C, FIREHOSE_LOCALPIN_PAGES,			\
		number of pages pinned locally)		\
	VAL(C, FIREHOSE_LOCALUNPIN_PAGES,		\
		number of pages unpinned locally)	\
	TIME(C, FIREHOSE_MOVE_TIME, unpin+pin time in   \
		firehose handler)			\
	TIME(C, FIREHOSE_BUILD_LIST_TIME, time to build \
		firehose list)				\
	TIME(C, FIREHOSE_MOVE_LOCAL, local bookkeeping	\
		in firehose reply handler)		\
	TIME(C, FIREHOSE_UNPIN_TIME, unpin time in	\
		firehose handler)			\
	TIME(C, FIREHOSE_PIN_TIME, pin time in firehose \
		handler)				\
	TIME(C, FIREHOSE_PUT_ONE, puts one fh move)	\
	TIME(C, FIREHOSE_PUT_MANY, puts many fh moves)	\
	TIME(C, FIREHOSE_PUT_ONESIDED, puts one-sided)  \
	TIME(C, FIREHOSE_GET_ONE, gets one fh move)	\
	TIME(C, FIREHOSE_GET_MANY, gets many fh moves)	\
	TIME(C, FIREHOSE_GET_ONESIDED, gets one-sided)


#endif

