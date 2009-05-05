
static char rcsid[] =
	"$Id: tev.c,v 1.10 2001/02/07 23:15:59 pvmsrc Exp $";

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


/*
 *	tev.c
 *
 *	Tracing routines.
 *
 */


#include <stdio.h>

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include "pmsg.h"
#include "tvdefs.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/

struct pmsg *midtobuf();


/***************
 **  Private  **
 **           **
 ***************/


/* "High Performance" Direct Packing Guts Macros :-) */

#define PACK_BYTE( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_byte) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, 1 )

#define PACK_CPLX( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_cplx) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(float) * 2 )

#define PACK_DCPLX( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_dcplx) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(double) * 2 )

#define PACK_DOUBLE( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_double) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(double) )

#define PACK_FLOAT( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_float) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(float) )

#define PACK_INT( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_int) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(int) )

#define PACK_UINT( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_int) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(int) )

#define PACK_LONG( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_long) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(long) )

#define PACK_ULONG( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_long) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(long) )

#define PACK_SHORT( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_short) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(short) )

#define PACK_USHORT( _vp, _cnt, _std ) \
	(pvmtrcmp->m_codef->enc_short) \
		( pvmtrcmp, (void *) _vp, _cnt, _std, sizeof(short) )

#define PACK_STRING( _vp ) \
	( pvmtrctmp = strlen( _vp ) + 1, \
		(pvmtrcmp->m_codef->enc_int) \
			( pvmtrcmp, (void *) &pvmtrctmp, 1, 1, sizeof(int) ), \
		(pvmtrcmp->m_codef->enc_byte) \
			( pvmtrcmp, (void *) _vp, pvmtrctmp, 1, 1 ) )


/* Trace Event Data Packing Routines - Descriptor */

static int
tev_pack_byte_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_BYTE | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_BYTE( datap, cnt, std ) );
}

static int
tev_pack_cplx_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_CPLX | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_CPLX( datap, cnt, std ) );
}

static int
tev_pack_dcplx_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_DCPLX | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_DCPLX( datap, cnt, std ) );
}

static int
tev_pack_double_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_DOUBLE | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_DOUBLE( datap, cnt, std ) );
}

static int
tev_pack_float_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_FLOAT | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_FLOAT( datap, cnt, std ) );
}

static int
tev_pack_int_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_INT | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_INT( datap, cnt, std ) );
}

static int
tev_pack_uint_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_UINT | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_UINT( datap, cnt, std ) );
}

static int
tev_pack_long_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_LONG | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_LONG( datap, cnt, std ) );
}

static int
tev_pack_ulong_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_ULONG | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_ULONG( datap, cnt, std ) );
}

static int
tev_pack_short_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_SHORT | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_SHORT( datap, cnt, std ) );
}

static int
tev_pack_ushort_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_USHORT | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_USHORT( datap, cnt, std ) );
}

static int
tev_pack_string_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int num;
	int cc;
	int i;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_STRING | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( std <= 0 )
			return( PvmBadParam );

		num = ( cnt + ( std - 1 ) ) / std;
		if ( (cc = PACK_INT( &num, 1, 1 )) )
			return( cc );

		for ( i=0 ; i < cnt ; i += std )
			if ( (cc = PACK_STRING( ((char **) datap)[i] )) )
				return( cc );

		return( 0 );
	}

	else
		return( PACK_STRING( (char *) datap ) );
}


/* Trace Event Data Packing Routines - Raw */

static int
tev_pack_byte_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_BYTE( datap, cnt, std ) );
}

static int
tev_pack_cplx_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_CPLX( datap, cnt, std ) );
}

static int
tev_pack_dcplx_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_DCPLX( datap, cnt, std ) );
}

static int
tev_pack_double_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_DOUBLE( datap, cnt, std ) );
}

static int
tev_pack_float_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_FLOAT( datap, cnt, std ) );
}

static int
tev_pack_int_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_INT( datap, cnt, std ) );
}

static int
tev_pack_uint_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_UINT( datap, cnt, std ) );
}

static int
tev_pack_long_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_LONG( datap, cnt, std ) );
}

static int
tev_pack_ulong_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_ULONG( datap, cnt, std ) );
}

static int
tev_pack_short_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_SHORT( datap, cnt, std ) );
}

static int
tev_pack_ushort_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int cc;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_USHORT( datap, cnt, std ) );
}

static int
tev_pack_string_raw( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int num;
	int cc;
	int i;

	if ( array == TEV_DATA_ARRAY )
	{
		if ( std <= 0 )
			return( PvmBadParam );

		num = ( cnt + ( std - 1 ) ) / std;
		if ( (cc = PACK_INT( &num, 1, 1 )) )
			return( cc );

		for ( i=0 ; i < cnt ; i += std )
			if ( (cc = PACK_STRING( ((char **) datap)[i] )) )
				return( cc );

		return( 0 );
	}

	else
		return( PACK_STRING( (char *) datap ) );
}


/* ARGSUSED */
static int
tev_pack_nop( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	return( 0 );
}


/* Trace Descriptor Packing Vector */

static struct pvmtrcencvec pvmtrccodef_desc = {
	tev_pack_byte_desc,
	tev_pack_cplx_desc,
	tev_pack_dcplx_desc,
	tev_pack_double_desc,
	tev_pack_float_desc,
	tev_pack_int_desc,
	tev_pack_uint_desc,
	tev_pack_long_desc,
	tev_pack_ulong_desc,
	tev_pack_short_desc,
	tev_pack_ushort_desc,
	tev_pack_string_desc };


/* Trace Raw Packing Vector */

static struct pvmtrcencvec pvmtrccodef_raw = {
	tev_pack_byte_raw,
	tev_pack_cplx_raw,
	tev_pack_dcplx_raw,
	tev_pack_double_raw,
	tev_pack_float_raw,
	tev_pack_int_raw,
	tev_pack_uint_raw,
	tev_pack_long_raw,
	tev_pack_ulong_raw,
	tev_pack_short_raw,
	tev_pack_ushort_raw,
	tev_pack_string_raw };


/* Trace NOP Packing Vector */

static struct pvmtrcencvec pvmtrccodef_nop = {
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop,
	tev_pack_nop };


/*
 *	tev_init()
 *
 *	Initialize Trace Status Vector.
 */

void
tev_init()
{
	int i;

	if ( pvmtrc.trctid > 0 && pvmmytid != pvmtrc.trctid )
	{
		/* Reset Trace Descriptor Counts */

		for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
		{
			pvmtevinfo[i].desc_status = 0;

			pvmtevinfo[i].mark.tv_sec =
				pvmtevinfo[i].mark.tv_usec = 0;

			pvmtevinfo[i].total.tv_sec =
				pvmtevinfo[i].total.tv_usec = 0;

			pvmtevinfo[i].count = 0;
		}
	}
}


/*
 *	tev_begin()
 *
 *	Trace Event Begin - create message, pack header
 */

int
tev_begin( kind, entry_exit )
int kind;
int entry_exit;
{
	struct timeval timestamp;

	int tsec, tusec;
	int newbuffer;
	int tmp;

	/* Get Timestamp */

	if ( pvmtrc.trcopt != PvmTraceCount )
	{
		gettimeofday( &timestamp, (struct timezone *) 0 );

		tsec = (int) timestamp.tv_sec;
		tusec = (int) timestamp.tv_usec;
	}

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			/* Check for Descriptor Dump (one each, begin/end events) */

			if ( !( (pvmtevinfo[ kind - TEV_FIRST ].desc_status)
				& entry_exit ) )
			{
				pvmtevinfo[ kind - TEV_FIRST ].desc_status |=
					entry_exit;

				/* Set Trace Event Packing Vector */

				pvmtrccodef = &pvmtrccodef_desc;

				/* Set Descriptor Mode */

				pvmtrcdesc = 1;
			}

			/* Set Regular Trace Event Packing & Mode */

			else
			{
				pvmtrccodef = &pvmtrccodef_raw;
			
				pvmtrcdesc = 0;
			}

			/* Initialize Trace Event Message Buffer? */

			newbuffer = 0;

			if ( !pvmtrcsbf )
			{
				pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
				pvmtrcmp = midtobuf( pvmtrcsbf );

				newbuffer++;
			}

			/* Select Trace Event Buffer (save current) */

			pvmtrcsbfsave = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Buffer Marker */

			if ( newbuffer && pvmtrc.trcbuf )
			{
				tmp = TEV_MARK_EVENT_BUFFER;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Descriptor Header */

			if ( pvmtrcdesc )
			{
				/* Pack Event Marker */

				tmp = TEV_MARK_EVENT_DESC;
				pvm_pkint( &tmp, 1, 1 );

				/* Pack Descriptor Event ID & Event Name */

				tmp = kind | entry_exit;
				pvm_pkint( &tmp, 1, 1 );

				pvm_pkstr( pvmtevinfo[ kind - TEV_FIRST ].name );
			}

			/* Regular Header */

			else
			{
				/* Pack Event Marker */

				tmp = TEV_MARK_EVENT_RECORD;
				pvm_pkint( &tmp, 1, 1 );

				/* Pack Event ID */

				tmp = kind | entry_exit;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Pack Event Header */

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );
			
			break;
		}

		case PvmTraceTime:
			pvmtrccodef = &pvmtrccodef_nop;
			pvmtevinfo[ kind - TEV_FIRST ].mark = timestamp;
			pvmtrcsavekind = kind;
			break;

		case PvmTraceCount:
			pvmtrccodef = &pvmtrccodef_nop;
			pvmtrcsavekind = kind;
			break;

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_begin()...\n",
				pvmtrc.trcopt );
			pvmtrccodef = &pvmtrccodef_nop;
			break;
	}

	return( 1 );
}


/*
 *	tev_fin()
 *
 *	Trace Event End - send message (check trace buffering)
 */

int
tev_fin()
{
	struct timeval timestamp;

	int flush;
	int size;
	int tmp;

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			/* Pack End of Event Descriptor Marker */

			if ( pvmtrcdesc )
			{
				tmp = TEV_MARK_EVENT_DESC_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Pack End of Event Marker */

			else
			{
				tmp = TEV_MARK_EVENT_RECORD_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Reset Send Buffer (Done Packing Stuff) */

			pvm_setsbuf( pvmtrcsbfsave );
			pvmtrcsbfsave = 0;

			/* Check for Trace Message Send */
			/* (pvmtrc.trcbuf == 0 means no buffering) */

			flush = 0;

			if ( !pvmtrc.trcbuf )
				flush++;

			else
			{
				if ( pvm_bufinfo( pvmtrcsbf, &size,
						(int *) NULL, (int *) NULL ) ) {
					pvmlogerror(
						"tev_fin() error - get trace buffer size\n" );
					size = -1;
				}

				if ( size >= pvmtrc.trcbuf )
					flush++;
			}

			if ( flush )
				tev_flush( 1 );
	
			break;
		}

		case PvmTraceTime:
			gettimeofday( &timestamp, (struct timezone *) 0 );
			TVXSUBY( &(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].total),
				&timestamp,
				&(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].mark) );
			(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].count)++;
			break;

		case PvmTraceCount:
			(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].count)++;
			break;

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_fin()...\n",
				pvmtrc.trcopt );
			break;
	}

	return( 1 );
}


/*
 *	tev_flush( setflag )
 *
 *		setflag - indicates whether current send buffer is
 *			already the trace buffer (0), or whether it needs
 *			to be set (1).
 *
 *	Trace Event Flush - send message, flush any trace buffer
 */

void
tev_flush( setflag )
int setflag;
{
	char *event_names[ TEV_MAX - TEV_FIRST + 1 ];

	int timings_usec[ TEV_MAX - TEV_FIRST + 1 ];
	int timings_sec[ TEV_MAX - TEV_FIRST + 1 ];
	int counts[ TEV_MAX - TEV_FIRST + 1 ];

	struct timeval timestamp;

	int tsec, tusec;
	int routetmp;
	int savebuf;
	int tmpbuf;
	int num;
	int tmp;
	int i;

	if ( pvmtrc.trctid <= 0 || pvmmytid == pvmtrc.trctid )
		return;

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			if ( setflag )
			{
				/* Verify Trace Record Message Buffer */

				if ( !pvmtrcsbf )
					return;

				/* Set Send Buffer */

				savebuf = pvm_setsbuf( pvmtrcsbf );
			}

			/* Pack End of Event Buffer Marker */
			/* (pvmtrcbuf == 0 means no buffering) */

			if ( pvmtrc.trcbuf )
			{
				tmp = TEV_MARK_EVENT_BUFFER_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			break;
		}

		case PvmTraceTime:
		{
			/* Set Up Send Buffer */

			pvmtrccodef = &pvmtrccodef_desc;

			pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
			pvmtrcmp = midtobuf( pvmtrcsbf );

			savebuf = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Marker */

			tmp = TEV_MARK_EVENT_DESC;
			pvm_pkint( &tmp, 1, 1 );

			/* Pack Descriptor Event ID & Event Name */

			tmp = TEV_TIMING;
			pvm_pkint( &tmp, 1, 1 );

			pvm_pkstr( pvmtevinfo[ TEV_TIMING - TEV_FIRST ].name );

			/* Pack Event Header */

			gettimeofday( &timestamp, (struct timezone *) 0 );

			tsec = (int) timestamp.tv_sec;
			tusec = (int) timestamp.tv_usec;

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );

			/* Collect Timing Array */

			num = 0;

			for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
			{
				if ( pvmtevinfo[i].count > 0 )
				{
					event_names[num] = pvmtevinfo[i].name;

					timings_sec[num] = (int) pvmtevinfo[i].total.tv_sec;
					timings_usec[num] =
						(int) pvmtevinfo[i].total.tv_usec;

					counts[num] = pvmtevinfo[i].count;

					pvmtevinfo[i].total.tv_sec =
						pvmtevinfo[i].total.tv_usec = 0;

					pvmtevinfo[i].count = 0;

					num++;
				}
			}

			/* Pack Number of Events & Arrays */

			TEV_PACK_INT( TEV_DID_VCT, TEV_DATA_SCALAR,
				&num, 1, 1 );

			TEV_PACK_STRING( TEV_DID_VID, TEV_DATA_ARRAY,
				event_names, num, 1 );

			TEV_PACK_INT( TEV_DID_TGS, TEV_DATA_ARRAY,
				timings_sec, num, 1 );

			TEV_PACK_INT( TEV_DID_TGU, TEV_DATA_ARRAY,
				timings_usec, num, 1 );

			TEV_PACK_INT( TEV_DID_PRF, TEV_DATA_ARRAY,
				counts, num, 1 );

			/* Pack End of Descriptor Marker */

			tmp = TEV_MARK_EVENT_DESC_END;
			pvm_pkint( &tmp, 1, 1 );

			setflag = 1;

			break;
		}

		case PvmTraceCount:
		{
			/* Set Up Send Buffer */

			pvmtrccodef = &pvmtrccodef_desc;

			pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
			pvmtrcmp = midtobuf( pvmtrcsbf );

			savebuf = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Marker */

			tmp = TEV_MARK_EVENT_DESC;
			pvm_pkint( &tmp, 1, 1 );

			/* Pack Descriptor Event ID & Event Name */

			tmp = TEV_PROFILING;
			pvm_pkint( &tmp, 1, 1 );

			pvm_pkstr( pvmtevinfo[ TEV_PROFILING - TEV_FIRST ].name );

			/* Pack Event Header */

			gettimeofday( &timestamp, (struct timezone *) 0 );

			tsec = (int) timestamp.tv_sec;
			tusec = (int) timestamp.tv_usec;

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );

			/* Collect Profiling Array */

			num = 0;

			for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
			{
				if ( pvmtevinfo[i].count > 0 )
				{
					event_names[num] = pvmtevinfo[i].name;

					counts[num] = pvmtevinfo[i].count;

					pvmtevinfo[i].count = 0;

					num++;
				}
			}

			/* Pack Number of Events & Arrays */

			TEV_PACK_INT( TEV_DID_VCT, TEV_DATA_SCALAR,
				&num, 1, 1 );

			TEV_PACK_STRING( TEV_DID_VID, TEV_DATA_ARRAY,
				event_names, num, 1 );

			TEV_PACK_INT( TEV_DID_PRF, TEV_DATA_ARRAY,
				counts, num, 1 );

			/* Pack End of Descriptor Marker */

			tmp = TEV_MARK_EVENT_DESC_END;
			pvm_pkint( &tmp, 1, 1 );

			setflag = 1;

			break;
		}

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_flush()...\n",
				pvmtrc.trcopt );
			return;
	}

	/* Reset Send Buffer (Done Packing Stuff) */

	if ( setflag )
		pvm_setsbuf( savebuf );

	/* Clear Out Trace Buffer Global Before Entering mroute()... */
	/* (save in tmpbuf for sending, needed for mhf_invoke event) */

	tmpbuf = pvmtrcsbf;

	pvmtrcsbf = 0;

	/* Send Message */

	if ( (routetmp = pvmrouteopt) == PvmRouteDirect )
		pvmrouteopt = PvmAllowDirect;

	pvmtrcmp->m_ctx = pvmtrc.trcctx;

	mroute( tmpbuf, pvmtrc.trctid, pvmtrc.trctag, &pvmtrcztv );

	pvmrouteopt = routetmp;

	/* Clean-up */

	pvm_freebuf( tmpbuf );
}


/*
 *	tev_do_trace()
 *
 *	Export TEV_DO_TRACE() so user code doesn't have to import
 *	masks and trace tid, etc.
 */

int
tev_do_trace( kind, entry_exit )
	int kind;
	int entry_exit;
{
	TEV_DECLS

	return( TEV_DO_TRACE( kind, entry_exit ) );
}


