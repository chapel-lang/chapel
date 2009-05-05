
static char rcsid[] =
	"$Id: pvmfrag.c,v 1.5 1999/07/08 19:00:12 kohl Exp $";

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
 *	pvmfrag.c
 *
 *	Frag buffer util.
 *
 * $Log: pvmfrag.c,v $
 * Revision 1.5  1999/07/08 19:00:12  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.4  1997/06/25  22:09:37  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.3  1997/04/24  20:58:53  pvmsrc
 * intialize fr_rip to 0
 *
 * Revision 1.2  1997/01/28  19:27:27  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:44:37  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1995/05/17  16:42:49  manchek
 * added support for CSPP shared memory.
 * use CLUMP_ALLOC option to clump frag headers
 *
 * Revision 1.2  1994/06/03  20:38:24  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
 * Initial revision
 *
 */

#include <pvm3.h>
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "listmac.h"
#include "pvmdabuf.h"


extern void pvmbailout();


/***************
 **  Private  **
 **           **
 ***************/

#ifdef IMA_CSPP

/* NOTE:  If we were doing flushes on node, we'd have to set dcache_stride
          to 32 for single node systems and 64 for multinode systems.
          But since we only do this for cross node systems, we already know
          it needs to be 64
*/

int dcache_stride = 64;
#endif

#ifdef	CLUMP_ALLOC
#ifndef	FRAG_CLUMP
#define	FRAG_CLUMP	500
#endif
static struct frag freefrags;		/* free frag header cache */
static int numfrags = 0;
#endif


static struct frag *
frag_get_header()
{
	struct frag *fp;
	int n;

#ifdef	CLUMP_ALLOC
	if (numfrags == 0) {
		freefrags.fr_link = freefrags.fr_rlink = &freefrags;
		if (!(fp = TALLOC(FRAG_CLUMP, struct frag, "frgs")))
			return (struct frag *)0;
		for (n = FRAG_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freefrags, fp, fr_link, fr_rlink);
			fp++;
		}
		numfrags = FRAG_CLUMP;
	}
	numfrags--;
	fp = freefrags.fr_link;
	LISTDELETE(fp, fr_link, fr_rlink);

#else
	fp = TALLOC(1, struct frag, "frag");
#endif

	return fp;
}


static
frag_put_header(fp)
	struct frag *fp;
{
#ifdef	CLUMP_ALLOC
	if (numfrags == 0)
		freefrags.fr_link = freefrags.fr_rlink = &freefrags;
	LISTPUTBEFORE(&freefrags, fp, fr_link, fr_rlink);
	numfrags++;

#else
	PVM_FREE(fp);
#endif
	return 0;
}


/**********************
 **  Frag Functions  **
 **                  **
 **********************/

/*	fr_new()
*
*	Create a new frag with 1 reference, not in a list.
*	If len is nonzero, len bytes are allocated as data space.
*	Else, the frag has no data (is a master or will get data later).
*/

struct frag *
fr_new(len)
	int len;	/* (max) buffer size or 0 */
{
	struct frag *fp;

	if (!(fp = frag_get_header()))
		goto oops;

	if (len) {	/* slave frag */
		fp->fr_link = fp->fr_rlink = 0;
		if (!(fp->fr_dat = fp->fr_buf = da_new(len))) {
			frag_put_header(fp);
			goto oops;
		}
		fp->fr_max = len;

	} else {	/* master */
		fp->fr_link = fp->fr_rlink = fp;
		fp->fr_dat = fp->fr_buf = 0;
		fp->fr_max = 0;
	}
	fp->fr_len = 0;
	fp->fr_u.ref = 1;
	fp->fr_u.dab = 1;
	fp->fr_u.spr = 0;
	fp->fr_rip = 0;
#ifdef IMA_CSPP
	fp->fr_num_unpacked = 0;
#endif
/*
	pvmlogprintf("fr_new() %d = %lx\n", len, fp);
*/
	return fp;

oops:
	pvmlogerror("fr_new() can't get memory\n");
	pvmbailout(0);
	return (struct frag*)0;
}


/*	fr_snew()
*
*	Create a new frag with 1 reference, not in a list.
*	Its buffer is assumed to be a writable, dense, non-databuf.
*/

struct frag *
fr_snew(cp, len)
	char *cp;	/* buffer */
	int len;	/* buffer size */
{
	struct frag *fp;

	if (!(fp = frag_get_header()))
		goto oops;

	fp->fr_link = fp->fr_rlink = 0;
	fp->fr_dat = fp->fr_buf = cp;
	fp->fr_max = fp->fr_len = len;
	fp->fr_u.ref = 1;
	fp->fr_u.dab = 0;
	fp->fr_u.spr = 0;
	fp->fr_rip = 0;
	return fp;

oops:
	pvmlogerror("fr_snew() can't get memory\n");
	pvmbailout(0);
	return (struct frag*)0;
}


/*	fr_unref()
*
*	Reduce frag refcount by 1.  If result is < 1:
*		If the frag is a master, unref all its slave frags
*		Else, unref its data.
*		Then, free the frag.
*/

void
fr_unref(fp)
	struct frag *fp;		/* master frag */
{
/*
	pvmlogprintf("fr_unref() %lx ref %d\n", fp, fp->fr_u.ref);
*/
	if (fp->fr_u.ref-- == 1) {
		struct frag *fp2, *fp3;

		if (fp->fr_buf) {		/* slave frag */
			if (fp->fr_u.dab)
#ifdef IMA_CSPP
			if (fp->fr_num_unpacked) {
				unsigned int addr = (unsigned int)(fp->fr_dat);
				int nbytes = fp->fr_num_unpacked;

				nbytes += (addr & 0x3f);
				addr &= ~0x3f;
				dcache_flush_region(addr, nbytes);
			}
#endif
				da_unref(fp->fr_buf);

		} else {				/* master frag */

	/* unref all frags in chain */
			for (fp2 = fp->fr_link; fp2 != fp; fp2 = fp3) {
				fp3 = fp2->fr_link;
				LISTDELETE(fp2, fr_link, fr_rlink);
				fr_unref(fp2);
			}
		}
		frag_put_header(fp);
	}
}


