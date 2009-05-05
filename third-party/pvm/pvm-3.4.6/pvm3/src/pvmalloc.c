
static char rcsid[] =
	"$Id: pvmalloc.c,v 1.4 1999/07/08 19:00:06 kohl Exp $";

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
 *	pvmalloc.c
 *
 *	Malloc filter interface.
 *
 * $Log: pvmalloc.c,v $
 * Revision 1.4  1999/07/08 19:00:06  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/25  22:09:18  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.2  1997/01/28  19:26:59  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:44:28  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:21  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:50  manchek
 * Initial revision
 *
 */

#include <pvm3.h>
#include "pvmalloc.h"

char *i_malloc(), *i_realloc();

/***************
 **  Private  **
 **           **
 ***************/


/*****************
 **  Interface  **
 **             **
 *****************/

char *
pvm_alloc(n, tag)
	unsigned n;
	char *tag;
{
	char *r;

	if (!(r = i_malloc(n, tag))) {
		pvmlogprintf("pvm_alloc: can't get %d bytes", n);
	}
	return r;
}


char *
pvm_realloc(p, n)
	char *p;
	unsigned n;
{
	char *r;

	if (!(r = i_realloc(p, n))) {
		pvmlogprintf("pvm_realloc: can't get %d bytes", n);
	}
	return r;
}


void
pvm_free(p)
	char *p;
{
	(void)i_free(p);
}

