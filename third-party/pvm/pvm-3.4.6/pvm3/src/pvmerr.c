
static char rcsid[] =
	"$Id: pvmerr.c,v 1.10 2007/01/04 22:46:34 pvmsrc Exp $";

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
 *	pvmerr.c
 *
 *	Text of PVM error messages.
 *
 * $Log: pvmerr.c,v $
 * Revision 1.10  2007/01/04 22:46:34  pvmsrc
 * Added new pvm_strerror() function.
 * 	- submitted by Frank Sonntag <frank.sonntag@metservice.com>
 * (Spanker=kohl)
 *
 * Revision 1.9  2005/08/22 15:13:21  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.8  2000/02/10 23:53:35  pvmsrc
 * Added new PvmIPLoopback error code:
 * 	- Master Host IP Address tied to Loopback.
 * 	- check for this case in addhosts(), don't even TRY to add hosts...
 * (Spanker=kohl)
 *
 * Revision 1.7  1999/07/08 19:00:12  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1997/10/01  15:35:41  pvmsrc
 * Added missing "PvmParentNotSet" name to errnames[] array.
 * 	- for errnamecode() routine...
 * (Spanker=kohl)
 *
 * Revision 1.5  1997/06/25  22:09:36  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.4  1997/06/23  21:26:15  pvmsrc
 * Added new PvmHostrNMstr error code.
 * 	- Hoster must run on master host.
 *
 * Revision 1.3  1997/01/28  19:27:26  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  21:18:30  pvmsrc
 * Moved #include "global.h" to end of #include's for typing.
 * Added #include of new "lpvm.h" in place of explicitly extern-ed
 * 	globals...
 *
 * Revision 1.1  1996/09/23  23:44:36  pvmsrc
 * Initial revision
 *
 */

#include <stdio.h>
#include <pvm3.h>
#include "lpvm.h"
#include <pvmtev.h>
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/


/***************
 **  Private  **
 **           **
 ***************/

static char *errnames[] = {
	"PvmOk",
	"",
	"PvmBadParam",
	"PvmMismatch",
	"PvmOverflow",
	"PvmNoData",
	"PvmNoHost",
	"PvmNoFile",
	"PvmDenied",
	"",
	"PvmNoMem",
	"",
	"PvmBadMsg",
	"",
	"PvmSysErr",
	"PvmNoBuf",
	"PvmNoSuchBuf",
	"PvmNullGroup",
	"PvmDupGroup",
	"PvmNoGroup",
	"PvmNotInGroup",
	"PvmNoInst",
	"PvmHostFail",
	"PvmNoParent",
	"PvmNotImpl",
	"PvmDSysErr",
	"PvmBadVersion",
	"PvmOutOfRes",
	"PvmDupHost",
	"PvmCantStart",
	"PvmAlready",
	"PvmNoTask",
	"PvmNotFound",
	"PvmExists",
	"PvmHostrNMstr",
	"PvmParentNotSet",
	"PvmIPLoopback",
	0
};


int
pvm_perror(s)
	char *s;
{
	if (pvmmytid == -1)
		fprintf(stderr, "libpvm [pid%d]: ", pvmmyupid);
	else
		fprintf(stderr, "libpvm [t%x]: ", pvmmytid);
	fprintf(stderr, "%s: %s\n",
		(s ? s : "(null)"),
		(pvm_errno <= 0 && pvm_errno > -pvm_nerr
				? pvm_errlist[-pvm_errno] : "Unknown Error"));
	return 0;
}


int
errnamecode(s)
	char *s;
{
	int i;

	for (i = 0; errnames[i]; i++)
		if (!strcmp(s, errnames[i]))
			return -i;
	return 0;
}


char *
pvm_strerror()
{
	return( ( pvm_errno <= 0 && pvm_errno > -pvm_nerr )
			? pvm_errlist[-pvm_errno] : "Unknown Error" );
}

