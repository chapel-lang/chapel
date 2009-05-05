
static char rcsid[] =
	"$Id: inheritb.c,v 1.4 2009/01/23 01:12:48 pvmsrc Exp $";

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
 *	Filename: 	inheritb.c
 *
 *  This example shows how a generic receive any message from any task
 *   may "select" the message to receive based on the context.
 *
 *  inheritb starts two children (2 x inherit2), each under their own
 *   context so that the parent (inheritb) may receive communication
 *   based on the child's context.
 *
 *  files used:  inheritb.c inherit2.c
 *
 *  usage:  inheritb
 *
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

main()
{
	char *me = "inheritb";
	int context_original, context_1, context_2;
	int ptid, tid, gptid, cc;
	char buf[100];
	char machine[25];
	int i=0;

	gethostname( machine, 25 );

	printf( "%d:%s: t%x on machine <%s> with context %d.\n",
			i++, me, pvm_mytid(), machine, pvm_getcontext() );

	context_original = pvm_getcontext(); /* retrieve initial context */

	context_1 = pvm_newcontext();		 /* get new context 1 */
	pvm_setcontext( context_1 );		 /* activate new context 1 */

	printf( "%d:%s: t%x on machine <%s> with new #1 context %d ",
			i++, me, pvm_mytid(), machine, pvm_getcontext() );
	printf( "--> spawn 1st inherit2.\n" );

	/* start 1st inherit2 worker @ context 1 */
	pvm_spawn( "inherit2", (char **) 0, PvmTaskDefault, "", 1, &tid );

	context_2 = pvm_newcontext();		 /* get new context 2 */
	pvm_setcontext(context_2);			 /* activate new context 2 */

	printf( "%d:%s: t%x on machine <%s> with new #2 context %d ",
			i++, me, pvm_mytid(), machine, pvm_getcontext() );
	printf( "--> spawn 2nd inherit2.\n" );

	/* start 2nd inherit2 worker @ context 2 */
	pvm_spawn( "inherit2", (char **) 0, PvmTaskDefault, "", 1, &tid );

	/* receive from inherit2 worker 2 @ context 2 */
	cc = pvm_recv( -1, -1 );
	pvm_bufinfo( cc, (int *) 0, (int *) 0, &tid );
	pvm_upkstr( buf );
	printf( "%d:%s: t%x %s <-- received from 2nd inherit2.\n",
			i++, me, tid, buf);

	/* reactivate initial context 1*/
	pvm_setcontext(context_1);

	printf( "%d:%s: t%x on machine <%s> with new #1 context %d.\n",
			i++, me, pvm_mytid(), machine, pvm_getcontext() );

	/* receive from inherit2 worker 1 @ context 1 */
	cc = pvm_recv( -1, -1 );
	pvm_bufinfo( cc, (int *) 0, (int *) 0, &tid );
	pvm_upkstr( buf );
	printf( "%d:%s: t%x %s <-- received from 1st inherit2.\n",
			i++, me, tid, buf );

	pvm_exit();
	exit( 0 );
}

