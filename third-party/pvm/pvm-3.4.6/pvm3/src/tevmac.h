
/* $Id: tevmac.h,v 1.8 1999/07/08 19:00:25 kohl Exp $ */

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
 *	tevmac.h
 *
 *	Tracing defs.
 *
 * $Log: tevmac.h,v $
 * Revision 1.8  1999/07/08 19:00:25  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1998/03/04  18:54:26  pvmsrc
 * Oops...  better initialize pvmampushed to 0 in TEV_DECL.
 * 	- otherwise, TEV_AMPUSHED might reference it before set...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.6  1997/12/23  20:24:43  pvmsrc
 * Added new TEV_PUSHTOP, TEV_AMPUSHED and TEV_POPTOP macros.
 * 	- to fake top level tracing, i.e. "push" the current tracing
 * 		depth and turn on full tracing again (until next "pop").
 * 	- for things like message handler functions whose whole existence
 * 		is inside another PVM call.
 * Renamed "xamtraced" local trace var (TEV_DECLS).
 * 	-> "pvmamtraced", no chance of name clashes... (ha ha)
 * (Spanker=kohl)
 *
 * Revision 1.5  1997/06/25  22:11:13  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.4  1997/04/30  21:33:25  pvmsrc
 * SGI Compiler Warning Cleanup.
 * 	- stupid ##@%&@(...  made explicit boolean expressions out of
 * 		assignments inside TEV_EXCLUSIVE conditional.  :-Q
 *
 * Revision 1.3  1997/01/28  19:28:35  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  21:49:50  pvmsrc
 * Modified for New Tracing Facility:
 * 	- added new struct pvmtrcencvec for trace event packing.
 * 	- modified tracing macros:
 * 		* TEV_DO_TRACE() requires 2 args now, kind & entry_exit.
 * 		* TEV_FIN(), tev_fin() now requires no args.
 * 		* TEV_DECLS, no need for xtrcbuf, xtrcsbf now.
 * 	- added TEV_PACK_*() macros for trace event contents,
 * 		for performance, uses enc_*() routines directly.
 *
 * Revision 1.1  1996/09/23  23:43:37  pvmsrc
 * Initial revision
 *
 * Revision 1.1  1994/06/03  20:38:32  manchek
 * Initial revision
 *
 */


/*****************************
 **  Internal Trace Structs **
 **                         **
 *****************************/

struct pvmtrcencvec {
	int (*enc_byte)();
	int (*enc_cplx)();
	int (*enc_dcplx)();
	int (*enc_double)();
	int (*enc_float)();
	int (*enc_int)();
	int (*enc_uint)();
	int (*enc_long)();
	int (*enc_ulong)();
	int (*enc_short)();
	int (*enc_ushort)();
	int (*enc_str)();
};


void tev_init();
void tev_flush();


/*****************************
 **  Internal Trace Macros  **
 **                         **
 *****************************/

/* to check that task is connected without a function call */

#define	BEATASK	( pvmmytid == -1 ? pvmbeatask() : 0 )

/* check tracing on event kind */

#define TEV_CHECK_TRACE( _kind ) \
	( !BEATASK && pvmtrc.trctid > 0 && pvmtrc.trctid != pvmmytid \
		&& TEV_MASK_CHECK( pvmtrc.tmask, _kind ) )

/* check tracing and generate start message */

#define	TEV_DO_TRACE( _kind, _entry_exit ) \
	( TEV_CHECK_TRACE( _kind ) && tev_begin( _kind, _entry_exit ) )

#define	TEV_FIN	tev_fin()

#define	TEV_DECLS		int pvmamtraced; \
						int pvmampushed = 0; \
						int pvmtoplvlsave;

#define	TEV_EXCLUSIVE	( (pvmamtraced = pvmtoplvl) != 0 \
							&& (pvmtoplvl = 0) == 0 )
#define	TEV_AMEXCL		pvmamtraced
#define	TEV_ENDEXCL		( pvmtoplvl = pvmamtraced )

#define TEV_PUSHTOP		( pvmtoplvlsave = pvmtoplvl, pvmtoplvl = 1, \
							pvmampushed = 1 )
#define TEV_AMPUSHED	pvmampushed
#define TEV_POPTOP		( pvmtoplvl = pvmtoplvlsave, pvmampushed = 0 )

/* packing macros */

#define TEV_PACK_BYTE( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_byte)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_CPLX( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_cplx)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_DCPLX( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_dcplx)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_DOUBLE( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_double)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_FLOAT( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_float)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_INT( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_int)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_UINT( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_uint)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_LONG( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_long)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_ULONG( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_ulong)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_SHORT( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_short)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_USHORT( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_ushort)( _did, _array, \
		(void *)(_datap), _cnt, _std )

#define TEV_PACK_STRING( _did, _array, _datap, _cnt, _std ) \
	(pvmtrccodef->enc_str)( _did, _array, \
		(void *)(_datap), _cnt, _std )

