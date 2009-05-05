
static char rcsid[] =
	"$Id: inherita.c,v 1.5 2009/01/23 01:12:48 pvmsrc Exp $";

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
 *	Filename: 	inherita.c
 *
 *  This example code performs two tests depending on how it is
 *   executed.
 *
 *  Form (test 1) confirms that a new context is inherited by all
 *   following children.  Here, inherita spawns inherit1 which in
 *   turn spawns inherit2 - all of which inherit the context of the
 *   initial inherita program.
 *
 *  Form (test 2) confirms that a child task's new context will be
 *   inherited by its subsequent child tasks.  Here, inherita spawns
 *   inherit3 which gets a new context and then spawns inherit2.
 *   inherit2 now inherits that new context from inherit3 and not
 *   the original from inherita.
 *
 *  files used:	test 1.  inherita inherit1 inherit2
 *		test 2.  inherita inherit3 inherit2
 *
 *  usage: test 1.  inherita inherit1
 *	  test 2.  inherita inherit3
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
	char *me = "inherita";
	int cc, tid, mytid;
	char buf[100];
	char machine[25];
	int context_1, context_2;
	int j;

	/* display_incomming_parameters( me, argc, argv ); */

	if ( argc < 2 ) {
		printf( "\nusage: inherita [inherit1 | inherit3]\n");
		pvm_exit();
		exit( 0 );
	}

	printf( "\nWill attempt to spawn <%s> from %s.\n\n", argv[1], me );

	if ( (mytid = pvm_mytid()) == PvmSysErr ) {	/* enroll in pvm */
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with original context %d.\n",
			me, pvm_mytid(), machine, pvm_getcontext() );

	context_2 = pvm_newcontext();			 /* create new context */
	context_1 = pvm_setcontext( context_2 ); /* activate new context */

	printf( "%s: t%x on machine <%s> with changed context %d.\n\n",
			me, pvm_mytid(), machine, pvm_getcontext() );

	/*
	 *  Spawn a child process to confirm that it will inherit the
	 *   parent context.
	 */
	cc = pvm_spawn( argv[1], (char **) 0, PvmTaskDefault, "", 1, &tid );
	if ( cc != 1 ) {
		printf( "%s: can't start %s\n", me, argv[1] );
		pvm_exit();
		exit( 0 );
	}

	for ( ; ; )
	{
		/*
		 *  wait to receive message from child
		 */
		cc = pvm_recv( -1, -1 );
		pvm_bufinfo( cc, (int *) 0, (int *) 0, &tid );
		pvm_upkstr( buf );

		printf( "%s: t%x %s\n", me, tid, buf );

		if ( strncmp( buf, "END", 3 ) == 0 )
			break;
	}

	printf( "\n%s: t%x on machine <%s> with changed context %d.\n",
			me, pvm_mytid(), machine, pvm_getcontext() );

	/* restore original context */
	context_2 = pvm_setcontext( context_1 );

	printf( "%s: t%x on machine <%s> with original context %d.\n",
			me, pvm_mytid(), machine, pvm_getcontext() );

	pvm_exit();
	exit( 0 );
}

