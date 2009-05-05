
static char rcsid[] =
	"$Id: pkt.c,v 1.4 1999/07/08 19:00:05 kohl Exp $";

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
 *	pkt.c
 *
 *	Packet buffers.
 *
 * $Log: pkt.c,v $
 * Revision 1.4  1999/07/08 19:00:05  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/27  17:32:32  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.2  1997/01/28  19:26:56  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:44:23  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1995/07/24  18:27:24  manchek
 * zero pk_cpos on create
 *
 * Revision 1.2  1994/06/03  20:38:20  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:49  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#ifndef WIN32
#include <sys/time.h>
#else
#include "pvmwin.h"
#endif

#include <pvm3.h>
#include "pvmalloc.h"
#include "pkt.h"
#include "pvmdabuf.h"
#include "listmac.h"
#include "bfunc.h"


extern void pvmbailout();


/***************
 **  Private  **
 **           **
 ***************/



/*********************
 **  Pkt Functions  **
 **                 **
 *********************/


/*	pk_new()
*
*	Create a new pkt, not in a list.
*	If len is nonzero, len bytes are allocated as data space.
*	Else, the pkt has no data (is a master or will get data later).
*
*	The following fields are uninitialized:
*	pk_src, pk_dst, pk_flag, pk_tag, pk_ctx, pk_enc, pk_wid, pk_crc, pk_hostd,
*	pk_seq, pk_ack, pk_rtv, pk_rta, pk_rto, pk_at, pk_nrt
*/

struct pkt *
pk_new(len)
	int len;	/* (max) buffer size or 0 */
{
	struct pkt *pp;

	if (!(pp = TALLOC(1, struct pkt, "pkt")))
		goto oops;
/*
	BZERO((char*)pp, sizeof(struct pkt));
*/

	if (len) {		/* slave pkt */
		if (!(pp->pk_dat = pp->pk_buf = da_new(len))) {
			PVM_FREE(pp);
			goto oops;
		}
		pp->pk_max = len;
		pp->pk_len = 0;
		pp->pk_link = pp->pk_rlink = 0;

	} else {		/* master */
		pp->pk_dat = pp->pk_buf = 0;
		pp->pk_link = pp->pk_rlink = pp;
	}
	pp->pk_cpos = 0;
	pp->pk_tlink = pp->pk_trlink = 0;
	return pp;

oops:
	pvmlogerror("pk_new() can't get memory\n");
	pvmbailout(0);
	return (struct pkt*)0;
}


void
pk_free(pp)
	struct pkt *pp;
{
	struct pkt *pp2, *pp3;

	if (pp->pk_buf) {		/* slave pkt */
		if (pp->pk_tlink) {
			LISTDELETE(pp, pk_tlink, pk_trlink);
		}
		da_unref(pp->pk_buf);

	} else {				/* master pkt */

	/* free all pkts in chain */
		for (pp2 = pp->pk_link; pp2 != pp; pp2 = pp3) {
			pp3 = pp2->pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			pk_free(pp2);
		}
	}
	PVM_FREE(pp);
}

