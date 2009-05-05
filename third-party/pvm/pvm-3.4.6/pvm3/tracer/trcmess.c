
static char rcsid[] =
	"$Id: trcmess.c,v 4.7 1998/01/28 23:02:05 pvmsrc Exp $";

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


/* Tracer Library Header */

#include "trclib.h"

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif


/* Routines */

int
trc_recv_messages( ID, max_count, status )
TRC_ID ID;
int max_count;
int *status;
{
	static int *tids = (int *) NULL;
	static int ntids = 0;

	TRC_HOST H;

	TRC_ID DEAD_ID_last;
	TRC_ID DEAD_ID_next;
	TRC_ID DEAD_ID;

	int msgtag;
	int others;
	int ecnt;
	int cnt;
	int len;
	int mid;
	int num;
	int tid;
	int src;
	int tmp;
	int cc;
	int i;

	/* Verify Message Codes Set */

	if ( TRC_HOST_ADD_NOTIFY_CODE == -1
		|| TRC_HOST_DEL_NOTIFY_CODE == -1
#ifndef USE_PVM_33
		|| ID->event_ctx == -1 || ID->output_ctx == -1
#endif
		|| ID->event_tag == -1 || ID->output_tag == -1 )
	{
		printf( "Error: Message Codes Not Set - %d %d %d %d %d %d\n",
			TRC_HOST_ADD_NOTIFY_CODE, TRC_HOST_DEL_NOTIFY_CODE,
			ID->event_ctx, ID->output_ctx,
			ID->event_tag, ID->output_tag );

		return( -1 );
	}

	/* Clear Out Flags */

	if ( status != NULL )
		*status = TRC_MSG_STATUS_NONE;

	/* Init Event Counter */

	ecnt = 0;

	/* Check for Host Sync Updates */

	ecnt += trc_check_host_sync( ID );

	/* Receive Stuff */

	cnt = 0;

	while ( cnt < max_count
		&& (mid = trc_get_msg( ID, &len, &msgtag, &src, &others )) > 0 )
	{
		/* Set Activity Flag */

		if ( status != NULL )
			*status |= TRC_MSG_STATUS_ACTIVE;

		/* Host Added */

		if ( msgtag == TRC_HOST_ADD_NOTIFY_CODE && src == 0x80000000 )
		{
			cc = pvm_upkint( &num, 1, 1 );

			if ( cc < 0 )
				pvm_perror( "Error Unpacking Host Add Notify" );

			else
			{
				if ( num > ntids )
				{
					ntids = num * 2;

					if ( tids )
						free( tids );

					tids = (int *) malloc( (unsigned) ntids
						* sizeof(int) );
					trc_memcheck( tids, "Host Add Notify Tids Array" );
				}

				cc = pvm_upkint( tids, num, 1 );

				if ( cc < 0 )
					pvm_perror( "Error Unpacking Host Add TIDs" );

				else
				{
					pvm_notify( PvmHostDelete,
						TRC_HOST_DEL_NOTIFY_CODE, num, tids );

					ecnt += trc_handle_host_add_notify( ID, tids, num );
				}
			}
		}

		/* Host Deleted */

		else if ( msgtag == TRC_HOST_DEL_NOTIFY_CODE
			&& src == 0x80000000 )
		{
			cc = pvm_upkint( &tid, 1, 1 );

			if ( cc < 0 )
				pvm_perror( "Error Unpacking Host Delete Notify" );

			else
				ecnt += trc_handle_host_del_notify( ID, tid );
		}

		/* PVM Trace Event */

		else if ( msgtag == ID->event_tag )
		{
			ecnt += trc_handle_trace_message( ID, src );

			if ( status != NULL )
			{
				if ( ecnt )
					*status |= TRC_MSG_STATUS_OVERWRITE;

				else
					*status |= TRC_MSG_STATUS_CLEANUP;
			}
		}

		/* PVM Output Event */

		else if ( msgtag == ID->output_tag )
		{
			ecnt += trc_handle_output_message( ID );

			if ( status != NULL )
			{
				if ( ecnt )
					*status |= TRC_MSG_STATUS_OVERWRITE;

				else
					*status |= TRC_MSG_STATUS_CLEANUP;
			}
		}

		/* Unknown Message */

		else
		{
			fprintf( stderr,
				"Tracer:  msg from 0x%x code %d len %d ?\n",
				src, msgtag, len );
		}

		if ( pvm_freebuf( mid ) < 0 )
			pvm_perror( "Error Freeing Message Buffer" );

		cnt++;
	}

	if ( mid < 0 )
	{
		pvm_perror( "PVM System Failure" );

		printf( "Tracer exiting...\n" );

		return( -1 );
	}

	if ( status != NULL )
	{
		if ( cnt == max_count )
			*status |= TRC_MSG_STATUS_MORE;
	}

	/* Try Flushing Dead IDs */

	if ( others )
	{
		DEAD_ID = TRC_DEAD_ID_LIST;

		DEAD_ID_last = (TRC_ID ) NULL;

		while ( DEAD_ID != NULL )
		{
			trc_recv_messages( DEAD_ID, 1, &tmp );

			if ( status != NULL )
				*status |= tmp;

			if ( trc_tevtasks_dead( DEAD_ID ) )
			{
				DEAD_ID_next = DEAD_ID->next;

				if ( DEAD_ID_last != NULL )
					DEAD_ID_last->next = DEAD_ID_next;

				else
					TRC_DEAD_ID_LIST = DEAD_ID_next;

				trc_free_tracer_id( &DEAD_ID );

				DEAD_ID = DEAD_ID_next;
			}

			else
			{
				DEAD_ID_last = DEAD_ID;

				DEAD_ID = DEAD_ID->next;
			}
		}
	}

	return( ecnt );
}


int
trc_get_msg( ID, len, msgtag, src, others )
TRC_ID ID;
int *len;
int *msgtag;
int *src;
int *others;
{
	int mid;
	int tag;

	/* Reset "Others" Flag */

	if ( others != NULL )
		*others = TRC_FALSE;

	/* Check for Host Add */

	if ( (mid = pvm_nrecv( -1, TRC_HOST_ADD_NOTIFY_CODE )) > 0 )
	{
		if ( pvm_bufinfo( mid, len, msgtag, src ) < 0 )
		{
			pvm_perror( "Error Checking Message Buffer" );

			return( -1 );
		}

		return( mid );
	}

	else if ( mid < 0 )
	{
		pvm_perror( "Error Checking Host Add Notify Message" );

		return( -1 );
	}

	/* Check for Host Delete */

	if ( (mid = pvm_nrecv( -1, TRC_HOST_DEL_NOTIFY_CODE )) > 0 )
	{
		if ( pvm_bufinfo( mid, len, msgtag, src ) < 0 )
		{
			pvm_perror( "Error Checking Message Buffer" );

			return( -1 );
		}

		return( mid );
	}

	else if ( mid < 0 )
	{
		pvm_perror( "Error Checking Host Delete Notify Message" );

		return( -1 );
	}

	/* Any message at all? */

	if ( (mid = pvm_probe( -1, -1 )) > 0 )
	{
		if ( pvm_bufinfo( mid, len, &tag, src ) < 0 )
		{
			pvm_perror( "Error Checking Message Buffer" );

			return( -1 );
		}

		if ( msgtag != NULL )
			*msgtag = tag;

		/* The next one's for us... */

		if ( tag == ID->event_tag || tag == ID->output_tag )
		{
			pvm_recv( -1, -1 );

			return( mid );
		}

		/* Or not... */

		else
		{
			if ( others != NULL )
				*others = TRC_TRUE;

			return( 0 );
		}
	}

	/* System Error */

	else if ( mid < 0 )
	{
		pvm_perror( "Error Probing for Trace / Output Messages" );

		return( -1 );
	}

	/* No Messages */

	else
		return( 0 );
}


int
trc_handle_trace_message( ID, src )
TRC_ID ID;
int src;
{
	TRC_DATADESC DD;

	TRC_TEVDESC TD;

	TRC_TEVTASK TT;

	TRC_HOST H;

	TRC_DID dptr;

	char diddesc[1024];
	char tmp[1024];
	char did[8];

	int desc_done;
	int did_done;
	int done;
	int omit;

	int entry_exit;
	int dump_event;
	int pvmd_tid;
	int did_int;
	int tusec;
	int tsec;
	int ecnt;
	int eid;
	int tid;
	int dt;

	int marker;
	int nest;

	ecnt = 0;

	TRC_PVMCKERR( pvm_upkint( &marker, 1, 1 ),
		"Unpacking Trace Message", return( ecnt ) );

	/* Old PVM 3.3 Trace Format */

	if ( marker > 0 )
	{
		tsec = marker;

		TRC_PVMCKERR( pvm_upkint( &tusec, 1, 1 ),
			"Event Parse Error", return( ecnt ) );

		TRC_PVMCKERR( pvm_upkint( &tid, 1, 1 ),
			"Event Parse Error", return( ecnt ) );

		TRC_PVMCKERR( pvm_upkint( &eid, 1, 1 ),
			"Event Parse Error", return( ecnt ) );

		ecnt += trc_store_old_trace_event( ID, tsec, tusec, tid, eid );

		return( ecnt );
	}

	/* New PVM 3.4 Format */

	/* Get Task & Host Info */

	TT = trc_get_tevtask_tid( ID, src );

	pvmd_tid = pvm_tidtohost( src );

	if ( !pvmd_tid )
		pvmd_tid = pvm_tidtohost( TRC_TID );

	H = trc_get_host_tid( pvmd_tid );

	/* Set Leftover Event Omit Flag */

	omit = ( ID->trace_out == NULL
		|| ( TT != NULL && TRC_TASK_OMIT( TT ) ) )
			? TRC_TRUE : TRC_FALSE;

	/* Process Event Message */

	nest = 0;

	done = 0;

	do
	{
		switch ( marker )
		{
			case TEV_MARK_EVENT_BUFFER:
			{
				nest++;

				break;
			}

			case TEV_MARK_EVENT_BUFFER_END:
			{
				if ( !(--nest) )
					done++;

				break;
			}

			case TEV_MARK_DATA_ID:
			{
				did_done = 0;

				do
				{
					TRC_PVMCKERR( pvm_upkint( &did_int, 1, 1 ),
						"Data ID Message", return( ecnt ) );

					if ( did_int != TEV_MARK_DATA_ID_END )
					{
						TRC_PVMCKERR( pvm_upkstr( did ),
							"Data ID Name", return( ecnt ) );

						TRC_PVMCKERR( pvm_upkstr( diddesc ),
							"Data ID Description", return( ecnt ) );

						dptr = (TRC_DID) trc_lookup_trie( TRC_DID_TRIE,
							did );

						if ( dptr == NULL
							|| dptr->id != did_int
							|| strcmp( dptr->desc, diddesc ) )
						{
							dptr = trc_create_did();

							dptr->id = did_int;

							dptr->name = trc_copy_str( did );

							dptr->desc = trc_copy_str( diddesc );

							dptr->next = TRC_DID_LIST;

							TRC_DID_LIST = dptr;

							trc_add_to_trie( TRC_DID_TRIE,
								dptr->name, (void *) dptr );
						}

						trc_set_didlist( H, dptr );
					}

					else
						did_done++;
				}
				while ( !did_done );

				if ( !nest )
					done++;

				break;
			}

			case TEV_MARK_EVENT_DESC:
			{
				TD = trc_create_tevdesc();

				TD->refcount = 1;

				TRC_PVMCKERR( pvm_upkint( &eid, 1, 1 ),
					"Event Descriptor ID", return( ecnt ) );

				TD->eid = eid & ~( TEV_EVENT_ENTRY | TEV_EVENT_EXIT );

				if ( eid & TEV_EVENT_ENTRY )
					TD->entry_exit = TRC_ENTRY_TEV;

				else if ( eid & TEV_EVENT_EXIT )
					TD->entry_exit = TRC_EXIT_TEV;

				else
					TD->entry_exit = TRC_IGNORE_TEV;

				TRC_PVMCKERR( pvm_upkstr( tmp ),
					"Event Descriptor Name", return( ecnt ) );

				TD->name = trc_copy_str( tmp );

				desc_done = 0;

				do
				{
					TRC_PVMCKERR( pvm_upkint( &did_int, 1, 1 ),
						"Semantic Data ID", return( ecnt ) );

					if ( did_int != TEV_MARK_EVENT_DESC_END )
					{
						if ( TD->ddesc == NULL )
							DD = TD->ddesc = trc_create_datadesc();

						else
							DD = DD->next = trc_create_datadesc();

						if ( H->didlist == NULL )
						{
							printf(
							"Warning: Missing DID List For Host %s\n",
								H->name );
						}

						DD->did = trc_get_did( H, did_int );

						if ( DD->did == NULL )
						{
							printf( "Error: DID=%d Not Found\n",
								did_int );

							trc_free_tevdesc( &TD );

							return( ecnt );
						}

						TRC_PVMCKERR( pvm_upkint( &dt, 1, 1 ),
							"Data Type", return( ecnt ) );

						DD->dt = dt &
							~( TEV_DATA_SCALAR | TEV_DATA_ARRAY );

						DD->array = dt &
							( TEV_DATA_SCALAR | TEV_DATA_ARRAY );

						if ( !trc_unpack_data_value( DD ) )
						{
							trc_free_tevdesc( &TD );

							return( ecnt );
						}
					}

					else
						desc_done++;
				}
				while ( !desc_done );

				dump_event = trc_add_tevdesc( ID, &TD, src, omit );

				trc_set_tevlist( TT, H, TD );

				if ( TD->dump )
					trc_dump_tevdesc( ID, TD, src );

				if ( dump_event )
					trc_store_data_values( ID, TD, src, omit );

				if ( !omit )
					ecnt++;

				if ( !nest )
					done++;

				break;
			}

			case TEV_MARK_USER_EVENT_RECORD:
			{
				TD = trc_create_tevdesc();

				TD->refcount = 1;

				TRC_PVMCKERR( pvm_upkint( &eid, 1, 1 ),
					"Event Descriptor ID", return( ecnt ) );

				TD->eid = eid & ~( TEV_EVENT_ENTRY | TEV_EVENT_EXIT );

				if ( eid & TEV_EVENT_ENTRY )
					TD->entry_exit = TRC_ENTRY_TEV;

				else if ( eid & TEV_EVENT_EXIT )
					TD->entry_exit = TRC_EXIT_TEV;

				else
					TD->entry_exit = TRC_IGNORE_TEV;

				TRC_PVMCKERR( pvm_upkstr( tmp ),
					"Event Descriptor Name", return( ecnt ) );

				TD->name = trc_copy_str( tmp );

				/* Yank Out Hardwired Timestamp */

				DD = TD->ddesc = trc_create_datadesc();

				DD->did = trc_get_did( (TRC_HOST) NULL, TEV_DID_TS );

				DD->dt = TEV_DATA_INT;
				DD->array = TEV_DATA_SCALAR;

				if ( !trc_unpack_data_value( DD ) )
				{
					trc_free_tevdesc( &TD );

					return( ecnt );
				}

				DD = DD->next = trc_create_datadesc();

				DD->did = trc_get_did( (TRC_HOST) NULL, TEV_DID_TU );

				DD->dt = TEV_DATA_INT;
				DD->array = TEV_DATA_SCALAR;

				if ( !trc_unpack_data_value( DD ) )
				{
					trc_free_tevdesc( &TD );

					return( ecnt );
				}

				desc_done = 0;

				do
				{
					TRC_PVMCKERR( pvm_upkint( &dt, 1, 1 ),
						"Data Type", return( ecnt ) );

					if ( dt != TEV_MARK_USER_EVENT_RECORD_END )
					{
						DD = DD->next = trc_create_datadesc();

						DD->dt = dt &
							~( TEV_DATA_SCALAR | TEV_DATA_ARRAY );

						DD->array = dt &
							( TEV_DATA_SCALAR | TEV_DATA_ARRAY );

						if ( !trc_unpack_data_value( DD ) )
						{
							trc_free_tevdesc( &TD );

							return( ecnt );
						}
					}

					else
						desc_done++;
				}
				while ( !desc_done );

				dump_event = trc_add_tevdesc( ID, &TD, src, omit );

				if ( TD->dump )
					trc_dump_tevdesc( ID, TD, src );

				if ( dump_event )
					trc_store_data_values( ID, TD, src, omit );

				if ( !omit )
					ecnt++;

				if ( !nest )
					done++;

				break;
			}

			case TEV_MARK_EVENT_RECORD:
			{
				TRC_PVMCKERR( pvm_upkint( &eid, 1, 1 ),
					"Event ID", return( ecnt ) );

				if ( eid & TEV_EVENT_ENTRY )
					entry_exit = TRC_ENTRY_TEV;

				else if ( eid & TEV_EVENT_EXIT )
					entry_exit = TRC_EXIT_TEV;

				else
					entry_exit = TRC_IGNORE_TEV;

				eid &= ~( TEV_EVENT_ENTRY | TEV_EVENT_EXIT );

				TD = trc_get_tevdesc( TT, H, eid, entry_exit );

				if ( TD != NULL )
				{
					if ( trc_store_trace_event( ID, TD, src, omit ) )
						ecnt++;
				}

				else
				{
					printf(
						"Error: Event ID %d(%d) %s T(%d/%x) H(%d/%x)\n",
						eid, entry_exit, "Descriptor Not Found",
						TT->tid, TT->tid, H->pvmd_tid, H->pvmd_tid );
				}

				if ( !nest )
					done++;

				break;
			}
		}

		if ( !done )
		{
			TRC_PVMCKERR( pvm_upkint( &marker, 1, 1 ),
				"Unpacking Trace Message", return( ecnt ) );
		}
	}
	while ( !done );

	return( ecnt );
}


int
trc_handle_output_message( ID )
TRC_ID ID;
{
	TRC_TEVTASK TT;

	char tmp[255];
	char msg[255];

	char *output;

	int ecnt;
	int omit;
	int len;
	int tid;

	ecnt = 0;

	TRC_PVMCKERR( pvm_upkint( &tid, 1, 1 ),
		"Error Unpacking Output Message", return( ecnt ) );

	TRC_PVMCKERR( pvm_upkint( &len, 1, 1 ),
		"Error Unpacking Output Length", return( ecnt ) );

	/* Set Leftover Event Omit Flag */

	TT = trc_get_tevtask_tid( ID, tid );

	omit = ( ID->trace_out == NULL
		|| ( TT != NULL && TRC_TASK_OMIT( TT ) ) )
			? TRC_TRUE : TRC_FALSE;

	/* Unpack Output */

	if ( len > 0 )
	{
		output = (char *) malloc( (unsigned) ( len + 1 )
			* sizeof(char) );
		trc_memcheck( output, "PVM Output String" );

		TRC_PVMCKERR( pvm_upkbyte( output, len, 1 ),
			"Error Unpacking Task Output",
			free(output); return( ecnt ) );

		output[ len ] = '\0';

		if ( !omit )
			ecnt = trc_store_output_event( ID, tid, output, len );

		free( output );
	}

	else
	{
		switch ( len )
		{
			case 0:
			{
				if ( !omit )
				{
					sprintf( tmp, "GOTEOF\n" );

					ecnt = trc_store_output_event( ID,
						tid, tmp, strlen(tmp) );
				}

				if ( TT != NULL )
				{
					if ( TT->tevstatus == TRC_TASK_ALIVE )
					{
						TT->outstatus = TRC_TASK_EOF;

						trc_check_for_dead_host( ID, TT );

						if ( !trc_tevtasks_alive( ID ) )
							trc_end_trace( ID );
					}

					else
						TT->outstatus = TRC_TASK_EOF;
				}

				else if ( !omit )
				{
					printf(
						"\nWarning: GOTEOF Unknown Task TID=0x%x.\n\n",
						tid );
				}

				break;
			}

			case -1:
			{
				if ( omit )
					break;

				sprintf( tmp, "GOTSPAWN\n" );

				ecnt = trc_store_output_event( ID,
					tid, tmp, strlen(tmp) );

				if ( TT == NULL )
				{
					sprintf( msg, "Task TID=0x%x Connected to %s",
						tid, TRC_NAME );

					trc_status_msg( ID, msg );

					TT = trc_create_tevtask();

					TT->tid = tid;

					TT->outstatus = TRC_TASK_OUT;
					TT->tevstatus = TRC_TASK_NOLIFE;

					TT->next = ID->tevtask_list;

					ID->tevtask_list = TT;
				}

				else
					TT->outstatus = TRC_TASK_OUT;

				break;
			}

			case -2:
			{
				if ( omit )
					break;

				sprintf( tmp, "CREATION\n" );

				ecnt = trc_store_output_event( ID,
					tid, tmp, strlen(tmp) );

				if ( TT == NULL )
				{
					sprintf( msg, "Task TID=0x%x Connected to %s",
						tid, TRC_NAME );

					trc_status_msg( ID, msg );

					TT = trc_create_tevtask();

					TT->tid = tid;

					TT->outstatus = TRC_TASK_OUT;
					TT->tevstatus = TRC_TASK_NOLIFE;

					TT->next = ID->tevtask_list;

					ID->tevtask_list = TT;
				}

				else
					TT->outstatus = TRC_TASK_OUT;

				break;
			}

			default:
			{
				if ( !omit )
				{
					fprintf( stderr,
						"Console: unknown output msg 0x%x?\n", tid );
				}
			}
		}
	}

	return( ecnt );
}

