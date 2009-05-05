
/* $Id: pvmgs_mac.h,v 1.6 1997/07/09 13:51:33 pvmsrc Exp $ */

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

/* File: pvmgs_mac.h - various macros that are used throughout the   */
/*     group server                                                  */
/* Set the pvm_errno to info, print the error, and then return the   */
/*     error                                                         */
#define DO_ERROR_RTN( info, caller ) \
	{ pvm_errno = info; pvm_perror( caller ); return( info ); }

/* Allocate new memory for a string based if length > mxlen          */ 
#define REALLOCSTRING( length, mxlen, name, caller ) \
	{ if ( (length) > (mxlen) ) \
		{ \
			if ( name != (char *) NULL ) \
				PVM_FREE( name ); \
			name = (char *) PVM_ALLOC( sizeof(char) * (length + 1), \
				caller ); \
			mxlen = length; \
		} \
	}

/* Init a send buffer and pack the single integer result             */
/* caller is used for error reporting                                */
#define PK_IRESULT( result, caller) \
	{ if ( pvm_initsend( PvmDataDefault ) < 0 ) \
		pvm_perror( caller ); \
		if ( pvm_pkint( &(result), 1, 1 ) < 0 ) \
			pvm_perror( caller ); \
	}                              

/* Pack state. If state is STATIC then pack all group information.   */
#define PK_STATE( tid, state, name, hash_list, ngroups, caller ) \
	{ if ( pvm_pkint( &(state), 1, 1 ) < 0 ) \
		pvm_perror( caller ); \
		if ( state == STATIC ) \
			if ( gs_newstaticcache( tid, name, hash_list, ngroups, \
					caller ) == PvmgsNotCached ) \
				gs_pkstaticinfo( gs_group( name, hash_list, ngroups, \
						NOCREATE ) ); \
	}

/* Send packed buffer to tid, msgtag. Caller used for err reporting  */
#define SENDRESULT( tid, msgtag, caller ) \
	if ( pvm_send( tid, msgtag ) < 0 ) \
		pvm_perror( caller );                                 

/* Create a send buffer, pack a single integer, and send it          */
#define SENDINTRESULT( result, tid, msgtag, caller ) \
	{ PK_IRESULT( result, caller ); SENDRESULT( tid, msgtag, caller ) }

/* Begin a trace event, keep track of the current trace level        */
/* Optionally packs a single integer argument                        */
#define BGN_TRACE( event, strarg, did, intarg ) \
	if ( TEV_EXCLUSIVE ) { \
		if (TEV_DO_TRACE( event, TEV_EVENT_ENTRY ) ) \
		{ \
			TEV_PACK_STRING( TEV_DID_GN, TEV_DATA_SCALAR, \
				strarg ? strarg: "", 1, 1 ); \
			if ( intarg != (int *) NULL ) \
				TEV_PACK_INT( did, TEV_DATA_SCALAR, intarg, 1, 1 ); \
			TEV_FIN; \
		} \
	}

/* End trace event and send back an integer */
#define END_TRACE( event, did, intarg ) \
	if ( TEV_AMEXCL ) { \
		if (TEV_DO_TRACE( event, TEV_EVENT_EXIT ) ) \
		{ \
			TEV_PACK_INT( did, TEV_DATA_SCALAR, intarg, 1, 1 ); \
			TEV_FIN; \
		} \
		TEV_ENDEXCL; \
	}

/* Max, min. Interval confines the argument to the interval          */
#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))                               
#endif
#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))                               
#endif
#define INTERVAL(x,a,y)  MIN(MAX((x),(a)), (y)) 

