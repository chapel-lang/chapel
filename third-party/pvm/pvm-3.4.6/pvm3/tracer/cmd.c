
static char rcsid[] =
	"$Id: cmd.c,v 4.8 1998/11/20 20:22:47 pvmsrc Exp $";

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

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif


/* Command Routines */

int
mask_proc( ac, av )
int ac;
char **av;
{
	char name[255];

	int on_off;
	int set;
	int e;
	int i;

	/* No Args */

	if ( ac == 1 )
	{
		printf( "\nTrace Mask:\n\n" );

		trc_print_tmask( TRACE_MASK );

		printf( "\n" );

		return( TRC_TRUE );
	}

	/* Parse Command Args */

	on_off = TRC_TRUE;

	set = 0;

	for ( i=1 ; i < ac ; i++ )
	{
		if ( !strcmp( av[i], "+" ) )
			on_off = TRC_TRUE;

		else if ( !strcmp( av[i], "-" ) )
			on_off = TRC_FALSE;

		else if ( !strcmp( av[i], "default" )
			|| !strcmp( av[i], "xpvm" )
			|| !strcmp( av[i], "XPVM" ) )
		{
			TEV_MASK_INIT( TRACE_MASK );

			TEV_MASK_SET( TRACE_MASK, TEV_BARRIER );
			TEV_MASK_SET( TRACE_MASK, TEV_BCAST );
			TEV_MASK_SET( TRACE_MASK, TEV_JOINGROUP );
			TEV_MASK_SET( TRACE_MASK, TEV_LVGROUP );
			TEV_MASK_SET( TRACE_MASK, TEV_MYTID );
			TEV_MASK_SET( TRACE_MASK, TEV_PARENT );
			TEV_MASK_SET( TRACE_MASK, TEV_SPAWN );
			TEV_MASK_SET( TRACE_MASK, TEV_SENDSIG );
			TEV_MASK_SET( TRACE_MASK, TEV_KILL );
			TEV_MASK_SET( TRACE_MASK, TEV_EXIT );
			TEV_MASK_SET( TRACE_MASK, TEV_MCAST );
			TEV_MASK_SET( TRACE_MASK, TEV_SEND );
			TEV_MASK_SET( TRACE_MASK, TEV_RECV );
			TEV_MASK_SET( TRACE_MASK, TEV_RECVF );
			TEV_MASK_SET( TRACE_MASK, TEV_NRECV );
			TEV_MASK_SET( TRACE_MASK, TEV_PSEND );
			TEV_MASK_SET( TRACE_MASK, TEV_PRECV );
			TEV_MASK_SET( TRACE_MASK, TEV_TRECV );
			TEV_MASK_SET( TRACE_MASK, TEV_PROBE );
			TEV_MASK_SET( TRACE_MASK, TEV_USER_DEFINED );

			set++;
		}

		else if ( !strcmp( av[i], "*" ) )
		{
			for ( e=TEV_FIRST ; e <= TEV_MAX ; e++ )
			{
				if ( on_off )
					TEV_MASK_SET( TRACE_MASK, e );

				else
					TEV_MASK_UNSET( TRACE_MASK, e );
			}

			set++;
		}

		else
		{
			if ( !strncmp( av[i], "pvm_", 4 ) )
				strcpy( name, av[i] + 4 );

			else
				strcpy( name, av[i] );

			for ( e=TEV_FIRST ; e <= TEV_MAX ; e++ )
			{
				if ( !strcmp( pvmtevinfo[e].name, name )
					|| !strcmp( pvmtevinfo[e].name, av[i] ) )
				{
					break;
				}
			}

			if ( e <= TEV_MAX )
			{
				if ( on_off )
					TEV_MASK_SET( TRACE_MASK, e );

				else
					TEV_MASK_UNSET( TRACE_MASK, e );

				set++;
			}

			else
				printf( "No such event \"%s\"\n", av[i] );
		}
	}

	/* Install New Trace Mask */

	if ( set )
	{
		pvm_settmask( PvmTaskChild, TRACE_MASK );

		printf( "\nNew Trace Mask:\n\n" );

		trc_print_tmask( TRACE_MASK );

		printf( "\n" );

		re_register_tracer();
	}

	return( TRC_TRUE );
}


int
buffer_proc( ac, av )
int ac;
char **av;
{
	int nbytes;

	if ( ac == 1 )
	{
		printf( "\nCurrent Trace Buffer Size is %d Bytes\n\n",
			TRACE_BUF );

		return( TRC_TRUE );
	}

	nbytes = atoi( av[1] );

	nbytes = nbytes < 0 ? 0 : nbytes;

	TRACE_BUF = nbytes;

	pvm_setopt( PvmTraceBuffer, TRACE_BUF );

	re_register_tracer();

	return( TRC_TRUE );
}


int
options_proc( ac, av )
int ac;
char **av;
{
	int cc = TRC_TRUE;

	if ( ac == 1 )
	{
		printf( "\nCurrent Tracing Option is " );
		
		switch ( TRACE_OPT )
		{
			case PvmTraceFull:
				printf( "full.\n\n" );
				break;

			case PvmTraceTime:
				printf( "time.\n\n" );
				break;

			case PvmTraceCount:
				printf( "count.\n\n" );
				break;

			default:
				printf( "undefined!\n\n" );
				cc = TRC_FALSE;
		}
		
		return( cc );
	}

	if ( !strcmp( av[1], "full" ) )
		TRACE_OPT = PvmTraceFull;
	
	else if ( !strcmp( av[1], "time" ) )
		TRACE_OPT = PvmTraceTime;

	else if ( !strcmp( av[1], "count" ) )
		TRACE_OPT = PvmTraceCount;

	else
	{
		printf( "\nUnknown Trace Option \"%s\".\n\n", av[1] );

		return( TRC_FALSE );
	}

	pvm_setopt( PvmTraceOptions, TRACE_OPT );

	re_register_tracer();

	return( cc );
}


void
re_register_tracer()
{
	/* Re-Register Tracer */

	if ( TRACER_REGISTERED )
	{
		if ( pvm_reg_tracer( -1, -1, -1, -1, CLEAR_MASK, -1, -1 ) > 0 )
		{
			printf( "Error Unregistering Tracer...  Exiting.\n" );

			exit( -1 );
		}

		else
			TRACER_REGISTERED = TRC_FALSE;
	}

	if ( pvm_reg_tracer( pvm_getcontext(), 666, pvm_getcontext(), 667,
		TRACE_MASK, TRACE_BUF, TRACE_OPT ) < 0 )
	{
		printf( "Tracer Already Connected...  Exiting.\n" );

		exit( -1 );
	}

	else
		TRACER_REGISTERED = TRC_TRUE;
}


int
help_proc( ac, av )
int ac;
char **av;
{
	int i;

	/* General Help */

	if ( ac == 1 )
	{
		printf( "\nTracer %s\n\n", TRC_VERSION );

		printf( "mask      - display or modify the trace mask.\n" );
		printf( "trace     - display or modify the trace mask.\n" );
		printf( "buffer    - display or set the trace buffer size.\n" );
		printf( "options   - display or set the trace options.\n" );
		printf( "help      - display this help information.\n" );
		printf( "help cmd  - display help information for cmd.\n" );

		printf( "quit      - close trace file and exit tracer.\n" );

		printf( "\n" );

		return( TRC_TRUE );
	}

	/* Dump Usage for Command */

	for ( i=1 ; i < ac ; i++ )
	{
		if ( !strcmp( av[i], "mask" ) )
		{
			printf(
		"usage:  mask [ [+|-] [ event | * ] ] [ default | xpvm ]\n" );
		}

		else if ( !strcmp( av[i], "trace" ) )
		{
			printf(
		"usage:  trace [ [+|-] [ event | * ] ] [ default | xpvm ]\n" );
		}

		else if ( !strcmp( av[i], "buffer" ) )
			printf( "usage:  buffer [ nbytes ]\n" );

		else if ( !strcmp( av[i], "options" ) )
			printf( "usage:  options [ full | time | count ]\n" );

		else
		{
			printf( "No Help Available for \"%s\"\n", av[i] );

			return( TRC_FALSE );
		}
	}

	return( TRC_TRUE );
}


void
quit_proc()
{
	printf( "Quitting Tracer.\n" );

	if ( TRACER_REGISTERED )
		pvm_reg_tracer( -1, -1, -1, -1, CLEAR_MASK, -1, -1 );

	if ( ID->trace_out != NULL && ID->trace_out != stdout )
		fclose( ID->trace_out );

	exit( -1 );
}


void
quit_cmd_proc( ac, av )
int ac;
char **av;
{
	quit_proc();
}


void
quit_sig_proc( sig )
int sig;
{
	quit_proc();
}

