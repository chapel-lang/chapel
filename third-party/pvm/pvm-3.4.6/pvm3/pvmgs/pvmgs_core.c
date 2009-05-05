
static char rcsid[] =
	"$Id: pvmgs_core.c,v 1.6 1997/10/22 22:14:13 pvmsrc Exp $";

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

/* This is the core of the group server. Everything is now dynamically
   allocated keep memory cost down and allow larger groups.  */
/*
 *	pvmgs.c
 *	PVM group server
 *
 *	This program maintains group lists.  Processes may join or add
 *	by sending the appropriate request to the group server.
 *	
 *	4 Mar 1993 adamb: Fixed gs_barrier to reset the barrier 
 *		after it's been reached
 *	8 Jul 1993 adamb: Fixed gs_register'pvm_delete so it won't
 *		print bogus error messages
 *	6 Feb 1994 adamb: Added DEADTID functionality, including removing
 *		a taskid from the blocking barrier list if needed
 *     24 Apr 1994 Donato: Added gs_tidlist() in exchange for gs_bcast()
 *      1 May 1994 Papadopoulos: removed length limit for groupnames
 *
*/



#include <stdio.h>
#include "pvm3.h"
#include "pvmalloc.h"
#include "pvmgsd.h"
#include "pvmproto.h"

/* ================ main() ========================================= */
/* Main program for the group server                                 */
int
main( argc, argv )
int argc;
char *argv[];
{
	GROUP_LIST hash_list[HASHSIZE];    /* open hashing table */
	int ngroups, gstid;
	int nameindex;                     /* index of pvmgs-defnd names */

	/* attach to the virtual machine */
	if( (gstid = pvm_mytid()) < 0 ) 
	{
		pvm_perror( GSNAME );
		return( -1 );
	}

	/* register this group server */
	if ( !gs_register( GSNAME, gstid ) ) 
	{
		/* set default context for system dynamic groups */
		pvm_setcontext( SYSCTX_DG );

		/* initialize the data structures */
		gs_init( hash_list, &ngroups );

		/* handle the group server functions */
		gs_handle( hash_list, &ngroups, &gstid );
	}

	pvm_exit();

	return( PvmOk );
}

/* ================ gs_init() ====================================== */
/* Initialize the data structures for holding group data             */
int 
gs_init( hash_list, ngroups )
GROUP_LIST_PTR hash_list; 
int *ngroups;
{
	int i;
	*ngroups = 0;
	gs_hash_init( hash_list );
	return( PvmOk );
}

/* ================ gs_deadtid() =================================== */
/* Called when a tid has left the VM ands needs to be deleted from   */
/* all existing groups                                               */
int
gs_deadtid( tid, hash_list, ngroups )
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	GROUP_LIST_PTR current;
	GROUP_LIST_PTR next;
	int i;

	for ( i=0; i < HASHSIZE ; i++ ) /* go thru entire list of groups */
	{
		current = hash_list+i;
		current = current->next;
		while ( current != (GROUP_LIST_PTR) NULL )
		{
			next = current->next;	/* call to gs_leave may free */
									/* struct pointed to by current */
			gs_leave( current->group->name, tid, hash_list, ngroups );
			current = next;
		}
	}

	return( PvmOk );
}

/* ================ gs_pstate () =================================== */
/* Dump the information held by the group server                     */
int
gs_pstate( name, hash_list, ngroups )
char *name;
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	GROUP_LIST_PTR current;
	GROUP_STRUCT_PTR group;
	int i, j;

	fprintf( stderr, "%s 0x%x, %d groups:\n",
			name, pvm_mytid(), *ngroups);

	for ( i=0 ; i < HASHSIZE ; i++ ) /* go thru entire list of groups */
	{
		current = hash_list + i;
		current = current->next;
		while ( current != (GROUP_LIST_PTR) NULL )
		{
			group = current->group;
			fprintf( stderr, "group: %s  size: %d ", 
					group->name, group->ntids );
			fprintf( stderr, "barrier_count: %d barrier_reached %d\n ", 
					group->barrier_count, group->barrier_reached );
			fprintf( stderr, "sgroup_count: %d sgroup_reached %d\n ", 
					group->sgroup_count, group->sgroup_reached );
			fprintf( stderr, " Tids: \n" );
			for ( j=0 ; j < group->maxntids ; j++ )
				if ( group->tids[j] != NOTID )
					fprintf( stderr, "(%d,  0x%x) ",
							j, group->tids[j] );
			fprintf( stderr, "\n" );
			if ( group->barrier_reached > 0 )
			{
				fprintf( stderr, "tids waiting on barrier:\n" );
				for ( j=0 ; j < group->barrier_reached ; j++ )
					fprintf( stderr, "0x%x  ", group->btids[j] );
				fprintf( stderr, "\n"	);
			}
			if ( group->sgroup_reached > 0 )
			{
				fprintf( stderr,
						"tids waiting on static formation:\n" );
				if ( group->stids != (int *) NULL )
					for ( j=0 ; j < group->sgroup_reached ; j++ )
						fprintf( stderr, "0x%x  ", group->stids[j] );
				fprintf( stderr, "\n" );
			}
			if ( group->nhosts > 0 )
			{
				fprintf( stderr, "Host characterization:\n" );
				for ( j=0 ; j < group->nhosts ; j++ )
					fprintf( stderr, "host %x - %d procs, %x coord \n",
							pvm_tidtohost( group->pcoord[j] ), 
							group->np_onhost[j], group->pcoord[j] ); 
			}
			current = current->next;
		}
	}

	return( PvmOk );
}

/* ================ gs_ls () ======================================= */
/* Pack up a message that lists the information held by the group    */
/* server                                                            */
int
gs_ls( hash_list, ngroups )
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	GROUP_LIST_PTR current;
	GROUP_LIST_PTR next;
	GROUP_STRUCT_PTR group;
	int i, j;

	pvm_initsend( PvmDataDefault );
	pvm_pkint( ngroups, 1, 1 );
	for ( i=0 ; i < HASHSIZE ; i++ ) /* go thru entire list of groups */
	{
		current = hash_list + i;
		current = current->next;
		while ( current != (GROUP_LIST_PTR) NULL )
		{
			group = current->group;
			pvm_pkint( &(group->len), 1, 1 );
			pvm_pkstr( group->name );
			pvm_pkint( &(group->ntids), 1, 1 );
			pvm_pkint( &(group->maxntids), 1, 1 );
			pvm_pkint( &(group->barrier_count), 1, 1 );
			pvm_pkint( &(group->barrier_reached), 1, 1 );
			pvm_pkint( group->tids,group->maxntids, 1 );
			if ( group->barrier_reached > 0 )
				pvm_pkint( group->btids, group->barrier_reached, 1 ); 
			current = current->next;
		}
	}

	return( PvmOk );
}

/* ================ gs_handle () =================================== */
/* Handle the incoming msg tags and call the appropriate operation   */
int
gs_handle( hash_list, ngroups, gstid )
GROUP_LIST_PTR hash_list;
int *ngroups, *gstid;
{
	int len, msgtag, tid, gid, cc, ftid, inst, i, size, ntids, listsize;
	int cnt, hosttid, nhosts, nmem_onhost, pcoord;
	int *nmem_onhostv = (int *) NULL, *pcoordv = (int *) NULL; 
	int mxupklen = 0;
	GROUP_STRUCT_PTR group;
	char *groupname = (char *) NULL;
	int *tidlist = (int *) NULL;
	int maxntids = 0, info;
	int gstate;

	while ( 1 )  
	{
		if ( (cc = pvm_recv( -1, -1 )) < 0 )  /* receive a request */ 
		{
			pvm_perror( "gs_handle(recv)" );
			return( cc );
		}
		/* get: length of message, message tag, and sending tid */
		if ( (cc = pvm_bufinfo( pvm_getrbuf(), &len, &msgtag, &tid ))
				< 0 ) 
		{
			pvm_perror("gs_handle(bufinfo)");
			return( cc );
		}

		/* Most group functions require unpacking a group name, so */
		/* Make sure variable groupname is always long enough */
		REALLOCSTRING( len, mxupklen , groupname, "gs_handle(entry)" );

		switch( msgtag ) 
		{
			case (DIE):		/* leave a suicide note */
				gs_pstate( "pvmgs", hash_list, ngroups );
				return( PvmOk );

			case (JOIN):	/* join a group with the lowest avail gid */
				pvm_upkstr( groupname );
				gid = gs_join( groupname, tid, hash_list, ngroups );
				/* tell me if the tid dies */
				if ( pvm_notify( PvmTaskExit, DEADTID, 1, &tid ) < 0 ) 
					pvm_perror( "pvmgs-join" );
				SENDINTRESULT( gid, tid, msgtag, "gs_handle(join)" );
				break;

			case (LEAVE):	/* leave a group */
				pvm_upkstr( groupname );
				cc = gs_leave( groupname, tid, hash_list, ngroups );
				SENDINTRESULT( cc, tid, msgtag, "gs_handle(leave)" );
				break;

			case (DEADTID):	/* task in one or more groups has died */ 
				pvm_upkint( &tid, 1, 1 );
				if ( tid < 0 ) 
				{
					fprintf( stderr, "pvmgs: weird dead tid x%x %d\n",
							tid, tid );
					break;
				}
				cc = gs_deadtid( tid, hash_list, ngroups );
				break;

			case (BARRIER):	/* Handle barrier for a group */ 
			case (BARRIERV):
				pvm_upkstr( groupname );
				pvm_upkint( &cnt, 1, 1 );
				cc = gs_barrier( groupname, msgtag,
						cnt, tid, hash_list, ngroups );
				/* send back a response only if there is an error */
				if ( cc < 0 ) 
					SENDINTRESULT( cc, tid, msgtag,
							"gs_handle(barrier)" );
				break;

			case (BCAST):	/* broadcast and tidlists */
			case (TIDLIST): 
				pvm_upkstr( groupname );
				cc = gs_tidlist( groupname, ((msgtag == BCAST) ? 0 : 1),
						hash_list, ngroups, &gstate ); 
				if ( cc )  /* there was error, send back error code */
					SENDINTRESULT( cc, tid, msgtag, "gs_handle(bcast)" )
				else  /* send the already-packed tid-list */
				{
					PK_STATE( tid, gstate, groupname, hash_list,
							ngroups, "gs_handle(bcast)" );
					SENDRESULT( tid, msgtag, "gs_handle(bcast)" );
				}
				break;

			case (GSIZE):	/* return the current group size */
				pvm_upkstr( groupname );
				size = gs_gsize( groupname, hash_list, ngroups,
						&gstate );
				PK_IRESULT( size, "gs_handle(gsize)" );
				PK_STATE( tid, gstate, groupname, hash_list, ngroups,
						"gs_handle(gsize)" );
				SENDRESULT( tid, msgtag, "gs_handle(gsize)" );
				break;

			case (GETINST):	/* return inst of tid in group groupname */
				pvm_upkstr( groupname );
				pvm_upkint( &ftid, 1, 1 );
				inst = gs_getinst( groupname, ftid, hash_list, ngroups,
						&gstate );
				PK_IRESULT( inst, "gs_handle(getinst)" );
				PK_STATE( tid, gstate, groupname, hash_list, ngroups,
						"gs_handle(getinst)" );
				SENDRESULT( tid, msgtag, "gs_handle(getinst)" );
				break;

			case (GETTID):	/* return tid from (group,instance) pair */
				pvm_upkstr( groupname );
				pvm_upkint( &inst, 1, 1 );
				ftid = gs_gettid( groupname, inst, hash_list, ngroups,
						&gstate );
				PK_IRESULT( ftid, "gs_handle(gettid)" );
				PK_STATE( tid, gstate, groupname, hash_list, ngroups,
						"gs_handle(gettid)" );
				SENDRESULT( tid, msgtag, "gs_handle(gettid)" );
				break;

			case (STATICGROUP):	/* Form a static group */
				pvm_upkstr( groupname );
				pvm_upkint( &size, 1, 1 );
				info = gs_static( groupname, size, tid, hash_list,
						ngroups );
				if ( info < 0 )
					SENDINTRESULT( info, tid, msgtag,
							"gs_handle(static)" );
				/* gs_static will mcast list to group when formed */
				break;

			case DUMP:
				gs_pstate( "DUMP", hash_list, ngroups );
				break;

			case GSLS: 
				gs_ls( hash_list, ngroups );
				pvm_send( tid, msgtag );
				break;

			case HOSTCHAR:	/* get host characteristics */
				pvm_upkstr( groupname );
				pvm_upkint( &hosttid, 1, 1 );
				gs_host_char( groupname, hash_list, ngroups, hosttid, 
						&pcoord, &nmem_onhost, &nhosts, &gstate );
				PK_IRESULT( nhosts, "gs_handle(hostchar)" );
				pvm_pkint( &nmem_onhost, 1, 1 );
				pvm_pkint( &pcoord, 1, 1 ); 
				PK_STATE( tid, gstate, groupname, hash_list, ngroups,
						"gs_handle(hostchar)" );
				SENDRESULT( tid, msgtag, "gs_handle(hostchar)" );
				break; 

			case HOSTCHARV:	/* get host characteristics */
				pvm_upkstr( groupname );
				gs_host_all( groupname, hash_list, ngroups, &pcoordv, 
						&nmem_onhostv, &nhosts, &gstate );
				PK_IRESULT( nhosts, "gs_handle(hostchar)" );
				pvm_pkint( nmem_onhostv, nhosts, 1 );
				pvm_pkint( pcoordv, nhosts, 1 ); 
				PK_STATE( tid, gstate, groupname, hash_list, ngroups,
						"gs_handle(hostchar)" );
				SENDRESULT( tid, msgtag, "gs_handle(hostchar)" );
				break;

			default:
				break;
		}  /* Switch - statement */
	} /* While - statement */
} 

