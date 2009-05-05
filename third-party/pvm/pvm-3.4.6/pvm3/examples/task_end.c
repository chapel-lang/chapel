
static char rcsid[] =
	"$Id: task_end.c,v 1.5 2009/01/23 01:12:57 pvmsrc Exp $";

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
 *	Filename: 	task_end.c
 *
 * This program will tell a task0 instance to terminate.
 * This is done by seeking the "known" service from a mailbox,
 * initiating communication telling the task to terminate itself.
 * The message mailbox provides the communication information
 * to the specified task0 instance.
 *
 *	usage: task_end [service_name] [index_nbr]
 *
 *  files used:  task_end.c taskf.c
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
#include "pvm3.h"

main( argc, argv )
int argc;
char *argv[];
{
	char *me = "task_end";
	char machine[25];		/* local machine that task1 is running on */
	int mytid;				/* tid of task1 */
	int my_context;			/* context of task1 - itself */

	int index;				/* index of the service seeking */
	char *service_name;		/* name of the service seeking */

	int their_context;		/* context of the service's owner */
							/*   - comm on this context */
	int msg_buf;			/* buffer to store mailbox's message */
	int their_tid;			/* tid "offering" the desired service */
	char server[25];		/* name of the machine "offering" service */

	char msg_txt[100];		/* message to send to service */

	/* display_incomming_parameters( me, argc, argv ); */

	printf( "\n" );
	if ( argc != 3 ) {
		printf( "\nusage: task_end [service_name] [index_nbr]\n" );
		exit( -1 );
	}

	index = atoi( argv[2] );
	service_name = argv[1];

	printf( "Looking for service name <%s> with index number [%d].\n",
			service_name, index );

	if ( (mytid = pvm_mytid()) == PvmSysErr ) {
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	/*
	 *  look for the user specified service name/index pair
	 */
	msg_buf = pvm_recvinfo( service_name, index, PvmMboxDefault );
	if ( msg_buf >= 0 ) {
		pvm_setrbuf( msg_buf );
		pvm_upkint( &their_tid, 1, 1 );
		pvm_upkint( &their_context, 1, 1 );
		pvm_upkstr( server );
		printf( "service name <%s> of index %d on <%s> id: %x   ",
				service_name, index, server, their_tid );
		printf( "with their context: %d\n", their_context );
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
				lpvmerr( "\n%s: task_end.c: error %d", me, msg_buf );
				break;
		} /* end_switch */
		exit( -1 );
	}

	/*
	 *  send END message to the service with their context set
	 */

	/* activate the server's context */
	my_context = pvm_setcontext( their_context );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );
	sprintf( msg_txt, "END" );
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( msg_txt );
	pvm_send( their_tid, 1 );

	/*
	 *  reset back to my original context
	 */
	pvm_setcontext( my_context );
	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	pvm_exit();
	exit( 0 );
}

