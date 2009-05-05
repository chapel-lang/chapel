
static char rcsid[] =
	"$Id: rme.c,v 1.5 2009/01/23 01:12:52 pvmsrc Exp $";

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
 *	Filename: 	rme.c		( remove mailbox entry )
 *
 *  This program will seek out a "well known" service from a mailbox
 *  and then remove it.
 *
 *  Don't bother with flags here as they are not used by pvm_delinfo()
 *
 *	usage: rme [service_name] [index_nbr]
 *
 *  Note: If a task is waiting for someone to communicate via mailbox
 *	information will be left stranded after this program removes 
 *	the associated entry from the mailbox database.
 *
 *  files used:  rme.c taskf.c
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
	char *me = "rme";
	char machine[25];
	int mytid;
	int info;
	int index;			 	/* index of the service seeking */
	char *service_name;		/* name of the service seeking */

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

	if ( ( mytid = pvm_mytid() ) == PvmSysErr ) {
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext() );

	/*
	 *  attempt to delete the ( service_name, index ) pair from the
	 *  mailbox
	 */
	if ( (info = pvm_delinfo( service_name, index, PvmMboxDefault ))
			< 0 ) {
		/*
		 *  did not find mailbox - tell user why and exit...
		 */
		switch ( info )
		{
			case PvmNotFound:
				printf(
					"\n%s: no service running for <%s> index <%d>\n",
						me, service_name, index );
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
				lpvmerr( "\n%s: rme.c: error %d", me, info );
				break;
		} /* end_switch */
		exit( -1 );
	} /* end_if */

	pvm_exit();
	exit( 0 );
}

