
static char rcsid[] =
	"$Id: taskf.c,v 1.5 2009/01/23 01:12:57 pvmsrc Exp $";

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

/**********************************************************************
 *	Filename: 	taskf.c
 *
 *  contains functions used in mailbox example codes
 *
 *  task0.c task1.c task_end.c
 *  lmbi.c gmbi.c rme.c
 *  others ???
 *
 *********************************************************************/

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include "pvm3.h"

/**********************************************************************
 *  Function: display_incomming_parameters
 *
 *  displays the command line parameters from program
 *********************************************************************/
void
display_incomming_parameters( who, argc, argv )
char *who;
int argc;
char *argv[];
{
	char *me = "display_incomming_parameters";
	int i;
	printf( "\n%s: %d incoming parameters to main.\n", who, argc );
	for ( i=0 ; i < argc ; i++ )
		printf( "%s: argv[%d] = <%s>\n", who, i, argv[i] );
} /* display_incomming_parameters */



/**********************************************************************
 *  Function: get_flagstring
 *
 *  flags: Incoming integer that represents the sum of Mbox flags.
 *
 *  returns: Outgoing string of flags set.
 *		may have:	PvmMboxDefault		d =  0
 *				PvmMboxPersistent	p =  1
 *				PvmMboxMultiInstance	m =  2
 *				PvmMboxOverWritable	o =  4
 *				PvmMboxFirstAvail	f =  8
 *				PvmMboxReadAndDelete	r = 16
 *
 *  returns a pointer to a string containing flags set
 *  returns NULL pointer on error
 *********************************************************************/
char *
get_flagstring( flags )
int flags;
{
	char *me = "get_flagstring";
	int size = 0;
	char *flagstring = ( char * ) NULL;

	/*
	 *  string space to return
	 */
	if ( (flagstring = (char *) calloc( 255, sizeof(char) )) == NULL ) {
		printf( "\n%s: failed on calloc.\n", me );
		return( ( char * ) NULL );
	}

	/* set to null so that empty string test works */
	flagstring[0] = '\0';

	if ( flags == PvmMboxDefault ) {
		strcat( flagstring, "PvmMboxDefault " );
	}
	else {
		if ( flags >= PvmMboxReadAndDelete ) {
			strcat( flagstring, "PvmMboxReadAndDelete " );
			flags -= PvmMboxReadAndDelete;
		}
		if ( flags >= PvmMboxFirstAvail ) {
			strcat( flagstring, "PvmMboxFirstAvail " );
			flags -= PvmMboxFirstAvail;
		}
		if ( flags >= PvmMboxOverWritable ) {
			strcat( flagstring, "PvmMboxOverWritable " );
			flags -= PvmMboxOverWritable;
		}
		if ( flags >= PvmMboxMultiInstance ) {
			strcat( flagstring, "PvmMboxMultiInstance " );
			flags -= PvmMboxMultiInstance;
		}
		if ( flags >= PvmMboxPersistent ) {
			strcat( flagstring, "PvmMboxPersistent " );
			flags -= PvmMboxPersistent;
		}
	}
	return( flagstring );
} /* get_flagstring */



/**********************************************************************
 *  Function: set_flags
 *
 *  flagsin: Incoming string containing characters representing the
 *   Mbox switches set.
 *           An empty string is assumed to be "d" for PvmMboxDefault.
 *		valid chars are ( d p m o f r ) in any order.
 *		Any bogus characters are ignored...
 *
 *  flagstring: Outgoing string of flags set.
 *		may have:	PvmMboxDefault		d =  0
 *				PvmMboxPersistent	p =  1
 *				PvmMboxMultiInstance	m =  2
 *				PvmMboxOverWritable	o =  4
 *				PvmMboxFirstAvail	f =  8
 *				PvmMboxReadAndDelete	r = 16
 *
 *  returns a positive integer representing addition of all set flags.
 *  returns a negative integer to indicate an error condition
 *********************************************************************/
int
set_flags( flagsin, flagstring )
char *flagsin;
char **flagstring;
{
	char *me = "set_flags";
	int size = 0;
	int flags = PvmMboxDefault;

	/*
	 *  check for existance of flag character and
	 *  set results accordingly
	 */

	/* ambiguious but do anyway... */
	if ( strchr( flagsin, 'd' ) != NULL ) {
		flags |= PvmMboxDefault;
	}
	if ( strchr( flagsin, 'r' ) != NULL ) {
		flags |= PvmMboxReadAndDelete;
	}
	if ( strchr( flagsin, 'f' ) != NULL ) {
		flags |= PvmMboxFirstAvail;
	}
	if ( strchr( flagsin, 'o' ) != NULL ) {
		flags |= PvmMboxOverWritable;
	}
	if ( strchr( flagsin, 'm' ) != NULL ) {
		flags |= PvmMboxMultiInstance;
	}
	if ( strchr( flagsin, 'p' ) != NULL ) {
		flags |= PvmMboxPersistent;
	}

	/*
	 *  get flags.
	 *  flags other than those expected "drfomp" will return as
	 *  PvmMboxDefault
	 */
	if ( (*flagstring = get_flagstring( flags )) == NULL ) return( -1 );
	return( flags );
} /* set_flags */

