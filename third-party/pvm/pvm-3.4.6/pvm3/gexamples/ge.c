
static char rcsid[] =
	"$Id: ge.c,v 1.2 1997/07/09 13:27:56 pvmsrc Exp $";

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
	Typical group example

	20 Mar 1993, modified example to reflect change in pvm_bcast()
*/

#include <stdio.h>
#include "pvm3.h"

int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, mygid, ctid[32];
	int nproc, i, indx;
	int cc;

	mytid = pvm_mytid();
	fprintf(stderr, "%s 0x%x enrolled\n", "ge", mytid);
	if (mytid < 0) {
		pvm_perror(argv[0]);
		return -1;
		}

	if (pvm_parent() == PvmNoParent) {
		if (argc != 2) goto usage;
		if ((nproc = atoi(argv[1])) < 0) goto usage;
		if (nproc > 32) goto usage;
		}

	/* join a group */
	mygid = pvm_joingroup("ge");
	fprintf(stderr, "%s 0x%x %d\n", "ge", mytid, mygid);

	/* if I'm the first to join then start the others */
	if (mygid == 0) {
		/* start a bunch of children */
    	if ((nproc-1) != pvm_spawn(argv[0], (char**) 0, 0, "", nproc-1, ctid))
			pvm_perror("spawn1");
		/* check them */
		for (i = 0; i < nproc-1; i++)
			fprintf(stderr, "0x%x\n", ctid[i]);
		/* tell them how many sibs */
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&nproc, 1, 1);
		pvm_mcast(ctid, nproc-1, 15);
		}
	else {
		/* find out the number of sibs */
		pvm_recv(pvm_parent(), 15);
		pvm_upkint(&nproc, 1, 1);
		fprintf(stderr,"nproc %d\n",nproc);
		}

	/* sync on a barrier */
	if (pvm_barrier("ge", nproc) < 0) {
		pvm_perror("barrier");
		pvm_lvgroup("ge");
		pvm_exit();
		return -1;
		}

	fprintf(stderr, "group %s size %d gid %d: sync\n", "ge", 
		pvm_gsize("ge"), mygid);

	/* everyone broadcast their gids and tids */
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&mygid, 1, 1);
	pvm_pkint(&mytid, 1, 1);
	pvm_bcast("ge", 63);

	/* recv all the gids and tids (except from myself) */
	for (i = 0; i < nproc-1; i++) {
		pvm_recv(-1, 63);
		pvm_upkint(&indx, 1, 1);
		pvm_upkint(ctid+indx, 1, 1);
		}
	/* set my tid too */
	ctid[mygid] = mytid;

	/* check to make sure the gids and tids are correct */
	for (i = 0; i < nproc; i++) {
		if (i != pvm_getinst("ge", ctid[i])) {
			fprintf(stderr, "gid %d doesn't match up!\n", i);
			}
		if (ctid[i] != pvm_gettid("ge", i)) {
			fprintf(stderr, "gid %d doesn't match up!\n", i);
			}
		}

	/* leave the group */
	if(pvm_lvgroup("ge") < 0)
			pvm_perror("leave");
	pvm_exit();
	printf("done\n");
	return 0;
usage:
	pvm_exit();
	fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
	return -1;
}
