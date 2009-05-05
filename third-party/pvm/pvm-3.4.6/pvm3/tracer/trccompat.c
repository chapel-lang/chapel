
static char rcsid[] = 
	"$Id: trccompat.c,v 4.6 1999/11/08 17:44:51 pvmsrc Exp $";

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

#include "trclib.h"

#include "trccompat.h"

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif


/* Old Trace File Generation Routines - For Compatibility */


/* Trace Lookup Trie Init Routines */

void
trc_init_old_events()
{
	int i;

	TRC_OLD_EVENT_TRIE = trc_create_triestack();

	for ( i=TRC_OLD_TEV_FIRST ; i < TRC_OLD_TRACE_MAX ; i++ )
	{
		if ( strcmp( TRC_OLD_TEV_TRACE_NAMES[i], "" ) )
		{
			trc_add_to_trie( TRC_OLD_EVENT_TRIE,
				TRC_OLD_TEV_TRACE_NAMES[i],
				(void *) ( ( i - TRC_OLD_TEV_FIRST ) + 1 ) );
		}

		TRC_OLD_TEV_TRACE[i] = 0;
	}
}


int
trc_process_old_trace_event( ID, name )
TRC_ID ID;
char *name;
{
	long tmp;

	int tusec;
	int tsec;
	int flag;
	int eid;
	int tid;

	if ( !trc_find_event_str( ID, "{" ) )
		return( TRC_FALSE );

	/* } to match above */

	flag = fscanf( ID->trace_in, "%d, %d, %d", &tsec, &tusec, &tid );

	if ( flag != 3 )
	{
		printf( "Incomplete Trace Record\n" );

		return( TRC_FALSE );
	}

	if ( tmp = (long) trc_lookup_trie( TRC_OLD_EVENT_TRIE, name ) )
	{
		eid = (int) tmp;

		eid = ( eid - 1 ) + TRC_OLD_TEV_FIRST;

		if ( ID->handle_old_event )
			(ID->handle_old_event)( ID, eid, tsec, tusec, tid );

		else
			trc_find_event_end( ID );

		return( TRC_TRUE );
	}

	else
	{
		printf( "Error: Event \"%s\" Not Found\n", name );

		trc_find_event_end( ID );

		return( TRC_FALSE );
	}
}


int
trc_store_old_trace_event( ID, tsec, tusec, tid, eid )
TRC_ID ID;
int tsec;
int tusec;
int tid;
int eid;
{
	static int *tids = (int *) NULL;
	static int ntids = 0;

	TRC_TEVTASK TT;

	char where[4096];
	char name[4096];
	char msg[255];

	int msgtag;
	int nbytes;
	int ignore;
	int count;
	int flags;
	int addr;
	int ecnt;
	int host;
	int type;
	int ptid;
	int dtid;
	int stid;
	int omit;
	int len;
	int buf;
	int num;
	int cc;
	int i;

	/* Set Leftover Event Omit Flag */

	TT = trc_get_tevtask_tid( ID, tid );

	omit = ( ID->trace_out == NULL
		|| ( TT != NULL && TRC_TASK_OMIT( TT ) ) )
			? TRC_TRUE : TRC_FALSE;

	/* Initialize Event Counter */

	ecnt = 0;

	if ( !omit )
	{
		/* Output Trace Event Descriptor (if necessary) */

		if ( !( TRC_OLD_TEV_TRACE[ eid ] ) )
		{
			fprintf( ID->trace_out, "\n" );

			if ( ID->event_dump_hdr )
				(ID->event_dump_hdr)( ID, tid );

			fprintf( ID->trace_out, "#%d: %s\n", eid + 1,
				TRC_OLD_TRACE_DESCRIPTORS[ eid ] );

			(TRC_OLD_TEV_TRACE[ eid ])++;
		}

		/* Update Trace Time */

		trc_update_trace_time( tsec, tusec );

		/* Output Trace Event Header */

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		fprintf( ID->trace_out, "\"%s\" { %d, %d, %d",
			TRC_OLD_TEV_TRACE_NAMES[ eid ], tsec, tusec, tid );  /* } */
	}

	/* Convert Trace to Output File */

	switch ( eid )
	{
		case TRC_OLD_TEV_NEWTASK:
		{
			if ( omit )
				break;

			pvm_upkint( &ptid, 1, 1 );
			pvm_upkint( &flags, 1, 1 );
			pvm_upkstr( name );

			if ( !strcmp( name, "" ) )
				strcpy( name, "-" );

			host = pvm_tidtohost( tid );

			fprintf( ID->trace_out, ", %d, %d, %d",
				ptid, host, flags );

			trc_dump_trace_str( ID, name, TRC_TRUE );

			ignore = 0;

			if ( !(ID->group_tasks) && TRC_GROUPTASK( name ) )
				ignore++;

			if ( TT == NULL )
			{
				if ( !ignore )
				{
					sprintf( msg, "Task TID=0x%x Connected to %s",
						tid, TRC_NAME );

					trc_status_msg( ID, msg );
				}

				TT = trc_create_tevtask();

				TT->tid = tid;

				TT->outstatus = TRC_TASK_NOOUT;

				TT->next = ID->tevtask_list;

				ID->tevtask_list = TT;
			}

			if ( !ignore )
				TT->tevstatus = TRC_TASK_ALIVE;
			
			else
				TT->tevstatus = TRC_TASK_IGNORE;

			break;
		}

		case TRC_OLD_TEV_SPNTASK:
		{
			if ( omit )
				break;

			trc_dump_old_pvm_event_fmt( ID, eid );

			if ( TT == NULL )
			{
				sprintf( msg, "Task TID=0x%x Connected to %s",
					tid, TRC_NAME );

				trc_status_msg( ID, msg );

				TT = trc_create_tevtask();

				TT->tid = tid;

				TT->tevstatus = TRC_TASK_ALIVE;
				TT->outstatus = TRC_TASK_NOOUT;

				TT->next = ID->tevtask_list;

				ID->tevtask_list = TT;
			}

			break;
		}

		case TRC_OLD_TEV_ENDTASK:
		{
			if ( !omit )
				trc_dump_old_pvm_event_fmt( ID, eid );

			if ( TT != NULL )
			{
				if ( TT->tevstatus == TRC_TASK_ALIVE )
				{
					TT->tevstatus = TRC_TASK_DEAD;

					trc_check_for_dead_host( ID, TT );

					if ( !trc_tevtasks_alive( ID ) )
						trc_end_trace( ID );
				}

				else
					TT->tevstatus = TRC_TASK_DEAD;
			}

			else if ( !omit )
			{
				printf( "\nWarning: ENDTASK Unknown Task TID=0x%x.\n\n",
					tid );
			}

			break;
		}

		case TRC_OLD_TEV_ADDHOSTS0:
		{
			if ( omit )
				break;

			pvm_upkint( &num, 1, 1 );

			fprintf( ID->trace_out, ", %d, [%d][100] { ", num, num );

			for ( i=0 ; i < num ; i++ )
			{
				pvm_upkstr( name );

				if ( i != num - 1 )
					fprintf( ID->trace_out, "\"%s\", ", name );
				
				else
					fprintf( ID->trace_out, "\"%s\" }", name );
			}

			break;
		}

		case TRC_OLD_TEV_ADDHOSTS1:
		{
			if ( omit )
				break;

			pvm_upkint( &cc, 1, 1 );

			fprintf( ID->trace_out, ", %d", cc );

			break;
		}

		case TRC_OLD_TEV_SPAWN0:
		{
			if ( omit )
				break;

			pvm_upkstr( name );
			pvm_upkint( &flags, 1, 1 );
			pvm_upkstr( where );
			pvm_upkint( &count, 1, 1 );

			trc_dump_trace_str( ID, name, TRC_TRUE );

			fprintf( ID->trace_out, ", %d", flags );

			trc_dump_trace_str( ID, where, TRC_TRUE );

			fprintf( ID->trace_out, ", %d", count );

			break;
		}

		case TRC_OLD_TEV_SPAWN1:
		{
			if ( omit )
				break;

			pvm_upkint( &num, 1, 1 );

			fprintf( ID->trace_out, ", %d, [%d] {", num, num );

			if ( num > 0 )
			{
				if ( num > ntids )
				{
					ntids = num * 2;

					if ( tids != NULL )
						free( tids );

					tids = (int *) malloc( (unsigned) ntids
						* sizeof(int) );
					trc_memcheck( tids, "Spawn TIDs" );
				}

				pvm_upkint( tids, num, 1 );

				for ( i=0 ; i < num ; i++ )
				{
					if ( i )
						fprintf( ID->trace_out, ", %d", tids[i] );
					
					else
						fprintf( ID->trace_out, " %d", tids[i] );
				}
			}

			fprintf( ID->trace_out, " }" );

			break;
		}

		case TRC_OLD_TEV_SEND0:
		{
			if ( omit )
				break;

			pvm_upkint( &dtid, 1, 1 );
			pvm_upkint( &msgtag, 1, 1 );

			fprintf( ID->trace_out, ", %d, %d", dtid, msgtag );

			break;
		}

		case TRC_OLD_TEV_MCAST0:
		{
			if ( omit )
				break;

			pvm_upkint( &num, 1, 1 );
			pvm_upkint( &msgtag, 1, 1 );

			fprintf( ID->trace_out, ", %d, %d, [%d] {",
				num, msgtag, num );

			if ( num > 0 )
			{
				if ( num > ntids )
				{
					ntids = num * 2;

					if ( tids != NULL )
						free( tids );

					tids = (int *) malloc( (unsigned) ntids
						* sizeof(int) );
					trc_memcheck( tids, "Mcast TIDs" );
				}

				pvm_upkint( tids, num, 1 );

				for ( i=0 ; i < num ; i++ )
				{
					if ( i )
						fprintf( ID->trace_out, ", %d", tids[i] );
				
					else
						fprintf( ID->trace_out, " %d", tids[i] );
				}
			}

			fprintf( ID->trace_out, " }" );

			break;
		}

		case TRC_OLD_TEV_PSEND0:
		{
			if ( omit )
				break;

			pvm_upkint( &dtid, 1, 1 );
			pvm_upkint( &msgtag, 1, 1 );
			pvm_upkint( &addr, 1, 1 );
			pvm_upkint( &len, 1, 1 );
			pvm_upkint( &type, 1, 1 );

			fprintf( ID->trace_out, ", %d, %d, %d, %d, %d",
				dtid, msgtag, addr, len, type );

			break;
		}

		case TRC_OLD_TEV_RECV1:
		case TRC_OLD_TEV_TRECV1:
		{
			if ( omit )
				break;

			pvm_upkint( &buf, 1, 1 );
			pvm_upkint( &nbytes, 1, 1 );

			if ( nbytes >= 0 )
			{
				pvm_upkint( &msgtag, 1, 1 );
				pvm_upkint( &stid, 1, 1 );
			}

			else
				msgtag = stid = -1;

			fprintf( ID->trace_out, ", %d, %d, %d, %d",
				buf, nbytes, msgtag, stid );

			break;
		}

		case TRC_OLD_TEV_PRECV1:
		{
			if ( omit )
				break;

			pvm_upkint( &buf, 1, 1 );

			fprintf( ID->trace_out, ", %d, 0, 0, 0", buf );

			break;
		}

		case TRC_OLD_TEV_NRECV1:
		{
			if ( omit )
				break;

			pvm_upkint( &buf, 1, 1 );

			if ( buf > 0 )
			{
				pvm_upkint( &nbytes, 1, 1 );

				if ( nbytes >= 0 )
				{
					pvm_upkint( &msgtag, 1, 1 );
					pvm_upkint( &stid, 1, 1 );
				}

				else
					msgtag = stid = -1;
			}

			else
				nbytes = msgtag = stid = -1;

			fprintf( ID->trace_out, ", %d, %d, %d, %d",
				buf, nbytes, msgtag, stid );

			break;
		}

		default:
		{
			if ( !omit )
				trc_dump_old_pvm_event_fmt( ID, eid );
		}
	}

	if ( !omit )
	{
		/* Finish Off Trace Event ({) */

		fprintf( ID->trace_out, " };;\n" );

		ecnt++;
	}

	return( ecnt );
}


void
trc_dump_old_pvm_event_fmt( ID, eid )
TRC_ID ID;
int eid;
{
	char buf[4096];

	char *fmt;

	int keeplist[10];

	int veclen;
	int index;
	int nkeep;
	int keep;
	int ival;

	fmt = trc_old_tev_formats[ eid ].fmt;

	if ( fmt != NULL )
	{
		nkeep = 0;

		while ( *fmt )
		{
			if ( *fmt == '%' )
			{
				fmt++;

				if ( *fmt == '*' )
				{
					keep = TRC_TRUE;

					fmt++;
				}

				else
					keep = TRC_FALSE;

				if ( *fmt == '$' )
				{
					fmt++;

					index = *fmt - '0';

					fmt++;

					veclen = ( index >= 0 && index < nkeep )
						? keeplist[index] : 0;
				}

				else
					veclen = 1;

				while ( veclen > 0 )
				{
					veclen--;

					switch ( *fmt )
					{
						case 'R':
						case 'd':
						case 'r':
						case 'x':
						{
							pvm_upkint( &ival, 1, 1 );
		
							fprintf( ID->trace_out, ", %d", ival );
		
							break;
						}
		
						case 'S':
						{
							pvm_upkstr( buf );
		
							trc_dump_trace_str( ID, buf, TRC_TRUE );
		
							break;
						}
		
						default:
							break;
					}
				}
	
				if ( *fmt )
					fmt++;

				if ( keep )
				{
					keeplist[nkeep] = ival < 0 ? 0 : ival;

					nkeep++;
				}
			}
			
			else
				fmt++;
		}
	}
}


void
trc_write_old_output_event( ID, tid, str )
TRC_ID ID;
int tid;
char *str;
{
	if ( !( TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_OUTPUT ] ) )
	{
		fprintf( ID->trace_out, "\n" );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		fprintf( ID->trace_out, "#%d: %s\n",
			TRC_OLD_TRACE_OUTPUT + 1,
			TRC_OLD_TRACE_DESCRIPTORS[ TRC_OLD_TRACE_OUTPUT ] );

		(TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_OUTPUT ])++;
	}

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	fprintf( ID->trace_out, "\"%s\" { %d, %ld, %d",
		TRC_OLD_TEV_TRACE_NAMES[ TRC_OLD_TRACE_OUTPUT ],
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, tid );

	trc_dump_trace_str( ID, str, TRC_TRUE );

	fprintf( ID->trace_out, " };;\n" );
}


void
trc_write_old_host_add_event( ID, H )
TRC_ID ID;
TRC_HOST H;
{
	if ( !( TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_ADD ] ) )
	{
		fprintf( ID->trace_out, "\n" );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, H->pvmd_tid );

		fprintf( ID->trace_out, "#%d: %s\n",
			TRC_OLD_TRACE_HOST_ADD + 1,
			TRC_OLD_TRACE_DESCRIPTORS[ TRC_OLD_TRACE_HOST_ADD ] );

		(TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_ADD ])++;
	}

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, H->pvmd_tid );

	fprintf( ID->trace_out, "\"%s\" { %d, %ld, %d",
		TRC_OLD_TEV_TRACE_NAMES[ TRC_OLD_TRACE_HOST_ADD ],
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, H->pvmd_tid );

	trc_dump_trace_str( ID, H->refname, TRC_TRUE );

	trc_dump_trace_str( ID, H->alias, TRC_TRUE );

	trc_dump_trace_str( ID, H->arch, TRC_TRUE );

	fprintf( ID->trace_out, ", %d };;\n", H->speed );
}


void
trc_write_old_host_del_event( ID, H, tid )
TRC_ID ID;
TRC_HOST H;
int tid;
{
	char tmp[1024];

	if ( !( TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_DEL ] ) )
	{
		fprintf( ID->trace_out, "\n" );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		fprintf( ID->trace_out, "#%d: %s\n",
			TRC_OLD_TRACE_HOST_DEL + 1,
			TRC_OLD_TRACE_DESCRIPTORS[ TRC_OLD_TRACE_HOST_DEL ] );

		(TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_DEL ])++;
	}

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	fprintf( ID->trace_out, "\"%s\" { %d, %ld, %d",
		TRC_OLD_TEV_TRACE_NAMES[ TRC_OLD_TRACE_HOST_DEL ],
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, tid );

	if ( strcmp( H->refname, "" ) )
	{
		sscanf( H->refname, "%s", tmp );

		trc_dump_trace_str( ID, tmp, TRC_TRUE );
	}

	else
		trc_dump_trace_str( ID, H->refname, TRC_TRUE );

	fprintf( ID->trace_out, " };;\n" );
}


void
trc_write_old_host_sync_event( ID, H )
TRC_ID ID;
TRC_HOST H;
{
	if ( !( TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_SYNC ] ) )
	{
		fprintf( ID->trace_out, "\n" );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, H->pvmd_tid );

		fprintf( ID->trace_out, "#%d: %s\n",
			TRC_OLD_TRACE_HOST_SYNC + 1,
			TRC_OLD_TRACE_DESCRIPTORS[ TRC_OLD_TRACE_HOST_SYNC ] );

		(TRC_OLD_TEV_TRACE[ TRC_OLD_TRACE_HOST_SYNC ])++;
	}

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, H->pvmd_tid );

	fprintf( ID->trace_out, "\"%s\" { %d, %ld, %d, %d, %ld };;\n",
		TRC_OLD_TEV_TRACE_NAMES[ TRC_OLD_TRACE_HOST_SYNC ],
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, H->pvmd_tid,
		H->delta.tv_sec, H->delta.tv_usec );

	H->last_time.tv_sec = TRC_TRACE_TIME.tv_sec;
	H->last_time.tv_usec = TRC_TRACE_TIME.tv_usec;
}


char *
trc_old_pvm_event_string( ID, eid )
TRC_ID ID;
int eid;
{
	char result[4096];
	char tmp[2048];
	char buf[1024];

	char *fmt;
	char *str;

	int keeplist[10];

	int vecflag;
	int veclen;
	int index;
	int nkeep;
	int keep;
	int ival;
	int sep;

	fmt = trc_old_tev_formats[ eid ].fmt;

	if ( fmt != NULL )
	{
		nkeep = 0;

		if ( *fmt != '=' )
			sprintf( result, "%s", trc_old_tev_formats[ eid ].name );

		else
			sprintf( result, "%s() ", trc_old_tev_formats[ eid ].name );
	
		while ( *fmt )
		{
			if ( *fmt == '%' )
			{
				fmt++;

				if ( *fmt == '*' )
				{
					keep = TRC_TRUE;

					fmt++;
				}

				else
					keep = TRC_FALSE;

				if ( *fmt == '$' )
				{
					fmt++;

					index = *fmt - '0';

					fmt++;

					veclen = ( index >= 0 && index < nkeep )
						? keeplist[index] : 0;

					vecflag = TRC_TRUE;

					sep = TRC_FALSE;
				}

				else
				{
					veclen = 1;

					vecflag = TRC_FALSE;
				}

				while ( veclen > 0 )
				{
					veclen--;

					if ( vecflag )
					{
						if ( sep )
							trc_append_str( result, " ", 4096 );

						sep = TRC_TRUE;
					}

					switch ( *fmt )
					{
						case 'd':
						{
							fscanf( ID->trace_in, ", %d", &ival );
		
							sprintf( tmp, " %d", ival );
		
							trc_append_str( result, tmp, 4096 );
	
							break;
						}
		
						/* decimal result code */
						case 'R':
						{
							fscanf( ID->trace_in, ", %d", &ival );
		
							sprintf( tmp, " %d", ival );
	
							trc_append_str( result, tmp, 4096 );
	
							if ( ival < 0 )
							{
								if ( ival <= 0
									&& ival > -trc_old_pvm_nerr )
								{
									sprintf( tmp, " (%s)",
										trc_old_pvm_errlist[ -ival ] );
								}
								
								else
								{
									sprintf( tmp, " %d (Unknown Error)",
										ival );
								}
	
								trc_append_str( result, tmp, 4096 );
							}
		
							break;
						}
		
						/* hex result code / decimal if negative */
						case 'r':
						{
							fscanf( ID->trace_in, ", %d", &ival );
		
							if ( ival < 0 )
							{
								if ( ival <= 0
									&& ival > -trc_old_pvm_nerr )
								{
									sprintf( tmp, " %d (%s)", ival,
										trc_old_pvm_errlist[ -ival ] );
								}
	
								else
								{
									sprintf( tmp, " %d (Unknown Error)",
										ival );
								}
							}
							
							else
								sprintf( tmp, " 0x%x", ival );
		
							trc_append_str( result, tmp, 4096 );
	
							break;
						}
		
						case 'x':
						{
							fscanf( ID->trace_in, ", %d", &ival );
		
							sprintf( tmp, " 0x%x", ival );
		
							trc_append_str( result, tmp, 4096 );
		
							break;
						}
		
						case 'S':
						{
							trc_read_old_trace_str( ID,
								buf, 1024, TRC_TRUE );
		
							sprintf( tmp, " \"%s\"", buf );
		
							trc_append_str( result, tmp, 4096 );
		
							break;
						}
		
						default:
						{
							sprintf( tmp, "%%%c", *fmt );
		
							trc_append_str( result, tmp, 4096 );
	
							break;
						}
					}
				}
	
				if ( *fmt )
					fmt++;
				
				if ( keep )
				{
					keeplist[nkeep] = ival < 0 ? 0 : ival;

					nkeep++;
				}
			}
			
			else
			{
				sprintf( tmp, "%c", *fmt++ );

				trc_append_str( result, tmp, 4096 );
			}
		}
	}

	else
		sprintf( result, "%s()", trc_old_tev_formats[ eid ].name );

	trc_find_event_end( ID );

	str = trc_copy_str( result );

	return( str );
}


int
trc_read_old_trace_str( ID, str, size, end_flag )
TRC_ID ID;
char *str;
int size;
int end_flag;
{
	char c;

	int i;

	*str = '\0';

	if ( !trc_find_event_str( ID, "\"" ) )
		return( TRC_FALSE );

	i = 0;

	while ( (c = getc( ID->trace_in )) != (char) EOF && c != '"'
		&& i < size - 1 )
	{
		str[i++] = c;
	}

	str[i] = '\0';

	TRC_CKEOF( c, "EOF Reading Trace String\n", return( TRC_FALSE ) );

	if ( c != '"' )
	{
		if ( !trc_find_event_str( ID, "\"" ) )
			return( TRC_FALSE );
	}

	if ( end_flag )
	{
		/* Matching { */

		if ( !trc_find_event_str( ID, "}" ) )
			return( TRC_FALSE );
	}

	return( TRC_TRUE );
}


void
trc_dump_old_sddf_headers()
{
	int i;

	printf( "\n" );
	printf( "XPVM Trace Format Descriptors for PVM 3.3 - SDDF:\n" );
	printf( "=================================================\n" );

	for ( i=TRC_OLD_TEV_FIRST ; i < TRC_OLD_TRACE_MAX ; i++ )
	{
		if ( strcmp( TRC_OLD_TRACE_DESCRIPTORS[i], "" ) )
		{
			printf( "\n#%d: %s\n", i + 1,
				TRC_OLD_TRACE_DESCRIPTORS[i] );
		}
	}

	exit( 0 );
}


void
trc_reset_old_descriptors()
{
	int i;

	for ( i=TRC_OLD_TEV_FIRST ; i < TRC_OLD_TRACE_MAX ; i++ )
		TRC_OLD_TEV_TRACE[i] = 0;
}

