/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2010/05/13 23:32:33 $
 * $Revision: 1.22.2.2 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#include <firehose_trace.h>

#define GASNET_EXTENDED_VERSION      1.12
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#if GASNET_CONDUIT_VAPI
  #define GASNET_EXTENDED_NAME         VAPI
#elif GASNET_CONDUIT_IBV
  #define GASNET_EXTENDED_NAME         IBV
#else
  #error "Exactly one of GASNET_CONDUIT_VAPI or GASNET_CONDUIT_IBV must be defined"
#endif
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
	GASNETI_COLL_STATS(CNT,VAL,TIME)     \
	GASNETI_FIREHOSE_STATS(CNT,VAL,TIME) \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)           

#define GASNETI_DIRECT_GET_BULK 1
#define GASNETI_DIRECT_PUT_BULK 1
#define GASNETI_DIRECT_MEMSET 1
#define GASNETI_DIRECT_WAIT_SYNCNB 1
#define GASNETI_DIRECT_WAIT_SYNCNBI_GETS 1
#define GASNETI_DIRECT_WAIT_SYNCNBI_PUTS 1

#endif

