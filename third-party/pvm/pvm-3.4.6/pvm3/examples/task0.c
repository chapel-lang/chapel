
static char rcsid[] =
	"$Id: task0.c,v 1.7 2009/01/23 01:12:52 pvmsrc Exp $";

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
 *	Filename:	task0.c
 *
 * This program will place a "well known" service into a mailbox
 * along with a communication context.  It will then wait for another
 * task to communicate via that context.
 *
 * task1.c is designed as the "other" communicating task.
 * task_end.c tells task0 to terminate itself and quit waiting.
 *
 * Note that you may load a number of these into the mailbox providing
 * they have unique <service_name> key.
 *
 * The task_end.c program will cause this service to gracefully exit
 * pvm.
 *
 *	usage: task0 [service_name] <flags>
 *
 *				PvmMboxDefault		d
 *				PvmMboxPersistent	p
 *				PvmMboxMultiInstance	m
 *				PvmMboxOverWritable	o
 *
 *  files used:  task0.c taskf.c
 */

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#ifndef WIN32
#include <unistd.h>		/* for gethostname */
#else
#include "pvmwin.h"
#endif
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include "pvm3.h"

main( argc, argv )
int argc;
char *argv[];
{
	char *me = "task0";
	char machine[25];
	int mytid;
	int context0, context1;
	char *service_name;
	int service_ctr = 0;	/* count of the # of service requests */
	int i;
	int index;				/* returned index for mailbox entry */

	char flagc[10];			/* hold characters for flags */
	int flags;				/* PvmMbox flags set */
	char *flagstring;		/* pointer to string of flag settings */

	int their_tid;			/* tid of task sending service request */
	char msg_txt[100];		/* message received as service request */
	int msg_buf;			/* message buffer to receive on */

	/* display_incomming_parameters( me, argc, argv ); */

	/*
	 *  validate input parameters
	 */
	if ( argc == 3 ) {
		/* we got what we wanted -- all three */
		strcpy( flagc, argv[2] );		/* copy flags to local */
	}
	else {
		if ( argc == 2 ) {
			/* assume flags left off */
			/*   - so force flags to PvmMboxDefault */
			strcpy( flagc, "d" );
		}
		else {
			/* unknown input sequence - complain and exit */
			printf( "\n\nusage: task0 [service_name] <flags>\n" );
			printf( "\tPvmMboxDefault\t\td\n" );
			printf( "\tPvmMboxPersistent\tp\n" );
			printf( "\tPvmMboxMultiInstance\tm\n" );
			printf( "\tPvmMboxOverWritable\to\n" );
			exit( -1 );
		}
	}

	printf( "%s: sending <%s> to set_flags.\n", me, flagc );
	if ( ( flags = set_flags( flagc, &flagstring ) ) < 0 ) {
		exit( -1 );	/* something failed - abort! */
	}

	service_name = argv[1];
	printf( "%s: service name <%s> with flags <%d> : <%s>.\n",
			me, service_name, flags, flagstring );

	if ( ( mytid = pvm_mytid() ) == PvmSysErr ) {
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	context1 = pvm_newcontext();			/* create new context1 */
	context0 = pvm_setcontext( context1 );	/* activate new context1 */

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	/*
	 *  create message to place in mailbox
	 */
	pvm_initsend( PvmDataDefault );
	pvm_packf( "%+ %d %d %s",
			PvmDataDefault, mytid, context1, machine );
	if ( (index = pvm_putinfo( service_name, pvm_getsbuf(), flags ))
			< 0 ) {
		/*
		 *  did not find mailbox - tell user why and exit...
		 */
		switch ( index ) {
			case PvmExists:
				printf( "\n%s: Names service already running\n", me );
				break;
			case PvmNotFound:
				printf( "\n%s: no service running\n", me );
				break;
			case PvmBadParam:
				printf( "\n%s: Invalid argument to pvm_recvinfo().\n",
						me );
				break;
			case PvmNoSuchBuf:
				printf( "\n%s: Message buffer id does not exist.\n",
						me );
				break;
			case PvmDenied:
				printf(
					"\n%s: Key locked by another task, can't delete.\n",
						me );
				break;
			default:
				lpvmerr( "\n%s: task0.c: error %d", me, index );
				break;
		} /* end_switch */
		exit( -1 );
	}

	printf( "%s: just inserted service name <%s> at index %d.\n",
			me, service_name, index );

	for ( ; ; )
	{
		/*
		 *  wait for service requests
		 */
		printf( "%s: t%x on machine <%s> with context %d ",
				me, mytid, machine, pvm_getcontext() );
		printf( "- waiting for service request.\n" );
		msg_buf = pvm_recv( -1, -1 );
		pvm_bufinfo( msg_buf, (int *) 0, (int *) 0, &their_tid );
		pvm_upkstr( msg_txt );
		printf( "%s: t%x %s  Request number %d.\n",
				me, mytid, msg_txt, ++service_ctr );

		if ( strncmp( msg_txt, "END", 3 ) == 0 )
			break;

		/*
		 *  send a greeting message to service requestor
		 */
		sprintf( msg_txt,
			"welcome to my server - you are service request number %d.",
				service_ctr );
		pvm_initsend( PvmDataDefault );
		pvm_pkstr( msg_txt );
		pvm_send( their_tid, 1 );
	}

	/*
	 *  reset back to my original context
	 */
	(void) pvm_setcontext( context0 );
	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );
	printf( "%s: handled %d service requests.\n", me, service_ctr );

	pvm_exit();
	exit( 0 );
}

