
/* $Id: tvdefs.h,v 1.4 1999/07/08 19:00:26 kohl Exp $ */

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
 *	tvdefs.h
 *
 *	Timeval manipulation stuff.
 *
 * $Log: tvdefs.h,v $
 * Revision 1.4  1999/07/08 19:00:26  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/25  22:11:14  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.2  1997/01/28  19:28:36  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:39  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:32  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */


#define	TVCLEAR(tvp)	((tvp)->tv_sec = (tvp)->tv_usec = 0)

#define	TVISSET(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)

#define	TVXLTY(xtv, ytv) \
	((xtv)->tv_sec < (ytv)->tv_sec || \
		((xtv)->tv_sec == (ytv)->tv_sec && (xtv)->tv_usec < (ytv)->tv_usec))

#define	TVXADDY(ztv, xtv, ytv)	\
	if (((ztv)->tv_usec = (xtv)->tv_usec + (ytv)->tv_usec) < 1000000) {	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec;	\
	} else {	\
		(ztv)->tv_usec -= 1000000;	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec + 1;	\
	}

#define	TVXSUBY(ztv, xtv, ytv)	\
	if ((xtv)->tv_usec >= (ytv)->tv_usec) {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec;	\
		(ztv)->tv_usec = (xtv)->tv_usec - (ytv)->tv_usec;	\
	} else {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec - 1;	\
		(ztv)->tv_usec = (xtv)->tv_usec + 1000000 - (ytv)->tv_usec;	\
	}

#define	TVDIVN(ytv, xtv, n)	{ \
	(ytv)->tv_usec = \
		(((xtv)->tv_usec + 1000000 * ((xtv)->tv_sec % (n))) / (n)); \
	(ytv)->tv_sec = ((xtv)->tv_sec / (n)); }

