
/* $Id: pvmalloc.h,v 1.4 1999/07/08 19:00:23 kohl Exp $ */

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
 *	pvmalloc.h
 *
 *	Malloc filter interface.
 *
 * $Log: pvmalloc.h,v $
 * Revision 1.4  1999/07/08 19:00:23  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/25  22:09:19  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.2  1997/01/28  19:28:18  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:25  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:21  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:50  manchek
 * Initial revision
 *
 */

#ifdef	HASSTDLIB
#include <stdlib.h>
#endif

#ifdef	USE_PVM_ALLOC
#define	PVM_ALLOC(n,g)		pvm_alloc((unsigned)(n),(g))
#define	PVM_REALLOC(p,n)	pvm_realloc((char*)(p),(unsigned)(n))
#define	PVM_FREE(p)			pvm_free((char*)(p))

#else
#define	PVM_ALLOC(n,g)		malloc((unsigned)(n))
#define	PVM_REALLOC(p,n)	realloc((char*)(p),(unsigned)(n))
#define	PVM_FREE(p)			free((char*)(p))
#endif

#define	TALLOC(n,t,g)		(t*)PVM_ALLOC((n)*sizeof(t),(g))
#define	TREALLOC(p,n,t)		(t*)PVM_REALLOC(p,(n)*sizeof(t))

#define	STRALLOC(s)			strcpy(TALLOC(strlen(s)+1,char,"str"),s)
/*
#define	STRALLOC(s)			strcpy((qq=TALLOC(strlen(s)+1,char,"str"),fprintf(stderr,"XXX STRALLOC %lx\n",qq),qq),s)
char *qq;
*/

char *pvm_alloc __ProtoGlarp__((unsigned n, char *g));
char *pvm_realloc __ProtoGlarp__((char *p, unsigned n));
void pvm_free __ProtoGlarp__((char *p));

