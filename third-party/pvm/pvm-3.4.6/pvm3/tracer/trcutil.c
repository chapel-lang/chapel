
static char rcsid[] = 
	"$Id: trcutil.c,v 4.16 2009/01/30 21:23:58 pvmsrc Exp $";

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


#include "trclib.h"

#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32
#include <winsock.h>
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#ifdef CTIMEISTIMET
#define PVM_TIMET time_t
#else
#define PVM_TIMET long
#endif

extern struct Pvmtevinfo pvmtevinfo[];


void
trc_tracer_init()
{
	char hname[1024];

	/* Host Name */

	TRC_HOST_ALIAS = (char *) NULL;

	TRC_HOST_NAME = (char *) NULL;

	if ( gethostname( hname, 1023 ) == -1 )
		perror( "Getting Hostname" );

	else
	{
		TRC_HOST_NAME = trc_copy_str( hname );

		TRC_HOST_ALIAS = trc_host_alias_str( TRC_HOST_NAME );
	}

	/* TRC_DID Semantic Identifier List */

	TRC_DID_LIST = (TRC_DID) NULL;

	/* Tracer ID List */

	TRC_DEAD_ID_LIST = (TRC_ID) NULL;

	TRC_ID_LIST = (TRC_ID) NULL;

	/* TEV Descriptor List */

	TRC_TEVDESC_LIST = (TRC_TEVDESC *) NULL;

	TRC_TEVDESC_SIZE = -1;

	/* Set Up Semantic Identifier Lookup Trie */

	trc_init_did_trie();

	/* Initialize Event Processing Tries */

	TRC_HANDLE_TRIE = trc_create_triestack();

	TRC_EVENT_TRIE = trc_create_triestack();

	/* Define Special Event Descriptors */

	trc_define_special_descriptors();

	/* Initialize Time Vars */

	gettimeofday( &TRC_TRACE_TIME, (struct timezone *) NULL );

	/* Initialize Other Trace Globals */

	TRC_VERSION = (char *) NULL;

	TRC_NAME = (char *) NULL;

	TRC_HOST_ADD_NOTIFY_CODE = -1;
	TRC_HOST_DEL_NOTIFY_CODE = -1;

	TRC_TID = -1;

	/* Initialize Old PVM 3.3 Trace Event Handling Stuff */

	trc_init_old_events();
}


TRC_ID
trc_get_tracer_id()
{
	TRC_ID ID;

	ID = trc_create_tracer_id();

	/* Initialize Flags */

	ID->group_tasks = TRC_FALSE;

	/* Add to List */

	ID->next = TRC_ID_LIST;

	TRC_ID_LIST = ID;

	return( ID );
}


void
trc_set_tracing_codes( ID )
TRC_ID ID;
{
#ifndef USE_PVM_33
	pvm_setopt( PvmTraceContext, ID->event_ctx );
#endif
	pvm_setopt( PvmTraceCode, ID->event_tag );
	pvm_setopt( PvmTraceTid, TRC_TID );

#ifndef USE_PVM_33
	pvm_setopt( PvmOutputContext, ID->output_ctx );
#endif	
	pvm_setopt( PvmOutputCode, ID->output_tag );
	pvm_setopt( PvmOutputTid, TRC_TID );
}


int
trc_set_trace_file( ID, trace_file )
TRC_ID ID;
char *trace_file;
{
	if ( trace_file != NULL )
	{
		if ( ID->trace_file != NULL )
			free( ID->trace_file );
		
		ID->trace_file = trc_copy_str( trace_file );

		return( TRC_TRUE );
	}

	else
	{
		trc_status_msg( ID, "Error: Null Trace File Name" );

		return( TRC_FALSE );
	}
}


int
trc_reset_trace_file( ID )
TRC_ID ID;
{
	TRC_TEVDESC TD;

	int fd;
	int i;

	/* Close Any Existing Trace File */

	if ( ID->trace_out != NULL && ID->trace_out != stdout )
		fclose( ID->trace_out );

	/* Clean Up Any Leftover Tasks */

	trc_free_tevtask_list( &(ID->tevtask_list) );

	/* Clean Up Any Dead Hosts */

	trc_cleanup_dead_hosts();

	/* Verify Trace File Set */

	if ( ID->trace_file == NULL )
	{
		trc_status_msg( ID, "Trace File Has Not Been Set." );

		return( TRC_FALSE );
	}

	/* Open Trace File */

	if ( strcmp( ID->trace_file, "" ) )
	{
		fd = open( ID->trace_file, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 );

		if ( !trc_fdcheck( fd, ID->trace_file ) )
			return( TRC_FALSE );

		ID->trace_out = fdopen( fd, "w" );

		if ( !trc_filecheck( ID->trace_out, ID->trace_file ) )
			return( TRC_FALSE );
	}

	else
		ID->trace_out = stdout;

	/* Dump Initial Trace File Header */

	trc_dump_trace_header( ID );

	/* Reset Event Name Lookup Trie */

	trc_free_event_descriptors( TRC_EVENT_TRIE );

	trc_free_triestack( &TRC_EVENT_TRIE );

	TRC_EVENT_TRIE = trc_create_triestack();

	/* Reset System Trace Descriptors */

	for ( i=0 ; i < TRC_TEVDESC_SIZE ; i++ )
	{
		TD = TRC_TEVDESC_LIST[i];

		while ( TD != NULL )
		{
			TD->dump = TRC_TRUE;

			TD = TD->next;
		}
	}

	/* Reset Time Vars */

	gettimeofday( &TRC_TRACE_TIME, (struct timezone *) NULL );

	/* Reset Old PVM 3.3 Trace Descriptor Flags */

	trc_reset_old_descriptors();

	/* Set Trace File Complete Flag */

	ID->complete = TRC_FALSE;

	/* Successful Reset */

	return( TRC_TRUE );
}


int
trc_set_output_file( ID, output_file )
TRC_ID ID;
char *output_file;
{
	if ( output_file != NULL )
	{
		if ( ID->output_file != NULL )
			free( ID->output_file );
		
		ID->output_file = trc_copy_str( output_file );

		return( TRC_TRUE );
	}

	else
	{
		trc_status_msg( ID, "Error: Null Output File Name" );

		return( TRC_FALSE );
	}
}


int
trc_open_output_file( ID )
TRC_ID ID;
{
	int fd;

	/* Close Any Existing Output File */

	if ( ID->output_fp != NULL && ID->output_fp != stdout )
		fclose( ID->output_fp );

	/* Verify Output File Set */

	if ( ID->output_file == NULL )
	{
		trc_status_msg( ID, "Output File Has Not Been Set." );

		return( TRC_FALSE );
	}

	/* Open Output File */

	if ( strcmp( ID->output_file, "" ) )
	{
		fd = open( ID->output_file, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 );

		if ( !trc_fdcheck( fd, ID->output_file ) )
			return( TRC_FALSE );

		ID->output_fp = fdopen( fd, "w" );

		if ( !trc_filecheck( ID->output_fp, ID->output_file ) )
			return( TRC_FALSE );
	}

	else
		ID->output_fp = stdout;

	/* Successful Opening */

	return( TRC_TRUE );
}


void
trc_print_tmask( tmask )
Pvmtmask tmask;
{
	static int indices[ TEV_MAX + 1 ];

	static int ncols = -1;
	static int nrows = -1;
	static int ntev = -1;

	static char fmt[64];

	int i, j, k;
	int maxlen;
	int width;
	int index;
	int flag;
	int len;
	int b;
	int c;
	int x;

	if ( ntev == -1 )
	{
		maxlen = -1;

		x = 0;

		for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
		{
			/* Skip Internal Events */

			if ( strcmp( pvmtevinfo[i].name, "newtask" )
				&& strcmp( pvmtevinfo[i].name, "spntask" )
				&& strcmp( pvmtevinfo[i].name, "endtask" )
				&& strcmp( pvmtevinfo[i].name, "timing" )
				&& strcmp( pvmtevinfo[i].name, "profiling" ) )
			{
				/* Determine Max Name Length */

				len = strlen( pvmtevinfo[i].name );

				if ( len > maxlen || maxlen == -1 )
					maxlen = len;

				/* Sort 'Em Ugly Like */

				flag = 0;

				for ( j=0 ; j < x && !flag ; j++ )
				{
					if ( strcmp( pvmtevinfo[i].name,
						pvmtevinfo[ indices[j] ].name ) < 0 )
					{
						for ( k=x+1 ; k > j ; k-- )
							indices[k] = indices[k - 1];

						indices[j] = i;

						x++;

						flag++;
					}
				}

				if ( !flag )
					indices[x++] = i;
			}
		}

		ntev = x;

		maxlen += 4;

		ncols = 79 / maxlen;

		width = (79 / ncols) - 4;

		sprintf( fmt, " %%c %%-%ds%%c", width );

		nrows = ( ntev + ncols - 1 ) / ncols;
	}

	for ( i=0 ; i < nrows ; i++ )
	{
		for ( c=0 ; c < ncols ; c++ )
		{
			index = i + (c * nrows);

			if ( index < ntev )
			{
				x = indices[ index ];

				b = TEV_MASK_CHECK( tmask, x );

				printf( fmt, (b ? '*' : ' '), pvmtevinfo[x].name,
					(c == ncols - 1 ? '\n' : ' ') );
			}

			else
				printf( "\n" );
		}
	}
}


TRC_TRIE
trc_create_triestack()
{
	TRC_TRIE tptr;
	TRC_TRIE tmp;

	int i;

	tmp = (TRC_TRIE) malloc( (unsigned) TRC_TRIE_SIZE
		* sizeof( struct trc_trie_struct ) );
	trc_memcheck( tmp, "Trie Structure" );

	for ( i=0 ; i < TRC_TRIE_SIZE ; i++ )
	{
		tptr = &(tmp[i]);

		tptr->valid = TRC_FALSE;

		tptr->str = (char *) NULL;

		tptr->value = (void *) NULL;

		tptr->next = (TRC_TRIE) NULL;
	}

	return( tmp );
}


void
trc_free_triestack( ptr )
TRC_TRIE *ptr;
{
	TRC_TRIE T;
	TRC_TRIE tptr;

	int i;

	if ( ptr == NULL || *ptr == NULL )
		return;

	T = *ptr;

	for ( i=0 ; i < TRC_TRIE_SIZE ; i++ )
	{
		tptr = &(T[i]);

		tptr->valid = TRC_FALSE;

		if ( tptr->str != NULL )
			free( tptr->str );

		tptr->str = (char *) NULL;

		tptr->value = (void *) NULL;

		if ( tptr->next != NULL )
			trc_free_triestack( &(tptr->next) );
	}

	free( T );

	*ptr = (TRC_TRIE) NULL;
}


int
trc_add_to_trie( T, str, value )
TRC_TRIE T;
char *str;
void *value;
{
	TRC_TRIE tptr;

	char *bump;

	int bumpindex;
	int marked;
	int index;
	int len;
	int i;

	if ( T == NULL )
	{
		printf( "\nError in trc_add_to_trie(): Null Trie\n\n" );

		return( TRC_FALSE );
	}

	if ( str == NULL || !strcmp( str, "" ) )
	{
		printf( "\nError in trc_add_to_trie(): Empty String\n\n" );

		return( TRC_FALSE );
	}

	len = strlen( str ) - 1;

	marked = 0;

	for ( i=0 ; i < len && !marked ; i++ )
	{
		index = trc_trie_index( str[i] );

		tptr = &(T[index]);

		if ( tptr->next != NULL )
			T = tptr->next;

		else
		{
			if ( !tptr->valid )
			{
				tptr->valid = TRC_TRUE;

				tptr->str = trc_copy_str( str );

				tptr->value = value;

				marked++;
			}

			else
			{
				tptr->next = trc_create_triestack();

				T = tptr->next;

				bump = tptr->str;

				if ( strlen( bump ) > i + 1 )
				{
					bumpindex = trc_trie_index( bump[ i + 1 ] );

					T[bumpindex].valid = TRC_TRUE;
					T[bumpindex].str = bump;
					T[bumpindex].value = tptr->value;

					tptr->valid = TRC_FALSE;
					tptr->str = (char *) NULL;
					tptr->value = (void *) NULL;
				}
			}
		}
	}

	if ( !marked )
	{
		index = trc_trie_index( str[len] );

		tptr = &(T[index]);

		if ( tptr->valid )
		{
			bump = tptr->str;

			if ( strlen( bump ) > len + 1 )
			{
				if ( tptr->next == NULL )
					tptr->next = trc_create_triestack();

				T = tptr->next;

				bumpindex = trc_trie_index( bump[ len + 1 ] );

				T[bumpindex].valid = TRC_TRUE;
				T[bumpindex].str = bump;
				T[bumpindex].value = tptr->value;
			}

			else
			{
				printf( "\nError: Duplicate %s (%lx)\n\n",
					bump, (long) tptr->value );

				return( TRC_FALSE );
			}
		}

		tptr->valid = TRC_TRUE;

		tptr->str = trc_copy_str( str );

		tptr->value = value;
	}

	return( TRC_TRUE );
}


void *
trc_lookup_trie( T, str )
TRC_TRIE T;
char *str;
{
	TRC_TRIE tptr;

	char *ptr;

	int len;
	int i;

	len = strlen( str ) - 1;

	ptr = str;

	for ( i=0 ; i < len ; i++ )
	{
		tptr = &(T[ trc_trie_index( *ptr++ ) ]);

		if ( tptr->next != NULL )
			T = tptr->next;

		else
		{
			if ( tptr->valid && !strcmp( tptr->str, str ) )
				return( tptr->value );

			else
				return( (void *) NULL );
		}
	}

	tptr = &(T[ trc_trie_index( *ptr ) ]);

	if ( tptr->valid && !strcmp( tptr->str, str ) )
		return( tptr->value );

	else
		return( (void *) NULL );
}


int
trc_trie_index( c )
char c;
{
	int index;

	if ( c >= 'a' && c <= 'z' )
		index = (int) ( c - 'a' );

	else if ( c >= 'A' && c <= 'Z' )
		index = (int) ( c - 'A' );
	
	else if ( c >= '0' && c <= '9' )
		index = 26 + (int) ( c - '0' );

	else
		index = TRC_TRIE_SIZE - 1;

	return( index );
}


TRC_DID
trc_create_did()
{
	TRC_DID tmp;

	tmp = (TRC_DID) malloc( sizeof( struct trc_did_struct ) );
	trc_memcheck( tmp, "Semantic Data ID Structure" );

	tmp->id = -1;

	tmp->name = (char *) NULL;

	tmp->desc = (char *) NULL;

	tmp->next = (TRC_DID) NULL;

	return( tmp );
}


void
trc_free_did( ptr )
TRC_DID *ptr;
{
	TRC_DID D;

	if ( ptr == NULL || *ptr == NULL )
		return;

	D = *ptr;

	D->id = -1;

	if ( D->name != NULL )
	{
		free( D->name );

		D->name = (char *) NULL;
	}

	if ( D->desc != NULL )
	{
		free( D->desc );

		D->desc = (char *) NULL;
	}

	D->next = (TRC_DID) NULL;

	free( D );

	*ptr = (TRC_DID) NULL;
}


int
trc_cmp_did( did1, did2 )
TRC_DID did1;
TRC_DID did2;
{
	if ( did1->id != did2->id )
		return( TRC_FALSE );

	if ( strcmp( did1->name, did2->name ) )
		return( TRC_FALSE );
	
	if ( strcmp( did1->desc, did2->desc ) )
		return( TRC_FALSE );
	
	return( TRC_TRUE );
}


TRC_DATADESC
trc_create_datadesc()
{
	TRC_DATADESC tmp;

	tmp = (TRC_DATADESC) malloc( sizeof( struct trc_datadesc_struct ) );
	trc_memcheck( tmp, "Data Description Structure" );

	tmp->did = (TRC_DID) NULL;

	tmp->dt = -1;

	tmp->array = -1;

	tmp->data = (TRC_VALUE) NULL;

	tmp->num = -1;

	tmp->next = (TRC_DATADESC) NULL;

	return( tmp );
}


void
trc_free_datadesc( ptr )
TRC_DATADESC *ptr;
{
	TRC_DATADESC DD;

	if ( ptr == NULL || *ptr == NULL )
		return;

	DD = *ptr;

	DD->did = (TRC_DID) NULL;

	DD->dt = -1;

	DD->array = -1;

	if ( DD->data != NULL )
	{
		free( DD->data );

		DD->data = (TRC_VALUE) NULL;
	}

	DD->num = -1;

	DD->next = (TRC_DATADESC) NULL;

	free( DD );

	*ptr = (TRC_DATADESC) NULL;
}


int
trc_cmp_datadesc( DD1, DD2 )
TRC_DATADESC DD1;
TRC_DATADESC DD2;
{
	TRC_DID D1, D2;

	D1 = DD1->did;
	D2 = DD2->did;

	if ( D1 != NULL && D2 != NULL )
	{
		if ( !trc_cmp_did( DD1->did, DD2->did ) )
			return( TRC_FALSE );
	}

	else
	{
		if ( D1 != NULL || D2 != NULL )
			return( TRC_FALSE );
	}

	if ( DD1->dt != DD2->dt || DD1->array != DD2->array )
		return( TRC_FALSE );

	return( TRC_TRUE );
}


int
trc_get_dt_str( dtstr )
char *dtstr;
{
	int i;

	for ( i=0 ; i < 16 ; i++ )
	{
		if ( !strcmp( TRC_TYPE_STRS[i], dtstr ) )
			return( i );
	}

	return( -1 );
}


TRC_TEVDESC
trc_create_tevdesc()
{
	TRC_TEVDESC tmp;

	tmp = (TRC_TEVDESC) malloc( sizeof( struct trc_tevdesc_struct ) );
	trc_memcheck( tmp, "Trace Event Descriptor Structure" );

	tmp->name = (char *) NULL;

	tmp->eid = -1;

	tmp->entry_exit = -1;

	tmp->index = -1;

	tmp->hid = -1;

	tmp->dump = -1;

	tmp->ddesc = (TRC_DATADESC) NULL;

	tmp->refcount = -1;

	tmp->next = (TRC_TEVDESC) NULL;

	return( tmp );
}


int
trc_free_tevdesc( ptr )
TRC_TEVDESC *ptr;
{
	TRC_TEVDESC TD;

	TRC_DATADESC DD;
	TRC_DATADESC DDnext;

	if ( ptr == NULL || *ptr == NULL )
		return( TRC_FALSE );

	TD = *ptr;

	(TD->refcount)--;

	if ( TD->refcount > 0 )
		return( TRC_FALSE );

	if ( TD->name != NULL )
		free( TD->name );

	TD->name = (char *) NULL;

	TD->eid = -1;

	TD->entry_exit = -1;

	TD->index = -1;

	TD->hid = -1;

	TD->dump = -1;

	DD = TD->ddesc;

	while ( DD != NULL )
	{
		DDnext = DD->next;

		trc_free_datadesc( &DD );

		DD = DDnext;
	}

	TD->ddesc = (TRC_DATADESC) NULL;

	TD->refcount = -1;

	TD->next = (TRC_TEVDESC) NULL;

	free( TD );

	*ptr = (TRC_TEVDESC) NULL;

	return( TRC_TRUE );
}


int
trc_cmp_tevdesc( TD1, TD2 )
TRC_TEVDESC TD1;
TRC_TEVDESC TD2;
{
	TRC_DATADESC DD1, DD2;

	if ( strcmp( TD1->name, TD2->name ) )
		return( TRC_FALSE );

	if ( TD1->eid != TD2->eid || TD1->entry_exit != TD2->entry_exit )
		return( TRC_FALSE );

	/* Ignore index & hid & dump */

	DD1 = TD1->ddesc;
	DD2 = TD2->ddesc;

	while ( DD1 != NULL && DD2 != NULL )
	{
		if ( !trc_cmp_datadesc( DD1, DD2 ) )
			return( TRC_FALSE );

		DD1 = DD1->next;
		DD2 = DD2->next;
	}

	if ( DD1 != NULL || DD2 != NULL )
		return( TRC_FALSE );

	return( TRC_TRUE );
}


void
trc_remove_tevdesc( ptr, index )
TRC_TEVDESC *ptr;
int index;
{
	TRC_TEVDESC TDnext;
	TRC_TEVDESC TDlast;
	TRC_TEVDESC TDptr;
	TRC_TEVDESC TD;

	int eid;

	if ( ptr == NULL || *ptr == NULL )
		return;

	/* Save Its Location */

	TD = *ptr;

	TDnext = TD->next;

	/* Try To Free It */

	if ( trc_free_tevdesc( ptr ) )
	{
		/* Fix Up Descriptor List */

		eid = TRC_TEVEID_OF( index );

		TDptr = TRC_TEVDESC_LIST[ eid ];

		TDlast = (TRC_TEVDESC) NULL;

		while ( TDptr != NULL )
		{
			if ( TDptr == TD )
			{
				if ( TDlast != NULL )
					TDlast->next = TDnext;

				else
					TRC_TEVDESC_LIST[ eid ] = TDnext;

				return;
			}

			else
			{
				TDlast = TDptr;

				TDptr = TDptr->next;
			}
		}
	}
}


void
trc_free_event_descriptors( T )
TRC_TRIE T;
{
	TRC_TRIE tptr;

	int i;

	if ( T == NULL )
		return;

	for ( i=0 ; i < TRC_TRIE_SIZE ; i++ )
	{
		tptr = &(T[i]);

		if ( tptr->value != NULL )
			trc_free_tevdesc( (TRC_TEVDESC *) &(tptr->value) );

		if ( tptr->next != NULL )
			trc_free_event_descriptors( tptr->next );
	}
}


TRC_TEVDESC_INFO
trc_create_tevdesc_info()
{
	TRC_TEVDESC_INFO tmp;

	tmp = (TRC_TEVDESC_INFO) malloc(
		sizeof( struct trc_tevdesc_info_struct ) );
	trc_memcheck( tmp, "Trace Event Descriptor Info Structure" );

	tmp->id = -1;

	tmp->dt = -1;

	tmp->array = -1;

	return( tmp );
}


void
trc_free_tevdesc_info( ptr )
TRC_TEVDESC_INFO *ptr;
{
	TRC_TEVDESC_INFO TI;

	if ( ptr == NULL || *ptr == NULL )
		return;

	TI = *ptr;

	TI->id = -1;

	TI->dt = -1;

	TI->array = -1;

	free( TI );

	*ptr = (TRC_TEVDESC_INFO) NULL;
}


TRC_TEVREC
trc_create_tevrec()
{
	TRC_TEVREC tmp;

	tmp = (TRC_TEVREC) malloc( sizeof( struct trc_tevrec_struct ) );
	trc_memcheck( tmp, "Trace Event Record Structure" );

	tmp->ddesc = (TRC_DATADESC) NULL;

	tmp->value = (TRC_VALUE) NULL;

	tmp->num = -1;

	tmp->next = (TRC_TEVREC) NULL;

	return( tmp );
}


void
trc_free_tevrec( ptr )
TRC_TEVREC *ptr;
{
	TRC_TEVREC TR;

	if ( ptr == NULL || *ptr == NULL )
		return;

	TR = *ptr;

	TR->ddesc = (TRC_DATADESC) NULL;

	if ( TR->value != NULL )
		free( TR->value );
	
	TR->value = (TRC_VALUE) NULL;

	TR->num = -1;

	if ( TR->next != NULL )
		trc_free_tevrec( &(TR->next) );

	free( *ptr );

	*ptr = (TRC_TEVREC) NULL;
}


TRC_TEVREC
trc_get_tevrec( TR, did )
TRC_TEVREC TR;
char *did;
{
	TRC_TEVREC trptr;

	trptr = TR;

	while ( trptr != NULL )
	{
		if ( !strcmp( did, trptr->ddesc->did->name ) )
			return( trptr );

		trptr = trptr->next;
	}

	return( trptr );
}


char *
trc_tevrec_string( TD, TR, tid, tsec, tusec )
TRC_TEVDESC TD;
TRC_TEVREC TR;
int tid;
int tsec;
int tusec;
{
	TRC_DATADESC DD;

	TRC_TEVREC trptr;

	char event_str[2048];
	char tmp[2048];

	char *tusec_pad;
	char *ptr;

	int first;
	int i;

	/* Dump Header Text */

	tusec_pad = trc_pad_num( tusec, 6 );

	if ( TD->entry_exit != TRC_IGNORE_TEV )
	{
		sprintf( event_str, "%s(%d.%d) @ %d.%s",
			TD->name, TD->entry_exit, TD->index, tsec, tusec_pad );
	}

	else
	{
		sprintf( event_str, "%s(%d) @ %d.%s",
			TD->name, TD->index, tsec, tusec_pad );
	}

	free( tusec_pad );

	/* Dump Event Data */

	first = TRC_TRUE;

	trptr = TR;

	while ( trptr != NULL )
	{
		DD = trptr->ddesc;

		/* Skip Over TS, TU, and TID */

		if ( strcmp( DD->did->name, "TS" )
			&& strcmp( DD->did->name, "TU" )
			&& strcmp( DD->did->name, "TID" ) )
		{
			/* Check for first data item -> put out : */

			if ( first )
			{
				trc_append_str( event_str, ": ", 2048 );

				first = TRC_FALSE;
			}

			/* Get Data Name */

			ptr = DD->did->name;

			i = 0;

			while ( *ptr != '\0' )
				tmp[i++] = *ptr++;

			tmp[i++] = '=';
			tmp[i++] = '\0';

			trc_append_str( event_str, tmp, 2048 );

			/* Get Data Itself */

			switch( DD->dt )
			{
				case TEV_DATA_NULL: break;

				case TEV_DATA_BYTE:
				{
					sprintf( tmp, "{%s}", (char *) trptr->value );

					trc_append_str( event_str, tmp, 2048 );

					break;
				}

				case TEV_DATA_CPLX:
				case TEV_DATA_FLOAT:
				{
					if ( trptr->num > 1 )
					{
						trc_append_str( event_str, "{ ", 2048 );
					}

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%f, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									float, i ) );
						}

						else
						{
							sprintf( tmp, "%f", 
								TRC_ARR_VALUE_OF( trptr->value,
									float, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_DCPLX:
				case TEV_DATA_DOUBLE:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%lf, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									double, i ) );
						}

						else
						{
							sprintf( tmp, "%lf", 
								TRC_ARR_VALUE_OF( trptr->value,
									double, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_INT:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%d, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									int, i ) );
						}

						else
						{
							sprintf( tmp, "%d", 
								TRC_ARR_VALUE_OF( trptr->value,
									int, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_UINT:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%u, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									unsigned, i ) );
						}

						else
						{
							sprintf( tmp, "%u", 
								TRC_ARR_VALUE_OF( trptr->value,
									unsigned, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_LONG:
				case TEV_DATA_ULONG:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%ld, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									long, i ) );
						}

						else
						{
							sprintf( tmp, "%ld", 
								TRC_ARR_VALUE_OF( trptr->value,
									long, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_SHORT:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%d, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									short, i ) );
						}

						else
						{
							sprintf( tmp, "%d", 
								TRC_ARR_VALUE_OF( trptr->value,
									short, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_USHORT:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "%d, ", 
								TRC_ARR_VALUE_OF( trptr->value,
									short, i ) );
						}

						else
						{
							sprintf( tmp, "%d", 
								TRC_ARR_VALUE_OF( trptr->value,
									short, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_STRING:
				{
					if ( trptr->num > 1 )
						trc_append_str( event_str, "{ ", 2048 );

					for ( i=0 ; i < trptr->num ; i++ )
					{
						if ( i < trptr->num - 1 )
						{
							sprintf( tmp, "{%s}, ",
								TRC_ARR_VALUE_OF( trptr->value,
									char *, i ) );
						}

						else
						{
							sprintf( tmp, "{%s}",
								TRC_ARR_VALUE_OF( trptr->value,
									char *, i ) );
						}

						trc_append_str( event_str, tmp, 2048 );
					}

					if ( trptr->num > 1 )
						trc_append_str( event_str, " }", 2048 );

					break;
				}

				case TEV_DATA_STRUCT_START:
				case TEV_DATA_STRUCT_END:
				case TEV_DATA_DEFERRED:
				{
					sprintf( tmp, "<DT Not Impl>" );

					trc_append_str( event_str, tmp, 2048 );
				}

				default:
				{
					sprintf( tmp, "<DT Unknown>" );

					trc_append_str( event_str, tmp, 2048 );
				}
			}

			if ( trptr->next != NULL )
			{
				sprintf( tmp, ", " );

				trc_append_str( event_str, tmp, 2048 );
			}
		}

		trptr = trptr->next;
	}

	trc_append_str( event_str, ".", 2048 );

	return( trc_copy_str( event_str ) );
}


TRC_VALUE
trc_make_value( dt, num )
int dt;
int num;
{
	TRC_VALUE tmp;

	int i;

	if ( num < 1 )
		return( (TRC_VALUE) NULL );

	tmp = (TRC_VALUE) NULL;

	switch ( dt )
	{
		case TEV_DATA_NULL: break;

		case TEV_DATA_BYTE:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( char ) );
			trc_memcheck( tmp, "Character Data Pointer" );

			break;
		}

		case TEV_DATA_STRING:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( char * ) );
			trc_memcheck( tmp, "String Data Pointer" );

			break;
		}

		case TEV_DATA_DCPLX:
		case TEV_DATA_DOUBLE:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( double ) );
			trc_memcheck( tmp, "Double Data Pointer" );

			break;
		}

		case TEV_DATA_CPLX:
		case TEV_DATA_FLOAT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( float ) );
			trc_memcheck( tmp, "Float Data Pointer" );

			break;
		}

		case TEV_DATA_INT:
		case TEV_DATA_UINT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( int ) );
			trc_memcheck( tmp, "Integer Data Pointer" );

			break;
		}

		case TEV_DATA_LONG:
		case TEV_DATA_ULONG:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( long ) );
			trc_memcheck( tmp, "Long Integer Data Pointer" );

			break;
		}

		case TEV_DATA_SHORT:
		case TEV_DATA_USHORT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( short ) );
			trc_memcheck( tmp, "Short Integer Data Pointer" );

			break;
		}

		case TEV_DATA_STRUCT_START:
		case TEV_DATA_STRUCT_END:
		case TEV_DATA_DEFERRED:
		{
			printf( "Value Data Type %d Not Implemented\n", dt );

			break;
		}

		default:
			printf( "Warning: Unknown Value Data Type %d\n", dt );
	}

	return( tmp );
}


TRC_VALUE
trc_copy_value( V, dt, num )
TRC_VALUE V;
int dt;
int num;
{
	TRC_VALUE tmp;

	int i;

	if ( V == NULL || num < 1 )
		return( (TRC_VALUE) NULL );

	tmp = (TRC_VALUE) NULL;

	switch ( dt )
	{
		case TEV_DATA_NULL: break;

		case TEV_DATA_BYTE:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( char ) );
			trc_memcheck( tmp, "Character Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, char, i ) =
					TRC_ARR_VALUE_OF( V, char, i );
			}

			break;
		}

		case TEV_DATA_STRING:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( char * ) );
			trc_memcheck( tmp, "String Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, char *, i ) =
					trc_copy_str( TRC_ARR_VALUE_OF( V, char *, i ) );
			}

			break;
		}

		case TEV_DATA_DCPLX:
		case TEV_DATA_DOUBLE:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( double ) );
			trc_memcheck( tmp, "Double Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, double, i ) =
					TRC_ARR_VALUE_OF( V, double, i );
			}

			break;
		}

		case TEV_DATA_CPLX:
		case TEV_DATA_FLOAT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( float ) );
			trc_memcheck( tmp, "Float Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, float, i ) =
					TRC_ARR_VALUE_OF( V, float, i );
			}

			break;
		}

		case TEV_DATA_INT:
		case TEV_DATA_UINT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( int ) );
			trc_memcheck( tmp, "Integer Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, int, i ) =
					TRC_ARR_VALUE_OF( V, int, i );
			}

			break;
		}

		case TEV_DATA_LONG:
		case TEV_DATA_ULONG:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num * sizeof( long ) );
			trc_memcheck( tmp, "Long Integer Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, long, i ) =
					TRC_ARR_VALUE_OF( V, long, i );
			}

			break;
		}

		case TEV_DATA_SHORT:
		case TEV_DATA_USHORT:
		{
			tmp = (TRC_VALUE) malloc( (unsigned) num
				* sizeof( short ) );
			trc_memcheck( tmp, "Short Integer Data Pointer" );

			for ( i=0 ; i < num ; i++ )
			{
				TRC_ARR_VALUE_OF( tmp, short, i ) =
					TRC_ARR_VALUE_OF( V, short, i );
			}

			break;
		}

		case TEV_DATA_STRUCT_START:
		case TEV_DATA_STRUCT_END:
		case TEV_DATA_DEFERRED:
		{
			printf( "Value Data Type %d Not Implemented\n", dt );

			break;
		}

		default:
			printf( "Warning: Unknown Value Data Type %d\n", dt );
	}

	return( tmp );
}


TRC_TEVTASK
trc_create_tevtask()
{
	TRC_TEVTASK tmp;

	tmp = (TRC_TEVTASK) malloc( sizeof( struct trc_tevtask_struct ) );
	trc_memcheck( tmp, "TEV Task Structure" );

	tmp->tid = -1;

	tmp->tevstatus = -1;
	tmp->outstatus = -1;

	tmp->tevlist = (void **) NULL;
	tmp->tevsize = -1;

	tmp->output = (char *) NULL;

	tmp->next = (TRC_TEVTASK) NULL;

	return( tmp );
}


void
trc_check_for_dead_host( ID, TT )
TRC_ID ID;
TRC_TEVTASK TT;
{
	TRC_TEVTASK TTptr;

	TRC_HOST Hlast;
	TRC_HOST H;

	int pvmd_tid;
	int alive;
	int tmp;

	/* Get Host Task ID, See if Host is "Dead" */

	pvmd_tid = pvm_tidtohost( TT->tid );

	H = trc_get_host_tid_last( pvmd_tid, &Hlast );

	if ( H->in_pvm != TRC_NOT_IN_PVM )
		return;

	/* Look for any living tasks on that host */

	TTptr = ID->tevtask_list;

	alive = 0;

	while ( TTptr != NULL && !alive )
	{
		tmp = pvm_tidtohost( TTptr->tid );

		if ( tmp == pvmd_tid
			&& ( TTptr->tevstatus == TRC_TASK_ALIVE
				|| TTptr->outstatus == TRC_TASK_OUT ) )
		{
			alive++;
		}

		TTptr = TTptr->next;
	}

	/* Host can finally rest in peace... */

	if ( !alive )
	{
		if ( Hlast != NULL )
			Hlast->next = H->next;
		
		else
			TRC_HOST_LIST = H->next;
		
		trc_free_host( &H );
	}
}


void
trc_free_tevtask( ptr )
TRC_TEVTASK *ptr;
{
	TRC_TEVTASK TT;

	int i;

	if ( ptr == NULL || *ptr == NULL )
		return;

	TT = *ptr;

	TT->tid = -1;

	TT->tevstatus = -1;
	TT->outstatus = -1;

	if ( TT->tevlist != NULL )
	{
		for ( i=0 ; i < TT->tevsize ; i++ )
			TT->tevlist[i] = (void *) NULL;

		free( TT->tevlist );

		TT->tevlist = (void **) NULL;
	}

	TT->tevsize = -1;

	if ( TT->output != NULL )
	{
		free( TT->output );

		TT->output = (char *) NULL;
	}

	TT->next = (TRC_TEVTASK) NULL;

	free( TT );

	*ptr = (TRC_TEVTASK) NULL;
}


void
trc_free_tevtask_list( tevtask_list )
TRC_TEVTASK *tevtask_list;
{
	TRC_TEVTASK TTnext;
	TRC_TEVTASK TT;

	int i;

	TT = *tevtask_list;

	*tevtask_list = (TRC_TEVTASK) NULL;

	while ( TT != NULL )
	{
		/* Save Next Ptr */

		TTnext = TT->next;

		/* Wipe Out Traced Tasks */

		if ( TT->tevstatus == TRC_TASK_DEAD
			&& TT->outstatus == TRC_TASK_EOF )
		{
			/* Wipe Out Associated Trace Event Descriptors */

			for ( i=0 ; i < TT->tevsize ; i++ )
			{
				if ( TT->tevlist[i] != NULL )
				{
					trc_remove_tevdesc(
						(TRC_TEVDESC *) &(TT->tevlist[i]), i );
				}
			}

			/* Free Traced Task */

			trc_free_tevtask( &TT );
		}

		/* Save Leftover Tasks Still In Progress */

		else
		{
			/* Set Event and Output Status Flags */

			if ( TT->tevstatus != TRC_TASK_DEAD )
				TT->tevstatus = TRC_TASK_WAITEND;
			
			if ( TT->outstatus != TRC_TASK_EOF )
				TT->outstatus = TRC_TASK_WAITEOF;

			/* Add Back On List */

			TT->next = *tevtask_list;

			*tevtask_list = TT;
		}

		/* Next Task */

		TT = TTnext;
	}
}


TRC_TEVTASK
trc_get_tevtask_tid( ID, tid )
TRC_ID ID;
int tid;
{
	TRC_TEVTASK TT;

	TT = ID->tevtask_list;

	while ( TT != NULL )
	{
		if ( TT->tid == tid )
			return( TT );

		TT = TT->next;
	}

	return( (TRC_TEVTASK) NULL );
}


int
trc_tevtasks_alive( ID )
TRC_ID ID;
{
	TRC_TEVTASK TT;

	TT = ID->tevtask_list;

	while ( TT != NULL )
	{
		if ( TT->tevstatus == TRC_TASK_ALIVE
			|| TT->outstatus == TRC_TASK_OUT )
		{
			return( TRC_TRUE );
		}

		TT = TT->next;
	}

	return( TRC_FALSE );
}


int
trc_tevtasks_dead( ID )
TRC_ID ID;
{
	TRC_TEVTASK TT;

	TT = ID->tevtask_list;

	while ( TT != NULL )
	{
		if ( TT->tevstatus != TRC_TASK_DEAD
			|| TT->outstatus != TRC_TASK_EOF )
		{
			return( TRC_FALSE );
		}

		TT = TT->next;
	}

	return( TRC_TRUE );
}


void
trc_end_trace( ID )
TRC_ID ID;
{
	if ( ID->trace_out != NULL )
		trc_status_msg( ID, "Trace File Complete" );

	trc_free_tevtask_list( &(ID->tevtask_list) );

	trc_cleanup_dead_hosts();

	ID->complete = TRC_TRUE;
}


TRC_HOST
trc_create_host()
{
	TRC_HOST tmp;

	int i;

	tmp = (TRC_HOST) malloc( sizeof( struct trc_host_struct ) );
	trc_memcheck( tmp, "Host Structure" );

	tmp->name = (char *) NULL;
	tmp->alias = (char *) NULL;
	tmp->refname = (char *) NULL;
	tmp->arch = (char *) NULL;

	tmp->delta.tv_usec = 0;
	tmp->delta.tv_sec = 0;

	tmp->last_time.tv_usec = 0;
	tmp->last_time.tv_sec = 0;

	tmp->pvmd_tid = -1;
	tmp->speed = -1;

	/* Tracing Info */

	tmp->tevlist = (void **) NULL;
	tmp->tevsize = -1;

	tmp->didlist = (void **) NULL;
	tmp->didsize = -1;

	tmp->ext = (void *) NULL;

	tmp->next = (TRC_HOST) NULL;

	return( tmp );
}


void
trc_free_host( ptr )
TRC_HOST *ptr;
{
	TRC_HOST H;

	int i;

	if ( ptr == NULL || *ptr == NULL )
		return;

	H = *ptr;

	if ( H->name != NULL )
	{
		free( H->name );

		H->name = (char *) NULL;
	}

	if ( H->alias != NULL )
	{
		free( H->alias );

		H->alias = (char *) NULL;
	}

	if ( H->refname != NULL )
	{
		free( H->refname );

		H->refname = (char *) NULL;
	}

	if ( H->arch != NULL )
	{
		free( H->arch );

		H->arch = (char *) NULL;
	}

	H->delta.tv_usec = 0;
	H->delta.tv_sec = 0;

	H->last_time.tv_usec = 0;
	H->last_time.tv_sec = 0;

	H->pvmd_tid = -1;
	H->speed = -1;

	/* Tracing Info */

	if ( H->tevlist != NULL )
	{
		for ( i=0 ; i < H->tevsize ; i++ )
			H->tevlist[i] = (void *) NULL;
		
		free( H->tevlist );

		H->tevlist = (void **) NULL;
	}

	H->tevsize = -1;

	if ( H->didlist != NULL )
	{
		for ( i=0 ; i < H->didsize ; i++ )
			H->didlist[i] = (void *) NULL;
		
		free( H->didlist );

		H->didlist = (void **) NULL;
	}

	H->didsize = -1;

	H->ext = (void *) NULL;

	H->next = (TRC_HOST) NULL;

	free( H );

	*ptr = (TRC_HOST) NULL;
}


void
trc_cleanup_dead_hosts()
{
	TRC_HOST Hlast;
	TRC_HOST Hnext;
	TRC_HOST H;

	H = TRC_HOST_LIST;

	Hlast = (TRC_HOST) NULL;

	while ( H != NULL )
	{
		Hnext = H->next;

		if ( H->in_pvm == TRC_NOT_IN_PVM )
		{
			if ( Hlast != NULL )
				Hlast->next = Hnext;
			
			else
				TRC_HOST_LIST = Hnext;
			
			trc_free_host( &H );
		}

		else
			Hlast = H;

		H = Hnext;
	}
}


TRC_HOST
trc_get_host_name( name )
char *name;
{
	TRC_HOST H;

	H = TRC_HOST_LIST;

	while ( H != NULL )
	{
		if ( TRC_HOST_COMPARE( H, name ) )
			return( H );

		H = H->next;
	}

	return( (TRC_HOST) NULL );
}


TRC_HOST
trc_get_host_tid( tid )
int tid;
{
	TRC_HOST H;

	H = TRC_HOST_LIST;

	while ( H != NULL )
	{
		if ( H->pvmd_tid == tid )
			return( H );

		H = H->next;
	}

	return( (TRC_HOST) NULL );
}


TRC_HOST
trc_get_host_tid_last( tid, last )
int tid;
TRC_HOST *last;
{
	TRC_HOST H;

	H = TRC_HOST_LIST;

	*last = (TRC_HOST) NULL;

	while ( H != NULL )
	{
		if ( H->pvmd_tid == tid )
			return( H );

		else
		{
			*last = H;

			H = H->next;
		}
	}

	return( (TRC_HOST) NULL );
}


int
trc_check_host_sync( ID )
TRC_ID ID;
{

#ifdef PIGS_FLY

	TRC_HOST H;

	int elapsed;
	int ecnt;

	H = TRC_HOST_LIST;

	ecnt = 0;

	while ( H != NULL )
	{
		if ( H->in_pvm == TRC_IN_PVM && !TRC_LOCALHOST( H->name ) )
		{
			elapsed = TRC_TIME_ELAPSED( H->last_time, TRC_TRACE_TIME );

			if ( elapsed > 10000000 )
				ecnt += trc_do_host_sync( ID, H, TRC_TRUE );
		}

		H = H->next;
	}

	return( ecnt );

#else

	return( 0 );

#endif

}


int
trc_do_host_sync( ID, H, dump_event )
TRC_ID ID;
TRC_HOST H;
int dump_event;
{
	static struct timeval hi = { 0, 0 };
	static struct timeval lo = { 0, 0 };

	struct timeval delta;
	struct timeval avg;

	int hiset, loset;
	int force;
	int valid;
	int ecnt;
	int i;

	if ( trc_compare( "0x", H->name ) )
		return( 0 );

	force = TRC_FALSE;

	valid = TRC_TRUE;

	hiset = loset = 0;

	ecnt = 0;

	avg.tv_sec = 0;
	avg.tv_usec = 0;

	for ( i=0 ; i < 10 && valid ; i++ )
	{
		TRC_PVMCKERR( pvm_hostsync( H->pvmd_tid,
			(struct timeval *) NULL, &delta ),
			"Host Sync", valid = TRC_FALSE );
		
		if ( valid )
		{
			if ( TRC_TIME_COMPARE( delta, hi ) > 0 || !hiset )
			{
				hi.tv_sec = delta.tv_sec;
				hi.tv_usec = delta.tv_usec;

				hiset++;
			}
			
			if ( TRC_TIME_COMPARE( delta, lo ) < 0 || !loset )
			{
				lo.tv_sec = delta.tv_sec;
				lo.tv_usec = delta.tv_usec;

				loset++;
			}

			avg.tv_sec += delta.tv_sec;
			avg.tv_usec += delta.tv_usec;
		}
	}

	if ( valid )
	{
		if ( H->delta.tv_sec != 0 || H->delta.tv_usec != 0 )
			force = TRC_TRUE;

		avg.tv_sec -= hi.tv_sec + lo.tv_sec;
		avg.tv_usec -= hi.tv_usec + lo.tv_usec;

		avg.tv_sec /= 8;
		avg.tv_usec /= 8;

		H->delta.tv_sec = avg.tv_sec;
		H->delta.tv_usec = avg.tv_usec;

		if ( H->delta.tv_usec < 0 )
		{
			H->delta.tv_usec += 1000000;

			(H->delta.tv_sec)--;
		}

		if ( dump_event &&
			( force || H->delta.tv_sec != 0 || H->delta.tv_usec != 0 ) )
		{
			trc_write_host_sync_event( ID, H );

			ecnt++;
		}

		else
		{
			H->last_time.tv_sec = TRC_TRACE_TIME.tv_sec;
			H->last_time.tv_usec = TRC_TRACE_TIME.tv_usec;
		}
	}

	return( ecnt );
}


TRC_ID
trc_create_tracer_id()
{
	TRC_ID tmp;

	tmp = (TRC_ID) malloc( sizeof( struct trc_id_struct ) );
	trc_memcheck( tmp, "Tracer ID Structure" );

	tmp->complete = -1;

	tmp->group_tasks = -1;

	tmp->event_ctx = -1;
	tmp->event_tag = -1;

	tmp->output_ctx = -1;
	tmp->output_tag = -1;

	tmp->tevtask_list = (TRC_TEVTASK) NULL;

	tmp->trace_file = (char *) NULL;

	tmp->trace_out = (FILE *) NULL;

	tmp->trace_in = (FILE *) NULL;

	tmp->output_file = (char *) NULL;

	tmp->output_fp = (FILE *) NULL;

	tmp->status_msg = (trc_vfp) NULL;
	tmp->event_dump_hdr = (trc_vfp) NULL;
	tmp->output_dump_hdr = (trc_vfp) NULL;
	tmp->handle_host_add_notify = (trc_vfp) NULL;
	tmp->handle_host_del_notify = (trc_vfp) NULL;
	tmp->handle_descriptor = (trc_vfp) NULL;
	tmp->handle_old_descriptor = (trc_vfp) NULL;
	tmp->handle_event = (trc_vfp) NULL;
	tmp->handle_old_event = (trc_vfp) NULL;
	tmp->handle_command = (trc_vfp) NULL;
	tmp->handle_comment = (trc_vfp) NULL;

	tmp->next = (TRC_ID) NULL;

	return( tmp );
}


void
trc_free_tracer_id( ptr )
TRC_ID *ptr;
{
	TRC_ID ID;

	if ( ptr == NULL || *ptr == NULL )
		return;

	ID = *ptr;

	if ( ID->tevtask_list != NULL )
	{
		trc_free_tevtask_list( &(ID->tevtask_list) );

		/* Still Some Unfinished Tasks - Save It */

		if ( ID->tevtask_list != NULL )
		{
			ID->next = TRC_DEAD_ID_LIST;

			TRC_DEAD_ID_LIST = ID;

			*ptr = (TRC_ID) NULL;

			return;
		}
	}

	ID->complete = -1;

	ID->group_tasks = -1;

	ID->event_ctx = -1;
	ID->event_tag = -1;

	ID->output_ctx = -1;
	ID->output_tag = -1;

	if ( ID->trace_file != NULL )
	{
		free( ID->trace_file );

		ID->trace_file = (char *) NULL;
	}

	if ( ID->trace_out != NULL )
	{
		if ( ID->trace_out != stdout )
			fclose( ID->trace_out );

		ID->trace_out = (FILE *) NULL;
	}

	if ( ID->trace_in != NULL )
	{
		if ( ID->trace_in != stdin )
			fclose( ID->trace_in );

		ID->trace_in = (FILE *) NULL;
	}

	if ( ID->output_file != NULL )
	{
		free( ID->output_file );

		ID->output_file = (char *) NULL;
	}

	if ( ID->output_fp != NULL )
	{
		if ( ID->output_fp != stdout )
			fclose( ID->output_fp );

		ID->output_fp = (FILE *) NULL;
	}

	ID->status_msg = (trc_vfp) NULL;
	ID->event_dump_hdr = (trc_vfp) NULL;
	ID->output_dump_hdr = (trc_vfp) NULL;
	ID->handle_host_add_notify = (trc_vfp) NULL;
	ID->handle_host_del_notify = (trc_vfp) NULL;
	ID->handle_descriptor = (trc_vfp) NULL;
	ID->handle_old_descriptor = (trc_vfp) NULL;
	ID->handle_event = (trc_vfp) NULL;
	ID->handle_old_event = (trc_vfp) NULL;
	ID->handle_command = (trc_vfp) NULL;
	ID->handle_comment = (trc_vfp) NULL;

	ID->next = (TRC_ID) NULL;

	free( ID );

	*ptr = (TRC_ID) NULL;
}


void
trc_initialize_hosts( ID )
TRC_ID ID;
{
	TRC_HOST H;

	struct pvmhostinfo *hostp;

	int narch;
	int nhost;
	int host;
	int i, j;
	int new;

	/* Set Up Host Add Notifies */

	pvm_notify( PvmHostAdd, TRC_HOST_ADD_NOTIFY_CODE, -1,
		(int *) NULL );

	/* Get Existing Configuration */

	TRC_PVMCKERR( pvm_config( &nhost, &narch, &hostp ),
		"Error Checking Existing Configuration", exit( -1 ) );

	/* Set up Host Delete Notify's */

	for ( i=0 ; i < nhost ; i++ )
		pvm_notify( PvmHostDelete,
			TRC_HOST_DEL_NOTIFY_CODE, 1, &(hostp[i].hi_tid) );

	/* Process Hosts */

	for ( i=0 ; i < nhost ; i++ )
	{
		new = trc_update_host( &H, hostp, i, -1 );

		/* Invoke User Custom Handler */

		if ( new && ID != NULL && ID->handle_host_add_notify )
			(ID->handle_host_add_notify)( H );
	}

	/* Fix TRC_LOCALHOST / TRC_HOST_NAME - Network Name in use... */

	host = pvm_tidtohost( TRC_TID );

	H = trc_get_host_tid( host );

	if ( H != NULL )
	{
		if ( TRC_HOST_NAME != NULL )
			free( TRC_HOST_NAME );

		TRC_HOST_NAME = trc_copy_str( H->name );

		if ( TRC_HOST_ALIAS != NULL )
			free( TRC_HOST_ALIAS );

		TRC_HOST_ALIAS = trc_copy_str( H->alias );

		/* Verify Host Delay Cleared */

		H->delta.tv_sec = H->delta.tv_usec = 0;
	}
}


int
trc_handle_host_add_notify( ID, tids, num )
TRC_ID ID;
int *tids;
int num;
{
	TRC_HOST H;

	struct pvmhostinfo *hostp;

	int found;
	int narch;
	int nhost;
	int ecnt;
	int i, j;
	int new;

	ecnt = 0;

	TRC_PVMCKERR( pvm_config( &nhost, &narch, &hostp ),
		"Error Checking Existing Configuration", exit( -1 ) );

	/* Look For New Hosts */

	for ( i=0 ; i < num ; i++ )
	{
		/* Find Hostinfo Struct */

		found = -1;

		for ( j=0 ; j < nhost && found < 0 ; j++ )
		{
			if ( hostp[j].hi_tid == tids[i] )
				found = j;
		}

		new = trc_update_host( &H, hostp, found, tids[i] );

		if ( new )
		{
			/* Invoke User Custom Handler */

			if ( ID->handle_host_add_notify )
				(ID->handle_host_add_notify)( H );

			/* Save Host Add Event to Trace File */

			if ( ID->trace_out != NULL )
			{
				trc_update_trace_time( -1, -1 );

				trc_write_host_add_event( ID, H );

				ecnt++;

				if ( H->delta.tv_sec != 0 || H->delta.tv_usec != 0 )
				{
					trc_write_host_sync_event( ID, H );

					ecnt++;
				}
			}
		}
	}

	return( ecnt );
}


int
trc_handle_host_del_notify( ID, tid )
TRC_ID ID;
int tid;
{
	TRC_HOST Hlast;
	TRC_HOST H;

	int ecnt;

	ecnt = 0;

	/* Lookup Dead Host */

	H = trc_get_host_tid_last( tid, &Hlast );

	if ( H != NULL )
	{
		/* Mark Host as Dead */

		H->in_pvm = TRC_NOT_IN_PVM;

		/* Invoke User Custom Handler */

		if ( ID->handle_host_del_notify )
			(ID->handle_host_del_notify)( H );

		/* Save Host Delete Event to Trace File */

		if ( ID->trace_out != NULL )
		{
			trc_update_trace_time( -1, -1 );

			trc_write_host_del_event( ID, H, tid );

			ecnt++;
		}
	}

	else
		printf( "Missing Host for Notify Delete, TID=0x%x\n", tid );

	return( ecnt );
}


int
trc_update_host( Hret, hostp, index, tid )
TRC_HOST *Hret;
struct pvmhostinfo *hostp;
int index;
int tid;
{
	TRC_HOST Hlast;
	TRC_HOST H;

	struct pvmhostinfo *HP;

	char tmp[1024];
	char tmp2[1024];

	int too_quick;
	int found;
	int i, j;
	int new;

	too_quick = 0;

	new = 0;

	if ( index >= 0 )
	{
		HP = &(hostp[index]);

		tid = HP->hi_tid;
	}
	
	else
		too_quick++;

	/* Check Host List */

	H = trc_get_host_tid_last( tid, &Hlast );

	/* Host Not Found, Add to Hosts List */

	if ( H == NULL )
	{
		if ( Hlast != NULL )
			H = Hlast->next = trc_create_host();
		
		else
			H = TRC_HOST_LIST = trc_create_host();

		if ( !too_quick )
		{
			H->name = trc_copy_str( HP->hi_name );

			H->alias = trc_host_alias_str( H->name );

			H->refname = trc_copy_str( HP->hi_name );

			H->arch = trc_copy_str( HP->hi_arch );

			H->pvmd_tid = HP->hi_tid;

			H->speed = HP->hi_speed;
		}

		else
		{
			sprintf( tmp, "0x%x", tid );

			H->name = trc_copy_str( tmp );

			H->alias = trc_copy_str( tmp );

			H->refname = trc_copy_str( "" );

			H->arch = trc_copy_str( "DUMMY" );

			H->pvmd_tid = tid;

			H->speed = 0;
		}

		new++;
	}

	H->in_pvm = TRC_IN_PVM;

	/* Set Host Delay */

	if ( !TRC_LOCALHOST( H->name ) )
		trc_do_host_sync( (TRC_ID) NULL, H, TRC_FALSE );

	*Hret = H;

	return( new );
}


void
trc_status_msg( ID, msg )
TRC_ID ID;
char *msg;
{
	if ( ID->status_msg )
		(ID->status_msg)( ID, msg );
	
	else
		fprintf( stderr, "%s\n", msg );
}


char *
trc_date_str()
{
	char tmp[255];

	char *result;

	PVM_TIMET t;

	time( &t );

	sprintf( tmp, "%s", ctime( &t ) );

	result = tmp;

	while ( *result != '\n' && *result != '\0' )
		result++;
	
	*result = '\0';

	result = trc_copy_str( tmp );

	return( result );
}


int
trc_append_str( result, str, maxlen )
char *result;
char *str;
int maxlen;
{
	int rlen;
	int len;
	int rc;
	int i;

	rlen = strlen( result );

	len = rlen + strlen( str ) + 1;

	rc = TRC_TRUE;

	if ( len <= maxlen )
		strcat( result, str );

	else
	{
		i = rlen;

		while ( i < maxlen - 4 )
		{
			result[i] = str[ i - rlen ];

			i++;
		}

		sprintf( (char *) (result + (maxlen - 4)), "..." );

		rc = TRC_FALSE;
	}

	return( rc );
}


char *
trc_host_alias_str( name )
char *name;
{
	char *ptr;
	char *tmp;

	char c;

	if ( name == NULL )
		return( (char *) NULL );

	ptr = name;

	while ( *ptr != '\0' && *ptr != '.' )
		ptr++;

	if ( *ptr == '.' )
	{
		c = *ptr;

		*ptr = '\0';

		tmp = trc_copy_str( name );

		*ptr = c;
	}

	else
		tmp = trc_copy_str( name );
	
	return( tmp );
}


char *
trc_pad_num( num, max )
long num;
long max;
{
	char tmp[1024];

	char *ptr;
	char *str;

	int nd;
	int nz;
	int i;

	sprintf( tmp, "%ld", num );

	nd = strlen( tmp );

	nz = max - nd;

	str = (char *) malloc( (unsigned) (max + 1) * sizeof(char) );
	trc_memcheck( str, "Numerical Pad String" );

	ptr = str;

	for ( i=0 ; i < nz ; i++ )
		*ptr++ = '0';

	sprintf( (char *) (str + nz), "%ld", num );

	return( str );
}


char *
trc_copy_str( str )
char *str;
{
	char *tmp;

	tmp = (char *) malloc( (unsigned) (strlen(str) + 1)
		* sizeof(char) );
	trc_memcheck(tmp,"Copy String");

	strcpy( tmp, str );

	return( tmp );
}


int
trc_fdcheck( fd, name )
int fd;
char *name;
{
	if ( fd < 0 )
	{
		fprintf( stderr, "\nError Opening File \"%s\"\n\n", name );

		return( TRC_FALSE );
	}

	return( TRC_TRUE );
}


int
trc_filecheck( fp, name )
FILE *fp;
char *name;
{
	if ( fp == NULL )
	{
		fprintf( stderr, "\nError Opening File \"%s\"\n\n", name );

		return( TRC_FALSE );
	}

	return( TRC_TRUE );
}


void
trc_memcheck( ptr, name )
void *ptr;
char *name;
{
	if ( ptr == NULL )
	{
		fprintf( stderr, "\nError Allocating Memory for \"%s\"\n\n",
			name );

		exit( -1 );
	}
}


int
trc_compare( x, y )
char *x;
char *y;
{
	while ( *x != '\0' )
	{
		if ( *x != *y )
			return( TRC_FALSE );

		x++; y++;
	}

	return( TRC_TRUE );
}

