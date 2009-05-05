
static char rcsid[] =
	"$Id: mhf_server.c,v 1.5 2009/01/23 01:12:52 pvmsrc Exp $";

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

/***************************************************************
*	Filename: mhf_server.c
*
*  This is a "server" program for testing message handler functions.
*  Use the mhf_tickle program to speak to this server.
*
*  Operation:
*	1. startup pvm console
*	2. startup the mhf_server
*	3. startup as many mhf_tickle instances as you wish
*		- follow menu options and in-code comments on
*		  how to run and what it tests / shows...
*
***************************************************************/

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

#define NOISE1 1	 /* sets level of program noise  - minimum noise */
/* #define NOISE2 2	 each level "up" adds noise  */

#define  PMH 0		/* display present message handlers		*/
#define  LMH 1		/* load message handler				*/
#define  DMH 2		/* delete message handler			*/
#define  ERJ 3		/* tell mhf_server() to pvm_exit() and rejoin */
#define  HIT 4		/* message to hit a specified handler...	*/
#define GSMB 7		/* NOT USED IN mhf_server() - use to grab the */
					/* mhf_server contact info from mailbox again... */
#define CEAT 8		/* client (this program ) exit pvm and terminate */
					/* self  - server not contacted... */
#define SEAT 9		/* server exits pvm and terminate self		*/

#define SC1 1		/* string_catcher1 */
#define SC2 2		/* string_catcher2 */
#define SC3 3		/* string_catcher3 */
#define SC4 4		/* string_catcher4 */
#define SC5 5		/* string_catcher5 */
#define IC1 6		/* int_catcher6 */
#define IC2 7		/* int_catcher7 */
#define IC3 8		/* int_catcher8 */
#define IC4 9		/* int_catcher9 */
#define IC5 0		/* int_catcher10 */

#define STR_LEN 100
#define YES 1
#define NO 0

/*
*  globals to bring back message handler information
*/
int mhf_catch;		/* should be same as number of "catcher" hit */
int mhf_magic_number;
char mhf_msg[STR_LEN];


/**********************************************************************/
/**********************************************************************/
/**********************************************************************
*  register this server via "well known" mailbox so others may contact us
*  mailbox name is set to...  server = "mhf_server"
*
*  Returns mailbox index where placed
*/
int
register_server( mytid, ctx, machine, server )
int mytid;
int ctx;
char *machine;
char *server;
{	char *me = "register_server";
	int index;

	pvm_initsend( PvmDataDefault );
	pvm_pkint( &mytid, 1, 1 );
	pvm_pkint( &ctx, 1, 1 );
	pvm_pkstr( machine );
	if ( ( index = pvm_putinfo( server, pvm_getsbuf( ), PvmMboxDefault ) ) < 0 ) {
		/*
		 *  problem inserting mailbox entry - tell user why, exit pvm, and terminate task...
		 */
		switch ( index ) {
			case PvmExists:
				printf( "\n%s: Named service already running\n", me );
				break;
			case PvmBadParam:
				printf( "\n%s: Invalid argument to pvm_recvinfo( ).\n", me );
				break;
			case PvmNoSuchBuf:
				printf( "\n%s: Message buffer id does not exist.\n", me );
				break;
			case PvmDenied:
				printf( "\n%s: Key locked by another task, can't delete.\n", me );
				break;
			case PvmNoMem:
				printf( "\n%s: Libpvm is unable to allocate memory to pack data.\n", me );
				break;
			case PvmNotFound:
				printf( "\n%s: Requested key does not exist.\n", me );
				break;
			default:
				lpvmerr( "\n%s: task0.c: error %d", me, index );
				break;
		} /* end_switch */
		return( index );		/* returns error code */
	}
	printf( "%s: mailbox... just inserted server <%s> at mailbox %d with context %d.\n", 
		me, server, index, ctx );
	return( index );
} /* register_server */


/**********************************************************************
*  Function:	string_catcher1		- unpack 1 string
*		string_catcher2		- unpack 2 strings
*		string_catcher3		- unpack 3 strings
*		string_catcher4		- unpack 4 strings
*		string_catcher5		- unpack 5 strings
*
*		int_catcher6		- unpack 1 integer
*		int_catcher7		- unpack 2 integers
*		int_catcher8		- unpack 3 integers
*		int_catcher9		- unpack 4 integers
*		int_catcher10		- unpack 5 integers
*
*  These are all simple message handlers to use in testing loading,
*  deleting, and using message handlers.
*
*  Each one simply echos out the information that is being sent.
*  Thus showing that a given handler may "handle" messages of a given type.
**********************************************************************/
int
string_catcher1( mid )
int mid;
{	char *me = "string_catcher1";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkstr( mhf_msg );
	printf( "\n%s: string_catcher%d received message -->> <%s>\n\n", me, mhf_catch, mhf_msg );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* string_catcher1 */

int
string_catcher2( mid )
int mid;
{	char *me = "string_catcher2";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkstr( mhf_msg );
	printf( "\n%s: string_catcher%d received message -->> <%s>\n\n", me, mhf_catch, mhf_msg );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* string_catcher2 */

int
string_catcher3( mid )
int mid;
{	char *me = "string_catcher3";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkstr( mhf_msg );
	printf( "\n%s: string_catcher%d received message -->> <%s>\n\n", me, mhf_catch, mhf_msg );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* string_catcher3 */

int
string_catcher4( mid )
int mid;
{	char *me = "string_catcher4";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkstr( mhf_msg );
	printf( "\n%s: string_catcher%d received message -->> <%s>\n\n", me, mhf_catch, mhf_msg );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* string_catcher4 */

int
string_catcher5( mid )
int mid;
{	char *me = "string_catcher5";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkstr( mhf_msg );
	printf( "\n%s: string_catcher%d received message -->> <%s>\n\n", me, mhf_catch, mhf_msg );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* string_catcher5 */

int
int_catcher6( mid )
int mid;
{	char *me = "int_catcher6";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkint( &mhf_magic_number, 1, 1 );
	printf( "\n%s: int_catcher%d received magic number -->> %d.\n\n", me, mhf_catch, mhf_magic_number );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* int_catcher6 */

int
int_catcher7( mid )
int mid;
{	char *me = "int_catcher7";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkint( &mhf_magic_number, 1, 1 );
	printf( "\n%s: int_catcher%d received magic number -->> %d.\n\n", me, mhf_catch, mhf_magic_number );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* int_catcher7 */

int
int_catcher8( mid )
int mid;
{	char *me = "int_catcher8";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkint( &mhf_magic_number, 1, 1 );
	printf( "\n%s: int_catcher%d received magic number -->> %d.\n\n", me, mhf_catch, mhf_magic_number );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* int_catcher8 */

int
int_catcher9( mid )
int mid;
{	char *me = "int_catcher9";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkint( &mhf_magic_number, 1, 1 );
	printf( "\n%s: int_catcher%d received magic number -->> %d.\n\n", me, mhf_catch, mhf_magic_number );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* int_catcher9 */

int
int_catcher10( mid )
int mid;
{	char *me = "int_catcher10";
	int rbf;
	rbf = pvm_setrbuf( mid );

	pvm_upkint( &mhf_catch, 1, 1 );
	pvm_upkint( &mhf_magic_number, 1, 1 );
	printf( "\n%s: int_catcher%d received magic number -->> %d.\n\n", me, mhf_catch, mhf_magic_number );

	pvm_freebuf( pvm_setrbuf( rbf ) );
	return 0;
} /* int_catcher10 */
/**********************************************************************/
/**********************************************************************/


/**********************************************************************/
main ( argc, argv ) 
int argc;
char *argv[];
{	char *me = "mhf_server";
	int cc;			/* local stuff */
	int mytid;		/* tid of this server */
	int ctid;		/* tid of request sender - to this server */
	int msgtag;		/* sender specified message tag */
	int length;		/* length of message */
	int sctx;		/* server's context */
	int info;		/* status return and temporary input values */
	int mhid;		/* message handler id */
	int lmh;		/* int represent local message handler to ping */
	int index;		/* mailbox entry index */
	char msg[STR_LEN];	/* used for local info messages */
	char msg_txt[STR_LEN];	/* used for received messages */
	char machine[25];
	int FOREVER = YES;

	/*
	*  for display - note IC5 position
	*/
	static char *mh_arr[] = { "IC5", "SC1", "SC2", "SC3", "SC4", "SC5", "IC1",
			"IC2", "IC3", "IC4" };

	/* expected message information coming from "tickle" program */
	char action_tag;	/* action for server to take -> p, l, d, x j */
	int src;		/* message source = sender's tid */
	int tag;		/* message tag */
	int ctx;		/* message context */

	if ( ( mytid = pvm_mytid( ) ) < 0 ){
		printf( "\n%s: try starting pvm first...\n", me );
		exit( -1 );
	}

	gethostname( machine, 25 );
	sctx = pvm_newcontext( );		/* get new context */
	pvm_setcontext( sctx );			/* activate new context */
	printf( "%s: t%x on machine <%s> with context %d.\n",
			me, mytid, machine, pvm_getcontext( ) );
	if ( register_server( mytid, sctx, machine, me ) < 0 ){
		pvm_exit( );
		exit( -1 );
	}

	pvmdisplaymhfinfo( me, "startup...", mytid );

	/*
	*  startup message "server" to wait for messages
	*  leave when receive the "END" message
	*
	*  Remember: message handlers are keying on specific
	*	source - tag - context --> tuples...
	*/
	while( FOREVER )
	{
		if ( ( cc = pvm_recv( -1, -1 ) ) < 0 ){		/* receive all */
			lpvmerr( "\nmhf_server: pvm_recv( ): error %d", cc );
			pvm_exit( ); exit( 0 );
		}
		/* who sent message */
		if ( (info = pvm_bufinfo( cc, &length, &msgtag, &ctid )) < 0 ){
			lpvmerr( "\nmhf_server: pvm_bufinfo( ): error %d", info );
			pvm_exit( ); exit( 0 );
		}

		switch ( msgtag ){
		case PMH:  /* display present message handlers */
				pvm_upkstr( msg_txt );
				sprintf( msg, "%s: PMH: %s", me, msg_txt );
				pvmdisplaymhfinfo( me, msg, mytid );
				break;

		/* load message handler */
		/* ( src=tid_of_sender ...usually..., tag, ctx ) */
		case LMH:
				/* tid of msg handler ONLY user - unless wildcard -1 */
				pvm_upkint( &src, 1, 1 );
				pvm_upkint( &tag, 1, 1 );
				pvm_upkint( &ctx, 1, 1 );
				/* number indicates which handler to load */
				pvm_upkint( &info, 1, 1 );
#ifdef NOISE2
				sprintf( msg,
					"%s: LMH: BEFORE load with pvm_addmhf( src = t%x, tag = %d, ctx = %d )  -> load %s",
					me, src, tag, ctx, mh_arr[info] );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				switch ( info ){
				case SC1:
					mhid = pvm_addmhf( src, tag, ctx, string_catcher1 );
					break;
				case SC2:
					mhid = pvm_addmhf( src, tag, ctx, string_catcher2 );
					break;
				case SC3:
					mhid = pvm_addmhf( src, tag, ctx, string_catcher3 );
					break;
				case SC4:
					mhid = pvm_addmhf( src, tag, ctx, string_catcher4 );
					break;
				case SC5:
					mhid = pvm_addmhf( src, tag, ctx, string_catcher5 );
					break;
				case IC1:
					mhid = pvm_addmhf( src, tag, ctx, int_catcher6 );
					break;
				case IC2:
					mhid = pvm_addmhf( src, tag, ctx, int_catcher7 );
					break;
				case IC3:
					mhid = pvm_addmhf( src, tag, ctx, int_catcher8 );
					break;
				case IC4:
					mhid = pvm_addmhf( src, tag, ctx, int_catcher9 );
					break;
				case IC5:
					mhid = pvm_addmhf( src, tag, ctx, int_catcher10 );
					break;
				default:
					printf( "\n%s: no such message handler <%d> to load...  Only 0 - 9 available!", me, info );
					mhid = PvmBadParam;
					break;
				} /* end_switch ( info ) */

				if ( mhid == PvmExists ){
					printf( "\n%s: message handler already exists...\n", me );
					break;
				}
				if ( mhid < 0 ){
					printf( "\n%s: pvm_addmhf failed with error code %d.\n", me, mhid );
					break;
				}
#ifdef NOISE1
				sprintf( msg, 
					"%s: LMH: AFTER load with pvm_addmhf( src = t%x, tag = %d, ctx = %d )  -> loaded %s", 
					me, src, tag, ctx, mh_arr[info] );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				break;

		case DMH:  /* delete message handler */
				pvm_upkint( &mhid, 1, 1 );
#ifdef NOISE2
				sprintf( msg, "%s: DMH: BEFORE delete with pvm_delmhf( mhid = %d )", me, mhid );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				if ( ( info = pvm_delmhf( mhid ) ) < 0 ){
					sprintf( msg,"%s: pvm_delmhf failed - no such handler existed.", me );
					lpvmerr( msg, PvmNotFound );
					break;
				}
				sprintf( msg, "%s: DMH: AFTER delete with pvm_delmhf( mhid = %d )", me, mhid );
				pvmdisplaymhfinfo( me, msg, mytid );
				break;

		case ERJ:  /* tell mhf_server() to pvm_exit( ) and then rejoin ->> check message handlers existance */
#ifdef NOISE2
				sprintf( msg, "%s: ERJ: BEFORE pvm_exit( )", me );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				/*
				*  exit pvm part...
				*/
				if ( ( info = pvm_exit( ) ) < 0 ){
					printf( "\n%s: pvm_exit failed with error code %d.\n", me, info );
					break;
				}
#ifdef NOISE1
				printf( "\n%s: Just did pvm_exit...\n", me );
				sprintf( msg, "%s: ERJ: AFTER pvm_exit( )", me );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				/*
				*  rejoin pvm part...
				*/
				if ( ( mytid = pvm_mytid( ) ) < 0 ){
					printf( "\n%s: could not restart - pvm not up...\n", me );
					exit( -1 );
				}

#ifdef NOISE1
				printf( "%s: just rejoined pvm...\n", me );
				printf( "%s: restarted as t%x on machine <%s> with context %d.\n", me, mytid, machine, pvm_getcontext( ) );
				sprintf( msg, "%s: ERJ: rejoin pvm AFTER pvm_exit( )", me );
				pvmdisplaymhfinfo( me, msg, mytid );
#endif
				/*
				*  re-register server via mailbox...
				*/
				if ( ( info = register_server( mytid, sctx, machine, me ) ) < 0 ){
					printf( "\n%s: register_server( ) failed with error code %d.\n", me, info );
					pvm_exit( );
					exit( -1 );
				}
#ifdef NOISE1
				printf( "%s: just registered server...\n", me );
#endif
				break;

		case HIT:  /* message to hit a specified handler... */

				pvm_upkint( &lmh, 1, 1 );	/* integer indicates which target trying to ping */

				if ( lmh == mhf_catch )
					printf( "\n%s: HIT:  ping target %d - %s   ", me, lmh, mh_arr[lmh] );
				else
					printf( "\n%s: NO HIT:  lmh = %d - %s   mhf_catch = %d - ?C?   ", 
						me, lmh, mh_arr[lmh], mhf_catch );

				switch ( lmh ) {
				case 1: case 2: case 3: case 4: case 5:
					pvm_upkstr( msg_txt );
					if ( strcmp( msg_txt, mhf_msg ) == 0 )
						printf( "strings match <%s>.\n", msg_txt );
					else
						printf( "strings NO match - local <%s>   mhf <%s>.\n", msg_txt, mhf_msg );
					break;
				case 0: case 6: case 7: case 8: case 9:
					pvm_upkint( &cc, 1, 1 );
					if ( cc == mhf_magic_number )
						printf( "magic_numbers match %d.\n", cc );
					else
						printf( "magic_numbers NO match - local %d   mhf %d.\n", cc, mhf_magic_number );
					break;
				}

				sprintf( msg, "%s: HIT:", me );
				pvmdisplaymhfinfo( me, msg, mytid );

				/*
				*  reset globals to some known values for testing
				*/
				mhf_catch = -99;
				mhf_magic_number = -99;
				strcpy ( mhf_msg, "NO MATCH" );
				break;

		case SEAT:  /* server exits pvm and terminate self */
				sprintf( msg, "%s: CEAT: Termiate Self...", me );
				pvmdisplaymhfinfo( me, msg, mytid );
				FOREVER = NO;				/* will cause program termination */
				break;

		default:/* invalid msgtag - just ignore the message
			*/
			printf( "%s: default: INVALID msgtag:   length = %d     msgtag = <%d>     ctid = t%x", 
				me, length, msgtag, ctid );
			break;
		} /* switch_end */
		fflush( stdout );
	} /* end_while_FOREVER */
	pvm_exit( );
	exit( 0 );
} /* end_main */

