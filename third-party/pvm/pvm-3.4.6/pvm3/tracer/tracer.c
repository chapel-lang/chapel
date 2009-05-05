
static char rcsid[] =
	"$Id: tracer.c,v 4.16 2001/09/25 21:18:46 pvmsrc Exp $";

/*
 *         Tracer version 1.0:  A Trace File Generator for PVM
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *           Authors:  James Arthur Kohl and G. A. Geist
 *                   (C) 1994 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear
 * in supporting documentation.
 *
 * Neither the Institution, Oak Ridge National Laboratory, nor the
 * Authors make any representations about the suitability of this
 * software for any purpose.  This software is provided ``as is''
 * without express or implied warranty.
 *
 * Tracer was funded by the U.S. Department of Energy.
 */


/* Tracer Headers */

#include "tracer.h"

#include "trcglob.h"

#ifdef NEEDSSELECTH 
#include <sys/select.h>
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <signal.h>

/* Event Receiving Loop Limit */

#define EVENT_COUNT    20


/* Command Routines */

#ifdef WIN32
void quit_sig_proc( int );
void quit_cmd_proc( int, char** );
#else
void quit_sig_proc();
void quit_cmd_proc();
#endif

void quit_proc();

int mask_proc();


/* Handler Routines */

void my_status_msg();
void program_init();
void read_args();
void pvm_init();
void trc_init();
void usage();
void work();


/* MAIN */

int
main( argc, argv )
int argc;
char **argv;
{
	/* Read Command Line Args */

	read_args( argc, argv );

	/* Initialize Program Constants & Structs */

	program_init();

	/* Initialize PVM */

	pvm_init();

	/* Initialize Tracer Globals */

	trc_init();

	/* Do Tracer-like Stuff */

	work();
}


void
work()
{
	struct timeval more_events;

	struct timeval *timeout;

	char cmd[1024];

	char *prompt;

#ifdef FDSETNOTSTRUCT
	fd_set rfds;
	fd_set fds;
#else
	struct fd_set rfds;
	struct fd_set fds;
#endif

	int *np;

	int nfds;
	int more;
	int max;
	int in;
	int n;
	int i;

#ifdef WIN32
#define read _read
	HANDLE hStdin;
	struct timeval zerotimeout = { 0, 0 };
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
#endif

	/* Initialize File Descriptors */

	FD_ZERO( &rfds );

	/* Get stdin input */
#ifndef WIN32
	in = fileno( stdin );

	FD_SET( in, &rfds );

	nfds = 1;
#else
	i=0;
#endif
	/* Get PVM Sockets */

	if ( pvm_getfds( &np ) > 0 )
	{
		FD_SET( np[0], &rfds );

		nfds = np[0] + 1;
	}

	/* Initialize Select Timeout - For Events Still Pending */

	more_events.tv_sec = 0;
	more_events.tv_usec = 0;

	/* Print Initial Prompt */

	prompt = "tracer> ";

	printf( prompt );
	fflush( stdout );

	/* Process Events Messages and User Input */

	while ( 1 )
	{
		/* Check for Event Messages */

		more = recv_events();

		if ( more )
			timeout = &more_events;
		
		else
#ifndef WIN32
			timeout = (struct timeval *) NULL;
#else
			timeout = &zerotimeout;
#endif

		/* Wait for Input */

		fds = rfds;

		if ( (n = SELECT( nfds, &fds, NULL, NULL, timeout )) == -1 )
			perror( "select" );

#ifdef WIN32
		else if (WAIT_TIMEOUT == WaitForSingleObject(hStdin,2000) )
			continue; /* no input */
#else
		else if ( n > 0 && FD_ISSET( in, &fds ) )
#endif
		{
#ifndef WIN32
			if ( (n = read(in, cmd, sizeof( cmd ) - 1 )) < 1 )
#else
	/* even if I set in=0 -> read(in, ...) fails. must be explicityly 0 ! 
	   am I missing something ?
	*/
			if ( (n = read(0, cmd, sizeof( cmd ) - 1 )) < 1 )

#endif
			{
				printf( "Quit\n" );

				quit_proc();
			}

			else
			{
				cmd[ n ] = '\0';

				handle_cmd( cmd );

				printf( prompt );
				fflush( stdout );
			}
		}
	}
}


int
handle_cmd( cmd )
char *cmd;
{
	char *av[128];

	int ac;

	ac = ( sizeof( av ) / sizeof( av[0] ) ) - 1;

	if ( acav( cmd, &ac, av ) )
	{
		printf( "\nError:  Command Too Long...\n\n" );

		return( TRC_FALSE );
	}

	/* "Ugly Parse" Command Ops */

	if ( ac >= 1 )
	{
		if ( !strcmp( av[0], "mask" ) || !strcmp( av[0], "trace" ) )
			mask_proc( ac, av );

		else if ( !strcmp( av[0], "buffer" )
			|| !strcmp( av[0], "buf" ) )
		{
			buffer_proc( ac, av );
		}

		else if ( !strcmp( av[0], "options" )
			|| !strcmp( av[0], "opt" ) )
		{
			options_proc( ac, av );
		}

		else if ( !strcmp( av[0], "help" ) || !strcmp( av[0], "h" )
			|| !strcmp( av[0], "?" ) )
		{
			help_proc( ac, av );
		}

		else if ( !strcmp( av[0], "q" ) || !strcmp( av[0], "quit" )
			|| !strcmp( av[0], "x" ) || !strcmp( av[0], "exit" ) )
		{
			quit_cmd_proc( ac, av );
		}

		else
		{
			printf( "Unknown Command \"%s\".\n", av[0] );

			return( TRC_FALSE );
		}
	}

	return( TRC_TRUE );
}


int
recv_events()
{
	int status;
	int ecnt;

	ecnt = trc_recv_messages( ID, EVENT_COUNT, &status );

	if ( ecnt < 0 )
		exit( -1 );

	if ( ecnt )
		fflush( ID->trace_out );

	return( status & TRC_MSG_STATUS_MORE );
}


void
read_args( argc, argv )
int argc;
char **argv;
{
	char tmp[1024];

	int i, j, k;
	int do_usage;
	int len;

	OUTPUT_FILE = (char *) NULL;

	TRACE_FILE = (char *) NULL;

	TRACE_BUF = 0;

	TRACE_OPT = PvmTraceFull;

	vflag = 0;

	do_usage = 0;

	for ( i=1 ; i < argc ; i++ )
	{
		if ( argv[i][0] == '-' )
		{
			k = i + 1;

			len = strlen( argv[i] );

			for ( j=0 ; j < len ; j++ )
			{
				switch ( argv[i][j] )
				{
					case 'H':
					case 'h':
						usage(); break;

					case 'T':
					{
						if ( TRACE_FILE != NULL )
							free( TRACE_FILE );

						TRACE_FILE = trc_copy_str( argv[k++] );

						break;
					}

					case 'O':
					{
						if ( OUTPUT_FILE != NULL )
							free( OUTPUT_FILE );

						OUTPUT_FILE = trc_copy_str( argv[k++] );

						break;
					}

					case 'B':
					{
						TRACE_BUF = atoi( argv[k++] );

						break;
					}

					case 'v': vflag++; break;

					case '-': break;

					default:
					{
						printf( "Unknown Option -%c\n", argv[i][j] );

						do_usage++;

						break;
					}
				}
			}

			i = k - 1;
		}

		else
		{
			if ( !strcmp( argv[i], "full" ) )
				TRACE_OPT = PvmTraceFull;

			else if ( !strcmp( argv[i], "time" ) )
				TRACE_OPT = PvmTraceTime;

			else if ( !strcmp( argv[i], "count" ) )
				TRACE_OPT = PvmTraceCount;

			else
			{
				printf( "Unknown Option %s\n", argv[i] );

				do_usage++;
			}
		}
	}

	if ( do_usage )
		usage();
}


void
usage()
{
	printf( "\nusage:  tracer [ -T tracefile ] [ -O outfile ] " );
	printf( "[ full | time | count ] [ -Hhev ]\n\n" );

	printf( "where:\n" );
	printf( "------\n" );

	printf( "-T file\t   = Use \"file\" as Trace File\n" );

	printf( "-O file\t   = Use \"file\" as Additional Output File\n" );

	printf( "-B nbytes\t   = Set Trace Buffer Size to nbytes\n" );

	printf( "full\t   = Set Tracing Option to Full\n" );
	printf( "time\t   = Set Tracing Option to Timing\n" );
	printf( "count\t   = Set Tracing Option to Count\n" );

	printf( "-H/h\t   = Print This Help Information\n" );

	printf( "-e\t   = Dump Raw PVM Event Text\n" );
	printf( "-v\t   = Verbose Operation\n" );

	printf( "\n" );

	exit( 0 );
}


void
program_init()
{
	struct passwd *pw;

	char hname[1024];
	char pvstr[255];
	char tmp[2048];

#ifdef WIN32
	char *username = 0;
	char *pstr;
#endif

	char *stripped;
	char *uppered;
	char *pvmtmp;
	char *home;

	int entry_exit;
	int release;
	int version;
	int fmt;
	int i;

	/* Get User Name for Trace File */

	if ( TRACE_FILE == NULL )
	{
		pvmtmp = pvmgettmp();

#ifndef WIN32
		if ( (pw = getpwuid( getuid() )) != NULL )
			sprintf( tmp, "%s/tracefile.%s", pvmtmp, pw->pw_name );
#else
		if ( !username )
			username = (char *) MyGetUserName();

		if ( username )
			sprintf( tmp, "%s\\tracefile.%s", pvmtmp, username );
#endif		
		else
#ifndef WIN32 
			sprintf( tmp, "%s/tracefile.%d", pvmtmp, getpid() );
#else
		{	
			fprintf( stderr,
				"You are not logged onto the machine!  Exiting.\n");	
			exit( -1 );
		}
#endif		
		TRACE_FILE = trc_copy_str( tmp );
	}

	/* Initialize Globals & Flags */

	TRACER_REGISTERED = TRC_FALSE;
}


void
pvm_init()
{
	char *av[2];

	int inum;
	int se;
	int cc;
	int i;

	/* Options */

	pvm_setopt( PvmResvTids, 1 );

	pvm_setopt( PvmRoute, PvmDontRoute );

	/* Get My TID */

	MYTID = pvm_mytid();

	if ( MYTID < 0 )
	{
		pvm_perror( "Error Joining PVM" );

		exit( -1 );
	}

	else
		printf( "\nTracer connected as tid=0x%x.\n", MYTID );

	/* Set Desired Signals */

	pvm_setopt( PvmNoReset, 1 );

#ifndef WIN32
	signal( SIGALRM, SIG_IGN );
	signal( SIGQUIT, (vfp) quit_sig_proc );
	signal( SIGINT, (vfp) quit_sig_proc );
#endif

	signal( SIGINT, quit_sig_proc );
	
	/* Set Trace Masks */

	TEV_MASK_INIT( CLEAR_MASK );

	av[0] = "mask";
	av[1] = "xpvm";

	mask_proc( 2, av );

	pvm_settmask( PvmTaskChild, TRACE_MASK );

	/* Set Trace Buffering */

	pvm_setopt( PvmTraceBuffer, TRACE_BUF );

	/* Set Tracing Options */

	pvm_setopt( PvmTraceOptions, TRACE_OPT );
}


void
trc_init()
{
	/* Initialize Tracer */

	trc_tracer_init();

	/* Set Tracer Globals */

	TRC_HOST_ADD_NOTIFY_CODE = 99;
	TRC_HOST_DEL_NOTIFY_CODE = 100;

	TRC_VERSION = MYVERSION;

	TRC_NAME = "Tracer";

	TRC_TID = MYTID;

	/* Get Tracer ID */

	ID = trc_get_tracer_id();

	/* Set Local Handler Routines */

	ID->status_msg = my_status_msg;

	/* Set Trace Event Message Codes */

	ID->event_ctx = pvm_getcontext();
	ID->event_tag = 666;

	ID->output_ctx = pvm_getcontext();
	ID->output_tag = 667;

	/* Set PVM Tracing Options */

	trc_set_tracing_codes( ID );

	/* Open Trace File */

	trc_set_trace_file( ID, TRACE_FILE );

	if ( !trc_reset_trace_file( ID ) )
		exit( -1 );

	/* Check Hosts */

	trc_initialize_hosts( ID );

	/* Dump Host Status Events */

	trc_save_host_status_events( ID );

	/* Open Additional Output File */

	if ( OUTPUT_FILE != NULL )
	{
		trc_set_output_file( ID, OUTPUT_FILE );

		trc_open_output_file( ID );
	}
}


/* Status Message Handler */

void my_status_msg( ID, msg )
TRC_ID ID;
char *msg;
{
	printf( "%s\n", msg );
}


/* Stolen from regular PVM console...  :-) */

/*	acav()
*
*	Parse a string into words separated by whitespace.
*	Max number of words is original value of *acp.
*
*	Trashes out the original string.
*	Returns 0 with av[0]..av[*acp - 1] pointing to the words.
*	Returns 1 if too many words.
*	Returns -1 if unbalanced quote.
*/

int
acav(s, acp, av)
	char *s;			/* the string to parse */
	int *acp;			/* max num words in, num words found out */
	char **av;			/* pointers to words */
{
	int ac = 0;			/* number of words found */
	char *p = s;		/* input scanner */
	char *q;			/* output */
	int n = *acp;		/* max number of words allowed */
	int mode = 0;		/* quote mode */

	while (*p) {
		while (isspace(*p)) p++;
		if (*p) {
			if (*p == '#')
				break;
			if (ac >= n) {
				*acp = ac;
				return 1;
			}
			q = p;
			av[ac++] = p;
			while (*p) {
				if (mode) {
					if (mode == '\\') {
						*q++ = *p;
						mode = 0;

					} else if (mode == *p) {
						mode = 0;

					} else
						*q++ = *p;

				} else {
					if (isspace(*p))
						break;

					switch (*p) {

					case '"':
					case '\'':
					case '\\':
						mode = *p;
						break;

					default:
						*q++ = *p;
						break;
					}
				}
				p++;
			}
			if (*p)
				p++;
			if (*q)
				*q = 0;
			if (mode) {
				printf("unmatched %c\n", (char)mode);
				return -1;
			}
		}
	}
	*acp = ac;
	return 0;
}

