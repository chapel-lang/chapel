
/* $Id: trcglob.h,v 4.2 1997/06/25 21:44:33 pvmsrc Exp $ */

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


/* Global Variables */

extern	TRC_ID		ID;

extern	Pvmtmask	TRACE_MASK;

extern	Pvmtmask	CLEAR_MASK;

extern	char		*OUTPUT_FILE;

extern	char		*TRACE_FILE;

extern	int 		TRACER_REGISTERED;

extern	int			TRACE_BUF;

extern	int			TRACE_OPT;

extern	int 		MYTID;

extern	int 		vflag;

