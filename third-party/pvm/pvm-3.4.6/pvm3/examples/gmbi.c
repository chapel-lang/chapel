
static char rcsid[] =
	"$Id: gmbi.c,v 1.7 2009/01/23 01:12:48 pvmsrc Exp $";

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
 *	Filename:	gmbi.c	get mailbox info
 *
 * This program will simply grab and dump the "info" fields
 * from the mailbox to display.
 *
 *  A search expression is used to specify which of the mailboxes
 *  to grab and display.  Note that from the command line prompt
 *  the expression should be enclosed within 'single' quotes so
 *  that your shell does not try to process it...
 *
 *  Example:  gmbi 'ra*ts'
 *  Will match any ( rats ) entry that contains at least one a:
 *			ie ) rats - raats - ra123ts - etc...
 *
 *  Both the unqualified  gmbi  as well as  gmbi '*'   
 *  will return all mailbox entries.
 *
 *
 *  usage: gmbi <expression>
 *
 *  files used:  gmbi.c taskf.c
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

char *get_flagstring();

main( argc, argv )
int argc;
char *argv[];
{
	char *me = "gmbi";
	int info;				/* status of function call */
	char pattern[100];		/* regular expression to search on */
	int nclasses;			/* number of classes returned */

	/* pointer to pvmmboxinfo structure */
	static struct pvmmboxinfo *classes
			= (struct pvmmboxinfo *) NULL;

	int i, j;				/* temp integer */

	/* display_incomming_parameters( me, argc, argv ); */

	/*
	 *  validate input parameters
	 */
	if ( argc == 2 ) {
		/*
		 *  got valid data -- use it...
		 */
		strcpy( pattern, argv[1] );		/* copy expression to local */
	}
	else {
		/*
		 *  didn't get the full command line input - see if can recover?
		 */
		if ( argc == 1 ) {
			/*
			 *  default to retrieve all mailbox entries
			 */
			strcpy( pattern, "*" );
		}
		else {
			/*
			 *  command line input is wacked - make user try again...
			 */
			printf( "\n\nusage: gmbi <expression>\n" );
			exit( -1 );
		}
	} /* end_if */

	/*
	 *  get the mailbox information for the user specified expression 
	 *  pattern and display results
	 */
	if ( (info = pvm_getmboxinfo( pattern, &nclasses, &classes )) < 0 ) {
		printf( "\n\nsomething broke in pvm_getmboxinfo...\n\n" );
		exit( -1 );
	}
	printf( "\n%s: %d mbox entries returned.", me, nclasses );
	printf( "\n-------------------------------" );
	for ( i=0 ; i < nclasses ; i++ ) {
		printf( "\n%s: class name: <%s>", me, classes[i].mi_name );
		for ( j=0 ; j < classes[i].mi_nentries ; j++ ) {
			printf(
			"\n\t: indices <%d>\towner_tid <t%x>\tflags <%d> = <%s>",
					classes[i].mi_indices[j], classes[i].mi_owners[j],
					classes[i].mi_flags[j],
					get_flagstring( classes[i].mi_flags[j] ) );
		} /* end_for j */
	} /* end_for i */
	printf( "\n\n" );
} /* end_main */

