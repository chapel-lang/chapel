
/* $Id: trclib.h,v 4.7 2002/04/02 20:53:50 pvmsrc Exp $ */

/*
 *         Tracer version 1.0:  A Trace File Generator for PVM
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *           Authors:  James Arthur Kohl and G. A. Geist
 *                   (C) 1994 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear
 * in supporting documentation.
 *
 * Neither the Institution, Oak Ridge National Laboratory, nor the
 * Authors make any representations about the suitability of this
 * software for any purpose.  This software is provided ``as is''
 * without express or implied warranty.
 *
 * Tracer was funded by the U.S. Department of Energy.
 */


/* System Header Files */

#include <stdio.h>
#include <sys/types.h>

#ifndef WIN32
#include <sys/time.h>
#else
#include "pvmwin.h"
#endif

#include <time.h>

#ifdef HASSTDLIB
#include <stdlib.h>
#else
#ifndef IMA_MACOSX
#include <malloc.h>
#endif
#endif


/* PVM Header File */

#include <pvm3.h>


/* PVM Trace Header File */

#ifdef USE_PVM_33

/* Backwards Compatibility Header for PVM 3.3 Usage */

#include "trccompatrev.h"

#else

#include <pvmtev.h>

#endif


/* Tracer Data Type Header */

#include "trctype.h"


/* Define Maximum Trace Message Unpacking Array Size */

#define TRC_MAX_UNPACK_ARR_SIZE    1024


/* PVM Error Check Macro */

#define TRC_NOP    (TRC_TMP_CC = TRC_TMP_CC)


#define TRC_PVMCKERR( _cmd, _str, _handle ) \
{ \
	TRC_TMP_CC = _cmd; \
\
	if ( TRC_TMP_CC < 0 ) \
	{ \
		pvm_perror( _str ); \
\
		_handle; \
	} \
} \
\


/* Trace File EOF Check Macro */

#define TRC_CKEOF( _c, _str, _handle ) \
{ \
	if ( _c == (char) EOF ) \
	{ \
		printf( _str ); \
\
		_handle; \
	} \
} \
\


/* Time Value Macros */

#define TRC_TIME_COMPARE( _t1, _t2 ) \
( \
	( (_t1).tv_sec > (_t2).tv_sec \
			|| ( (_t1).tv_sec == (_t2).tv_sec \
				&& (_t1).tv_usec > (_t2).tv_usec ) ) ? 1 : \
		( ( (_t1).tv_sec == (_t2).tv_sec \
			&& (_t1).tv_usec == (_t2).tv_usec ) ? 0 : -1 ) \
) \
\

#define TRC_TIME_ELAPSED( _t1, _t2 ) \
( \
	( ( (_t2).tv_sec - (_t1).tv_sec ) * 1000000 ) \
		+ ( (_t2).tv_usec - (_t1).tv_usec ) \
) \
\


/* Task Status Constants */

#define TRC_TASK_NOLIFE     0
#define TRC_TASK_ALIVE      1
#define TRC_TASK_DEAD       2
#define TRC_TASK_IGNORE     3
#define TRC_TASK_WAITEND    4


/* Task Output Status Constants */

#define TRC_TASK_NOOUT      0
#define TRC_TASK_OUT        1
#define TRC_TASK_EOF        2
#define TRC_TASK_WAITEOF    3


/* Task Omission Condition Macro */

#define TRC_TASK_OMIT( _tevtask ) \
	( (_tevtask)->tevstatus == TRC_TASK_WAITEND \
		|| (_tevtask)->outstatus == TRC_TASK_WAITEOF )


/* TRC_TRIE Size */

#define TRC_TRIE_SIZE 37


/* Trace Event Descriptor Index Lookup Macro */

#define TRC_TEVINDEX_OF( _eid, _entry_exit ) \
	( ( (_eid) * TRC_MAX_TEV ) + (_entry_exit) )


#define TRC_TEVEID_OF( _index ) \
	( (_index) / TRC_MAX_TEV )


/* Internal Trace Event Constants */

#define TRC_RESERVED_INTERNAL    0x0190  /* ((400+n) * 2) + 1 < 1000 */


#define TRC_HOST_ADD		(TRC_RESERVED_INTERNAL + 1)
#define TRC_HOST_DEL		(TRC_RESERVED_INTERNAL + 2)
#define TRC_HOST_SYNC		(TRC_RESERVED_INTERNAL + 3)
#define TRC_OUTPUT			(TRC_RESERVED_INTERNAL + 4)
#define TRC_MAX				(TRC_RESERVED_INTERNAL + 5)


/* Externals */

#ifndef USE_PVM_33

extern  struct Pvmtevdid    pvmtevdidlist[];

#endif


/* Routine Prototypes */

#include "trcproto.h"


/* Global Variables */

extern	struct timeval	TRC_TRACE_TIME;

extern	TRC_TEVDESC		TRC_OUTPUT_TEVDESC;
extern	TRC_TEVDESC		TRC_HOST_ADD_TEVDESC;
extern	TRC_TEVDESC		TRC_HOST_DEL_TEVDESC;
extern	TRC_TEVDESC		TRC_HOST_SYNC_TEVDESC;

extern	TRC_TEVDESC		*TRC_TEVDESC_LIST;

extern	int				TRC_TEVDESC_SIZE;

extern	TRC_TEVREC		TRC_TMP_TR;

extern	TRC_TRIE		TRC_OLD_EVENT_TRIE;

extern	TRC_TRIE		TRC_HANDLE_TRIE;

extern	TRC_TRIE		TRC_EVENT_TRIE;

extern	TRC_TRIE		TRC_DID_TRIE;

extern	TRC_DID			TRC_LOCAL_DID[TEV_DID_MAX - TEV_DID_FIRST + 1];

extern	TRC_DID			TRC_DID_LIST;

extern	TRC_ID			TRC_DEAD_ID_LIST;

extern	TRC_ID			TRC_ID_LIST;

extern	char			*TRC_TYPE_STRS[16];

extern	char			*TRC_VERSION;

extern	char			*TRC_NAME;

extern	char			*TRC_TMP_HOST;

extern	TRC_HOST		TRC_HOST_LIST;

extern	char			*TRC_HOST_NAME;

extern	char			*TRC_HOST_ALIAS;

extern	int				TRC_HOST_ADD_NOTIFY_CODE;
extern	int				TRC_HOST_DEL_NOTIFY_CODE;

extern	int				TRC_TID;

extern	int				TRC_TMP_CMP;

extern	int				TRC_TMP_CC;

