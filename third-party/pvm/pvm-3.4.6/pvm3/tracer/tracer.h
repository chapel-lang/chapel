
/* $Id: tracer.h,v 4.6 2002/04/02 20:53:50 pvmsrc Exp $ */

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


/* System Header Files */

#include <stdio.h>

#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <sys/types.h>

#ifndef WIN32
#include <sys/time.h>
#include <pwd.h>
#else
#include "pvmwin.h"
#include <time.h>
#endif

#ifdef HASSTDLIB
#include <stdlib.h>
#else
#ifndef IMA_MACOSX
#include <malloc.h>
#endif
#endif

#include <signal.h>
#include <ctype.h>


/* Header Files */

#include <pvm3.h>

#ifdef USE_PVM_33
#include "../src/tdpro.h"
#else
#include <pvmproto.h>
#endif

#include <pvmtev.h>

#include "trcdef.h"

typedef void (*vfp)();
typedef int (*ifp)();


/* Defined Constants & Macros */


/* This Tracer's Version */

#define MYVERSION "1.0.0"


/* Select Macro (based on FDSETISINT) */

#ifdef FDSETISINT

#define SELECT( _nfds, _rfdsp, _wfdsp, _efdsp, _tvalp ) \
	select( (_nfds), \
		(int *) (_rfdsp), (int *) (_wfdsp), (int *) (_efdsp), \
		(struct timeval *) (_tvalp) )

#else

#define SELECT( _nfds, _rfdsp, _wfdsp, _efdsp, _tvalp ) \
	select( (_nfds), \
		(fd_set *) (_rfdsp), (fd_set *) (_wfdsp), (fd_set *) (_efdsp), \
		(struct timeval *) (_tvalp) )

#endif


/* Routines */

FILE	*fopen();

void	re_register_tracer();


/* Externals */

extern	struct Pvmtevinfo	pvmtevinfo[];

extern	int 	errno;

