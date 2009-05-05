
/* $Id: myalloc.h,v 1.5 1999/07/08 18:59:42 kohl Exp $ */

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
 *	myalloc.h
 *
 *	Malloc defines.
 *
 * $Log: myalloc.h,v $
 * Revision 1.5  1999/07/08 18:59:42  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.4  1997/07/09  13:21:10  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.3  1997/01/28  19:13:24  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/09/26  21:13:45  pvmsrc
 * Spanked the stupid CVS "Id" strings.
 * 	- comments for headers, not rcsid's stupid...  :-Q
 *
 * Revision 1.1  1996/09/23  20:25:43  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1994/11/07  20:53:43  manchek
 * include stdlib if available
 *
 * Revision 1.2  1994/06/03  20:01:53  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:30:32  manchek
 * Initial revision
 *
 */

#ifdef HASSTDLIB
#include <stdlib.h>
#endif

#define	MY_ALLOC(n,g)		malloc((unsigned)(n))
#define	MY_REALLOC(p,n)		realloc((char*)(p),(unsigned)(n))
#define	MY_FREE(p)			free((char*)(p))

#define	TALLOC(n,t,g)		(t*)MY_ALLOC((n)*sizeof(t),(g))
#define	TREALLOC(p,n,t)		(t*)MY_REALLOC(p,(n)*sizeof(t))

#define	STRALLOC(s)			strcpy(TALLOC(strlen(s)+1,char,"str"),s)

