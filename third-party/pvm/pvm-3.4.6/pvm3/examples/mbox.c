
static char rcsid[] =
	"$Id: mbox.c,v 1.12 2009/01/23 01:12:52 pvmsrc Exp $";

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

/*********************************************************************

   mbox - PVM 3.4 Message Mailbox Test Program

   author:  James Arthur Kohl
            Oak Ridge National Laboratory
            March, 1997.

**********************************************************************/

/*********************************************************************

  PVM 3.4 Message Mailbox Tests:
  ------------------------------

  All tests to be run, in order, on a single host VM, on the master
  host of 2+ host VM, and on the slave host of 2+ host VM.

  An '*' marking indicates that, for multiple host VMs, the marked
  task should be spawned on a different host than the other non-marked
  tasks.

  Tests are of the form:
        ( test invocation ) -> expected status returned

  1.    ( mbox ) -> 1

  2.  * ( mbox query ) -> 0

  3.    ( mbox sleep=3 & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1

  4.  * ( mbox query ) -> 0

  5.    ( mbox sleep=3 & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1
        ( mbox sleep=3 & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1
  
  6.  * ( mbox query ) -> 0
  
  7.    ( mbox sleep=3 & ) -> 1
        ( mbox excl ) -> 0
  
  8.  * ( mbox query ) -> 0
  
  9.    ( mbox sleep=3 & ) -> 1
        ( mbox favail ) -> 1
      * ( mbox query ) -> 0
  
  10.   ( mbox sleep=3 & ) -> 1
        ( mbox favail excl ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 0
      * ( mbox query ) -> 1
  
  11. * ( mbox query ) -> 0
  
  12.   ( mbox sleep=3 & ) -> 1
        ( mbox sleep=3 favail excl & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 1
      * ( mbox query ) -> 2
  
  13. * ( mbox query ) -> 0
  
  14.   ( mbox sleep=3 lock & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1
        ( mbox ) -> 0
        ( mbox lock ) -> 0
        ( mbox excl ) -> 0
        ( mbox lock excl ) -> 0
        ( mbox delete 0 ) -> 0
        ( mbox favail ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 0
      * ( mbox query ) -> 1
  
  15.   ( mbox query ) -> 0
  
  16.   ( mbox sleep=3 lock & ) -> 1
        ( mbox sleep=3 favail & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 1
      * ( mbox query ) -> 2
        ( mbox favail ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 0
      * ( mbox query ) -> 1
  
  17.   ( mbox query ) -> 0
  
  18.   ( mbox sleep=3 lock & ) -> 1
        ( mbox sleep=3 favail & ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 1
      * ( mbox query ) -> 2
        ( mbox favail excl ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 1
      * ( mbox query ) -> 2
  
  19.   ( mbox query ) -> 0
  
  20.   ( mbox pers ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1
        ( mbox delete 0 ) -> 1
      * ( mbox query ) -> 0
  
  21.   ( mbox pers lock ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query ) -> 1
        ( mbox delete 0 ) -> 1
      * ( mbox query ) -> 0
  
  22.   ( mbox pers ) -> 1
      * ( mbox delete 0 ) -> 1
  
  23.   ( mbox pers ) -> 1
        ( mbox pers favail ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 0
      * ( mbox query ) -> 1
        ( mbox delete 0 ) -> 1
  
  24.   ( mbox pers ) -> 1
        ( mbox pers excl ) -> 0
        ( mbox pers favail excl ) -> 1
        ( mbox pers favail excl ) -> 1
      * ( mbox query 0 ) -> 1
      * ( mbox query 1 ) -> 1
      * ( mbox query 2 ) -> 1
      * ( mbox query ) -> 3
        ( mbox delete ) -> 3
  
  25.   ( mbox sleep=10 noreset & ) -> 1
        ( ... do pvm reset ... )
        ( mbox query 0 ) -> 1
  
  26.   ( mbox delete ) -> 0
  
  27.   ( mbox noexit ) -> 1
      * ( mbox query ) -> 0

**********************************************************************/
  
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

#define MBOX_CLASS	"mbox_test"

void dump_flags();
void set_flags();
void usage();

int mytid;

main( argc, argv )
int argc;
char *argv[];
{
	char CLASS_NAME[255];

	struct pvmmboxinfo *classes;
	int nclasses;

	char pattern[255];

	char *cmds[100];
	int ncmds;

	char bufout[255];
	char bufin[255];

	char *arg;

	int NINDICES = 10;

	int do_noreset;
	int do_favail;
	int do_unpack;
	int do_minst;
	int do_owrit;
	int do_rddel;
	int do_sleep;
	int do_exit;
	int do_pers;
	int do_wait;

	int index;
	int flags;
	int done;
	int i, j;
	int ret;
	int tmp;
	int cc;

	/* Parse Command Line */

	strcpy( CLASS_NAME, MBOX_CLASS );

	do_noreset = 0;
	do_favail = 0;
	do_unpack = 1;
	do_minst = 0;
	do_owrit = 0;
	do_rddel = 0;
	do_sleep = 0;
	do_exit = 1;
	do_pers = 0;
	do_wait = 0;

	ncmds = 0;

	for ( i=1 ; i < argc ; i++ )
	{
		arg = argv[i];

		if ( arg[0] == '-' )
			arg++;

		if ( !strcmp( arg, "pers" ) )
			do_pers++;

		else if ( !strcmp( arg, "favail" ) )
			do_favail++;

		else if ( !strcmp( arg, "sleep" ) )
			do_sleep = 10;

		else if ( !strncmp( arg, "sleep=", 6 ) )
			sscanf( arg + 6, "%d", &do_sleep );

		else if ( !strcmp( arg, "owrit" ) )
			do_owrit++;

		else if ( !strcmp( arg, "minst" ) )
			do_minst++;

		else if ( !strcmp( arg, "rddel" ) )
			do_rddel++;

		else if ( !strcmp( arg, "wait" ) )
			do_wait++;

		else if ( !strcmp( arg, "noexit" ) )
			do_exit = 0;

		else if ( !strcmp( arg, "noreset" ) )
			do_noreset = 1;

		else if ( !strncmp( arg, "name=", 5 ) )
		{
			strcpy( CLASS_NAME, arg + 5 );
			do_unpack = 0;
		}

		else if ( !strncmp( arg, "nindices=", 9 ) )
			NINDICES = atoi( arg + 9 );

		else if ( !strcmp( arg, "help" ) )
			usage();

		else
			cmds[ ncmds++ ] = arg;
	}

	/* Enter PVM */

	mytid = pvm_mytid();

	if ( mytid < 0 )
	{
		printf( "*** Could not connect to PVM...  Exiting... ***\n" );

		exit( -1 );
	}
	
	else
		printf( "mbox started as tid=0x%x\n", mytid );

	ret = 0;

	if ( do_noreset )
		pvm_setopt( PvmNoReset, 1 );

	/* Do Basic Stuff & Check */

	if ( !ncmds )
	{
		strcpy( bufin, "This Message Left Here By Mbox" );

		set_flags( &flags, do_pers, do_favail, do_owrit, do_minst,
			do_rddel, do_wait );

		index = stuff_msg( CLASS_NAME, flags, bufin, mytid );

		if ( index < 0 )
			exit( -1 );

		cc = check_msg( CLASS_NAME, index, 0, bufout, &tmp, do_unpack );

		if ( cc < 0 )
			exit( -1 );
		
		if ( strcmp( bufout, bufin ) )
		{
			printf( "String Mismatch Error:  \"%s\"  !=  \"%s\"\n",
				bufout, bufin );
		}

		else if ( tmp != mytid )
		{
			printf( "Number Mismatch Error:  %d  !=  %d\n",
				tmp, mytid );
		}

		else
		{
			printf( "Retrieved Message is a Match.\n" );

			ret++;
		}

		if ( do_sleep )
			pvmsleep( do_sleep );
	}

	/* Execute Commands */

	else
	{
		if ( !strcmp( cmds[0], "query" ) )
		{
			set_flags( &flags, do_pers, do_favail, do_owrit, do_minst,
				do_rddel, do_wait );

			if ( ncmds == 1 )
			{
				for ( i=0 ; i < NINDICES ; i++ )
				{
					cc = check_msg( CLASS_NAME, i, flags,
						bufout, &tmp, do_unpack );

					if ( cc < 0 )
						printf( "No Message at Index=%d\n", i );

					else
					{
						printf( "Message at Index=%d:  \"%s\"  0x%x\n",
							i, bufout, tmp );

						ret++;
					}
				}
			}

			else
			{
				index = atoi( cmds[1] );

				cc = check_msg( CLASS_NAME, index, flags,
					bufout, &tmp, do_unpack );

 				if ( cc < 0 )
					printf( "No Message at Index=%d\n", index );

				else
				{
					printf( "Message at Index=%d:  \"%s\"  0x%x\n",
						index, bufout, tmp );

					ret++;
				}
			}
		}

		else if ( !strcmp( cmds[0], "delete" ) )
		{
			set_flags( &flags, do_pers, do_favail, do_owrit, do_minst,
				do_rddel, do_wait );

			if ( ncmds == 1 )
			{
				for ( i=0 ; i < NINDICES ; i++ )
				{
					cc = pvm_delinfo( CLASS_NAME, i, flags );

					if ( cc < 0 )
					{
						pvm_perror( "pvm_delinfo()" );

						printf( "No Message to Delete at Index=%d\n",
							i );
					}

					else
					{
						printf( "Message at Index=%d Deleted.\n", i );

						ret++;
					}
				}
			}

			else
			{
				index = atoi( cmds[1] );

				cc = pvm_delinfo( CLASS_NAME, index, flags );

				if ( cc < 0 )
				{
					pvm_perror( "pvm_delinfo()" );

					printf( "No Message to Delete at Index=%d\n",
						index );
				}

				else
				{
					printf( "Message at Index=%d Deleted.\n", index );

					ret++;
				}
			}
		}

		else if ( !strcmp( cmds[0], "querynoreset" ) )
		{
			done = 0;
			i = 0;

			while ( !done )
			{
				cc = check_msg( PVMNORESETCLASS, i, PvmMboxFirstAvail,
					bufout, &tmp, 0 );

				if ( cc < 0 )
					done++;

				else
				{
					pvm_upkint( &tmp, 1, 1 );

					printf( "NoReset Message at Index=%d:  0x%x\n",
						i, tmp );

					ret++;

					i++;
				}
			}
		}

		else if ( !strcmp( cmds[0], "spawn" ) )
		{
			pvm_spawn( "mbox", argv + 2,
				PvmTaskHost | PvmHostCompl, ".", 1, &cc );

			if ( cc < 0 )
				pvm_perror( "Spawning on Other Host" );
			
			else
			{
				pvm_notify( PvmTaskExit, 100, 1, &cc );

				pvm_recv( -1, 100 );
			}
		}

		else if ( !strcmp( cmds[0], "info" ) )
		{
			if ( ncmds == 1 )
				strcpy( pattern, "*" );
			
			else
				strcpy( pattern, cmds[1] );

			if ( (cc = pvm_getmboxinfo( pattern, &nclasses, &classes ))
					< 0 )
				pvm_perror( "Querying Mbox Info" );
			
			else
			{
				for ( i=0 ; i < nclasses ; i++ )
				{
					printf( "Class \"%s\" has %d entries:\n",
						classes[i].mi_name, classes[i].mi_nentries );
					
					for ( j=0 ; j < classes[i].mi_nentries ; j++ )
					{
						printf( "\tindex=%d owner=0x%x/%d flags=0x%x\n",
							classes[i].mi_indices[j],
							classes[i].mi_owners[j],
							classes[i].mi_owners[j],
							classes[i].mi_flags[j] );
					}

					printf( "\n" );
				}
			}
		}

		else if ( !strcmp( cmds[0], "default" ) )
		{
			set_flags( &flags, 1 /*do_pers*/, 1 /*do_favail*/, do_owrit,
				1 /*do_minst*/, do_rddel, do_wait );

			for ( i=0 ; i < 6 ; i++ )
			{
				sprintf( bufin,
					"This Message Left Here By Mbox (%d)", i );

				index = stuff_msg( CLASS_NAME, flags, bufin, mytid );

				if ( index < 0 )
					exit( -1 );

				cc = check_msg( CLASS_NAME, index, 0, bufout, &tmp,
					do_unpack );

				if ( cc < 0 )
					exit( -1 );
		
				if ( strcmp( bufout, bufin ) )
				{
					printf(
						"String Mismatch Error:  \"%s\"  !=  \"%s\"\n",
						bufout, bufin );
				}

				else if ( tmp != mytid )
				{
					printf( "Number Mismatch Error:  %d  !=  %d\n",
						tmp, mytid );
				}

				else
				{
					printf( "Retrieved Message is a Match.\n" );

					ret++;
				}
			}

			if ( do_sleep )
				pvmsleep( do_sleep );

			cc = pvm_delinfo( CLASS_NAME, 3, flags );

			if ( cc < 0 )
			{
				pvm_perror( "pvm_delinfo()" );

				printf( "No Message to Delete at Index=%d\n",
					index );
			}

			else
			{
				printf( "Message at Index=%d Deleted.\n", index );

				ret++;
			}

			sprintf( bufin,
				"This Message Left Here By Mbox (X)" );

			index = stuff_msg( CLASS_NAME, flags, bufin, mytid );

			if ( index < 0 )
				exit( -1 );

			cc = check_msg( CLASS_NAME, index, 0, bufout, &tmp,
				do_unpack );

			if ( cc < 0 )
				exit( -1 );
	
			if ( strcmp( bufout, bufin ) )
			{
				printf(
					"String Mismatch Error:  \"%s\"  !=  \"%s\"\n",
					bufout, bufin );
			}

			else if ( tmp != mytid )
			{
				printf( "Number Mismatch Error:  %d  !=  %d\n",
					tmp, mytid );
			}

			else
			{
				printf( "Retrieved Message is a Match.\n" );

				ret++;
			}

			if ( do_sleep )
				pvmsleep( do_sleep );

			for ( i=0 ; i < 3 ; i++ )
			{
				cc = pvm_delinfo( CLASS_NAME, i, flags );

				if ( cc < 0 )
				{
					pvm_perror( "pvm_delinfo()" );

					printf( "No Message to Delete at Index=%d\n", i );
				}

				else
				{
					printf( "Message at Index=%d Deleted.\n", i );

					ret++;
				}
			}

			set_flags( &flags, 0 /*do_pers*/, 1 /*do_favail*/,
				0 /*do_owrit*/, 1 /*do_minst*/, 1 /*do_rddel*/,
				do_wait );

			cc = check_msg( CLASS_NAME, 0, flags,
				bufout, &tmp, do_unpack );

			if ( cc < 0 )
				exit( -1 );
	
			printf( "Retrieved Message is \"%s\" from tid=%d.\n",
				bufout, tmp );

			ret++;

			set_flags( &flags, 0 /*do_pers*/, 1 /*do_favail*/,
				0 /*do_owrit*/, 1 /*do_minst*/, 0 /*do_rddel*/,
				do_wait );

			cc = check_msg( CLASS_NAME, 0, flags,
				bufout, &tmp, do_unpack );

			if ( cc < 0 )
				exit( -1 );

			printf( "Retrieved Message is \"%s\" from tid=%d.\n",
				bufout, tmp );

			ret++;
		}

		else if ( !strcmp( cmds[0], "default33" ) )
		{
			for ( i=0 ; i < 7 ; i++ )
			{
				tmp = ( i == 6 ) ? -1 : i;

				index = pvm_insert( CLASS_NAME, tmp, i * 2 );

				if ( index < 0 )
				{
					pvm_perror( "pvm_insert()" );
					exit( -1 );
				}

				else
				{
					printf(
					"Value %d Successfully Inserted at %d (req=%d)\n",
						i * 2, index, tmp );

					ret++;
				}

				cc = pvm_insert( CLASS_NAME, index, i * 3 );

				if ( cc < 0 )
				{
					printf(
						"Re-insert at Index=%d Failed as Expected.\n",
						index );

					ret++;
				}

				else
				{
					printf( "Uh-Oh, Re-Insert at Index=%d Succeeded!\n",
						index );
				}

				cc = pvm_lookup( CLASS_NAME, index, &tmp );

				if ( cc < 0 )
				{
					pvm_perror( "pvm_lookup()" );
					exit( -1 );
				}
		
				if ( tmp != i * 2 )
					printf( "Mismatch Error:  %d != %d\n", tmp, i * 2 );

				else
				{
					printf( "Retrieved Value is a Match.\n" );

					ret++;
				}
			}

			if ( do_sleep )
				pvmsleep( do_sleep );

			index = 3;

			cc = pvm_delete( CLASS_NAME, index );

			if ( cc < 0 )
			{
				pvm_perror( "pvm_delete()" );

				printf( "No Message to Delete at Index=%d\n",
					index );
			}

			else
			{
				printf( "Message at Index=%d Deleted.\n", index );

				ret++;
			}

			index = pvm_insert( CLASS_NAME, -1, 123 );

			if ( index < 0 )
			{
				pvm_perror( "pvm_insert()" );
				exit( -1 );
			}

			else
			{
				printf(
				"Value %d Successfully Inserted at %d (req=%d)\n",
					123, index, -1 );

				ret++;
			}

			cc = pvm_insert( CLASS_NAME, index, 246 );

			if ( cc < 0 )
			{
				printf(
					"Re-insert at Index=%d Failed as Expected.\n",
					index );

				ret++;
			}

			else
			{
				printf( "Uh-Oh, Re-Insert at Index=%d Succeeded!\n",
					index );
			}

			cc = pvm_lookup( CLASS_NAME, index, &tmp );

			if ( cc < 0 )
			{
				pvm_perror( "pvm_lookup()" );
				exit( -1 );
			}
	
			if ( tmp != 123 )
				printf( "Mismatch Error:  %d != %d\n", tmp, 123 );

			else
			{
				printf( "Retrieved Value is a Match.\n" );

				ret++;
			}

			if ( do_sleep )
				pvmsleep( do_sleep );

			for ( i=0 ; i < 3 ; i++ )
			{
				cc = pvm_delete( CLASS_NAME, i );

				if ( cc < 0 )
				{
					pvm_perror( "pvm_delete()" );

					printf( "No Message to Delete at Index=%d\n",
						i );
				}

				else
				{
					printf( "Message at Index=%d Deleted.\n", i );

					ret++;
				}
			}

			cc = pvm_lookup( CLASS_NAME, -1, &tmp );

			if ( cc < 0 )
			{
				pvm_perror( "pvm_lookup()" );
				exit( -1 );
			}
	
			printf( "First Available Lookup at Index=%d -> %d\n",
				cc, tmp );

			ret++;
		}

		else if ( !strcmp( cmds[0], "direct" ) )
		{
			set_flags( &flags, 0 /*do_pers*/, 1 /*do_favail*/, do_owrit,
				1 /*do_minst*/, do_rddel, do_wait );

			for ( i=0 ; i < 3 ; i++ )
			{
				sprintf( bufin,
					"This Message Left Here By Mbox (%d)", i );

				index = stuff_msg( CLASS_NAME, flags, bufin, mytid );

				if ( index < 0 )
					exit( -1 );

				cc = check_msg( CLASS_NAME, index, 0, bufout, &tmp,
					do_unpack );

				if ( cc < 0 )
					exit( -1 );
		
				if ( strcmp( bufout, bufin ) )
				{
					printf(
						"String Mismatch Error:  \"%s\"  !=  \"%s\"\n",
						bufout, bufin );
				}

				else if ( tmp != mytid )
				{
					printf( "Number Mismatch Error:  %d  !=  %d\n",
						tmp, mytid );
				}

				else
				{
					printf( "Retrieved Message is a Match.\n" );

					ret++;
				}
			}

			if ( do_sleep )
				pvmsleep( do_sleep );

			set_flags( &flags, 0 /*do_pers*/, 0 /*do_favail*/, do_owrit,
				0 /*do_minst*/, do_rddel, do_wait );

			sprintf( bufin,
				"This Message Left Here By Mbox (X)" );

			index = stuff_msg( CLASS_NAME,
				flags | PvmMboxDirectIndex( 1 ), bufin, mytid );

			if ( index < 0 )
				exit( -1 );

			for ( i=0 ; i < 3 ; i++ )
			{
				cc = check_msg( CLASS_NAME, i, 0, bufout, &tmp,
					do_unpack );

				if ( cc < 0 )
					exit( -1 );
			}
		}
	}

	if ( do_exit )
		pvm_exit();

	exit( ret );
}


void
set_flags( flags,
	do_pers, do_favail, do_owrit, do_minst, do_rddel, do_wait )
int *flags;
int do_pers;
int do_favail;
int do_owrit;
int do_minst;
int do_rddel;
int do_wait;
{
	*flags = PvmMboxDefault;

	if ( do_pers )
		*flags |= PvmMboxPersistent;

	if ( do_favail )
		*flags |= PvmMboxFirstAvail;

	if ( do_owrit )
		*flags |= PvmMboxOverWritable;

	if ( do_minst )
		*flags |= PvmMboxMultiInstance;

	if ( do_rddel )
		*flags |= PvmMboxReadAndDelete;

	if ( do_wait )
		*flags |= PvmMboxWaitForInfo;
}


void
dump_flags( flags )
int flags;
{
	if ( flags & PvmMboxPersistent )
		printf( " PERSISTENT" );

	if ( flags & PvmMboxFirstAvail )
		printf( " FIRST_AVAIL" );

	if ( flags & PvmMboxOverWritable )
		printf( " OVER_WRITABLE" );

	if ( flags & PvmMboxMultiInstance )
		printf( " MULTI_INSTANCE" );

	if ( flags & PvmMboxReadAndDelete )
		printf( " READ_AND_DELETE" );
}


int
stuff_msg( class, flags, str, num )
char *class;
int flags;
char *str;
int num;
{
	int cc;

	/* Create Message */

	pvm_initsend( PvmDataDefault );

	pvm_pkstr( str );
	pvm_pkint( &num, 1, 1 );

	/* Insert Into Class */

	cc = pvm_putinfo( class, pvm_getsbuf(), flags );

	if ( cc < 0 )
		pvm_perror( "Inserting Message" );

	else
	{
		printf( "Inserted Message Into \"%s\" at index=%d:",
			class, cc );
		dump_flags( flags );
		printf( "\n" );
		printf( "\t\"%s\"\n", str );
		printf( "\t0x%x\n", num );
	}

	pvm_freebuf( pvm_getsbuf() );

	return( cc );
}


int
check_msg( class, index, flags, str, num, do_unpack )
char *class;
int index;
int flags;
char *str;
int *num;
int do_unpack;
{
	int cc;

	/* Retrieve Message */

	cc = pvm_recvinfo( class, index, flags );

	if ( cc < 0 )
		pvm_perror( "Retrieving Message" );

	else
	{
		if ( do_unpack )
		{
			pvm_upkstr( str );
			pvm_upkint( num, 1, 1 );
		}

		printf( "Retrieved Message From \"%s\" at index=%d:",
			class, index );
		dump_flags( flags );
		printf( "\n" );

		if ( do_unpack )
		{
			printf( "\t\"%s\"\n", str );
			printf( "\t0x%x\n", *num );
		}

		else
			printf( "\t(not unpacked)\n" );
	}

	return( cc );
}


void
usage()
{
	printf( "\n" );
	printf( "usage:  mbox [pers] [favail] [owrit] [minst] [rddel]\n" );
	printf( "\t[sleep[=<secs>]] [name=<class_name>]\n" );
	printf( "\t[query [<index>]] [wait] [noreset] [noexit]\n" );
	printf( "\t[info [<pattern>]] [delete [<index>]] [direct]\n" );
	printf( "\t[default] [default33] [nindices=<n>] [help]\n" );
	printf( "\n" );

	exit( 0 );
}

