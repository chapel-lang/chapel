
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
 *	hostc.h
 *
 *	Host cache definitions.
 *
 * $Log: hostc.h,v $
 * Revision 1.3  1999/07/08 18:59:49  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.2  1997/07/09  13:31:26  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.1  1996/09/23  22:30:00  pvmsrc
 * Initial revision
 *
 */


struct hostc {
	struct hostc *link, *rlink;
	int pvmd_tid;
	char *name;
	char *alias;
	char *arch;
	int speed;
	struct timeval delta;
};


int host_init __ProtoGlarp__(( int atag, int dtag, int (*acb)(), int (*dcb)() ));

int host_add __ProtoGlarp__(( void ));

int host_delete __ProtoGlarp__(( void ));

struct hostc *host_findtid __ProtoGlarp__(( int tid ));

struct hostc *host_findname __ProtoGlarp__(( char *name ));

struct hostc *host_next __ProtoGlarp__(( struct hostc *hp ));

int host_show __ProtoGlarp__(( void ));

