
static char rcsid[] =
	"$Id: gexamp.c,v 1.2 1997/07/09 13:27:57 pvmsrc Exp $";

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
*/

#include <stdio.h>
#ifdef	SYSVSTR
#include <string.h>
#define RINDEX(s,c) strrchr(s,c)
#else
#include <strings.h>
#define RINDEX(s,c) rindex(s,c)
#endif
#include "pvm3.h"

int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, mygid, ctid[32];
	int nproc, i, indx;
	char *gp;

	mytid = pvm_mytid();
	fprintf(stderr, "%s 0x%x enrolled\n", argv[0], mytid);
	if (mytid < 0) {
		pvm_perror(argv[0]);
		return -1;
		}

	if (argc != 2) goto usage;
	if ((nproc = atoi(argv[1])) < 1) goto usage;
	if (nproc > 32) goto usage;

	/* join a group */
	/* the group name is the last path of the path name */
	gp = RINDEX(argv[0],'/');
	if (gp == 0) 
		gp = argv[0];
	else
		gp++;
	mygid = pvm_joingroup(gp);

	fprintf(stderr, "%s 0x%x %d\n", gp, mytid, mygid);
	if (mygid < 0) {
		pvm_exit();
		return -1;
	}

	/* if I'm the first to join then start the others */
	if (mygid == 0) {
		/* start a bunch of children */
    	pvm_spawn(gp, argv+1, PvmTaskDefault, "", nproc-1, ctid);
		/* check them */
		for (i = 0; i < nproc-1; i++)
			fprintf(stderr, "0x%x\n", ctid[i]);
		}

	/* sync on a barrier */
	if (pvm_barrier(gp, nproc) < 0) {
		pvm_perror(argv[0]);
		pvm_lvgroup(argv[0]);
		pvm_exit();
		return -1;
		}

	fprintf(stderr, "%s %d: sync\n", argv[0], mygid);

	/* everyone broadcast their gids and tids */
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&mygid, 1, 1);
	pvm_pkint(&mytid, 1, 1);
	pvm_bcast(gp, 63);

	/* recv all the gids and tids (except from myself) */
	ctid[mygid] = mytid;
	for (i = 0; i < nproc-1; i++) {
		pvm_recv(-1, 63);
		pvm_upkint(&indx, 1, 1);
		pvm_upkint(ctid+indx, 1, 1);
		}

	/* check to make sure the gids and tids are correct */
	for (i = 0; i < nproc; i++) {
		if (i != pvm_getinst(gp, ctid[i])) {
			fprintf(stderr, "gid %d doesn't match up!\n", i);
			}
		if (ctid[i] != pvm_gettid(gp, i)) {
			fprintf(stderr, "gid %d doesn't match up!\n", i);
			}
		}

	/* leave the group */
        pvm_barrier(gp,nproc);
	if(pvm_lvgroup(gp) < 0)
			pvm_perror(gp);

	pvm_exit();
	return 0;
usage:
	fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
	return -1;
}
