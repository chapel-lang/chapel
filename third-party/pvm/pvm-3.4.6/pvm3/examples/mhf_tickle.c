
static char rcsid[] =
	"$Id: mhf_tickle.c,v 1.5 2009/01/23 01:12:52 pvmsrc Exp $";

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
*	Filename: mhf_tickle.c
*
*  This program "tickles" the mhf_server program for testing
*  message handler functionality.  Also serves as an example
*  of how you may use and drive the message handlers...
*
*  Operation:
*	1. startup pvm console
*	2. startup the mhf_server
*	3. startup as many mhf_tickle instances as you wish
*		- follow menu options and in-code comments on
*		  how to run and what it tests / shows...
*
*  This code is dedicated to "Tickle-Me-Elmore".
*  And yes that is Elmore....
***************************************************************/

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#ifndef WIN32
#include <sys/errno.h>
#else
#include <errno.h>
#endif
#include "pvm3.h"

/* #define NOISE1 1	 sets level of program noise  - minimum noise */

#define  PMH 0		/* display present message handlers		*/
#define  LMH 1		/* load message handler				*/
#define  DMH 2		/* delete message handler			*/
#define  ERJ 3		/* tell mhf_server() to pvm_exit( ) and then rejoin. */
#define  HIT 4		/* message to hit a specified handler...	*/
#define GSMB 7		/* use to grab the mhf_server contact info from mailbox again... */
#define CEAT 8		/* client ( this program ) exit pvm and terminate self  - server not contacted... */
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
#define TRUE 1
#define FALSE 0
#define YES TRUE
#define NO FALSE
#define SUCCESS 0
#ifdef WIN32
#undef  FAILED		/* necessary on win32 because of winerror.h */
#endif
#define FAILED -1

extern int errno;


/***************************************************************/

void eat_line( c )
char c;
{
	while ( c != '\n' ) c = getchar( );	/* eat the remaining chars on the line */
} /* eat_line */


/*
*  get single character and discard remainder of input line
*/
char
getc_line( c )
char *c;
{
	scanf( "%c", c );	/* c is the address ( pointer ) for the char storage */
	eat_line( *c );
	return( *c );
} /* getc_line */


/*
* get string - no spaces
*/
char *
gets_lim_nw( max_len, s )
int max_len;
char *s;
{	char *me = "gets_lim_nw";
	char c;
	int i = 0;
	int first_char = TRUE;

	while ( ( ( c = getchar( ) ) != '\n' ) && ( i < ( max_len - 1 ) ) )
	{
		if ( !( isspace( c ) ) )			/* not a white space character */
		{
			/* printf( "\n%s: save char [%c] in position s[%d]", me, c, i );	sls-debug */
			s[i++] = c;			/* not a white space, so keep the character */
			first_char = FALSE;		/* mark having read the first non-white space char */
		}
		else					/* is a white space character */
		{
			if ( !( first_char ) )	/* other than first character to read is whitespace */
				break;		/* kick out of the read loop */
		}
	}

	s[i] = (char) NULL;

	eat_line( c );	/* eat the remaining characters on this line */

	return( s );
} /* gets_lim_nw */


/*
*  look for the user specified service name
*/
int
locate_server( service, tid, ctx, server )
char *service;
int *tid;
int *ctx;
char *server;
{	char *me = "locate_server";
	int msg_buf;


	msg_buf = pvm_recvinfo( service, 0, PvmMboxDefault );
	if ( msg_buf >= 0 ) {
		pvm_setrbuf( msg_buf );
		pvm_upkint( tid, 1, 1 );
		pvm_upkint( ctx, 1, 1 );
		pvm_upkstr( server );
	}
	else {
		/*
		 *  problem retrieving mailbox entry - tell user why, exit pvm, and terminate task...
		 */
		switch ( msg_buf ) {
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
				lpvmerr( "\nmhf_tickle: error %d", msg_buf );
				break;
		} /* end_switch */
		return( msg_buf );
	}
/*
	printf( "%s: just retrieved mailbox with stid = t%x   sctx = %d   server = %s   service = %s.\n", 
		me, *tid, *ctx, server, service );
*/
	return( msg_buf );
} /* locate_server */




/*
*  returns integer representing which message handler to load
*/
int
mhl( )
{	char *me = "mhl";
	char menu_selection;
	int got_menu_selection = NO;
	char zero = '0';

	do{
		printf( "\n" );
		printf( "\n   Load Message Handler" );
		printf( "\n   --------------------" );
		printf( "\n" );
		printf( "\n     1. string_catcher1" );
		printf( "\n     2. string_catcher2" );
		printf( "\n     3. string_catcher3" );
		printf( "\n     4. string_catcher4" );
		printf( "\n     5. string_catcher5" );
		printf( "\n     6. int_catcher6" );
		printf( "\n     7. int_catcher7" );
		printf( "\n     8. int_catcher8" );
		printf( "\n     9. int_catcher9" );
		printf( "\n     0. int_catcher10" );
		printf( "\n" );
		printf( "\n          Enter Selection: " );
		getc_line( &menu_selection );
		switch ( menu_selection ) {
			case '0': /* drop through */
			case '1': /* drop through */
			case '2': /* drop through */
			case '3': /* drop through */
			case '4': /* drop through */
			case '5': /* drop through */
			case '6': /* drop through */
			case '7': /* drop through */
			case '8': /* drop through */
			case '9': got_menu_selection = YES;
		}
	} while ( !got_menu_selection );
	return ( menu_selection-zero );
} /* mhl */


/*
*  returns integer value representing option selected ( not char value )
*/
int
mhto( )
{	char *me = "mhto";
	char menu_selection;
	int got_menu_selection = NO;
	char zero = '0';

	do{
		printf( "\n" );
		printf( "\n   Message Handler Trial Options" );
		printf( "\n   -----------------------------" );
		printf( "\n" );
		printf( "\n     0. PMH - display present message handlers." );
		printf( "\n     1. LMH - load message handler." );
		printf( "\n     2. DMH - delete message handler." );
		printf( "\n     3. ERJ - tell mhf_server() to pvm_exit( ) and then rejoin." );
		printf( "\n     4. HIT - message to hit a specified handler..." );
		printf( "\n     5.     - available." );
		printf( "\n     6.     - available." );
		printf( "\n     7. GSMB - get server from mail box again ( use if drop connection )." );
		printf( "\n     8. CEAT - client ( this program ) exit pvm and terminate self." );
		printf( "\n     9. SEAT - tell server to exit pvm and terminate self." );
		printf( "\n" );
		printf( "\n          Enter Selection: " );
		getc_line( &menu_selection );
		switch ( menu_selection ) {
			case '0':	/* drop through */
			case '1':	/* drop through */
			case '2':	/* drop through */
			case '3':	/* drop through */
			case '4':	/* drop through */
			case '7':	/* drop through */
			case '8':	/* drop through */
			case '9': got_menu_selection = YES;
		}
	} while ( !got_menu_selection );
	return ( menu_selection-zero );
} /* mhto */


/*
* get hex - move to next line
*/
int 
getx_line( i )
int *i;
{	char *me = "geti_line";
	char c[STR_LEN];	/* temporary location to hold input string to convert to integer */

	gets_lim_nw( STR_LEN, c );	/* grab the input string */
	*i =  strtol( c, ( char ** )NULL, 16 );	/* convert string to hexadecimal, bas 16 */
	if ( errno == ERANGE ) return( FAILED );
	return( SUCCESS );
} /* getx_line */


/*
* get integer - move to next line
*/
int
geti_line( i )
int *i;
{	char *me = "geti_line";
	char c[STR_LEN];	/* temporary location to hold input string to convert to integer */

	gets_lim_nw( STR_LEN, c );	/* grab the input string */
	*i =  strtol( c, ( char ** )NULL, 10 );	/* convert string to integer, base 10 */
	if ( errno == ERANGE ) return( FAILED );
	return( SUCCESS );
} /* geti_line */


/*
*  discard current input line
*/


/*
*  simple way to coordinate some timing tasks...
*/
void
press_enter_key( message )
char *message;
{	char c = (char) NULL;
	printf( "%s", message );
	eat_line( c );
} /* press_enter_key */


/*
*  get string within length limit allow only single white spaces
*/
char *
gets_lim_ows( max_len, s )
int max_len;
char *s;
{
	char c;
	int i = 0;
	int last_space = TRUE;

	while ( ( ( c = getchar( ) ) != '\n' ) && ( i < ( max_len - 1 ) ) )
	{
		if ( isspace( c ) )
		{
			c = ' ';	/* ensure that c is a char space - not other white space */
			if ( !( last_space ) )
			{			/* last char was not a space	*/
				s[i++] = c;	/* so add to string		*/
				/*printf( "\nsave char [%c]", c );	sls-debug */
			}
			last_space = TRUE;	/* mark last char read was a white space */
		}
		else
		{
			s[i++] = c;
			last_space = FALSE;	/* mark last char read was not a space */
			/*printf( "\nsave char [%c]", c );	sls-debug */
		}
	}

	/* don't permit to end on space char - so move backward to first non-white space char 		*/
	/* note that this may result in one-less than the maximum number of chars available being read 	*/
	while ( isspace( s[--i] ) ) { /* do nothing... */ }

	s[++i] = (char) NULL;

	eat_line( c );	/* eat the remaining characters on this line */

	return( s );
} /* gets_lim_ows */




/***************************************************************/
main ( argc, argv ) 
int argc;
char *argv[];
{	char *me = "mhf_tickle";
	char machine[25];
	int mytid;
	int ctx;			 /* client original context */
	int i;
	char msg_txt[STR_LEN];	/* used for sending messages */

	int stid;		/* server's tid */
	int sctx;		/* server's context */
	char *service = "mhf_server";
	char server[25];	/* machine name of server */

	int mh_src;		/* message handler source ( tid ) to talk to */
	int mh_tag;		/* message handler tag to talk to */
	int mh_ctx;		/* message handler context to talk to */
	int mhid;		/* message handler id - used to delete them */

	int option;		/* user entered option selection */
	int load_mh;		/* message handler to load */

	/*
	*  for display - note IC5 position
	*/
	static char *mh_arr[] = { "IC5", "SC1", "SC2", "SC3", "SC4", "SC5", "IC1", "IC2", "IC3", "IC4" };

	mytid = pvm_mytid( );
	ctx = pvm_getcontext( );			/* original starting context */
	gethostname( machine, 25 );
#ifdef NOISE1
	printf( "%s: t%x on machine <%s> with context %d.\n", me, mytid, machine, ctx );
#endif

	if ( locate_server( service, &stid, &sctx, server ) < 0 ){
		pvm_exit( );
		exit( -1 );
	}
#ifdef NOISE1
	printf( "%s: just retrieved mailbox with stid = t%x   sctx = %d   server = %s   service = %s.\n", me, stid, sctx, server, service );
#endif

	do{

	option = mhto( );	/* get Message Handler Trial Options */

	switch ( option ){
	case PMH:/*****
		*  display present message handlers
		*/
		printf( "\n%s: PMH - display present message handlers\n", me );
		pvm_setcontext( sctx );		/* set my context to that of server */
		sprintf( msg_txt, "Are you talking to me!!!" );
		pvm_initsend( PvmDataDefault );
		pvm_pkstr( msg_txt );
		pvm_send( stid, PMH );
		printf( "%s: sent--<%s> to:  stid = t%x   sctx = %d.\n", me, msg_txt, stid, sctx );fflush( stdout );
		break;

	case LMH:/*****
		*  load message handler, ( src=tid_of_sender, tag, ctx )
		*/
		printf( "\n%s: client:  src=tid = t%x  ctx = %d.", me, mytid, ctx );
		printf( "\n%s: server:  src=tid = t%x  ctx = %d.\n", me, stid, sctx );
		printf( "\n%s: LMH - load message handler", me );
		pvm_setcontext( sctx );		/* set my context to that of server */
		pvm_initsend( PvmDataDefault );

		mh_src = mytid;		/* can't spoof a source tid - must use existing one... */
		printf( "\n%s: message source tid: t%x\n", me, mh_src );

		pvm_pkint( &mh_src, 1, 1 );

		printf( "%s: message source tag: ", me );
		if ( geti_line( &mh_tag ) != SUCCESS ){
			printf( "\n%s: problem on source tag - try again...\n", me );
			break;
		}
		pvm_pkint( &mh_tag, 1, 1 );

		printf( "%s: message source ctx: ", me );
		if ( geti_line( &mh_ctx ) != SUCCESS ){
			printf( "\n%s: problem on context input - try again...\n", me );
			break;
		}
		pvm_pkint( &mh_ctx, 1, 1 );

		load_mh = mhl( );

		pvm_pkint( &load_mh, 1, 1 );	/* handler to load in mhf_server( ) code */
		printf( "\n%s: will attempt loading message handler %s with src = t%x = %d,  tag = %d, ctx = %d\n", 
			me, mh_arr[load_mh], mh_src, mh_src, mh_tag, mh_ctx );
		pvm_send( stid, LMH );
		break;

	case DMH:/*****
		*  delete message handler
		*/
		printf( "\n%s: DMH - delete message handler", me );
		pvm_setcontext( sctx );		/* set my context to that of server */
		printf( "\n%s: delete message handler id: ", me );
		if ( geti_line( &mhid ) != SUCCESS ){
			printf( "\n%s: problem on message handler id input - try again...\n", me );
			break;
		}
		pvm_initsend( PvmDataDefault );
		pvm_pkint( &mhid, 1, 1 );
		pvm_send( stid, DMH );
		break;

	case ERJ:/*****
		*  tell mhf_server() to pvm_exit( ) and then rejoin. ->> check message handlers existance
		*/
		printf( "\n%s: ERJ - tell mhf_server() to pvm_exit( )", me );
		pvm_setcontext( sctx );		/* set my context to that of server */
		pvm_initsend( PvmDataDefault );
		pvm_send( stid, ERJ );
		/*
		*  must relocate the server as past info is now stale
		*/
		printf( "%s: Looking for service = %s.\n", me, service );
		press_enter_key( "Press Enter key when server is ready to continue..." );
		if ( locate_server( service, &stid, &sctx, server ) < 0 ){
			pvm_exit( );
			exit( -1 );
		}
		printf( "%s: just retrieved mailbox with stid = t%x   sctx = %d   server = %s   service = %s.\n",
			me, stid, sctx, server, service );
		break;

	case HIT:/*****
		* message to hit a specified handler...
		*/
		printf( "\n%s: HIT - message to hit a specified handler...", me );

		printf( "\n%s: set message tag: ", me );
		if ( geti_line( &mh_tag ) != SUCCESS ){
			printf( "\n%s: problem on source tag - try again...\n", me );
			break;
		}

		printf( "%s: message source ctx: ", me );
		if ( geti_line( &mh_ctx ) != SUCCESS ){
			printf( "\n%s: problem on context input - try again...\n", me );
			break;
		}

		pvm_setcontext( mh_ctx );		/* set my context to that which handler expects */

		load_mh = mhl( );		/* select handler to "hit" */
		pvm_initsend( PvmDataDefault );
		pvm_pkint( &load_mh, 1, 1 );	/* integer indicates which trying to ping */
		switch ( load_mh ) {
			case 1: case 2: case 3: case 4: case 5:
				printf( "\n%s: Enter message to ping message handler string_catcher%d: ",me, load_mh );
				gets_lim_ows( STR_LEN, msg_txt );
				printf( "\n%s: will ping %s:string_catcher%d with message <%s>.\n", me, mh_arr[load_mh], load_mh, msg_txt );
				pvm_pkstr( msg_txt );
				break;

			case 0: case 6: case 7: case 8: case 9:
				printf( "\n%s: Enter integer to ping message handler int_catcher%d: ",me, load_mh );
				geti_line( &i );
				printf( "\n%s: will ping %s:int_catcher%d with integer <%d>.\n", me, mh_arr[load_mh], load_mh, i );
				pvm_pkint( &i, 1, 1 );
				break;

			default: /* should never get here... */
				printf( "\n\n%s: mhl( ) returned value <%d> out of bounds...\n\n", me, load_mh );
				pvm_exit( );
				exit( -1 );
				break;
		}
		pvm_send( stid, mh_tag );		/* the send that the mhf should intercept */

		/*
		*  now tickle the mhf_server so that it knows that a 
		*  message should have been intercepted on its behalf
		*/
		pvm_setcontext( sctx );		/* set my context to that of server */
		pvm_initsend( PvmDataDefault );
		pvm_pkint( &load_mh, 1, 1 );	/* integer indicates which trying to ping */
		switch ( load_mh ) {
			case 1: case 2: case 3: case 4: case 5:
				pvm_pkstr( msg_txt );
				break;
			case 0: case 6: case 7: case 8: case 9:
				pvm_pkint( &i, 1, 1 );
				break;
		}
		pvm_send( stid, HIT );			/* the send to "tickle" mhf_server with */
		break;

	case GSMB:/*****
		*  use to grab the mhf_server contact info from mailbox again...
		*  No actual contact with mhf_server - only its entry into mailbox
		*
		*  Necessary if another instance of mhf_tickle does a ERJ on you...
		*/
		if ( locate_server( service, &stid, &sctx, server ) < 0 ){
			pvm_exit( );
			exit( -1 );
		}
#ifdef NOISE1
		printf( "%s: just retrieved mailbox with stid = t%x   sctx = %d   server = %s   service = %s.\n", me, stid, sctx, server, service );
#endif
		break;

	case SEAT:/*****
		*  server exits pvm and terminate self
		*/
		printf( "\n%s: SEAT - server exits pvm and terminate self\n\n", me );
		pvm_setcontext( sctx );		/* set my context to that of server */
		pvm_initsend( PvmDataDefault );
		pvm_send( stid, SEAT );
		pvm_exit( );
		exit( 0 );
		break;

	case CEAT:/*****
		*  client ( this program ) exits pvm and terminate self - server not contacted...
		*/
		printf( "\n%s: CEAT - client ( this program ) exits pvm and terminate self\n\n", me );
		pvm_exit( );
		exit( 0 );
		break;

	default:/*******
		*  invalid msgtag - just ignore the message
		*/
		printf( "\n%s: default - junk input - try again...", me );
			break;
	}
	pvm_setcontext( ctx );		/* reset context to that of client program mhf_tickle( ) */
	} while ( ( option != SEAT ) && ( option != CEAT ) );



	pvm_exit( );
	exit( 0 );
} /* end_main */

