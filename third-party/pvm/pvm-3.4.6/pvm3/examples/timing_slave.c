
static char rcsid[] =
	"$Id: timing_slave.c,v 1.2 1997/07/09 13:26:19 pvmsrc Exp $";

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
*	timing_slave.c
*
*	See timing.c
*/

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "pvm3.h"

#define ENCODING  PvmDataRaw

main(argc, argv)
	int argc;
	char **argv;
{
	int mytid;   /* my task id */
	int dtid;    /* driver task */
	int bufid;
	int n = 0;

	/* enroll in pvm */

	mytid = pvm_mytid();

    /* tell parent I am ready */

    pvm_setopt(PvmRoute, PvmRouteDirect);
    pvm_initsend(ENCODING);
    pvm_send( pvm_parent(), 0 );

	/* pack mytid in buffer */

	pvm_initsend(ENCODING);
	pvm_pkint(&mytid, 1, 1);

	/* our job is just to echo back to the sender when we get a message */

	while (1) {
		bufid = pvm_recv(-1, -1);
		pvm_bufinfo(bufid, (int*)0, (int*)0, &dtid);
		pvm_freebuf(pvm_getrbuf());  /* for shared memory refcount hang */
		pvm_send(dtid, 2);
/*
		printf("echo %d\n", ++n);
*/
	}
}

