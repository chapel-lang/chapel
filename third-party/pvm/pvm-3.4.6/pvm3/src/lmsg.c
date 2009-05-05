
static char rcsid[] =
	"$Id: lmsg.c,v 1.3 1997/06/25 22:08:50 pvmsrc Exp $";

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
 *	lmsg.c
 *
 *	low level message functions.
 *
 */

#include <pvm3.h>
#include "pvmalloc.h"
#include "listmac.h"
#include "pvmdabuf.h"
#include "pvmmagic.h"
#include "lmsg.h"


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
#ifndef	MSGID_CLUMP
#define	MSGID_CLUMP 50
#endif
static struct msgid freemsgids;		/* free msgid header cache */
static int nummsgids = 0;
#endif


static struct msgid *
msgid_get_header()
{
	struct msgid *mp;
	int n;

#ifdef	CLUMP_ALLOC
	if (nummsgids == 0) {
		freemsgids.ms_link = freemsgids.ms_rlink = &freemsgids;
		if (!(mp = TALLOC(MSGID_CLUMP, struct msgid, "mids")))
			return (struct msgid *)0;
		for (n = MSGID_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freemsgids, mp, ms_link, ms_rlink);
			mp++;
		}
		nummsgids = MSGID_CLUMP;
	}
	nummsgids--;
	mp = freemsgids.ms_link;
	LISTDELETE(mp, ms_link, ms_rlink);

#else
	mp = TALLOC(1, struct msgid, "mid");
#endif

	mp->magic = LMSG_MSGID;  /* set the magic number */
	return mp;
}


static
msgid_put_header(mp)
	struct msgid *mp;
{

	if (BADMAGIC(LMSG_MSGID, mp->magic))
	{
		pvmlogerror("msgid_put_header(): bad magic number \n");
		pvmbailout(0);
		return PvmSysErr;
	}			
#ifdef	CLUMP_ALLOC
	if (nummsgids == 0)
		freemsgids.ms_link = freemsgids.ms_rlink = &freemsgids;
	LISTPUTBEFORE(&freemsgids, mp, ms_link, ms_rlink);
	nummsgids++;

#else
	PVM_FREE(mp);
#endif
	return 0;
}


/**********************
 **  Lmsg Functions  **
 **                  **
 **********************/

/*	msgid_new()
*
*	Create a new message id
*/

struct msgid *
msgid_new()
{
	struct msgid *mp;

	if (!(mp = msgid_get_header()))
		goto oops;

	mp->id = -1;
	mp->otid = -1;
	mp->ctxt = -1;
	mp->complete = 0;
	mp->ubuf = (char *) 0;

	return mp;


oops:
	pvmlogerror("msgid_new() can't get memory\n");
	pvmbailout(0);
	return (struct msgid *) 0;
}


/*	msgid_free()
*
*	free the msgid structure
*/

void
msgid_free(mp)
	struct msgid *mp;
{
		if (mp)
			msgid_put_header(mp);
}


