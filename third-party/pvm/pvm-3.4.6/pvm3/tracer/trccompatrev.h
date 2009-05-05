
/* $Id: trccompatrev.h,v 4.1 1996/10/31 20:47:52 pvmsrc Exp $ */

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


/*
 *	trccompatrev.h
 *
 *	New PVM 3.4 Libpvm tracing includes for 3.3 compilation.
 *
 */


#define TEV_FIRST			0
#define TEV_MAX				0


#define TEV_HEADER			0


#define TEV_DID_FIRST		0
#define TEV_DID_HN			0
#define TEV_DID_HNA			0
#define TEV_DID_HA			0
#define TEV_DID_HSP			0
#define TEV_DID_HDS			0
#define TEV_DID_HDU			0
#define TEV_DID_OS			0
#define TEV_DID_TID			0
#define TEV_DID_TS			0
#define TEV_DID_TU			0
#define TEV_DID_MAX			0


#define TEV_EVENT_ENTRY		0x4000
#define TEV_EVENT_EXIT		0x8000


/* Trace Event Info Structures */

struct Pvmtevinfo
{
	char *name;
	int desc_status;
	struct timeval mark;
	struct timeval total;
	int count;
};

extern struct Pvmtevinfo pvmtevinfo[];


struct Pvmtevdid
{
	char *did;
	char *desc;
};

extern struct Pvmtevdid pvmtevdidlist[];


/* Trace Markers */

#define TEV_MARK_EVENT_BUFFER		-1
#define TEV_MARK_EVENT_BUFFER_END	-2
#define TEV_MARK_DATA_ID			-3
#define TEV_MARK_DATA_ID_END		-4
#define TEV_MARK_EVENT_DESC			-5
#define TEV_MARK_EVENT_DESC_END		-6
#define TEV_MARK_EVENT_RECORD		-7
#define TEV_MARK_EVENT_RECORD_END	-8
#define TEV_MARK_USER_EVENT_RECORD	-9
#define TEV_MARK_USER_EVENT_RECORD_END -10


/* Trace Event Data Types */

#define TEV_DATA_NULL			0
#define TEV_DATA_BYTE			1
#define TEV_DATA_CPLX			2
#define TEV_DATA_DCPLX			3
#define TEV_DATA_DOUBLE			4
#define TEV_DATA_FLOAT			5
#define TEV_DATA_INT			6
#define TEV_DATA_UINT			7
#define TEV_DATA_LONG			8
#define TEV_DATA_ULONG			9
#define TEV_DATA_SHORT			10
#define TEV_DATA_USHORT			11
#define TEV_DATA_STRING			12
#define TEV_DATA_STRUCT_START	13
#define TEV_DATA_STRUCT_END		14
#define TEV_DATA_DEFERRED		15

#define TEV_DATA_SCALAR		0x00
#define TEV_DATA_ARRAY		0x80


/* Trace Mask Macros & Constants */

#define TEV_MASK_LENGTH 32

typedef char Pvmtmask[TEV_MASK_LENGTH];

#define TEV_MASK_SET( m, k )	( m[ (k) / 4 ] |= ( 1 << ((k) & 3) ) )

#define TEV_MASK_UNSET( m, k )	( m[ (k) / 4 ] &= ~( 1 << ((k) & 3) ) )

#define TEV_MASK_CHECK( m, k )	( m[ (k) / 4 ] & (1 << ((k) & 3) ) )

#define TEV_MASK_INIT( m ) \
{ \
	int tev_init_mask_i = TEV_MASK_LENGTH - 1; \
	m[ tev_init_mask_i ] = 0; \
	while ( tev_init_mask_i-- > 0 ) \
		m[ tev_init_mask_i ] = '@'; \
}

