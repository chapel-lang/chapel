
static char rcsid[] =
	"$Id: ftocstr.c,v 1.3 1997/11/04 21:49:53 pvmsrc Exp $";

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

#include "../src/bfunc.h"

#ifndef min
#define min(i,j) ((i)<(j)?(i):(j))
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

ftocstr(ds, dl, ss, sl)
    char *ds, *ss;      /* dst, src ptrs */
    int dl;             /* dst max len */
    int sl;             /* src len */
{
    char *p;

    for (p = ss + sl; --p >= ss && *p == ' '; ) ;
    sl = p - ss + 1;
    dl--;
    ds[0] = 0;
    if (sl > dl)
        return 1;
    strncat(ds, ss, min(sl, dl));
    return 0;
}


ctofstr(ds, dl, ss)
	char *ds;		/* dest space */
	int dl;			/* max dest length */
	char *ss;		/* src string (0-term) */
{
	int sl = strlen(ss);

	if (dl <= sl)
		BCOPY(ss, ds, dl);

	else {
		BCOPY(ss, ds, sl);
		dl -= sl;
		ds += sl;
		while (dl-- > 0)
			*ds++ = ' ';
	}
	return 0;
}


