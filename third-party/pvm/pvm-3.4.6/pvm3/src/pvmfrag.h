
/* $Id: pvmfrag.h,v 1.7 1999/07/08 19:00:24 kohl Exp $ */

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
 *	pvmfrag.h
 *
 *	Frag buffer util.
 *
 * $Log: pvmfrag.h,v $
 * Revision 1.7  1999/07/08 19:00:24  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1997/06/25  22:09:38  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.5  1997/04/30  21:26:28  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.4  1997/04/24  20:57:55  pvmsrc
 * added filed fr_rip to frag structure for frags received inplace
 *
 * Revision 1.3  1997/03/06  21:12:13  pvmsrc
 * 			added fr_seq and fr_src members to frag structure.
 * 			used by mppchunk.c for ordering. future for attached
 * 			sequence numbers on every fragment.
 *
 * Revision 1.2  1997/01/28  19:28:21  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:29  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1995/05/17  16:43:54  manchek
 * added support for CSPP shared memory
 *
 * Revision 1.2  1994/06/03  20:38:25  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
 * Initial revision
 *
 */

struct frag {
	struct frag *fr_link;	/* chain or 0 */
	struct frag *fr_rlink;
	char *fr_buf;			/* buffer or zero if master frag */
	char *fr_dat;			/* data */
	int fr_max;				/* size of buffer */
	int fr_len;				/* length of data */
	struct {
		unsigned int ref : 16;	/* refcount (of chain if master, */
								/* else frag) */
		unsigned int dab : 1;	/* buffer is a databuf */
		unsigned int spr : 1;	/* sparse data (csz, lnc valid) */
	} fr_u;
	int fr_csz;				/* chunk size */
	int fr_lnc;				/* lead to next chunk */
	int fr_seq;				/* sequence number. Used for mpp machines */
	int fr_src;				/* message src. Used for mpp machines */
	int fr_rip;				/* received inplace, don't put on recvq */
#ifdef IMA_CSPP
	int fr_num_unpacked;	/* so we know how much to flush */
#endif
};

struct frag *fr_new __ProtoGlarp__(( int n ));
struct frag *fr_snew __ProtoGlarp__(( char *cp, int n ));
void fr_unref __ProtoGlarp__(( struct frag *f ));

