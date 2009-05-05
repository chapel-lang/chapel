
/* $Id: bfunc.h,v 1.4 1999/07/08 19:00:19 kohl Exp $ */

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
 *	bfunc.h
 *
 *	Bcopy/Memcpy patch.
 *
 * $Log: bfunc.h,v $
 * Revision 1.4  1999/07/08 19:00:19  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/25  22:08:42  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.2  1997/01/28  19:27:53  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:11  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:12  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:46  manchek
 * Initial revision
 *
 */


#if defined(SYSVBFUNC)
#include <memory.h>
#define BZERO(d,n)      memset(d,0,n)
#define BCMP(s,d,n)     memcmp(d,s,n)
#define BCOPY(s,d,n)    memcpy(d,s,n)

#else
#define BZERO(d,n)      bzero(d,n)
#define BCMP(s,d,n)     bcmp(s,d,n)
#define BCOPY(s,d,n)    bcopy(s,d,n)

#endif

