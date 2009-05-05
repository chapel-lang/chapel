
static char rcsid[] =
	"$Id: nmdclass.c,v 1.4 1999/07/08 19:00:04 kohl Exp $";

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
 *	nmdclass.c
 *
 *	Name-server database.
 *
 * $Log: nmdclass.c,v $
 * Revision 1.4  1999/07/08 19:00:04  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/25  22:09:16  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.2  1997/01/28  19:26:55  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:44:22  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:19  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:49  manchek
 * Initial revision
 *
 */


#include <stdio.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <pvm3.h>
#include "pvmalloc.h"
#include "listmac.h"


/*
*	one of these for each name (class) in the database
*/

struct nmdcls {
	struct nmdcls *nc_link;		/* dll of peers */
	struct nmdcls *nc_rlink;
	char *nc_name;				/* class name or 0 if list master */
	struct nmdind *nc_ent;		/* entries in class */
};

/*
*	one of these for each index (entry) in the database
*/

struct nmdind {
	struct nmdind *ni_link;		/* dll of peers */
	struct nmdind *ni_rlink;
	int ni_ind;					/* index in class or -1 if master */
	int ni_data;				/* value */
};


/***************
 **  Private  **
 **           **
 ***************/

static char pvmtxt[1024];				/* scratch for error log */
static struct nmdcls *classes = 0;		/* the database */


struct nmdind *
ni_new(ind)
	int ind;
{
	struct nmdind *ep;

	if (ep = TALLOC(1, struct nmdind, "nmdi")) {
		ep->ni_link = ep->ni_rlink = ep;
		ep->ni_data = 0;
		ep->ni_ind = ind;
	}
	return ep;
}


struct nmdcls *
nc_new(name)
	char *name;
{
	struct nmdcls *np;

	if (np = TALLOC(1, struct nmdcls, "nmdc")) {
		if (name) {
			np->nc_name = STRALLOC(name);
			np->nc_ent = ni_new(-1);
			LISTPUTBEFORE(classes, np, nc_link, nc_rlink);

		} else {
			np->nc_link = np->nc_rlink = np;
			np->nc_name = 0;
			np->nc_ent = 0;
		}
	}
	return np;
}


struct nmdcls *
nc_find(name)
	char *name;
{
	struct nmdcls *np;

	for (np = classes->nc_link; np != classes; np = np->nc_link)
		if (!strcmp(np->nc_name, name))
			return np;
	return (struct nmdcls*)0;
}


int
nmd_init()
{
	classes = nc_new((char*)0);
	return 0;
}


int
nmd_insert(name, req, data)
	char *name;
	int req;
	int data;
{
	struct nmdcls *np;
	struct nmdind *ep, *ep2;

	if (!(np = nc_find(name)))
		np = nc_new(name);

	if (req >= 0) {
		for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
			if (ep->ni_ind >= req)
				break;
		if (ep->ni_ind == req)
			ep = 0;

	} else {
		req = 0;
		for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link) {
			if (ep->ni_ind != req)
				break;
			req = ep->ni_ind + 1;
		}
	}

	if (ep) {
		ep2 = ni_new(req);
		ep2->ni_data = data;
		LISTPUTBEFORE(ep, ep2, ni_link, ni_rlink);

	} else
		req = -1;

	return req;
}


int
nmd_delete(name, req)
	char *name;
	int req;
{
	struct nmdcls *np;
	struct nmdind *ep = 0;

	if (np = nc_find(name)) {
		if (req >= 0) {
			for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
				if (ep->ni_ind == req)
					break;
			if (ep == np->nc_ent)
				ep = 0;
		}
	}

	if (ep) {
		req = ep->ni_ind;
		LISTDELETE(ep, ni_link, ni_rlink);
		PVM_FREE(ep);
		if (np->nc_ent->ni_link == np->nc_ent) {
			LISTDELETE(np, nc_link, nc_rlink);
			PVM_FREE(np->nc_name);
			PVM_FREE(np->nc_ent);
			PVM_FREE(np);
		}
	} else
		req = -1;

	return req;
}


int
nmd_lookup(name, req, datap)
	char *name;
	int req;
	int *datap;
{
	struct nmdcls *np;
	struct nmdind *ep = 0;

	if (np = nc_find(name)) {
		if (req >= 0) {
			for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link)
				if (ep->ni_ind == req)
					break;
		} else
			ep = np->nc_ent->ni_link;

		if (ep == np->nc_ent)
			ep = 0;
	}

	if (ep) {
		req = ep->ni_ind;
		*datap = ep->ni_data;

	} else
		req = -1;

	return req;
}


int
nmd_dump(np)
	struct nmdcls *np;
{
	struct nmdind *ep;

	sprintf(pvmtxt, "<%s>\n", np->nc_name);
	pvmlogerror(pvmtxt);
	for (ep = np->nc_ent->ni_link; ep != np->nc_ent; ep = ep->ni_link) {
		sprintf(pvmtxt, "%4d: 0x%08x\n", ep->ni_ind, ep->ni_data);
		pvmlogerror(pvmtxt);
	}
	return 0;
}


int
nmd_dumpall()
{
	struct nmdcls *np;

	pvmlogerror("nmd_dumpall():\n");
	for (np = classes->nc_link; np != classes; np = np->nc_link)
		nmd_dump(np);
	return 0;
}


