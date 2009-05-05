
static char rcsid[] =
	"$Id: inherit2.c,v 1.4 2009/01/23 01:12:48 pvmsrc Exp $";

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
 *	Filename: 	inherit2.c
 *
 *  support program for inherita.c and inheritb.c
 *    - read docs in those files.
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
	char *me = "inherit2";
	int ptid;
	char buf[100];
	char machine[25];

	ptid = pvm_parent();

	gethostname( machine, 25 );

	sprintf( buf, "\t%s t%x on machine <%s> with context %d.",
			me, pvm_mytid(), machine, pvm_getcontext() );

	/* Send message to parent */
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( buf );
	pvm_send( ptid, 1 );

	pvm_exit();
	exit( 0 );
}

