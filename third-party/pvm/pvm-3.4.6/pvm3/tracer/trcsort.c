
static char rcsid[] =
	"$Id: trcsort.c,v 4.14 2009/01/30 21:23:58 pvmsrc Exp $";

/*
 *         Trcsort version 1.0:  A Trace File Sorter for PVM
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HASSTDLIB
#include <stdlib.h>
#else
#ifndef IMA_MACOSX
#include <malloc.h>
#endif
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvm3.h>
#include <pvmtev.h>

#include "trcdef.h"


/* Constants */

#define HANDLE_HOST_SYNC    1

#define MAX_OPEN_FP    10


/* Macros */

#define TIME_COMPARE( _t1, _t2 ) \
( \
	( (_t1).tv_sec > (_t2).tv_sec \
			|| ( (_t1).tv_sec == (_t2).tv_sec \
				&& (_t1).tv_usec > (_t2).tv_usec ) ) ? 1 : \
		( ( (_t1).tv_sec == (_t2).tv_sec \
			&& (_t1).tv_usec == (_t2).tv_usec ) ? 0 : -1 ) \
) \
\


/* Data Structures */

struct fp_buffer_struct
{
	FILE *fp;
	char *rwstr;
	/* TMPFILE */ struct tmpfile_struct *tmpfile;
	/* FPBUF */ struct fp_buffer_struct *next;
};

typedef struct fp_buffer_struct *FPBUF;


struct tmpfile_struct
{
	FPBUF fpbuf;
	char *fname;
	long lastseek;
	TRC_TEVDESC savetd;
	TRC_TEVREC savetr;
	int tid;
	struct timeval nexttime;
	struct timeval lasttime;
	struct timeval delta;
	int valid;
	int eof;
	/* TMPFILE */ struct tmpfile_struct *next;
};

typedef struct tmpfile_struct *TMPFILE;


/* Handler Routines */

TRC_TEVREC save_tevrec();

TMPFILE create_tmpfile();
TMPFILE get_tmpfile();

FPBUF create_fpbuf();

void get_tevrec_tmpfile();
void handle_event_split();
void handle_event_join();
void handle_passthru();
void my_status_msg();
void program_init();
void free_fpbuf();
void read_args();
void sync_time();
void close_fp();
void trc_init();
void cleanup();
void usage();
void bail();

int verify_fp();


/* Globals */

TMPFILE CURRENTTF;
TMPFILE	LASTTF;
TMPFILE	TMPF;

FPBUF	FPS;
int		NFPS;

TRC_ID	ID;

char	DIRNAME[255];

char	*TRACE_OUTFILE;
char	*TRACE_INFILE;
char	*OUTPUT_FILE;

int		vflag;


/* MAIN */

int
main( argc, argv )
int argc;
char **argv;
{
	TMPFILE NEXTTF;
	TMPFILE HOSTTF;
	TMPFILE TF;

	static struct timeval tmpt = { -1, -1 };

	int host;
	int done;
	int eof;

	/* Read Command Line Args */

	read_args( argc, argv );

	/* Initialize Program Constants & Structs */

	program_init();

	/* Initialize Tracer Globals */

	trc_init();

	/* Parse Input Trace, Create Subfiles */

	while ( trc_read_trace_event( ID, &eof ) );

	fclose( ID->trace_in );

	/* Flush Temporary File Ptrs, Reset Subfiles for Reading */

	TF = TMPF;

	while ( TF != NULL )
	{
		close_fp( TF );

		TF->eof = TRC_FALSE;

		TF = TF->next;
	}

	/* Reset Pass Thru -> All Goes to Output Trace Now */

	LASTTF = (TMPFILE) NULL;

	/* Set Event Handling for Joining */

	ID->handle_event = handle_event_join;

	/* Read In First Event From Each Subfile (Prime Pump) */

	TF = TMPF;

	while ( TF != NULL )
	{
		/* Check Setting of Time Stamp Delta */

		host = pvm_tidtohost( TF->tid );

		if ( host != TF->tid )
		{
			HOSTTF = get_tmpfile( host, &tmpt, TRC_FALSE );

			if ( HOSTTF != NULL )
			{
				TF->delta.tv_sec = HOSTTF->delta.tv_sec;
				TF->delta.tv_usec = HOSTTF->delta.tv_usec;
			}
		}

		/* Get First Event (Record) */

		TF->valid = TRC_FALSE;

		do
		{
			get_tevrec_tmpfile( TF );
		}
		while( !(TF->valid) && !(TF->eof) );

		if ( TF->eof )
			close_fp( TF );

		TF = TF->next;
	}

	/* Read In Subfiles, Dump Sorted Trace */

	do
	{
		done = TRC_TRUE;

		/* Find "Next" Trace Event */

		NEXTTF = (TMPFILE) NULL;

		TF = TMPF;

		while ( TF != NULL )
		{
			if ( TF->valid )
			{
				if ( NEXTTF != NULL )
				{
					if ( TIME_COMPARE( TF->nexttime,
							NEXTTF->nexttime ) < 0
						|| ( TIME_COMPARE( TF->nexttime,
								NEXTTF->nexttime ) == 0
							&& TF->tid < NEXTTF->tid ) )
					{
						NEXTTF = TF;
					}
				}

				else
					NEXTTF = TF;

				done = TRC_FALSE;
			}

			TF = TF->next;
		}

		/* Dump It */

		if ( NEXTTF != NULL )
		{
			trc_store_tevrec( ID, NEXTTF->savetd, NEXTTF->savetr,
					NEXTTF->tid );

			/* Get Next Event for Task */

			NEXTTF->valid = TRC_FALSE;

			do
			{
				get_tevrec_tmpfile( NEXTTF );
			}
			while( !(NEXTTF->valid) && !(NEXTTF->eof) );

			if ( NEXTTF->eof )
				close_fp( NEXTTF );
		}
	}
	while ( !done );

	fclose( ID->trace_out );

	cleanup();

	exit( 0 );
}


void
handle_event_split( trcid, TD, TR )
TRC_ID trcid;
TRC_TEVDESC TD;
TRC_TEVREC TR;
{
	TMPFILE TFptr;
	TMPFILE TF;

	FILE *fpsave;

	struct timeval tmpt;

	int tid;

	if ( trcid != ID )
	{
		printf( "Error: Unknown Trace ID\n" );

		return;
	}

	/* Grab Task ID from Event */

	tid = TRC_GET_TEVREC_VALUE( TR, "TID", int );

	tmpt.tv_sec = TRC_GET_TEVREC_VALUE( TR, "TS", int );
	tmpt.tv_usec = TRC_GET_TEVREC_VALUE( TR, "TU", int );

	/* Key Off of TID & Time Stamp to Select Temp File */

	TF = get_tmpfile( tid, &tmpt, TRC_TRUE );

	if ( TF == NULL )
	{
		printf( "\nError Creating Temporary Trace File\n\n" );
		bail();
	}

	/* Set Time Stamp Marker */

	TF->lasttime.tv_sec = tmpt.tv_sec;
	TF->lasttime.tv_usec = tmpt.tv_usec;

	/* Check for Host Sync */

	if ( TD->hid == HANDLE_HOST_SYNC )
	{
		TF->delta.tv_sec = TRC_GET_TEVREC_VALUE( TR, "HDS", int );
		TF->delta.tv_usec = TRC_GET_TEVREC_VALUE( TR, "HDU", int );

		TFptr = TMPF;

		while ( TFptr != NULL )
		{
			if ( TFptr->tid == TF->tid && TFptr != TF )
			{
				TFptr->delta.tv_sec = TF->delta.tv_sec;
				TFptr->delta.tv_usec = TF->delta.tv_usec;
			}

			TFptr = TFptr->next;
		}
	}

	/* Check for Descriptor Dump */

	if ( TD->dump )
		trc_dump_tevdesc( ID, TD, -1 );

	/* Write Event to Temp File */

	if ( !verify_fp( TF, "a" ) )
		bail();

	fpsave = ID->trace_out;

	ID->trace_out = TF->fpbuf->fp;

	trc_store_tevrec( ID, TD, TR, tid );

	ID->trace_out = fpsave;
	
	/* Save Last Temp File Ptr for Pass Thru */

	LASTTF = TF;
}


void
handle_event_join( trcid, TD, TR )
TRC_ID trcid;
TRC_TEVDESC TD;
TRC_TEVREC TR;
{
	if ( trcid != ID )
	{
		printf( "Error: Unknown Trace ID\n" );

		return;
	}

	if ( CURRENTTF == NULL )
	{
		printf( "Warning:  Current Temp File Not Set...\n" );

		return;
	}

	CURRENTTF->nexttime.tv_sec = TRC_GET_TEVREC_VALUE( TR, "TS", int );
	CURRENTTF->nexttime.tv_usec = TRC_GET_TEVREC_VALUE( TR, "TU", int );

	sync_time( CURRENTTF );

	CURRENTTF->savetd = TD;
	CURRENTTF->savetr = save_tevrec( TR );

	CURRENTTF->valid = TRC_TRUE;
}


void
handle_passthru( trcid, str )
TRC_ID trcid;
char *str;
{
	if ( trcid != ID )
	{
		printf( "Error: Unknown Trace ID\n" );

		return;
	}

	if ( LASTTF != NULL )
	{
		if ( !verify_fp( LASTTF, "a" ) )
			bail();

		fprintf( LASTTF->fpbuf->fp, "%s\n", str );
	}
	
	else
		fprintf( ID->trace_out, "%s\n", str );
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

	TRACE_OUTFILE = (char *) NULL;
	TRACE_INFILE = (char *) NULL;
	OUTPUT_FILE = (char *) NULL;

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
					case 'o':
					{
						if ( TRACE_OUTFILE != NULL )
							free( TRACE_OUTFILE );
			
						TRACE_OUTFILE = trc_copy_str( argv[ k++ ] );

						break;
					}

					case 'O':
					{
						if ( OUTPUT_FILE != NULL )
							free( OUTPUT_FILE );
			
						OUTPUT_FILE = trc_copy_str( argv[ k++ ] );

						break;
					}

					case 'H':
					case 'h':
						usage(); break;

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
			if ( TRACE_INFILE != NULL )
				free( TRACE_INFILE );
			
			TRACE_INFILE = trc_copy_str( argv[i] );
		}
	}

	if ( do_usage )
		usage();
}


void
usage()
{
	printf( "\nusage:  trcsort [ <trace_infile> ] " );
	printf( "[ -o <trace_outfile> ] " );
	printf( "[ -O <output_file> ] " );
	printf( "[ -v ] " );
	printf( "\n\n" );

	printf( "where:\n" );
	printf( "------\n" );

	printf( "<trace_infile>    =    " );
	printf( "trace file to be sorted (default is stdin)\n" );

	printf( "<trace_outfile>   =    " );
	printf( "file to store sorted trace (default is stdout)\n" );

	printf( "<output_file>     =    " );
	printf( "file to store separate copy of any task output\n" );

	printf( "-v                =    " );
	printf( "display dynamic open file usage\n" );

	printf( "\n" );

	exit( 0 );
}


void
program_init()
{
	/* Create Temp Directory for Subfiles */

	sprintf( DIRNAME, "tmpsort.%d", (int) getpid() );

	if ( mkdir( DIRNAME, S_IRWXU | S_IRWXG | S_IRWXO ) )
	{
		printf( "\nError Creating Temporary Directory \"%s\".\n\n",
				DIRNAME );

		exit( -1 );
	}

	/* Initialize Temporary File Ptrs */

	LASTTF = (TMPFILE) NULL;

	TMPF = (TMPFILE) NULL;

	FPS = (FPBUF) NULL;
	NFPS = 0;
}


void
trc_init()
{
	int fd;

	/* Initialize Tracer */

	trc_tracer_init();

	/* Get Tracer ID */

	ID = trc_get_tracer_id();

	/* Set Local Handler Routines */

	ID->handle_command = handle_passthru;
	ID->handle_comment = handle_passthru;
	ID->handle_event = handle_event_split;
	ID->status_msg = my_status_msg;

	/* Set Up Handle ID for Host Sync Events */

	trc_add_to_trie( TRC_HANDLE_TRIE,
		"host_sync",    (void *) HANDLE_HOST_SYNC );

	/* Create In & Out Trace File Ptrs */

	if ( TRACE_INFILE != NULL )
	{
		ID->trace_in = fopen( TRACE_INFILE, "r" );
		if ( !trc_filecheck( ID->trace_in, TRACE_INFILE ) )
			bail();
	}

	else
		ID->trace_in = stdin;

	if ( TRACE_OUTFILE != NULL )
	{
		fd = open( TRACE_OUTFILE, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 );
		if ( !trc_fdcheck( fd, TRACE_OUTFILE ) )
			bail();

		ID->trace_out = fdopen( fd, "w" );
		if ( !trc_filecheck( ID->trace_out, TRACE_OUTFILE ) )
			bail();
	}

	else
		ID->trace_out = stdout;

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


/* TRC_TEVREC Routines */

TRC_TEVREC
save_tevrec( TR )	/* actually, more like "steal_tevrec" :-) */
TRC_TEVREC TR;
{
	TRC_TEVREC tmp;

	tmp = trc_create_tevrec();

	tmp->ddesc = TR->ddesc;
	TR->ddesc = (TRC_DATADESC) NULL;

	tmp->value = TR->value;
	TR->value = (TRC_VALUE) NULL;

	tmp->num = TR->num;
	TR->num = -1;

	tmp->next = TR->next;
	TR->next = (TRC_TEVREC) NULL;

	return( tmp );
}


/* TMPFILE Routines */

TMPFILE
create_tmpfile()
{
	TMPFILE tmp;

	tmp = (TMPFILE) malloc( sizeof( struct tmpfile_struct ) );
	trc_memcheck( tmp, "Temporary File Structure" );

	tmp->fpbuf = (FPBUF) NULL;
	tmp->fname = (char *) NULL;
	tmp->lastseek = (long) 0;

	tmp->savetr = (TRC_TEVREC) NULL;
	tmp->tid = -1;

	tmp->nexttime.tv_sec = -1;
	tmp->nexttime.tv_usec = -1;

	tmp->lasttime.tv_sec = -1;
	tmp->lasttime.tv_usec = -1;

	tmp->delta.tv_sec = 0;
	tmp->delta.tv_usec = 0;

	tmp->valid = -1;
	tmp->eof = -1;
	tmp->next = (TMPFILE) NULL;

	return( tmp );
}


TMPFILE
get_tmpfile( tid, tmptp, createflag )
int tid;
struct timeval *tmptp;
int createflag;
{
	TMPFILE TF;

	static struct timeval delta = { 0, 0 };

	char fname[255];

	int index;

	TF = TMPF;

	index = 1;

	while ( TF != NULL )
	{
		if ( TF->tid == tid )
		{
			if ( ( tmptp->tv_sec == -1 && tmptp->tv_usec == -1 )
					|| TIME_COMPARE( *tmptp, TF->lasttime ) >= 0 )
			{
				return( TF );
			}

			delta.tv_sec = TF->delta.tv_sec;
			delta.tv_usec = TF->delta.tv_usec;

			index++;
		}

		TF = TF->next;
	}

	if ( !createflag )
		return( (TMPFILE) NULL );

	TF = create_tmpfile();

	TF->tid = tid;

	if ( index > 1 )
		sprintf( fname, "%s/tmp.%x.%d", DIRNAME, tid, index );

	else
		sprintf( fname, "%s/tmp.%x", DIRNAME, tid );

	TF->fname = trc_copy_str( fname );

	if ( !verify_fp( TF, "w" ) )
	{
		free( TF );
		return( (TMPFILE) NULL );
	}

	if ( index > 1 )
	{
		TF->delta.tv_sec = delta.tv_sec;
		TF->delta.tv_usec = delta.tv_usec;
	}

	TF->next = TMPF;

	TMPF = TF;

	return( TF );
}


void
get_tevrec_tmpfile( TF )
TMPFILE TF;
{
	FILE *savefp;

	if ( TF->eof )
		return;

	CURRENTTF = TF;

	if ( !verify_fp( TF, "r" ) )
		bail();

	savefp = ID->trace_in;

	ID->trace_in = TF->fpbuf->fp;

	trc_read_trace_event( ID, &(TF->eof) );

	ID->trace_in = savefp;
}


void
sync_time( TF )
TMPFILE TF;
{
	TF->nexttime.tv_sec += TF->delta.tv_sec;
	TF->nexttime.tv_usec += TF->delta.tv_usec;

	if ( TF->nexttime.tv_usec > 1000000 )
	{
		TF->nexttime.tv_usec -= 1000000;

		(TF->nexttime.tv_sec)++;
	}

	else if ( TF->nexttime.tv_usec < 0 )
	{
		TF->nexttime.tv_usec += 1000000;

		(TF->nexttime.tv_sec)--;
	}
}


/* FPBUF Routines */

FPBUF
create_fpbuf()
{
	FPBUF tmp;

	tmp = (FPBUF) malloc( sizeof( struct fp_buffer_struct ) );
	trc_memcheck( tmp, "File Buffer Structure" );

	tmp->fp = (FILE *) NULL;

	tmp->rwstr = (char *) NULL;

	tmp->tmpfile = (TMPFILE) NULL;

	tmp->next = (FPBUF) NULL;

	if ( vflag )
		{ printf( "+" ); fflush( stdout ); }

	return( tmp );
}


void
free_fpbuf( ptr )
FPBUF *ptr;
{
	FPBUF FPB;

	if ( ptr == NULL || *ptr == NULL )
		return;
	
	FPB = *ptr;

	/* Close Any Open File Ptr */

	if ( FPB->fp != NULL )
	{
		if ( FPB->tmpfile != NULL )
			FPB->tmpfile->lastseek = ftell( FPB->fp );

		fclose( FPB->fp );

		FPB->fp = (FILE *) NULL;
	}

	FPB->rwstr = (char *) NULL;

	if ( FPB->tmpfile != NULL )
	{
		FPB->tmpfile->fpbuf = (FPBUF) NULL;

		FPB->tmpfile = (TMPFILE) NULL;
	}

	FPB->next = (FPBUF) NULL;

	free( FPB );

	*ptr = (FPBUF) NULL;

	if ( vflag )
		{ printf( "\b \b" ); fflush( stdout ); }
}


int
verify_fp( TF, rwstr )
TMPFILE TF;
char *rwstr;
{
	FPBUF FPBlast;
	FPBUF FPBptr;
	FPBUF FPB;

	int fd;

	/* File Ptr Buffer Already There */

	if ( TF->fpbuf )
	{
		/* Verify Same Mode */

		if ( !strcmp( TF->fpbuf->rwstr, rwstr ) )
		{
			/* Move to Top of List */

			if ( FPS != TF->fpbuf )
			{
				if ( FPS == NULL )
				{
					printf( "Warning:  Empty File Ptr Buffer List\n" );

					return( TRC_TRUE );
				}

				FPB = FPS;

				while ( FPB->next != NULL )
				{
					if ( FPB->next == TF->fpbuf )
					{
						FPB->next = TF->fpbuf->next;

						TF->fpbuf->next = FPS;

						FPS = TF->fpbuf;

						return( TRC_TRUE );
					}

					FPB = FPB->next;
				}

				printf(
					"Warning:  Verify File Ptr Buffer Not Found\n" );
			}

			return( TRC_TRUE );
		}

		/* Switching File Modes, Close File Ptr */

		else
			close_fp( TF );
	}

	/* Check for Flush */

	if ( NFPS >= MAX_OPEN_FP )
	{
		FPBptr = FPS;

		FPBlast = (FPBUF) NULL;

		while ( FPBptr->next != NULL )
		{
			FPBlast = FPBptr;
			FPBptr = FPBptr->next;
		}

		if ( FPBlast != NULL )
			free_fpbuf( &(FPBlast->next) );

		else
			free_fpbuf( &(FPS) );

		NFPS--;
	}

	/* Create New File Ptr Buffer */

	FPB = create_fpbuf();

	if ( !strcmp( rwstr, "w" ) )
	{
		fd = open( TF->fname, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 );

		if ( !trc_fdcheck( fd, TF->fname ) )
			return( TRC_FALSE );

		FPB->fp = fdopen( fd, rwstr );
	}

	else if ( !strcmp( rwstr, "a" ) )
	{
		fd = open( TF->fname,
				O_RDWR|O_APPEND|O_CREAT|O_EXCL|O_TRUNC, 0600 );

		if ( !trc_fdcheck( fd, TF->fname ) )
			return( TRC_FALSE );

		FPB->fp = fdopen( fd, rwstr );
	}

	else
		FPB->fp = fopen( TF->fname, rwstr );

	if ( !trc_filecheck( FPB->fp, TF->fname ) )
	{
		free_fpbuf( &FPB );

		return( TRC_FALSE );
	}

	else
	{
		/* Add File Ptr Buffer to List */

		FPB->rwstr = rwstr;

		FPB->tmpfile = TF;
		TF->fpbuf = FPB;

		FPB->next = FPS;
		FPS = FPB;

		NFPS++;

		/* Check for File Seek */

		if ( TF->lastseek != 0 )
		{
			if ( fseek( FPB->fp, TF->lastseek, 0 ) )
			{
				printf( "\nError in fseek() of Temp File\n\n" );
				bail();
			}
		}

		/* Success */

		return( TRC_TRUE );
	}
}


void
close_fp( TF )
TMPFILE TF;
{
	FPBUF FPBlast;
	FPBUF FPB;

	int flag;

	/* Check for Null File Ptr Buffer */

	if ( TF->fpbuf == NULL )
	{
		TF->lastseek = (long) 0;

		return;
	}

	/* Remove Buffer From List */

	if ( FPS != NULL )
	{
		if ( FPS == TF->fpbuf )
		{
			FPS = TF->fpbuf->next;

			NFPS--;
		}

		else
		{
			FPB = FPS;

			flag = 0;

			while ( FPB->next != NULL && !flag )
			{
				if ( FPB->next == TF->fpbuf )
				{
					FPB->next = TF->fpbuf->next;

					NFPS--;

					flag++;
				}

				else
					FPB = FPB->next;
			}

			if ( !flag )
			{
				printf(
					"Warning:  Close File Ptr Buffer Not Found.\n" );
			}
		}
	}

	free_fpbuf( &(TF->fpbuf) );

	TF->lastseek = (long) 0;
}


/* Clean Up Temp Directory */

void
cleanup()
{
	TMPFILE TF;

	TF = TMPF;

	while ( TF != NULL )
	{
		if ( TF->fname != NULL )
			unlink( TF->fname );

		TF = TF->next;
	}

	unlink( DIRNAME );
}


void
bail()
{
	cleanup();

	exit( -1 );
}

