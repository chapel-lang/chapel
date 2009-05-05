
static char rcsid[] =
	"$Id: trcglob.c,v 4.2 1997/06/25 21:44:32 pvmsrc Exp $";

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


/* Tracer Header */

#include "tracer.h"


/* Globals Variables */

TRC_ID		ID;

Pvmtmask	TRACE_MASK;

Pvmtmask	CLEAR_MASK;

char		*OUTPUT_FILE;

char		*TRACE_FILE;

int			TRACER_REGISTERED;

int			TRACE_BUF;

int			TRACE_OPT;

int 		MYTID;

int 		vflag;

