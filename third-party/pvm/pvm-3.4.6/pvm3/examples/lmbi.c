
static char rcsid[] =
	"$Id: lmbi.c,v 1.7 2009/01/23 01:12:48 pvmsrc Exp $";

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
 * Filename: lmbi.c
 *
 * This program will "populate" the mailbox with a number of entries
 * so that another program ( gmbi.c ) may then retrieve these entries 
 * using the pvm_getmboxinfo() function.
 *
 * Flags will be set the same for all instances started.
 *
 * usage: lmbi #instances <flags> <exit_status )
 *
 *     flags:
 *         PvmMboxDefault         d
 *         PvmMboxPersistent      p
 *         PvmMboxMultiInstance   m
 *         PvmMboxOverWritable    o
 *
 *     exit_status:
 *         wait for "end" message        w
 *         exit when finished loading    e  ( see note 1 )
 *
 *  Note 1:
 *  -------
 *      Unless the "p" flag is set - all entries to the maibox
 *      created by this run will be removed when this task exits.
 *      So, only when "p" is set should the "e" exit status be used.
 *      Unless you wnat the mailbox entries to be created and then 
 *      immediately removed.
 *
 *      Remember that with "p" and "e" flag set that the tid will
 *      become 0 for the associated mailbox entry( s ) since the "owner"
 *      task will no longer exist. -- this is normal and expected...
 *
 *  files used:  lmbi.c taskf.c
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
	char *me = "lmbi";
	char machine[25];
	char message[250];
	int mytid;
	char service_name[25];
	int i;
	int index;				/* returned index for mailbox entry */

	char flagc[10];			/* hold characters for flags */
	int flags;				/* PvmMbox flags set */
	char *flagstring;		/* pointer to string of flag settings */

	int their_tid;			/* tid of task sending service request */
	char msg_txt[100];		/* message received as service request */
	int msg_buf;			/* message buffer to receive on */

	int entries = 0;		/* # of entries to load into mailbox db */
	char exit_status;		/* incomming exit_status flag value */
	int context;			/* context of this message */

	/* display_incomming_parameters( me, argc, argv ); */

	/*
	 *  validate input parameters
	 */
	if ( argc == 4 ) {
		/* correct quantity - anyway... */
		entries = atoi( argv[1] );	/* number of entries to insert */
		strcpy( flagc, argv[2] );	/* copy flags to local */
		exit_status = argv[3][0];	/* grab only the 1st character */
		if ( exit_status != 'w' && exit_status != 'e' ) {
			/* unknown exit_status entered - complain and exit */
			printf( "\n\nInvalid exit_status value <%c> entered.\n",
				exit_status );
			printf( "\texit_status:\n" );
			printf( "\t\twait for end message\t\tw\n" );
			printf( "\t\texit when finished loading\te\n" );
			exit( -1 );
		}
	}
	else {
		/* unknown input sequence - complain and exit */
		printf( "\n\nusage: lmbi #instances <flags> <exit_status>\n" );
		printf( "\tflags:\n" );
		printf( "\t\tPvmMboxDefault\t\td\n" );
		printf( "\t\tPvmMboxPersistent\tp\n" );
		printf( "\t\tPvmMboxMultiInstance\tm\n" );
		printf( "\t\tPvmMboxOverWritable\to\n" );
		printf( "\texit_status:\n" );
		printf( "\t\twait for end message\t\tw\n" );
		printf( "\t\texit when finished loading\te\n" );
		exit( -1 );
	}

	printf( "%s: sending <%s> to set_flags.\n", me, flagc );
	if ( ( flags = set_flags( flagc, &flagstring ) ) < 0 ) {
		exit( -1 );	/* something failed - abort! */
	}

	printf( "%s: create %d mailbox instances ", me, entries );
	printf( "with flags set to <%d> : <%s>.\n", flags, flagstring );

	if ( ( mytid = pvm_mytid() ) == PvmSysErr ) {
		printf( "\nPVM not up!\n" );
		exit( -1 );
	}

	gethostname( machine, 25 );

	printf( "%s: t%x on machine <%s> with context %d.\n\n",
			me, mytid, machine, pvm_getcontext() );

	/*
	 * create and insert the mailbox entries
	 */
	for ( i=0 ; i < entries ; i++ )
	{
		/*
		 *  create message to place in mailbox
		 */
		context = pvm_getcontext();
		sprintf( service_name, "t%x%s%d", mytid, me, i );
		sprintf( message,
				"%s: service_name %s   flags %d = %s   machine %s   tid t%x   context %d", 
				me, service_name, flags, flagstring, machine, mytid,
				context );
		pvm_initsend( PvmDataDefault );
		pvm_packf( "%+ %d %d %s %s",
				PvmDataDefault, mytid, context, machine, message );
		if ( (index = pvm_putinfo( service_name, pvm_getsbuf(), flags ))
				== PvmExists ) {
			printf( "can't register - service already running\n" );
			exit( -1 );
		}
		printf( "%s: just inserted entry <%s> at index %d.\n",
				me, service_name, index );
		printf( "\tmessage inserted: %s\n", message );
	} /* end_for */

	/*
	 *  Decide if should hang around and wait for another task to shut
	 *  me down or if I should simply exit on my own.
	 *  This decision is important in the case of using ( or not ) the
	 *  persistent flag as mailbox entries without the persistent flag
	 *  marked will be removed when their creating task exits.
	 *  Thus must either mark as persistent or hang around if I want
	 *  to look at or keep around these mailbox entries.
	 */
	if ( exit_status == 'w' ) {
		for ( ; ; )
		{
			/*
			 *  wait for exit request
			 */
			printf("%s: t%x on machine <%s> with context %d",
				me, mytid, machine, pvm_getcontext());
			printf(" - waiting for exit request.\n");
			msg_buf = pvm_recv( -1, -1 );
			pvm_bufinfo( msg_buf, (int *) 0, (int *) 0, &their_tid );
			pvm_upkstr( msg_txt );
			printf( "%s: t%x %s.\n", me, mytid, msg_txt );
			if ( strncmp( msg_txt, "END", 3 ) == 0 ) break;
		} /* end_for */
	} /* end_if */
	pvm_exit();
	exit( 0 );
}

