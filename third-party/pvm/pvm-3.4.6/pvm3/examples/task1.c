
static char rcsid[] =
	"$Id: task1.c,v 1.6 2009/01/23 01:12:52 pvmsrc Exp $";

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
 *	Filename: 	task1.c
 *
 * This program will seek out a "well known" service from a mailbox
 * and then initiate communication with that service via the
 * mailbox "message" specified communication context.
 *
 * This is designed to be used along with task0.c as they have
 * coordinated mailbox message formats.
 *
 *	usage: task1 [service_name] <index> <flags>
 *
 *				PvmMboxDefault		d
 *				PvmMboxFirstAvail	f
 *				PvmMboxReadAndDelete	r
 *
 *  Note: A task started by task0.c is waiting for someone to
 *  communicate via mailbox information.  If task1 communicates via
 *	a PvmMboxReadAndDelete - the mailbox entry will be removed but
 *	the waiting task will be left to wait indefinately since no
 *	longer is the associated mailbox information available.
 *	Additional code is required to clean-up waiting task too -
 *	but that is not the intent of this code example...
 *
 *  files used:  task1.c taskf.c
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
	char *me = "task1";
	char machine[25];		/* local machine that task1 is running on */
	int mytid;				/* tid of task1 */
	int my_context;			/* context of task1 - itself */
	int i;
	int index = 0;			/* index of mailbox message */

	char *service_name;		/* name of the service seeking */

	int their_context;		/* context of the service's owner */
							/*   - comm on this context */
	int msg_buf;			/* buffer to store mailbox's message */
	int their_tid;			/* tid "offering" the desired service */
	char server[25];		/* name of machine "offering" the service */

	char msg_txt[100];		/* message to send to service */

	char flagc[10];			/* hold characters for flags */
	int flags;				/* PvmMbox flags set */
	char *flagstring;		/* pointer to string of flag settings */

	/* display_incomming_parameters( me, argc, argv ); */

	/*
	 *  validate input parameters
	 */
	if ( argc == 4 ) {
		/* we got what we wanted -- all three */
		strcpy( flagc, argv[3] );		/* copy flags to local */
		index = atoi( argv[2] );		/* get the index value */
	}
	else {
		if ( argc == 2 ) {
			/* assume index and flags left off */
			/*   - so force index = 0, flags = PvmMboxDefault */
			strcpy( flagc, "d" );
			index = 0;
		}
		else{
			/* unknown input sequence - complain and exit */
			printf( "\n\nusage: task1 [service_name] <index> <flags>\n" );
			printf( "\tPvmMboxDefault\t\td\n" );
			printf( "\tPvmMboxFirstAvail\tf\n" );
			printf( "\tPvmMboxReadAndDelete\tr\n\n" );
			exit( -1 );
		}
	}

	/* printf( "%s: sending <%s> to set_flags.\n", me, flagc ); */
	if ( ( flags = set_flags( flagc, &flagstring ) ) < 0 ) {
		exit( -1 );	/* something failed - abort! */
	}

	service_name = argv[1];
	printf( "%s: Looking for service name <%s>, index %d ",
			me, service_name, index );
	printf( "- my flags are <%d> : <%s>.\n", flags, flagstring );

	if ( ( mytid = pvm_mytid() ) == PvmSysErr ) {
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	/*
	 *  look for the user specified service name
	 */
	msg_buf = pvm_recvinfo( service_name, index, flags );
	if ( msg_buf >= 0 ) {
		pvm_setrbuf( msg_buf );
		pvm_upkint( &their_tid, 1, 1 );
		pvm_upkint( &their_context, 1, 1 );
		pvm_upkstr( server );

		/*
		 *  Note that this displays the "index" requested which may
		 *  not be the actual one of that returned if PvmMboxFirstAvail
		 *  flag is set...
		 */
		printf( "%s: service name <%s> and index %d with flags <%s> ",
				me, service_name, index, flagstring );
		printf( "tid: %x and with context: %d\n",
				their_tid, their_context );
	}
	else {
		switch ( msg_buf )
		{
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
				lpvmerr( "task1.c: error", msg_buf );
				break;
		} /* end_switch */
		exit( -1 );
	}

	/*
	 *  send message to the service with their context set
	 */

	/* activate the server's context */
	my_context = pvm_setcontext( their_context );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	sprintf( msg_txt, "\t%s t%x on machine <%s> with context %d.",
			me, mytid, machine, pvm_getcontext() );
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( msg_txt );
	pvm_send( their_tid, 1 );

	/*
	 *  receive message from service
	 */
	msg_buf = pvm_recv( their_tid, -1 );
	pvm_bufinfo( msg_buf, (int *) 0, (int *) 0, &their_tid );
	pvm_upkstr( msg_txt );
	printf( "%s: t%x  from task t%x %s\n",
			me, mytid, their_tid, msg_txt );

	/*
	 *  reset back to my original context
	 */
	pvm_setcontext( my_context );
	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	pvm_exit();
	exit( 0 );
}

