
static char rcsid[] =
	"$Id: pvmdtev.c,v 1.7 2001/02/07 23:15:53 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */


/*
 *  pvmdtev.c
 *
 *  PVMD Tracing routines.
 *
 */


/*
 *  Trace Descriptors Generated Manually (for simplicity)
 *
 */


#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <pvmproto.h>
#include "pmsg.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"


#define CHECKLISTSIZE 100


static int init_checklist[CHECKLISTSIZE];
static int init_start = 0;
static int init_end = 0;


void
tev_init_pvmd( dst, ctx, tag )
	int dst;
	int ctx;
	int tag;
{
	struct Pvmtevdid *ptr;

	struct pmsg *mp;

	int i;

	/* Check for Existing Tracer */

	for ( i=init_start ; i != init_end ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( init_checklist[i] == dst )
			return;
	}

	init_checklist[init_end] = dst;

	init_end = ( init_end + 1 ) % CHECKLISTSIZE;

	init_start = ( init_end == init_start ) ?
		( init_start + 1 ) % CHECKLISTSIZE : init_start;

	/* Send Semantic Identifiers to Trace Collector */

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	pkint( mp, TEV_MARK_DATA_ID );

	for ( i=0 ; i <= TEV_DID_MAX ; i++ )
	{
		ptr = &(pvmtevdidlist[i]);

		/* Pack Semantic ID, Name & Description */

		pkint( mp, i );

		pkstr( mp, ptr->did );

		pkstr( mp, ptr->desc );
	}

	pkint( mp, TEV_MARK_DATA_ID_END );

	sendmessage( mp );
}


void
tev_send_newtask( dst, ctx, tag, tid, ptid, flags, name )
	int dst;
	int ctx;
	int tag;
	int tid;
	int ptid;
	int flags;
	char *name;
{
	static int checklist[CHECKLISTSIZE];
	static int start = 0;
	static int end = 0;

	struct timeval now;

	struct pmsg *mp;

	int flag;
	int tmp;
	int i;

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	gettimeofday( &now, (struct timezone *) 0 );

	/* Check for Init */

	tev_init_pvmd( dst, ctx, tag );

	/* Check for Descriptor */

	flag = 0;

	for ( i=start ; i != end && !flag ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( checklist[i] == dst )
			flag++;
	}

	/* Pack NEWTASK Event Message (with descriptor if necessary) */

	/* Header */

	if ( !flag )
	{
		pkint( mp, TEV_MARK_EVENT_DESC );

		pkint( mp, TEV_NEWTASK );

		pkstr( mp, pvmtevinfo[ TEV_NEWTASK ].name );

		checklist[end] = dst;

		end = ( end + 1 ) % CHECKLISTSIZE;

		start = ( end == start ) ?
			( start + 1 ) % CHECKLISTSIZE : start;
	}

	else
	{
		pkint( mp, TEV_MARK_EVENT_RECORD );

		pkint( mp, TEV_NEWTASK );
	}

	/* Time Stamp Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_sec );

	/* Time Stamp Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_usec );

	/* Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TID );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, tid );

	/* Parent Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_PT );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, ptid );

	/* Task Flags */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TF );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, flags );

	/* Task Name */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TN );
		pkint( mp, TEV_DATA_STRING | TEV_DATA_SCALAR );
	}

	pkstr( mp, name );

	/* End Marker */

	if ( !flag )
		pkint( mp, TEV_MARK_EVENT_DESC_END );
	
	else
		pkint( mp, TEV_MARK_EVENT_RECORD_END );

	sendmessage( mp );
}


void
tev_send_spntask( dst, ctx, tag, tid, ptid )
	int dst;
	int ctx;
	int tag;
	int tid;
	int ptid;
{
	static int checklist[CHECKLISTSIZE];
	static int start = 0;
	static int end = 0;

	struct timeval now;

	struct pmsg *mp;

	int flag;
	int tmp;
	int i;

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	gettimeofday( &now, (struct timezone *) 0 );

	/* Check for Init */

	tev_init_pvmd( dst, ctx, tag );

	/* Check for Descriptor */

	flag = 0;

	for ( i=start ; i != end && !flag ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( checklist[i] == dst )
			flag++;
	}

	/* Pack SPNTASK Event Message (with descriptor if necessary) */

	/* Header */

	if ( !flag )
	{
		pkint( mp, TEV_MARK_EVENT_DESC );

		pkint( mp, TEV_SPNTASK );

		pkstr( mp, pvmtevinfo[ TEV_SPNTASK ].name );

		checklist[end] = dst;

		end = ( end + 1 ) % CHECKLISTSIZE;

		start = ( end == start ) ?
			( start + 1 ) % CHECKLISTSIZE : start;
	}

	else
	{
		pkint( mp, TEV_MARK_EVENT_RECORD );

		pkint( mp, TEV_SPNTASK );
	}

	/* Time Stamp Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_sec );

	/* Time Stamp Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_usec );

	/* Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TID );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, tid );

	/* Parent Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_PT );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, ptid );

	/* End Marker */

	if ( !flag )
		pkint( mp, TEV_MARK_EVENT_DESC_END );

	else
		pkint( mp, TEV_MARK_EVENT_RECORD_END );

	sendmessage( mp );
}


void
tev_send_endtask( dst, ctx, tag, tid, status, usec, uusec, ssec, susec )
	int dst;
	int ctx;
	int tag;
	int tid;
	int status;
	long usec;
	long uusec;
	long ssec;
	long susec;
{
	static int checklist[CHECKLISTSIZE];
	static int start = 0;
	static int end = 0;

	struct timeval now;

	struct pmsg *mp;

	int flag;
	int tmp;
	int i;

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	gettimeofday( &now, (struct timezone *) 0 );

	/* Check for Init */

	tev_init_pvmd( dst, ctx, tag );

	/* Check for Descriptor */

	flag = 0;

	for ( i=start ; i != end && !flag ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( checklist[i] == dst )
			flag++;
	}

	/* Pack ENDTASK Event Message (with descriptor if necessary) */

	/* Header */

	if ( !flag )
	{
		pkint( mp, TEV_MARK_EVENT_DESC );

		pkint( mp, TEV_ENDTASK );

		pkstr( mp, pvmtevinfo[ TEV_ENDTASK ].name );

		checklist[end] = dst;

		end = ( end + 1 ) % CHECKLISTSIZE;

		start = ( end == start ) ?
			( start + 1 ) % CHECKLISTSIZE : start;
	}

	else
	{
		pkint( mp, TEV_MARK_EVENT_RECORD );

		pkint( mp, TEV_ENDTASK );
	}

	/* Time Stamp Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_sec );

	/* Time Stamp Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_usec );

	/* Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TID );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, tid );

	/* Task Status */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TST );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, status );

	/* User Time Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TUS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) usec );

	/* User Time Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TUU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) uusec );

	/* System Time Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TSS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) ssec );

	/* System Time Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TSU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) susec );

	/* End Marker */

	if ( !flag )
		pkint( mp, TEV_MARK_EVENT_DESC_END );

	else
		pkint( mp, TEV_MARK_EVENT_RECORD_END );

	sendmessage( mp );
}

