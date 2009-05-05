
static char rcsid[] = 
	"$Id: trcfile.c,v 4.15 1998/11/02 14:35:57 pvmsrc Exp $";

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


/* Trace Lookup Trie Init Routines */

void
trc_init_did_trie()
{
	TRC_DID dptr;

	int i;

	TRC_DID_TRIE = trc_create_triestack();

	for ( i=TEV_DID_FIRST ; i <= TEV_DID_MAX ; i++ )
	{
		dptr = trc_create_did();

		dptr->id = i;

		dptr->name = trc_copy_str( pvmtevdidlist[i].did );

		dptr->desc = trc_copy_str( pvmtevdidlist[i].desc );

		dptr->next = TRC_DID_LIST;

		TRC_DID_LIST = dptr;

		trc_add_to_trie( TRC_DID_TRIE, dptr->name, (void *) dptr );

		TRC_LOCAL_DID[i] = dptr;
	}
}


/* Trace File Generation Routines */

void
trc_set_didlist( H, D )
TRC_HOST H;
TRC_DID D;
{
	if ( H != NULL )
	{
		trc_check_listsize( &(H->didlist), &(H->didsize), D->id );

		H->didlist[ D->id ] = (void *) D;
	}

	else
		printf( "Error: No Host for TEV Semantic Identifier List\n" );
}


TRC_DID
trc_get_did( H, id )
TRC_HOST H;
int id;
{
	TRC_DID D;

	D = (TRC_DID) NULL;

	/* Use Host Lookup */

	if ( H != NULL && H->didlist != NULL && id < H->didsize )
		D = (TRC_DID) (H->didlist[ id ]);

	/* Use Default Local Lookup */

	else if ( id >= TEV_DID_FIRST && id <= TEV_DID_MAX )
		D = TRC_LOCAL_DID[ id ];

	return( D );
}


int
trc_add_tevdesc( ID, TDptr, tid, omit )
TRC_ID ID;
TRC_TEVDESC *TDptr;
int tid;
int omit;
{
	TRC_TEVDESC *newlist;

	TRC_TEVDESC tdptr;
	TRC_TEVDESC last;

	int newsize;
	int index;
	int eid;
	int i;

	/* Verify Descriptor List is Large Enough */

	eid = (*TDptr)->eid;

	if ( eid >= TRC_TEVDESC_SIZE )
	{
		newsize = 2 * ( eid + 1 );

		newlist = (TRC_TEVDESC *) malloc( (unsigned) newsize
			* sizeof( TRC_TEVDESC ) );
		trc_memcheck( newlist, "New TRC_TEVDESC Pointer List" );

		for ( i=0 ; i < TRC_TEVDESC_SIZE ; i++ )
		{
			newlist[i] = TRC_TEVDESC_LIST[i];

			TRC_TEVDESC_LIST[i] = (TRC_TEVDESC) NULL;
		}

		i = ( TRC_TEVDESC_SIZE >= 0 ) ? TRC_TEVDESC_SIZE : 0;

		while ( i < newsize )
		{
			newlist[i] = (TRC_TEVDESC) NULL;

			i++;
		}

		free( TRC_TEVDESC_LIST );

		TRC_TEVDESC_LIST = newlist;

		TRC_TEVDESC_SIZE = newsize;
	}

	/* Search for Existing Descriptor */

	tdptr = TRC_TEVDESC_LIST[ eid ];

	last = (TRC_TEVDESC) NULL;

	index = 0;

	while ( tdptr != NULL )
	{
		if ( trc_cmp_tevdesc( *TDptr, tdptr ) )
		{
			if ( !omit && tdptr->dump && ID != NULL )
				trc_dump_tevdesc( ID, tdptr, tid );

			(*TDptr)->index = tdptr->index;

			if ( ID != NULL )
				trc_store_data_values( ID, *TDptr, tid, omit );

			trc_free_tevdesc( TDptr );

			*TDptr = tdptr;

			(tdptr->refcount)++;

			return( TRC_FALSE );
		}

		index++;

		last = tdptr;

		tdptr = tdptr->next;
	}

	/* Add New Descriptor */

	if ( last != NULL )
		last->next = *TDptr;

	else
		TRC_TEVDESC_LIST[ eid ] = *TDptr;

	(*TDptr)->index = index;

	if ( omit )
		(*TDptr)->dump = TRC_FALSE;

	else
		(*TDptr)->dump = TRC_TRUE;

	return( TRC_TRUE );
}


void
trc_set_tevlist( TT, H, TD )
TRC_TEVTASK TT;
TRC_HOST H;
TRC_TEVDESC TD;
{
	if ( TT != NULL )
	{
		trc_check_listsize( &(TT->tevlist), &(TT->tevsize),
			TRC_TEVINDEX_OF( TD->eid, TRC_MAX_TEV ) );

		TT->tevlist[ TRC_TEVINDEX_OF( TD->eid, TD->entry_exit ) ] =
			(void *) TD;
	}

	else if ( H != NULL )
	{
		trc_check_listsize( &(H->tevlist), &(H->tevsize),
			TRC_TEVINDEX_OF( TD->eid, TRC_MAX_TEV ) );

		H->tevlist[ TRC_TEVINDEX_OF( TD->eid, TD->entry_exit ) ] =
			(void *) TD;
	}

	else
		printf( "Error: No Task or Host for TEV Index List\n" );
}


TRC_TEVDESC
trc_get_tevdesc( TT, H, eid, entry_exit )
TRC_TEVTASK TT;
TRC_HOST H;
int eid;
int entry_exit;
{
	TRC_TEVDESC TD;

	if ( TT != NULL )
	{
		TD = (TRC_TEVDESC)
			(TT->tevlist[ TRC_TEVINDEX_OF(eid,entry_exit) ]);
	}

	else if ( H != NULL )
	{
		TD = (TRC_TEVDESC)
			(H->tevlist[ TRC_TEVINDEX_OF(eid,entry_exit) ]);
	}

	else
		TD = (TRC_TEVDESC) NULL;

	return( TD );
}


void
trc_dump_tevdesc( ID, TD, tid )
TRC_ID ID;
TRC_TEVDESC TD;
int tid;
{
	TRC_DATADESC DD;

	TRC_DID D;

	int user_defined;
	int dummy;

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	fprintf( ID->trace_out, "\n" );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	if ( TD->index >= 0 )
	{
		fprintf( ID->trace_out, "#%d: \"%s(",  /* ) */
			( 1000 * ( TD->index + 1 ) ) + ( 2 * TD->eid )
				+ ( ( TD->entry_exit == TRC_EXIT_TEV ) ? 1 : 0 ),
			TD->name );

		if ( TD->entry_exit == TRC_ENTRY_TEV )
			fprintf( ID->trace_out, "0." );

		else if ( TD->entry_exit == TRC_EXIT_TEV )
			fprintf( ID->trace_out, "1." );

		fprintf( ID->trace_out, "%d", TD->index );

		fprintf( ID->trace_out, /* ( */ ")\"\n" );
	}

	else
		fprintf( ID->trace_out, "#%d: \"%s\"\n", TD->eid, TD->name );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	fprintf( ID->trace_out, "{\n" );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	/* Sneak in Task ID for User Defined Events */

	user_defined = !strcmp( TD->name, "user_defined" );

	if ( user_defined )
	{
		D = trc_get_did( (TRC_HOST) NULL, TEV_DID_TID );

		fprintf( ID->trace_out, "\t// \"%s\" \"%s\"\n",
			D->name, D->desc );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		fprintf( ID->trace_out, "\t%s \"%s\";\n",
			TRC_TYPE_STRS[ TEV_DATA_INT ], D->name );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );
	}

	/* Dump (Remainder) of Descriptor */

	DD = TD->ddesc;

	dummy = 0;

	while ( DD != NULL )
	{
		if ( !user_defined || DD->did != NULL )
		{
			fprintf( ID->trace_out, "\t// \"%s\" \"%s\"\n",
				DD->did->name, DD->did->desc );
		}

		else
		{
			fprintf( ID->trace_out,
				"\t// \"USR%d\" \"User Defined Value #%d\"\n",
				dummy, dummy );
		}

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		if ( !user_defined || DD->did != NULL )
		{
			fprintf( ID->trace_out, "\t%s \"%s\"",
				TRC_TYPE_STRS[ DD->dt ], DD->did->name );
		}

		else
		{
			fprintf( ID->trace_out, "\t%s \"USR%d\"",
				TRC_TYPE_STRS[ DD->dt ], dummy );

			dummy++;
		}

		if ( DD->array == TEV_DATA_ARRAY )
			fprintf( ID->trace_out, "[]" );

		if ( DD->dt == TEV_DATA_STRING )
			fprintf( ID->trace_out, "[]" );

		fprintf( ID->trace_out, ";\n" );

		if ( ID->event_dump_hdr )
			(ID->event_dump_hdr)( ID, tid );

		DD = DD->next;
	}

	fprintf( ID->trace_out, "};;\n" );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	fprintf( ID->trace_out, "\n" );

	TD->dump = TRC_FALSE;
}


void
trc_check_listsize( list, size, index )
void ***list;
int *size;
int index;
{
	void **newlist;

	int newsize;
	int i;

	if ( index >= *size )
	{
		newsize = 2 * index;

		newsize = newsize ? newsize : 100;

		newlist = (void **) malloc( (unsigned) newsize
			* sizeof(void *) );
		trc_memcheck( newlist, "New TRC_TEVDESC Index List" );

		for ( i=0 ; i < *size ; i++ )
		{
			newlist[i] = (*list)[i];

			(*list)[i] = (void *) NULL;
		}

		for ( i=( ((*size) >= 0) ? (*size) : 0 ) ; i < newsize ; i++ )
			newlist[i] = (void *) NULL;

		if ( *list )
			free( *list );

		*list = newlist;
		*size = newsize;
	}
}


void
trc_store_event_header( ID, TD, tid )
TRC_ID ID;
TRC_TEVDESC TD;
int tid;
{
	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, tid );

	if ( TD->index >= 0 )
	{
		fprintf( ID->trace_out, "\"%s(", TD->name );

		if ( TD->entry_exit == TRC_ENTRY_TEV )
			fprintf( ID->trace_out, "0." );

		else if ( TD->entry_exit == TRC_EXIT_TEV )
			fprintf( ID->trace_out, "1." );

		fprintf( ID->trace_out, "%d", TD->index );

		fprintf( ID->trace_out, ")\" { " );

		/* matching } */
	}

	else
		fprintf( ID->trace_out, "\"%s\" { ", TD->name );
		/* matching } */
}


void
trc_store_data_values( ID, TD, tid, omit )
TRC_ID ID;
TRC_TEVDESC TD;
int tid;
int omit;
{
	TRC_DATADESC DD;

	TRC_TEVTASK TT;

	char msg[1024];

	char *str;

	int user_defined;
	int newtask;
	int endtask;
	int maxlen;
	int ignore;
	int tusec;
	int tsec;
	int len;
	int i;

	/* Set New/End Task Flags */

	TT = (TRC_TEVTASK) NULL;

	tsec = tusec = -1;

	user_defined = TRC_FALSE;
	newtask = TRC_FALSE;
	endtask = TRC_FALSE;

	if ( !strcmp( TD->name, "newtask" )
		|| !strcmp( TD->name, "spntask" ) )
	{
		newtask = TRC_TRUE;
	}

	else if ( !strcmp( TD->name, "endtask" ) )
		endtask = TRC_TRUE;

	else if ( !strcmp( TD->name, "user_defined" ) )
		user_defined = TRC_TRUE;

	ignore = TRC_FALSE;

	if ( !omit )
	{
		/* Store Event Header */

		trc_store_event_header( ID, TD, tid );

		/* Sneak in Task ID for User Defined Events */

		if ( user_defined )
			fprintf( ID->trace_out, "%d, ", tid );
	}

	/* Store Data Values */

	DD = TD->ddesc;

	while ( DD != NULL )
	{
		/* Dump Data Value */

		if ( !omit && DD->array == TEV_DATA_ARRAY )
		{
			if ( DD->dt == TEV_DATA_CPLX || DD->dt == TEV_DATA_DCPLX )
				fprintf( ID->trace_out, "[%d] { ", DD->num * 2 );

			/* matching } */

			else if ( DD->dt == TEV_DATA_STRING )
				fprintf( ID->trace_out, "[%d] ", DD->num );

			else if ( DD->dt == TEV_DATA_BYTE )
				fprintf( ID->trace_out, "[%d] { ", DD->num + 1 );

			else
				fprintf( ID->trace_out, "[%d] { ", DD->num );

			/* matching } } */
		}

		switch ( DD->dt )
		{
			case TEV_DATA_NULL: break;

			case TEV_DATA_BYTE:
			{
				if ( omit )
					break;

				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%c",
						TRC_ARR_VALUE_OF( DD->data, char, i ) );
				}

				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				break;
			}

			case TEV_DATA_CPLX:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%f, %f",
						TRC_ARR_VALUE_OF( DD->data, float, 2 * i ),
						TRC_ARR_VALUE_OF( DD->data, float,
							(2 * i) + 1 ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_DCPLX:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%lf, %lf",
						TRC_ARR_VALUE_OF( DD->data, double, 2 * i ),
						TRC_ARR_VALUE_OF( DD->data, double,
							(2 * i) + 1 ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_DOUBLE:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%lf",
						TRC_ARR_VALUE_OF( DD->data, double, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_FLOAT:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%f",
						TRC_ARR_VALUE_OF( DD->data, float, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_INT:
			{
				if ( !omit )
				{
					for ( i=0 ; i < DD->num ; i++ )
					{
						fprintf( ID->trace_out, "%d",
							TRC_ARR_VALUE_OF( DD->data, int, i ) );

						if ( i < DD->num - 1 )
							fprintf( ID->trace_out, ", " );
					}
				}

				if ( ( newtask || endtask )
					&& !strcmp( DD->did->name, "TID" ) )
				{
					if ( !omit && newtask )
					{
						TT = trc_get_tevtask_tid( ID,
							TRC_VALUE_OF( DD->data, int ) );

						if ( TT == NULL )
						{
							if ( !ignore )
							{
								sprintf( msg,
									"Task TID=0x%x Connected to %s",
									TRC_VALUE_OF( DD->data, int ),
									TRC_NAME );

								trc_status_msg( ID, msg );
							}

							TT = trc_create_tevtask();

							TT->tid = TRC_VALUE_OF( DD->data, int );

							TT->outstatus = TRC_TASK_NOOUT;

							TT->next = ID->tevtask_list;

							ID->tevtask_list = TT;
						}

						if ( !ignore )
							TT->tevstatus = TRC_TASK_ALIVE;
					
						else
							TT->tevstatus = TRC_TASK_IGNORE;
					}

					if ( endtask )
					{
						TT = trc_get_tevtask_tid( ID,
							TRC_VALUE_OF( DD->data, int ) );

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
							TRC_VALUE_OF( DD->data, int ) );
						}
					}
				}

				else if ( !user_defined
					&& !strcmp( DD->did->name, "TS" ) )
				{
					tsec = TRC_VALUE_OF( DD->data, int );
				}
				
				else if ( !user_defined
					&& !strcmp( DD->did->name, "TU" ) )
				{
					tusec = TRC_VALUE_OF( DD->data, int );
				}

				break;
			}

			case TEV_DATA_UINT:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%u", (unsigned)
						TRC_ARR_VALUE_OF( DD->data, int, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_LONG:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%ld",
						TRC_ARR_VALUE_OF( DD->data, long, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_ULONG:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%ld",
						TRC_ARR_VALUE_OF( DD->data, long, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_SHORT:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%d",
						TRC_ARR_VALUE_OF( DD->data, short, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_USHORT:
			{
				if ( omit )
					break;

				for ( i=0 ; i < DD->num ; i++ )
				{
					fprintf( ID->trace_out, "%u",
						TRC_ARR_VALUE_OF( DD->data, short, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_STRING:
			{
				if ( omit )
					break;

				if ( DD->num < 1 )
					break;

				maxlen = 0;

				for ( i=0 ; i < DD->num ; i++ )
				{
					len = strlen(
						TRC_ARR_VALUE_OF( DD->data, char *, i ) );

					if ( len > maxlen )
						maxlen = len;
				}

				if ( newtask && !strcmp( DD->did->name, "TN" ) )
				{
					if ( !(ID->group_tasks)
						&& TRC_GROUPTASK( (char *) DD->data ) )
					{
						ignore = TRC_TRUE;

						if ( TT != NULL )
						{
							sprintf( msg, "Ignoring Task \"%s\"\n",
								(char *) DD->data );

							trc_status_msg( ID, msg );

							TT->tevstatus = TRC_TASK_IGNORE;
						}
					}
				}

				fprintf( ID->trace_out, "[%d] { ", maxlen + 1 );

				for ( i=0 ; i < DD->num ; i++ )
				{
					str = TRC_ARR_VALUE_OF( DD->data, char *, i );

					TRC_ARR_VALUE_OF( DD->data, char *, i ) =
						(char *) NULL;

					fprintf( ID->trace_out, "\"%s\"", str );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );

					free( str );
				}

				if ( DD->array == TEV_DATA_SCALAR )
					fprintf( ID->trace_out, " }" );

				break;
			}

			case TEV_DATA_STRUCT_START:
			case TEV_DATA_STRUCT_END:
			case TEV_DATA_DEFERRED:
			{
				if ( !omit )
					printf( "DT %d Not Impl\n", DD->dt );

				break;
			}

			default:
			{
				if ( !omit )
					printf( "DT %d Unknown\n", DD->dt );
			}
		}

		if ( !omit )
		{
			/* matching { */

			if ( DD->array == TEV_DATA_ARRAY )
				fprintf( ID->trace_out, " }" );

			if ( DD->next != NULL )
				fprintf( ID->trace_out, ", " );
		}

		/* Free Data Value */

		free( DD->data );

		DD->data = (TRC_VALUE) NULL;

		DD->num = -1;

		/* Next Data Value */

		DD = DD->next;
	}

	if ( !omit )
	{
		/* matching { */

		fprintf( ID->trace_out, " };;\n" );

		if ( tsec != -1 && tusec != -1 )
			trc_update_trace_time( tsec, tusec );
	}
}


int
trc_store_trace_event( ID, TD, tid, omit )
TRC_ID ID;
TRC_TEVDESC TD;
int tid;
int omit;
{
	TRC_DATADESC DD;

	TRC_TEVTASK TT;

	char	upk_byte[TRC_MAX_UNPACK_ARR_SIZE];
	float	upk_float[TRC_MAX_UNPACK_ARR_SIZE * 2];
	double	upk_double[TRC_MAX_UNPACK_ARR_SIZE * 2];
	int		upk_int[TRC_MAX_UNPACK_ARR_SIZE];
	long	upk_long[TRC_MAX_UNPACK_ARR_SIZE];
	short	upk_short[TRC_MAX_UNPACK_ARR_SIZE];
	char	upk_str[TRC_MAX_UNPACK_ARR_SIZE];

	char **strarr;

	char msg[1024];

	int newtask;
	int endtask;
	int marker;
	int maxlen;
	int ignore;
	int tusec;
	int tsec;
	int len;
	int num;
	int i;

	/* Dump Descriptor (if necessary) */

	if ( !omit && TD->dump )
		trc_dump_tevdesc( ID, TD, tid );

	/* Set New Task Flag */

	TT = (TRC_TEVTASK) NULL;

	tsec = tusec = -1;

	newtask = TRC_FALSE;
	endtask = TRC_FALSE;

	if ( !strcmp( TD->name, "newtask" )
		|| !strcmp( TD->name, "spntask" ) )
	{
		newtask = TRC_TRUE;
	}

	else if ( !strcmp( TD->name, "endtask" ) )
		endtask = TRC_TRUE;

	ignore = TRC_FALSE;

	/* Store Event Header */

	if ( !omit )
		trc_store_event_header( ID, TD, tid );

	/* Store Remainder of Event */

	DD = TD->ddesc;

	while ( DD != NULL )
	{
		if ( DD->array == TEV_DATA_ARRAY )
		{
			TRC_PVMCKERR( pvm_upkint( &num, 1, 1 ),
				"Array Unpack", return( TRC_FALSE ) );

			if ( num > TRC_MAX_UNPACK_ARR_SIZE
				&& DD->dt != TEV_DATA_STRING )
			{
				if ( !omit )
					printf( "Error: Unpack Size %d Too Large\n", num );

				return( TRC_FALSE );
			}

			if ( !omit )
			{
				if ( DD->dt == TEV_DATA_CPLX
					|| DD->dt == TEV_DATA_DCPLX )
				{
					fprintf( ID->trace_out, "[%d] { ", num * 2 );
					/* matching } */
				}

				else if ( DD->dt == TEV_DATA_STRING )
					fprintf( ID->trace_out, "[%d] ", num );

				else if ( DD->dt == TEV_DATA_BYTE )
				{
					fprintf( ID->trace_out, "[%d] { ", num + 1 );
					/* matching } */
				}

				else
				{
					fprintf( ID->trace_out, "[%d] { ", num );
					/* matching } */
				}
			}
		}

		else
			num = 1;

		switch ( DD->dt )
		{
			case TEV_DATA_NULL: break;

			case TEV_DATA_BYTE:
			{
				TRC_PVMCKERR( pvm_upkbyte( upk_byte, num, 1 ),
					"Event BYTE Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				for ( i=0 ; i < num ; i++ )
					fprintf( ID->trace_out, "%c", upk_byte[i] );

				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				break;
			}

			case TEV_DATA_CPLX:
			{
				TRC_PVMCKERR( pvm_upkfloat( upk_float, num * 2, 1 ),
					"Event CPLX Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%f, %f",
						upk_float[ 2 * i ],
						upk_float[ (2 * i) + 1 ] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_DCPLX:
			{
				TRC_PVMCKERR( pvm_upkdouble( upk_double, num * 2, 1 ),
					"Event DCPLX Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%lf, %lf",
						upk_double[ 2 * i ],
						upk_double[ (2 * i) + 1 ] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_DOUBLE:
			{
				TRC_PVMCKERR( pvm_upkdouble( upk_double, num, 1 ),
					"Event DOUBLE Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%lf", upk_double[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_FLOAT:
			{
				TRC_PVMCKERR( pvm_upkfloat( upk_float, num, 1 ),
					"Event FLOAT Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%f", upk_float[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_INT:
			{
				TRC_PVMCKERR( pvm_upkint( upk_int, num, 1 ),
					"Event INT Unpack", return( TRC_FALSE ) );

				if ( !omit )
				{
					for ( i=0 ; i < num ; i++ )
					{
						fprintf( ID->trace_out, "%d", upk_int[i] );

						if ( i < num - 1 )
							fprintf( ID->trace_out, ", " );
					}
				}

				if ( ( newtask || endtask )
					&& !strcmp( DD->did->name, "TID" ) )
				{
					if ( !omit && newtask )
					{
						TT = trc_get_tevtask_tid( ID, upk_int[0] );

						if ( TT == NULL )
						{
							if ( !ignore )
							{
								sprintf( msg,
									"Task TID=0x%x Connected to %s",
									upk_int[0], TRC_NAME );
								
								trc_status_msg( ID, msg );
							}

							TT = trc_create_tevtask();

							TT->tid = upk_int[0];

							TT->outstatus = TRC_TASK_NOOUT;

							TT->next = ID->tevtask_list;

							ID->tevtask_list = TT;
						}

						if ( !ignore )
							TT->tevstatus = TRC_TASK_ALIVE;
					
						else
							TT->tevstatus = TRC_TASK_IGNORE;
					}

					if ( endtask )
					{
						TT = trc_get_tevtask_tid( ID, upk_int[0] );

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
							upk_int[0] );
						}
					}
				}

				else if ( !strcmp( DD->did->name, "TS" ) )
					tsec = upk_int[0];
				
				else if ( !strcmp( DD->did->name, "TU" ) )
					tusec = upk_int[0];

				break;
			}

			case TEV_DATA_UINT:
			{
				TRC_PVMCKERR( pvm_upkint( upk_int, num, 1 ),
					"Event UINT Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%u",
						(unsigned) upk_int[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_LONG:
			{
				TRC_PVMCKERR( pvm_upklong( upk_long, num, 1 ),
					"Event LONG Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%ld", upk_long[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_ULONG:
			{
				TRC_PVMCKERR( pvm_upklong( upk_long, num, 1 ),
					"Event ULONG Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%ld", upk_long[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_SHORT:
			{
				TRC_PVMCKERR( pvm_upkshort( upk_short, num, 1 ),
					"Event SHORT Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%d", upk_short[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_USHORT:
			{
				TRC_PVMCKERR( pvm_upkshort( upk_short, num, 1 ),
					"Event USHORT Unpack", return( TRC_FALSE ) );

				if ( omit )
					break;

				for ( i=0 ; i < num ; i++ )
				{
					fprintf( ID->trace_out, "%u", upk_short[i] );

					if ( i < num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_STRING:
			{
				if ( num < 1 )
					break;

				strarr = (char **) malloc( (unsigned) num
						* sizeof(char *) );
				trc_memcheck( strarr, "String Array" );

				maxlen = 0;

				for ( i=0 ; i < num ; i++ )
				{
					TRC_PVMCKERR( pvm_upkstr( upk_str ),
						"Event STRING Unpack", return( TRC_FALSE ) );

					if ( (len = strlen( upk_str )) > maxlen )
						maxlen = len;

					strarr[i] = trc_copy_str( upk_str );
				}

				if ( !omit && newtask
					&& !strcmp( DD->did->name, "TN" ) )
				{
					if ( !(ID->group_tasks)
						&& TRC_GROUPTASK( upk_str ) )
					{
						ignore = TRC_TRUE;

						if ( TT != NULL )
						{
							sprintf( msg,
								"Ignoring Task \"%s\"", upk_str );

							trc_status_msg( ID, msg );

							TT->tevstatus = TRC_TASK_IGNORE;
						}
					}
				}

				if ( !omit )
				{
					fprintf( ID->trace_out, "[%d] { ", maxlen + 1 );

					for ( i=0 ; i < num ; i++ )
					{
						fprintf( ID->trace_out, "\"%s\"", strarr[i] );

						if ( i < num - 1 )
							fprintf( ID->trace_out, ", " );

						free( strarr[i] );
					}

					if ( DD->array == TEV_DATA_SCALAR )
						fprintf( ID->trace_out, " }" );
				}

				else
				{
					for ( i=0 ; i < num ; i++ )
						free( strarr[i] );
				}

				free( strarr );

				break;
			}

			case TEV_DATA_STRUCT_START:
			case TEV_DATA_STRUCT_END:
			case TEV_DATA_DEFERRED:
			{
				if ( !omit )
					printf( "DT Not Impl\n" );

				break;
			}

			default:
			{
				if ( !omit )
					printf( "DT Unknown\n" );
			}
		}

		if ( !omit )
		{
			/* matching { */

			if ( DD->array == TEV_DATA_ARRAY )
				fprintf( ID->trace_out, " }" );

			if ( DD->next != NULL )
				fprintf( ID->trace_out, ", " );
		}

		DD = DD->next;
	}

	TRC_PVMCKERR( pvm_upkint( &marker, 1, 1 ),
		"Data ID Message", return( TRC_FALSE ) );

	if ( !omit )
	{
		/* matching { */

		if ( marker == TEV_MARK_EVENT_RECORD_END )
			fprintf( ID->trace_out, " };;\n" );

		else
		{
			printf( "Error: No Event End Marker\n" );

			return( TRC_FALSE );
		}

		if ( tsec != -1 && tusec != -1 )
			trc_update_trace_time( tsec, tusec );

		return( TRC_TRUE );
	}

	else
		return( TRC_FALSE );
}


trc_unpack_data_value( DD )
TRC_DATADESC DD;
{
	char upk_str[TRC_MAX_UNPACK_ARR_SIZE];

	int num;
	int i;

	if ( DD->array == TEV_DATA_ARRAY )
	{
		TRC_PVMCKERR( pvm_upkint( &num, 1, 1 ),
			"Array Unpack", return( TRC_FALSE ) );
	}

	else
		num = 1;

	DD->data = trc_make_value( DD->dt, num );

	DD->num = num;

	switch ( DD->dt )
	{
		case TEV_DATA_NULL: break;

		case TEV_DATA_BYTE:
		{
			TRC_PVMCKERR( pvm_upkbyte( DD->data, num, 1 ),
				"Event BYTE Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_CPLX:
		{
			TRC_PVMCKERR( pvm_upkfloat( DD->data, num * 2, 1 ),
				"Event CPLX Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_DCPLX:
		{
			TRC_PVMCKERR( pvm_upkdouble( DD->data, num * 2, 1 ),
				"Event DCPLX Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_DOUBLE:
		{
			TRC_PVMCKERR( pvm_upkdouble( DD->data, num, 1 ),
				"Event DOUBLE Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_FLOAT:
		{
			TRC_PVMCKERR( pvm_upkfloat( DD->data, num, 1 ),
				"Event FLOAT Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_INT:
		{
			TRC_PVMCKERR( pvm_upkint( DD->data, num, 1 ),
				"Event INT Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_UINT:
		{
			TRC_PVMCKERR( pvm_upkint( DD->data, num, 1 ),
				"Event UINT Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_LONG:
		{
			TRC_PVMCKERR( pvm_upklong( DD->data, num, 1 ),
				"Event LONG Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_ULONG:
		{
			TRC_PVMCKERR( pvm_upklong( DD->data, num, 1 ),
				"Event ULONG Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_SHORT:
		{
			TRC_PVMCKERR( pvm_upkshort( DD->data, num, 1 ),
				"Event SHORT Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_USHORT:
		{
			TRC_PVMCKERR( pvm_upkshort( DD->data, num, 1 ),
				"Event USHORT Unpack", return( TRC_FALSE ) );

			break;
		}

		case TEV_DATA_STRING:
		{
			if ( num < 1 )
				break;

			DD->data = (TRC_VALUE) malloc( (unsigned) num
					* sizeof(char *) );
			trc_memcheck( DD->data, "String Array" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_PVMCKERR( pvm_upkstr( upk_str ),
					"Event STRING Unpack", return( TRC_FALSE ) );

				((char **) DD->data)[i] = trc_copy_str( upk_str );
			}

			break;
		}

		case TEV_DATA_STRUCT_START:
		case TEV_DATA_STRUCT_END:
		case TEV_DATA_DEFERRED:
			printf( "DT %d Not Impl\n", DD->dt );
			break;

		default:
			printf( "DT %d Unknown\n", DD->dt );
	}

	return( TRC_TRUE );
}


void
trc_store_tevrec( ID, TD, TR, tid )
TRC_ID ID;
TRC_TEVDESC TD;
TRC_TEVREC TR;
int tid;
{
	TRC_DATADESC DD;

	TRC_TEVREC trptr;

	char *str;

	int user_defined;
	int maxlen;
	int len;
	int i;

	/* Set New/End Task Flags */

	user_defined = TRC_FALSE;

	if ( !strcmp( TD->name, "user_defined" ) )
		user_defined = TRC_TRUE;

	/* Store Event Header */

	trc_store_event_header( ID, TD, tid );

	/* Sneak in Task ID for User Defined Events */

	if ( user_defined )
		fprintf( ID->trace_out, "%d, ", tid );

	/* Store Data Values */

	DD = TD->ddesc;

	trptr = TR;

	while ( DD != NULL )
	{
		/* Dump Data Value */

		if ( DD->array == TEV_DATA_ARRAY )
		{
			if ( DD->dt == TEV_DATA_STRING )
				fprintf( ID->trace_out, "[%d] ", trptr->num );

			else
				fprintf( ID->trace_out, "[%d] { ", trptr->num );

			/* matching } */
		}

		switch ( DD->dt )
		{
			case TEV_DATA_NULL: break;

			case TEV_DATA_BYTE:
			{
				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				for ( i=0 ; i < trptr->num - 1 ; i++ )
				{
					fprintf( ID->trace_out, "%c",
						TRC_ARR_VALUE_OF( trptr->value, char, i ) );
				}

				if ( DD->array == TEV_DATA_ARRAY )
					fprintf( ID->trace_out, "\"" );

				break;
			}

			case TEV_DATA_CPLX:
			case TEV_DATA_FLOAT:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%f",
						TRC_ARR_VALUE_OF( trptr->value, float, i ) );

					if ( i < DD->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_DCPLX:
			case TEV_DATA_DOUBLE:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%lf",
						TRC_ARR_VALUE_OF( trptr->value, double, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_INT:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%d",
						TRC_ARR_VALUE_OF( trptr->value, int, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_UINT:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%u", (unsigned)
						TRC_ARR_VALUE_OF( trptr->value, int, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_LONG:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%ld",
						TRC_ARR_VALUE_OF( trptr->value, long, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_ULONG:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%ld",
						TRC_ARR_VALUE_OF( trptr->value, long, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_SHORT:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%d",
						TRC_ARR_VALUE_OF( trptr->value, short, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_USHORT:
			{
				for ( i=0 ; i < trptr->num ; i++ )
				{
					fprintf( ID->trace_out, "%u",
						TRC_ARR_VALUE_OF( trptr->value, short, i ) );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );
				}

				break;
			}

			case TEV_DATA_STRING:
			{
				if ( trptr->num < 1 )
					break;

				maxlen = 0;

				for ( i=0 ; i < trptr->num ; i++ )
				{
					len = strlen(
						TRC_ARR_VALUE_OF( trptr->value, char *, i ) );

					if ( len > maxlen )
						maxlen = len;
				}

				fprintf( ID->trace_out, "[%d] { ", maxlen + 1 );

				for ( i=0 ; i < trptr->num ; i++ )
				{
					str = TRC_ARR_VALUE_OF( trptr->value, char *, i );

					fprintf( ID->trace_out, "\"%s\"", str );

					if ( i < trptr->num - 1 )
						fprintf( ID->trace_out, ", " );

					free( str );
				}

				if ( DD->array == TEV_DATA_SCALAR )
					fprintf( ID->trace_out, " }" );

				break;
			}

			case TEV_DATA_STRUCT_START:
			case TEV_DATA_STRUCT_END:
			case TEV_DATA_DEFERRED:
			{
				printf( "DT %d Not Impl\n", DD->dt );

				break;
			}

			default:
			{
				printf( "DT %d Unknown\n", DD->dt );
			}
		}

		/* matching { */

		if ( DD->array == TEV_DATA_ARRAY )
			fprintf( ID->trace_out, " }" );

		if ( DD->next != NULL )
			fprintf( ID->trace_out, ", " );

		/* Next Data Value */

		DD = DD->next;

		trptr = trptr->next;
	}

	/* matching { */

	fprintf( ID->trace_out, " };;\n" );
}


trc_store_output_event( ID, tid, str, len )
TRC_ID ID;
int tid;
char *str;
int len;
{
	TRC_TEVTASK TT;

	char tmp[4096];

	int index;
	int num;
	int i;

	num = 0;

	TT = trc_get_tevtask_tid( ID, tid );

	if ( TT != NULL && TT->output != NULL )
	{
		strcpy( tmp, TT->output );

		free( TT->output );

		TT->output = (char *) NULL;

		if ( !strcmp( str, "GOTEOF\n" ) )
		{
			trc_write_output_event( ID, tid, tmp );

			index = 0;

			num++;
		}

		else
			index = strlen( tmp );
	}

	else
		index = 0;

	for ( i=0 ; i < len ; i++ )
	{
		if ( str[i] != '\n' )
			tmp[index++] = str[i];

		else
		{
			tmp[index] = '\0';

			trc_write_output_event( ID, tid, tmp );

			index = 0;

			num++;
		}
	}

	if ( index )
	{
		tmp[index] = '\0';

		if ( TT != NULL )
			TT->output = trc_copy_str( tmp );

		else
		{
			printf( "Warning: Partial Task Output Line\n" );

			trc_write_output_event( ID, tid, tmp );

			num++;
		}
	}

	return( num );
}


TRC_TEVDESC_INFO_STRUCT TRC_OUTPUT_INFO[] =
{
	{ TEV_DID_TS, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TU, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TID, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_OS, TEV_DATA_STRING, TEV_DATA_SCALAR }
};


void
trc_write_output_event( ID, tid, str )
TRC_ID ID;
int tid;
char *str;
{

#ifdef USE_PVM_33

	trc_write_old_output_event( ID, tid, str );

#else

	/* Dump Task Output Into Trace File */

	if ( TRC_OUTPUT_TEVDESC->dump )
		trc_dump_tevdesc( ID, TRC_OUTPUT_TEVDESC, tid );

	trc_store_event_header( ID, TRC_OUTPUT_TEVDESC, tid );

	fprintf( ID->trace_out, "%d, %ld, %d",
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, tid );

	trc_dump_trace_str( ID, str, TRC_TRUE );

	/* matching { */

	fprintf( ID->trace_out, " };;\n" );

#endif

	/* Dump Into Separate Output File If Requested */

	trc_task_output( ID, tid, str );
}


int
trc_save_host_status_events( ID )
TRC_ID ID;
{
	TRC_HOST H;

	int num;

	if ( ID->trace_out == NULL )
		return( 0 );

	H = TRC_HOST_LIST;

	num = 0;

	while ( H != NULL )
	{
		trc_write_host_add_event( ID, H );

		num++;

		if ( H->delta.tv_sec != 0 || H->delta.tv_usec != 0 )
		{
			trc_write_host_sync_event( ID, H );

			num++;
		}

		H = H->next;
	}

	if ( num )
		fflush( ID->trace_out );

	return( num );
}


TRC_TEVDESC_INFO_STRUCT TRC_HOST_ADD_INFO[] =
{
	{ TEV_DID_TS, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TU, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TID, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_HN, TEV_DATA_STRING, TEV_DATA_SCALAR },
	{ TEV_DID_HNA, TEV_DATA_STRING, TEV_DATA_SCALAR },
	{ TEV_DID_HA, TEV_DATA_STRING, TEV_DATA_SCALAR },
	{ TEV_DID_HSP, TEV_DATA_INT, TEV_DATA_SCALAR }
};


void
trc_write_host_add_event( ID, H )
TRC_ID ID;
TRC_HOST H;
{

#ifdef USE_PVM_33

	trc_write_old_host_add_event( ID, H );

#else

	if ( TRC_HOST_ADD_TEVDESC->dump )
		trc_dump_tevdesc( ID, TRC_HOST_ADD_TEVDESC, H->pvmd_tid );

	trc_store_event_header( ID, TRC_HOST_ADD_TEVDESC, H->pvmd_tid );

	fprintf( ID->trace_out, "%d, %ld, %d",
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, H->pvmd_tid );

	trc_dump_trace_str( ID, H->refname, TRC_TRUE );

	trc_dump_trace_str( ID, H->alias, TRC_TRUE );

	trc_dump_trace_str( ID, H->arch, TRC_TRUE );

	/* matching { */

	fprintf( ID->trace_out, ", %d };;\n", H->speed );

#endif

}


TRC_TEVDESC_INFO_STRUCT TRC_HOST_DEL_INFO[] =
{
	{ TEV_DID_TS, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TU, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TID, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_HN, TEV_DATA_STRING, TEV_DATA_SCALAR }
};


void
trc_write_host_del_event( ID, H, tid )
TRC_ID ID;
TRC_HOST H;
int tid;
{

#ifdef USE_PVM_33

	trc_write_old_host_del_event( ID, H, tid );

#else

	char tmp[1024];

	if ( TRC_HOST_DEL_TEVDESC->dump )
		trc_dump_tevdesc( ID, TRC_HOST_DEL_TEVDESC, tid );

	trc_store_event_header( ID, TRC_HOST_DEL_TEVDESC, tid );

	fprintf( ID->trace_out, "%d, %ld, %d",
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, tid );

	if ( strcmp( H->refname, "" ) )
	{
		sscanf( H->refname, "%s", tmp );

		trc_dump_trace_str( ID, tmp, TRC_TRUE );
	}

	else
		trc_dump_trace_str( ID, H->refname, TRC_TRUE );

	/* matching { */

	fprintf( ID->trace_out, " };;\n" );

#endif

}


TRC_TEVDESC_INFO_STRUCT TRC_HOST_SYNC_INFO[] =
{
	{ TEV_DID_TS, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TU, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_TID, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_HDS, TEV_DATA_INT, TEV_DATA_SCALAR },
	{ TEV_DID_HDU, TEV_DATA_INT, TEV_DATA_SCALAR },
};


void
trc_write_host_sync_event( ID, H )
TRC_ID ID;
TRC_HOST H;
{

#ifdef USE_PVM_33

	trc_write_old_host_sync_event( ID, H );

#else

	if ( TRC_HOST_SYNC_TEVDESC->dump )
		trc_dump_tevdesc( ID, TRC_HOST_SYNC_TEVDESC, H->pvmd_tid );

	trc_store_event_header( ID, TRC_HOST_SYNC_TEVDESC, H->pvmd_tid );

	/* matching { */

	fprintf( ID->trace_out, "%d, %ld, %d, %d, %ld };;\n",
		TRC_TRACE_TIME.tv_sec, TRC_TRACE_TIME.tv_usec, H->pvmd_tid,
		H->delta.tv_sec, H->delta.tv_usec );

#endif

	H->last_time.tv_sec = TRC_TRACE_TIME.tv_sec;
	H->last_time.tv_usec = TRC_TRACE_TIME.tv_usec;
}


void
trc_define_special_descriptors()
{
	unsigned info_size = sizeof( struct trc_tevdesc_info_struct );

	TRC_OUTPUT_TEVDESC = trc_make_tevdesc( (TRC_ID) NULL,
		TRC_OUTPUT, TRC_IGNORE_TEV, "output", TRC_OUTPUT_INFO,
		(int) ( sizeof(TRC_OUTPUT_INFO) / info_size ) );

	TRC_HOST_ADD_TEVDESC = trc_make_tevdesc( (TRC_ID) NULL,
		TRC_HOST_ADD, TRC_IGNORE_TEV, "host_add", TRC_HOST_ADD_INFO,
		(int) ( sizeof(TRC_HOST_ADD_INFO) / info_size ) );

	TRC_HOST_DEL_TEVDESC = trc_make_tevdesc( (TRC_ID) NULL,
		TRC_HOST_DEL, TRC_IGNORE_TEV, "host_del", TRC_HOST_DEL_INFO,
		(int) ( sizeof(TRC_HOST_DEL_INFO) / info_size ) );

	TRC_HOST_SYNC_TEVDESC = trc_make_tevdesc( (TRC_ID) NULL,
		TRC_HOST_SYNC, TRC_IGNORE_TEV, "host_sync", TRC_HOST_SYNC_INFO,
		(int) ( sizeof(TRC_HOST_SYNC_INFO) / info_size ) );
}


TRC_TEVDESC
trc_make_tevdesc( ID, eid, entry_exit, name, info, ninfo )
TRC_ID ID;
int eid;
int entry_exit;
char *name;
TRC_TEVDESC_INFO info;
int ninfo;
{
	TRC_TEVDESC_INFO TI;

	TRC_DATADESC DD;

	TRC_TEVDESC TD;

	int i;

	/* New Descriptor */

	TD = trc_create_tevdesc();

	TD->refcount = 1;

	TD->eid = eid;

	TD->entry_exit = entry_exit;

	TD->name = trc_copy_str( name );

	/* Semantic IDs / Data Types */

	for ( i=0 ; i < ninfo ; i++ )
	{
		TI = &(info[i]);

		if ( TD->ddesc == NULL )
			DD = TD->ddesc = trc_create_datadesc();
		
		else
			DD = DD->next = trc_create_datadesc();

		DD->did = trc_get_did( (TRC_HOST) NULL, TI->id );

		DD->dt = TI->dt;

		DD->array = TI->array;
	}

	trc_add_tevdesc( ID, &TD, TRC_TID, TRC_FALSE );

	return( TD );
}


/* Handle Task Output */

void
trc_task_output( ID, tid, str )
TRC_ID ID;
int tid;
char *str;
{
	char tmp[4096];

	if ( ID->output_fp == NULL )
		return;

	if ( !strcmp( str, "GOTEOF" ) )
	{
		if ( ID->output_dump_hdr )
		{
			(ID->output_dump_hdr)( ID, tid );

			fprintf( ID->output_fp, "EOF\n" );
		}

		else
			fprintf( ID->output_fp, "[0x%x] EOF\n", tid );

		fflush( ID->output_fp );
	}

	else if ( strcmp( str, "CREATION" ) && strcmp( str, "GOTSPAWN" ) )
	{
		if ( ID->output_dump_hdr )
		{
			(ID->output_dump_hdr)( ID, tid );

			fprintf( ID->output_fp, "%s\n", str );
		}

		else
			fprintf( ID->output_fp, "[0x%x] %s\n", tid, str );

		fflush( ID->output_fp );
	}
}


/* Time Routines */

void
trc_update_trace_time( sec, usec )
int sec;
int usec;
{
	struct timeval tm;

	if ( sec == -1 )
	{
		gettimeofday( &tm, (struct timezone *) NULL );

		sec = tm.tv_sec;
		usec = tm.tv_usec;
	}

	if ( sec > TRC_TRACE_TIME.tv_sec ||
		( sec == TRC_TRACE_TIME.tv_sec
			&& usec > TRC_TRACE_TIME.tv_usec ) )
	{
		TRC_TRACE_TIME.tv_sec = sec;
		TRC_TRACE_TIME.tv_usec = usec;
	}
}


/* Trace Utility Routines */

void
trc_dump_trace_header( ID )
TRC_ID ID;
{
	char *ds;

	if ( ID->trace_out == NULL )
		return;

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, TRC_TID );

	fprintf( ID->trace_out, "/*\n" );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, TRC_TID );

	ds = trc_date_str();

	fprintf( ID->trace_out, " * \"Creation Date\" \"%s\"\n", ds );

	free( ds );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, TRC_TID );

	fprintf( ID->trace_out, " * \"Machine\" \"%s %s (PVM %s)\"\n",
		TRC_NAME, TRC_VERSION, pvm_version() );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, TRC_TID );

	fprintf( ID->trace_out, " */ ;;\n" );

	if ( ID->event_dump_hdr )
		(ID->event_dump_hdr)( ID, TRC_TID );

	fprintf( ID->trace_out, "\n" );
}


void
trc_dump_trace_str( ID, str, comma_flag )
TRC_ID ID;
char *str;
int comma_flag;
{
	if ( comma_flag )
	{
		fprintf( ID->trace_out, ", [%u] { \"%s\" }",
			(unsigned) ( strlen( str ) + 1 ), str );
	}

	else
	{
		fprintf( ID->trace_out, "[%u] { \"%s\" }",
			(unsigned) ( strlen( str ) + 1 ), str );
	}
}


/* Trace Processing Routines */

int
trc_read_trace_event( ID, eof )
TRC_ID ID;
int *eof;
{
	TRC_TEVDESC TD;

	char tmp[4096];
	char c;

	int entry_exit;
	int dummy;
	int index;
	int code;
	int flag;
	int eid;
	int ee;
	int i;

	*eof = TRC_FALSE;

	if ( ID->trace_in == NULL )
		return( TRC_FALSE );

	while ( TRC_TRUE )
	{
		c = getc( ID->trace_in );

		if ( c != (char) EOF )
		{
			switch ( c )
			{
				/* Record Descriptor Packet */

				case '#':
				{
					/* Get event ID */

					flag = fscanf( ID->trace_in, "%d", &code );

					if ( flag != 1 )
					{
						printf( "Error Reading Event ID\n" );

						return( TRC_FALSE );
					}

					/* Get event name */

					if ( !trc_find_event_str( ID, "\"" ) )
						return( TRC_FALSE );

					i = 0;

					while ( (c = getc( ID->trace_in )) != (char) EOF
						&& c != '"' && c != '(' )
					{
						tmp[i++] = c;
					}

					TRC_CKEOF( c, "EOF Reading Event Descriptor Name\n",
						return( TRC_FALSE ) );

					tmp[i] = '\0';

					/* PVM 3.4 Trace */

					if ( c == '(' )
					{
						/* Decode event ID */

						index = ( code / 1000 ) - 1;

						eid = ( code - ((index + 1) * 1000) ) / 2;

						ee = code - ((index + 1) * 1000) - (2 * eid);

						/* Read in entry/exit or index value */

						flag = fscanf( ID->trace_in, "%d", &dummy );

						if ( flag != 1 )
						{
							printf( "Error Reading Event Index\n" );

							return( TRC_FALSE );
						}

						/* Read in next char for parsing context */

						c = getc( ID->trace_in );

						TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
							return( TRC_FALSE ) );

						if ( c == '.' )
						{
							if ( ee != dummy )
							{
								printf( "Warning:  " );
								printf( "Entry/Exit Mismatch  " );
								printf( "%d != %d\n", ee, dummy );
							}

							entry_exit = TRC_IGNORE_TEV;

							if ( dummy == 0 )
								entry_exit = TRC_ENTRY_TEV;

							else if ( dummy == 1 )
								entry_exit = TRC_EXIT_TEV;

							flag = fscanf( ID->trace_in, "%d", &dummy );

							if ( flag != 1 )
							{
								printf( "Error Reading Event Index\n" );

								return( TRC_FALSE );
							}

							if ( index != dummy )
							{
								printf( "Warning:  " );
								printf( "Descriptor Index Mismatch  " );
								printf( "%d != %d\n", index, dummy );

								index = dummy;
							}

							if ( !trc_find_event_str( ID, ")" ) )
								return( TRC_FALSE );
						}

						else
						{
							entry_exit = TRC_IGNORE_TEV;

							if ( ee == 1 )
							{
								printf( "Warning:  " );
								printf( "Entry/Exit Mismatch  " );
								printf( "%d != %d\n", ee, entry_exit );
							}

							if ( index != dummy )
							{
								printf( "Warning:  " );
								printf( "Descriptor Index Mismatch  " );
								printf( "%d != %d\n", index, dummy );

								index = dummy;
							}
						}

						if ( !trc_find_event_str( ID, "\"" ) )
							return( TRC_FALSE );

						if ( !(TD = trc_read_descriptor(
							ID, eid, tmp, entry_exit, index )) )
						{
							return( TRC_FALSE );
						}

						if ( ID->handle_descriptor )
							(ID->handle_descriptor)( ID, TD );
					}

					/* PVM 3.3 Trace - Ignore Descriptor */

					else
					{
						eid = code;

						if ( tmp[ i - 1 ] == '0' )
							entry_exit = TRC_ENTRY_TEV;

						else if ( tmp[ i - 1 ] == '1' )
							entry_exit = TRC_EXIT_TEV;

						else
							entry_exit = TRC_IGNORE_TEV;

						if ( !(TD = trc_read_descriptor(
							ID, eid, tmp, entry_exit, -1 )) )
						{
							return( TRC_FALSE );
						}

						if ( ID->handle_old_descriptor )
							(ID->handle_old_descriptor)( ID, TD );
					}

					break;
				}

				/* Record Data Packet */

				case '"':
				{
					i = 0;

					while ( (c = getc( ID->trace_in )) != (char) EOF
						&& c != '"' && c != '(' )
					{
						tmp[i++] = c;
					}

					TRC_CKEOF( c, "EOF Reading Event Record Name\n",
						return( TRC_FALSE ) );

					tmp[i] = '\0';

					/* PVM 3.4 Trace */

					if ( c == '(' )
					{
						/* Read in entry/exit or index value */

						flag = fscanf( ID->trace_in, "%d", &dummy );

						if ( flag != 1 )
						{
							printf( "Error Reading Event Index\n" );

							return( TRC_FALSE );
						}

						/* Read in next char for parsing context */

						c = getc( ID->trace_in );

						TRC_CKEOF( c, "EOF Reading Event Record\n",
							return( TRC_FALSE ) );

						if ( c == '.' )
						{
							entry_exit = TRC_IGNORE_TEV;

							if ( dummy == 0 )
								entry_exit = TRC_ENTRY_TEV;
							
							else if ( dummy == 1 )
								entry_exit = TRC_EXIT_TEV;
							
							flag = fscanf( ID->trace_in, "%d", &index );

							if ( flag != 1 )
							{
								printf( "Error Reading Event Index\n" );

								return( TRC_FALSE );
							}

							if ( !trc_find_event_str( ID, ")" ) )
								return( TRC_FALSE );
						}

						else
						{
							entry_exit = TRC_IGNORE_TEV;

							index = dummy;
						}

						if ( !trc_find_event_str( ID, "\"" ) )
							return( TRC_FALSE );

						return( trc_process_trace_event(
							ID, tmp, entry_exit, index ) );
					}

					/* PVM 3.3 Trace */

					else
					{
						return(
							trc_process_old_trace_event( ID, tmp ) );
					}

					/* break; unreachable */
				}

				/* Command Packet */

				case '%':
				{
					tmp[0] = '%';

					if ( !trc_find_event_end_ret( ID, tmp + 1, 4095 ) )
						return( TRC_FALSE );

					if ( ID->handle_command )
						(ID->handle_command)( ID, tmp );

					break;
				}

				/* Stream Attribute Packet */

				case '/':
				{
					c = getc( ID->trace_in );

					TRC_CKEOF( c, "EOF Reading Trace Comment\n",
						return( TRC_FALSE ) );

					if ( c != '*' )
					{
						printf( "\nError Reading Trace File\n" );
						printf( "\t- '*' expected (/* */)\n\n" );
					}

					strcpy( tmp, "/*" );

					if ( !trc_find_event_end_ret( ID, tmp + 2, 4094 ) )
						return( TRC_FALSE );

					if ( ID->handle_comment )
						(ID->handle_comment)( ID, tmp );

					break;
				}

				/* White Space */

				case ' ':
				case '\t':
				case '\n':
				{
					break;
				}
			}
		}

		else
		{
			*eof = TRC_TRUE;

			return( TRC_FALSE );
		}
	}
}


TRC_TEVDESC
trc_read_descriptor( ID, eid, name, entry_exit, index )
TRC_ID ID;
int eid;
char *name;
int entry_exit;
int index;
{
	TRC_DATADESC DD;

	TRC_TEVDESC TD;
	TRC_TEVDESC tdptr;

	char diddesc[1024];
	char dtstr[255];
	char did[8];

	char last;
	char c;

	long tmp;

	int done;
	int i;

	/* Find first bracket */

	if ( !trc_find_event_str( ID, "{" ) )
		return( (TRC_TEVDESC) NULL );

	/* matching } */

	/* Create Descriptor Structure */

	TD = trc_create_tevdesc();

	TD->refcount = 1;

	TD->name = trc_copy_str( name );

	TD->eid = eid;

	TD->entry_exit = entry_exit;

	TD->index = index;

	/* Read in Data Descriptor Statements */

	done = 0;

	do
	{
		/* matching { */

		while ( (c = getc( ID->trace_in )) != (char) EOF
			&& c != '/' && c != '}' );

		TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
			return( (TRC_TEVDESC) NULL ) );

		/* Another Descriptor Statement */

		if ( c == '/' )
		{
			if ( TD->ddesc == NULL )
				DD = TD->ddesc = trc_create_datadesc();

			else
				DD = DD->next = trc_create_datadesc();

			/* Get Second '/' */

			c = getc( ID->trace_in );

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			if ( c != '/' )
			{
				printf( "Error Parsing Event Descriptor\n" );

				return( (TRC_TEVDESC) NULL );
			}

			/* Read in TRC_DID Name */

			if ( !trc_find_event_str( ID, "\"" ) )
				return( (TRC_TEVDESC) NULL );

			i = 0;

			while ( (c = getc( ID->trace_in )) != (char) EOF
				&& c != '"' && i < 8 )
			{
				did[i++] = c;
			}

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			if ( c != '"' )
			{
				printf( "Error Parsing Event Descriptor\n" );

				return( (TRC_TEVDESC) NULL );
			}

			did[i] = '\0';

			/* Read in TRC_DID Description */

			if ( !trc_find_event_str( ID, "\"" ) )
				return( (TRC_TEVDESC) NULL );

			i = 0;

			while ( (c = getc( ID->trace_in )) != (char) EOF
				&& c != '"' && i < 1024 )
			{
				diddesc[i++] = c;
			}

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			if ( c != '"' )
			{
				printf( "Error Parsing Event Descriptor\n" );

				return( (TRC_TEVDESC) NULL );
			}

			diddesc[i] = '\0';

			/* Get Descriptor TRC_DID struct */

			DD->did = (TRC_DID) trc_lookup_trie( TRC_DID_TRIE,
				(void *) did );

			if ( DD->did == NULL )
			{
				DD->did = trc_create_did();

				DD->did->name = trc_copy_str( did );

				DD->did->desc = trc_copy_str( diddesc );
			}

			/* Get Data Type */

			while ( (c = getc( ID->trace_in )) != (char) EOF
				&& ( c == ' ' || c == '\t' || c == '\n' ) );

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			dtstr[0] = c;

			i = 1;

			while ( (c = getc( ID->trace_in )) != (char) EOF
				&& c != ' ' && c != '\t' && c != '\n' && c != '"' )
			{
				dtstr[i++] = c;
			}

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			dtstr[i] = '\0';

			DD->dt = trc_get_dt_str( dtstr );

			if ( DD->dt < 0 )
			{
				printf( "Error: Unknown Data Type \"%s\".\n", dtstr );

				return( (TRC_TEVDESC) NULL );
			}

			/* Skip over var name */

			if ( c != '"' )
			{
				if ( !trc_find_event_str( ID, "\"" ) )
					return( (TRC_TEVDESC) NULL );
			}

			if ( !trc_find_event_str( ID, "\"" ) )
				return( (TRC_TEVDESC) NULL );

			/* Check for Data Array */

			c = getc( ID->trace_in );

			TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
				return( (TRC_TEVDESC) NULL ) );

			if ( c == '[' )
			{
				if ( DD->dt == TEV_DATA_BYTE
					|| DD->dt == TEV_DATA_STRING )
				{
					/* Get closing ']' */

					c = getc( ID->trace_in );

					TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
						return( (TRC_TEVDESC) NULL ) );

					if ( c != ']' )
					{
						printf( "Error Parsing Event Descriptor\n" );

						return( (TRC_TEVDESC) NULL );
					}

					/* Check for another [] */

					c = getc( ID->trace_in );

					TRC_CKEOF( c, "EOF Reading Event Descriptor\n",
						return( (TRC_TEVDESC) NULL ) );

					/* [][] -> STRING[] */
					if ( c == '[' )
					{
						if ( DD->dt == TEV_DATA_BYTE )
							DD->dt = TEV_DATA_STRING;

						DD->array = TEV_DATA_ARRAY;
					}

					/* [] -> BYTE[] */
					else
					{
						if ( DD->dt == TEV_DATA_STRING )
							DD->dt = TEV_DATA_BYTE;

						DD->array = TEV_DATA_ARRAY;
					}
				}

				else
					DD->array = TEV_DATA_ARRAY;
			}

			else
				DD->array = TEV_DATA_SCALAR;
		}

		/* End of Descriptor */

		else
		{
			if ( !trc_find_event_end( ID ) )
				return( (TRC_TEVDESC) NULL );

			done++;
		}
	}
	while ( !done );

	tdptr = (TRC_TEVDESC) trc_lookup_trie( TRC_EVENT_TRIE, name );

	if ( tdptr == NULL )
		trc_add_to_trie( TRC_EVENT_TRIE, name, (void *) TD );

	else
	{
		while ( tdptr->next != NULL )
		{
			/* Use index, too - trace file events will...  D-Oh! */
			if ( trc_cmp_tevdesc( TD, tdptr )
				&& TD->index == tdptr->index )
			{
				trc_free_tevdesc( &TD );

				return( tdptr );
			}

			tdptr = tdptr->next;
		}

		/* Use index, too - trace file events will...  D-Oh! */
		if ( trc_cmp_tevdesc( TD, tdptr )
			&& TD->index == tdptr->index )
		{
			trc_free_tevdesc( &TD );

			return( tdptr );
		}

		else
			tdptr->next = TD;
	}

	/* Set Handle ID */

	tmp = (long) trc_lookup_trie( TRC_HANDLE_TRIE, TD->name );

	TD->hid = (int) tmp;

	return( TD );
}


int
trc_process_trace_event( ID, name, entry_exit, index )
TRC_ID ID;
char *name;
int entry_exit;
int index;
{
	TRC_DATADESC DD;

	TRC_TEVDESC TD;

	TRC_TEVREC TR;
	TRC_TEVREC trptr;

	char	read_str[TRC_MAX_UNPACK_ARR_SIZE];

	unsigned short	read_ushort;
	unsigned long	read_ulong;
	unsigned int	read_uint;

	double	read_double;
	float	read_float;
	short	read_short;
	long	read_long;
	int		read_int;

	char c;

	int i, j;
	int flag;
	int num;

	if ( !(ID->handle_event) )
	{
		trc_find_event_end( ID );

		return( TRC_TRUE );
	}

	TD = (TRC_TEVDESC) trc_lookup_trie( TRC_EVENT_TRIE, name );

	while ( TD != NULL &&
		( TD->entry_exit != entry_exit || TD->index != index ) )
	{
		TD = TD->next;
	}

	if ( TD == NULL )
	{
		printf( "Error: Event \"%s\" Descriptor Not Found\n", name );

		trc_find_event_end( ID );

		return( TRC_FALSE );
	}

	if ( !trc_find_event_str( ID, "{" ) )
		return( TRC_FALSE );
	
	/* matching } */

	DD = TD->ddesc;

	TR = (TRC_TEVREC) NULL;

	while ( DD != NULL )
	{
		if ( TR == NULL )
			trptr = TR = trc_create_tevrec();
		
		else
			trptr = trptr->next = trc_create_tevrec();

		trptr->ddesc = DD;

		if ( DD->array == TEV_DATA_ARRAY )
		{
			if ( !trc_find_event_str( ID, "[" ) )
				return( TRC_FALSE );

			flag = fscanf( ID->trace_in, "%d", &num );

			if ( flag != 1 )
			{
				printf( "Error Parsing Event Record\n" );

				trc_find_event_end( ID );

				return( TRC_FALSE );
			}

			if ( !trc_find_event_str( ID, "{" ) )
				return( TRC_FALSE );

			/* matching } */
		}

		else
			num = 1;

		trptr->value = trc_make_value( DD->dt, ( num > 0 ) ? num : 1 );

		if ( trptr->value == NULL )
		{
			printf( "Error Allocating Value dt=%d num=%d\n",
				DD->dt, num );

			return( TRC_FALSE );
		}

		trptr->num = num;

		switch ( DD->dt )
		{
			case TEV_DATA_NULL: break;

			case TEV_DATA_BYTE:
			{
				if ( DD->array == TEV_DATA_ARRAY )
				{
					if ( !trc_find_event_str( ID, "\"" ) )
						return( TRC_FALSE );

					i = 0;

					while ( i < num
						&& (c = getc( ID->trace_in )) != (char) EOF
						&& c != '"' )
					{
						TRC_ARR_VALUE_OF( trptr->value, char, i++ ) = c;
					}

					TRC_CKEOF( c, "EOF Parsing Event Record\n",
						return( TRC_FALSE ) );

					if ( i == num )
					{
						printf( "Error: Character Overflow\n" );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, char, i ) = '\0';
				}

				else
				{
					while ( (c = getc( ID->trace_in )) != (char) EOF
						&& ( c == ' ' || c == '\t' || c == '\n' ) );

					TRC_CKEOF( c, "EOF Parsing Event Record\n",
						return( TRC_FALSE ) );

					TRC_VALUE_OF( trptr->value, char ) = c;
				}

				break;
			}

			case TEV_DATA_CPLX:
			case TEV_DATA_FLOAT:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%f", &read_float );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, float, i ) =
						read_float;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_DCPLX:
			case TEV_DATA_DOUBLE:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%lf", &read_double );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, double, i ) =
						read_double;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_INT:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%d", &read_int );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, int, i ) = read_int;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_UINT:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%u", &read_uint );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, unsigned int, i ) =
						read_uint;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_LONG:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%ld", &read_long );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, long, i ) =
						read_long;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_ULONG:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%lu", &read_ulong );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, unsigned long, i ) =
						read_ulong;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_SHORT:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%hd", &read_short );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, short, i ) =
						read_short;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_USHORT:
			{
				for ( i=0 ; i < num ; i++ )
				{
					flag = fscanf( ID->trace_in, "%hu", &read_ushort );

					if ( flag != 1 )
					{
						printf( "Error Parsing Event Record\n" );

						trc_find_event_end( ID );

						return( TRC_FALSE );
					}

					TRC_ARR_VALUE_OF( trptr->value, unsigned short, i )
						= read_ushort;

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				break;
			}

			case TEV_DATA_STRING:
			{
				if ( DD->array == TEV_DATA_SCALAR )
				{
					if ( !trc_find_event_str( ID, "{" ) )
						return( TRC_FALSE );
				}

				for ( i=0 ; i < num ; i++ )
				{
					if ( !trc_find_event_str( ID, "\"" ) )
						return( TRC_FALSE );

					j = 0;

					while ( (c = getc( ID->trace_in )) != (char) EOF
						&& c != '"' )
					{
						read_str[j++] = c;
					}

					TRC_CKEOF( c, "EOF Parsing Event Record\n",
						return( TRC_FALSE ) );

					read_str[j] = '\0';

					TRC_ARR_VALUE_OF( trptr->value, char *, i ) =
						trc_copy_str( read_str );

					if ( i < num - 1 )
					{
						if ( !trc_find_event_str( ID, "," ) )
							return( TRC_FALSE );
					}
				}

				if ( DD->array == TEV_DATA_SCALAR )
				{
					if ( !trc_find_event_str( ID, "}" ) )
						return( TRC_FALSE );
				}

				break;
			}

			case TEV_DATA_STRUCT_START:
			case TEV_DATA_STRUCT_END:
			case TEV_DATA_DEFERRED:
			{
				printf( "DT Not Impl\n" );

				trc_find_event_end( ID );

				return( TRC_FALSE );
			}

			default:
			{
				printf( "DT Unknown\n" );

				trc_find_event_end( ID );

				return( TRC_FALSE );
			}
		}

		if ( DD->array == TEV_DATA_ARRAY )
		{
			/* matching { */

			if ( !trc_find_event_str( ID, "}" ) )
				return( TRC_FALSE );
		}

		if ( DD->next != NULL )
		{
			if ( !trc_find_event_str( ID, "," ) )
				return( TRC_FALSE );
		}

		DD = DD->next;
	}

	(ID->handle_event)( ID, TD, TR );

	trc_free_tevrec( &TR );

	return( TRC_TRUE );
}


int
trc_find_event_str_ret( ID, str, retstr, retlen )
TRC_ID ID;
char *str;
char *retstr;
int retlen;
{
	char tmp[1024];

	char c;

	int index;
	int i, j;
	int nope;
	int len;
	int r;

	len = strlen( str );

	for ( i=0 ; i < len ; i++ )
		tmp[i] = '\0';

	r = 0;

	i = 0;

	while ( (c = getc( ID->trace_in )) != (char) EOF )
	{
		if ( retstr && r < retlen - 1 )
			retstr[r++] = c;

		tmp[i] = c;

		nope = 0;

		for ( j=0 ; j < len && !nope ; j++ )
		{
			index = i - len + j + 1;

			index = index < 0 ? index + len : index;

			if ( tmp[index] != str[j] )
				nope++;
		}

		if ( !nope )
		{
			if ( retstr )
				retstr[r] = '\0';

			return( TRC_TRUE );
		}

		i = ( i + 1 ) % len;
	}

	printf( "EOF Reading Trace File - \"%s\" not found.\n", str );

	if ( retstr )
		retstr[r] = '\0';

	return( TRC_FALSE );
}


int
trc_find_event_str( ID, str )
TRC_ID ID;
char *str;
{
	return( trc_find_event_str_ret( ID, str, (char *) NULL, 0 ) );
}


int
trc_find_event_end_ret( ID, retstr, retlen )
TRC_ID ID;
char *retstr;
int retlen;
{
	return( trc_find_event_str_ret( ID, ";;", retstr, retlen ) );
}


int
trc_find_event_end( ID )
TRC_ID ID;
{
	return( trc_find_event_end_ret( ID, (char *) NULL, 0 ) );
}

