
static char rcsid[] =
	"$Id: trc.c,v 1.7 2000/02/15 17:06:20 pvmsrc Exp $";

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
 * trc.c
 *
 * Tracer Library Interface Routines
 *
 */


/* Tracer Library External Header */

#include <stdio.h>
#ifndef WIN32
#include <sys/time.h>
#else
#include <sys/timeb.h>
#include <time.h>
#endif

#include <pvm3.h>
#include <pvmtev.h>

#include "job.h"

extern	int			mytid;		/* from cons.c */
extern	int			joboffset;	/* from cons.c */

extern	struct job	*joblist;	/* from cons.c */


/* Set Up Tracer Library */

int
trc_init()
{
	/* Initialize Tracer */

	trc_tracer_init();

	/* Set Tracer Globals */

	TRC_HOST_ADD_NOTIFY_CODE = TrcHostAddTag;
	TRC_HOST_DEL_NOTIFY_CODE = TrcHostDelTag;

	TRC_VERSION = trc_copy_str( pvm_version() );

	TRC_NAME = "Console";

	TRC_TID = mytid;

	/* Check Hosts */

	trc_initialize_hosts( (TRC_ID) NULL );

	return( 0 );
}


struct job *get_job_trcid( ID )
TRC_ID ID;
{
	struct job *jp;

	jp = joblist->j_link;

	while ( jp != joblist )
	{
		if ( jp->j_trcid == ID )
			return( jp );

		jp = jp->j_link;
	}

	fprintf( stderr, "Warning:  Matching Job Trace ID Not Found\n" );

	return( (struct job *) NULL );
}


/* Tracer Stub Routines - Called by library... */


/* Status Message Handler */

void
status_msg( ID, msg )
TRC_ID ID;
char *msg;
{
	struct job *jp;

	/* Find Corresponding Job */

	jp = get_job_trcid( ID );

	if ( jp != NULL )
		fprintf( stderr, "[%d] libpvmtrc: %s\n",
				jp->j_jid - joboffset, msg );
}


/* Trace Event Line Header */

void
event_dump_hdr( ID, tid )
TRC_ID ID;
int tid;
{
	struct job *jp;
	int show;

	jp = get_job_trcid( ID );

	if ( jp != NULL ) {
		show = pvm_getopt( PvmShowTids );
		if ( show )
			fprintf( ID->trace_out, "[T%d:t%x] ",
					jp->j_jid - joboffset, tid );
	}
}


/* Task Output Line Header */

void
output_dump_hdr( ID, tid )
TRC_ID ID;
int tid;
{
	struct job *jp;
	int show;

	jp = get_job_trcid( ID );

	if ( jp != NULL ) {
		show = pvm_getopt( PvmShowTids );
		if ( show )
			fprintf( ID->output_fp, "[%d:t%x] ",
					jp->j_jid - joboffset, tid );
	}
}

